// ARcBridgeDesignShell.cpp: implementation of the CARcBridgeDesignShell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define LANE_OFFSET_STT 0.600
//#define LANE_WIDTH_STD  3.000

CARcBridgeDesignShell::CARcBridgeDesignShell()
{
	m_bInitialMesh = FALSE;
}

CARcBridgeDesignShell::~CARcBridgeDesignShell()
{

}

void CARcBridgeDesignShell::MakeMidasData(BOOL bRemoveAllData)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;

	CFEMManage *pFEM = &pBri->m_FemShell;
	if(bRemoveAllData)
		pFEM->RemoveAllData();
	
	// 토압계산할때 2DFrame의 높이를 사용한다.
	if(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetSize() == 0)
	{
		m_pStd->m_pFrame->MakeXyCoordinates(FALSE);
	}

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	pFEM->m_eDecimal = bLsd? CFEMManage::eDec6 : CFEMManage::eDec3;

	m_pStd->CalculateCofficient();
	m_pStd->m_pLoad->CalculateAll();
	m_pStd->m_pMeshCalc->CalculateAll();

	if(bLsd)
	{
		pFEM->SYSTEM_VAR.m_szUnitType = "N";
		pFEM->SYSTEM_VAR.m_szLengthType = "MM";
	}
	else
	{
		pFEM->SYSTEM_VAR.m_szUnitType = pBri->m_pARcBridgeDataStd->IsSI() ? "KN" : "TONF";
	}
	
	MakeMidasJoint(pFEM);
	MakeMidasMaterial(pFEM);
	MakeMidasShellSection(pFEM);
	MakeMidasFrameSection(pFEM);
	MakeMidasShellElement(pFEM);	
	MakeMidasFrameElement(pFEM);

	if(bLsd)
 		MakeMidasLoad_Lsd(pFEM);
	else
		MakeMidasLoad(pFEM);

	MakeMidasCombine(pFEM);
	MakeMidasConstraints(pFEM);
}

void CARcBridgeDesignShell::MakeSapData(BOOL bStt, BOOL bLeft, BOOL bRemoveAllData)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CFEMManage *pFEM = pBri->GetFemWingWall(bStt, bLeft);
	if(bRemoveAllData)
	{
		pFEM->RemoveAllData();
	}
	pFEM->m_eDecimal = bLsd? CFEMManage::eDec6 : CFEMManage::eDec3;

	m_pStd->CalculateCofficient();
	m_pStd->m_pLoad->CalculateAll();

	MakeMeshWingWall(bStt, bLeft);

	pFEM->SYSTEM_VAR.m_UX = pFEM->SYSTEM_VAR.m_UY = pFEM->SYSTEM_VAR.m_UZ = 1;
	pFEM->SYSTEM_VAR.m_RX = pFEM->SYSTEM_VAR.m_RY = pFEM->SYSTEM_VAR.m_RZ = 1;

	if(bLsd)
	{
		pFEM->SYSTEM_VAR.m_szUnitType = "N";
		pFEM->SYSTEM_VAR.m_szLengthType = "MM";
	}
	else
	{
		pFEM->SYSTEM_VAR.m_szUnitType = pBri->m_pARcBridgeDataStd->IsSI() ? "KN" : "TONF";
	}

	MakeSapJoint(pFEM, bStt, bLeft);
	MakeSapMaterial(pFEM, bStt, bLeft);
	MakeSapShellSection(pFEM, bStt, bLeft);
	MakeSapShellElement(pFEM, bStt, bLeft);
	MakeSapLoad(pFEM, bStt, bLeft);
	MakeSapCombine(pFEM, bStt, bLeft);
	MakeSapConstraints(pFEM, bStt, bLeft);
}


void CARcBridgeDesignShell::MakeSapJoint(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	long nIdxJoint = 0;
	long nSizeJoint = m_vArrWing.GetSize();

	for(long ix = 0; ix < nSizeJoint; ix++)
	{
		CVector vPoint = m_vArrWing.GetAt(ix);

		pFEM->AddJoint((USHORT)nIdxJoint++, vPoint);
	}

	long nSize = m_lArrFix.GetSize();
	for(long ix = 0; ix < nSize; ix ++)
	{
		long nIndex = m_lArrFix.GetAt(ix);
		CJoint *pJoint = pFEM->GetJoint((USHORT)nIndex);

		pJoint->m_UX = pJoint->m_UY = pJoint->m_UZ = pJoint->m_RX = pJoint->m_RY = pJoint->m_RZ = 1;
	}
}

void CARcBridgeDesignShell::MakeSapMaterial(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double Ec = pBri->m_pARcBridgeDataStd->m_Ec*10; // kgf/cm2 -> tonf/m2
	double Wc = toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete);
	double gV = pBri->GetGravity();
	double dT = 0;
	double dU = bLsd? pData->m_RatePoasong : 0.15;
	double dA = 0.00001;

	gV = GetValueUnitChange(gV,UNIT_CGS_M,pData->m_szM);
	Ec = GetValueUnitChange(Ec,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	Wc = GetValueUnitChange(Wc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);

	if(bLsd)
	{
		Ec = pBri->GetValueEc(bStt? ePartWingWallStt : ePartWingWallEnd, TRUE);
	}
	
	pFEM->AddMaterial(_T("CONC"),Ec,Wc,_T('C'),Wc/gV, dT, dU, dA);
}

void CARcBridgeDesignShell::MakeSapShellSection(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing==NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	CString szMaterial = _T("CONC");
	double dThick1 = bLsd? pWing->m_dT1 : toM(pWing->m_dT1);
	double dThick2 = bLsd? pWing->m_dT2 : toM(pWing->m_dT2);

	pFEM->AddShellSection(_T("THICK1"), _T("SHELL,THICK"), szMaterial, dThick1, dThick1);

	if(dThick1 != dThick2 && (dThick2 > 0))
	{
		pFEM->AddShellSection(_T("THICK2"), _T("SHELL,THICK"), szMaterial, dThick2, dThick2);
	}
}

void CARcBridgeDesignShell::MakeSapShellElement(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing==NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	CString szSection  = _T("");
	double dThick1 = bLsd? pWing->m_dT1 : toM(pWing->m_dT1);
	double dThick2 = bLsd? pWing->m_dT2 : toM(pWing->m_dT2);

	BOOL bByunDan = (dThick1 != dThick2 && (dThick2 > 0)) ? TRUE : FALSE;

	long nIdx = 0;
	long nSize = m_EleArray.GetSize();
	CVector v1,v2,v3,v4,Vcen;
	for(long nEle = 0; nEle < nSize; nEle++)
	{
		CIntFour Ele = m_EleArray.GetAt(nEle);
		szSection = _T("THICK1");

		v1 = pFEM->GetJoint((USHORT)(Ele.a-1))->m_vPoint;
		v2 = pFEM->GetJoint((USHORT)(Ele.b-1))->m_vPoint;
		v3 = pFEM->GetJoint((USHORT)(Ele.c-1))->m_vPoint;
		v4 = pFEM->GetJoint((USHORT)(Ele.d-1))->m_vPoint;
		if(bByunDan)
		{
			if(pWing->m_nDirHunch == 1 && (v1.z <= m_xyThick.y && v2.z <= m_xyThick.y && v3.z <= m_xyThick.y && v4.z <= m_xyThick.y))
			{
				szSection = _T("THICK2");
			}
			else if(pWing->m_nDirHunch == 2)
			{
				if(bStt && (v1.x >= m_xyThick.x && v2.x >= m_xyThick.x && v3.x >= m_xyThick.x && v4.x >= m_xyThick.x))
				{
					szSection = _T("THICK2");
				}
				if(!bStt && (v1.x <= m_xyThick.x && v2.x <= m_xyThick.x && v3.x <= m_xyThick.x && v4.x <= m_xyThick.x))
				{
					szSection = _T("THICK2");
				}
			}
		}

		Vcen.x = (v1.x + v2.x + v3.x + v4.x) / 4;
		Vcen.z = (v1.z + v2.z + v3.z + v4.z) / 4;

		CShell *pShell = pFEM->AddShell((USHORT)(nIdx++), (USHORT)(Ele.a-1), (USHORT)(Ele.b-1), (USHORT)(Ele.c-1), (USHORT)(Ele.d-1),szSection, _T("CONC"));
		pShell->m_LongAttrib[LONGSHELL_0_AREA_POS] = WINGSHELL_AREA_D; // 0~4 , A,B,C,D,A'
		if(pWing->m_dL2 > 0)
		{
			double dHeightBC = toM(MAX(m_xyWingFootStt.y, m_xyWingFootEnd.y) + fabs(m_xyWingFootStt.x - m_xyWingFootEnd.x));
			double dDistAD = bStt ? toM(MIN(m_xyWingUpperStt.x, m_xyWingUpperEnd.x)) + toM(pWing->m_dL1) : toM(MAX(m_xyWingUpperStt.x, m_xyWingUpperEnd.x)) - toM(pWing->m_dL1);
			if(bLsd)
			{
				dHeightBC = frM(dHeightBC);
				dDistAD = frM(dDistAD);
			}

			if( (bStt && Vcen.x > dDistAD) || (!bStt && Vcen.x < dDistAD) )
			{
				if(Vcen.z < dHeightBC)
				{
					pShell->m_LongAttrib[LONGSHELL_0_AREA_POS] = WINGSHELL_AREA_B;
				}
				else
				{
					pShell->m_LongAttrib[LONGSHELL_0_AREA_POS] = WINGSHELL_AREA_A;
					if(pWing->m_nDirHunch == 1 && (szSection == _T("THICK2")))
					{
						pShell->m_LongAttrib[LONGSHELL_0_AREA_POS] = WINGSHELL_AREA_A_;
					}
				}
			}
			else
			{
				BOOL bStop = TRUE;
			}
		}
	}
}

void CARcBridgeDesignShell::MakeSapLoad(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing==NULL) return;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
//	double dLoadValue = 0;
	CDoubleArray dArrJoint, dArrLoadLive[2], dArrLoadToap;
	long nIndexWing = (bStt? 0 : 2) + (bLeft? 0 : 1);
	m_pStd->m_pWing[nIndexWing]->CalculateAll(bStt, bLeft);

	double dLiveAlways[2];
	double Ko = m_pStd->m_dCoefficientKo;
	dLiveAlways[0] = pData->IsRailDesign() ? 1.50+3.50 : pBri->m_dLiveLoadAlways; // 철도교 궤도중량,열차하중
	dLiveAlways[0] = GetValueUnitChange(dLiveAlways[0], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	dLiveAlways[1] = m_pStd->m_pWing[nIndexWing]->GetLoadAlways_LS();

	double dWeightBackFill = pBri->m_dUnitWeigthBackFill;
	double dK0 = m_pStd->m_dCoefficientKo;

//	double dHeightFromUpper = 0;
	CDPoint xy(0,0), vY(0,1), xyMatch(0,0);
	CDPoint xyWingStt = toM(m_xyWingUpperStt);
	CDPoint xyWingEnd = toM(m_xyWingUpperEnd);
	long nSize = pFEM->GetJointSize();
	for(long ix = 0; ix < nSize; ix++)
	{
		CJoint *pJoint = pFEM->GetJoint((unsigned short)ix);
		xy = CDPoint(pJoint->m_vPoint.x, pJoint->m_vPoint.z);
		if(bLsd)
		{
			xy = toM(xy);
		}
		double dHeightFromUpper = 0;
		if(GetXyMatchSegAndLine(xyWingStt, xyWingEnd, xy, vY, xyMatch))
		{
			dHeightFromUpper = xyMatch.y - xy.y + toM(pWing->m_dHeightSoil);
		}
		dArrJoint.Add(ix);
		// (ARCBRIDGE-2328) 벽체 토압과 맞추지 않고 관용법 적용
//		double dLoadValue = m_pStd->m_pLoad->GetLoadSoilPressure(dHeightFromUpper, bStt, FALSE, FALSE);
		double dLoadValue = dK0 * dWeightBackFill * dHeightFromUpper;
		dLoadValue = GetValueUnitChange(dLoadValue, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		dArrLoadToap.Add(dLoadValue);
		dArrLoadLive[0].Add(Ko * dLiveAlways[0]);
		dArrLoadLive[1].Add(Ko * dLiveAlways[1]);
	}

	pFEM->AddPattern(_T("PATTTOAP"), dArrJoint, dArrLoadToap);
	if(pBri->m_nSelectWingLoad_LS_ES == 1 || pBri->m_nSelectWingLoad_LS_ES == 2 || !bLsd)
	{
		pFEM->AddPattern(_T("PATTLIVE"), dArrJoint, dArrLoadLive[0]);
	}
	if((pBri->m_nSelectWingLoad_LS_ES == 0 || pBri->m_nSelectWingLoad_LS_ES == 2) && bLsd)
	{
		pFEM->AddPattern(_T("PATTLI2"), dArrJoint, dArrLoadLive[1]);
	}

	//////////////////////////////////////////////////////////////////////////
	long nIndexLoad = 0;
	pFEM->MakeElementLoadForce(nIndexLoad+1);
	CString strLoad = _T("DEAD");
	pFEM->m_szLCName.SetAt(nIndexLoad, strLoad);
	pFEM->m_szLCComment.SetAt(nIndexLoad, hggettext("날개벽자중"));
	pFEM->m_dSelfWeight.SetAt(nIndexLoad,1.0);	
	nIndexLoad++;

	//
	for(long nLive=0; nLive<2; ++nLive)
	{
		if(nLive == 0 && pBri->m_nSelectWingLoad_LS_ES == 0 && bLsd) continue;
		if(nLive == 1 && (pBri->m_nSelectWingLoad_LS_ES == 1 || !bLsd)) continue;

		pFEM->MakeShellLoadForce(nIndexLoad+1);
		strLoad = (nLive == 0)? _T("LIVE") : _T("LIVE2");
		pFEM->m_szLCName.SetAt(nIndexLoad, strLoad);

		CString szComment(_T("상재하중"));
		if(bLsd)
		{
			szComment += (nLive == 0)? _T("ES") : _T("LS");
		}

		pFEM->m_szLCComment.SetAt(nIndexLoad, szComment);

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);

			CLoadForceShell *pELoad = pShell->GetLoadForce(nIndexLoad);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce((nLive == 0 || !bLsd)? _T("PATTLIVE") : _T("PATTLI2"),1,CLoadForcePress::HYDRO);
		}	
		nIndexLoad++;
	}

	//
	pFEM->MakeShellLoadForce(nIndexLoad+1);
	strLoad = _T("TOAP");
	pFEM->m_szLCName.SetAt(nIndexLoad, strLoad);
	pFEM->m_szLCComment.SetAt(nIndexLoad, hggettext("수평토압"));

	POSITION pos = pFEM->m_ShellList.GetHeadPosition();
	while(pos)		
	{
		CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);

		CLoadForceShell *pELoad = pShell->GetLoadForce(nIndexLoad);
		if(pELoad == NULL) continue;
		pELoad->m_szLoadTitle = strLoad;
		pELoad->SetLoadForce(_T("PATTTOAP"), 1 ,CLoadForcePress::HYDRO);
	}		
}

void CARcBridgeDesignShell::MakeSapCombine(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	double dDeadFactor[WING_CASE_COUNT] = { 0, 0, 0 };
	double dLiveFactor[WING_CASE_COUNT] = { 0, 0, 0 };
	double dToapFactor[WING_CASE_COUNT] = { 0, 0, 0 };
	double dLiveFactorLS[WING_CASE_COUNT] = { 0, 0, 0 };

	if(bLsd)
	{
		CLsdManagerBridge *pLsdManager = pBri->m_plsdManager;
		for(long i = 0; i < WING_CASE_COUNT; i++)
		{
			double dEta(0);
			CString szCombo(_T(""));
			CLsdLoadCase *pLoadCase = pBri->m_plsdManager->GetLsdLoadCaseWingWallOrAps(dEta, szCombo, i != WING_FACTOR_ULT, i == WING_FACTOR_USE_JUDGE, FALSE, bStt);
			if(!pLoadCase) continue;

			dDeadFactor[i] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_DC1);
			dToapFactor[i] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_EH);
			dLiveFactor[i] = max(pLoadCase->GetLoadFactor(eLSD_LOADCASE_ES), pLoadCase->GetLoadFactor(eLSD_LOADCASE_ES2));
			dLiveFactorLS[i] = max(pLoadCase->GetLoadFactor(eLSD_LOADCASE_LS), pLoadCase->GetLoadFactor(eLSD_LOADCASE_LS2));
		}
	}
	else
	{
		dDeadFactor[WING_FACTOR_USE] = 1.0;
		dLiveFactor[WING_FACTOR_USE] = 1.0;
		dToapFactor[WING_FACTOR_USE] = 1.0;
		dDeadFactor[WING_FACTOR_ULT] = 1.0;
		dLiveFactor[WING_FACTOR_ULT] = pBri->m_dFactorLive;
		dToapFactor[WING_FACTOR_ULT] = pBri->m_dFactorToap;
	}

	CString szCombo(_T(""));
	for(long i = 0; i < WING_CASE_COUNT; i++)
	{
		if(!bLsd && i>WING_FACTOR_USE)
			break;

		szCombo.Format("COMB%d", i+1);

		pFEM->AddCombo(szCombo,_T("DEAD"), 0, 0, dDeadFactor[i], FALSE);
		pFEM->AddCombo(szCombo,_T("TOAP"), 0, 0, dToapFactor[i], FALSE);
		if(bLsd)
		{
			if(pBri->m_nSelectWingLoad_LS_ES == 1 || pBri->m_nSelectWingLoad_LS_ES == 2)
				pFEM->AddCombo(szCombo,_T("LIVE"), 0, 0, dLiveFactor[i], FALSE);
			if(pBri->m_nSelectWingLoad_LS_ES == 0 || pBri->m_nSelectWingLoad_LS_ES == 2)
				pFEM->AddCombo(szCombo,_T("LIVE2"), 0, 0, dLiveFactorLS[i], FALSE);
		}
		else
		{
			pFEM->AddCombo(szCombo,_T("LIVE"), 0, 0, dLiveFactor[1], FALSE);
		}
	}
}

void CARcBridgeDesignShell::MakeSapConstraints(CFEMManage *pFEM, BOOL bStt, BOOL bLeft)
{

}

BOOL CARcBridgeDesignShell::IsPileJoint(CDPoint xyJoint)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(pBri->IsBoxType() == FALSE)
		return FALSE;

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	CDPoint xyPile(0,0), xyPileNear(0, 0);

	// 파일위치를 돌리면서 같은 위치의 파일이 있는지 찾는방식
	// 곡선에 사각이 있는 경우 파일위치와 부재 절점 위치에 오차가 생겨서 파일 위치를 못찾는 경우가 발생한다.
	/*for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;
		for(long kk = 0; kk < pOrg->countSide; kk++)
		{
			xyPile = toM(pBri->GetXyPilePos(pFooting, ix, kk));
			if(ComparePt(xyJoint, xyPile, _T("="), toM(MIN_LEN_SHELL * 5)))
			{
				return TRUE;
			}
		}
	}*/

	// 전체 절점과 비교해서 말뚝 위치와 가장 가까운 절점이라면 TRUE
	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
	// 먼저 확인을 위한 위치에서 가장 가까운 말뚝 위치를 찾고 
	double dDistMin(0);
	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;
		for(long kk = 0; kk < pOrg->countSide; kk++)
		{
			xyPile = toM(pBri->GetXyPilePos(pFooting, ix, kk));
			double dDist = ~(xyPile - xyJoint);

			if((ix == 0 && kk == 0) || dDistMin > dDist)
			{
				dDistMin = dDist;
				xyPileNear = xyPile;
			}
		}
	}

	// 찾은 말뚝 위치에 현재 절점이 가장 가까운 절점이라면 TRUE 그렇지 않으면 FALSE
	BOOL bPilePos = TRUE;
	long nSize = pMeshCalc->m_vArrSlabvPoint_real.GetSize();
	for(long ix=0; ix<nSize; ++ix)
	{
		CDPoint xyPos = toM(pMeshCalc->m_vArrSlabvPoint_real.GetAt(ix));
		double dDist = ~(xyPileNear - xyPos);
		if(Compare(dDistMin, dDist, _T(">"), 0.001))
		{
			bPilePos = FALSE;
			break;
		}
	}

	return bPilePos;
}

void CARcBridgeDesignShell::MakeMidasJoint(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	long nQtySpan = pBri->m_nQtyJigan;

	CVector vPoint(0,0,0);
	long nIdxJoint = 0;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bArch = pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;

	long nJ = 0, nShoe = 0, nV = 0, i = 0;
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		long nSizePoint = pMeshCalc->m_vArrSlab[iUD].GetSize();
		for(i=0; i<nSizePoint; i++)
		{
			vPoint = pMeshCalc->m_vArrSlab[iUD].GetAt(i);
			BOOL bShoePos = pMeshCalc->IsShoePoint(nIdxJoint,nJ,nShoe,nV);
			CJoint *pJnt = pFEM->AddJoint((USHORT)(nIdxJoint++),vPoint);
			if(bShoePos)
			{
				long nDir = pBri->GetJijum(nJ)->m_nDirShoe[nV][nShoe];
				if(nDir==0) pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = 1;
				if(nDir==1) pJnt->m_UY = pJnt->m_UZ = 1;
				if(nDir==2) pJnt->m_UX = pJnt->m_UZ = 1;
				if(nDir==3) pJnt->m_UZ = 1;
				//
				pJnt->m_LongAttrib[LONGSHELL_1_JIJUMNO]  = nJ;
				pJnt->m_LongAttrib[LONGSHELL_2_SHOE_POS] = nShoe;
			}
			if(iUD == iUPPER) continue;

			// 하부슬래브에 대한 SPring
			if(pBri->IsPileBase(-1))
			{
				if(pBri->IsExtLine())
				{
					// 확폭일때는 Node위치와 말뚝위치가 잘 맞지 않는다.
					// Node위치가 말뚝위치인지 검사하지 않고 거꾸로 말뚝위치에서 가까운 절점을 찾는다.
				}
				else
				{
					CDPoint xyJoint = pMeshCalc->m_vArrSlabvPoint_real.GetAt(i);
					if(IsPileJoint(toM(xyJoint)))	// IsPileJoint() 함수는 m단위로 비교 한다.
					{
						pJnt->m_vSpring.x  = 1000; // Temp
						pJnt->m_vSpring.y  = 1000;
						pJnt->m_vSpring.z  = 1000;
						pJnt->m_vSpringR.x = 1000;
						pJnt->m_vSpringR.y = 1000;
						pJnt->m_vSpringR.z = 1000;
					}
				}
			}
			else
			{
				pJnt->m_vSpring.x  = 1000;
				pJnt->m_vSpring.y  = 1000;
				pJnt->m_vSpring.z  = 1000;
				pJnt->m_vSpringR.x = 1000;
				pJnt->m_vSpringR.y = 1000;
				pJnt->m_vSpringR.z = 1000;
			}
		}

		if(iUD == iLOWER && pBri->IsExtLine())
		{
			// 말뚝위치 절점 
			SetSpringPilePosForExtLine(pFEM, nSizePoint);
		}
	}

	if(bArch)
	{
		long nSizePoint = pMeshCalc->m_vArrArchRib.GetSize();
		long i=0; for(i=0; i<nSizePoint; i++)
		{
			vPoint = pMeshCalc->m_vArrArchRib.GetAt(i);

			CJoint *pJnt = pFEM->AddJoint((USHORT)(nIdxJoint++),vPoint);
			if(pMeshCalc->IsArchRibFixPoint(i))
			{
				pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
			}
		}
	}
	long nJijum=0; for(nJijum=0; nJijum<=nQtySpan; nJijum++)
	{
		long nSizePoint = pMeshCalc->m_vArrWall[nJijum].GetSize();
		long i=0; for(i=0; i<nSizePoint; i++)
		{
			vPoint = pMeshCalc->m_vArrWall[nJijum].GetAt(i);
			CJoint *pJnt = pFEM->AddJoint((USHORT)nIdxJoint,vPoint);
			if(pMeshCalc->IsWallFixPoint(nJijum,i))
			{
				pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
			}
			nIdxJoint++;
		}
	}

	pFEM->RecalcExeRect();
}

void CARcBridgeDesignShell::MakeMidasMaterial(CFEMManage *pFEM)
{
	m_pStd->m_pFrame->MakeSapDataMaterial(pFEM, FALSE);
}

void CARcBridgeDesignShell::MakeMidasShellSection(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	// LSD에서 구조물강도를 분리하면서 구조물위치별로 따로 적용되는지 판단해야 한다.
	BOOL bUseLowerSlab = m_pStd->m_pFrame->IsUseMaterialLowerSlab() || (pBri->IsBoxType() && Compare(pBri->m_dTS, pBri->m_dTS_Lower, _T("="), 0.1) == FALSE);

	CString szSection  = "SLAB";
	CString szMaterial = "CONC";

	double dThick = toM(pBri->m_dTS);
	double dWidth = UNIT_METER;

	if(bLsd)
	{
		dThick = frM(dThick);
		szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartUpperSlab, TRUE));
	}
	if(bUseLowerSlab)
	{
		szSection = _T("USLAB");
	}

	pFEM->AddShellSection(szSection,"SHELL,THICK",szMaterial,dThick,dWidth);

	if(bUseLowerSlab)
	{
		szSection = _T("LSLAB");
		dThick = toM(pBri->m_dTS_Lower);
		if(bLsd)
		{
			dThick = frM(dThick);
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartLowerSlab, TRUE));
		}

		pFEM->AddShellSection(szSection,"SHELL,THICK",szMaterial,dThick,dWidth);

		// 부상방지저판 두께가 하부슬래브 두께와 다르다면 Section 추가해준다.
		if(pBri->m_bExistProtectionRiseFooting && pBri->m_dPRF_W > 0 && !Compare(pBri->m_dTS_Lower, pBri->m_dPRF_H1 + pBri->m_dPRF_H2, _T("="), 1.0))
		{
			dThick = frM(pBri->m_dPRF_H1 + pBri->m_dPRF_H2);
			szSection = _T("LSLAB_PF");
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartLowerSlab, TRUE));
		}
	}

	long nJijum=0; for(nJijum=0; nJijum<pBri->GetCountJijum(); nJijum++)
	{
		long nTypeColumn = pBri->GetTypeColumnByJijum(nJijum);
		if(nTypeColumn!=-1) continue;
		if(pBri->IsWall(nJijum)==FALSE) continue;

		dThick = toM(pBri->GetThickWall(nJijum));
		szSection.Format("WALL%d",nJijum+1);

		if(bLsd)
		{
			dThick = frM(dThick);
		}

		if(nJijum == 0)
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallStt, TRUE));
		else if(nJijum == pBri->GetCountJijum() -1)
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallEnd, TRUE));
		else
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartInWall, TRUE));

		pFEM->AddShellSection(szSection,"SHELL,THICK",szMaterial,dThick,dWidth);
	}

	// ARCHRIB 단면
	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	if(bArch)
	{
		CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
		long nSize =  pMeshCalc->m_dArrThick_Archrib.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			dThick = pMeshCalc->m_dArrThick_Archrib.GetAt(n);
			szSection.Format("ARCH%d",n+1);
			pFEM->AddShellSection(szSection,"SHELL,THICK",szMaterial,dThick,dWidth);
		}
	}
}

void CARcBridgeDesignShell::MakeMidasShellElement(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLine = pBri->GetLineBase();

	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	// LSD에서 구조물강도를 분리하면서 구조물위치별로 따로 적용되는지 판단해야 한다.
	BOOL bUseLowerSlab = m_pStd->m_pFrame->IsUseMaterialLowerSlab() || (pBri->IsBoxType() && Compare(pBri->m_dTS, pBri->m_dTS_Lower, _T("="), 0.1) == FALSE);

	CString szSection  = "SLAB";
	CString szMaterial(_T("CONC"));

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	long nQtyHDan = pMeshCalc->GetCountHDanPos() - 1;
	long nCountJijum = pBri->GetCountJijum();
	
	long nIdxJoint[4];
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(bUseLowerSlab)
			szSection  = (iUD == iUPPER)? _T("USLAB") : _T("LSLAB");
		if(bLsd)
		{
			szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc((iUD == iUPPER)? ePartUpperSlab : ePartLowerSlab, TRUE));
		}

		long nQtyElement = pMeshCalc->m_pListMeshSlab[iUD].GetSize();
		if(nQtyElement==0) break;
		long i=0; for(i=0; i<nQtyElement; i++)
		{
			CMeshFace *pFace = pMeshCalc->m_pListMeshSlab[iUD].GetAt(i);
			nIdxJoint[0] = pFace->m_nIndexJoint[1];
			nIdxJoint[1] = pFace->m_nIndexJoint[0];
			nIdxJoint[2] = pFace->m_nIndexJoint[2];
			nIdxJoint[3] = pFace->m_nIndexJoint[3];
			CShell *pShell = pFEM->AddShell((USHORT)pFace->m_nIndexFace, (USHORT)nIdxJoint[0], (USHORT)nIdxJoint[1], (USHORT)nIdxJoint[2], (USHORT)nIdxJoint[3],szSection, szMaterial);
			pShell->m_nAttrib = (iUD == iUPPER) ? SHELL_ELEMENT_UPPERSLAB : SHELL_ELEMENT_LOWERSLAB;
			pShell->m_LongAttrib[LONGSHELL_0_HDAN_NODE] = pFace->m_nIndexHDan;
			pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] = pFace->m_nSpanSlab; // 지간번호
			pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT] = pFace->m_bSpanCheckStt;
			pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END] = pFace->m_bSpanCheckEnd;
		}
	}

	// 벽체
	long nJijum=0; for(nJijum=0; nJijum<nCountJijum; nJijum++)
	{
		long nQtyElement = pMeshCalc->m_pListMeshWall[nJijum].GetSize();
		if(nJijum>pBri->m_nQtyJigan) break;
		if(nQtyElement==0) continue;

		// 29428 : 가각 고려
		long nQtyHDanWall = nQtyHDan;
		if(pBri->m_bIsExp)
		{
			if(nJijum == 0)
			{
				nQtyHDanWall += pMeshCalc->m_dArrHDanPosGagak[0][0].GetSize();
				nQtyHDanWall += pMeshCalc->m_dArrHDanPosGagak[0][1].GetSize();
			}
			else if(nJijum == pBri->m_nQtyJigan)
			{
				nQtyHDanWall += pMeshCalc->m_dArrHDanPosGagak[1][0].GetSize();
				nQtyHDanWall += pMeshCalc->m_dArrHDanPosGagak[1][1].GetSize();
			}
		}

		if(bLsd)
		{
			if(nJijum == 0)
				szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallStt, TRUE));
			else if(nJijum == nCountJijum -1)
				szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallEnd, TRUE));
			else
				szMaterial = m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartInWall, TRUE));
		}

		long nMidLayer = (long)RoundUp((double)(nQtyElement)/(double)(nQtyHDanWall)/2,0);
		long i=0; for(i=0; i<nQtyElement; i++)
		{
			CMeshFace *pFace = pMeshCalc->m_pListMeshWall[nJijum].GetAt(i);
			if(nJijum==pBri->m_nQtyJigan)
			{
				nIdxJoint[0] = pFace->m_nIndexJoint[3];
				nIdxJoint[1] = pFace->m_nIndexJoint[0];
				nIdxJoint[2] = pFace->m_nIndexJoint[1];
				nIdxJoint[3] = pFace->m_nIndexJoint[2];
			}
			else
			{
				nIdxJoint[0] = pFace->m_nIndexJoint[0];
				nIdxJoint[1] = pFace->m_nIndexJoint[3];
				nIdxJoint[2] = pFace->m_nIndexJoint[2];
				nIdxJoint[3] = pFace->m_nIndexJoint[1];
			}

			szSection.Format("WALL%d",nJijum+1);
			CShell *pShell = pFEM->AddShell((USHORT)pFace->m_nIndexFace, (USHORT)nIdxJoint[0], (USHORT)nIdxJoint[1], (USHORT)nIdxJoint[2], (USHORT)nIdxJoint[3],szSection, szMaterial);
			pShell->m_nAttrib = SHELL_ELEMENT_WALL;
			pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] = nJijum; // 지점번호
			if(i>=nQtyHDanWall && i<nQtyHDanWall*2)
				pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT] = TRUE; // 벽체상단 검토위치
			if(i>=(nQtyElement-nQtyHDanWall))
				pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END] = TRUE; // 벽체하단 검토위치
			if(i>=nQtyHDanWall*nMidLayer && i<nQtyHDanWall*(nMidLayer+1))
				pShell->m_BoolAttrib[BOOLSHELL_2_CHECKPOS_MID] = TRUE; // 벽체중앙 검토위치
		}
	}
	
	// 아치리브
	if(bArch)
	{
		for(long nArchRib=0; nArchRib<MAX_JIJUM_COUNT_RC; nArchRib++)
		{
			long nQtyElement = pMeshCalc->m_pListMeshArchRib[nArchRib].GetSize();
			if(nQtyElement==0) break;
			long i=0; for(i=0; i<nQtyElement; i++)
			{
				CMeshFace *pFace = pMeshCalc->m_pListMeshArchRib[nArchRib].GetAt(i);
				nIdxJoint[0] = pFace->m_nIndexJoint[1];
				nIdxJoint[1] = pFace->m_nIndexJoint[0];
				nIdxJoint[2] = pFace->m_nIndexJoint[2];
				nIdxJoint[3] = pFace->m_nIndexJoint[3];
				szSection.Format("ARCH%d", pFace->m_nIndexHDan+1);

				CShell *pShell = pFEM->AddShell((USHORT)pFace->m_nIndexFace, (USHORT)nIdxJoint[0], (USHORT)nIdxJoint[1], (USHORT)nIdxJoint[2], (USHORT)nIdxJoint[3],szSection, "CONC");
				pShell->m_nAttrib = SHELL_ELEMENT_ARCHRIB;
				pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] = pFace->m_nSpanSlab; // 지간번호
				pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT] = pFace->m_bSpanCheckStt;
				pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END] = pFace->m_bSpanCheckEnd;
			}
		}
	}
}

void CARcBridgeDesignShell::MakeMidasLoad(CFEMManage *pFEM)
{	
	CRcBridgeRebar		*pBri	   = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad	   = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData	   = pBri->m_pARcBridgeDataStd;

	long nQtySpan = pBri->m_nQtyJigan;
	//////////////////////////////////////////////////////////////////////////
	
	double dLoadValue  = 1;
	long nLoadCaseIndex = 0;

	long nLoadCase   = BRIDGE_LOAD_DEAD;
	CString strLoad  = pData->GetStringLoadCase(nLoadCase);
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		double dSelfWeight = 1.0;

		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "구체자중 및 사하중");
		pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	

		long nHDanNode = 0;
		double dWidthHDan = 0;
		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
			{			
				nHDanNode  = pShell->m_LongAttrib[LONGSHELL_0_HDAN_NODE];
				dWidthHDan = toM(pBri->GetLengthHDan(nHDanNode));
				dLoadValue = m_pStd->m_pLoad->GetWeightHDan(nHDanNode);

				CGuardWallRC *pGW = pBri->GetGuardWallByHDan(nHDanNode); // 추가하중이 있는 경우에 추가
				if(pGW && pGW->m_dForce>0 && !pGW->IsTypeHDanOnlyNone())
				{
					dLoadValue += pGW->m_dForce;
				}
				if(dLoadValue == 0) continue;

				dLoadValue = GetValueUnitChange(dLoadValue,UNIT_CGS_TONF,pData->m_szTonf)/dWidthHDan;
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
			}
		}		

		++nLoadCaseIndex;
	}
	// 연직토압
	nLoadCase   = BRIDGE_LOAD_SOIL;
	dLoadValue = GetValueUnitChange(pLoad->m_dSoilPressureVert,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && Compare(dLoadValue,0.0,">"))
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "연직토압");

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			if(Compare(dLoadValue,0.0,"=")) break;
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
			{			
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
			}
		}		
		++nLoadCaseIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	// 양토압
	nLoadCase   = BRIDGE_LOAD_TOAP_BOTH;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad   = pData->GetStringLoadCase(nLoadCase);		
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평토압(양토압)");

		double dHStt = pFEM->GetJoint(0)->m_vPoint.z;
		double dHEnd = dHStt + m_pStd->GetElDiffModelingEnd();
		// 시점측벽체
		double dToap1 = 0, dToap2=0, dToap3=0, dToap4=0;		
		long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
		{
			CShell *pS = pFEM->GetShell((USHORT)i);					
			if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==0)
			{			
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dToap1 = pLoad->GetLoadSoilPressure(dHStt - pJnt1->m_vPoint.z);
				dToap2 = pLoad->GetLoadSoilPressure(dHStt - pJnt2->m_vPoint.z);
				dToap3 = pLoad->GetLoadSoilPressure(dHStt - pJnt3->m_vPoint.z);
				dToap4 = pLoad->GetLoadSoilPressure(dHStt - pJnt4->m_vPoint.z);
				
				dToap1 = GetValueUnitChange(dToap1,UNIT_CGS_TONF,pData->m_szTonf);
				dToap2 = GetValueUnitChange(dToap2,UNIT_CGS_TONF,pData->m_szTonf);
				dToap3 = GetValueUnitChange(dToap3,UNIT_CGS_TONF,pData->m_szTonf);
				dToap4 = GetValueUnitChange(dToap4,UNIT_CGS_TONF,pData->m_szTonf);

				pELoad->m_szLoadTitle = strLoad;
				double dPress[5] = {0,-dToap1,-dToap2,-dToap3,-dToap4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
			}
			// 종점측벽체
			else if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==nQtySpan)
			{			
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dToap1 = pLoad->GetLoadSoilPressure(dHEnd - pJnt1->m_vPoint.z);
				dToap2 = pLoad->GetLoadSoilPressure(dHEnd - pJnt2->m_vPoint.z);
				dToap3 = pLoad->GetLoadSoilPressure(dHEnd - pJnt3->m_vPoint.z);
				dToap4 = pLoad->GetLoadSoilPressure(dHEnd - pJnt4->m_vPoint.z);
				
				dToap1 = GetValueUnitChange(dToap1,UNIT_CGS_TONF,pData->m_szTonf);
				dToap2 = GetValueUnitChange(dToap2,UNIT_CGS_TONF,pData->m_szTonf);
				dToap3 = GetValueUnitChange(dToap3,UNIT_CGS_TONF,pData->m_szTonf);
				dToap4 = GetValueUnitChange(dToap4,UNIT_CGS_TONF,pData->m_szTonf);

				pELoad->m_szLoadTitle = strLoad;
				double dPress[5] = {0,-dToap1,-dToap2,-dToap3,-dToap4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
			}
		}
		++nLoadCaseIndex;
	}

	// 편토압
	nLoadCase = BRIDGE_LOAD_TOAP_ONE;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		// 시점측벽체
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
		strLoad   = pData->GetStringLoadCase(nLoadCase) + "L";
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평토압(시점벽체)");

		double dHStt = pFEM->GetJoint(0)->m_vPoint.z;
		double dHEnd = dHStt + m_pStd->GetElDiffModelingEnd();
		double dToap1 = 0, dToap2=0, dToap3=0, dToap4=0;		
		long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
		{
			CShell *pS = pFEM->GetShell((USHORT)i);					
			if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==0)
			{			
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dToap1 = pLoad->GetLoadSoilPressure(dHStt - pJnt1->m_vPoint.z);
				dToap2 = pLoad->GetLoadSoilPressure(dHStt - pJnt2->m_vPoint.z);
				dToap3 = pLoad->GetLoadSoilPressure(dHStt - pJnt3->m_vPoint.z);
				dToap4 = pLoad->GetLoadSoilPressure(dHStt - pJnt4->m_vPoint.z);
				
				dToap1 = GetValueUnitChange(dToap1,UNIT_CGS_TONF,pData->m_szTonf);
				dToap2 = GetValueUnitChange(dToap2,UNIT_CGS_TONF,pData->m_szTonf);
				dToap3 = GetValueUnitChange(dToap3,UNIT_CGS_TONF,pData->m_szTonf);
				dToap4 = GetValueUnitChange(dToap4,UNIT_CGS_TONF,pData->m_szTonf);

				pELoad->m_szLoadTitle = strLoad;
				double dPress[5] = {0,-dToap1,-dToap2,-dToap3,-dToap4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
			}
		}
		++nLoadCaseIndex;
		// 종점측벽체
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
		strLoad   = pData->GetStringLoadCase(nLoadCase) + "R";
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평토압(종점벽체)");

		for(i=0; i<pFEM->GetShellSize(); i++)
		{
			CShell *pS = pFEM->GetShell((USHORT)i);					
			if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==nQtySpan)
			{			
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dToap1 = pLoad->GetLoadSoilPressure(dHEnd - pJnt1->m_vPoint.z);
				dToap2 = pLoad->GetLoadSoilPressure(dHEnd - pJnt2->m_vPoint.z);
				dToap3 = pLoad->GetLoadSoilPressure(dHEnd - pJnt3->m_vPoint.z);
				dToap4 = pLoad->GetLoadSoilPressure(dHEnd - pJnt4->m_vPoint.z);
				
				dToap1 = GetValueUnitChange(dToap1,UNIT_CGS_TONF,pData->m_szTonf);
				dToap2 = GetValueUnitChange(dToap2,UNIT_CGS_TONF,pData->m_szTonf);
				dToap3 = GetValueUnitChange(dToap3,UNIT_CGS_TONF,pData->m_szTonf);
				dToap4 = GetValueUnitChange(dToap4,UNIT_CGS_TONF,pData->m_szTonf);

				pELoad->m_szLoadTitle = strLoad;
				double dPress[5] = {0,-dToap1,-dToap2,-dToap3,-dToap4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
			}
		}
		++nLoadCaseIndex;
	}

	MakeSapDataLoadWaterPress_Usd(pFEM, nLoadCaseIndex);

	//////////////////////////////////////////////////////////////////////////
	// 상하부 온도차
	nLoadCase   = BRIDGE_LOAD_TEMP1;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "온도하중(+)");

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoad;
			if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
			{	
				if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==1) pELoad->SetLoadForceTempOrShrink(pLoad->m_dLoadTempertureAxial, CLoadForceTemperatures::TEMP);
				if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==2) pELoad->SetLoadForce(pLoad->m_dLoadTempertureUpDown);
			}
			else if(pBri->m_bSelectTempLoadAddWall)
			{
				pELoad->SetLoadForceTempOrShrink(pLoad->m_dLoadTempertureAxial, CLoadForceTemperatures::TEMP);
			}
		}		
		++nLoadCaseIndex;
	}

	// 온도변화
	nLoadCase   = BRIDGE_LOAD_TEMP2;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "온도하중(-)");

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoad;
			if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
			{	
				if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==1) pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadTempertureAxial, CLoadForceTemperatures::TEMP);
				if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==2) pELoad->SetLoadForce(-pLoad->m_dLoadTempertureUpDown);
			}
			else if(pBri->m_bSelectTempLoadAddWall)
			{
				pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadTempertureAxial, CLoadForceTemperatures::TEMP);
			}
		}		
		++nLoadCaseIndex;
	}

	// 건조수축
	nLoadCase   = BRIDGE_LOAD_SHRINK;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "건조수축");

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pBri->m_bSelectShrinkLoadAddWall)
			{
				// 전체 재하
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadDryAndShrinkage, CLoadForceTemperatures::TEMP);			
			}
			else
			{
				// Slab에만 재하
				if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
				{			
					CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;
					pELoad->m_szLoadTitle = strLoad;
					pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadDryAndShrinkage, CLoadForceTemperatures::TEMP);			
				}
			}
		}		
		++nLoadCaseIndex;
	}

	// 지점침하
	MakeSapDataLoadSubsid(pFEM, nLoadCaseIndex);

	//////////////////////////////////////////////////////////////////////////
	nLoadCase  = BRIDGE_LOAD_LR;
	dLoadValue = GetValueUnitChange(pLoad->m_dRail_LoadLR,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && Compare(dLoadValue,0.0,">"))
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad   = pData->GetStringLoadCase(nLoadCase);		
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "장대레일 종하중");
		
		++nLoadCaseIndex;		
	}

	//////////////////////////////////////////////////////////////////////////
	nLoadCase  = BRIDGE_LOAD_SB;
	dLoadValue = GetValueUnitChange(pLoad->m_dRail_LoadSB,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && Compare(dLoadValue,0.0,">"))
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad   = pData->GetStringLoadCase(nLoadCase);		
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "시동 및 제동하중");
		
		++nLoadCaseIndex;		
	}

	//////////////////////////////////////////////////////////////////////////
	nLoadCase = BRIDGE_LOAD_LIVE_FULL;
	if(pBri->m_bApplyLoad[nLoadCase] || pBri->m_bApplyLoad[BRIDGE_LOAD_TANK])
	{
		// 보도활하중
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex);

		// 활하중 //////////////////////////////////////////////////////////////////////////
		MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex);
	}
}

void CARcBridgeDesignShell::MakeMidasCombine(CFEMManage *pFEM)
{
	if(m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign())
	{
		m_pStd->m_pFrame->MakeSapDataCombineNormal_Lsd(pFEM,TRUE,TRUE);
		m_pStd->m_pFrame->MakeSapDataCombineEnv_Lsd(pFEM, FALSE, FALSE);
		m_pStd->m_pFrame->MakeSapDataCombineEtaCase_Lsd(pFEM, FALSE, FALSE);
	}
	else
	{
		m_pStd->m_pFrame->MakeSapDataCombineNormal(pFEM,TRUE,TRUE);
	}
}

void CARcBridgeDesignShell::MakeMidasFrameElement(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeMeshCalc  *pMeshCalc = m_pStd->m_pMeshCalc;

	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return;	

	long nINode = 0, nJNode = 0;
	long nIdxElement = pFEM->GetShellSize();
	long nJijum=0; for(nJijum=1; nJijum<pBri->m_nQtyJigan; nJijum++)
	{
		long nTypeColumn = pBri->GetTypeColumnByJijum(nJijum);
		if(nTypeColumn==-1) continue;

		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		CDPoint vAng = pBri->GetAngleJijum(nJijum);

		for(long nCol=0; nCol<pWall->m_nCountColumn; nCol++)
		{
			if(nCol>=pMeshCalc->m_dArrPointJijum[iUPPER][nJijum].GetSize()) break;
			nINode = (long)pMeshCalc->m_dArrPointJijum[iUPPER][nJijum].GetAt(nCol);
			if(pBri->IsBoxType())
			{
				nJNode = (long)pMeshCalc->m_dArrPointJijum[iLOWER][nJijum].GetAt(nCol);
			}
			else
			{
				nJNode = pMeshCalc->m_vArrSlab[iUPPER].GetSize();
				long i=0; for(i=0; i<nJijum; i++)
				{
					nJNode += pMeshCalc->m_vArrWall[i].GetSize();
				}
				nJNode += (long)pMeshCalc->m_dArrPointWallFix[nJijum].GetAt(nCol);
			}

			CElement *pEle = pFEM->AddElement((USHORT)nIdxElement++, (USHORT)nINode, (USHORT)nJNode);
			pEle->m_nFlag	= ELEMENT_MIDWALL; 		
			pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO]	= nJijum;
			pEle->m_LongAttrib[LONG_2_ELEMENT_COLUMN_INDEX] = nCol;
			pEle->m_szFrameSectionName.Format("COLU%02d",nJijum);
			pEle->m_dAng = 90 - vAng.GetAngleDegree();
		}
	}
}

// 기둥부재만
//#define COL_TYPE_CIRCLE		0
//#define COL_TYPE_RECTANGLE	1
//#define COL_TYPE_TRACK		2
//#define COL_TYPE_POLY			3
void CARcBridgeDesignShell::MakeMidasFrameSection(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return;

	CString szMaterial = bLsd? m_pStd->m_pFrame->GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartInWall, TRUE)) : "CONC";
	CString szSection = _T("");
	long nJijum=0; for(nJijum=1; nJijum<pBri->m_nQtyJigan; nJijum++)
	{
		long nTypeColumn = pBri->GetTypeColumnByJijum(nJijum);
		if(nTypeColumn==-1) continue;

		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		szSection.Format("COLU%02d",nJijum);

		if(bLsd)
		{
			if(nTypeColumn==COL_TYPE_CIRCLE)
				pFEM->AddFrameSectionOfCiclcuralColumn(szSection,szMaterial,pWall->m_Column.m_dB[iOUTTER][iUPPER],1.00);
			else if(nTypeColumn==COL_TYPE_RECTANGLE)
				pFEM->AddFrameSectionOfRectangleColumn(szSection,szMaterial,pWall->m_Column.m_dH[iOUTTER][iUPPER],pWall->m_Column.m_dB[iOUTTER][iUPPER],1.00);
			else
			{
				CColumnApp *pCol  = &pWall->m_Column;
				double Area = frM2(pCol->GetColumnArea());
				double I33  = frM4(pCol->GetColumnI33_I22(TRUE));
				double I22  = frM4(pCol->GetColumnI33_I22(FALSE));
				double J    = 0.000;
				pFEM->AddFrameSection(szSection,szMaterial,Area,I33,I22,J);
			}
		}
		else
		{
			if(nTypeColumn==COL_TYPE_CIRCLE)
				pFEM->AddFrameSectionOfCiclcuralColumn(szSection,szMaterial,toM(pWall->m_Column.m_dB[iOUTTER][iUPPER]),1.00);
			else if(nTypeColumn==COL_TYPE_RECTANGLE)
				pFEM->AddFrameSectionOfRectangleColumn(szSection,szMaterial,toM(pWall->m_Column.m_dH[iOUTTER][iUPPER]),toM(pWall->m_Column.m_dB[iOUTTER][iUPPER]),1.00);
			else
			{
				CColumnApp *pCol  = &pWall->m_Column;
				double Area = pCol->GetColumnArea();
				double I33  = pCol->GetColumnI33_I22(TRUE);
				double I22  = pCol->GetColumnI33_I22(FALSE);
				double J    = 0.000;
				pFEM->AddFrameSection(szSection,szMaterial,Area,I33,I22,J);
			}
		}
	}
}

void CARcBridgeDesignShell::MakeMidasConstraints(CFEMManage *pFEM)
{
	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;

	long nSizeConstSlab = pMeshCalc->m_dArrConstraintJointIdx_Slab.GetSize();
	long nSizeConstArch = pMeshCalc->m_dArrConstraintJointIdx_Archrib.GetSize();
	CString strName = _T("");
	CString strType = _T("EQUAL");
// 	long nIdxNode1 = 0;
// 	long nIdxNode2 = 0;

	if(nSizeConstArch == nSizeConstSlab)
	{
		long n=0; for(n=0; n<nSizeConstSlab; n++)
		{
			long nIdxNode1 = (long)(pMeshCalc->m_dArrConstraintJointIdx_Slab.GetAt(n) + 1);
			long nIdxNode2 = (long)(pMeshCalc->m_dArrConstraintJointIdx_Archrib.GetAt(n) + 1);
			strName.Format("EQUAL%d", n);
			pFEM->AddConstraints(strName, strType, 1, 0, 1, 0, 1, 0, nIdxNode1, nIdxNode2);
		}
	}
}

void CARcBridgeDesignShell::MakeMeshWingWall(BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing==NULL) return;

	m_vArrWing.RemoveAll();
	m_lArrFix.RemoveAll();
	m_EleArray.RemoveAll();

	/////---Cm2 Mesh
	surf_remesh_q4::registration ("Licensed to HANGIL_IT.", "9CE164F59049"); // WIN 32
//	surf_remesh_q4::registration ("Licensed to HANGIL_IT.", "DFCF6E2CC3A3"); // WIN 64
	surf_remesh_q4::mesher	the_remesher;
	surf_remesh_q4::mesher::data_type data;

	//////////////////////////////////////////////////////////////////////////
	CIntThreeArray EleArray;
	CDPointArray xyArrInit;
	CRebarPlacing rp;
	CTwinVector tv;
	CDPoint xy(0,0);
	double dDistThick = 0;
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CTwinVectorArray tvArrWing, tvArrFix, tvArrSK;
	CDPointArray xyArrNodeThick, xyArrNodeEnd;
	long nSize = 0;
	for(long nA = 0; nA < 2; nA++)
	{
		if(nA > 0 && pWing->m_nDirHunch == 0) continue;

		CDoubleArray dArr;
		CDPoint xyBracketUpperEnd = pBri->GetTvArrayWingWallInitialMesh(tvArrWing, tvArrFix, tvArrSK, m_xyWingUpperStt, m_xyWingUpperEnd, m_xyWingFootStt, m_xyWingFootEnd, dDistThick, nA, bStt, bLeft);

		BOOL bInnerPoint = (nA == 1 && pWing->m_nDirHunch == 2) ? TRUE : FALSE;
		CTwinVectorArray tvArrInit;
		if(bInnerPoint == FALSE)
		{
			rp.SeperatorTwinVectorArea(tvArrWing, tvArrInit, dArr);
		}

		// 좌표추가
		nSize = bInnerPoint ? tvArrWing.GetSize() : tvArrInit.GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			tv = bInnerPoint ? tvArrWing.GetAt(ix) : tvArrInit.GetAt(ix);
			xy = tv.m_v1;
			if(xyArrInit.GetIndex(xy) == -1)
			{
				if(nA == 1 && Compare(xy.x, dDistThick, _T("=")) == FALSE)
				{
					xyArrNodeEnd.Add(xy);
				}
				xyArrInit.Add(xy);
			}
			xy = tv.m_v2;
			if(xyArrInit.GetIndex(xy) == -1)
			{
				if(nA == 1 && Compare(xy.x, dDistThick, _T("=")) == FALSE)
				{
					xyArrNodeEnd.Add(xy);
				}
				xyArrInit.Add(xy);
			}
		}
		// 가로방향 변단면인 경우 노리점을 추가
		CDPoint xyNoriEnd(0,0);
		if(nA == 0 && pWing->m_nDirHunch == 2)
		{
			long se = bStt ? -1 : 1;
			xyNoriEnd = m_xyWingUpperEnd - pWing->m_dHL * CDPoint(0,1);
			xyNoriEnd.x += -pWing->m_dL1 * se;
			xyNoriEnd.y += -pWing->m_dL1 / pWing->m_dSW;
			if(Compare(pWing->m_dL1, pWing->m_dLT + (pWing->m_dT2 - pWing->m_dT1) * pWing->m_dSH, _T("=")) && pWing->m_dL2 > 0)
			{
				xyArrInit.Add(xyNoriEnd);
				xyArrNodeThick.Add(xyNoriEnd);
			}
		}
		// 부재추가
		if(bInnerPoint)
		{
			CIntThree ele;
			CDPoint xyInner = CDPoint((dDistThick + m_xyWingUpperEnd.x) / 2., m_xyWingUpperEnd.y - pWing->m_dHL / 2.);
			long nIndexInner = xyArrInit.GetSize();
			xyArrInit.Add(xyInner);
			xyArrNodeEnd.Sort(FALSE, FALSE);
			long nSize = xyArrNodeEnd.GetSize();
			for(long ix = 0; ix < nSize; ix++)
			{
				xyArrNodeThick.Add(xyArrNodeEnd.GetAt(ix));
			}
			nSize = xyArrNodeThick.GetSize();
			for(long ix = 0; ix < nSize; ix++)
			{
				ele.a = xyArrInit.GetIndex(xyArrNodeThick.GetAt(ix));
				ele.b = ix == nSize - 1 ? xyArrInit.GetIndex(xyArrNodeThick.GetAt(0)) : xyArrInit.GetIndex(xyArrNodeThick.GetAt(ix + 1));
				ele.c = nIndexInner;
				EleArray.Add(ele);
			}
		}
		else
		{
			nSize = dArr.GetSize();
			long nIdxStt = 0;
			for(long ix = 0; ix < nSize; ix++)
			{
				long nPoly = (long)dArr.GetAt(ix);
				CTwinVector tv1 = tvArrInit.GetAt(nIdxStt);
				CTwinVector tv2 = tvArrInit.GetAt(nIdxStt + 2);
				if(nPoly == 3)
				{
					tv2 = tvArrInit.GetAt(nIdxStt + 1);
				}
				tv1.Sort(TRUE, FALSE);
				tv2.Sort(TRUE, FALSE);
				nIdxStt += nPoly;

				MakeInitialMesh(tv1, tv2, xyArrInit, xyBracketUpperEnd, xyNoriEnd, EleArray, nPoly == 3, bStt, bLeft);
			}
		}

		if(nA == 0 && pWing->m_nDirHunch == 2)
		{
			// 좌표들중에 두께거리에 해당하는 절점을 저장
			long nSize = xyArrInit.GetSize();
			for(long ix = 0; ix < nSize; ix++)
			{
				xy = xyArrInit.GetAt(ix);
				if(Compare(xy.x, dDistThick, _T("=")) && xy.y > xyNoriEnd.y)
				{
					xyArrNodeThick.Add(xy);
				}
			}
			xyArrNodeThick.Sort(FALSE, TRUE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//1. 좌표
	CVector vPos(0,0,0);	
	nSize = xyArrInit.GetSize();
	data.pos.resize(3,nSize);
	for(long ix = 0; ix < nSize; ix++)
	{
		CDPoint xy = xyArrInit.GetAt(ix);
		data.pos(0, ix) = bLsd? xy.x : toM(xy.x);
		data.pos(1, ix) = 0;
		data.pos(2, ix) = bLsd? xy.y : toM(xy.y);
	}

	//2. 초기부재
	nSize = EleArray.GetSize();
	data.connectM.resize(3,nSize);
	for(long ix = 0; ix < nSize; ix++)
	{
		CIntThree Ele = EleArray.GetAt(ix);
		data.connectM(0, ix) = Ele.a;
		data.connectM(1, ix) = Ele.b;
		data.connectM(2, ix) = Ele.c;
	}

	// 3. Skleton Line
	m_xyThick = CDPoint(0, 0);
	if(pWing->m_nDirHunch != 0 && tvArrSK.GetSize() > 0)
	{
		long nSizeSK = tvArrSK.GetSize();
// 		long nSizeNode = 0;
		long nSizeSkleton = 0;

		for(long ix = 0; ix < nSizeSK; ix++)
		{
			long nIndexSk = 0;

			tv = tvArrSK.GetAt(ix);
			CDPoint xy1 = CDPoint(tv.m_v1.x, tv.m_v1.y);
			CDPoint xy2 = CDPoint(tv.m_v2.x, tv.m_v2.y);
			CDPointArray xyArrSK;
			long nSizeNode = xyArrInit.GetSize();
			for(long kk = 0; kk < nSizeNode; kk++)
			{
				xy = xyArrInit.GetAt(kk);
				if(IsOverlapPointWithLine(xy1, xy2, xy))
				{
					xyArrSK.Add(xy);
				}
			}
			xyArrSK.Sort(pWing->m_nDirHunch == 1);
			m_xyThick = bLsd? xyArrSK.GetAt(0) : toM(xyArrSK.GetAt(0));
			
			nSizeNode = xyArrSK.GetSize();
			nSizeSkleton += nSizeNode - 1;
			data.skeleton_edges_in.resize(2, nSizeSkleton);

			for(long kk = 0; kk < nSizeNode - 1; kk++)
			{
				xy = xyArrSK.GetAt(kk);
				data.skeleton_edges_in(0, nIndexSk) = xyArrInit.GetIndex(xy);
				xy = xyArrSK.GetAt(kk + 1);
				data.skeleton_edges_in(1, nIndexSk) = xyArrInit.GetIndex(xy);
				nIndexSk++;
			}
		}
	}

	// 고정점을 추가 : 경우에 따라 고정면의 모양이 오차가 발생
	// 그래도 오차가 발생하면 FIX 점에 오차를 주어야 할 것 같음.... (if(IsOverlapPointWithLine(xy1, xy2, CDPoint(vPos.x, vPos.z)))를 사용하지 않고)
	long nSizeFix = tvArrFix.GetSize();
	for(long ix = 0; ix < nSizeFix; ix++)
	{
		tv = tvArrFix.GetAt(ix);
		CDPoint xy1 = CDPoint(tv.m_v1.x, tv.m_v1.y);
		CDPoint xy2 = CDPoint(tv.m_v2.x, tv.m_v2.y);
		long nIdx1 = xyArrInit.GetIndex(xy1);
		long nIdx2 = xyArrInit.GetIndex(xy2);
		if(nIdx1 > 0 && nIdx2 > 0)
		{
			if(ix == 0)
			{
				data.hard_nodes.push_back(nIdx1);
			}
			data.hard_nodes.push_back(nIdx2);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// RUN
	double dMeshSize = pBri->m_dMeshSizeWing[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];
	the_remesher.mode.max_h = bLsd? dMeshSize : toM(dMeshSize);
	the_remesher.mode.min_h = 0.001;
	the_remesher.mode.all_quad_flag = TRUE;
	the_remesher.mode.initial_cleanup_flag = TRUE;

	if(m_bInitialMesh == FALSE)
	{
		the_remesher.run(data);
		data.error_code;
	}

	// 절점
	nSize = tvArrFix.GetSize();
	long nNodeSize = data.pos.cols();
	for(int ix = 0; ix < nNodeSize; ix++)
	{
		vPos.x = data.pos(0, ix);
		vPos.y = data.pos(1, ix);
		vPos.z = data.pos(2, ix);

		m_vArrWing.Add(vPos);
		for(long kk = 0; kk < nSize; kk++)
		{
			tv = tvArrFix.GetAt(kk);
			CDPoint xy1 = bLsd? CDPoint(tv.m_v1.x, tv.m_v1.y) : toM(CDPoint(tv.m_v1.x, tv.m_v1.y));
			CDPoint xy2 = bLsd? CDPoint(tv.m_v2.x, tv.m_v2.y) : toM(CDPoint(tv.m_v2.x, tv.m_v2.y));
			if(IsOverlapPointWithLine(xy1, xy2, CDPoint(vPos.x, vPos.z)))
			{
				m_lArrFix.Add(ix);
				break;
			}
		}
	}

	// 부재
	CIntFour Ele;
	long nEleSize = data.connectM.cols();
	for(int ix = 0; ix < nEleSize; ix++)
	{
		Ele.a = data.connectM(0, ix) + 1;
		Ele.b = data.connectM(1, ix) + 1;
		Ele.c = data.connectM(2, ix) + 1;
		Ele.d = data.connectM(3, ix) + 1;
		if(m_bInitialMesh == TRUE)
		{
			Ele.d = data.connectM(2, ix) + 1;
		}
		m_EleArray.Add(Ele); 
	}
}

void CARcBridgeDesignShell::MakeInitialMesh(CTwinVector tvLower, CTwinVector tvUpper, CDPointArray &xyArrInit, CDPoint xyBracket, CDPoint xyNoriEnd, CIntThreeArray &EleArray, BOOL bTriangle, BOOL bStt, BOOL bLeft)
{
	CIntThree ele;

	if(bTriangle)
	{
		CDPoint xyL1(tvLower.m_v1.x,tvLower.m_v1.y), xyL2(tvLower.m_v2.x,tvLower.m_v2.y), xyU1(tvUpper.m_v1.x,tvUpper.m_v1.y), xyU2(tvUpper.m_v2.x,tvUpper.m_v2.y);
		ele.a = xyArrInit.GetIndex(tvLower.m_v1);
		ele.b = xyArrInit.GetIndex(tvLower.m_v2);
		if(ComparePt(xyU1, xyL1 , _T("=")) == FALSE && ComparePt(xyU1, xyL2 , _T("=")) == FALSE)
		{
			ele.c = xyArrInit.GetIndex(tvUpper.m_v1);
		}
		else
		{
			ele.c = xyArrInit.GetIndex(tvUpper.m_v2);
		}
		EleArray.Add(ele);
		return;
	}

	// 두개로 쪼개서 생성
	if(xyNoriEnd.IsNull() == FALSE && tvLower.m_v1.y < xyNoriEnd.y && tvUpper.m_v1.y > xyNoriEnd.y)
	{
		for(long ix = 0; ix < 2; ix++)
		{
			CTwinVector tvU = tvUpper;
			CTwinVector tvL = tvLower;
			if(ix == 0)
			{
				tvU.m_v1.y = tvU.m_v2.y = xyNoriEnd.y;
				xyArrInit.Add(tvU.m_v1);
				xyArrInit.Add(tvU.m_v2);
			}
			else
			{
				tvL.m_v1.y = tvL.m_v2.y = xyNoriEnd.y;
			}
			ele.a = xyArrInit.GetIndex(tvL.m_v1);
			ele.b = xyArrInit.GetIndex(tvU.m_v1);
			ele.c = xyArrInit.GetIndex(tvU.m_v2);
			EleArray.Add(ele);
			ele.a = xyArrInit.GetIndex(tvL.m_v2);
			ele.b = xyArrInit.GetIndex(tvU.m_v2);
			ele.c = xyArrInit.GetIndex(tvL.m_v1);
			EleArray.Add(ele);
		}
		return;
	}

	if(Compare(tvLower.m_v1.y, tvUpper.m_v1.y, _T("=")) == FALSE)
	{
		ele.a = xyArrInit.GetIndex(tvLower.m_v1);
		ele.b = xyArrInit.GetIndex(tvUpper.m_v1);
		ele.c = xyArrInit.GetIndex(tvUpper.m_v2);
		EleArray.Add(ele);
	}

	if(Compare(tvLower.m_v2.y, tvUpper.m_v2.y, _T("=")) == FALSE)
	{
		ele.a = xyArrInit.GetIndex(tvLower.m_v2);
		ele.b = xyArrInit.GetIndex(tvUpper.m_v2);
		ele.c = xyArrInit.GetIndex(tvLower.m_v1);
		if(IsOverlapPointWithLine(tvLower.m_v1, tvLower.m_v2, xyBracket))
		{
			ele.a = xyArrInit.GetIndex(xyBracket);
			if(ele.a == -1)
			{
				ele.a = xyArrInit.Add(xyBracket);
			}

			EleArray.Add(ele);
			ele.c = xyArrInit.GetIndex(tvLower.m_v2);
			EleArray.Add(ele);
		}
		else
		{
			EleArray.Add(ele);
		}
	}
}

void CARcBridgeDesignShell::MakeMidasLoad_Lsd( CFEMManage *pFEM )
{
	CRcBridgeRebar		*pBri	   = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad	   = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData	   = pBri->m_pARcBridgeDataStd;
	
	long nLoadCaseIndex = 0;
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	long nQtySpan = pBri->m_nQtyJigan;

	// 고정하중 DC / DW
	// 연직토압 EV
	MakeSapDataLoadDead_Lsd(pFEM, nLoadCaseIndex);

	// 수평토압 EH
	MakeSapDataLoadHoriSoilPress_Lsd(pFEM, nLoadCaseIndex);

	// 수압(연직수압 EVW, 내부수압 WAI)
	MakeSapDataLoadWaterPress_Lsd(pFEM, nLoadCaseIndex);

	// 건조수축 SH
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SH))
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SH, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SH, TRUE);

		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pBri->m_bSelectShrinkLoadAddWall)
			{
				// 전체 재하
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadDryAndShrinkage, CLoadForceTemperatures::TEMP);			
			}
			else
			{
				// Slab에만 재하
				if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
				{			
					CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;
					pELoad->m_szLoadTitle = strLoadMark;
					pELoad->SetLoadForceTempOrShrink(-pLoad->m_dLoadDryAndShrinkage, CLoadForceTemperatures::TEMP);			
				}
			}
		}		
		++nLoadCaseIndex;
	}

	// 온도하중
	MakeSapDataLoadTemperature(pFEM, nLoadCaseIndex);

	// 지점침하
	MakeSapDataLoadSubsid(pFEM, nLoadCaseIndex);


	// 보도활하중
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_PL))
	{
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex);
	}
	
	// 활하중 //////////////////////////////////////////////////////////////////////////	
	MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex);

	// 상재활하중 LS
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LS))
	{
		for(long ixDir=0; ixDir<2; ++ixDir)
		{
			BOOL bLeft = (ixDir == iLEFT);
			ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_LS);
			eLoadCase = bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2;

			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);

			double dLoadValue = GetValueUnitChange(pLoad->m_dLoadLsApply[ixDir]	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
			if(dLoadValue == 0)
				continue;

			pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			double dToap1 = 0, dToap2=0, dToap3=0, dToap4=0;		
			long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
			{
				CShell *pS = pFEM->GetShell((USHORT)i);

				if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==(bLeft? 0 : nQtySpan))
				{
					CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;

					pELoad->m_szLoadTitle = strLoadMark;
					double dPress[5] = {0,-dLoadValue,-dLoadValue,-dLoadValue,-dLoadValue};
					pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);
				}
			}

			++nLoadCaseIndex;
		}
	}

	// 상재토하중 ES
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_ES))
	{
		for(long ixDir=0; ixDir<2; ++ixDir)
		{
			BOOL bLeft = (ixDir == iLEFT);
			ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_ES);

			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, TRUE);

			double dLoadValue = GetValueUnitChange(pLoad->GetLoadLiveAlwaysHori(), UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
			if(dLoadValue == 0)
				continue;

			pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			double dToap1 = 0, dToap2=0, dToap3=0, dToap4=0;		
			long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
			{
				CShell *pS = pFEM->GetShell((USHORT)i);

				if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==(bLeft? 0 : nQtySpan))
				{
					CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;

					pELoad->m_szLoadTitle = strLoadMark;
					double dPress[5] = {0,-dLoadValue,-dLoadValue,-dLoadValue,-dLoadValue};
					pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);
				}
			}

			++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadDead_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;

	double dLoadValue(0);
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	// 고정하중 DC / DW
	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix==0)? FALSE : TRUE;
		ELSDLoadSelectKind eLoadSelect = bDW? eLSD_LOADSELECT_DW : eLSD_LOADSELECT_DC;

		if(pBri->m_plsdManager->IsSelectedLoadDesign(eLoadSelect))
		{
			strLoadName = pBri->m_plsdManager->GetStringSelectLoadLsd(eLoadSelect, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringSelectLoadLsd(eLoadSelect, TRUE);

			pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
			if(bDW == FALSE)
			{
				double dSelfWeight = 1.0;
				pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	
			}

			long nHDanNode = 0;
			double dWidthHDan = 0;
			POSITION pos = pFEM->m_ShellList.GetHeadPosition();
			while(pos)		
			{
				CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
				if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
				{	
					nHDanNode  = pShell->m_LongAttrib[LONGSHELL_0_HDAN_NODE];
					dWidthHDan = pBri->GetLengthHDan(nHDanNode);

					CGuardWallRC *pGW = pBri->GetGuardWallByHDan(nHDanNode); // 추가하중이 있는 경우에 추가
					BOOL bSelDW = (pGW && pGW->m_bDWload)? TRUE : FALSE;

					if(bDW == (bSelDW || (pGW && pGW->IsPaveOnHDan())))
						dLoadValue = m_pStd->m_pLoad->GetWeightHDan(nHDanNode, bDW);
					else
						dLoadValue = 0;

					// 추가하중은 무조건 DW
					if(bDW && pGW && pGW->m_dForce>0 && !pGW->IsTypeHDanOnlyNone())
					{
						dLoadValue += pGW->m_dForce;
					}
					if(dLoadValue == 0) continue;

					dLoadValue = GetValueUnitChange(dLoadValue,UNIT_CGS_TONF_M,pData->m_szTonf_M)/dWidthHDan;
					CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;
					pELoad->m_szLoadTitle = strLoadMark;
					pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
				}
			}
			++nLoadCaseIndex;
		}
	}

	// 연직토압 EV
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV))
	{
		BOOL bUnderGroundWater = pBri->m_bApplyGroundWaterLevel;

		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !bUnderGroundWater) continue;

			BOOL bAdd = FALSE;
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, TRUE);

			dLoadValue  = GetValueUnitChange(pLoad->m_dSoilPressureVert	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
			if(dLoadValue > 0)
			{
				pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

				POSITION pos = pFEM->m_ShellList.GetHeadPosition();
				while(pos)		
				{
					if(Compare(dLoadValue,0.0,"=")) break;
					CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
					if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
					{			
						CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
						if(pELoad == NULL) continue;
						pELoad->m_szLoadTitle = strLoadMark;
						pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
					}
				}
				bAdd = TRUE;
			}

			if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
			{
				// 부상방지 저판에 작용하는 연직토압
				double dVertSoilStt = GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(TRUE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
				double dVertSoilEnd = GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(FALSE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
				if(bWater)
				{
					dVertSoilStt += GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(TRUE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
					dVertSoilEnd += GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(FALSE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
				}

				if(bAdd == FALSE)
				{
					pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

					pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
					pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
				}

				POSITION pos = pFEM->m_ShellList.GetHeadPosition();
				while(pos)		
				{
					CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
					if(pShell->m_nAttrib == SHELL_ELEMENT_LOWERSLAB && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] < 0)
					{
						BOOL bStt = pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] == -1;

						CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
						if(pELoad == NULL) continue;
						pELoad->m_szLoadTitle = strLoadMark;
						pELoad->SetLoadForce(CVector(0,0,0), 0, bStt? -dVertSoilStt: -dVertSoilEnd, "GZ", CLoadForcePress::PRES);
					}
				}
				bAdd = TRUE;
			}
			if(bAdd)
				++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadHoriSoilPress_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	//////////////////////////////////////////////////////////////////////////
	BOOL bOutWall = pBri->IsOutsideWall();
	BOOL bUnderGroundWater = pBri->m_bApplyGroundWaterLevel;
	long nQtySpan = pBri->m_nQtyJigan;

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	if(bOutWall == FALSE)
		return;
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EH) == FALSE)
		return;

	for(long ixW=0; ixW<2; ++ixW)
	{
		BOOL bWater = (ixW == 1)? TRUE : FALSE;
		if(bWater && !bUnderGroundWater) continue;

		for(long ixDir=0; ixDir<2; ++ixDir)
		{
			BOOL bLeft = (ixDir == iLEFT);
			ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_EH);

			double dHStt = pFEM->GetJoint(0)->m_vPoint.z;
			if(bLeft == FALSE)
			{
				dHStt += frM(m_pStd->GetElDiffModelingEnd());
			}
			
			if(bWater)
				eLoadCase = bLeft? eLSD_LOADCASE_EHW1 : eLSD_LOADCASE_EHW2;
			else
				eLoadCase = bLeft? eLSD_LOADCASE_EH : eLSD_LOADCASE_EH2;

			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);

			pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			double dToap1 = 0, dToap2=0, dToap3=0, dToap4=0;		
			long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
			{
				CShell *pS = pFEM->GetShell((USHORT)i);

				if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==(bLeft? 0 : nQtySpan))
				{
					CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;
					CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
					CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
					CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
					CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
					dToap1 = pLoad->GetLoadSoilPressure(toM(dHStt - pJnt1->m_vPoint.z), bLeft, bWater);
					dToap2 = pLoad->GetLoadSoilPressure(toM(dHStt - pJnt2->m_vPoint.z), bLeft, bWater);
					dToap3 = pLoad->GetLoadSoilPressure(toM(dHStt - pJnt3->m_vPoint.z), bLeft, bWater);
					dToap4 = pLoad->GetLoadSoilPressure(toM(dHStt - pJnt4->m_vPoint.z), bLeft, bWater);

					dToap1 = GetValueUnitChange(dToap1,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
					dToap2 = GetValueUnitChange(dToap2,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
					dToap3 = GetValueUnitChange(dToap3,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
					dToap4 = GetValueUnitChange(dToap4,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

					pELoad->m_szLoadTitle = strLoadMark;
					double dPress[5] = {0,-dToap1,-dToap2,-dToap3,-dToap4};
					pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);
				}
			}

			++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadTemperature( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TU) == FALSE)
		return;

	double dThick = pBri->m_dTS;
	double dRatioThick = 1.0 / dThick;

	for(long nLoad=0; nLoad<2; ++nLoad)
	{
		for(long ixPM=0; ixPM<2; ++ixPM)
		{
			BOOL bPlus = (ixPM == 0)? TRUE : FALSE;
			long nPM = bPlus? 1 : -1;

			ELSDLoadCaseKind eLoad = (nLoad == 0)? (bPlus? eLSD_LOADCASE_TU21 : eLSD_LOADCASE_TU22) : (bPlus? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM);
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoad, FALSE);
			strLoadMark = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoad, TRUE);

			pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			POSITION pos = pFEM->m_ShellList.GetHeadPosition();
			while(pos)		
			{
				CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoadMark;

				if(nLoad == 0)
				{
					if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
					{	
						pELoad->SetLoadForceTempOrShrink(pLoad->m_dLoadTempertureAxial * nPM, CLoadForceTemperatures::TEMP);
					}
					else if(pBri->m_bSelectTempLoadAddWall)
					{
						pELoad->SetLoadForceTempOrShrink(pLoad->m_dLoadTempertureAxial * nPM, CLoadForceTemperatures::TEMP);
					}
				}
				else
				{
					if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
					{
						// (ARCBRIDGE-2810) MIDAS에서는 두께를 자동보정하여 적용되므로 산출된 온도차값을 그대로 적용하면 됩니다.
						// pELoad->SetLoadForce(pLoad->m_dLoadTempertureUpDown * nPM * dRatioThick);
						pELoad->SetLoadForce(pLoad->m_dLoadTgValueT[ixPM]);
					}
				}
				
			}		
			++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadSubsid( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeMeshCalc  *pMeshCalc = m_pStd->m_pMeshCalc;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoad(FALSE);
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	if(bLsd)
	{
		bApplyLoad = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD);
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SD1, FALSE);
		strLoadMark  = _T("SD"); // pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SD1, TRUE);
	}
	else
	{
		bApplyLoad = pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID];
		strLoadName	= _T("지점침하");
		strLoadMark	= pData->GetStringLoadCase(BRIDGE_LOAD_SUBSID);
	}
	double dSubsidenceDisp = bLsd? frM(pLoad->m_dLoadSubsidence) : pLoad->m_dLoadSubsidence;

	//////////////////////////////////////////////////////////////////////////
	// (40943) Slab교일때도 안들어가는걸로 되있었는데 들어가는걸로.
	if(bApplyLoad && pBri->IsBoxType() == FALSE)
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		CString szName,szFixIdx;
		long nSubStt = -1;
		long nSubEnd = -1;
		long nIdxJointFixStt = 0;
		long nIdxJointFixEnd = 0;
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			// 시점측
			nIdxJointFixStt = pMeshCalc->GetJointJijumFixStt(0);
			nIdxJointFixEnd = pMeshCalc->GetJointJijumFixEnd(0);
			long nIdxJointFixStt2 = pMeshCalc->GetJointJijumFixStt(1);
			long nIdxJointFixEnd2 = pMeshCalc->GetJointJijumFixEnd(1);

			szName.Format("FOOT%d", 1);
			szFixIdx.Format("%dto%d %dto%d",nIdxJointFixStt,nIdxJointFixEnd,nIdxJointFixStt2,nIdxJointFixEnd2);
			pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);

			// 종점측
			nIdxJointFixStt = pMeshCalc->GetJointJijumFixStt(3);
			nIdxJointFixEnd = pMeshCalc->GetJointJijumFixEnd(3);
			nIdxJointFixStt2 = pMeshCalc->GetJointJijumFixStt(2);
			nIdxJointFixEnd2 = pMeshCalc->GetJointJijumFixEnd(2);

			szName.Format("FOOT%d", 2);
			szFixIdx.Format("%dto%d %dto%d",nIdxJointFixStt,nIdxJointFixEnd,nIdxJointFixStt2,nIdxJointFixEnd2);
			pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);

			nSubStt = 0;
			nSubEnd = 2;
		}
		else
		{
			long nQtyJigan = pBri->m_nQtyJigan;

			long nJijum=0; for(nJijum=0; nJijum<=nQtyJigan; nJijum++)
			{
				if(pMeshCalc->m_dArrPointWallFix[nJijum].GetSize()>0)
				{
					nIdxJointFixStt = pMeshCalc->GetJointJijumFixStt(nJijum);
					nIdxJointFixEnd = pMeshCalc->GetJointJijumFixEnd(nJijum);

					szName.Format("WALL%d",nJijum+1);
					szFixIdx.Format("%dto%d",nIdxJointFixStt,nIdxJointFixEnd);
					pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);
					if(nSubStt==-1) nSubStt = nJijum;
					nSubEnd = nJijum;
				}
				else if(pMeshCalc->m_dArrJointShoeoPos[0][nJijum].GetSize()>0)
				{
					long nSizeJoint = pMeshCalc->m_dArrJointShoeoPos[0][nJijum].GetSize();
					szFixIdx = _T("");
					CString szTemp = _T("");
					long n=0; for(n=0; n<nSizeJoint; n++)
					{
						nIdxJointFixStt = (long)pMeshCalc->m_dArrJointShoeoPos[0][nJijum].GetAt(n);
						szTemp.Format("%d ",nIdxJointFixStt+1);
						szFixIdx += szTemp;
					}
					szName.Format("SHOE%d",nJijum+1);
					pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);

					if(nSubStt==-1) nSubStt = nJijum;
					nSubEnd = nJijum;
				}
				else if(pMeshCalc->m_dArrJointShoeoPos[1][nJijum].GetSize()>0)
				{
					long nSizeJoint = pMeshCalc->m_dArrJointShoeoPos[1][nJijum].GetSize();
					szFixIdx = _T("");
					CString szTemp = _T("");
					long n=0; for(n=0; n<nSizeJoint; n++)
					{
						nIdxJointFixStt = (long)pMeshCalc->m_dArrJointShoeoPos[1][nJijum].GetAt(n);
						szTemp.Format("%d ",nIdxJointFixStt+1);
						szFixIdx += szTemp;
					}
					szName.Format("SHOE%d",nJijum+1);
					pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);

					if(nSubStt==-1) nSubStt = nJijum;
					nSubEnd = nJijum;
				}
			}
		}

		pFEM->SetSettleCase(strLoadMark,1,nSubEnd-nSubStt,1.00);

		++nLoadCaseIndex;
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadLiveFootWay( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pLoad->m_dLiveLoadFootway <=0) return;

	BOOL bLsd = pData->IsLsdDesign();
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	if(bLsd)
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, TRUE);	
	}
	else
	{
		strLoadName	= _T("보도활하중");
		strLoadMark	= _T("FW");
	}

	pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

	double dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoadFootway,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	long nHDanNode = 0;
	POSITION pos = pFEM->m_ShellList.GetHeadPosition();
	while(pos)		
	{
		CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
		if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
		{			
			nHDanNode  = pShell->m_LongAttrib[LONGSHELL_0_HDAN_NODE];
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDanNode);
			if(!pGW || !pGW->IsTypeHDanRoadFootway()) continue;
			
			CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
		}
	}		
	++nLoadCaseIndex;
}

void CARcBridgeDesignShell::MakeSapDataLoadLiveLoadAll( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoad(FALSE);
	BOOL bApplyLoadHeavy = pBri->IsApplyTankLoad();
	if(bLsd)
		bApplyLoad = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1);
	else
		bApplyLoad = pBri->m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL];

	if(bApplyLoad == FALSE && bApplyLoadHeavy == FALSE)
		return;

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;

	if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
	{
		MakeSapDataLoadLiveAlways(pFEM, nLoadCaseIndex);
	}
	else
	{
		CString szLane = _T("");

		long nIdxLane  = 0;
		long nSizeNode = pMeshCalc->m_dArrStation.GetSize(); // 교축방향으로 LANE에 걸리는 NODE수
		long nQtyLaneGroup = pMeshCalc->GetSizeLaneGroup();
		long nQtyLaneTotal = 0;

		pLoad->m_bExistLane = (nQtyLaneGroup>0)? TRUE : FALSE;

		BOOL bLeftStt = (pBri->m_nSelectStartLane == 0)? TRUE : FALSE;
		double dAngStt    = 90 - pBri->GetAngleJijum(0).GetAngleDegree();
		double dAngEnd    = 90 - pBri->GetAngleJijum(pBri->m_nQtyJigan).GetAngleDegree();
		// (ARCBRIDGE-2934) 확폭일때 확폭선형에 의한 각도 고려
		if(pBri->IsExtLine())
		{
			double dStaStt = pBri->m_dStationBridgeStt;
			double dStaEnd = dStaStt + pBri->m_dLengthBridge;
			CDPoint xyAngStt = pBri->GetAngleAzimuthJijum(0);
			CDPoint xyAngEnd = pBri->GetAngleAzimuthJijum(pBri->m_nQtyJigan);
			
			CLineInfo* pLineBase = pBri->GetLineBase();
			CLineInfo* pLineLeft = pBri->GetLineLeftRight(TRUE);

			CDPoint xyStt = pLineBase->GetXyLine(dStaStt);
			CDPoint xyEnd = pLineBase->GetXyLine(dStaEnd);

			double dStaLeftStt(0), dStaLeftEnd(0);
			pLineLeft->GetStationMatchByXyAndAngle(xyStt, xyAngStt, dStaLeftStt);
			pLineLeft->GetStationMatchByXyAndAngle(xyEnd, xyAngEnd, dStaLeftEnd);
			
			CDPoint xyAngLeftStt = pLineLeft->GetAngleAzimuth(dStaLeftStt);
			CDPoint xyAngLeftEnd = pLineLeft->GetAngleAzimuth(dStaLeftEnd);

			dAngStt = xyAngStt.BetweenAngle(xyAngLeftStt).GetAngleDegree() - 90;
			dAngEnd = 90 - xyAngEnd.BetweenAngle(xyAngLeftEnd).GetAngleDegree();
		}
		double iValue	  = 0; // 충격계수... 뒤에서 지정
		// 차선그룹이 시작되는 좌측의 점을 기준으로 거리를 입력
		if(nQtyLaneGroup > 0 && pBri->m_pARcBridgeDataStd->IsRailDesign())
		{
			long nGroup=0; for(nGroup=0; nGroup<nQtyLaneGroup; nGroup++)
			{
				long nIdxNodeLane= 0;
				long nIdxStt = 0;
				long nIdxEnd = 0;
				long nCountLane  = pMeshCalc->GetCountLaneForRailLoad(nGroup, nIdxStt, nIdxEnd);
				long nIdxSttLane = pMeshCalc->GetPosLaneGroupStt(nGroup);
				double dDistStt  = pMeshCalc->m_dArrHDanPos.GetAt(nIdxSttLane); // 0.600 [도설기]2.1.3
				double dWidthLane = toM(pMeshCalc->GetWidthLaneForRailLoad(nGroup));

				CDoubleArray dArrPoint;
				pMeshCalc->GetPointArrayLaneGroupStt(nGroup, dArrPoint);
				for(long nLane=0; nLane<nCountLane; nLane++)
				{
					szLane.Format("LANE%02d",1+nIdxLane++);
					CSurfLane *pSurfLane  = pFEM->AddSurfLane(szLane, (long)pLoad->m_dDesignRoadWidth,dAngStt,dAngEnd, CSurfLane::MOVE_BOTH);

					double dDistLane = toM(-(dDistStt) + pBri->GetLengthRailByLine(nIdxStt+nLane));
					if(bLeftStt == FALSE) dDistLane = dWidthLane - dDistLane;

					for(long nNode=0; nNode<nSizeNode; nNode++)
					{
						nIdxNodeLane = (long)dArrPoint.GetAt(nNode);
						pFEM->AddSurfLaneData(pSurfLane, nIdxNodeLane+1, dDistLane, iValue, nNode==0);
					}
				}
				nQtyLaneTotal += nCountLane;
			}

			double B = pLoad->m_dRail_WidthDistribute;
			double I = pLoad->m_dCoefficientImpact[0];
			double N = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? 1 : 2;			

			long nSizeWheel  = pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetSize();
			long i=0; for(i=0; i<nSizeWheel; i++)
			{
				long nType = (long)pBri->m_pARcBridgeDataStd->m_dArrTypeLoadTrainWheel.GetAt(i);
				double Load = pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetAt(i);
				Load = GetValueUnitChange(Load,UNIT_CGS_TONF,pData->m_szTonf);
				if(i==nSizeWheel-1)
				{
					if(nType==2)						
						pFEM->AddVehicleDB("LRAIL", 1, Round(Load*N*(1+I)/B,3), 0.0, 0.0);					
					else											
						pFEM->AddVehicleDB("LRAIL", 0, Round(Load*N*(1+I)/B,3), 0.0, 0.0);					
				}
				else
				{
					double D = pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetAt(i);
					if(nType==2)					
						pFEM->AddVehicleDB("LRAIL", 1, Round(Load*N*(1+I)/B,3), D, D);
					else					
						pFEM->AddVehicleDB("LRAIL", 0, Round(Load*N*(1+I)/B,3), D, D);																
				}				
			}
			//				pFEM->AddVehicleClass("VECL1", "LRAIL");
			double dRF[6] = {1,};
			/////////////////////////////////// 중요 !!! = Scale Factor 모르겠음 ! ///////////////////////
			pFEM->AddMovingLoad("MOVE1",1,dRF,"LRAIL","SLAB",1,1,1);			
		}
		else if(nQtyLaneGroup > 0)
		{
			BOOL bLeftStt = (pBri->m_nSelectStartLane == 0)? TRUE : FALSE;
			long nGroup=0; for(nGroup=0; nGroup<nQtyLaneGroup; nGroup++)
			{
				long nIdxNodeLane= 0;
				long nCountLane  = pMeshCalc->GetCountLane(nGroup);
				//long nIdxSttLane = pMeshCalc->GetPosLaneGroupStt(nGroup);
				double dWidthLane = toM(pMeshCalc->GetWidthLane(nGroup));
				//double dDistStt  = toM(pMeshCalc->m_dArrHDanPos.GetAt(nIdxSttLane)) + (pLoad->m_dDesignRoadWidth-pLoad->m_dDesignTruckWidth)/2; // 0.600 [도설기]2.1.3
				CDoubleArray dArrPoint;
				pMeshCalc->GetPointArrayLaneGroupStt(nGroup, dArrPoint);
				for(long nLane=0; nLane<nCountLane; nLane++)
				{
					szLane.Format("LANE%02d",1+nIdxLane++);
					CSurfLane *pSurfLane  = pFEM->AddSurfLane(szLane, (long)(bLsd? frM(pLoad->m_dDesignRoadWidth) : pLoad->m_dDesignRoadWidth),dAngStt,dAngEnd, CSurfLane::MOVE_BOTH);

					// dDistStt를 언제 부터 뺏는지 확인이 안됨..
					double dDistLane = nLane*pLoad->m_dDesignRoadWidth + pLoad->m_dDesignRoadWidth*0.5;
					if(bLeftStt == FALSE) dDistLane = dWidthLane - dDistLane;
					for(long nNode=0; nNode<nSizeNode; nNode++)
					{
						nIdxNodeLane = (long)dArrPoint.GetAt(nNode);
						pFEM->AddSurfLaneData(pSurfLane, nIdxNodeLane+1, bLsd? frM(dDistLane) : dDistLane, iValue, nNode==0);
					}
				}
				nQtyLaneTotal += nCountLane;
			}

			if(bApplyLoad)
			{
				// MIDAS에서 i를 자동 적용
				// MIDAS INPUT작성시에도 활하중에 직접 넣는다.
				//double I   = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? pLoad->m_dCoefficientImpact : 0;
				double I	= 0;
				double dPfDesign = m_pStd->GetWheelLoadFront();
				double dPrDesign = m_pStd->GetWheelLoadRear();
				double DtbDesign = m_pStd->GetWheelLoadDistribution();
				double dPmDesign = m_pStd->GetWheelLoadPm();
				double dPsDesign = m_pStd->GetWheelLoadPs();
				double dPm_Design = m_pStd->GetWheelLoadMiddle();

				double Pf  = GetValueUnitChange(dPfDesign,UNIT_CGS_TONF,pData->m_szTonf);
				double Pr  = GetValueUnitChange(dPrDesign,UNIT_CGS_TONF,pData->m_szTonf);
				double Dtb = GetValueUnitChange(DtbDesign,UNIT_CGS_TONF,pData->m_szTonf);
				double Pm  = GetValueUnitChange(dPmDesign,UNIT_CGS_TONF,pData->m_szTonf);
				double Ps  = GetValueUnitChange(dPsDesign,UNIT_CGS_TONF,pData->m_szTonf);
				//double Pmt = GetValueUnitChange(pLoad->m_dPmApplyTruck[0]/(1+I),UNIT_CGS_TONF,pData->m_szTonf);
				double Pmt = GetValueUnitChange(dPm_Design,UNIT_CGS_TONF,pData->m_szTonf);
				double wl = GetValueUnitChange(pLoad->m_dLaneLoad,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

				if(bLsd)
				{
					// (ARCBRIDGE-2759) 하중계산 에서 계산된 하중은 2D 기준으로 구해진 값이다.
					// 3D 모델에 적용할때는 두바퀴가 다는것으로 적용해서 X2를 해주고 표준차로 하중은 폭(3m)로 나눈 값이기 때문에 다시 폭을 곱해준다.

					double dDist1 = 3600;
					double dDist2 = 1200;
					double dDist3 = 7200;

					// 3D Plate는 무조건 Midas로...
					if(FALSE) // pBri->m_nSelect3DShellTool!=SOLVE_MIDAS)
					{
						pFEM->AddVehicleDB("KLTRG",0,Pf*2,0,0);
						pFEM->AddVehicleDB("KLTRG",0,Pm*2,dDist1,dDist1);
						pFEM->AddVehicleDB("KLTRG",0,Pm*2,dDist2,dDist2);
						pFEM->AddVehicleDB("KLTRG",0,Pr*2,dDist3,dDist3);

						pFEM->AddVehicleDB("KLLAN",0,Pf * 0.75,0,0);
						pFEM->AddVehicleDB("KLLAN",0,Pm * 0.75,dDist1,dDist1);
						pFEM->AddVehicleDB("KLLAN",0,Pm * 0.75,dDist2,dDist2);
						pFEM->AddVehicleDB("KLLAN",0,Pr * 0.75,dDist3,dDist3);
						pFEM->AddVehicleDB("KLLAN",2,wl,0,0);
					}
					else
					{
						pFEM->AddVehicleDB("KLTRG",0,Pf * 2,dDist1,dDist1);
						pFEM->AddVehicleDB("KLTRG",0,Pmt * 2,dDist2,dDist2);
						pFEM->AddVehicleDB("KLTRG",0,Pmt * 2,dDist3,dDist3);
						pFEM->AddVehicleDB("KLTRG",0,Pr * 2,dDist3,dDist3);

						pFEM->AddVehicleDB("KLLAN",1,wl * 3.0,0,0);
						pFEM->AddVehicleDB("KLLAN",0,Pf * 2 * 0.75,dDist1,dDist1);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 2 * 0.75,dDist2,dDist2);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 2 * 0.75,dDist3,dDist3);
						pFEM->AddVehicleDB("KLLAN",0,Pr * 2 * 0.75,dDist3,dDist3);
					}
					

					pFEM->AddVehicleClass("KLTRG", "KLTRG");
					pFEM->AddVehicleClass("KLLAN", "KLLAN");			

					//////////////////////////////////////////////////////////////////////////
					double dRF[6] = {1.0, 0.9, 0.8, 0.7, }; // {1.0, 0.9, 0.8, 0.7, 0.65, }; 4개를 입력해야 한다.
					double ScaleFactor = 1.00 + pLoad->m_dCoefficientImpact[0];
					long nMinLane = 1;

					// (ARCBRIDGE-3289) 다차로 하중계수를 쓰지 않아도 갯수는 네개를 써야한다. 값을 1.0으로 바꿔준다.
					// long nRF = (pBri->m_bLoadMultiRoadSurConfficient && pData->m_nLSDRoadType == 1) ? 4 : 1;
					long nRF = 4;
					if(pBri->m_bLoadMultiRoadSurConfficient == FALSE)
					{
						for(long ix=0; ix<4; ++ix)
							dRF[ix] = 1.0;
					}
				
					// (ARCBRIDGE-2620) 차선 그룹이 나눠져 있어도 모든 차선을 다 재하하고 있기 때문에 하나만 넣는다.
					//for(nGroup=0; nGroup<nQtyLaneGroup; nGroup++)
					{
						pFEM->AddMovingLoad("KLTRG",nRF,dRF,"KLTRG","SLAB",ScaleFactor,nMinLane,nQtyLaneTotal);
						pFEM->AddMovingLoad("KLLAN",nRF,dRF,"KLLAN","SLAB",ScaleFactor,nMinLane,nQtyLaneTotal);
					}

					// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다. 조합이 없으므로 무조건 출력
					// if(pBri->m_plsdManager->IsUsedComboKind(eLSD_COMBINE_STRESS))
					{
						if(FALSE) //pBri->m_nSelect3DShellTool!=SOLVE_MIDAS)
						{
							pFEM->AddVehicleDB("KLTFG",0,Pf * 2 * 0.8,0,0);
							pFEM->AddVehicleDB("KLTFG",0,Pm * 2 * 0.8,dDist1,dDist1);
							pFEM->AddVehicleDB("KLTFG",0,Pm * 2 * 0.8,dDist2,dDist2);
							pFEM->AddVehicleDB("KLTFG",0,Pr * 2 * 0.8,dDist3,dDist3);
						}
						else
						{
							pFEM->AddVehicleDB("KLTFG",0,Pf * 2 * 0.8,dDist1,dDist1);
							pFEM->AddVehicleDB("KLTFG",0,Pmt * 2 * 0.8,dDist2,dDist2);
							pFEM->AddVehicleDB("KLTFG",0,Pmt * 2 * 0.8,dDist3,dDist3);
							pFEM->AddVehicleDB("KLTFG",0,Pr * 2 * 0.8,dDist3,dDist3);
						}

						pFEM->AddVehicleClass("KLTFG", "KLTFG");

						//for(nGroup=0; nGroup<nQtyLaneGroup; nGroup++)
						{
							// (ARCBRIDGE-3290) 빈도수 빼고 다차로 하중계수 적용
							// (ARCBRIDGE-3290) 피로하중은 충격계수를 ScaleFactor 쓰지 않고 1.15적용
							// double dTF[6] = {1.0, 0.85, 0.8, 0.8, };		// {1.0, 0.85, 0.8 } 4개를 입력해야 한다.
							CMovingLoad *pMoving = pFEM->AddMovingLoad("KLTFG",nRF,dRF,"KLTFG","SLAB",1.15,nMinLane,nQtyLaneTotal);

							// (ARCBRIDGE-3290) 피로하중 산정시 각각의 lane설정후 조합하는것으로 Lane설정을 변경
							pMoving->m_nLType = 2;
						}
					}
				}
				else
				{
					pFEM->AddVehicleDB("DB",0,Pf*2,4.2,4.2);
					pFEM->AddVehicleDB("DB",0,Pr*2,4.2,4.2);
					pFEM->AddVehicleDB("DB",0,Pr*2,9.0,9.0);
					pFEM->AddVehicleDL("DL",0,Dtb,Pm,Ps);

					pFEM->AddVehicleClass("DB", "DB");
					pFEM->AddVehicleClass("DL", "DL");			

					//////////////////////////////////////////////////////////////////////////
					long nRF = 4;
					double dRF[6] = {1.0, 1.0, 0.9, 0.75, };
					double ScaleFactor = 1.00 + pLoad->m_dCoefficientImpact[0];
					long nMinLane = 1;
					for(nGroup=0; nGroup<nQtyLaneGroup; nGroup++)
					{
						pFEM->AddMovingLoad("DB",nRF,dRF,"DB","SLAB",ScaleFactor,nMinLane,nQtyLaneTotal);
						pFEM->AddMovingLoad("DL",nRF,dRF,"DL","SLAB",ScaleFactor,nMinLane,nQtyLaneTotal);
					}
				}
			}		
			if(bApplyLoadHeavy)
			{
				double dWidth = pLoad->m_dTank_TrailerWidth + pLoad->m_dTank_E; // 전폭하중으로 재하
				double dPtr1 = GetValueUnitChange(pLoad->m_dTank_Ptrailer1/(1+pLoad->m_dTank_i)*dWidth,UNIT_CGS_TONF,pData->m_szTonf); // MIDAS에서 i를 자동 적용
				double dPtr2 = GetValueUnitChange(pLoad->m_dTank_Ptrailer2/(1+pLoad->m_dTank_i)*dWidth,UNIT_CGS_TONF,pData->m_szTonf);
				double dPtr3 = GetValueUnitChange(pLoad->m_dTank_Ptrailer3/(1+pLoad->m_dTank_i)*dWidth,UNIT_CGS_TONF,pData->m_szTonf);

				pFEM->AddVehicleDB("TRAILOR",0,dPtr1,pLoad->m_dTank_TrailerL1,pLoad->m_dTank_TrailerL1);
				pFEM->AddVehicleDB("TRAILOR",0,dPtr2,pLoad->m_dTank_TrailerL2,pLoad->m_dTank_TrailerL2);
				pFEM->AddVehicleDB("TRAILOR",0,dPtr2,pLoad->m_dTank_TrailerL3,pLoad->m_dTank_TrailerL3);
				pFEM->AddVehicleDB("TRAILOR",0,dPtr3,pLoad->m_dTank_TrailerL4,pLoad->m_dTank_TrailerL4);
				pFEM->AddVehicleDB("TRAILOR",0,dPtr3,9.0,9.0);
				//
				// 마이다스는 분포이동하중이 적용이 안되므로 10등분해서 재하
				dWidth = pLoad->m_dTank_TankWidth + pLoad->m_dTank_E; // 전폭하중으로 재하
				double dDiv   = 10;
				double dPLoad = GetValueUnitChange(pLoad->m_dTank_Ptank/(1+pLoad->m_dTank_i)*pLoad->m_dTank_TankLength*dWidth/dDiv,UNIT_CGS_TONF,pData->m_szTonf);
				double dPDist = pLoad->m_dTank_TankLength/dDiv;
				long i=0; for(i=0; i<=(long)dDiv; i++)
				{
					if(i==0)
					{
						pFEM->AddVehicleDB("TANK",0,dPLoad*0.5,dPDist*0.5,dPDist*0.5);
					}
					else if(i==(long)dDiv-1)
					{
						pFEM->AddVehicleDB("TANK",0,dPLoad,dPDist*0.5,dPDist*0.5);
					}
					else if(i==(long)dDiv)
					{
						pFEM->AddVehicleDB("TANK",0,dPLoad*0.5,0,0);
					}
					else
					{
						pFEM->AddVehicleDB("TANK",0,dPLoad,dPDist,dPDist);
					}
				}
				//
				pFEM->AddVehicleClass("TRAILOR", "TRAILOR");
				pFEM->AddVehicleClass("TANK"   , "TANK");			


				double dRF[6] = {1,};
				double ScaleFactor = 1.00 + pLoad->m_dTank_i;
				pFEM->AddMovingLoad("TRAILOR",1,dRF,"TRAILOR","SLAB",ScaleFactor,1,1);
				pFEM->AddMovingLoad("TANK"   ,1,dRF,"TANK"   ,"SLAB",ScaleFactor,1,1);
			}
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadLiveAlways( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	long nQtySpan = pBri->m_nQtyJigan;

	if(bLsd)
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LS, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LS, TRUE);	
	}
	else
	{
		strLoadName	= _T("상재활하중");
		strLoadMark	= pData->GetStringLoadCase(BRIDGE_LOAD_LIVE_FULL);;
	}

	pFEM->MakeShellLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

	double dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	POSITION pos = pFEM->m_ShellList.GetHeadPosition();
	while(pos)		
	{
		if(Compare(dLoadValue,0.0,"=")) break;
		CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
		if(pShell->m_nAttrib == SHELL_ELEMENT_UPPERSLAB)
		{			
			CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(CVector(0,0,0), 0, -dLoadValue, "GZ", CLoadForcePress::PRES);
		}
	}		

	// 측벽에 작용하는 활하중
	//CJoint *pJntStt = pFEM->GetJoint(0);
	// 시점측벽체
	dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoaUR_Phl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
	{
		CShell *pS = pFEM->GetShell((USHORT)i);					
		if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==0)
		{			
			CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoadMark;
			double dPress[5] = {0,-dLoadValue,-dLoadValue,-dLoadValue,-dLoadValue};
			pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
		}
		// 종점측벽체
		else if(pS->m_nAttrib == SHELL_ELEMENT_WALL && pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==nQtySpan)
		{			
			CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
			if(pELoad == NULL) continue;
			pELoad->m_szLoadTitle = strLoadMark;
			double dPress[5] = {0,-dLoadValue,-dLoadValue,-dLoadValue,-dLoadValue};
			pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);			
		}
	}
	++nLoadCaseIndex;
}

void CARcBridgeDesignShell::MakeSapDataLoadWaterPress_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CLineInfo *pLine = pBri->GetLineBase();

	double dLoadValue(0);
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	// 수평수압 WA
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_WA))
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1, TRUE);

		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
		double dEle_Stt  = toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(0).x) - dUT);
		
		CJoint *pJntStt = pFEM->GetJoint(0);
		double dWater1 = 0, dWater2=0, dWater3=0, dWater4=0;
		long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
		{
			CShell *pS = pFEM->GetShell((USHORT)i);

			if(pS->m_nAttrib == SHELL_ELEMENT_WALL && (pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==0 || pS->m_LongAttrib[LONGSHELL_1_JIJUMNO] == pBri->m_nQtyJigan))
			{
				BOOL bLeft = (pS->m_LongAttrib[LONGSHELL_1_JIJUMNO] == 0)? TRUE : FALSE;
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dWater1 = pLoad->GetLoadWaterPressure(dEle_Stt - toM(pJntStt->m_vPoint.z - pJnt1->m_vPoint.z));
				dWater2 = pLoad->GetLoadWaterPressure(dEle_Stt - toM(pJntStt->m_vPoint.z - pJnt2->m_vPoint.z));
				dWater3 = pLoad->GetLoadWaterPressure(dEle_Stt - toM(pJntStt->m_vPoint.z - pJnt3->m_vPoint.z));
				dWater4 = pLoad->GetLoadWaterPressure(dEle_Stt - toM(pJntStt->m_vPoint.z - pJnt4->m_vPoint.z));
				if(dWater1 <= 0 && dWater2 <= 0 && dWater3 <= 0 && dWater4 <= 0) continue;

				dWater1 = GetValueUnitChange(dWater1,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater2 = GetValueUnitChange(dWater2,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater3 = GetValueUnitChange(dWater3,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater4 = GetValueUnitChange(dWater4,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

				pELoad->m_szLoadTitle = strLoadMark;
				double dPress[5] = {0,-dWater1,-dWater2,-dWater3,-dWater4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);
			}
		}
		++nLoadCaseIndex;
	}

	// 연직수압 EVVW
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV))
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW, TRUE);

		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		dLoadValue = GetValueUnitChange(pLoad->m_dLoadWaterVert, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pShell->m_nAttrib == SHELL_ELEMENT_LOWERSLAB && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] < 0)
			{
				BOOL bStt = pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] == -1;

				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(CVector(0,0,0), 0, bStt? -dLoadValue: -dLoadValue, "GZ", CLoadForcePress::PRES);
			}
		}

		++nLoadCaseIndex;
	}

	// 내부수압
	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
	{
		ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_EVI);

		switch (pBri->m_nSelectInteriorWaterPress)
		{
		case 0:
			eLoadCase = eLSD_LOADCASE_EVI;
			break;
		case 1:
			eLoadCase = eLSD_LOADCASE_BPI;
			break;
		case 2:
			eLoadCase = eLSD_LOADCASE_WA21;
			break;
		}

		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);

		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dHeightWater = pBri->m_dHeightInteriorWaterPress;

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pShell->m_nAttrib == SHELL_ELEMENT_LOWERSLAB && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] >= 0)
			{
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(CVector(0,0,0), 0, -dUW_Water * dHeightWater, "GZ", CLoadForcePress::PRES);
			}
		}

		++nLoadCaseIndex;
	}
}

void CARcBridgeDesignShell::SetSpringPilePosForExtLine( CFEMManage *pFEM, long nSttJointIndex )
{
	// 확폭일때는 말뚝위치에 정확하게 절점이 생성되지 않으므로 가장 가까운 절점을 찾아준다.
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(pBri->IsBoxType() == FALSE)
		return;

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
	long nSizePos = pMeshCalc->m_vArrSlabvPoint_real.GetSize();

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	CDPoint xyPile(0,0);
	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;
		for(long kk = 0; kk < pOrg->countSide; kk++)
		{
			xyPile = pBri->GetXyPilePos(pFooting, ix, kk);

			long nJoint = -1;
			double dDistMin = 0;
			for(long nx=0; nx<nSizePos; ++nx)
			{
				CDPoint xyJoint = pMeshCalc->m_vArrSlabvPoint_real.GetAt(nx);
				double dDist = ~(xyPile - xyJoint);

				if(nx == 0 || dDist < dDistMin)
				{
					nJoint = nx;
					dDistMin = dDist;
				}
			}
			
			CJoint *pJoint = pFEM->GetJoint(nSttJointIndex + nJoint);
			if(pJoint)
			{
				pJoint->m_vSpring.x  = 1000; // Temp
				pJoint->m_vSpring.y  = 1000;
				pJoint->m_vSpring.z  = 1000;
				pJoint->m_vSpringR.x = 1000;
				pJoint->m_vSpringR.y = 1000;
				pJoint->m_vSpringR.z = 1000;
			}
		}
	}
}

void CARcBridgeDesignShell::MakeSapDataLoadWaterPress_Usd( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeMeshCalc *pMeshCalc = m_pStd->m_pMeshCalc;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CLineInfo *pLine = pBri->GetLineBase();

	double dLoadValue(0);
	CString strLoadName(_T(""));
	CString strLoad(_T(""));

	// 수평수압 WA
	long nLoadCase = BRIDGE_LOAD_WATERHORI;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평수압");

		double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
		double dEle_Stt  = toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(0).x) - dUT);

		CJoint *pJntStt = pFEM->GetJoint(0);
		double dWater1 = 0, dWater2=0, dWater3=0, dWater4=0;
		long i=0; for(i=0; i<pFEM->GetShellSize(); i++)
		{
			CShell *pS = pFEM->GetShell((USHORT)i);

			if(pS->m_nAttrib == SHELL_ELEMENT_WALL && (pS->m_LongAttrib[LONGSHELL_1_JIJUMNO]==0 || pS->m_LongAttrib[LONGSHELL_1_JIJUMNO] == pBri->m_nQtyJigan))
			{
				BOOL bLeft = (pS->m_LongAttrib[LONGSHELL_1_JIJUMNO] == 0)? TRUE : FALSE;
				CLoadForceShell *pELoad = pS->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				CJoint *pJnt1 = pFEM->GetJoint(pS->m_nJ[0]);
				CJoint *pJnt2 = pFEM->GetJoint(pS->m_nJ[1]);
				CJoint *pJnt3 = pFEM->GetJoint(pS->m_nJ[2]);
				CJoint *pJnt4 = pFEM->GetJoint(pS->m_nJ[3]);
				dWater1 = pLoad->GetLoadWaterPressure(dEle_Stt - (pJntStt->m_vPoint.z - pJnt1->m_vPoint.z));
				dWater2 = pLoad->GetLoadWaterPressure(dEle_Stt - (pJntStt->m_vPoint.z - pJnt2->m_vPoint.z));
				dWater3 = pLoad->GetLoadWaterPressure(dEle_Stt - (pJntStt->m_vPoint.z - pJnt3->m_vPoint.z));
				dWater4 = pLoad->GetLoadWaterPressure(dEle_Stt - (pJntStt->m_vPoint.z - pJnt4->m_vPoint.z));
				if(dWater1 <= 0 && dWater2 <= 0 && dWater3 <= 0 && dWater4 <= 0) continue;

				dWater1 = GetValueUnitChange(dWater1,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater2 = GetValueUnitChange(dWater2,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater3 = GetValueUnitChange(dWater3,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWater4 = GetValueUnitChange(dWater4,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

				pELoad->m_szLoadTitle = strLoad;
				double dPress[5] = {0,-dWater1,-dWater2,-dWater3,-dWater4};
				pELoad->SetLoadForce(CVector(0,0,0),dPress,"LZ",CLoadForcePress::HYDRO);
			}
		}
		++nLoadCaseIndex;
	}

	// 연직수압
	nLoadCase = BRIDGE_LOAD_WATERVERT;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeShellLoadForce(nLoadCaseIndex+1);

		strLoad  = pData->GetStringLoadCase(nLoadCase);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "연직수압");

		dLoadValue = GetValueUnitChange(pLoad->m_dLoadWaterVert, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);

		POSITION pos = pFEM->m_ShellList.GetHeadPosition();
		while(pos)		
		{
			CShell *pShell = (CShell *)pFEM->m_ShellList.GetNext(pos);
			if(pShell->m_nAttrib == SHELL_ELEMENT_LOWERSLAB && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] < 0)
			{
				BOOL bStt = pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] == -1;

				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(CVector(0,0,0), 0, bStt? -dLoadValue: -dLoadValue, "GZ", CLoadForcePress::PRES);
			}

			// 하부슬래브 부력
			if(pShell->m_nAttrib == SHELL_ELEMENT_LOWERSLAB && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] >= 0)
			{
				CLoadForceShell *pELoad = pShell->GetLoadForce(nLoadCaseIndex);
				if(pELoad == NULL) continue;
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(CVector(0,0,0), 0, pLoad->m_dLoadBuoyancy, "GZ", CLoadForcePress::PRES);
			}
		}

		++nLoadCaseIndex;
	}
}
