// ARcBridgeCalcFooting.cpp: implementation of the CARcBridgeCalcFooting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

#define LSD_CHECK_AXIAL_COEF		0.66
#define LSD_CHECK_AXIALMOMENT_COEF	1.00
#define EFFECTIVE_WIDTH_DL	3.00
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcFooting::CARcBridgeCalcFooting()
{
	long i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		long j=0; for(j=0; j<4; j++)
		{
			m_dATensionUpperMUse[i][j] = 0;				// ������� ������� ���Ʈ
			m_dATensionLowerMUse[i][j] = 0;				// ������� �ϸ����� ���Ʈ
			m_dATensionUpperMUlt[i][j] = 0;				// ������� ������� ���Ʈ
			m_dATensionLowerMUlt[i][j] = 0;				// ������� �ϸ����� ���Ʈ
			m_dATensionUpperVUlt[i][j] = 0;				// ������� ������� ���ܷ�
			m_dATensionLowerVUlt[i][j] = 0;				// ������� �ϸ����� ���ܷ�
		}
	}

	for (i = 0; i <MAX_JIJUM_COUNT_RC; ++i)
	{
		for (HGINT32 jx = 0; jx < eCountEnvTypeLsd; ++jx)
		{
			for (HGINT32 kx = 0; kx < 4; ++kx)
			{
				m_dATensionUpperMLsd[i][jx][kx] = 0;
				m_dATensionLowerMLsd[i][jx][kx] = 0;
				m_dATensionUpperVLsd[i][jx][kx] = 0;
				m_dATensionLowerVLsd[i][jx][kx] = 0;
			}
		}
	}

	for(i=0; i<MAX_MASS_DAN*2; i++)
	{
		m_dB[i];
		m_dH[i];
		m_dA[i];
		m_dDist[i];
		m_dVer[i];
		m_dHor[i];
		m_dMoment[i];
	}

	m_dSelfWeight = 0;
	m_dPaveWeight = 0;
	m_dAdditionalDC = 0;
	m_dAdditionalDW = 0;
	m_dLive1		= 0;
	m_dLive2		= 0;
	m_dPr = 0;
	m_dPf = 0;
	m_dPm = 0;
	m_dLiveLoad	= 0;
	m_dSideFriction = 0;
	m_dBuoyance = 0;
	m_dSF_Buoyance = 0;
	m_dInteriorWaterWeight = 0;
}

CARcBridgeCalcFooting::~CARcBridgeCalcFooting()
{
	int i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		AhTPADelete(&m_pListLoadUse[i],(CLoadFooting*)0);
		AhTPADelete(&m_pListLoadUlt[i],(CLoadFooting*)0);
		AhTPADelete(&m_pListLoadSafty[i],(CLoadFooting*)0);

		for(long ixEnv=0; ixEnv<eCountEnvTypeLsd; ++ixEnv)
		{
			AhTPADelete(&m_pListLoadLsd[i][ixEnv],(CLoadFooting*)0);
		}
	}
	
	ClearVectorPtr(m_pListLsdFootingSafe);
}

void CARcBridgeCalcFooting::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd)
		CalculateAll_Lsd();
	else
		CalculateAll_Usd();
}

void CARcBridgeCalcFooting::CalculateAll_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	long nCntJijum = pBri->IsBoxType() ? 1 : m_pStd->m_pBri->GetCountJijum();
	for(long ix=0; ix<nCntJijum; ix++)
	{
		for(long ixEnv=0; ixEnv<eCountEnvTypeLsd; ++ixEnv)
		{
			AhTPADelete(&m_pListLoadLsd[ix][ixEnv],(CLoadFooting*)0);
		}
	}
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) 
	{
		return;
	}

	if(pBri->IsBoxType())
	{
		CalculateFootingAndPile_BoxType_Lsd();
	}
	else
	{
		CalculateLoad_Lsd();

		// ��������
		CheckFootingSafeLSD();

		CalculateFootingAndPile_Lsd();
	}
}

void CARcBridgeCalcFooting::CalculateAll_Usd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
		
	long nCntJijum = pBri->IsBoxType() ? 1 : m_pStd->m_pBri->GetCountJijum();
	for(long ix=0; ix<nCntJijum; ix++)
	{
		AhTPADelete(&m_pListLoadUse[ix],(CLoadFooting*)0);
		AhTPADelete(&m_pListLoadUlt[ix],(CLoadFooting*)0);
		AhTPADelete(&m_pListLoadSafty[ix],(CLoadFooting*)0);
	}
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) 
	{
		return;
	}

	if(pBri->IsArchType()) 
	{
		pBri->TrimRibByFootingUpper();
	}

	if(pBri->IsBoxType())
	{
		CalculateFootingAndPile_BoxType_Usd();
	}
	else
	{
		CalculateLoad_Usd();
		CalculateFootingAndPile_Usd();
	}
}

void CARcBridgeCalcFooting::CalculateFootingAndPile_BoxType_Usd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) return;

	for(long ix = 0; ix < 4; ix++)
	{
		m_dArrDispX[ix].RemoveAll();
		m_dArrDispZ[ix].RemoveAll();
		m_dArrDispR[ix].RemoveAll();
	}
	long nJijum = 0;
	CFootingApp *	pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL || pFooting->m_bIs == FALSE) return;

	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	if(pPile && pBri->IsPileBase(nJijum))
	{
		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		m_pStd->SetCalcPileData(pCalcPile);
		AhTPADelete(&pCalcPile->m_pListUse, (CPileLoad*)0);
		AhTPADelete(&pCalcPile->m_pListUlt, (CPileLoad*)0);

		double dMaxH = 0, dMaxM = 0, dMaxV = 0, dMinV = 100000000, dMaxV_Ult = 0; // ���պΰ���
		double Momet = 0, Shear = 0, Axial = 0, DispX = 0, DispZ = 0, DispR = 0;
		long nCountPile = pPile->m_nCountOrg;
		for(long ix = 0; ix < nCountPile; ix++)
		{
			long nIdxJoint = pBri->GetOutValueForPileDesign_Usd(COMBO_SERVICE, ix, Momet, Shear, Axial, DispX, DispZ, DispR);
			CPileLoad *pLoad = new CPileLoad;
			pLoad->m_nTypeLoad  = ix + 1;
			pLoad->m_dMoment    = GetValueUnitChange(Momet, pData->m_szTonfM, UNIT_CGS_TONFM);	// (+)(-)
			pLoad->m_dForceHori = GetValueUnitChange(Shear, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)(-)
			pLoad->m_dForceVert = GetValueUnitChange(Axial, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)	
			m_dArrDispX[0].Add(DispX);
			m_dArrDispZ[0].Add(DispZ);
			m_dArrDispR[0].Add(DispR);
			//				pLoad->m_sName.Format(_T("��� %02d"), nIdxJoint + 1);
			pLoad->m_sName.Format(_T("���� %02d"), nIdxJoint + 1);
			pCalcPile->m_pListUse.Add(pLoad); // �������
			dMaxM = MAX(dMaxM, fabs(pLoad->m_dMoment));
			dMaxV = MAX(dMaxV, fabs(pLoad->m_dForceVert));
			dMinV = MIN(dMinV, fabs(pLoad->m_dForceVert));
			dMaxH = MAX(dMaxH, fabs(pLoad->m_dForceHori));
		}
		for(long ix = 0; ix < nCountPile; ix++)
		{
			long nIdxJoint = pBri->GetOutValueForPileDesign_Usd(COMBO_ULTIMATE, ix, Momet, Shear, Axial, DispX, DispZ, DispR);
			CPileLoad *pLoad = new CPileLoad;
			pLoad->m_nTypeLoad  = ix + 1;
			pLoad->m_dMoment    = GetValueUnitChange(Momet, pData->m_szTonfM, UNIT_CGS_TONFM);	// (+)(-)
			pLoad->m_dForceHori = GetValueUnitChange(Shear, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)(-)
			pLoad->m_dForceVert = GetValueUnitChange(Axial, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)	
			m_dArrDispX[1].Add(DispX);
			m_dArrDispZ[1].Add(DispZ);
			m_dArrDispR[1].Add(DispR);

			//				pLoad->m_sName.Format(_T("��� %02d"), nIdxJoint + 1);
			pLoad->m_sName.Format(_T("���� %02d"), nIdxJoint + 1);
			pCalcPile->m_pListUlt.Add(pLoad); // �������
			dMaxV_Ult = MAX(dMaxV_Ult, fabs(pLoad->m_dForceVert));
		}

		double dMaxHe = 0, dMaxMe = 0, dMaxVe = 0, dMinVe = 100000000; // ���պΰ���
		for(long nEQ = 0; nEQ < 2; nEQ++)
		{
			if(nEQ == 1 && pBri->m_nSelectModelingCountEQ != 2) break;
			//BOOL bFunction = (nEQ == 1 || (nEQ == 0 && pBri->m_nSelectModelingCountEQ == 1)) ? TRUE : FALSE;
			for(long ix = 0; ix < nCountPile; ix++)
			{
				long nIdxJoint = pBri->GetOutValueForPileDesign_Usd(COMBO_EARTHQUAKE + nEQ, ix, Momet, Shear, Axial, DispX, DispZ, DispR);
				CPileLoad *pLoad = new CPileLoad;
				pLoad->m_dWCaseApply |= LOAD_PILE_EQ;
				pLoad->m_nTypeLoad  = ix + 1;
				pLoad->m_dMoment    = GetValueUnitChange(Momet, pData->m_szTonfM, UNIT_CGS_TONFM);	// (+)(-)
				pLoad->m_dForceHori = GetValueUnitChange(Shear, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)(-)
				pLoad->m_dForceVert = GetValueUnitChange(Axial, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)	
				m_dArrDispX[2+nEQ].Add(DispX);
				m_dArrDispZ[2+nEQ].Add(DispZ);
				m_dArrDispR[2+nEQ].Add(DispR);
				//					pLoad->m_sName.Format(_T("%s %02d"), bFunction ? _T("���") : _T("�ر�") , nIdxJoint + 1);
				pLoad->m_sName.Format(_T("���� %02d"), nIdxJoint + 1);
				pCalcPile->m_pListUse.Add(pLoad); // ��������
				dMaxMe = MAX(dMaxMe, fabs(pLoad->m_dMoment));
				dMaxVe = MAX(dMaxVe, fabs(pLoad->m_dForceVert));
				dMinVe = MIN(dMinVe, fabs(pLoad->m_dForceVert));
				dMaxHe = MAX(dMaxHe, fabs(pLoad->m_dForceHori));
			}
		}
		pCalcPile->CalculateAll();
		pCalcPile->SetUserForceCheckConnectionExtendFooting(FALSE, dMaxH, dMaxM, dMaxV, dMinV, dMaxV_Ult);
		pCalcPile->SetUserForceCheckConnectionExtendFooting(TRUE, dMaxHe, dMaxMe, dMaxVe, dMinVe, dMaxV_Ult);
		pCalcPile->SetUserInputForce(TRUE);
		pCalcPile->CheckConnectionExtendFooting();
	}

	// �η°���� ������ ���
	CalculateBuoyancyCheck();
	CalculateSupportCheck_Usd(FALSE);
}

void CARcBridgeCalcFooting::CalculateFootingAndPile_BoxType_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) return;

	for(long ix = 0; ix < 4; ix++)
	{
		m_dArrDispX[ix].RemoveAll();
		m_dArrDispZ[ix].RemoveAll();
		m_dArrDispR[ix].RemoveAll();
	}
	long nJijum = 0;
	CFootingApp *	pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL || pFooting->m_bIs == FALSE) return;

	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	if(pPile && pBri->IsPileBase(nJijum))
	{
		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		m_pStd->SetCalcPileData(pCalcPile);
		AhTPADelete(&pCalcPile->m_pListUse, (CPileLoad*)0);
		AhTPADelete(&pCalcPile->m_pListUlt, (CPileLoad*)0);

		double Momet = 0, Shear = 0, Axial = 0, DispX = 0, DispZ = 0, DispR = 0;
		long nCountPile = pPile->m_nCountOrg;

		for(long nTypeEnv=0; nTypeEnv<eCountEnvTypeLsd; ++nTypeEnv)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)nTypeEnv;
			CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			long nTypeCase(0);
			switch(eEnvType)
			{
			case eEnvLimit:
				nTypeCase = PILE_CASE_ULTIMATE;	break;
			case eEnvExtremeI:
			case eEnvExtremeII:
				nTypeCase = PILE_CASE_EXTREME;	break;
			case eEnvService:
			case eEnvService_Crack:
				nTypeCase = PILE_CASE_USE;	break;
			default:
				break;
			}

			for(long ix = 0; ix < nCountPile; ix++)
			{
				// (ARCBRIDGE-2580) �ؼ�������� �ݷ����� ���°��� �� 1mm�� �ݷ��̴�.
				long nIdxJoint = pBri->GetOutValueForPileDesign_Lsd(eEnvType, ix, Momet, Shear, Axial, DispX, DispZ, DispR);
				CPileLoad *pLoad = new CPileLoad;
				pLoad->m_nTypeLoad  = ix + 1;
				pLoad->m_dMoment    = GetValueUnitChange(Momet, pData->m_szTonfM, UNIT_CGS_TONFM);	// (+)(-)
				pLoad->m_dForceHori = GetValueUnitChange(Shear, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)(-)
				pLoad->m_dForceVert = GetValueUnitChange(Axial, pData->m_szTonf, UNIT_CGS_TONF);;	// (+)	
				pLoad->m_bUsabilityCheck	= (eEnvType == eEnvService);
				m_dArrDispX[nTypeCase].Add(DispX);
				m_dArrDispZ[nTypeCase].Add(DispZ);
				m_dArrDispR[nTypeCase].Add(DispR);
				//				pLoad->m_sName.Format(_T("��� %02d"), nIdxJoint + 1);
				pLoad->m_sName.Format(_T("���� %02d"), nIdxJoint + 1);
				
				pCalcPile->AddPileLoad(nTypeCase, pLoad);
			}
		}

		pCalcPile->CalculateAll();
		pCalcPile->SetUserInputForce(FALSE);
		pCalcPile->CheckConnectionExtendFooting();
	}

	// �η°���� ������ ���
	CalculateBuoyancyCheck();
	CalculateSupportCheck_Lsd(FALSE);
}

void CARcBridgeCalcFooting::CalculateFootingAndPile_Usd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double VCGS = 0;
	double HCGS = 0;
	double MCGS = 0;
	// ���� �����϶� ���Ұ��
	long nCntJijum = m_pStd->m_pBri->GetCountJijum();
	for(long i=0; i<nCntJijum; i++)
	{
		CDesignPile *pCalcPile = m_pStd->GetDesignPile(i);
		if(pCalcPile==NULL) continue;

		m_pStd->SetCalcPileData(pCalcPile);
		AhTPADelete(&pCalcPile->m_pListUse, (CPileLoad*)0);
		AhTPADelete(&pCalcPile->m_pListUlt, (CPileLoad*)0);
		long nSizeUse = m_pListLoadUse[i].GetSize();
		for(long j=0; j<nSizeUse; j++)
		{
			CLoadFooting *pLoad = m_pListLoadUse[i].GetAt(j);
			if(pLoad->m_nJijum!=pCalcPile->m_nIndexJijum) continue;
			if(pBri->IsApplyEarthQuakeDesign()==FALSE && j==3) // ������ ������
			{
				continue;
			}

			CPileLoad *pLoadUse = new CPileLoad;
			pLoadUse->Clear();

			if(j==3) 
				pLoadUse->m_dWCaseApply = LOAD_PILE_EQ;
			else
			{
				long nIndexBaseCombo = m_pStd->m_pBri->GetBaseIndexCombo(COMBO_SERVICE, pLoad->m_nCombo);
				DWORD dWCase = m_pStd->m_pBri->GetLoadCombo(nIndexBaseCombo,COMBO_SERVICE);
				if(dWCase&LOAD_CASE_EQ) pLoadUse->m_dWCaseApply |= LOAD_PILE_EQ;
				if(dWCase&LOAD_CASE_TEMP1 || dWCase&LOAD_CASE_TEMP2 || dWCase&LOAD_CASE_SHRINK) pLoadUse->m_dWCaseApply |= LOAD_PILE_TEMP;
				if(dWCase&LOAD_CASE_SB) pLoadUse->m_dWCaseApply |= LOAD_PILE_ZEDONG;
			}

			MCGS = GetValueUnitChange(pLoad->m_dPileLoadM, pData->m_szTonfM, UNIT_CGS_TONFM);
			HCGS = GetValueUnitChange(pLoad->m_dPileLoadH, pData->m_szTonf, UNIT_CGS_TONF);
			VCGS = GetValueUnitChange(pLoad->m_dPileLoadV, pData->m_szTonf, UNIT_CGS_TONF);

			// ���� m�� ������ �ƴ� ���� ��ü�� ���� �̹Ƿ� ������ ���� �����ش�.
			pLoadUse->m_nTypeLoad  = pLoad->m_nCombo + 1;	// �������� Index
			pLoadUse->m_dMoment    = MCGS;	// (+)(-)
			pLoadUse->m_dForceHori = HCGS;	// (+)(-)
			pLoadUse->m_dForceVert = VCGS;	// (+)	
			pLoadUse->m_sName = pLoad->m_sTitleSub;
			pCalcPile->m_pListUse.Add(pLoadUse); // �������
		}
		long nSizeUlt = m_pListLoadUlt[i].GetSize();
		for(long j=0; j<nSizeUlt; j++)
		{
			CLoadFooting *pLoad = m_pListLoadUlt[i].GetAt(j);
			if(pLoad->m_nJijum!=pCalcPile->m_nIndexJijum) continue;
			if(pBri->IsApplyEarthQuakeDesign()==FALSE && j==3) // ������ ������
			{
				continue;
			}

			CPileLoad *pLoadUlt = new CPileLoad;
			pLoadUlt->Clear();

			if(j==3) 
				pLoadUlt->m_dWCaseApply = LOAD_PILE_EQ;
			else
			{
				long nIndexBaseCombo = m_pStd->m_pBri->GetBaseIndexCombo(COMBO_ULTIMATE, pLoad->m_nCombo);
				DWORD dWCase = m_pStd->m_pBri->GetLoadCombo(nIndexBaseCombo,COMBO_ULTIMATE);
				if(dWCase&LOAD_CASE_EQ) pLoadUlt->m_dWCaseApply |= LOAD_PILE_EQ;
				if(dWCase&LOAD_CASE_TEMP1 || dWCase&LOAD_CASE_TEMP2 || dWCase&LOAD_CASE_SHRINK) pLoadUlt->m_dWCaseApply |= LOAD_PILE_TEMP;
				if(dWCase&LOAD_CASE_SB) pLoadUlt->m_dWCaseApply |= LOAD_PILE_ZEDONG;
			}

			MCGS = GetValueUnitChange(pLoad->m_dPileLoadM, pData->m_szTonfM, UNIT_CGS_TONFM);
			HCGS = GetValueUnitChange(pLoad->m_dPileLoadH, pData->m_szTonf, UNIT_CGS_TONF);
			VCGS = GetValueUnitChange(pLoad->m_dPileLoadV, pData->m_szTonf, UNIT_CGS_TONF);

			pLoadUlt->m_nTypeLoad  = pLoad->m_nCombo + 1;	// �������� Index
			pLoadUlt->m_dMoment    = MCGS;	// (+)(-)
			pLoadUlt->m_dForceHori = HCGS;	// (+)(-)
			pLoadUlt->m_dForceVert = VCGS;	// (+)	
			pLoadUlt->m_sName = pLoad->m_sTitleSub;
			pCalcPile->m_pListUlt.Add(pLoadUlt); // �������
		}
		pCalcPile->CalculateAll();
	}
	//
	CalculateSection_Usd();
}

void CARcBridgeCalcFooting::CalculateFootingAndPile_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	// ���� �����϶� ���Ұ��
	long nCntJijum = m_pStd->m_pBri->GetCountJijum();
	for(long nJ=0; nJ<nCntJijum; ++nJ)
	{
		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJ);
		if(pCalcPile==NULL) continue;

		m_pStd->SetCalcPileData(pCalcPile);
		AhTPADelete(&pCalcPile->m_pListUse, (CPileLoad*)0);
		AhTPADelete(&pCalcPile->m_pListUlt, (CPileLoad*)0);
		AhTPADelete(&pCalcPile->m_pListExtreme, (CPileLoad*)0);

		for(long nTypeEnv=0; nTypeEnv<eCountEnvTypeLsd; ++nTypeEnv)
		{
			// ������ ��뼺 ���� ������ �ڷ� ���� �Ǵϱ� ������ �ٲ��ش�.
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)nTypeEnv;
			if(eEnvType == eEnvService)
				eEnvType = eEnvService_Crack;
			else if(eEnvType == eEnvService_Crack)
				eEnvType = eEnvService;

			CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			long nTypeCase(0);
			switch(eEnvType)
			{
			case eEnvLimit:
				nTypeCase = PILE_CASE_ULTIMATE;	break;
			case eEnvExtremeI:
			case eEnvExtremeII:
				nTypeCase = PILE_CASE_EXTREME;	break;
			case eEnvService:
			case eEnvService_Crack:
				nTypeCase = PILE_CASE_USE;	break;
			default:
				break;
			}

			long nSizeCase = GetCountLoadFootingLsd(nJ, eEnvType);
			for(long jx=0; jx<nSizeCase; jx++)
			{
				CLoadFooting *pLoad = GetLoadFootingLsd(nJ, eEnvType, jx);
				if(pLoad == NULL) continue;

				CPileLoad *pPileLoad = new CPileLoad;
				pPileLoad->m_nTypeLoad  = pLoad->m_nCombo + 1;	// �������� Index
				pPileLoad->m_dMoment    = GetValueUnitChange(pLoad->m_dPileLoadM, pData->m_szTonfM, UNIT_CGS_TONFM);
				pPileLoad->m_dForceHori = GetValueUnitChange(pLoad->m_dPileLoadH, pData->m_szTonf, UNIT_CGS_TONF);
				pPileLoad->m_dForceVert = GetValueUnitChange(pLoad->m_dPileLoadV, pData->m_szTonf, UNIT_CGS_TONF);
				pPileLoad->m_sName = pLoad->m_sTitleSub;
				//pPileLoad->m_sDescription = pLoad->m_sTitle;
				pPileLoad->m_bUsabilityCheck	= (eEnvType == eEnvService);
				pLoad->m_sCombo;

				pCalcPile->AddPileLoad(nTypeCase, pPileLoad);
			}
		}

		pCalcPile->CalculateAll();
	}
	//
	CalculateSection_Lsd();
}

void CARcBridgeCalcFooting::WriteCalcFootingDesign(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	BOOL bTrap = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP;
	BOOL bTrap2 = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	BOOL bArch	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	BOOL bFirst = TRUE;
//	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	double dUwConc = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwAscon = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightAsphalt), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwBackFill = pDataStd->GetValueUnitType(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dwLiveAlways = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2);
	double dHeightPave = toM(pBri->m_dUnderGroundHeightPavement);
	if(pDataStd->IsRailDesign())
	{
		dHeightPave = 0;
	}

	// ��� ������ ������ �ϴ� Footing ���� ��ŭ �� ��´�.
	long nCntJijum = pBri->GetCountJijum();
	CLoadFooting *pData;

	int i=0; for(i=0; i<nCntJijum; i++)
	{
		long nIndexSmall = 1;
		if((i==0 || i== nCntJijum-1) && !pBri->IsOutsideWall()) continue;
		if(m_pListLoadUse[i].GetSize() == 0 || m_pListLoadUlt[i].GetSize() == 0) continue;
		
		pData = m_pListLoadUlt[i].GetAt(0);
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		if(nIndexMiddle != 1) XLText->AddFormatText("$s+99");
		XLText->AddFormatText("$s+05$e$h$a%d.%d %s$n", nIndexTitle, nIndexMiddle++, pData->m_sTitle);

		CDomyun *pDom = new CDomyun;
		DrawFootingSection(pDom, pBri, i);
		long nRowDom = (long)MAX(MIN(toM(pDom->GetExtHeight())*3/2, 20), 10);
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		XLText->PushRow();

		BOOL bInWall = (!pBri->IsOutsideWall() || (i!=0 && i!=pBri->m_nQtyJigan));
		double wB = (pData->m_nJijum == pBri->GetCountJijum()-1)? toM(pData->m_pFooting->m_dFWR):toM(pData->m_pFooting->m_dFWL);
		double H = toM(pData->m_pFooting->GetHeight());

		for(long nLine=0; nLine < nRowDom-3; nLine++)
		{
			XLText->AddFormatText("$u");
		}

		if(Compare(pData->m_dFactorDeadLoad, 0.0, _T(">"), 0.001))
		{
			if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
				||(bTrap2 && (i==0 || i==pBri->m_nQtyJigan)) || bArch)
			{
				double dA = 0;
				if(bTrap)
				{
					double dW = toM(pData->m_pFooting->GetWidth());

					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRH) : toM(pData->m_pFooting->m_dFSLH);
					double dFEW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFERW) : toM(pData->m_pFooting->m_dFELW);
					double dFSW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRW) : toM(pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}
				else
				{
					double dW = toM(pData->m_pFooting->GetWidth());
					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = toM((i==0)? pData->m_pFooting->m_dFSRH : pData->m_pFooting->m_dFSLH);
					double dFEW = toM((i==0)? pData->m_pFooting->m_dFERW : pData->m_pFooting->m_dFELW);
					double dFSW = toM((i==0)? pData->m_pFooting->m_dFSRW : pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}			

				*XLText  += 5;

				XLText->AddFormatText("$h$t$t$m+02��������$m+03$c=$r$e%.3f x %.3f x %.2f$n", dA, dUwConc, pData->m_dFactorDeadLoad);
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_FOOTW], pDataStd->m_szTonf_M);
			}
			else
			{
				XLText->AddFormatText("$h$t$t$m+02��������$m+03$c=$r$e%.3f x %.3f x %.3f x %.2f$n", toM(pData->m_pFooting->GetWidth()), H, dUwConc, pData->m_dFactorDeadLoad);
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_FOOTW], pDataStd->m_szTonf_M);
			}
		}
		else
		{
			XLText->AddText("$h$t$t$m+02��������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
		}

		
		
		if(pData->m_nJijum==0 || pData->m_nJijum==pBri->GetCountJijum()-1)
		{
			if(Compare(pData->m_dFactorSoilHor, 0.0, _T(">"), 0.001))
			{
				XLText->AddFormatText("$h$t$t$m+02������$m+03$c=$r$e%.3f x %.3f x %.3f x %.2f$n", wB, pData->m_dHightBackFill, dUwBackFill, pData->m_dFactorSoilHor);
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_BACKFILL], pDataStd->m_szTonf_M);
			}
			else
			{
				XLText->AddText("$h$t$t$m+02������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
			}
			
			
			if(pBri->m_bUnderGroundRahmen)
			{
				CDPoint xyStt(0, 0);
				xyStt.x	= pBri->GetStationOnJijum(pData->m_nJijum, 0);
				if(pData->m_nJijum==0) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, -pBri->m_dWS, 0).x;
				else xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, pBri->m_dWE, 0).x;
				
				double dElBackFill = pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y;
				double dLiveAlwyas = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pDataStd->m_szTonf_M2);

				if(Compare(pData->m_dFactorLiveLoad, 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02����Ȱ����$m+03$c=$r$e%.3f x %.3f x %.2f$n", wB, dLiveAlwyas, pData->m_dFactorLiveLoad);
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_LIVE], pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+02����Ȱ����$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(Compare(pData->m_dFactorSoilHor, 0.0, _T(">"), 0.001))
				{
					BOOL bOn = TRUE;
					if(pBri->m_bUnderGroundPaveDetail)
					{
						XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r");

						double dTickPave = 0;
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightPyo > 0)	// ǥ��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightPyo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightPyo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06(%.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightGee > 0)	// ����
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightGee < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightGee;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06(%.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightMiddle > 0)	// �߰���
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightMiddle < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightMiddle;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightBozo > 0)	// ��������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightBozo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightBozo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightDong > 0)	// ���������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightDong < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightDong;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightEps > 0)	// EPS��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightEps < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightEps;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->GetUnderGroundPaveHeightTosa() > 0)	// ���
						{
							if(dElBackFill - pBri->GetUnderGroundPaveHeightTosa()< pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->GetUnderGroundPaveHeightTosa();

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %.3f x %.3f$m+04", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							else XLText->AddFormatText("$c+$r%.3f x %.3f$n", toM(dTickPave), pDataStd->GetValueUnitType(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
							//dElBackFill -= dTickPave;
							bOn = !bOn;
						}
					}
					else if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						bOn = TRUE;
						XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r(%.3f x %.3f$m+04$c+$r%.3f + %.3f$n", dHeightPave, (pBri->m_nTypePavement==0)? dUwAscon:dUwConc, toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
						//					XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r(%.3f x %.3f + %.3f + %.3f)$n", toM(pBri->m_dUnderGroundHeightPavement), (pBri->m_nTypePavement==0)? dUwAscon:dUwConc, toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+09$f3%.3f$me1$m+02x$r$f3%.3f$me1$r)$n", toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+05$c=$r x %.3f$n", wB);
					}

					if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						if(bOn) XLText->AddFormatText("$h$t$t$m+06) x %.3f x %.2f$n", wB, pData->m_dFactorSoilHor);
						else XLText->AddFormatText("$e)$r$cx$%.3f x %.2f$n", wB, pData->m_dFactorSoilHor);

						XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_PAVE], pDataStd->m_szTonf_M);
					}

					if(pBri->m_nSelectModelingHeight != 0)
						XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
				}
				else
				{
					XLText->AddText("$h$t$t$m+02�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}

				*XLText  += nRowDom-10;
			}
			else if(pBri->m_dTP > 0)
			{
				if(Compare(pData->m_dFactorSoilHor, 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r$e%.3f x %.3f x %.3f x %.2f$n", wB, toM(pBri->m_dTP), dUwAscon, pData->m_dFactorSoilHor);
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_PAVE], pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+02�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(Compare(pData->m_dFactorLiveLoad, 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02���Ȱ����$m+03$c=$r$e%.3f x %.3f x %.2f$n", wB, dwLiveAlways, pData->m_dFactorLiveLoad);
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_LIVE], pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+02���Ȱ����$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				
				if(pBri->m_nSelectModelingHeight != 0)
					XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
				
				*XLText  += nRowDom-12;
			}
		}
		else 
		{
			if(pBri->m_nSelectModelingHeight != 0)
				XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
			*XLText  += nRowDom-6;
		}
		XLText->PopRow();
		*XLText  += 1;

		// ��米���� �����϶��� ������ ���ɹ� ���� ���� �ٰ� Table�� ����Ѵ�.
		if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
			||(bTrap2 && (pData->m_nJijum==0 || pData->m_nJijum==pBri->m_nQtyJigan)) || bArch)
		{
			*XLText += 3;
			XLText->AddFormatText("$u$u$u$u$u");
			pData->MakeTableFootCenter(XLText);
		}

		// ����������� TRAP2�϶��� �ΰ��� ���� ����ش�..
		XLText->AddFormatText("$s+07$e$h %d)$r��� ��������$n", nIndexSmall++);
		if(bTrap2 || bArch) XLText->AddFormatText("$e$h$r%s$r%s$n$h", GetNumberString(1,NUM_CIRCLE), (i==0)? "���� ��ü" : "���� ��ü");
		MakeTableLoadSum(XLText, i, FALSE, FALSE, 0, FALSE);
		if(bTrap2 || bArch)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			if(bTrap2) XLText->AddFormatText("$e$h$r%s$r��米��$n$h", GetNumberString(2,NUM_CIRCLE));
			else if(bArch) XLText->AddFormatText("$e$h$r%s$r��ġ ����$n$h", GetNumberString(2,NUM_CIRCLE));
			MakeTableLoadSum(XLText, i, FALSE, FALSE, 0, TRUE);
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+14$e$h %d)$r��Ÿ �ۿ�����$n", nIndexSmall++);
		// ���� ��Ÿ �ۿ�����
		pData = m_pListLoadUlt[i].GetAt(0);
		pData->MakeTableOtherLoad(XLText);
		// ������ ��Ÿ �ۿ�����
		pData = m_pListLoadUlt[i].GetAt(3);
		pData->MakeTableOtherLoad(XLText);
		*XLText += 1;

		if(bTrap2 || bArch)
		{
			XLText->AddFormatText("$h$r$c*$r���ʿ� �ۿ��ϴ� ������ ��ü�ϴܰ� %s�� �ۿ��ϴ� ���/���Ʈ��$n", bTrap2? "��米���ϴ�" : "���������");
			XLText->AddFormatText("$h$r$r�ջ��Ͽ� ���� ū ���� �ش��ϴ� Combo�� �ܸ���� ���Ͽ���.$n");
		}

		if(pBri->IsPileBase(i))				// ���ұ��� �����϶��� ���ҵ��� �ۿ���� ���ϱ� ���ؼ�..
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+16$e$h %d)$r���� �������ۿ�� ����$n", nIndexSmall++);
			int j=0; for(j=0; j<4; j++)
			{
				pData = m_pListLoadUlt[i].GetAt(j);
				pData->MakeTableLoadAll(XLText, j);
				pData->WriteForceCenter_Pile(XLText);
			}
			
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+99$e$h %d)$r���� �����ۿ�� ����$n", nIndexSmall++);
			XLText->AddFormatText("$e$h$r%s$r���ҵ��� ���$n$h", GetNumberString(1,NUM_CIRCLE));
			MakeTableCenterPile(XLText, i);
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$e$h$r%s$r���ҵ��� �ۿ��$n$h", GetNumberString(2,NUM_CIRCLE));
			MakeTableLoadSum(XLText, i, FALSE, FALSE, 1);
			*XLText += 1;
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+99$e$h %d)$r������ �ܸ�� ����$n", nIndexSmall++);
		bFirst = TRUE;
		if(pBri->IsPileBase(i))
		{
			XLText->AddFormatText("$e$h$m+16[16.���Ұ��� ����]$n");
			XLText->GoPosition(NO_MOVE,COL_POS_1);
		}
		else
		{
			*XLText += 1;
		}
		int j=0; 
		for(j=0; j<4; j++)
		{
			if(m_pListLoadUlt[i].GetSize() <= j) continue;
			pData = m_pListLoadUlt[i].GetAt(j);
	
			if(!pBri->IsPileBase(i))
			{
				pData->MakeTableLoadAll(XLText, j);
				pData->WriteSectionDesign(XLText, FALSE, bFirst);
				bFirst = FALSE;
			}
			else
			{
				pData->MakeTableLoadAll(XLText, j, FALSE);
				pData->WriteSectionDesign_Pile(XLText, FALSE, bFirst);
				bFirst = FALSE;
			}
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+99$e$h %d)$r������ �����Ʈ ����$n", nIndexSmall++);
		*XLText += 1;
		bFirst = TRUE;
		for(j=0; j<4; j++)
		{
			if(m_pListLoadUse[i].GetSize() <= j) continue;
			pData = m_pListLoadUse[i].GetAt(j);

			if(!pBri->IsPileBase(i))
			{
				pData->MakeTableLoadAll(XLText, j, FALSE);
				pData->WriteSectionDesign(XLText, TRUE, bFirst);
				bFirst = FALSE;
			}
			else
			{
				pData->MakeTableLoadAll(XLText, j, FALSE);
				pData->WriteSectionDesign_Pile(XLText, TRUE, bFirst);
				bFirst = FALSE;
			}
		}


		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+17$e$h %d)$r���ʼ��� ����� SUMMARY$n", nIndexSmall++);
		MakeTableSectionForceSum(XLText, i);

		//////////////////////////////////////////////////////////////////////////////////
		// ���� �ܸ� ����
		// �߰������δ� �ޱ��� ��� �ϸ� �ϸ������϶� ����° ���ڸ� TRUE�� �ش�..
		long nIndexSub =1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h %d)$r�ܸ����$n", nIndexSmall++);
		CConcBasic *pConc = pBri->GetConcDataFooting(i, TRUE, FALSE, -1);
		if(pConc != NULL && pConc->m_Moment !=0)
		{
			XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//			pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
			pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
		}

		pConc = pBri->GetConcDataFooting(i, TRUE, TRUE);
		if(pConc != NULL && pConc->m_Moment !=0)
		{
			XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//			pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
			pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
		}

		if((bTrap2 || bArch) && !bInWall)
		{
			//// �ձ�2
			CConcBasic *pConc = pBri->GetConcDataFooting(i, TRUE, FALSE, 0);
			if(pConc != NULL && pConc->m_Moment !=0)
			{
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//				pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
				pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
			}
			
			pConc = pBri->GetConcDataFooting(i, TRUE, TRUE, 0);
			if(pConc != NULL && pConc->m_Moment !=0)
			{
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//				pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
				pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
			}

			if(pBri->m_bCheckFrontFootTrap2)
			{
				//// �ձ�3
				pConc = pBri->GetConcDataFooting(i, TRUE, FALSE, 1);
				if(pConc != NULL && pConc->m_Moment !=0)
				{
					XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
					CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
	//				pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
					pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
				}
				
				pConc = pBri->GetConcDataFooting(i, TRUE, TRUE, 1);
				if(pConc != NULL && pConc->m_Moment !=0)
				{
					XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
					CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
	//				pBeam.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
					pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
				}
			}
		}

		pConc = pBri->GetConcDataFooting(i, FALSE, (bInWall)? TRUE : FALSE);
		if(pConc != NULL && pConc->m_Moment !=0)
		{
			XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam2(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//			pBeam2.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
			pBeam2.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
		}
		
		pConc = pBri->GetConcDataFooting(i, FALSE, (bInWall)? FALSE : TRUE);
		if(pConc != NULL && pConc->m_Moment != 0)
		{
			XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam2(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
//			pBeam2.MakeOutputDesignExcelFile(XLText, 2, (pData->m_pFooting->m_rebarShearDetail.m_nType!=TYPE_SHEAR_SPACER),FALSE);
			pBeam2.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
		}


		////////////////////////////////////////////////////////////////////////////////////
		nIndexSub = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h %d)$r��뼺 ����$n", nIndexSmall++);

		pConc = pBri->GetConcDataFooting(i, TRUE, FALSE);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
			pBeam.MakeOutputCrackExcelFile(XLText, 2, FALSE);
		}
		
		pConc = pBri->GetConcDataFooting(i, TRUE, TRUE);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
			pBeam.MakeOutputCrackExcelFile(XLText, 2, FALSE);
		}
		
		pConc = pBri->GetConcDataFooting(i, FALSE, (bInWall)? TRUE : FALSE);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam2(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
			pBeam2.MakeOutputCrackExcelFile(XLText, 2, FALSE);
		}
		
		pConc = pBri->GetConcDataFooting(i, FALSE, (bInWall)? FALSE : TRUE);
		if(pConc != NULL && pConc->m_MomentUse != 0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			CRectBeam pBeam2(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
			pBeam2.MakeOutputCrackExcelFile(XLText, 2, FALSE);
		}

		////////////////////////////////////////////////////////////////////////////////////
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h $e%d)$r���ö�� ����$n$n", nIndexSmall++);

		nIndexSub = 0;
		WriteSupportCheckFooting(XLText, nIndexSub++, i, TRUE);
		WriteSupportCheckFooting(XLText, nIndexSub++, i, FALSE);
	}
}

void CARcBridgeCalcFooting::WriteCalcFootingDesignLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pDataStd->IsLsdDesign();
	if( bLsd == FALSE )
		return;

	BOOL bTrap = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP;
	BOOL bTrap2 = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	BOOL bArch	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	
	//	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	double dUwConc = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwAscon = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightAsphalt), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwBackFill = pDataStd->GetValueUnitType(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dwLiveAlways = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2);
	double dHeightPave = toM(pBri->m_dUnderGroundHeightPavement);
	if(pDataStd->IsRailDesign())
	{
		dHeightPave = 0;
	}

	const HGINT32 iDecM = pDataStd->GetDecimal(UNIT_CGS_M);
	const HGINT32 iDecTon_M2 = pDataStd->GetDecimal(UNIT_CGS_TONF_M2);
	const HGINT32 iDecTon_M3 = pDataStd->GetDecimal(UNIT_CGS_TONF_M3);

	// ��� ������ ������ �ϴ� Footing ���� ��ŭ �� ��´�.
	long nCntJijum = pBri->GetCountJijum();
	CLoadFooting *pData = NULL;
	for(HGINT32 ixJijum = 0; ixJijum < nCntJijum; ++ixJijum)
	{
		long nIndexSmall = 1;
		if((ixJijum == 0 || ixJijum == nCntJijum-1) && !pBri->IsOutsideWall()) continue;

		pData = GetLoadFootingLsd(ixJijum, eEnvLimit, 0);
		if(pData == NULL) continue;

		EPartOfBridge ePart = ixJijum == 0 ? ePartFootStt : ixJijum == nCntJijum - 1 ? ePartFootEnd : ePartInFoot;

		double dLaodFactor[VER_LOAD_COUNT] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
		dLaodFactor[VER_LOAD_LIVE] = pData->m_dFactorLiveLoad;
		dLaodFactor[VER_LOAD_FOOTW] = pData->m_dFactorDeadLoad;
		dLaodFactor[VER_LOAD_PAVE] = pData->m_dFactorPave;
		dLaodFactor[VER_LOAD_BACKFILL] = pData->m_dFactorSoilHor;
		dLaodFactor[VER_LOAD_DEAD] = pData->m_dFactorDeadLoad;

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		if(nIndexMiddle != 1) XLText->AddFormatText("$s+99");
		XLText->AddFormatText("$s+05$e$h$a%d.%d %s$n", nIndexTitle, nIndexMiddle++, pData->m_sTitle);

		CDomyun *pDom = new CDomyun;
		DrawFootingSection(pDom, pBri, ixJijum);
		long nRowDom = (long)MAX(MIN(toM(pDom->GetExtHeight())*3/2, 20), 10);
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		XLText->PushRow();

		BOOL bInWall = (!pBri->IsOutsideWall() || (ixJijum!=0 && ixJijum!=pBri->m_nQtyJigan));
		double wB = (pData->m_nJijum == pBri->GetCountJijum()-1)? toM(pData->m_pFooting->m_dFWR):toM(pData->m_pFooting->m_dFWL);
		double H = toM(pData->m_pFooting->GetHeight());

		for(long nLine=0; nLine < nRowDom-3; nLine++)
		{
			XLText->AddFormatText("$u");
		}

		if(Compare(dLaodFactor[VER_LOAD_FOOTW], 0.0, _T(">"), 0.001))
		{
			if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
				||(bTrap2 && (ixJijum==0 || ixJijum==pBri->m_nQtyJigan)) || bArch)
			{
				double dA = 0;
				if(bTrap)
				{
					double dW = toM(pData->m_pFooting->GetWidth());

					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRH) : toM(pData->m_pFooting->m_dFSLH);
					double dFEW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFERW) : toM(pData->m_pFooting->m_dFELW);
					double dFSW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRW) : toM(pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}
				else
				{
					double dW = toM(pData->m_pFooting->GetWidth());
					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = toM((ixJijum==0)? pData->m_pFooting->m_dFSRH : pData->m_pFooting->m_dFSLH);
					double dFEW = toM((ixJijum==0)? pData->m_pFooting->m_dFERW : pData->m_pFooting->m_dFELW);
					double dFSW = toM((ixJijum==0)? pData->m_pFooting->m_dFSRW : pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}			

				*XLText += 5;
				XLText->AddFormatText("$h$t$t$m+01��������$m+03$c=$r$e%s x %s$n", 
					pDataStd->GetStrDoubleUnitChange(dA, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_FOOTW]/dLaodFactor[VER_LOAD_FOOTW], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
			}
			else
			{
				XLText->AddFormatText("$h$t$t$m+01��������$m+03$c=$r$e%s x %s x %s$n", 
					pDataStd->GetStrDoubleUnitChange(toM(pData->m_pFooting->GetWidth()), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(H, UNIT_CGS_M, pDataStd->m_szM), 
					pDataStd->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n",
					pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_FOOTW]/dLaodFactor[VER_LOAD_FOOTW], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
			}
		}
		else
		{
			XLText->AddText("$h$t$t$m+01��������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
		}

		if(pData->m_nJijum==0 || pData->m_nJijum==pBri->GetCountJijum()-1)
		{
			if(Compare(dLaodFactor[VER_LOAD_BACKFILL], 0.0, _T(">"), 0.001))
			{
				XLText->AddFormatText("$h$t$t$m+01������$m+03$c=$r$e%s x %s x %s$n",
					pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(pData->m_dHightBackFill, UNIT_CGS_M),
					pDataStd->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n",
					pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_BACKFILL]/dLaodFactor[VER_LOAD_BACKFILL], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
			}
			else
			{
				XLText->AddText("$h$t$t$m+01������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
			}

			if(pBri->m_bUnderGroundRahmen)
			{
				CDPoint xyStt(0, 0);
				xyStt.x	= pBri->GetStationOnJijum(pData->m_nJijum, 0);
				if(pData->m_nJijum==0) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, -pBri->m_dWS, 0).x;
				else xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, pBri->m_dWE, 0).x;

				double dElBackFill = pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y;
				double dLiveAlwyas = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pDataStd->m_szTonf_M2);

				if(Compare(dLaodFactor[VER_LOAD_LIVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+01����Ȱ����$m+03$c=$r$e%s x %s$n",
						pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dLiveAlwyas, UNIT_CGS_TONF_M2));
					XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_LIVE]/dLaodFactor[VER_LOAD_LIVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+01����Ȱ����$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}

				if(Compare(dLaodFactor[VER_LOAD_PAVE], 0.0, _T(">"), 0.001))
				{
					BOOL bOn = TRUE;
					if(pBri->m_bUnderGroundPaveDetail)
					{
						XLText->AddFormatText("$h$t$t$m+01�� ��$m+03$c=$r");

						double dTickPave = 0;
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightPyo > 0)	// ǥ��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightPyo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightPyo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05(%s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightGee > 0)	// ����
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightGee < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightGee;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05(%s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightMiddle > 0)	// �߰���
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightMiddle < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightMiddle;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05 %s x %s$m+04", 
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightBozo > 0)	// ��������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightBozo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightBozo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightDong > 0)	// ���������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightDong < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightDong;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightEps > 0)	// EPS��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightEps < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightEps;

							if(bOn) XLText->AddFormatText("$h$t$t$m+05 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->GetUnderGroundPaveHeightTosa() > 0)	// ���
						{
							if(dElBackFill - pBri->GetUnderGroundPaveHeightTosa()< pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->GetUnderGroundPaveHeightTosa();

							if(bOn) XLText->AddFormatText("$h$t$t$m+05 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n", 
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							//dElBackFill -= dTickPave;
							bOn = !bOn;
						}
					}
					else if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						bOn = TRUE;
						XLText->AddFormatText("$h$t$t$m+01�� ��$m+03$c=$r(%s x %s$m+04$c+$r%s + %s$n",
							pDataStd->GetStrDoubleUnitChange(dHeightPave, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble((pBri->m_nTypePavement==0)? dUwAscon:dUwConc, UNIT_CGS_TONF_M3),
							pDataStd->GetStrDoubleUnitChange(toM(pBri->m_dUnderGroundHeightMortar), UNIT_CGS_M, pDataStd->m_szM),
							pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
						//					XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r(%.3f x %.3f + %.3f + %.3f)$n", toM(pBri->m_dUnderGroundHeightPavement), (pBri->m_nTypePavement==0)? dUwAscon:dUwConc, toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+09$f3%.3f$me1$m+02x$r$f3%.3f$me1$r)$n", toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+05$c=$r x %.3f$n", wB);
					}

					if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						if(bOn) XLText->AddFormatText("$h$t$t$m+05) x %s$n", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM));
						else XLText->AddFormatText("$e)$r$c$me1x %s$n", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM));

						XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_PAVE]/dLaodFactor[VER_LOAD_PAVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
					}
				}
				else
				{
					XLText->AddText("$h$t$t$m+01�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(pBri->m_nSelectModelingHeight != 0)
					XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");

				*XLText  += nRowDom-10;
			}
			else if(pBri->m_dTP > 0)
			{
				if(Compare(dLaodFactor[VER_LOAD_PAVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+01�� ��$m+03$c=$r$e%s x %s x %s$n",
						pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toM(pBri->m_dTP), UNIT_CGS_M, pDataStd->m_szM),
						pDataStd->GetStrDouble(dUwAscon, UNIT_CGS_TONF_M3));
					XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_PAVE]/dLaodFactor[VER_LOAD_PAVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+01�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(Compare(dLaodFactor[VER_LOAD_LIVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+01%s$m+03$c=$r$e%s x %s$n",
						bLsd ? "��ä���������" : "���Ȱ����", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dwLiveAlways, UNIT_CGS_TONF_M2));
					XLText->AddFormatText("$h$t$t$m+01$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_LIVE]/dLaodFactor[VER_LOAD_LIVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddFormatText("$h$t$t$m+01%s$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n", bLsd ? "��ä���������" : "���Ȱ����");
				}
				

				if(pBri->m_nSelectModelingHeight != 0)
					XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");

				*XLText  += nRowDom-12;
			}
		}
		else 
		{
			if(pBri->m_nSelectModelingHeight != 0)
				XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
			*XLText  += nRowDom-6;
		}
		XLText->PopRow();
		*XLText  += 1;

		// ��米���� �����϶��� ������ ���ɹ� ���� ���� �ٰ� Table�� ����Ѵ�.
		if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
			||(bTrap2 && (pData->m_nJijum==0 || pData->m_nJijum==pBri->m_nQtyJigan)) || bArch)
		{
			*XLText += 3;
			XLText->AddFormatText("$u$u$u$u$u");
			pData->MakeTableFootCenter(XLText);
		}

		CConcBasic *pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE);
		if (pConc)
		{
			if(pConc->m_strDesignDegree == _T("A"))
			{
				XLText->AddFormatText(_T("$h$r������ A�� ��տ� ���� �� �տ��� �Ѱ���� �������� �ƴϹǷ� �տ� ���並 �������� �ʽ��ϴ�.$n$h"));
			}
			else
			{
				CString sDesign= _T(""), sKind=_T(""), sExtra = _T("");
				double dCrackWitdh = pBri->GetCrackLimitWidth_LSD(ePart, FALSE, sKind, sDesign);

				BOOL bUseApsCrack = pBri->IsUseSelectLoadCaseWingOrApsMatchDesignRating(pConc->m_strDesignDegree ,ePart);
				if(pBri->IsCheckPosEtaUseLoad(ePart) == FALSE || bUseApsCrack == FALSE)
				{
					XLText->AddFormatText(_T("$h �� ���� ȯ�濡 ���� ���� ���� ��� : %s %s$n$h"), sDesign, sExtra);
					XLText->AddFormatText(_T("$h������� ���տ��� �Ѱ���� ������ ���� ���������� �̰����Ͽ����Ƿ� ��뼺 ����� �������� �ʴ´�.$n$n$h"));
				}
				else if(sDesign != _T("A"))
					XLText->AddFormatText(_T("$h$r ������ %s �̹Ƿ� �տ��� �Ѱ���´� %s, ǥ�� �Ѱ�տ����� %.1fmm�� ����.$n$h"), sDesign,sKind,dCrackWitdh );
			}
		}
		
		// ����������� TRAP2�϶��� �ΰ��� ���� ����ش�..
		XLText->AddFormatText("$s+07$e$h %d)$r��� ��������$n", nIndexSmall++);
		if(bTrap2 || bArch) XLText->AddFormatText("$e$h$r%s$r%s$n$h", GetNumberString(1,NUM_CIRCLE), (ixJijum==0)? "���� ��ü" : "���� ��ü");
			
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		MakeTableLoadForFooting_Lsd(XLText, pBri, pData->m_nJijum, FALSE, TRUE, TRUE);

		if(bTrap2 || bArch)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			if(bTrap2) XLText->AddFormatText("$e$h$r%s$r��米��$n$h", GetNumberString(2,NUM_CIRCLE));
			else if(bArch) XLText->AddFormatText("$e$h$r%s$r��ġ ����$n$h", GetNumberString(2,NUM_CIRCLE));
				
			MakeTableLoadForFooting_Lsd(XLText, pBri, pData->m_nJijum, TRUE, TRUE, TRUE);
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+14$e$h %d)$r��Ÿ �ۿ�����$n", nIndexSmall++);
		// ���� ��Ÿ �ۿ�����
		pData = GetLoadFootingLsd(ixJijum, eEnvLimit, 0);
		if(pData != NULL)
		{
			pData->MakeTableOtherLoadLsd(XLText);
		}

		// ������ ��Ÿ �ۿ�����
		pData = NULL;
		if(pBri->IsValidComboEnvType(eEnvExtremeI))
			pData = GetLoadFootingLsd(ixJijum, eEnvExtremeI, 0);
		else if(pBri->IsValidComboEnvType(eEnvExtremeII))
			pData = GetLoadFootingLsd(ixJijum, eEnvExtremeII, 0);

		if(pData != NULL)
		{
			pData->MakeTableOtherLoadLsd(XLText);
		}
		pData = GetLoadFootingLsd(ixJijum, eEnvLimit, 0);
		
		*XLText += 1;

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+14$e$h %d)$r��������$n", nIndexSmall++);

		long nIndexCircle = 1;
		m_pStd->m_pLoad->MakeTableOutValueEta_Lsd(XLText, 3, nIndexCircle++);

		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(nIndexCircle++, NUM_CIRCLE), "��������");

		MakeTableLoadCombo(XLText, pBri, pData->m_nJijum, FALSE);

		if(bTrap2 || bArch)
		{
			XLText->AddFormatText("$h$r$c*$r���ʿ� �ۿ��ϴ� ������ ��ü�ϴܰ� %s�� �ۿ��ϴ� ���/���Ʈ��$n", bTrap2? "��米���ϴ�" : "���������");
			XLText->AddFormatText("$h$r$r�ջ��Ͽ� ���� ū ���� �ش��ϴ� Combo�� �ܸ���� ���Ͽ���.$n");
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$$e$h %d)$r�����ۿ�� ����$n", nIndexSmall++);

		nIndexCircle = 0;
		for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
		{
			if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
				continue;

			for (HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
			{
				pData = GetLoadFootingLsd(ixJijum, (ETypeEnvComboLsd)ix, jx);
				if(pData == NULL) continue;

				pData->MakeTableLoadAll(XLText, nIndexCircle++);
			}
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$$e$h %d)$r�����ۿ�� ����$n", nIndexSmall++);
		MakeTableLoadFooting(XLText, pBri, ixJijum, FALSE);


		if(pBri->IsPileBase(ixJijum))				// ���ұ��� �����϶��� ���ҵ��� �ۿ���� ���ϱ� ���ؼ�..
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+99$e$h %d)$r���ҵ��ɿ����� �ۿ����� ����$n", nIndexSmall++);

			XLText->AddFormatText("$e$h$r%s$r���ҵ����� �ۿ����� ���$n$h", GetNumberString(1,NUM_CIRCLE));

			HGDOUBLE dDosim = GetCenterOfFigure(pBri, ixJijum);
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddText(_T("$h��M = ��Mr - ��Mo$n"));
			XLText->AddFormatText(_T("$h e = %s( = %s) - ��M / ��V$n"), "���ҵ���", pDataStd->GetStrDoubleUnitChange(dDosim, UNIT_CGS_M, pDataStd->m_szM));
			XLText->AddText(_T("$hMc = V x e$n$n"));

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$e$h$r%s$r���ҵ����� ���$n$h", "��");
			MakeTableCenterPile(XLText, ixJijum);
			
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$e$h$r%s$r�������պ� �ۿ��$n$h", GetNumberString(2,NUM_CIRCLE));
			MakeTableCenterPileLoad(XLText, pBri, ixJijum);
			*XLText += 1;
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+99$e$h %d)$r������ �ܸ�� ����$n", nIndexSmall++);
		if(pBri->IsPileBase(ixJijum))
		{
			XLText->AddFormatText("$e$h$m+16[16.���Ұ��� ����]$n");
			XLText->GoPosition(NO_MOVE,COL_POS_1);
		}
		else
		{
			*XLText += 1;
		}

		nIndexCircle = 1;
		if(!pBri->IsPileBase(ixJijum))
		{
			MakeTableLoadSubgradeReaction(XLText, pBri, ixJijum, nIndexCircle++);
			WriteSectionDesign_Lsd(XLText, pBri, ixJijum, nIndexCircle++);
		}
		else
		{
			WriteSectionDesign_Pile(XLText, pBri, ixJijum, nIndexCircle++);
		}
		
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+17$e$h %d)$r���ʼ��� ����� SUMMARY$n", nIndexSmall++);
		MakeTableSectionForceSum_Lsd(XLText, pBri, ixJijum);

		//////////////////////////////////////////////////////////////////////////////////
		// ���� �ܸ� ����
		// �߰������δ� �ޱ��� ��� �ϸ� �ϸ������϶� ����° ���ڸ� TRUE�� �ش�..
		long nIndexSub =1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h %d)$r�ܸ����$n", nIndexSmall++);
		
		for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
		{
			pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
			if(pConc != NULL && pConc->m_Moment !=0)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);

				long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

				CLsdBeam pBeam(pConc, nTypeStatus);			
				pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
			}
		}
		
		for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
		{
			pConc = pBri->GetConcDataFooting(ixJijum, TRUE, TRUE, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
			if(pConc != NULL && pConc->m_Moment !=0)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);

				long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
				
				CLsdBeam pBeam(pConc, nTypeStatus);			
				pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
			}
		}

		if((bTrap2 || bArch) && !bInWall)
		{
			//// �ձ�2
			for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
			{
				CConcBasic *pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE, 0, static_cast<ETypeEnvComboLsd>(ixEnvType));
				if(pConc != NULL && pConc->m_Moment !=0)
				{
					XLText->GoPosition(NO_MOVE,COL_POS_1);

					long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
					XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
					
					CLsdBeam pBeam(pConc, nTypeStatus);			
					pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
				}
			}

			for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
			{
				pConc = pBri->GetConcDataFooting(ixJijum, TRUE, TRUE, 0, static_cast<ETypeEnvComboLsd>(ixEnvType));
				if(pConc != NULL && pConc->m_Moment !=0)
				{
					XLText->GoPosition(NO_MOVE,COL_POS_1);

					long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
					XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

					CLsdBeam pBeam(pConc, nTypeStatus);
					pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
				}
			}

			if(pBri->m_bCheckFrontFootTrap2)
			{
				//// �ձ�3
				for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
				{
					pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE, 1, static_cast<ETypeEnvComboLsd>(ixEnvType));
					if(pConc != NULL && pConc->m_Moment !=0)
					{
						XLText->GoPosition(NO_MOVE,COL_POS_1);

						long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
						XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

						CLsdBeam pBeam(pConc, nTypeStatus);
						pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
					}
				}
				
				for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
				{
					pConc = pBri->GetConcDataFooting(ixJijum, TRUE, TRUE, 1, static_cast<ETypeEnvComboLsd>(ixEnvType));
					if(pConc != NULL && pConc->m_Moment !=0)
					{
						XLText->GoPosition(NO_MOVE,COL_POS_1);

						long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
						XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

						CLsdBeam pBeam(pConc, nTypeStatus);
						pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
					}
				}
			}
		}
		
		for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
		{
			pConc = pBri->GetConcDataFooting(ixJijum, FALSE, (bInWall)? TRUE : FALSE, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
			if(pConc != NULL && pConc->m_Moment !=0)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);

				long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

				CLsdBeam pBeam(pConc, nTypeStatus);
				pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
			}
		}

		
		for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
		{
			pConc = pBri->GetConcDataFooting(ixJijum, FALSE, (bInWall)? FALSE : TRUE, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
			if(pConc != NULL && pConc->m_Moment != 0)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);

				long nTypeStatus = ixEnvType == eEnvLimit ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN;
				XLText->AddFormatText("$s+40$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);

				CLsdBeam pBeam(pConc, nTypeStatus);
				pBeam.MakeOutputDesignExcelFile(XLText, COL_POS_3, TRUE,FALSE);
			}
		}


		////////////////////////////////////////////////////////////////////////////////////
		nIndexSub = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h %d)$r��뼺 ����$n", nIndexSmall++);

		pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE, -1, eEnvService);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			
			CLsdBeam LsdBeam(pConc, LSD_ULS_DESIGN);
			LsdBeam.CalculateUsability();
			LsdBeam.MakeOutputUsabilityExcelFile(XLText, COL_POS_3, FALSE);
		}

		pConc = pBri->GetConcDataFooting(ixJijum, TRUE, TRUE, -1, eEnvService);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			
			CLsdBeam LsdBeam(pConc, LSD_ULS_DESIGN);
			LsdBeam.CalculateUsability();
			LsdBeam.MakeOutputUsabilityExcelFile(XLText, COL_POS_3, FALSE);
		}

		pConc = pBri->GetConcDataFooting(ixJijum, FALSE, (bInWall)? TRUE : FALSE, -1, eEnvService);
		if(pConc != NULL && pConc->m_MomentUse !=0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �������$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			
			CLsdBeam LsdBeam(pConc, LSD_ULS_DESIGN);
			LsdBeam.CalculateUsability();
			LsdBeam.MakeOutputUsabilityExcelFile(XLText, COL_POS_3, FALSE);
		}

		pConc = pBri->GetConcDataFooting(ixJijum, FALSE, (bInWall)? FALSE : TRUE, -1, eEnvService);
		if(pConc != NULL && pConc->m_MomentUse != 0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$h$r$e%s$r%s - �ϸ�����$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle);
			
			CLsdBeam LsdBeam(pConc, LSD_ULS_DESIGN);
			LsdBeam.CalculateUsability();
			LsdBeam.MakeOutputUsabilityExcelFile(XLText, COL_POS_3, FALSE);
		}

		////////////////////////////////////////////////////////////////////////////////////
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$e$h $e%d)$r���ö�� ����$n$n", nIndexSmall++);

		nIndexSub = 0;

		pConc = pBri->GetConcDataFooting(ixJijum, TRUE, FALSE);
		if(pConc)
		{
			XLText->AddFormatText("$s+12$h$r$e%s$r%s$n", GetNumberString(++nIndexSub,NUM_CIRCLE), pConc->m_sTitle);
			
			CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
			pBeam.MakeOutputDryingShrinkageExcelFile(XLText, COL_POS_3, nIndexSub);
			*XLText += 1;
		}

		pConc = pBri->GetConcDataFooting(ixJijum, FALSE, FALSE);
		if(pConc)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+12$h$r$e%s$r%s$n", GetNumberString(++nIndexSub,NUM_CIRCLE), pConc->m_sTitle);

			CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
			pBeam.MakeOutputDryingShrinkageExcelFile(XLText, COL_POS_3, nIndexSub++);
			*XLText += 1;
		}
	}
}

void CARcBridgeCalcFooting::WriteCalcFootingSafeCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pDataStd->IsLsdDesign();
	BOOL bTrap = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP;
	BOOL bTrap2 = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	BOOL bArch	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	BOOL bFirst = TRUE;
//	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	
	double dUwConc = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwAscon = pDataStd->GetValueUnitType(toTon(pDataStd->m_pBasicConcInfo->m_UWeightAsphalt), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dUwBackFill = pDataStd->GetValueUnitType(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dwLiveAlways = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2);
	double dHeightPave = toM(pBri->m_dUnderGroundHeightPavement);
	if(pDataStd->IsRailDesign())
	{
		dHeightPave = 0;
	}

	const HGINT32 iDecM = pDataStd->GetDecimal(UNIT_CGS_M);
	const HGINT32 iDecTon_M2 = pDataStd->GetDecimal(UNIT_CGS_TONF_M2);
	const HGINT32 iDecTon_M3 = pDataStd->GetDecimal(UNIT_CGS_TONF_M3);
	
	long nCntJijum = pBri->GetCountJijum();
	CLoadFooting *pData = NULL;
	
	int i=0; for(i=0; i<nCntJijum; i++)
	{
		long nIndexSmall = 1;
		if((i==0 || i== nCntJijum-1) && !pBri->IsOutsideWall()) continue;
		if (bLsd)
		{
			if (GetCountLoadFootingLsd(i, eEnvLimit) == 0)
				continue;
		}
		else
		{
			if(m_pListLoadSafty[i].GetSize() == 0 || m_pListLoadUlt[i].GetSize() == 0) continue;
		}
		
		// Lsd �϶��� ���������� ������� ���� ������ �ǹǷ� ���հ���� �����ش�.
		double dLaodFactor[VER_LOAD_COUNT] = { 1.0, 1.0, 1.0, 1.0, 1.0 };
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		if (bLsd)
		{
			pData = GetLoadFootingLsd(i, eEnvLimit, 0);
			if(pData == NULL) continue;

			dLaodFactor[VER_LOAD_LIVE] = pData->m_dFactorLiveLoad;
			dLaodFactor[VER_LOAD_FOOTW] = pData->m_dFactorDeadLoad;
			dLaodFactor[VER_LOAD_PAVE] = pData->m_dFactorPave;
			dLaodFactor[VER_LOAD_BACKFILL] = pData->m_dFactorSoilHor;
			dLaodFactor[VER_LOAD_DEAD] = pData->m_dFactorDeadLoad;
		}
		else
		{
			pData = m_pListLoadSafty[i].GetAt(0);
		}
		
		if(nIndexMiddle != 1) XLText->AddFormatText("$s+99");
		XLText->AddFormatText("$s+05$e$h$a%d.%d %s$n", nIndexTitle, nIndexMiddle++, pData->m_sTitle);
		
		CDomyun *pDom = new CDomyun;
		DrawFootingSection(pDom, pBri, i);
		long nRowDom = (long)MAX(MIN(toM(pDom->GetExtHeight())*3/2, 20), 10);  
		XLText->InsertDomyun(pDom,m_pStd->m_pView, nRowDom,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		XLText->PushRow();
		
		double wB = (pData->m_nJijum == pBri->GetCountJijum()-1)? toM(pData->m_pFooting->m_dFWR):toM(pData->m_pFooting->m_dFWL);
		double H = toM(pData->m_pFooting->GetHeight());
		
		for(long nLine=0; nLine < nRowDom-3; nLine++)
		{
			XLText->AddFormatText("$u");
		}
		if(Compare(dLaodFactor[VER_LOAD_FOOTW], 0.0, _T(">"), 0.001))
		{
			if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
				||(bTrap2 && (i==0 || i==pBri->m_nQtyJigan)) || bArch)
			{
				double dA = 0;
				if(bTrap)
				{
					double dW = toM(pData->m_pFooting->GetWidth());

					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRH) : toM(pData->m_pFooting->m_dFSLH);
					double dFEW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFERW) : toM(pData->m_pFooting->m_dFELW);
					double dFSW = (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(pData->m_pFooting->m_dFSRW) : toM(pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}
				else
				{
					double dW = toM(pData->m_pFooting->GetWidth());
					double dFH = toM(pData->m_pFooting->GetHeight(FALSE));
					double dFSH = toM((i==0)? pData->m_pFooting->m_dFSRH : pData->m_pFooting->m_dFSLH);
					double dFEW = toM((i==0)? pData->m_pFooting->m_dFERW : pData->m_pFooting->m_dFELW);
					double dFSW = toM((i==0)? pData->m_pFooting->m_dFSRW : pData->m_pFooting->m_dFSLW);

					dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
				}			

				*XLText += 5;

				XLText->AddFormatText("$h$t$t$m+02��������$m+03$c=$r$e%s x %s$n", 
					pDataStd->GetStrDoubleUnitChange(dA, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_FOOTW]/dLaodFactor[VER_LOAD_FOOTW], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);			
			}
			else
			{
				XLText->AddFormatText("$h$t$t$m+02��������$m+03$c=$r$e%s x %s x %s$n", 
					pDataStd->GetStrDoubleUnitChange(toM(pData->m_pFooting->GetWidth()), UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(H, UNIT_CGS_M, pDataStd->m_szM), 
					pDataStd->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n",
					pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_FOOTW]/dLaodFactor[VER_LOAD_FOOTW], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
			}
		}
		else
		{
			XLText->AddText("$h$t$t$m+02��������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
		}

		if(pData->m_nJijum==0 || pData->m_nJijum==pBri->GetCountJijum()-1)
		{
			if(Compare(dLaodFactor[VER_LOAD_BACKFILL], 0.0, _T(">"), 0.001))
			{
				XLText->AddFormatText("$h$t$t$m+02������$m+03$c=$r$e%s x %s x %s$n",
					pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(pData->m_dHightBackFill, UNIT_CGS_M),
					pDataStd->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n",
					pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_BACKFILL]/dLaodFactor[VER_LOAD_BACKFILL], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
			}
			else
			{
				XLText->AddText("$h$t$t$m+02������$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
			}
			
			if(pBri->m_bUnderGroundRahmen)
			{
				CDPoint xyStt(0, 0);
				xyStt.x	= pBri->GetStationOnJijum(pData->m_nJijum, 0);
				if(pData->m_nJijum==0) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, -pBri->m_dWS, 0).x;
				else xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, pBri->m_dWE, 0).x;

				double dElBackFill = pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y;
				double dLiveAlwyas = pDataStd->GetValueUnitType(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pDataStd->m_szTonf_M2);

				if(Compare(dLaodFactor[VER_LOAD_LIVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02����Ȱ����$m+03$c=$r$e%s x %s$n",
						pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dLiveAlwyas, UNIT_CGS_TONF_M2));
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_LIVE]/dLaodFactor[VER_LOAD_LIVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+02����Ȱ����$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(Compare(dLaodFactor[VER_LOAD_PAVE], 0.0, _T(">"), 0.001))
				{
					BOOL bOn = TRUE;
					if(pBri->m_bUnderGroundPaveDetail)
					{
						XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r");

						double dTickPave = 0;
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightPyo > 0)	// ǥ��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightPyo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightPyo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06(%s x %s$m+04", 
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightGee > 0)	// ����
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightGee < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightGee;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06(%s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightMiddle > 0)	// �߰���
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightMiddle < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightMiddle;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %s x %s$m+04", 
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightBozo > 0)	// ��������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightBozo < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightBozo;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightDong > 0)	// ���������
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightDong < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightDong;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->m_dUnderGroundPaveHeightEps > 0)	// EPS��
						{
							if(dElBackFill - pBri->m_dUnderGroundPaveHeightEps < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->m_dUnderGroundPaveHeightEps;

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM), 
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							dElBackFill -= dTickPave;
							bOn = !bOn;
						}
						if(dElBackFill > pData->m_pFooting->m_dEL && pBri->GetUnderGroundPaveHeightTosa() > 0)	// ���
						{
							if(dElBackFill - pBri->GetUnderGroundPaveHeightTosa() < pData->m_pFooting->m_dEL) dTickPave = (dElBackFill - pData->m_pFooting->m_dEL);
							else dTickPave = pBri->GetUnderGroundPaveHeightTosa();

							if(bOn) XLText->AddFormatText("$h$t$t$m+06 %s x %s$m+04",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							else XLText->AddFormatText("$c+$r%s x %s$n",
								pDataStd->GetStrDoubleUnitChange(toM(dTickPave), UNIT_CGS_M, pDataStd->m_szM),
								pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
							//dElBackFill -= dTickPave;
							bOn = !bOn;
						}
					}
					else if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						bOn = TRUE;
						XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r(%s x %s$m+04$c+$r%s x %s$n",
							pDataStd->GetStrDoubleUnitChange(dHeightPave, UNIT_CGS_M, pDataStd->m_szM),
							pDataStd->GetStrDouble((pBri->m_nTypePavement==0)? dUwAscon:dUwConc, UNIT_CGS_TONF_M3),
							pDataStd->GetStrDoubleUnitChange(toM(pBri->m_dUnderGroundHeightMortar), UNIT_CGS_M, pDataStd->m_szM),
							pDataStd->GetStrDoubleUnitChange(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3));
						//					XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r(%.3f x %.3f + %.3f + %.3f)$n", toM(pBri->m_dUnderGroundHeightPavement), (pBri->m_nTypePavement==0)? dUwAscon:dUwConc, toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+09$f3%.3f$me1$m+02x$r$f3%.3f$me1$r)$n", toM(pBri->m_dUnderGroundHeightMortar), pDataStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_TONF_M2, pDataStd->m_szTonf_M2));
						//					XLText->AddFormatText("$h$t$t$m+05$c=$r x %.3f$n", wB);
					}

					if(pData->m_dVerLoad[VER_LOAD_PAVE] > 0)
					{
						if(bOn) XLText->AddFormatText("$h$t$t$m+06) x %s$n", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM));
						else XLText->AddFormatText("$e)$r$c%sx %s$n", bLsd ? "$me1" : "", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM));

						XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_PAVE]/dLaodFactor[VER_LOAD_PAVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
					}
				}
				else
				{
					XLText->AddText("$h$t$t$m+02�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}
				
				if(pBri->m_nSelectModelingHeight != 0)
					XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");

				*XLText += nRowDom-10;
			}
			else if(pBri->m_dTP > 0)
			{
				if(Compare(dLaodFactor[VER_LOAD_PAVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02�� ��$m+03$c=$r$e%s x %s x %s$n",
						pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDoubleUnitChange(toM(pBri->m_dTP), UNIT_CGS_M, pDataStd->m_szM),
						pDataStd->GetStrDouble(dUwAscon, UNIT_CGS_TONF_M3));
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e%s$me2$m+03%s$n", pDataStd->GetStrDouble(pData->m_dVerLoad[VER_LOAD_PAVE]/dLaodFactor[VER_LOAD_PAVE], UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddText("$h$t$t$m+02�� ��$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n");
				}

				if(Compare(dLaodFactor[VER_LOAD_LIVE], 0.0, _T(">"), 0.001))
				{
					XLText->AddFormatText("$h$t$t$m+02%s$m+03$c=$r$e%s x %s$n",
						bLsd ? "��ä���������" : "���Ȱ����", pDataStd->GetStrDoubleUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM), pDataStd->GetStrDouble(dwLiveAlways, UNIT_CGS_TONF_M2));
					// XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%.3f$me2$m+03%s$n", pData->m_dVerLoad[VER_LOAD_LIVE], pDataStd->m_szTonf_M);
					// �������� ����� ���� ���� �ȳ��ö��� �����Ƿ� ���Ȱ��� �����ʰ� �ٽ� ����� ����.
					XLText->AddFormatText("$h$t$t$m+02$m+03$c=$r$e$f3%s$me2$m+03%s$n",
						pDataStd->GetStrDouble(GetValueUnitChange(wB, UNIT_CGS_M, pDataStd->m_szM) * dwLiveAlways, UNIT_CGS_TONF_M), pDataStd->m_szTonf_M);
				}
				else
				{
					XLText->AddFormatText("$h$t$t$m+02%s$m+03$c=$r�������� ���ÿ��� ���� �Ǿ����ϴ�.$n$n", bLsd ? "��ä���������" : "���Ȱ����");
				}
				

				if(pBri->m_nSelectModelingHeight != 0)
					XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
				*XLText += nRowDom-12;
			}
		}
		else
		{
			if(pBri->m_nSelectModelingHeight != 0)
				XLText->AddFormatText("$n$h$t$t$�� �� �𵵴� �����߽� �������� �ۼ��� �Ϲݵ� �Դϴ�.$n");
			*XLText += nRowDom-6;
		}

		XLText->PopRow();
		*XLText += 1;

		// ��米���� �����϶��� ������ ���ɹ� ���� ���� �ٰ� Table�� ����Ѵ�.
		if((bTrap && (pData->m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || pData->m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
			||(bTrap2 && (pData->m_nJijum==0 || pData->m_nJijum==pBri->m_nQtyJigan)) || bArch)
		{
			*XLText += 3;
			XLText->AddFormatText("$u$u$u$u$u");
			pData->MakeTableFootCenter(XLText);
		}	
		
		// ����������� TRAP2�϶��� �ΰ��� ���� ����ش�..
		XLText->AddFormatText("$s+07$e$h %d)$r��� ��������$n", nIndexSmall++);
		if(bTrap2 || bArch) XLText->AddFormatText("$e$h$r%s$r%s$n$h", GetNumberString(1,NUM_CIRCLE), (i==0)? "���� ��ü" : "���� ��ü");
		if (bLsd)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			MakeTableLoadForFooting_Lsd(XLText, pBri, pData->m_nJijum, FALSE, FALSE, TRUE);
		}
		else
		{
			MakeTableLoadSum(XLText, i, TRUE, TRUE);
		}
		
		if(bTrap2 || bArch)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			if(bTrap2) XLText->AddFormatText("$e$h$r%s$r��米��$n$h", GetNumberString(2,NUM_CIRCLE));
			else if(bArch) XLText->AddFormatText("$e$h$r%s$r��ġ ����$n$h", GetNumberString(2,NUM_CIRCLE));

			if (bLsd)
			{
				MakeTableLoadForFooting_Lsd(XLText, pBri, pData->m_nJijum, TRUE, FALSE, TRUE);
			}
			else
			{
				MakeTableLoadSum(XLText, i, TRUE, TRUE, 0, TRUE);
			}
		}

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+14$e$h %d)$r��Ÿ �ۿ�����$n", nIndexSmall++);

		if (bLsd)
		{
			pData->MakeTableOtherLoadLsd(XLText);

			pData = NULL;
			if(pBri->IsValidComboEnvType(eEnvExtremeI))
				pData = GetLoadFootingLsd(i,eEnvExtremeI, 0);
			else if(pBri->IsValidComboEnvType(eEnvExtremeII))
				pData = GetLoadFootingLsd(i,eEnvExtremeII, 0);
			if(pData)
				pData->MakeTableOtherLoadLsd(XLText);
			
			pData = GetLoadFootingLsd(i,eEnvLimit, 0);
		}
		else
		{
			// ���� ��Ÿ �ۿ�����
			pData->MakeTableOtherLoad(XLText);
			// ������ ��Ÿ �ۿ�����
			pData = m_pListLoadSafty[i].GetAt(3);
			pData->MakeTableOtherLoad(XLText);
		}
		
		if (bLsd)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+14$e$h %d)$r��������$n", nIndexSmall++);

			long nIndexCircle = 1;
			m_pStd->m_pLoad->MakeTableOutValueEta_Lsd(XLText, 3, nIndexCircle++);

			XLText->GoPosition(NO_MOVE, COL_POS_2);
			XLText->AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(nIndexCircle++, NUM_CIRCLE), "��������");

			MakeTableLoadCombo(XLText, pBri, pData->m_nJijum, TRUE);
		}

		if(bTrap2 || bArch)
		{
			XLText->AddFormatText("$h$r$c*$r���ʿ� �ۿ��ϴ� ������ ��ü�ϴܰ� %s�� �ۿ��ϴ� ���/���Ʈ��$n", bTrap2? "��米���ϴ�" : "���������");
			XLText->AddFormatText("$h$r$r�ջ��Ͽ� ���� ū ���� �ش��ϴ� Combo�� �ܸ���� ���Ͽ���.$n");
		}
	
		// MASS �����϶� MASS������ ���ݹݷ��� ������ ��꿡 ���� �ñ��.
		if(pBri->m_bApplyMassCalcSafe && pData->m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
		{
			CalculateForceMASS(i);
			long nOverPage = GetCountSectMASS(i) + 2 + 10;
			CString str = _T("");
			str.Format("$s+%d", nOverPage);
			XLText->AddFormatText(str.GetBuffer(10));
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+15$e$h %d)$rMASS������ �ۿ�� ����$n", nIndexSmall++);
			MakeTableForceMASS(XLText, i);
		}
		
		if (bLsd)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+20$e$h %d)$r�����ۿ�� ����$n", nIndexSmall++);

			long nIndexCircle = 0;
			for (HGINT32 ixEnvType = 0; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
			{
				if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE)
					continue;

				if(ixEnvType == eEnvService)
					continue;

				for (HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
				{
					pData = GetLoadFootingLsd(i, (ETypeEnvComboLsd)ixEnvType, jx);
					if(pData == NULL) continue;

					pData->MakeTableLoadAll(XLText, nIndexCircle++);
				}
			}
			
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$$e$h %d)$r�����ۿ�� ����$n", nIndexSmall++);
			MakeTableLoadFooting(XLText, pBri, i, TRUE);

			CLsdFootingDesign* pFootingDesign = GetLsdFootingDesign(i);
			if(pFootingDesign == NULL) continue;

			const HGINT32 nSize = pFootingDesign->GetSizeSafeCheck(eLSD_COMBINATION_LIMIT) + pFootingDesign->GetSizeSafeCheck(eLSD_COMBINATION_EXTREME);
			BOOL bPileBase = pBri->IsPileBase(i);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%02d$e$h %d)$r������ ���� ����$n", bPileBase ? 1 : nSize + 3, nIndexSmall++);
			pFootingDesign->WriteOverTurningCheck(XLText, 2);


			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%02d$e$h %d)$rȰ���� ���� ����$n", bPileBase ? 1 : nSize + 13, nIndexSmall++);
			pFootingDesign->WriteSlidingCheck(XLText, 2);


			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%02d$e$h %d)$r�����¿� ���� ����$n", bPileBase ? 1 : nSize + 16, nIndexSmall++);
			pFootingDesign->WriteSupportForceCheck(XLText, 2);


			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+%02d$e$h %d)$rħ�Ϸ��� ���� ����$n", bPileBase ? 1 : 30, nIndexSmall++);
			pFootingDesign->WriteSubsidenceSafe(XLText, 2);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+40$e$h %d)$r���� �߾��ۿ�� ����$n", nIndexSmall++);
			pFootingDesign->WriteCalcCenterLoad(XLText, 2);

			if (pBri->IsPileBase(i) == FALSE)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText(_T("$h%s$n$h"), hggettext("��  ������ ����������"));
				pFootingDesign->WritelSupportForceCheck(XLText, 2);
			}
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+99$e$h %d)$r���� ��������$n", nIndexSmall++);

			if(pBri->IsPileBase(i))
			{
				XLText->AddFormatText("$h$r$e'- ���ұ����̹Ƿ� ���ʿ� : �����Ұ��䡻����$n$h");
				XLText->AddFormatText("$h$n");

				XLText->AddFormatText("$s+16$e$h %d)$r���� �������ۿ�� ����$n", nIndexSmall++);
				int j=0; for(j=0; j<4; j++)
				{
					pData = m_pListLoadSafty[i].GetAt(j);
					pData->MakeTableLoadAll(XLText, j);
					pData->WriteForceCenter_Pile(XLText);
				}

				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$s+99$e$h %d)$r���� �����ۿ�� ����$n", nIndexSmall++);
				XLText->AddFormatText("$e$h$r%s$r���ҵ��� ���$n$h", GetNumberString(1,NUM_CIRCLE));
				MakeTableCenterPile(XLText, i);
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$e$h$r%s$r���ҵ��� �ۿ��$n$h", GetNumberString(2,NUM_CIRCLE));
				MakeTableLoadSum(XLText, i, TRUE, TRUE, 1);
			}
			else
			{
				int j=0; for(j=0; j<4; j++)
				{
					long nIndexSub = 1;
					pData = m_pListLoadSafty[i].GetAt(j);
					pData->MakeTableLoadAll(XLText, j);

					pData->WriteCheckOverturning(XLText, nIndexSub++, bFirst);
					pData->WriteCheckSliding(XLText, nIndexSub++, bFirst);
					pData->WriteCheckSupportForce(XLText, nIndexSub++, bFirst);
					bFirst = FALSE;
				}
			}
		}

		WriteCheckFootingRigidBody(XLText, nIndexSmall++,i);
	}
}

void CARcBridgeCalcFooting::DrawFootingSection(CDomyun *pDomP,  CRcBridgeRebar *pBri, long nJijum)
{
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	CHgBaseDrawStd DrawStd;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bTrap = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP);
	BOOL bTrap2 = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
	BOOL bArch	= (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	BOOL bStt = (nJijum == 0)? TRUE:FALSE;
	BOOL bEnd = (nJijum == pBri->GetCountJijum()-1)? TRUE:FALSE;
	
	// ���� ������ TRAP2�ΰ�쿡 ���� �׸����� �ٷ� return���ѹ�����.
	if(bTrap2 && (nJijum<=1 || nJijum>=pBri->m_nQtyJigan-1) || bArch)
	{
		if(nJijum==0 || nJijum==pBri->m_nQtyJigan) DrawFootingSection_Type_Trap2(pDomP, pBri, nJijum==0);
		return;
	}

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	CDrawExPile drawExPile(pPile, m_pStd->m_pARoadOptionStd);
	
	CWallApp Wall;
	pBri->MakeWallByOutWall(Wall, nJijum==0, FALSE);
	CDPoint xyStt(0, 0);
	xyStt.x	= pBri->GetStationOnJijum(nJijum, 0);
	if(nJijum == 0) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, -pBri->m_dWS, 0).x;
	else if(nJijum == pBri->m_nQtyJigan-1) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, pBri->m_dWE, 0).x;
	
	double dHightBackFill = pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y - pFooting->m_dEL + Wall.m_dELBDiff;
	
	// ���ʰ� �������� �׸�
	pBri->SyncBridge();
	
	BOOL bSlopeR = (bTrap && pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE);
	BOOL bSlopeL = (bTrap && pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE);
	CDPoint xyBase = CDPoint(0,0);		// ������....
	CDPoint xyVerDimStt = CDPoint(0,0);
	CDPoint xyVerDimEnd = xyVerDimStt;
	CDoubleArray dArrVerDim;
	double dTemp = 0;
	CString szVerDimDir = "LEFT";

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	Dom.SetScaleDim(80);
// 	if(pBri->IsPileBase(nJijum) == FALSE) Dom.SetTextHeight(5);
// 	else Dom.SetTextHeight(5);
	Dom.SetTextHeight(5);
	Dom.SetArrowHeadSize(2);
	Dom.SetDimVLen(7);
	Dom.SetDimExo(4);

	dArrVerDim.RemoveAll();
	if(bEnd || (bTrap && bSlopeL))
	{
		xyBase.x = pFooting->m_xyBottomCenter.x - pFooting->GetWidth()/2;
		xyBase.y = pFooting->m_xyBottomCenter.y;
		szVerDimDir = "RIGHT";
		xyVerDimStt = xyBase;
		xyVerDimStt.x += pFooting->GetWidth();
	}
	else
	{
		xyBase.x = pFooting->m_xyBottomCenter.x + pFooting->GetWidth()/2;
		xyBase.y = pFooting->m_xyBottomCenter.y;
		szVerDimDir = "LEFT";
		xyVerDimStt = xyBase;
		xyVerDimStt.x -= pFooting->GetWidth();
	}
	
	xyVerDimEnd = xyVerDimStt;
	if(bTrap)
	{
		dTemp = pFooting->GetHeight(FALSE);
		dArrVerDim.Add(dTemp);
		xyVerDimEnd.y += dTemp;
		dTemp = pFooting->m_dFH;
		dArrVerDim.Add(dTemp);
		xyVerDimEnd.y += dTemp;
	}
	else
	{
		dTemp = pFooting->GetHeight();
		dArrVerDim.Add(dTemp);
		xyVerDimEnd.y += dTemp;
	}
	
	if(bStt || bEnd)
	{
		dArrVerDim.Add(dHightBackFill);
		xyVerDimEnd.y += dHightBackFill;
		if(pBri->m_bUnderGroundRahmen)
		{
			dArrVerDim.Add(pBri->m_dUnderGroundDesignHeight);
			xyVerDimEnd.y += pBri->m_dUnderGroundDesignHeight;
		}
		else
		{
			dArrVerDim.Add(pBri->m_dTP);
			xyVerDimEnd.y += pBri->m_dTP;
		}
	}

	CDRect rectBridge;	// ������ �簢 ����
	rectBridge = pBri->m_tvArrVertSection.GetRect();
	
	// ���ܸ�
	CDRect rectVert;
	CDRect rect;

	
	// �߰���ü�������̸� �߰���ü�� �׸���
	CWallApp *pWall = pBri->GetInWall(nJijum-1);
	if(pWall)
	{
		CTwinVectorArray tvTot;
		
		tvTot.AddFromTvArray(pWall->m_tvArrFront);
		if(pWall->m_bTopIsColumn)
			tvTot.AddFromTvArray(pWall->m_Column.m_tvArrFront);
		tvTot.AddFromTvArray(pWall->m_footing.m_tvArrFront);
		tvTot.AddFromTvArray(pWall->m_footing.m_exFooting.m_tvArrFront);
		
		CDRect rect = tvTot.GetRect();
		rect.left -= 300;
		rect.right += 300;
		rect.top  = rectBridge.top + 300;
		rect.bottom -= 300;
		tvTot.AddFromTvArray(pBri->m_tvArrVertSection);
		
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
			DrawStd.DrawTvArray(&Dom, tvTot);
		else 
			DrawStd.DrawCuttingTvArray(&Dom, tvTot, rect);
	}
	// ������ü �������̸� ������ü�� �׸���
	else
	{
		CTwinVectorArray tvTot;
		tvTot.AddFromTvArray(pBri->m_tvArrVertSection);
		tvTot.AddFromTvArray(pFooting->m_tvArrFront);
		tvTot.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);
		
		rect = tvTot.GetRect();
		CDRect rcTemp = pFooting->m_exFooting.m_tvArrFront.GetRect();
		
		rect.right	= rcTemp.right + 100;
		rect.left	= rcTemp.left - 100;

		rect.top = rectBridge.top + 100;
		rect.bottom -= 100;

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
			DrawStd.DrawTvArray(&Dom, tvTot);
		else
			DrawStd.DrawCuttingTvArray(&Dom, tvTot, rect);			
	}

	Dom.Circle(xyBase.x, xyBase.y, 200);
	long iDir = (bEnd || (bTrap && bSlopeL))? -1:1;
	Dom.TextOut(xyBase.x+700*iDir, xyBase.y-700, "������");
	// ġ����..
	long nCntDim = dArrVerDim.GetSize();
	Dom.SetDirection(szVerDimDir);
	Dom.DimMoveTo(xyVerDimStt.x, xyVerDimStt.y, 0, "");
	int i=0; for(i=0;i<nCntDim;i++)
	{
		Dom.DimLineTo(dArrVerDim.GetAt(i), 0, COMMA(dArrVerDim.GetAt(i)));
	}

	double dThickWall = pBri->GetThickWall(nJijum);
	Dom.SetDirection("TOP");

	if(bSlopeR)
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(-pFooting->m_dFW, 0, COMMA(pFooting->m_dFW));
		Dom.DimLineTo(-pFooting->m_dFSLW, 0, COMMA(pFooting->m_dFSLW));
	}
	else if(bSlopeL)
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(pFooting->m_dFW, 0, COMMA(pFooting->m_dFW));
		Dom.DimLineTo(pFooting->m_dFSRW, 0, COMMA(pFooting->m_dFSRW));
	}
	else if(bEnd)
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(pFooting->m_dFWL, 0, COMMA(pFooting->m_dFWL));
		Dom.DimLineTo(dThickWall, 0, COMMA(dThickWall));
		Dom.DimLineTo(pFooting->m_dFWR, 0, COMMA(pFooting->m_dFWR));
	}
	else
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(-pFooting->m_dFWR, 0, COMMA(pFooting->m_dFWR));
		Dom.DimLineTo(-dThickWall, 0, COMMA(dThickWall));
		Dom.DimLineTo(-pFooting->m_dFWL, 0, COMMA(pFooting->m_dFWL));
	}
	

	if(pBri->IsPileBase(nJijum))
	{
		
		Dom.SetTextHeight(5);
		drawExPile.DrawFront(&Dom, TRUE, FALSE, FALSE);
		drawExPile.m_bApplyDimSettingOfBaseDom	= TRUE;
		drawExPile.DimFront(&Dom, 0, FALSE, FALSE, TRUE ,TRUE, FALSE);
		drawExPile.m_bApplyDimSettingOfBaseDom	= FALSE;
	}
	else
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyBase.x, xyBase.y, 0, "");
		if(bEnd || (bTrap && bSlopeL)) Dom.DimLineTo(pFooting->GetWidth(), 0, COMMA(pFooting->GetWidth()));
		else Dom.DimLineTo(-pFooting->GetWidth(), 0, COMMA(pFooting->GetWidth()));
	}

	CDPoint xyCen;
	double wB = 0;
	if(bEnd || (bTrap && bSlopeL))
	{
		if(bTrap)
		{
			CTwinVector tvTemp = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
			xyCen = tvTemp.m_v1;
		}
		else
		{
			xyCen.x = xyBase.x + pFooting->m_dFWL + dThickWall/2;
			xyCen.y = xyBase.y + pFooting->GetHeight();
		}
		
		Dom.DimArrowTo(xyCen.x+400, xyCen.y, 800, 4);
		wB = pFooting->m_dFWR;
		CObInsert *pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENTR", xyCen.x, xyCen.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");
	}
	else
	{
		if(bTrap)
		{
			CTwinVector tvTemp = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
			xyCen = tvTemp.m_v1;
		}
		else
		{
			xyCen.x = xyBase.x - pFooting->m_dFWR - dThickWall/2;
			xyCen.y = xyBase.y + pFooting->GetHeight();
		}
		
		Dom.DimArrowTo(xyCen.x-400, xyCen.y, 800, 0);
		wB = pFooting->m_dFWL;
		CObInsert *pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENT", xyCen.x, xyCen.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");
	}
	Dom.DimArrowTo(xyCen.x, xyCen.y+600, 600, 6);
	if(bStt || bEnd)
	{
		Dom.MoveTo(xyVerDimEnd.x, xyVerDimEnd.y);
		if(bStt)Dom.LineTo(rect.BottomRight().x, xyVerDimEnd.y);
		else Dom.LineTo(rect.BottomLeft().x, xyVerDimEnd.y);
		Dom.MoveTo(xyVerDimEnd.x, xyVerDimEnd.y+800);
		Dom.LineTo(xyVerDimEnd.x+wB*iDir, xyVerDimEnd.y+800);
		for(i=0; i<7; i++)
		{
			Dom.DimArrowTo(xyVerDimEnd.x+wB/6*i*iDir, xyVerDimEnd.y+800, 800, 6);
		}
		CString str;
		double dUpperLoad = pBri->m_dLiveLoadAlways;
		if(pBri->m_dLiveLoadAlways>0 && pBri->m_dDeadLoadAlways>0)
			str.Format("q=%.3f %s, q=%.3f %s", pData->GetValueUnitType(pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2), pData->m_szTonf_M2,
			                                   pData->GetValueUnitType(pBri->m_dDeadLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2), pData->m_szTonf_M2);
		else
			str.Format("q = %.3f %s", pData->GetValueUnitType(dUpperLoad, UNIT_CGS_TONF_M2, pData->m_szTonf_M2), pData->m_szTonf_M2);
		Dom.SetTextAlignHorz(TA_CENTER);
		if(pBri->IsPileBase(nJijum) == FALSE) Dom.SetTextHeight(5);
		else Dom.SetTextHeight(6);
		Dom.TextOut(xyVerDimEnd.x+wB/2*iDir, xyVerDimEnd.y +1000, str);
		Dom.MoveTo(xyVerDimStt.x, xyVerDimStt.y);
		Dom.LineTo(xyVerDimEnd.x, xyVerDimEnd.y);
	}

	Dom.SetCalcExtRect();
	rectVert = Dom.GetExtRect();


	*pDomP << Dom;
}

void CARcBridgeCalcFooting::CalculateLoad_Usd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType()) 	return;

	CString strTitle = _T("");
	CFootingApp *pFooting(NULL);

	double Momet = 0;
	double Shear = 0; 
	double Axial = 0;
	long nTypeCheck = 0;
	long nCombo	= 0;

	long nCntJijum = pBri->GetCountJijum();
	int i=0; for(i=0; i<nCntJijum; i++)
	{
		BOOL bTrap2 = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (i==0 || i==nCntJijum-1));
		BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && (i==0 || i==nCntJijum-1));

		pFooting = pBri->GetFootingByJijumNumber(i);
		if(pFooting == NULL || pFooting->m_bIs == FALSE) continue;
		
		if((i==0 && !pBri->IsOutWall(TRUE)) || (i==nCntJijum-1 && !pBri->IsOutWall(FALSE))) continue;
		else if(i == 0) strTitle = "������ ����";
		else if(i == nCntJijum-1) strTitle = "������ ����";
		else strTitle.Format("�߰�������%d ����", i);

		long nIndexAdd = -1;
		if(bTrap2) nIndexAdd = (i==0)? 1 : pBri->m_nQtyJigan-1;
		else if(bArch) nIndexAdd = (i==0)? pBri->m_nQtyJigan+1 : pBri->m_nQtyJigan+2;
		
		int j=0; for(j=0; j<4; j++)
		{
			CLoadFooting *pLoadUse = new CLoadFooting(m_pStd, pBri, i, j, TRUE);		// 0: ����ִ�, 1: �θ��Ʈ�ִ�, 2: �����Ʈ�ִ�, 3: ������
			CLoadFooting *pLoadUlt = new CLoadFooting(m_pStd, pBri, i, j, FALSE);
			CLoadFooting *pLoadSafety = new CLoadFooting(m_pStd, pBri, i, j, TRUE);
//////////////////////////////////////////////////////////////////////////
			if(j == 0) nTypeCheck = FOOTING_CHECK_AXIAL;
			else if(j == 1) nTypeCheck = FOOTING_CHECK_MOMENT_MINUS;
			else if(j == 2) nTypeCheck = FOOTING_CHECK_MOMENT_PLUS;
			else if(j == 3) nTypeCheck = FOOTING_CHECK_EQ;

			pLoadUse->m_sTitle = strTitle;
			pLoadUlt->m_sTitle = strTitle;
			pLoadSafety->m_sTitle = strTitle;

			// �������
			BOOL bNoneImpact = (pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? TRUE : FALSE;
			nCombo = pBri->GetOutValueForFootingDesign(i, nTypeCheck, Momet, Shear, Axial, TRUE, nIndexAdd, -1, FALSE);

			CalculateForceMASS(i);
			pLoadUse->m_nCombo		= nCombo;
			pLoadUse->m_dUpperLoadV = Axial;
			pLoadUse->m_dUpperLoadH = Shear;
			pLoadUse->m_dUpperLoadM = Momet;
			pLoadUse->m_dQmaxMass	= GetQmaxMASS(i);

			if(bNoneImpact)
			{
				// �������信 ��ݰ�� �������� ������ ��ݰ���� �������� ���� ������ ���� �����Ѵ�.
				nCombo = pBri->GetOutValueForFootingDesign(i, nTypeCheck, Momet, Shear, Axial, TRUE, nIndexAdd, -1, TRUE);
			}
			pLoadSafety->m_nCombo		= nCombo;
			pLoadSafety->m_dUpperLoadV = Axial;
			pLoadSafety->m_dUpperLoadH = Shear;
			pLoadSafety->m_dUpperLoadM = Momet;
			pLoadSafety->m_dQmaxMass	= GetQmaxMASS(i);

			// �������
			nCombo = pBri->GetOutValueForFootingDesign(i, nTypeCheck, Momet, Shear, Axial, FALSE, nIndexAdd);
			pLoadUlt->m_nCombo		= nCombo;
			pLoadUlt->m_dUpperLoadV = Axial;
			pLoadUlt->m_dUpperLoadH = Shear;
			pLoadUlt->m_dUpperLoadM = Momet;
			pLoadUlt->m_dQmaxMass	= GetQmaxMASS(i);

			///////////////////////////////////////////////////////////////////////////
			// ���������� TRAP2 �̰� ������ �����϶� ��米���� ���� �ۿ����  �־��ش�.
			if(bTrap2 || bArch)
			{
				// �������
				nCombo = pBri->GetOutValueForFootingDesign(nIndexAdd, nTypeCheck, Momet, Shear, Axial, TRUE, i, i);
				pLoadUse->m_dUpperLoadV_Add = Axial;
				pLoadUse->m_dUpperLoadH_Add = Shear;
				pLoadUse->m_dUpperLoadM_Add = Momet;
				pLoadUse->m_nCombo_Add = nCombo;
				pLoadUse->m_bTwoUpperLoad = TRUE;

				if(bNoneImpact)
				{
					// �������信 ��ݰ�� �������� ������ ��ݰ���� �������� ���� ������ ���� �����Ѵ�.
					nCombo = pBri->GetOutValueForFootingDesign(nIndexAdd, nTypeCheck, Momet, Shear, Axial, TRUE, i, i, TRUE);
				}
				pLoadSafety->m_nCombo_Add		= nCombo;
				pLoadSafety->m_dUpperLoadV_Add = Axial;
				pLoadSafety->m_dUpperLoadH_Add = Shear;
				pLoadSafety->m_dUpperLoadM_Add = Momet;
				pLoadSafety->m_bTwoUpperLoad = TRUE;
				
				// �������
				nCombo = pBri->GetOutValueForFootingDesign(nIndexAdd, nTypeCheck, Momet, Shear, Axial, FALSE, i, i);
				pLoadUlt->m_dUpperLoadV_Add = Axial;
				pLoadUlt->m_dUpperLoadH_Add = Shear;
				pLoadUlt->m_dUpperLoadM_Add = Momet;
				pLoadUlt->m_nCombo_Add = nCombo;
				pLoadUlt->m_bTwoUpperLoad = TRUE;
			}
			
//////////////////////////////////////////////////////////////////////////
			pLoadUse->CalculateBaseLoad();
			pLoadUse->CalculateApplyLoad();

			pLoadSafety->CalculateBaseLoad();
			pLoadSafety->CalculateApplyLoad();

			pLoadUlt->CalculateBaseLoad();
			pLoadUlt->CalculateApplyLoad();
			
			//////////////////////////////////////////////////////////////////////////
			// �Է��� ������信 �ʿ��ϹǷ� ���ʰ� ���� �����϶��� �������� �ؾ���
			pLoadUse->CalulateQaMax();
			pLoadUse->CalculateOverturningCheck();
			pLoadUse->CalculateSlidingCheck();
			pLoadUse->CalculateSupportForce();

			pLoadSafety->CalulateQaMax();
			pLoadSafety->CalculateOverturningCheck();
			pLoadSafety->CalculateSlidingCheck();
			pLoadSafety->CalculateSupportForce();
			
			pLoadUlt->CalulateQaMax();
			pLoadUlt->CalculateOverturningCheck();
			pLoadUlt->CalculateSlidingCheck();
			pLoadUlt->CalculateSupportForce();
			//////////////////////////////////////////////////////////////////////////
			if(pBri->IsPileBase(i))
			{
				pLoadUse->CalulatePileCentroidLoad();
				pLoadSafety->CalulatePileCentroidLoad();
				pLoadUlt->CalulatePileCentroidLoad();
			}
			m_pListLoadUse[i].Add(pLoadUse);
			m_pListLoadUlt[i].Add(pLoadUlt);
			m_pListLoadSafty[i].Add(pLoadSafety);
		}
	}
}

void CARcBridgeCalcFooting::CalculateLoad_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType()) 	return;

	CString strTitle = _T("");
	CFootingApp *pFooting(NULL);

	double Momet = 0;
	double Shear = 0; 
	double Axial = 0;
	long nCombo	= 0;
	long nIdxModel = 0;

	long nCntJijum = pBri->GetCountJijum();
	for(long nJ=0; nJ<nCntJijum; ++nJ)
	{
		EPartOfBridge ePart = (nJ == 0)? ePartFootStt : ((nJ == pBri->m_nQtyJigan)? ePartFootEnd : ePartInFoot);

		BOOL bTrap2 = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (nJ==0 || nJ==nCntJijum-1));
		BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && (nJ==0 || nJ==nCntJijum-1));

		pFooting = pBri->GetFootingByJijumNumber(nJ);
		if(pFooting == NULL || pFooting->m_bIs == FALSE) continue;

		if((nJ==0 && !pBri->IsOutWall(TRUE)) || (nJ==nCntJijum-1 && !pBri->IsOutWall(FALSE))) continue;
		else if(nJ == 0) strTitle = _T("������ ����");
		else if(nJ == nCntJijum-1) strTitle = _T("������ ����");
		else strTitle.Format(_T("�߰�������%d ����"), nJ);

		long nIndexAdd = -1;
		if(bTrap2) nIndexAdd = (nJ==0)? 1 : pBri->m_nQtyJigan-1;
		else if(bArch) nIndexAdd = (nJ==0)? pBri->m_nQtyJigan+1 : pBri->m_nQtyJigan+2;

		for(long ixEnv=0; ixEnv<eCountEnvTypeLsd; ++ixEnv)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnv;
			CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);

			// ����Ѱ���� �϶��� ���������(��ü����)�� ��뼺�����(������ ���)�� ���� �����Ѵ�.
			for(long ixCheck=0; ixCheck<eCountFootCheck; ++ixCheck)
			{
				CLoadFooting *pLoadFoot = new CLoadFooting(m_pStd, pBri, nJ, ixCheck, FALSE, eEnvType);
				pLoadFoot->m_sTitle = strTitle;

				nCombo = pBri->GetOutValueForFootingDesign_Lsd(nJ, eEnvType, (EFootingCheckLsd)ixCheck, Momet, Shear, Axial, nIdxModel, nIndexAdd);
				if(nCombo < 0) continue;

				CString szCombo(_T(""));
				if(eEnvType == eEnvService_Crack)
				{
					long nIndexCur(0);
					for(long nL=eLSD_COMBINE_SERVICE_I; nL<=eLSD_COMBINE_SERVICE_V; nL++)
					{
						ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

						std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
						long nSize = LCArray.size();

						if(nCombo < nIndexCur + nSize)
						{
							nCombo = (nCombo - nIndexCur);
							szCombo = LCArray[nCombo]->GetName();
							break;
						}
						nIndexCur += nSize;
					}
				}
				else
				{
					szCombo = pBri->GetStringComboEta_Lsd(eKindCombo, ePart, nCombo + 1, TRUE, nIdxModel);
					if(nIdxModel > 1)
					{
						nCombo = pBri->m_plsdManager->GetIndexLoasdCaseByName(eKindCombo, eLSD_LOADCASE_DIR_BRI, szCombo);
					}
				}

				CalculateForceMASS(nJ);
				pLoadFoot->m_nCombo		= nCombo;
				pLoadFoot->m_sCombo		= szCombo;
				pLoadFoot->m_dUpperLoadV = Axial;
				pLoadFoot->m_dUpperLoadH = Shear;
				pLoadFoot->m_dUpperLoadM = Momet;
				pLoadFoot->m_dQmaxMass	= GetQmaxMASS(nJ);
				pLoadFoot->m_sTitleSub	= pBri->GetStringFootingCheckCase((EFootingCheckLsd)ixCheck);

				if(bTrap2 || bArch)
				{
					pBri->GetOutValueForFootingDesign_Lsd(nIndexAdd, eEnvType, (EFootingCheckLsd)ixCheck, Momet, Shear, Axial, nIdxModel, nJ, nJ);
					
					pLoadFoot->m_dUpperLoadV_Add = Axial;
					pLoadFoot->m_dUpperLoadH_Add = Shear;
					pLoadFoot->m_dUpperLoadM_Add = Momet;
					pLoadFoot->m_nCombo_Add = nCombo;
					pLoadFoot->m_bTwoUpperLoad = TRUE;
				}

				pLoadFoot->CalculateBaseLoad();
				pLoadFoot->CalculateApplyLoad();

				//////////////////////////////////////////////////////////////////////////
				// �Է��� ������信 �ʿ��ϹǷ� ���ʰ� ���� �����϶��� �������� �ؾ���
				pLoadFoot->CalulateQaMax();
				pLoadFoot->CalculateOverturningCheck();
				pLoadFoot->CalculateSlidingCheck();
				pLoadFoot->CalculateSupportForce();

				if(pBri->IsPileBase(nJ))
				{
					pLoadFoot->CalulatePileCentroidLoad();
				}

				m_pListLoadLsd[nJ][eEnvType].Add(pLoadFoot);
			}
		}
	}
}

void CARcBridgeCalcFooting::CalculateSection_Usd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType()) return;

	long nCntJijum = m_pStd->m_pBri->GetCountJijum();
	long i=0; for(i=0; i<nCntJijum; i++)
	{
		BOOL bInWall = (!pBri->IsOutsideWall() || (i!=0 && i!=pBri->m_nQtyJigan));

		if((i==0 && !m_pStd->m_pBri->IsOutWall(TRUE)) || (i==nCntJijum-1 && !m_pStd->m_pBri->IsOutWall(FALSE))) continue;
		if(m_pListLoadUse[i].GetSize() == 0 || m_pListLoadUlt[i].GetSize() == 0) continue;

		BOOL bIsTwoLoad = FALSE;
		double dHeightReal = 0;
		long j=0; for(j=0; j<4; j++)
		{
			CLoadFooting *pLoadUse = m_pListLoadUse[i].GetAt(j);
			CLoadFooting *pLoadUlt = m_pListLoadUlt[i].GetAt(j);

			pLoadUse->CalculateSectionDesign();
			pLoadUlt->CalculateSectionDesign();

			if(j == 0)
			{
				bIsTwoLoad = pLoadUse->m_bTwoUpperLoad;
				if(bIsTwoLoad) {
					double dDistTemp = 0.;
					pLoadUse->GetDistFrontTrap2AndArch(dDistTemp, dHeightReal);
				}
			}

			m_dATensionUpperVUlt[i][0] = MIN(m_dATensionUpperVUlt[i][0], pLoadUlt->m_dVuFront);
			m_dATensionUpperMUlt[i][0] = MIN(m_dATensionUpperMUlt[i][0], pLoadUlt->m_dMuFront);
			if(j < 3) m_dATensionUpperMUse[i][0] = MIN(m_dATensionUpperMUse[i][0], pLoadUse->m_dMuFront);

			m_dATensionLowerVUlt[i][0] = MAX(m_dATensionLowerVUlt[i][0], pLoadUlt->m_dVuFront);
			m_dATensionLowerMUlt[i][0] = MAX(m_dATensionLowerMUlt[i][0], pLoadUlt->m_dMuFront);
			if(j < 3) m_dATensionLowerMUse[i][0] = MAX(m_dATensionLowerMUse[i][0], pLoadUse->m_dMuFront);

			m_dATensionUpperVUlt[i][1] = MAX(m_dATensionUpperVUlt[i][1], pLoadUlt->m_dVuBack);
			m_dATensionUpperMUlt[i][1] = MAX(m_dATensionUpperMUlt[i][1], pLoadUlt->m_dMuBack);
			if(j < 3) m_dATensionUpperMUse[i][1] = MAX(m_dATensionUpperMUse[i][1], pLoadUse->m_dMuBack);
			
			m_dATensionLowerVUlt[i][1] = MIN(m_dATensionLowerVUlt[i][1], pLoadUlt->m_dVuBack);
			m_dATensionLowerMUlt[i][1] = MIN(m_dATensionLowerMUlt[i][1], pLoadUlt->m_dMuBack);
			if(j < 3) m_dATensionLowerMUse[i][1] = MIN(m_dATensionLowerMUse[i][1], pLoadUse->m_dMuBack);

			if(pLoadUlt->m_bTwoUpperLoad)
			{
				m_dATensionUpperVUlt[i][2] = MIN(m_dATensionUpperVUlt[i][2], pLoadUlt->m_dVuFront);
				m_dATensionUpperMUlt[i][2] = MIN(m_dATensionUpperMUlt[i][2], pLoadUlt->m_dMuFront2);
				if(j < 3)
				{
					m_dATensionUpperMUse[i][2] = MIN(m_dATensionUpperMUse[i][2], pLoadUse->m_dMuFront2);
				}
				
				m_dATensionLowerVUlt[i][2] = MAX(m_dATensionLowerVUlt[i][2], pLoadUlt->m_dVuFront);
				m_dATensionLowerMUlt[i][2] = MAX(m_dATensionLowerMUlt[i][2], pLoadUlt->m_dMuFront2);
				if(j < 3)
				{
					m_dATensionLowerMUse[i][2] = MAX(m_dATensionLowerMUse[i][2], pLoadUse->m_dMuFront2);
				}

				if(pBri->m_bCheckFrontFootTrap2)
				{
					m_dATensionUpperVUlt[i][3] = MIN(m_dATensionUpperVUlt[i][3], pLoadUlt->m_dVuFront);
					m_dATensionUpperMUlt[i][3] = MIN(m_dATensionUpperMUlt[i][3], pLoadUlt->m_dMuFront3);

					m_dATensionLowerVUlt[i][3] = MAX(m_dATensionLowerVUlt[i][3], pLoadUlt->m_dVuFront);
					m_dATensionLowerMUlt[i][3] = MAX(m_dATensionLowerMUlt[i][3], pLoadUlt->m_dMuFront3);
					if(j < 3)
					{
						m_dATensionUpperMUse[i][3] = MIN(m_dATensionUpperMUse[i][3], pLoadUse->m_dMuFront3);
						m_dATensionLowerMUse[i][3] = MAX(m_dATensionLowerMUse[i][3], pLoadUse->m_dMuFront3);
					}
				}
			}
		}
		
		for(j=0; j<4; j++)
		{
			m_dATensionUpperVUlt[i][j] = fabs(m_dATensionUpperVUlt[i][j]);
			m_dATensionUpperMUlt[i][j] = fabs(m_dATensionUpperMUlt[i][j]);
			m_dATensionUpperMUse[i][j] = fabs(m_dATensionUpperMUse[i][j]);
			
			m_dATensionLowerVUlt[i][j] = fabs(m_dATensionLowerVUlt[i][j]);
			m_dATensionLowerMUlt[i][j] = fabs(m_dATensionLowerMUlt[i][j]);
			m_dATensionLowerMUse[i][j] = fabs(m_dATensionLowerMUse[i][j]);
		}

		// �߰������� ��� �ޱ��� �����߾����� �ձ��� �ޱ� �ۿ���Ʈ�� ū�ɷ� �������ش�.
		CConcBasic *pConc;
		pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, FALSE);		// �ձ� - �ϸ� ����
		if(pConc != NULL)
		{
			pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMUlt[i][0], pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMUse[i][0], pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVUlt[i][0], pData->m_szTonf, UNIT_CGS_TONF);
			pConc->m_MomentCrack	= pConc->m_MomentUse;
			if(bIsTwoLoad && frM(dHeightReal) > pConc->m_Ho)
			{
				double dHun = (frM(dHeightReal) - pConc->m_Ho) * 3;
				pConc->m_Hunch = dHun;
			}
		}

		pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, TRUE);		// �ձ� - ��� ����
		if(pConc != NULL)
		{
			pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMUlt[i][0], pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMUse[i][0], pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVUlt[i][0], pData->m_szTonf, UNIT_CGS_TONF);
			pConc->m_MomentCrack	= pConc->m_MomentUse;
			if(bIsTwoLoad && frM(dHeightReal) > pConc->m_Ho)
			{
				double dHun = (frM(dHeightReal) - pConc->m_Ho) * 3;
				pConc->m_Hunch = dHun;
			}
		}
		
		pConc = m_pStd->m_pBri->GetConcDataFooting(i, FALSE, (bInWall)? TRUE : FALSE);		// �ޱ� - ��� ����
		if(pConc != NULL)
		{
			double dMomentApply = bInWall? MAX(m_dATensionUpperMUlt[i][0], m_dATensionUpperMUlt[i][1]) : m_dATensionUpperMUlt[i][1];
			double dMomentUseApply = bInWall? MAX(m_dATensionUpperMUse[i][0], m_dATensionUpperMUse[i][1]) : m_dATensionUpperMUse[i][1];
			double dShearApply = bInWall? MAX(m_dATensionUpperVUlt[i][0], m_dATensionUpperVUlt[i][1]) : m_dATensionUpperVUlt[i][1];
			pConc->m_Moment = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_MomentUse = GetValueUnitChange(dMomentUseApply, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_Shear = GetValueUnitChange(dShearApply, pData->m_szTonf, UNIT_CGS_TONF);
			pConc->m_MomentCrack	= pConc->m_MomentUse;
		}

		pConc = m_pStd->m_pBri->GetConcDataFooting(i, FALSE, (bInWall)? FALSE : TRUE);		// �ޱ� - �ϸ� ����
		if(pConc != NULL)
		{
			double dMomentApply = bInWall? MAX(m_dATensionLowerMUlt[i][0], m_dATensionLowerMUlt[i][1]) : m_dATensionLowerMUlt[i][1];
			double dMomentUseApply = bInWall? MAX(m_dATensionLowerMUse[i][0], m_dATensionLowerMUse[i][1]) : m_dATensionLowerMUse[i][1];
			double dShearApply = bInWall? MAX(m_dATensionLowerVUlt[i][0], m_dATensionLowerVUlt[i][1]) : m_dATensionLowerVUlt[i][1];
			pConc->m_Moment = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_MomentUse = GetValueUnitChange(dMomentUseApply, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_Shear = GetValueUnitChange(dShearApply, pData->m_szTonf, UNIT_CGS_TONF);
			pConc->m_MomentCrack	= pConc->m_MomentUse;
		}

		// �ձ����� ������ġ �߰�
		if(bIsTwoLoad && !bInWall)
		{
			pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, FALSE, 0);		// �ձ�2 - �ϸ� ����
			if(pConc != NULL)
			{
				pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMUlt[i][2], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMUse[i][2], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVUlt[i][2], pData->m_szTonf, UNIT_CGS_TONF);
				pConc->m_MomentCrack	= pConc->m_MomentUse;
			}
			
			pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, TRUE, 0);		// �ձ�2 - ��� ����
			if(pConc != NULL)
			{
				pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMUlt[i][2], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMUse[i][2], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVUlt[i][2], pData->m_szTonf, UNIT_CGS_TONF);
				pConc->m_MomentCrack	= pConc->m_MomentUse;
			}

			//////////
			pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, FALSE, 1);		// �ձ�3 - �ϸ� ����
			if(pConc != NULL)
			{
				pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMUlt[i][3], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMUse[i][3], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVUlt[i][3], pData->m_szTonf, UNIT_CGS_TONF);
				pConc->m_MomentCrack	= pConc->m_MomentUse;
			}
			
			pConc = m_pStd->m_pBri->GetConcDataFooting(i, TRUE, TRUE, 1);		// �ձ�3 - ��� ����
			if(pConc != NULL)
			{
				pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMUlt[i][3], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMUse[i][3], pData->m_szTonfM, UNIT_CGS_TONFM);
				pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVUlt[i][3], pData->m_szTonf, UNIT_CGS_TONF);
				pConc->m_MomentCrack	= pConc->m_MomentUse;
			}
		}
	}
}

void CARcBridgeCalcFooting::CalculateSection_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType()) return;

	long nCntJijum = m_pStd->m_pBri->GetCountJijum();
	for(long nJ=0; nJ<nCntJijum; nJ++)
	{
		BOOL bInWall = (!pBri->IsOutsideWall() || (nJ!=0 && nJ!=pBri->m_nQtyJigan));

		if((nJ==0 && !m_pStd->m_pBri->IsOutWall(TRUE)) || (nJ==nCntJijum-1 && !m_pStd->m_pBri->IsOutWall(FALSE))) continue;

		for(long ixEnv=0; ixEnv<eCountEnvTypeLsd; ++ixEnv)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnv;
			CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			BOOL bIsTwoLoad = FALSE;
			double dHeightReal = 0;

			for(long ixCheck=0; ixCheck<eCountFootCheck; ++ixCheck)
			{
				CLoadFooting *pLoad = GetLoadFootingLsd(nJ, eEnvType, ixCheck);
				if(pLoad == NULL) continue;

				pLoad->CalculateSectionDesign();

				if(ixCheck == 0)
				{
					bIsTwoLoad = pLoad->m_bTwoUpperLoad;
					if(bIsTwoLoad) 
					{
						double dDistTemp = 0.;
						pLoad->GetDistFrontTrap2AndArch(dDistTemp, dHeightReal);
					}
				}

				m_dATensionUpperVLsd[nJ][ixEnv][0] = MIN(m_dATensionUpperVLsd[nJ][ixEnv][0], pLoad->m_dVuFront);
				m_dATensionUpperMLsd[nJ][ixEnv][0] = MIN(m_dATensionUpperMLsd[nJ][ixEnv][0], pLoad->m_dMuFront);

				m_dATensionLowerVLsd[nJ][ixEnv][0] = MAX(m_dATensionLowerVLsd[nJ][ixEnv][0], pLoad->m_dVuFront);
				m_dATensionLowerMLsd[nJ][ixEnv][0] = MAX(m_dATensionLowerMLsd[nJ][ixEnv][0], pLoad->m_dMuFront);

				m_dATensionUpperVLsd[nJ][ixEnv][1] = MAX(m_dATensionUpperVLsd[nJ][ixEnv][1], pLoad->m_dVuBack);
				m_dATensionUpperMLsd[nJ][ixEnv][1] = MAX(m_dATensionUpperMLsd[nJ][ixEnv][1], pLoad->m_dMuBack);

				m_dATensionLowerVLsd[nJ][ixEnv][1] = MIN(m_dATensionLowerVLsd[nJ][ixEnv][1], pLoad->m_dVuBack);
				m_dATensionLowerMLsd[nJ][ixEnv][1] = MIN(m_dATensionLowerMLsd[nJ][ixEnv][1], pLoad->m_dMuBack);

				if(pLoad->m_bTwoUpperLoad)
				{
					m_dATensionUpperVLsd[nJ][ixEnv][2] = MIN(m_dATensionUpperVLsd[nJ][ixEnv][2], pLoad->m_dVuFront);
					m_dATensionUpperMLsd[nJ][ixEnv][2] = MIN(m_dATensionUpperMLsd[nJ][ixEnv][2], pLoad->m_dMuFront2);

					m_dATensionLowerVLsd[nJ][ixEnv][2] = MAX(m_dATensionLowerVLsd[nJ][ixEnv][2], pLoad->m_dVuFront);
					m_dATensionLowerMLsd[nJ][ixEnv][2] = MAX(m_dATensionLowerMLsd[nJ][ixEnv][2], pLoad->m_dMuFront2);
				}

				if(pBri->m_bCheckFrontFootTrap2)
				{
					m_dATensionUpperVLsd[nJ][ixEnv][3] = MIN(m_dATensionUpperVLsd[nJ][ixEnv][3], pLoad->m_dVuFront);
					m_dATensionUpperMLsd[nJ][ixEnv][3] = MIN(m_dATensionUpperMLsd[nJ][ixEnv][3], pLoad->m_dMuFront3);

					m_dATensionLowerVLsd[nJ][ixEnv][3] = MAX(m_dATensionLowerVLsd[nJ][ixEnv][3], pLoad->m_dVuFront);
					m_dATensionLowerMLsd[nJ][ixEnv][3] = MAX(m_dATensionLowerMLsd[nJ][ixEnv][3], pLoad->m_dMuFront3);
				}
			}

			for(long ix=0; ix<3; ++ix)
			{
				m_dATensionUpperVLsd[nJ][ixEnv][ix] = fabs(m_dATensionUpperVLsd[nJ][ixEnv][ix]);
				m_dATensionUpperMLsd[nJ][ixEnv][ix] = fabs(m_dATensionUpperMLsd[nJ][ixEnv][ix]);
				m_dATensionLowerVLsd[nJ][ixEnv][ix] = fabs(m_dATensionLowerVLsd[nJ][ixEnv][ix]);
				m_dATensionLowerMLsd[nJ][ixEnv][ix] = fabs(m_dATensionLowerMLsd[nJ][ixEnv][ix]);
			}

			// �߰������� ��� �ޱ��� �����߾����� �ձ��� �ޱ� �ۿ���Ʈ�� ū�ɷ� �������ش�.
			// ����¿� ����ܸ� ���� �������ش�
			double dWidth = frM(UNIT_METER);
			CConcBasic *pConc;
			pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, FALSE, -1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ� - �ϸ� ����
			if(pConc != NULL)
			{
				if(ixEnv == eEnvService)
				{
					pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else if(ixEnv == eEnvService_Crack)
				{
					pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionLowerMLsd[nJ][eEnvService_Crack][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else
					pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				
				pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVLsd[nJ][ixEnv][0], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				if(bIsTwoLoad && frM(dHeightReal) > pConc->m_Ho)
				{
					double dHun = (frM(dHeightReal) - pConc->m_Ho) * 3;
					pConc->m_Hunch = dHun;
				}
			}

			pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, TRUE, -1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ� - ��� ����
			if(pConc != NULL)
			{
				if(ixEnv == eEnvService)
				{
					pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else if(ixEnv == eEnvService_Crack)
				{
					pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionUpperMLsd[nJ][eEnvService_Crack][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else
					pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][0], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				
				pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVLsd[nJ][ixEnv][0], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				if(bIsTwoLoad && frM(dHeightReal) > pConc->m_Ho)
				{
					double dHun = (frM(dHeightReal) - pConc->m_Ho) * 3;
					pConc->m_Hunch = dHun;
				}
			}

			pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, FALSE, FALSE, -1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ޱ� - ��� ����
			if(pConc != NULL)
			{

				double dMomentApply = bInWall? MAX(m_dATensionLowerMLsd[nJ][ixEnv][0], m_dATensionLowerMLsd[nJ][ixEnv][1]) : m_dATensionUpperMLsd[nJ][ixEnv][1];
				double dShearApply = bInWall? MAX(m_dATensionLowerVLsd[nJ][ixEnv][0], m_dATensionLowerVLsd[nJ][ixEnv][1]) : m_dATensionUpperVLsd[nJ][ixEnv][1];

				if(ixEnv == eEnvService)
				{
					pConc->m_MomentUse = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else if(ixEnv == eEnvService_Crack)
				{
					double dMoment_Crack = bInWall? MAX(m_dATensionLowerMLsd[nJ][eEnvService_Crack][0], m_dATensionLowerMLsd[nJ][eEnvService_Crack][1]) : m_dATensionUpperMLsd[nJ][eEnvService_Crack][1];
					pConc->m_MomentCrack	= GetValueUnitChange(dMoment_Crack, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else
					pConc->m_Moment = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				
				pConc->m_Shear = GetValueUnitChange(dShearApply, pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
			}

			pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, FALSE, TRUE, -1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ޱ� - �ϸ� ����
			if(pConc != NULL)
			{
				double dMomentApply = bInWall? MAX(m_dATensionUpperMLsd[nJ][ixEnv][0], m_dATensionUpperMLsd[nJ][ixEnv][1]) : m_dATensionLowerMLsd[nJ][ixEnv][1];
				double dShearApply = bInWall? MAX(m_dATensionUpperVLsd[nJ][ixEnv][0], m_dATensionUpperVLsd[nJ][ixEnv][1]) : m_dATensionLowerVLsd[nJ][ixEnv][1];
				
				if(ixEnv == eEnvService)
				{
					pConc->m_MomentUse = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else if(ixEnv == eEnvService_Crack)
				{
					double dMoment_Crack = bInWall? MAX(m_dATensionUpperMLsd[nJ][eEnvService_Crack][0], m_dATensionUpperMLsd[nJ][eEnvService_Crack][1]) : m_dATensionLowerMLsd[nJ][eEnvService_Crack][1];
					pConc->m_MomentCrack	= GetValueUnitChange(dMoment_Crack, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
				}
				else
					pConc->m_Moment = GetValueUnitChange(dMomentApply, pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;

				pConc->m_Shear = GetValueUnitChange(dShearApply, pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
			}

			// �ձ����� ������ġ �߰�
			if(bIsTwoLoad && !bInWall)
			{
				pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, FALSE, 0, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ�2 - �ϸ� ����
				if(pConc != NULL)
				{
					if(ixEnv == eEnvService)
					{
						pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else if(ixEnv == eEnvService_Crack)
					{
						pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionLowerMLsd[nJ][eEnvService_Crack][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else
						pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					
					pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVLsd[nJ][ixEnv][2], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				}

				pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, TRUE, 0, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ�2 - ��� ����
				if(pConc != NULL)
				{
					if(ixEnv == eEnvService)
					{
						pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else if(ixEnv == eEnvService_Crack)
					{
						pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionUpperMLsd[eEnvService_Crack][ixEnv][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else
						pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][2], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					
					pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVLsd[nJ][ixEnv][2], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				}

				//////////
				pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, FALSE, 1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ�3 - �ϸ� ����
				if(pConc != NULL)
				{
					if(ixEnv == eEnvService)
					{
						pConc->m_MomentUse = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else if(ixEnv == eEnvService_Crack)
					{
						pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionLowerMLsd[nJ][eEnvService_Crack][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else
						pConc->m_Moment = GetValueUnitChange(m_dATensionLowerMLsd[nJ][ixEnv][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					
					pConc->m_Shear = GetValueUnitChange(m_dATensionLowerVLsd[nJ][ixEnv][3], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				}

				pConc = m_pStd->m_pBri->GetConcDataFooting(nJ, TRUE, TRUE, 1, static_cast<ETypeEnvComboLsd>(ixEnv));		// �ձ�3 - ��� ����
				if(pConc != NULL)
				{
					if(ixEnv == eEnvService)
					{
						pConc->m_MomentUse = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else if(ixEnv == eEnvService_Crack)
					{
						pConc->m_MomentCrack	= GetValueUnitChange(m_dATensionUpperMLsd[nJ][eEnvService_Crack][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;
					}
					else
						pConc->m_Moment = GetValueUnitChange(m_dATensionUpperMLsd[nJ][ixEnv][3], pData->m_szTonfM, UNIT_CGS_TONFM) * dWidth;

					pConc->m_Shear = GetValueUnitChange(m_dATensionUpperVLsd[nJ][ixEnv][3], pData->m_szTonf, UNIT_CGS_TONF) * dWidth;
				}
			}
		}
	}
}

void CARcBridgeCalcFooting::MakeTableLoadSum(CXLFormatText *XLText, long nIndex, BOOL bUseLoad, BOOL bSafety, long nType, BOOL bAdd, long eEnvLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pStd = pBri->m_pARcBridgeDataStd;

	HGBOOL bLsd = pStd->IsLsdDesign();
	HGBOOL bNoneImpact = (bUseLoad && pStd->IsRailDesign2017() && pStd->m_nTypeFootingSafetyApplyCoefficientImpact == 1)? TRUE : FALSE;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	
	double dLoadV[eCountFootCheck];
	double dLoadH[eCountFootCheck];
	double dLoadM[eCountFootCheck];
	CStringArray szArrCombo;
	CString szCombo = _T("");

	CLoadFooting *pData;

	CString sTitle = _T("");
	CString strOpt = _T("");
	CString str = _T("");

	HGINT32 icCheck = bLsd ? eCountFootCheck : 4;
	int i=0; for(i=0; i<icCheck; i++)
	{
		if( bLsd )
			pData = GetLoadFootingLsd(nIndex, (ETypeEnvComboLsd)eEnvLimit, i);
		else
			pData =  (bUseLoad)? (bSafety? m_pListLoadSafty[nIndex].GetAt(i) : m_pListLoadUse[nIndex].GetAt(i)) : m_pListLoadUlt[nIndex].GetAt(i);

		//if(pData == NULL) continue;
		
		if(nType == 0)
		{
			dLoadV[i] = (!bAdd)? pData->m_dUpperLoadV : pData->m_dUpperLoadV_Add;
			dLoadH[i] = (!bAdd)? pData->m_dUpperLoadH : pData->m_dUpperLoadH_Add;
			dLoadM[i] = (!bAdd)? pData->m_dUpperLoadM : pData->m_dUpperLoadM_Add;

			
			CString szComboHead = bUseLoad ? "SV" : "UL";
			if(i==FOOTING_CHECK_EQ) szComboHead = "EQ";

			if(pBri->IsApplyEarthQuakeDesign() == FALSE && i == FOOTING_CHECK_EQ)
			{
				dLoadV[i] = 0;
				dLoadH[i] = 0;
				dLoadM[i] = 0;
				szComboHead = _T("");
			}
			
			szCombo.Format("%s%02d", szComboHead, bAdd? pData->m_nCombo_Add+1 : pData->m_nCombo+1);
			if(bNoneImpact)
				szCombo += _T("-S");

			szArrCombo.Add(szCombo);
		}
		else if(nType == 1)
		{
			dLoadV[i] = pData->m_dPileLoadV;
			dLoadH[i] = pData->m_dPileLoadH;
			dLoadM[i] = pData->m_dPileLoadM;
		}
		else return;
	}

	int nCntRow = (nType == 0)? 7 : 6;
	if (bLsd)
		nCntRow = 5;

	HGINT32 icFixRow = bLsd ? 2 : 3;
	XLText->AddTable(nCntRow-1, 24-1, icFixRow, 1);
	
	// Title
	int nWidthJoin2[] = { 4, 4, 4, 4, 4, 4 };
	CString TitleTemp1[] = { "�� ��", "�� ��\n�� ��", "�θ��Ʈ\n�� ��", "�����Ʈ\n�� ��", "�� �� ��", "�� ��" };
	CString TitleTemp2[] = { "�� ��", pBri->GetStringFootingCheckCase(eFootCheckAxial), pBri->GetStringFootingCheckCase(eFootCheckMomentMinus), pBri->GetStringFootingCheckCase(eFootCheckMomentPlus), 
	pBri->GetStringFootingCheckCase(eFootCheckHoriMax), pBri->GetStringFootingCheckCase(eFootCheckHoriMin) };
	
	CString *Title2 = bLsd ? TitleTemp2 : TitleTemp1;
	
	if (bLsd == FALSE)
	{
		int nWidthJoin1[] = { 4, 12, 4, 4 };
		char *Title1[] = { "�� ��", "�� �� ��", "�� �� ��", "�� ��" };

		long nCntMerge = 0;
		for(i=0; i<4; i++)
		{
			if(i==0 || i==2 || i==3) nCntMerge = 2;
			else nCntMerge = 0;

			XLText->AddMergeFormatText(nCntMerge, nWidthJoin1[i]-1,"$c%s",Title1[i]);
			XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
		}

		XLText->AddText("$n$h");
	}
	
	for(i=0; i<6; i++)
	{
		XLText->AddMergeFormatText(1,nWidthJoin2[i]-1,"$c%s",Title2[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
	}
	
	XLText->AddText("$n$n$h");
	
	// Data 
	char *sItem[] = { "������(V)", "�����(H)", "���Ʈ(M)", "Combo" };
	for(i=0; i<nCntRow-icFixRow; i++)
	{
		int j=0; for(j=0; j<6; j++)
		{
			if(j == 0) 
				str.Format("%s", sItem[i]);
			else if(bLsd == FALSE && j == 5) 
				str = _T("");
			else if(i == 0) 
				str.Format("$f3%.3f", dLoadV[j-1]) ;
			else if(i == 1) 
				str.Format("$f3%.3f", dLoadH[j-1]) ;
			else if(i == 2) 
				str.Format("$f3%.3f", dLoadM[j-1]) ;
			else if(i == 3) 
				str = szArrCombo.GetAt(j-1);
	
			if(bLsd == FALSE && j == 4 && pBri->IsApplyEarthQuakeDesign() == FALSE) // ������ + �������� �̰��
			{
				XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",_T("-"));
			}
			else
			{
				XLText->AddMergeFormatText(0,nWidthJoin2[i]-1,"$c%s",str);
			}
			XLText->AddFormatText("$m+0%d",nWidthJoin2[i]);
		}
		XLText->AddText("$n$h");
	}
	if(nType == 1 && (pData->m_dDistCenttoDosim != 0)) 
		XLText->AddFormatText("$h$i��)$r$e �����ۿ�Moment(M) = ������(V) x (���� �߾ӿ��� ���ɱ����� �Ÿ� - M/V)$n");
	else if(nType == 1 && (pData->m_dDistCenttoDosim == 0)) 
		XLText->AddFormatText("$h$i��)$r$e ���� �߾Ӱ� ���� ������ ��ġ�ϹǷ� �����߾� �ۿ���� ���$n");
	XLText->AddText("$n$h");
}

void CARcBridgeCalcFooting::MakeTableCenterPile(CXLFormatText *XLText, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	
	CString str = _T("");
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL || pBri->IsPileBase(nJijum)==FALSE) return;

	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	BOOL bInWall = (nJijum == 0 || nJijum == pBri->GetCountJijum()-1)? FALSE:TRUE;
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	long nRowCnt = pPile->m_nCountOrg+2;
	XLText->AddTable(nRowCnt-1, 24-1, 1, 1);

	int nWidthJoin1[] = { 4, 4, 4, 8, 4 };

	CString strTitle1, strTitle2;
	strTitle1.Format("�� ��(%s)", pData->m_szM);
	strTitle2.Format("�� ��(%s)", pData->m_szM);
	
	// Title
	char *Title1[] = { "�� ��", strTitle1.GetBuffer(strTitle1.GetLength()), "�� ��", "�Ÿ� x ����", strTitle2.GetBuffer(strTitle2.GetLength()) };
	long nCntMerge = 0;
	long i=0; for(i=0; i<5; i++)
	{
		XLText->AddMergeFormatText(nCntMerge, nWidthJoin1[i]-1,"$c%s",Title1[i]);
		XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$h");

	pPile->SetPileArray();

	const CString strDecimalPoint3 = bLsd ? "" : "$f3";
	
	double dDist	= (bInWall)? toM(pFooting->GetWidth()):0;
	double dEa		= 0;
	double dCalc	= 0;
	double dDosim	= 0;
	double dEaTot	= 0;
	double dCalcTot	= 0;
	// Data
	for(i=0; i<nRowCnt-1; i++)
	{
		if(i>=0 && i<nRowCnt-2)
		{
			if(!bInWall) dDist += toM(pPile->m_ArrayOrg[i][0]);
			else dDist -= toM(pPile->m_ArrayOrg[i][0]);
			dEa	= pPile->m_ArrayOrg[i][1];
			dCalc = dDist * dEa;
			
			dEaTot	+= dEa;
			dCalcTot += dCalc;
		}

		if(i == nRowCnt-2) dDosim = dCalcTot / dEaTot;

		int j=0; for(j=0; j<5; j++)
		{
			if(j==0)
			{
				if(i==nRowCnt-2) str = _T("��");
				else str.Format("%d ��", i+1);
			}
			else if(j==1 && i==nRowCnt-2) str = _T("");
			else if(j == 1) str.Format("%s%s", strDecimalPoint3, pData->GetStrDoubleUnitChange(dDist, UNIT_CGS_M, pData->m_szM));
			else if(j==2 && i==nRowCnt-2) str.Format("%s%.3f", strDecimalPoint3, dEaTot);
			else if(j == 2) str.Format("%s%.3f", strDecimalPoint3, dEa);
			else if(j==3 && i==nRowCnt-2) str.Format("%s%s", strDecimalPoint3, pData->GetStrDoubleUnitChange(dCalcTot, UNIT_CGS_M, pData->m_szM));
			else if(j == 3) str.Format("%s%s", strDecimalPoint3, pData->GetStrDoubleUnitChange(dCalc, UNIT_CGS_M, pData->m_szM));
			else if(j==4 && i==nRowCnt-2) str.Format("%s%s", strDecimalPoint3, pData->GetStrDoubleUnitChange(dDosim, UNIT_CGS_M, pData->m_szM));
			else if(j == 4) str = _T("");
			
			XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcFooting::MakeTableCenterPileLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	const HGINT32 icRow = GetCountLoadFooting(pBri, FALSE);
	const HGINT32 icCol = 5;
	const HGINT32 icWidthCell = 4;
	XLText->AddTable(icRow, icCol*icWidthCell - 1, 1, 1);

	CString szUnit_Moment = pDataStd->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pDataStd->GetStringUnitType(UNIT_CGS_TONF);

	CString strTemp  = "V("+szUnit_Force+")";
	CString strTemp1 = "H("+szUnit_Force+")";
	CString strTemp2 = "e("+pDataStd->m_szM+")";
	CString strTemp3 = "Mc("+szUnit_Moment+")";

	char *Title[] = { "����", strTemp.GetBuffer(strTemp.GetLength()), strTemp1.GetBuffer(strTemp1.GetLength()), strTemp2.GetBuffer(strTemp2.GetLength()), strTemp3.GetBuffer(strTemp3.GetLength()) };

	// Title
	long j=0; for(j=0; j<5; j++)
	{
		XLText->AddMergeFormatText(0,icWidthCell-1,"$c%s",Title[j]);
		XLText->AddFormatText("$m+0%d",icWidthCell);
	}
	XLText->AddText("$n$h");

	CString str;
	CString strOpt = "";
	CLoadFooting *pData = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;

		for (HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);

			for(j=0; j<5; j++)
			{
				if(pData == NULL)
					str = _T("-");
				else if(j == 0)
					str = pData->m_sCombo;
				else if(j == 1)
					str.Format("%s", pDataStd->GetStrDouble(pData->m_dPileLoadV, UNIT_CGS_TONF));
				else if(j == 2)
					str.Format("%s", pDataStd->GetStrDouble(pData->m_dPileLoadH, UNIT_CGS_TONF));
				else if(j == 3)
					str.Format("%s", pDataStd->GetStrDouble(pData->m_dE - pData->m_dDistCenttoDosim, UNIT_CGS_M));
				else if(j == 4)
					str.Format("%s", pDataStd->GetStrDouble(pData->m_dPileLoadM, UNIT_CGS_TONFM, TRUE));

				XLText->AddMergeFormatText(0, icWidthCell-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", icWidthCell);
			}

			XLText->AddText("$n$h");			
		}
	}

	XLText->AddText(_T("$n$h"));
}

void CARcBridgeCalcFooting::MakeTableSectionForceSum(CXLFormatText *XLText, long nIndex)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	BOOL bCheckAdd = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;

	if(bCheckAdd)
	{
		long nTypeFoot = m_pListLoadUse[nIndex].GetAt(0)->m_pFooting->GetTypeByInputValue();
		if(nTypeFoot==0)	 bCheckAdd = FALSE;
	}
	BOOL bFootExt = (m_pListLoadUse[nIndex].GetAt(0)->m_pFooting->m_dFELW!=0.0 || m_pListLoadUse[nIndex].GetAt(0)->m_pFooting->m_dFERW!=0.0)? TRUE : FALSE;

	CLoadFooting *pDataUse(NULL);
	CLoadFooting *pDataUlt(NULL);
	
	CString sTitle = _T("");
	CString strOpt = _T("");
	CString str = _T("");
	
	int nCntRow = 10;	// �ɼ�
	if(bCheckAdd && bFootExt) nCntRow += 4;
	if(bCheckAdd && pBri->m_bCheckFrontFootTrap2) nCntRow += 4;
	
	int nCntCol = 7;
	XLText->AddFormatText("$h$c-$r$e����m��$n$h");
	XLText->AddTable(nCntRow-1, 24-1, 1, 9);
	
	// Title
	int nWidthJoin1[] = { 9, 4, 4, 4, 3 };
	int nWidthJoin2[] = { 3, 4, 2, 4, 4, 4, 3 };

	CString str1, str2, str3;
	str1.Format("�������\n���Ʈ(%s)", pDataStd->m_szTonfM);
	str2.Format("�������\n���ܷ�(%s)", pDataStd->m_szTonf);
	str3.Format("�������\n���Ʈ(%s)", pDataStd->m_szTonfM);
	char *Title1[] = { "�� ��", str1.GetBuffer(str1.GetLength()), str2.GetBuffer(str2.GetLength()), str3.GetBuffer(str3.GetLength()), "�� ��" };
	char *Title2[] = { "����ִ�", "�θ��Ʈ �ִ�", "�����Ʈ �ִ�", "" };
	
	long i=0; for(i=0; i<5; i++)
	{		
		XLText->AddMergeFormatText(1, nWidthJoin1[i]-1,"$c%s",Title1[i]);
		XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");
	
	long nCountData = 2;
	if(bCheckAdd && bFootExt) nCountData++;
	if(bCheckAdd && pBri->m_bCheckFrontFootTrap2) nCountData++;
	// Data 
	for(i=0; i<4; i++)
	{
		BOOL bNormal = (i<3) ? TRUE : FALSE;
		pDataUse = m_pListLoadUse[nIndex].GetAt(i);
		pDataUlt = m_pListLoadUlt[nIndex].GetAt(i);

		long j=0; for(j=0; j<nCntCol; j++)
		{
			if(j == 0)
			{
				str.Format("%s", bNormal ? "����":"������");
				if(i == 0)
				{
					XLText->AddMergeFormatText(nCountData*3-1, 0,"$c%s",str);
				}
			}
			else if(j == 1) str = Title2[i];
			else if(j == 2) str = _T("�� ��");
			else if(j == 3) str.Format("$f3%.3f", pDataUlt->m_dMuFront);
			else if(j == 4) str.Format("$f3%.3f", pDataUlt->m_dVuFront);
			else if(j == 5) str.Format("$f3%.3f", pDataUse->m_dMuFront);
			else if(j == 6) str = _T("");
			
			if(0) //j==0 && i==0)
			{
				// $md11" �̰ɷ� ������ ��� Row������ ���ڸ��� �Ǹ� ������ �ȵȴ�.
				// �׷��� ������ ���������� �����ϴ°ɷ� �ٲ�,,
				if(pBri->m_bCheckFrontFootTrap2)
				{
					XLText->AddFormatText(bCheckAdd? ((pBri->m_bCheckFrontFootTrap2 && bFootExt) ? "$md11" : "$md8") : "$md5");
				}
				else
					XLText->AddFormatText((bCheckAdd && bFootExt)? "$md8" : "$md5");
			}
			if(j==0 && i==3)
			{
				XLText->AddFormatText("$md%d", nCountData-1);
			}
			if(j==1)
			{
				XLText->AddFormatText("$md%d", nCountData-1);
			}

			if((j==3 || j==4 || j==5) &&bNormal==FALSE && pBri->IsApplyEarthQuakeDesign()==FALSE)
			{
				str = _T("-");
			}

			if(j==0 && i==3)
				XLText->AddMergeFormatText(0,nWidthJoin2[j] + nWidthJoin2[j + 1] -1,"$c%s",str);
			else
				XLText->AddMergeFormatText(0,nWidthJoin2[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin2[j]);
		}
		XLText->AddText("$n$h");

		if(bCheckAdd)
		{
			if(bFootExt)
			{
				long j=0; for(j=0; j<nCntCol; j++)
				{
					if(j == 0) str.Format("%s", bNormal ? "����":"������");
					else if(j == 1) str = Title2[i];
					else if(j == 2) str = _T("�ձ�2");
					else if(j == 3) str.Format("$f3%.3f", pDataUlt->m_dMuFront2);
					else if(j == 4) str.Format("$f3%.3f", pDataUlt->m_dVuFront);
					else if(j == 5) str.Format("$f3%.3f", pDataUse->m_dMuFront2);
					else if(j == 6) str = _T("");
					
					if((j==3 || j==4 || j==5) &&bNormal==FALSE && pBri->IsApplyEarthQuakeDesign()==FALSE)
					{
						str = _T("-");
					}
					XLText->AddMergeFormatText(0,nWidthJoin2[j]-1,"$c%s",str);
					XLText->AddFormatText("$m+0%d",nWidthJoin2[j]);
				}
				XLText->AddText("$n$h");
			}

			if(pBri->m_bCheckFrontFootTrap2)
			{
				for(j=0; j<nCntCol; j++)
				{
					if(j == 0) str.Format("%s", bNormal ? "����":"������");
					else if(j == 1) str = Title2[i];
					else if(j == 2) str = _T("�ձ�3");
					else if(j == 3) str.Format("$f3%.3f", pDataUlt->m_dMuFront3);
					else if(j == 4) str.Format("$f3%.3f", pDataUlt->m_dVuFront);
					else if(j == 5) str.Format("$f3%.3f", pDataUse->m_dMuFront3);
					else if(j == 6) str = _T("");
					
					if((j==3 || j==4 || j==5) &&bNormal==FALSE && pBri->IsApplyEarthQuakeDesign()==FALSE)
					{
						str = _T("-");
					}
					XLText->AddMergeFormatText(0,nWidthJoin2[j]-1,"$c%s",str);
					XLText->AddFormatText("$m+0%d",nWidthJoin2[j]);
				}
				XLText->AddText("$n$h");
			}
		}

		for(j=0; j<nCntCol; j++)
		{
			if(j == 0) str.Format("%s", bNormal ? "����":"������");
			else if(j == 1) str = Title2[i];
			else if(j == 2) str = _T("�� ��");
			else if(j == 3) str.Format("$f3%.3f", pDataUlt->m_dMuBack);
			else if(j == 4) str.Format("$f3%.3f", pDataUlt->m_dVuBack);
			else if(j == 5) str.Format("$f3%.3f", pDataUse->m_dMuBack);
			else if(j == 6) str = _T("");
			
			if((j==3 || j==4 || j==5) &&bNormal==FALSE && pBri->IsApplyEarthQuakeDesign()==FALSE)
			{
				str = _T("-");
			}
			XLText->AddMergeFormatText(0,nWidthJoin2[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin2[j]);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");

	// Title
	nCntRow = 6;
	if(bCheckAdd && bFootExt) nCntRow += 2;
	if(bCheckAdd && pBri->m_bCheckFrontFootTrap2) nCntRow += 2;
	XLText->AddTable(nCntRow-1, 24-1, 1, 9);
	
	for(i=0; i<5; i++)
	{		
		XLText->AddMergeFormatText(1, nWidthJoin1[i]-1,"$c%s",Title1[i]);
		XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$n$h");

	// DATA
	char *szItem[] = { "�ձ� ���� �����", "�ޱ� ���� �����", "�ձ�2 ���� �����", "�ձ�3 ���� �����" };
	int nWidthJoin3[] = { 6, 3, 4, 4, 4, 3 };
	long nCntItem = bCheckAdd? (pBri->m_bCheckFrontFootTrap2 ? 4 :3) : 2;
	for(i=0; i<nCntItem; i++)
	{
		if(i==2 && bFootExt==FALSE) continue;
		long j=0; for(j=0; j<6; j++)
		{
			if(j == 0) str.Format("%s", szItem[i]);
			else if(j == 1) str = "�������";
			else if(j == 2) str.Format("$f3%.3f", m_dATensionUpperMUlt[nIndex][i]);
			else if(j == 3) str.Format("$f3%.3f", m_dATensionUpperVUlt[nIndex][i]);
			else if(j == 4) str.Format("$f3%.3f", m_dATensionUpperMUse[nIndex][i]);
			else if(j == 5) str = _T("");

			if(str == "$f30.000") str = "-";
			if(j==0) XLText->AddFormatText("$md1");
			if((i==0 || i==2) && (j>1 && j<6)) XLText->AddFormatText("$b034");

			XLText->AddMergeFormatText(0,nWidthJoin3[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin3[j]);	
		}
		XLText->AddText("$n$h");

		for(j=0; j<6; j++)
		{
			if(j == 0) str.Format("%s", szItem[i]);
			else if(j == 1) str = "�ϸ�����";
			else if(j == 2) str.Format("$f3%.3f", m_dATensionLowerMUlt[nIndex][i]);
			else if(j == 3) str.Format("$f3%.3f", m_dATensionLowerVUlt[nIndex][i]);
			else if(j == 4) str.Format("$f3%.3f", m_dATensionLowerMUse[nIndex][i]);
			else if(j == 5) str = _T("");
			
			if(str == "$f30.000") str = "-";
			if(i==1 && (j>1 && j<6)) XLText->AddFormatText("$b034");
			
			XLText->AddMergeFormatText(0,nWidthJoin3[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin3[j]);	
		}
		XLText->AddText("$n$h");
	}
}

void CARcBridgeCalcFooting::MakeTableSectionForceSum_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	int nWidthJoin1[] = { 13, 4, 4, 3 };
	int nWidthJoin2[] = { 5, 4, 4, 4, 4, 3 };

	CString str1, str2;
	str1.Format("���Ʈ(%s)", pDataStd->m_szTonfM);
	str2.Format("���ܷ�(%s)", pDataStd->m_szTonf);
	char *Title1[] = { "�� ��", str1.GetBuffer(str1.GetLength()), str2.GetBuffer(str2.GetLength()), "�� ��" };

	long nCountCase = pBri->GetCountApplyEnvType();
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$c-$r$e����mm��$n$h");
	XLText->AddTable(nCountCase * 4, 24-1, 1, 13);

	long i=0; for(i=0; i<4; i++)
	{		
		XLText->AddMergeFormatText(0, nWidthJoin1[i]-1,"$c%s",Title1[i]);
		XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$h");

	for (HGINT32 ixFront = 0; ixFront < 2; ++ixFront)
	{
		const CString strFront = ixFront == 0 ? "�ձ� ���� �����" : "�ޱ� ���� �����";

		for (HGINT32 ixEnvType = eEnvLimit; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
		{
			if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE)
				continue;

			const CString strEnvType = pBri->GetStringComboEnvType_Lsd(static_cast<ETypeEnvComboLsd>(ixEnvType), TRUE, TRUE);

			for (HGINT32 ixUpper = 0; ixUpper < 2; ++ixUpper)
			{
				const CString strUpper = ixUpper == 0 ? "�������" : "�ϸ�����";

				if (ixEnvType == eEnvLimit && ixUpper == 0)
					XLText->AddMergeFormatText(nCountCase * 2 - 1,nWidthJoin2[0]-1,"$c%s",strFront);
				
				XLText->AddFormatText("$m+0%d",nWidthJoin2[0]);

				if (ixUpper == 0)
					XLText->AddMergeFormatText(1,nWidthJoin2[1]-1,"$c%s",strEnvType);
				
				XLText->AddFormatText("$m+0%d",nWidthJoin2[1]);
				
				XLText->AddMergeFormatText(0,nWidthJoin2[2]-1,"$c%s",strUpper);
				XLText->AddFormatText("$m+0%d",nWidthJoin2[2]);
				
				const HGDOUBLE dMoment = ixUpper == 0 ? m_dATensionUpperMLsd[nJijum][ixEnvType][ixFront] : m_dATensionLowerMLsd[nJijum][ixEnvType][ixFront];
				XLText->AddMergeFormatText(0,nWidthJoin2[3]-1,"$c%s",pDataStd->GetStrDouble(dMoment, UNIT_CGS_TONFM));
				XLText->AddFormatText("$m+0%d",nWidthJoin2[3]);

				const HGDOUBLE dShearForce = ixUpper == 0 ? m_dATensionUpperVLsd[nJijum][ixEnvType][ixFront] : m_dATensionLowerVLsd[nJijum][ixEnvType][ixFront];
				XLText->AddMergeFormatText(0,nWidthJoin2[4]-1,"$c%s",pDataStd->GetStrDouble(dShearForce, UNIT_CGS_TONF));
				XLText->AddFormatText("$m+0%d",nWidthJoin2[4]);
				
				XLText->AddMergeFormatText(0,nWidthJoin2[5]-1,"$c%s","");

				XLText->AddText("$n$h");
			}
		}
	}

	XLText->AddText("$n$h");
}

BOOL CARcBridgeCalcFooting::GetCheckValueOverturning(long nJijum, double *dMr, double *dMo, BOOL bNormal)
{
	BOOL bOK	= TRUE;
	double Mr	= 0.;
	double Mo	= 0.;

	if(m_pListLoadSafty[nJijum].GetSize() == 0)
	{
		*dMr = Mr;
		*dMo = Mo;

		return FALSE;
	}

	if(bNormal)
	{
		double dSF	= 0.;
		long i=0; for(i=0; i<3; i++)
		{
			CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(i);
			if(i == 0)
			{
				dSF = pLoadFooting->m_dMrTotal / pLoadFooting->m_dMoTotal;
				Mr = pLoadFooting->m_dMrTotal;
				Mo = pLoadFooting->m_dMoTotal;
			}
			if(dSF > (pLoadFooting->m_dMrTotal / pLoadFooting->m_dMoTotal))
			{
				dSF = pLoadFooting->m_dMrTotal / pLoadFooting->m_dMoTotal;
				Mr = pLoadFooting->m_dMrTotal;
				Mo = pLoadFooting->m_dMoTotal;
			}
			if(pLoadFooting->m_bOkOverturning == FALSE) bOK = FALSE;
		}
	}
	else
	{
		CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(3);
		Mr = pLoadFooting->m_dMrTotal;
		Mo = pLoadFooting->m_dMoTotal;
		if(pLoadFooting->m_bOkOverturning == FALSE) bOK = FALSE;
	}

	*dMr = Mr;
	*dMo = Mo;

	return bOK;
}

BOOL CARcBridgeCalcFooting::GetCheckValueSliding(long nJijum, double *dHu, double *dH, BOOL bNormal)
{
	BOOL	bOK = TRUE;
	double Hu	= 0.;
	double H	= 0.;

	if(m_pListLoadSafty[nJijum].GetSize() == 0)
	{
		*dHu = Hu;
		*dH = H;

		return FALSE;
	}

	if(bNormal)
	{
		double dSF	= 0.;
		long i=0; for(i=0; i<3; i++)
		{
			CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(i);
			if(i == 0)
			{
				dSF = pLoadFooting->m_dHu / fabs(pLoadFooting->m_dHorTotal);
				Hu = pLoadFooting->m_dHu;
				H = fabs(pLoadFooting->m_dHorTotal);
			}
			if(dSF > (pLoadFooting->m_dHu / fabs(pLoadFooting->m_dHorTotal)))
			{
				dSF = pLoadFooting->m_dHu / fabs(pLoadFooting->m_dHorTotal);
				Hu = pLoadFooting->m_dHu;
				H = fabs(pLoadFooting->m_dHorTotal);
			}
			if(pLoadFooting->m_bOkSliding == FALSE) bOK = FALSE;
		}
	}
	else
	{
		CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(3);
		Hu = pLoadFooting->m_dHu;
		H = fabs(pLoadFooting->m_dHorTotal);
		if(pLoadFooting->m_bOkSliding == FALSE) bOK = FALSE;
	}
	*dHu = Hu;
	*dH = H;

	return bOK;
}

BOOL CARcBridgeCalcFooting::GetCheckValueSupport(long nJijum, double *dQa, double *dQmax, BOOL bNormal, ELSDLoadGroupKind eKindLsd)
{
	BOOL bOK	= TRUE;
	double Qa	= 0.;
	double Qmax	= 0.;

	if(m_pStd->m_pBri->IsBoxType())
	{
		CRcBridgeRebar *pBri = m_pStd->m_pBri;
		CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

		long nJijum = 0;
		CFootingApp *pFooting = pBri->GetFooting(nJijum);
		if(pFooting == NULL) return FALSE;

		BOOL bEarthquake = !bNormal;
		double Q_Normal = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Q_EarthQ = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Qa	= (bEarthquake) ? Q_EarthQ : Q_Normal;
		double Qa_(0);
		double dQmaxApply(0);

		if(pData->IsLsdDesign())
		{
			double dFactor(0);
			CLsdFootingDesign* pFootingDesign = GetLsdFootingDesign(0);
			if(pFootingDesign)
			{
				dFactor = pFootingDesign->sCoffSF.supporResitCoff[pFootingDesign->m_ixTypeGround][eKindLsd];
			}
			
			ETypeEnvComboLsd eEnvQmax = eEnvService;
			long nIndexQa = 0;
			if(eKindLsd != eLSD_COMBINATION_SERVICE)
			{
				nIndexQa = eKindLsd == eLSD_COMBINATION_LIMIT ? 1 : 2;
				eEnvQmax = eKindLsd == eLSD_COMBINATION_LIMIT ? eEnvLimit : eEnvExtremeI;
			}
			double Qe  = pFooting->m_bAutoCalcSupportForce ? (m_dQaEquation * dFactor) : pFootingDesign->sCoffSF.Qa_user[nIndexQa];//eLSD_COMBINATION_LIMIT : eLSD_COMBINATION_EXTREME];
			Qa_ = Qe;	// MIN(Qa, Qe);
			dQmaxApply = m_dQMax[eEnvQmax];
		}
		else
		{
			double SF  = (bEarthquake) ? 2 : 3;
			double Qe  = m_dQaEquation / SF;
			Qa_ = MIN(Qa, Qe);
			dQmaxApply = m_dQMax[0];
		}
		
		*dQa = Qa_;
		*dQmax = dQmaxApply;
		bOK = Qa_ >= dQmaxApply ? TRUE : FALSE;

		return bOK;
	}

	if(m_pListLoadSafty[nJijum].GetSize() == 0)
	{
		*dQa = Qa;
		*dQmax = Qmax;

		return FALSE;
	}

	if(bNormal)
	{
		double dSF	= 0.;
		long i=0; for(i=0; i<3; i++)
		{
			CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(i);

			double dQuse = 0;
			if(m_pStd->m_pBri->m_bApplyMassCalcSafe && pLoadFooting->m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
				dQuse = pLoadFooting->m_dQmax + pLoadFooting->m_dQmaxMass;
			else dQuse = pLoadFooting->m_dQmax;

			if(i == 0)
			{
				Qmax = dQuse;
				dSF = pLoadFooting->m_dQa / Qmax;
				Qa = pLoadFooting->m_dQa;
			}
			if(dSF > (pLoadFooting->m_dQa / dQuse))
			{
				Qmax = dQuse;
				dSF = pLoadFooting->m_dQa / Qmax;
				Qa = pLoadFooting->m_dQa;
			}
			if(pLoadFooting->m_bOkSupportForce == FALSE) bOK = FALSE;
		}
	}
	else
	{
		CLoadFooting *pLoadFooting = m_pListLoadSafty[nJijum].GetAt(3);
		if(m_pStd->m_pBri->m_bApplyMassCalcSafe && pLoadFooting->m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
			Qmax = pLoadFooting->m_dQmax + pLoadFooting->m_dQmaxMass;
		else Qmax = pLoadFooting->m_dQmax;

		Qa = pLoadFooting->m_dQa;
		if(pLoadFooting->m_bOkSupportForce == FALSE) bOK = FALSE;
	}
	*dQa = Qa;
	*dQmax = Qmax;

	return bOK;
}

void CARcBridgeCalcFooting::GetCheckValuePileForce(long nJijum, double *dRa, double *dPmax, BOOL bNormal)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	if(pBri->IsBoxType() == FALSE)
	{
		long nCase = 0;
		long nTypeCase = PILE_CASE_USE;
		if(bNormal == FALSE)
			nTypeCase = bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH;
		*dPmax = pDataStd->GetValueUnitType(pCalcPile->GetMaxReaction(!bNormal, nCase, nTypeCase), UNIT_CGS_TONF, pDataStd->m_szTonf);
	}
	else
	{
		long nJijum = 0;
 		CFootingApp *pFooting = pBri->GetFooting(nJijum);
 		if(pFooting == NULL) return;
 		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
 		if(pPile == NULL) return;

		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		long nPileSu = pPile->m_nCountOrg;
		if(nPileSu == 0) return;

		long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

		double dVmax = 0;
		for(long nT = 0; nT < nCase; nT++)
		{
			BOOL bNormalCase = (nT!=0) ? FALSE : TRUE;
			if(bNormal != bNormalCase) continue;

			for(long n=0; n<nPileSu; n++)
			{
				CPileLoad *pLoad = NULL; 
				if(nT == 0)
					pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, n, TRUE);
				else
				{
					if(bLsd)
						pLoad = pCalcPile->m_pListExtreme.GetAt(n);
					else
						pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);
				}

				double dDistPile = GetDistMaxPile(n);
				if(!bLsd)
					dDistPile = toM(dDistPile);

				dVmax = MAX(dVmax, GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pDataStd->m_szTonf) * dDistPile);
			}
		}
		*dPmax = dVmax;
	}

	long nTypeCase = bNormal? PILE_CASE_USE : (bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH);
	*dRa = (bLsd) ? pCalcPile->m_Ra[nTypeCase] : pDataStd->GetValueUnitType(pCalcPile->m_Ra[nTypeCase], UNIT_CGS_TONF, pDataStd->m_szTonf);
}

void CARcBridgeCalcFooting::GetCheckValuePileBending(long nJijum, double *dFsa, double *dFmax, BOOL bNormal, BOOL bTens)
{
	CARcBridgeDataStd *pDataStd = m_pStd->m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);

	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	if(m_pStd->m_pBri->IsBoxType() == FALSE)
	{
		long nCase = 0;
		CFootingApp *pFooting = m_pStd->m_pBri->GetFootingByJijumNumber(nJijum);

		long nTypePileCase = bNormal? PILE_CASE_USE : (bLsd ? PILE_CASE_EXTREME : PILE_CASE_NORMAL);

		*dFmax = pCalcPile->GetMaxStress(!bNormal, nCase, nTypePileCase, TRUE);	// ���� ����� ���� ��ȯ��
		if(pFooting->m_exFooting.m_Pile.IsPHC() )
		{
			*dFsa = pDataStd->GetValueUnitType(pCalcPile->m_PHC_Fca[nTypePileCase], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
		}
		else
		{
			*dFsa = pDataStd->GetValueUnitType(pCalcPile->m_dArrFsa[nTypePileCase][nCase], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
		}
	}
	else
	{
		CRcBridgeRebar *pBri = m_pStd->m_pBri;
		long nJijum = 0;
		CFootingApp *pFooting = pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		long nPileSu = pPile->m_nCountOrg;
		if(nPileSu == 0) return;

		long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

		if(pPile->IsPHC())
		{
			double Ae  = frCm2(pPile->m_dAe_PHC);
			double Ze  = frCm3(pPile->m_dZe_PHC);
			double fce = GetValueUnitChange(pPile->m_dFce2_PHC, UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
			
			double dFa[2];
			if(bTens)
			{
				dFa[0] = GetValueUnitChange(pCalcPile->m_PHC_Fta[PILE_CASE_USE], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				dFa[1] = GetValueUnitChange(pCalcPile->m_PHC_Fta[bLsd ? PILE_CASE_EXTREME : PILE_CASE_EARTH], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
			}
			else
			{
				dFa[0] = GetValueUnitChange(pCalcPile->m_PHC_Fca[PILE_CASE_USE], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				dFa[1] = GetValueUnitChange(pCalcPile->m_PHC_Fca[bLsd ? PILE_CASE_EXTREME : PILE_CASE_EARTH], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
			}

			double ftMax = 0;
			for(long nT=0; nT<nCase; nT++)
			{
				BOOL bNormalCase = (nT!=0) ? FALSE : TRUE;
				if(bNormal != bNormalCase) continue;

				for(long n=0; n<nPileSu; n++)
				{
					CPileLoad *pLoad = NULL;
					if(nT == 0)
						pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, n, TRUE);
					else
					{
						if(bLsd)
							pLoad = pCalcPile->GetPileLoad(PILE_CASE_EXTREME, n);
						else
							pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);
					}

					double dDistPile = GetDistMaxPile(n);
					if(!bLsd)
						dDistPile = toM(dDistPile);

					double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pDataStd->m_szTonf) * dDistPile;
					double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pDataStd->m_szTonfM) * dDistPile;

// 					double ft   = fce - dMi*1000000/Ze + dVi*1000/Ae;
// 					double fc   = fce + dMi*1000000/Ze + dVi*1000/Ae;
// 					double fca  = bEarthQuake? dFca[1] : dFca[0];
// 					double fta  = bEarthQuake? dFta[1] : dFta[0];
					if(bTens)
					{
						if(bLsd)
							ftMax = MIN(ftMax, fce - dMi/Ze + dVi/Ae);
						else
							ftMax = MIN(ftMax, fce - dMi*1000000/Ze + dVi*1000/Ae);
					}
					else
					{
						if(bLsd)
							ftMax = MAX(ftMax, fce + dMi/Ze + dVi/Ae);
						else
							ftMax = MAX(ftMax, fce + dMi*1000000/Ze + dVi*1000/Ae);
					}
				}
			}
			*dFmax = ftMax;
			*dFsa  = bNormal ? dFa[0] : dFa[1];
		}
		else
		{
			double yIp  = toM(pPile->m_dDia-pPile->m_dT2*2)/2.0/toM4(frCm4(pCalcPile->m_I33));
			double Ap   = toM2(frCm2(pCalcPile->m_Ap));

			double fmax = 0;
			for(long nT=0; nT<nCase; nT++)
			{
				BOOL bNormalCase = (nT!=0) ? FALSE : TRUE;
				if(bNormal != bNormalCase) continue;

				for(long n=0; n<nPileSu; n++)
				{
					CPileLoad *pLoad = NULL;
					if(nT == 0)
						pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, n, TRUE);
					else
					{
						if(bLsd)
							pLoad = pCalcPile->GetPileLoad(PILE_CASE_EXTREME, n);
						else
							pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);
					}

					double dDistPile = GetDistMaxPile(n);
					if(!bLsd)
						dDistPile = toM(dDistPile);

					double dVi = pLoad->m_dForceVert * dDistPile;
					double dMi = pLoad->m_dMoment * dDistPile;

					fmax = MAX(fmax, GetValueUnitChange((dVi/Ap + dMi*yIp)/10,UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2));

				}
			}
			*dFmax = fmax;
			if(bTens)
				*dFsa  = pCalcPile->GetTsa(!bNormal);
			else
				*dFsa  = pCalcPile->GetFsa(!bNormal);
		}
	}
}

void CARcBridgeCalcFooting::GetCheckValuePileShear(long nJijum, double *dTsa, double *dTmax, BOOL bNormal)
{
	CARcBridgeDataStd *pDataStd = m_pStd->m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);

	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	if(m_pStd->m_pBri->IsBoxType() == FALSE)
	{
		long nTypePileCase = bNormal? PILE_CASE_USE : (bLsd ? PILE_CASE_EXTREME : PILE_CASE_EARTH);

		double D		= toCm(pCalcPile->m_pPile->m_dDia);
		double T1		= toCm(pCalcPile->m_pPile->m_dT1);
		double T2		= toCm(pCalcPile->m_pPile->m_dT2);
		double D1		= D-T2*2;
		double d		= D-T1*2;
		double alpha	= 4*(D1*D1+D1*d+d*d) / (3*(D1*D1+d*d));	
		double Ap   = toM2(frCm2(pCalcPile->m_Ap));	
		double Ho   = 0;
		long nCaseMax = 0;
		if(bNormal)
		{
			double dMaxValue = 0;
			long nCase = 0;
			for(nCase=0; nCase<3; nCase++)
			{
//				double HoFix = 0.;
				double HoFix = fabs(pCalcPile->GetValueMaxMinPileHead(nCase,PILE_FORCE_HORI,-1,TRUE,TRUE,PILE_CASE_USE,TRUE));
// 				double HoHin = 0.;
				double HoHin = fabs(pCalcPile->GetValueMaxMinPileHead(nCase,PILE_FORCE_HORI,-1,FALSE,TRUE,PILE_CASE_USE,TRUE));

				double dTsa_case = pCalcPile->m_dArrTsa[nTypePileCase][nCase];

				if(dMaxValue < MAX(HoFix, HoHin) / dTsa_case)
				{
					Ho = MAX(HoFix, HoHin);
					nCaseMax = nCase;
					dMaxValue = MAX(HoFix, HoHin) / dTsa_case;
				}
			}
		}
		else
		{
			nCaseMax = 3;
			double HoFix = fabs(pCalcPile->GetValueMaxMinPileHead(nCaseMax,PILE_FORCE_HORI,-1,TRUE,TRUE, nTypePileCase,TRUE));
			double HoHin = fabs(pCalcPile->GetValueMaxMinPileHead(nCaseMax,PILE_FORCE_HORI,-1,FALSE,TRUE, nTypePileCase,TRUE));
			Ho = MAX(HoFix, HoHin);
		}
		double Tmax = GetValueUnitChange((alpha * Ho / Ap)*0.1, UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);	// tonf/m2 -> kgf/cm2

		* dTmax = Tmax;
		*dTsa = pDataStd->GetValueUnitType(pCalcPile->m_dArrTsa[nTypePileCase][nCaseMax], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
	}
	else
	{
		CRcBridgeRebar *pBri = m_pStd->m_pBri;
		long nJijum = 0;
		CFootingApp *pFooting = pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		long nPileSu = pPile->m_nCountOrg;
		if(nPileSu == 0) return;

		long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

		double D		= pPile->m_dDia;
		double T1		= pPile->m_dT1;
		double T2		= pPile->m_dT2;
		double D1		= D-T2*2;
		double d		= D-T1*2;
		double Ap		= pCalcPile->m_Ap;
		double alpha	= 4*(D1*D1+D1*d+d*d) / (3*(D1*D1+d*d));

		double Tmax = 0;
		for(long nT=0; nT<nCase; nT++)
		{
			BOOL bNormalCase = (nT!=0) ? FALSE : TRUE;
			if(bNormal != bNormalCase) continue;
			for(long n=0; n<nPileSu; n++)
			{
				CPileLoad *pLoad = NULL;
				if(nT == 0)
					pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, n, TRUE);
				else
				{
					if(bLsd)
						pLoad = pCalcPile->GetPileLoad(PILE_CASE_EXTREME, n);
					else
						pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);
				}

				double dDistPile = GetDistMaxPile(n);
				if(!bLsd)
					dDistPile = toM(dDistPile);

				double Ho = pLoad->m_dForceHori * dDistPile;
				Tmax = MAX(Tmax, GetValueUnitChange((alpha * frTon(Ho)) / Ap, UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2));	// tonf/m2 -> kgf/cm2
			}
		}
		* dTmax = Tmax;
		*dTsa = pCalcPile->GetTsa(!bNormal);
	}
}

void CARcBridgeCalcFooting::GetCheckValuePileDisplacement(long nJijum, double *dDisa, double *dDismax, BOOL bNormal)
{
	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	if(m_pStd->m_pBri->IsBoxType() == FALSE)
	{
		HGBOOL bPileASD = (m_pStd->m_pBri->IsBoredPileASD(nJijum) || m_pStd->m_pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		long nCase = 0;
		long nTypeLoad = PILE_CASE_USE;
		if(bNormal == FALSE)
			nTypeLoad = (bLsd)? PILE_CASE_EXTREME : PILE_CASE_EARTH;

		if(bLsd && bPileASD == FALSE)
			*dDismax = pCalcPile->GetMaxDisplacement(!bNormal, nCase, nTypeLoad);
		else
			*dDismax = frM(pCalcPile->GetMaxDisplacement(!bNormal, nCase, nTypeLoad));
		
		if(bNormal) *dDisa = frM(pCalcPile->m_Disa[0]);
		else *dDisa = frM(pCalcPile->m_Disa[1]);
	}
	else
	{
		CDesignPile *pCalcPile = m_pStd->GetDesignPile(0);

		// (ARCBRIDGE-1473) �𵨸��� X-Z ������� �������. �׷��� ��������� X,Z �� �ƴ϶� X,Y��
		double dDispMax[2];
		dDispMax[0] = m_dArrDispX[0].Max();
		dDispMax[1] = MAX(m_dArrDispX[2].Max(), m_dArrDispX[3].Max());

		long nTypeLoad = PILE_CASE_USE;
		if(bNormal == FALSE)
			nTypeLoad = (bLsd)? PILE_CASE_EXTREME : PILE_CASE_EARTH;

		if(bLsd)
			*dDismax = dDispMax[bNormal ? 0 : 1];
		else
			*dDismax = frM(dDispMax[bNormal ? 0 : 1]);
		*dDisa = frM(pCalcPile->m_Disa[nTypeLoad]);
	}
}

void CARcBridgeCalcFooting::GetCheckValuePileForceMinus(long nJijum, double *dPa, double *dPmin, BOOL bNormal)
{
	CARcBridgeDataStd *pDataStd = m_pStd->m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);

	if(m_pStd->m_pBri->IsBoxType())
	{
		*dPa = 0;
		*dPmin = 0;
		return;
	}
	
	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nTypeLoad = PILE_CASE_USE;
	if(bNormal == FALSE)
		nTypeLoad = (bLsd)? PILE_CASE_EXTREME : PILE_CASE_EARTH;

	long nCase = 0; 
	*dPmin = pDataStd->GetValueUnitType(pCalcPile->GetMinReaction(!bNormal, nCase, nTypeLoad), UNIT_CGS_TONF, pDataStd->m_szTonf);
	if(bNormal) 
		*dPa = bLsd ? pCalcPile->m_Pa[PILE_CASE_USE] : pDataStd->GetValueUnitType(pCalcPile->m_Pa[0], UNIT_CGS_TONF, pDataStd->m_szTonf);
	else 
		*dPa = bLsd ? pCalcPile->m_Pa[PILE_CASE_EXTREME] : pDataStd->GetValueUnitType(pCalcPile->m_Pa[1], UNIT_CGS_TONF, pDataStd->m_szTonf);
}

void CARcBridgeCalcFooting::WriteSupportCheckFooting(CXLFormatText *XLText, long nIndexSub, long nJijum, BOOL bFront)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	CConcBasic *pConcData;

//	double dRatio		= 0.;
	double dAreaReq		= 0.;
	double dAreaUseC	= 0.;
	double dAreaUseT	= 0.;
	double dAreaUseTot	= 0.;
	//double dAreaUseComp = 0.;
	CString szComp = _T("");
	CString szOkNg = _T("");
	
	pConcData = pBri->GetConcDataFooting(nJijum, bFront, FALSE);
	if(pConcData == NULL) return;

	dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
	dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
	dAreaUseTot = dAreaUseC + dAreaUseT;
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+20$e$h%s$r%s$n$n$h", GetNumberString(nIndexSub+1,NUM_CIRCLE), pConcData->m_sTitle);
	
	XLText->AddFormatText("$h$r$c-$r$e���ö�ٷ�$n$h");
	XLText->AddFormatText("$h$r$r$r$e�����$m+02$c:$r$e");
	long i=0; for(i=0; i<MAX_REBAR_DAN; i++)
	{
		if(pConcData->m_CSupportBarDia[i] == 0) continue;
		if(i != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_CSupportBarDia[i], pConcData->m_Fy), pConcData->m_CSupportBarCTC[i]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02��$n$h", dAreaUseC);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02��$n$h", dAreaUseC);
	
	XLText->AddFormatText("$h$r$r$r$e�����$m+02$c:$r$e");
	for(i=0; i<MAX_REBAR_DAN; i++)
	{
		if(pConcData->m_TSupportBarDia[i] == 0) continue;
		if(i != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_TSupportBarDia[i], pConcData->m_Fy), pConcData->m_TSupportBarCTC[i]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02��$n$h", dAreaUseT);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02��$n$h", dAreaUseT);
	
	XLText->AddFormatText("$u$h$m+06$-07$n$h");
	if(bSI) XLText->AddFormatText("$m+08$c$��$r$c=$r$e$f1%.1f$me1$m+02��$n$h", dAreaUseTot);
	else XLText->AddFormatText("$m+08$c$��$r$c=$r$e$f3%.3f$me1$m+02��$n$h", dAreaUseTot);
	XLText->AddFormatText("$n$h");
	
	if(pBri->m_bCheckSuppoerBarFoot15hs)
	{
		double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
		double dThick = MIN(dMaxH, pConcData->m_Ho);
		double dB = pConcData->m_Bo;
		double dReqOrg = (bSI) ? 0.0015 * dThick * dB: toCm2(0.0015 * dThick * dB);
		dAreaReq = MIN(dReqOrg, (bSI ? 1800 : 18));

		
		XLText->AddFormatText("$h$r$c-$r$e0.0015hb  [���α�������� 4.3.9]$n$h");
		if(pData->IsDoro2010())
			XLText->AddFormatText("$h$r$r$eh : ����β�$m+04b : ���� ��$n$h");
		else
			XLText->AddFormatText("$h$r$r$eh : ����β� �ִ� %.0f$m+07b : ���� ��$n$h", dMaxH);
		
		XLText->AddFormatText("$h$r$r$r$e�ʿ� ö�ٷ�$m+03$c:$r$e");
		if(pData->m_nTypeKciCode != 0)	// Kci 2007
		{
			if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ��  (��1800)$n$h", dThick, dB, dReqOrg);
			else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ��  (��1800)$n$h", toCm(dThick), toCm(dB), dReqOrg);

			dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";

			if(0.0015*dThick*1000>1800 && pData->IsDoro2010())
			{
				XLText->AddFormatText(_T("$h$r$r$e�ʿ�ö�ٷ��� 1800������ Ŭ��� ����,�µ� ö�� �ܸ����� ���� m��$n"));//zzzz�ٸ��߱�
				XLText->AddFormatText(_T("$h$r$r$e1800������ ũ�� ���� �ʿ䰡 ����. [��ũ��Ʈ����������� 5.7.2]$n$h"));
			}

			if(bSI) 
				XLText->AddFormatText("$h$m+12$c��$r %.1f ��$m+03$c%s$r$r$e$f1%.1f ��$m+03$c��$r$e", dAreaReq, szComp, dAreaUseTot);
			else 
				XLText->AddFormatText("$h$m+12$c��$r %.3f ��$m+03$c%s$r$r$e$f3%.3f ��$m+03$c��$r$e", dAreaReq, szComp, dAreaUseTot);
		}
		else
		{
			if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ��", dThick, dB, dAreaReq);
			else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ��", toCm(dThick), toCm(dB), dAreaReq);

			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(bSI) 
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ��$m+03$c��$r$e", szComp, dAreaUseTot);
			else 
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ��$m+03$c��$r$e", szComp, dAreaUseTot);
		}

		if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
// 
// 		else
// 		{
// 			if(bSI) 
// 				XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ��", dThick, dB, dAreaReq);
// 			else 
// 				XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ��", toCm(dThick), toCm(dB), dAreaReq);
// 
// 			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
// 			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
// 			if(bSI) 
// 				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ��$m+03$c��$r$e", szComp, dAreaUseTot);
// 			else 
// 				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ��$m+03$c��$r$e", szComp, dAreaUseTot);
// 		
// 			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
// 			XLText->AddFormatText("$a%s$n$h", szOkNg);
// 		}
	}
	
	if(pBri->m_bCheckSuppoerBarFootArea)
	{
//		double dRatio		= 0.;
		double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
		double dRatio = pConcData->m_Fy > 4000 ? 0.002*4000./pConcData->m_Fy : pBri->m_dCheckSuppoerBarFootArea;
		dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
		
		if (pData->m_nTypeKciCode == 0 || pData->m_nTypeKciCode == 1)
		{
			XLText->AddFormatText("$h$r$c-$r$e�ܸ�����(�µ� �� �������� ö��) [��ũ��Ʈ����������� �ؼ� P.111]$n$h");
		}
		else
		{
			XLText->AddFormatText("$h$r$c-$r$e�ܸ�����(�µ� �� �������� ö��) [��ũ��Ʈ�������� 5.7.2]$n$h");
		}

		XLText->AddFormatText("$h$r$r$e�ܸ������ %.4f �̻�$n$h", dRatio);
		XLText->AddFormatText("$h$r$r$r$e�ʿ�ö�ٷ�$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
		if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ��", dRatio, pConcData->m_Bo, pConcData->m_Ho, dAreaReq);
		else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ��", dRatio, toCm(pConcData->m_Bo), toCm(pConcData->m_Ho), dAreaReq);
		
		if(pData->m_nTypeKciCode != 0)
			dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
		szComp = (dAreaReq > dAreaUseTot)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
		if(pData->m_nTypeKciCode != 0 && dAreaSection * dRatio >= 1800)	// Kci 2007
		{
			if(bSI)
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ��$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e�ʿ�ö�ٷ��� 1800������ Ŭ��� ����,�µ� ö�� �ܸ����� ���� m��$n");
				XLText->AddFormatText("$h$r$r$e1800������ ũ�� ���� �ʿ䰡 ����.(KCI 2007) [��ũ��Ʈ����������� 5.7.2]$m+19$c��$r$e%s$n$h", szOkNg);
			}
			else
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ��$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e�ʿ�ö�ٷ��� 18������ Ŭ��� ����,�µ� ö�� �ܸ����� ���� m��$n");
				XLText->AddFormatText("$h$r$r$e18������ ũ�� ���� �ʿ䰡 ����.(KCI 2007) [��ũ��Ʈ����������� 5.7.2]$m+19$c��$r$e%s$n$h", szOkNg);
			}
		}
		else
		{
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ��$m+03$c��$r$e", szComp, dAreaUseTot);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ��$m+03$c��$r$e", szComp, dAreaUseTot);
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}
	}
	
	if(pBri->m_bCheckSuppoerBarFoot300)
	{
		dAreaReq = (bSI)? 300:toCm2(300);
		
		XLText->AddFormatText("$h$r$c-$r$e����� 300�� �̻� [���α�������� 4.3.9, ö��������� P.254]$n$h");
		XLText->AddFormatText("$h$r$r$r$e�ʿ�ö�ٷ�$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("$f1%.1f ��", dAreaReq);
		else XLText->AddFormatText("$f3%.3f ��", dAreaReq);
		
//		double dAreaUseComp = 0.;
		double dAreaUseComp = MIN(dAreaUseC, dAreaUseT);
		szComp = (dAreaReq > dAreaUseComp)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
		if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ��$m+03$c��$r$e", szComp, dAreaUseComp);
		else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ��$m+03$c��$r$e", szComp, dAreaUseComp);
		
		if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
	}
	XLText->AddFormatText("$n$h");
}

void CARcBridgeCalcFooting::WriteCheckFootingRigidBody(CXLFormatText *XLText, long nIndex, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	CBoringData *pBoring  = pBri->GetBoringDataJijumBase(nJijum);

	BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) ? TRUE : FALSE;
	CDesignPile *pCalcPile = bPileBase ? m_pStd->GetDesignPile(nJijum) : NULL;

	EPartOfBridge ePart = (nJijum == 0)? ePartFootStt : ((nJijum == pBri->m_nQtyJigan)? ePartFootEnd : ePartInFoot);
	double alpha = pFooting->m_dFactorAlpha[0]; // ���α��������,�Ϻα���,5.5.7 (����)
	double Eo    = pFooting->m_dBoringEo;			// pBoring->m_dEo;
	double L	 = toM(pFooting->GetLength());
	double B	 = toM(pFooting->GetWidth());
	double H     = toM(pFooting->GetHeight());
	double Ec	 = pBri->GetValueEc(ePart)*10;	// (kgf/cm��->tomf/m2)
//	double Bv    = Round(sqrt(L*B),3);
//	double kvo	 = 1/30.0 * alpha * Eo;
//	double kv    = kvo * 10 * pow(Bv/30.0,-3./4.); // (kgf/cm��->tomf/m2)
	double Bv    = Round(sqrt(L*B),3);		// m
	double kvo	 = 1/30.0 * alpha * Eo;		// kgf/cm3
	double kv    = kvo * pow(toCm(frM(Bv))/30.0,-3./4.) * 1000;;	// kgf/cm3 -> tonf/m3
	double PileSu= bPileBase ? pFooting->m_exFooting.m_Pile.GetPileSu() : 0;
	double k	 = kv;
	double Kv_pile  = 0;
	if(bPileBase)
	{
		Kv_pile = pCalcPile->m_Kv;
		k = Kv_pile*PileSu/(L*B);
	}
	double beta  = pow((3*k)/(Ec*H*H*H),1./4.);

	double a	 = 0;
	double e	 = 0;
	double ramda_= 0;
//	double FW    = (nJijum==0) ? toM(pFooting->m_dFWL) : toM(pFooting->m_dFWR); // �ޱ��� ����....
	double FW    = MAX(toM(pFooting->m_dFWL), toM(pFooting->m_dFWR)); // (34519)�ձ��� �ޱ��� ������ ����
	double l	 = MIN(0,L/2); // (40862) �����̹Ƿ� 0�� ����
	double b	 = MIN(FW,B/2);
	double ramda = MAX(l,b);
	if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
	{
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		if(pWall->m_bTopIsColumn)
		{
			l = MIN(toM(pWall->GetDistFromFootingToColumn(TRUE)),L/2); // ����....�
			if(pWall->m_nCountColumn==1)
			{
				b	  = MIN(FW,B/2);
				ramda = MAX(l,b);
			}
			else
			{
				a	  = 1.30;
				e	  = FW;
				b	  = toM(pWall->m_dDistColumn[1]/2 - pWall->m_Column.m_dB[iOUTTER][iUPPER]); // ��հ� ���Ÿ��� 1/2
				ramda_= MAX(l,b);
				ramda = a*(ramda_*ramda_+e*e) / (ramda_+e);
			}
		}
	}
	BOOL bOK_RigidBody1 = (ramda*beta<=1.0) ? TRUE : FALSE;

	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+45$e$h%d)$rȮ����� �β��� ���� ��ü ���� : [���α� ������� 5.4.5.2]$n",nIndex);

	XLText->AddFormatText("$h$r - Ȯ����ʴ� ����μ� �ʿ��� �β��� Ȯ���԰� ���ÿ� ��ü�μ� ��޵Ǵ�$n");
	XLText->AddFormatText("$h$r   �β��� ���������� ��Ģ���� �Ѵ�.$n");
	XLText->AddFormatText("$h$r - Ȯ����ʿ� ���� ��ü�μ��� ��޿��δ� ���ݹݷ� �� ���ҹݷ¿� ��ġ��$n");
	XLText->AddFormatText("$h$r   Ȯ������� ������ ������ ����Ͽ� �����ϴ� �����μ�, �Ϲ�������$n");
	XLText->AddFormatText("$h$r   ������ ���� ������ �� ��ü�� ����� �� �ִ�.$n$n");

	XLText->AddFormatText("$h$m+03��� �� 1.0$n");
	XLText->AddFormatText("$h$r���⼭$m+04$p[FOOTRIGID_BETA:Equation]$n$n");

	XLText->AddFormatText("$h$r k  : kv - ���������� ���, kp - ���ұ����� ���$n");
	CString str = _T("");
	if(bPileBase)
	{
		str.Format(" kp : ȯ�����ݹݷ°�� (%s)", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$r%s$n",str);
		str.Format("   = %s x %.0f / (%s x %s) = %s %s",
			pData->GetStrDoubleUnitChange(Kv_pile,UNIT_CGS_TONF_M,pData->m_szTonf_M, FALSE, 0),PileSu,pData->GetStrDoubleUnitChange(L, UNIT_CGS_M, pData->m_szM),
			pData->GetStrDoubleUnitChange(B, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(k,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), pData->m_szTonf_M3);
		XLText->AddFormatText("$h$r%s$n",str);
		XLText->AddFormatText("$h$m+04$p[FOOTRIGID_KP:Equation]$n$n");
		str.Format(" Kv : 1�� ������ ����� ���������� (= %s %s) ,�����Ұ�꡻����",pData->GetStrDoubleUnitChange(Kv_pile,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), pData->m_szTonf_M);
		XLText->AddFormatText("$h$r%s$n",str);
	}
	else
	{
		str.Format(" kv : �������� ���ݹݷ°�� (%s)", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$r%s$n",str);
//		str.Format("   = %.3f x (��(%.3f x %.3f) / 30)^-�� = %.3f %s",GetValueUnitChange(kvo,UNIT_CGS_KGF_CM3,pData->m_szKgf_CM3)*10,B,L,GetValueUnitChange(kv,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), pData->m_szTonf_M3);
		if(pData->IsSI())
		{
			str.Format("   = %s x (��(%s x %s) / %s)^-��) = %s %s",
				pData->GetStrDoubleUnitChange(kvo,UNIT_CGS_KGF_CM3,pData->m_szKgf_CM3, TRUE, 3), pData->GetStrDoubleUnitChange(B, UNIT_CGS_M, pData->m_szM), 
				pData->GetStrDoubleUnitChange(L, UNIT_CGS_M, pData->m_szM), bLsd? _T("300") : _T("0.3"),
				pData->GetStrDoubleUnitChange(kv,UNIT_CGS_TONF_M3,pData->m_szTonf_M3, TRUE, 3), pData->m_szTonf_M3);
		}
		else
		{
			str.Format("   = %.3f x (��(%.1f x %.1f) / 30)^-��) x 1000 = %.3f %s",kvo,toCm(frM(B)),toCm(frM(L)),GetValueUnitChange(kv,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), pData->m_szTonf_M3);
		}

		XLText->AddFormatText("$h$r%s$n",str);
//		str.Format("Kvo = 1/30*��*Eo = 1/30 x %.3f x %.3f = %.3f %s",alpha,GetValueUnitChange(Eo,UNIT_CGS_KGF_M2,pData->m_szKgf_M2),GetValueUnitChange(kvo,UNIT_CGS_KGF_CM3,pData->m_szKgf_CM3), pData->m_szKgf_CM3);
		if(bLsd || pData->IsSI())
		{
			if(bLsd)
				XLText->AddFormatText("$h$m+04$p[FOOTRIGID_KV_LSD]$n$n");
			else
				XLText->AddFormatText("$h$m+04$p[FOOTRIGID_KV_SI]$n$n");
			str.Format("Kvo = 1/%s*��*Eo = 1/%s x %.3f x %s = %s %s", bLsd? _T("300") : _T("0.3"), bLsd? _T("300") : _T("0.3"), 
				alpha,pData->GetStrDoubleUnitChange(Eo,UNIT_CGS_KGF_CM2, pData->m_szTonf_M2/*pData->m_szKgf_CM2*/),
				pData->GetStrDoubleUnitChange(kvo,UNIT_CGS_KGF_CM3,pData->m_szKgf_CM3,FALSE, CELL_DEC_3),pData->m_szKgf_CM3);
		}
		else
		{
			XLText->AddFormatText("$h$m+04$p[FOOTRIGID_KV]$n$n");
			str.Format("Kvo = 1/30*��*Eo = 1/30 x %.3f x %.3f = %.2f %s",alpha,Eo,kvo,pData->m_szKgf_CM3);
		}
		XLText->AddFormatText("$h$r%s$n",str);
	}
	str.Format(" L  : Ȯ������� ���� (= %s %s)", pData->GetStrDoubleUnitChange(L, UNIT_CGS_M, pData->m_szM), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format(" B  : Ȯ������� ��   (= %s %s)",pData->GetStrDoubleUnitChange(B, UNIT_CGS_M, pData->m_szM), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	if(bPileBase)
	{
		str.Format("n*m : ������ ���� (= %.0f ��)",PileSu);
		XLText->AddFormatText("$h$r%s$n",str);
	}
	str.Format(" E  : Ȯ������� ź����� (= %s %s)",pData->GetStrDoubleUnitChange(Ec,UNIT_CGS_TONF_M2,pData->m_szTonf_M2), pData->m_szTonf_M2);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format(" h  : Ȯ������� ��յβ� (= %s %s)", pData->GetStrDoubleUnitChange(H, UNIT_CGS_M, pData->m_szM), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	XLText->AddFormatText("$h$r�� : Ȯ������� ȯ�� ������� (%s)$n", pData->m_szM);
	str.Format("�� = ����[(3 x %s) / (%s x %s��)] = %.3f (m-1)",
		pData->GetStrDoubleUnitChange(k,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), pData->GetStrDoubleUnitChange(Ec,UNIT_CGS_TONF_M2,pData->m_szTonf_M2),
		pData->GetStrDoubleUnitChange(H, UNIT_CGS_M, pData->m_szM),beta);
	XLText->AddFormatText("$h$r%s$n$n",str);

	// ����Ȯ�����
	BOOL bIndependenceBase = TRUE;
	if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
	{
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		if(pWall->m_bTopIsColumn && pWall->m_nCountColumn>1)
		{
			bIndependenceBase = FALSE;
		}
	}
	if(bIndependenceBase)
	{
		XLText->AddFormatText("$h$r�� ����Ȯ����� �� ��Ȯ������� ���$n");
		str.Format("�� = MAX(l,b) = MAX(%s,%s) = %s$n",
			pData->GetStrDoubleUnitChange(l, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(b, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(ramda, UNIT_CGS_M, pData->m_szM));
		XLText->AddFormatText("$h$m+02%s$n",str);
		str.Format("�ٸ�, l �� L/2�̸�  l = L/2 = %s", pData->GetStrDoubleUnitChange(L/2, UNIT_CGS_M, pData->m_szM));
		XLText->AddFormatText("$h$m+02%s$n",str);
		str.Format("      b �� B/2�̸�  b = B/2 = %s", pData->GetStrDoubleUnitChange(B/2, UNIT_CGS_M, pData->m_szM));
		XLText->AddFormatText("$h$m+02%s$n",str);

		XLText->AddFormatText("$h$m+02$p[FOOTRIGID_BASE1]$n$n");
		*XLText += 6;

		if(bOK_RigidBody1)
			str.Format("��� = %.3f x %.3f = %.3f �� 1.0, �� O.K",beta,ramda,beta*ramda);
		else
			str.Format("��� = %.3f x %.3f = %.3f �� 1.0, �� N.G",beta,ramda,beta*ramda);
		XLText->AddFormatText("$h$m+02%s$n",str);
	}
	// ����Ȯ�����
	else
	{
		XLText->AddFormatText("$h$r�� ����Ȯ������� ���$n");
		str.Format("�� = a x (��'��+ e��) / (��'+ e)");
		XLText->AddFormatText("$h$m+02%s$n",str);
		str.Format("   = %.3f x (%.3f��+ %.3f��) / (%.3f+ %.3f)",a,ramda_,e,ramda_,e);
		XLText->AddFormatText("$h$m+02%s$n",str);
		str.Format("   = %.3f",ramda);
		XLText->AddFormatText("$h$m+02%s$n",str);

		XLText->AddFormatText("$h$m+02$p[FOOTRIGID_BASE2]$n$n");
		*XLText += 6;

		if(bOK_RigidBody1)
			str.Format("��� = %.3f x %.3f = %.3f �� 1.0, �� O.K",beta,ramda,beta*ramda);
		else
			str.Format("��� = %.3f x %.3f = %.3f �� 1.0, �� N.G",beta,ramda,beta*ramda);
		XLText->AddFormatText("$h$m+02%s$n",str);
	}
	if(bOK_RigidBody1==FALSE && pBri->m_bApplyFootingMinThick==TRUE)
	{
		*XLText += 1;
		CString strCombin = (H > L/5)? "<" : ">";
		str = (H > L/5)? "O.K" : "N.G";
		XLText->AddFormatText("$h$r�� ���������� ��������� ū ����(Ȥ�� �Ϲ�)���� ��ġ�Ǵ� ��� Ȯ����� �β��� ���Ѱ���$n");
		XLText->AddFormatText("$h$r   Ȯ����� �亯�� 1/5������ ���Ƶ� ����.$n");
		XLText->AddFormatText("$h$r$r�庯���� x 1/5$m+04$c=$r%s x 1/5$m+04$c=$r%s %s$m+02$c%s$r���ʵβ�(=%s %s)$m+05$c��$r%s$n",
			pData->GetStrDoubleUnitChange(L, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(L/5, UNIT_CGS_M, pData->m_szM),
			pData->m_szM, strCombin, pData->GetStrDoubleUnitChange(H, UNIT_CGS_M, pData->m_szM), pData->m_szM, str);
	}
}

long CARcBridgeCalcFooting::GetCountSectMASS(long nJijum)
{
	CFootingApp *pFoot = m_pStd->m_pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return 0;
	CExFootingApp *pExFoot = &pFoot->m_exFooting;
	if(!pExFoot) return 0;

	long nCntSect = 0;

	if(pExFoot->m_nType != EXFOOTING_TYPE_MASS) nCntSect = 0;
	else if(pExFoot->m_nCountDan==0 || pExFoot->m_bFrontDanMass==FALSE) nCntSect = 1;
	else
	{
//		double dH = 0.;
// 		double dW = 0.;
// 		double dWs = 0.;
		nCntSect++;
		long nDan = pExFoot->m_nCountDan;
		long i=0; for(i=0; i<nDan; i++)
		{
			double dH = pExFoot->m_dEFH[i];
			double dW = pExFoot->m_dEFW[i];
			double dWs = pExFoot->m_dEFS[i];

			if(dH > 0 && dW > 0) nCntSect++;
			if(dH > 0 && dWs > 0) nCntSect++;
		}
	}

	return nCntSect;
}

void CARcBridgeCalcFooting::CalculateForceMASS(long nJijum)
{
	CARcBridgeDataStd *pDataStd = m_pStd->m_pBri->m_pARcBridgeDataStd;
	
	CFootingApp *pFoot = m_pStd->m_pBri->GetFootingByJijumNumber(nJijum);
	CExFootingApp *pExFoot = &pFoot->m_exFooting;
	
	if(!m_pStd->m_pBri->m_bApplyMassCalcSafe || pExFoot->m_nType != EXFOOTING_TYPE_MASS) return;

	BOOL bLsd = pDataStd->IsLsdDesign();
	BOOL bEnd	  = (nJijum == m_pStd->m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	double dKh	  = m_pStd->GetKhValue(FALSE, !bEnd, eGoalEQ_None);
	double dUW_conc = GetValueUnitChange(toTon(pDataStd->m_pBasicConcInfo->m_UWeightConcrete), UNIT_CGS_TONF_M3, pDataStd->m_szTonf_M3);
	double dWMass = bLsd? pExFoot->GetWidth() : toM(pExFoot->GetWidth());
	double dHMass = bLsd? pExFoot->m_dEFHR : toM(pExFoot->m_dEFHR);
	double dHSum  = 0.;
	double dWSum  = 0.;
	BOOL bDistDir = ((pExFoot->m_bLeftOrgOfMass && bEnd) || (!pExFoot->m_bLeftOrgOfMass && !bEnd))? TRUE:FALSE;
	
//	double dTempH = 0.;
//	double dTempW = 0.;
//	double dTempWs = 0.;
//  ���ʿ� dEf�� �����ϸ� ���簢�������� ����� �߻���. (AAbutPier�� ���� �����)
//	double dEf = toM((bEnd)? pExFoot->m_dEFWR : pExFoot->m_dEFWL);
    double dEf = 0.;
	
	long nSec = 0;
	long nDan = pExFoot->m_nCountDan;
	long i=0; for(i=0; i<nDan; i++)
	{
		// ������ ������������ �ִ� ���� ������ ���� �ʿ䰡 ����, ��ü���̷� ����
		if(pExFoot->m_bFrontDanMass==FALSE)
			break;
		double dTempH = bLsd? pExFoot->m_dEFH[i] : toM(pExFoot->m_dEFH[i]);
		double dTempW = bLsd? pExFoot->m_dEFW[i] : toM(pExFoot->m_dEFW[i]);
		double dTempWs = bLsd? pExFoot->m_dEFS[i] : toM(pExFoot->m_dEFS[i]);
		if(i == 0)
		{
			if(bLsd)
				dTempW += pExFoot->m_bLeftOrgOfMass? pExFoot->m_dEFWL : pExFoot->m_dEFWR;
			else
				dTempW += toM(pExFoot->m_bLeftOrgOfMass? pExFoot->m_dEFWL : pExFoot->m_dEFWR);
		}
		
		if(dTempH > 0 && dTempW > 0) 
		{
			nSec++;
			dWSum += dTempW;
			dHSum += dTempH;
			m_dB[nSec] = dWSum;
			m_dH[nSec] = dTempH;
			m_dA[nSec] = m_dB[nSec] * m_dH[nSec];
			m_dDist[nSec] = (bDistDir)? (dWSum/2-dEf) : (dWMass-dEf)-dWSum/2;
			m_dVer[nSec] = m_dA[nSec] * dUW_conc;
			m_dHor[nSec] = m_dVer[nSec] * dKh;
			m_dMoment[nSec] = m_dVer[nSec] * m_dDist[nSec];
		}
		if(dTempH > 0 && dTempWs > 0)
		{
			nSec++;
			m_dB[nSec] = dTempWs;
			m_dH[nSec] = dTempH;
			m_dA[nSec] = (m_dB[nSec] * m_dH[nSec])/2;
			m_dDist[nSec] = (bDistDir)? (dWSum-dEf)+dTempWs/3 : (dWMass-dEf)-dWSum-dTempWs/3;
			m_dVer[nSec] = m_dA[nSec] * dUW_conc;
			m_dHor[nSec] = m_dVer[nSec] * dKh;
			m_dMoment[nSec] = m_dVer[nSec] * m_dDist[nSec];
			
			dWSum += dTempWs;
		}
	}
	
	// �� ����...
	m_dB[0] = dWMass;
	m_dH[0] = dHMass - dHSum;
	m_dA[0] = m_dB[0] * m_dH[0];
	m_dDist[0] = dWMass/2 - dEf;
	m_dVer[0] = m_dA[0] * dUW_conc;
	m_dHor[0] = m_dVer[0] * dKh;
	m_dMoment[0] = m_dVer[0] * m_dDist[0];
	
}

// nIndCol - 0:B(m), 1:H(m), 2:A(m2), 3:�Ÿ�(m), 4:V(ton), 5:H(ton), 6:M(ton.m)
double CARcBridgeCalcFooting::GetValueForceMASS(long nJijum, long nIndSect, long nIndCol, BOOL bTot)
{
	double dRetVal = 0;
	long nCntSec = GetCountSectMASS(nJijum);
	
	if(bTot)
	{
		long i=0; for(i=0; i<nCntSec; i++)
		{
			switch(nIndCol)
			{
			case 0: dRetVal += m_dB[i]; break;
			case 1: dRetVal += m_dH[i]; break;
			case 2: dRetVal += m_dA[i]; break;
			case 3: dRetVal += m_dDist[i]; break;
			case 4: dRetVal += m_dVer[i]; break;
			case 5: dRetVal += m_dHor[i]; break;
			case 6: dRetVal += m_dMoment[i]; break;
			default: dRetVal =0;
			}
		}
	}
	else
	{
		if(nIndSect > nCntSec) return 0;

		switch(nIndCol)
		{
			case 0: dRetVal = m_dB[nIndSect]; break;
			case 1: dRetVal = m_dH[nIndSect]; break;
			case 2: dRetVal = m_dA[nIndSect]; break;
			case 3: dRetVal = m_dDist[nIndSect]; break;
			case 4: dRetVal = m_dVer[nIndSect]; break;
			case 5: dRetVal = m_dHor[nIndSect]; break;
			case 6: dRetVal = m_dMoment[nIndSect]; break;
			default: dRetVal =0;
		}
	}

	return dRetVal;
}

double CARcBridgeCalcFooting::GetQmaxMASS(long nJijum)
{
	CFootingApp *pFoot = m_pStd->m_pBri->GetFootingByJijumNumber(nJijum);
	CExFootingApp *pExFoot = &pFoot->m_exFooting;
	
	double dQmax = 0.;
//	double dE	 = 0.;
//	double dX	 = 0.;

	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && pExFoot->m_nType == EXFOOTING_TYPE_MASS)
	{
		BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
		double dWMass = bLsd? pExFoot->GetWidth() : toM(pExFoot->GetWidth());
		
		double dVerTot = GetValueForceMASS(nJijum, 0, 4, TRUE);
		double dMoTot = GetValueForceMASS(nJijum, 0, 6, TRUE);
		
		double dE = dWMass/2 - dMoTot/dVerTot;
		double dX = 3 * (dWMass/2 - dE);
		
		if(dX > dWMass)
		{
			dQmax = (dVerTot/dWMass) * (1 + 6*fabs(dE)/dWMass);
		}
		else
		{
			dQmax = 2.0*dVerTot/dX;
		}
	}
	
	return dQmax;
}

void CARcBridgeCalcFooting::MakeTableForceMASS(CXLFormatText *XLText, long nJijum)
{
	CARcBridgeDataStd *pDataStd = m_pStd->m_pBri->m_pARcBridgeDataStd;
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);

	CFootingApp *pFoot = m_pStd->m_pBri->GetFootingByJijumNumber(nJijum);
	CExFootingApp *pExFoot = &pFoot->m_exFooting;
	
	CString sTitle = _T("");
	CString strOpt = _T("");
	CString str = _T("");
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// e, xL, Qmax, Qmin ���
	BOOL bLsd = pDataStd->IsLsdDesign();
	BOOL bEnd = (nJijum == m_pStd->m_pBri->m_nQtyJigan)? TRUE : FALSE;
	double dKh	  = m_pStd->GetKhValue(FALSE, !bEnd, eGoalEQ_None);
	double dWMass =  bLsd? pExFoot->GetWidth() : toM(pExFoot->GetWidth());
	
	double dQmax = 0;
	double dQmin = 0;
	double dE	 = 0;
	double dX	 = 0;
	
	double dVerTot = GetValueForceMASS(nJijum, 0, 4, TRUE);
	double dMoTot = GetValueForceMASS(nJijum, 0, 6, TRUE);
	
	dE = dWMass/2 - dMoTot/dVerTot;
	dX = 3 * (dWMass/2 - dE);
	
	if(dX > dWMass)
	{
		dQmax = (dVerTot/dWMass) * (1 + 6*fabs(dE)/dWMass);
		dQmin = (dVerTot/dWMass) * (1 - 6*fabs(dE)/dWMass);
	}
	else
	{
		dQmax = 2.0*dVerTot/dX;
		dQmin = 0;
	}
	///////////////////////////////////////////////////////////////////////////////////////////

	int nCntSec = GetCountSectMASS(nJijum);
	int nCntRow = nCntSec+2;
	int nCntCol = 8;
	XLText->AddTable(nCntRow-1, 24-1, 1, 3);
	
	// Title
	int nWidthJoin1[] = { 3, 3, 3, 3, 3, 3, 3, 3 };
	
	CString str1, str2, str3, str4, str5, str6, str7;
	str1.Format("B(%s)", pDataStd->m_szM);
	str2.Format("H(%s)", pDataStd->m_szM);
	str3.Format("A(%s)", pDataStd->m_szM2);
	str4.Format("�Ÿ�(%s)", pDataStd->m_szM);
	str5.Format("V(%s)", pDataStd->m_szTonf);
	str6.Format("H(%s)", pDataStd->m_szTonf);
	str7.Format("M(%s)", pDataStd->m_szTonfM);
	char *Title1[] = { "�� ��", str1.GetBuffer(str1.GetLength()), str2.GetBuffer(str2.GetLength()), str3.GetBuffer(str3.GetLength()), str4.GetBuffer(str4.GetLength()), str5.GetBuffer(str5.GetLength()), str6.GetBuffer(str6.GetLength()), str7.GetBuffer(str7.GetLength()) };

	long i=0; for(i=0; i<nCntCol; i++)
	{		
		XLText->AddMergeFormatText(0, nWidthJoin1[i]-1,"$c%s",Title1[i]);
		XLText->AddFormatText("$m+%02d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$h");

	// Data
	for(i=0; i<nCntRow-1; i++)
	{
		int j=0; for(j=0; j<nCntCol; j++)
		{
			if(i != nCntRow-2 && j == 0) str.Format("%s", GetNumberString(i+1,NUM_CIRCLE));
			else if(i == nCntRow-2 && j == 0) str = _T("��");
			else if(i == nCntRow-2 && (j==1 || j==2 || j==4)) str = _T("-");
			else if(i == nCntRow-2) str.Format("%s", pDataStd->GetStrDouble(GetValueForceMASS(nJijum, i, j-1, TRUE), (j==5 || j==6)? UNIT_CGS_TONF_M : UNIT_CGS_M));
			else str.Format("%s", pDataStd->GetStrDouble(GetValueForceMASS(nJijum, i, j-1, FALSE), (j==5 || j==6)? UNIT_CGS_TONF_M : UNIT_CGS_M));
			
			XLText->AddMergeFormatText(0,nWidthJoin1[i]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddFormatText("$h$c*$r$e �����(H)�� V x Kh(=%.3f)�� �����Ѵ�.$n", dKh);
	XLText->AddFormatText("$h$c*$r$e �ۿ�Ÿ��� �ձ��ʿ����� �������� �����Ѵ�.$n");
	XLText->AddFormatText("$h$r$e e = %s/2 - %s/%s$m+08$c=$r$e%s %s$n", pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), 
		pDataStd->GetStrDouble(dMoTot, UNIT_CGS_TONFM), pDataStd->GetStrDouble(dVerTot, UNIT_CGS_TONF), pDataStd->GetStrDouble(dE,UNIT_CGS_M), pDataStd->m_szM);
	XLText->AddFormatText("$h$r$e x = 3 x (%s/2 - %s)$m+08$c=$r$e%s %s$n", pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), pDataStd->GetStrDouble(dE, UNIT_CGS_M), pDataStd->GetStrDouble(dX, UNIT_CGS_M), pDataStd->m_szM);
	if(dX > dWMass)
	{
		XLText->AddFormatText("$h$c*$r$e ����ݷ��� �ۿ�����(x=%s %s)�� B(=%s)���� ū ��� : ��ٸ��� ����$n", 
			pDataStd->GetStrDouble(dX, UNIT_CGS_M), pDataStd->m_szM, pDataStd->GetStrDouble(dWMass, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$e Qmax = (%s/%s) x (1 + 6 x %s/%s)$m+14$c=$r$e%s %s$n", 
			pDataStd->GetStrDouble(dVerTot, UNIT_CGS_TONF), pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), pDataStd->GetStrDouble(fabs(dE), UNIT_CGS_M), 
			pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), pDataStd->GetStrDouble(dQmax, UNIT_CGS_TONF_M2), pDataStd->m_szTonf_M2);
		XLText->AddFormatText("$h$r$e Qmin = (%s/%s) x (1 - 6 x %s/%s)$m+14$c=$r$e%s %s$n", 
			pDataStd->GetStrDouble(dVerTot, UNIT_CGS_TONF), pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), pDataStd->GetStrDouble(fabs(dE), UNIT_CGS_M), 
			pDataStd->GetStrDouble(dWMass, UNIT_CGS_M), pDataStd->GetStrDouble(dQmin, UNIT_CGS_TONF_M2), pDataStd->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$c*$r$e ����ݷ��� �ۿ�����(x=%s %s)�� B(=%s)���� ���� ��� : �ﰢ�� ����$n", 
			pDataStd->GetStrDouble(dX, UNIT_CGS_M), pDataStd->m_szM, pDataStd->GetStrDouble(dWMass, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$e Qmax = 2 x %s/%s$m+08$c=$r$e%s %s$n", 
			pDataStd->GetStrDouble(dVerTot, UNIT_CGS_TONF), pDataStd->GetStrDouble(dX, UNIT_CGS_M), pDataStd->GetStrDouble(dQmax, UNIT_CGS_TONF_M2), pDataStd->m_szTonf_M2);
		XLText->AddFormatText("$h$r$e Qmin = 0$n");
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcFooting::DrawFootingSection_Type_Trap2(CDomyun *pDomP,  CRcBridgeRebar *pBri, BOOL bStt)
{
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	CHgBaseDrawStd DrawStd;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = bStt? 0:pBri->m_nQtyJigan;
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	CDrawExPile drawExPile(pPile, m_pStd->m_pARoadOptionStd);
	
	CWallApp Wall;
	pBri->MakeWallByOutWall(Wall, bStt, FALSE);
	CDPoint xyStt(0, 0);
	xyStt.x	= pBri->GetStationOnJijum(nJijum, 0);
	if(bStt) xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, -pBri->m_dWS, 0).x;
	else xyStt.x = pBri->GetXySlabUpperByOffsetFrStation(xyStt.x, pBri->m_dWE, 0).x;
	double dHightBackFill = pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y - pFooting->m_dEL + Wall.m_dELBDiff;
	// ���ʰ� �������� �׸�
	pBri->SyncBridge();

	// ��� ���� ���� ��ġ(TRAP2 ������ �ΰ��� �ȴ�.)
	CDPoint xyCen1, xyCen2;			// xyCen1 : ������. xyCen2 : ��米��

	CDPoint xyBase = CDPoint(0,0);		// ������....
	CDPoint xyVerDimStt = CDPoint(0,0);
	CDPoint xyVerDimEnd = xyVerDimStt;
	CDoubleArray dArrVerDim;
	double dTemp = 0;
	CString szVerDimDir = "LEFT";

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	Dom.SetScaleDim(80);
	Dom.SetTextHeight(5);
	Dom.SetArrowHeadSize(2);
	Dom.SetDimVLen(7);
	Dom.SetDimExo(4);

	//���� ġ��
	dArrVerDim.RemoveAll();
	if(!bStt)
	{
		xyBase.x = pFooting->m_xyBottomCenter.x - pFooting->GetWidth()/2;
		xyBase.y = pFooting->m_xyBottomCenter.y;
		szVerDimDir = "RIGHT";
		xyVerDimStt = xyBase;
		xyVerDimStt.x += pFooting->GetWidth();
	}
	else
	{
		xyBase.x = pFooting->m_xyBottomCenter.x + pFooting->GetWidth()/2;
		xyBase.y = pFooting->m_xyBottomCenter.y;
		szVerDimDir = "LEFT";
		xyVerDimStt = xyBase;
		xyVerDimStt.x -= pFooting->GetWidth();
	}	
	xyVerDimEnd = xyVerDimStt;

	dTemp = pFooting->m_dFH;
	dArrVerDim.Add(dTemp);
	xyVerDimEnd.y += dTemp;
	dTemp = bStt? pFooting->m_dFSRH : pFooting->m_dFSLH;
	dArrVerDim.Add(dTemp);
	xyVerDimEnd.y += dTemp;

	dArrVerDim.Add(dHightBackFill);
	xyVerDimEnd.y += dHightBackFill;
	if(pBri->m_bUnderGroundRahmen)
	{
		dArrVerDim.Add(pBri->m_dUnderGroundDesignHeight);
		xyVerDimEnd.y += pBri->m_dUnderGroundDesignHeight;
	}
	else
	{
		dArrVerDim.Add(pBri->m_dTP);
		xyVerDimEnd.y += pBri->m_dTP;
	}

	CDRect rectBridge;	// ������ �簢 ����
	rectBridge = pBri->m_tvArrVertSection.GetRect();
	
	// ���ܸ�
	CDRect rectVert;
	CDRect rect;


	CTwinVectorArray tvTot;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CTwinVector tvTemp = pBri->m_tvArrVertSectionRibCen.GetAt(bStt? 0:pBri->m_tvArrVertSectionRibCen.GetSize()-1);
		xyCen2 = (bStt)? tvTemp.m_v1 : tvTemp.m_v2;
	}
	else
	{
		// �߰���ü ���ʴ� ������ �������� ���� �׷����� ��..
		CWallApp *pWall = pBri->GetInWall(bStt? 0 : pBri->GetCountInWall()-1);
		if(pWall)
		{
			tvTot.AddFromTvArray(pWall->m_tvArrFront);
			if(pWall->m_bTopIsColumn)
				tvTot.AddFromTvArray(pWall->m_Column.m_tvArrFront);
			
			CTwinVector tvTemp = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
			xyCen2 = tvTemp.m_v1;
		}
	}
	
	// ������ü �������̸� ������ü�� �׸���
	tvTot.AddFromTvArray(pBri->m_tvArrVertSection);
	tvTot.AddFromTvArray(pFooting->m_tvArrFront);
	tvTot.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);
	rect = tvTot.GetRect();

	CDRect rcTemp = pFooting->m_exFooting.m_tvArrFront.GetRect();
	rect.right	= rcTemp.right + 100;
	rect.left	= rcTemp.left - 100;
	
	rect.top = rectBridge.top + 100;
	rect.bottom -= 100;
	
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		DrawStd.DrawTvArray(&Dom, tvTot);
	else
		DrawStd.DrawCuttingTvArray(&Dom, tvTot, rect);			

	// ġ���� �� ��ũ..
	Dom.Circle(xyBase.x, xyBase.y, 200);
	long iDir = (bStt)? 1:-1;
	Dom.TextOut(xyBase.x+700*iDir, xyBase.y-700, "������");
	// ġ����..
	long nCntDim = dArrVerDim.GetSize();
	Dom.SetDirection(szVerDimDir);
	Dom.DimMoveTo(xyVerDimStt.x, xyVerDimStt.y, 0, "");
	int i=0; for(i=0;i<nCntDim;i++)
	{
		Dom.DimLineTo(dArrVerDim.GetAt(i), 0, COMMA(dArrVerDim.GetAt(i)));
	}

	double dThickWall = pBri->GetThickWall(nJijum);	// + (bStt ? pBri->m_dWSHW : pBri->m_dWEHW);
	Dom.SetDirection("TOP");

	if(bStt)
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(-pFooting->m_dFERW, 0, COMMA(pFooting->m_dFERW));
		Dom.DimLineTo(-pFooting->m_dFSRW, 0, COMMA(pFooting->m_dFSRW));
		Dom.DimLineTo(-pFooting->m_dFWR, 0, COMMA(pFooting->m_dFWR));
		Dom.DimLineTo(-dThickWall, 0, COMMA(dThickWall));
		Dom.DimLineTo(-pFooting->m_dFWL, 0, COMMA(pFooting->m_dFWL));
	}
	else
	{
		Dom.DimMoveTo(xyBase.x, xyBase.y+pFooting->GetHeight()+1000, 0, "");
		Dom.DimLineTo(pFooting->m_dFELW, 0, COMMA(pFooting->m_dFELW));
		Dom.DimLineTo(pFooting->m_dFSLW, 0, COMMA(pFooting->m_dFSLW));
		Dom.DimLineTo(pFooting->m_dFWL, 0, COMMA(pFooting->m_dFWL));
		Dom.DimLineTo(dThickWall, 0, COMMA(dThickWall));
		Dom.DimLineTo(pFooting->m_dFWR, 0, COMMA(pFooting->m_dFWR));
	}
	

	if(pBri->IsPileBase(nJijum))
	{
		
		Dom.SetTextHeight(5);
		drawExPile.DrawFront(&Dom, TRUE, FALSE, FALSE);
		drawExPile.m_bApplyDimSettingOfBaseDom	= TRUE;
		drawExPile.DimFront(&Dom, 0, FALSE, FALSE, TRUE ,TRUE, FALSE);
		drawExPile.m_bApplyDimSettingOfBaseDom	= FALSE;
	}
	else
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyBase.x, xyBase.y, 0, "");
		if(!bStt) Dom.DimLineTo(pFooting->GetWidth(), 0, COMMA(pFooting->GetWidth()));
		else Dom.DimLineTo(-pFooting->GetWidth(), 0, COMMA(pFooting->GetWidth()));
	}

	double wB = 0;
	if(!bStt)
	{
		wB = pFooting->m_dFWR;
		xyCen1.x = xyBase.x + pFooting->m_dFELW + pFooting->m_dFSLW + pFooting->m_dFWL + dThickWall/2;
		xyCen1.y = xyBase.y + pFooting->GetHeight(TRUE);
		
		Dom.DimArrowTo(xyCen1.x+400, xyCen1.y, 800, 4);
		CObInsert *pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENT2", xyCen1.x, xyCen1.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");

		Dom.DimArrowTo(xyCen2.x+400, xyCen2.y, 800, 4);
		pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENT2", xyCen2.x, xyCen2.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");
	}
	else
	{
		wB = pFooting->m_dFWL;
		xyCen1.x = xyBase.x - pFooting->m_dFERW - pFooting->m_dFSRW - pFooting->m_dFWR - dThickWall/2;
		xyCen1.y = xyBase.y + pFooting->GetHeight(TRUE);
		
		Dom.DimArrowTo(xyCen1.x-400, xyCen1.y, 800, 0);
		CObInsert *pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENT", xyCen1.x, xyCen1.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");

		Dom.DimArrowTo(xyCen2.x-400, xyCen2.y, 800, 0);
		pIns	= CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGMODMOMENT", xyCen2.x, xyCen2.y+500, 80, 0);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "+");
	}
	Dom.DimArrowTo(xyCen1.x, xyCen1.y+600, 600, 6);
	Dom.DimArrowTo(xyCen2.x, xyCen2.y+600, 600, 6);

	
	Dom.MoveTo(xyVerDimEnd.x, xyVerDimEnd.y);
	if(bStt)Dom.LineTo(rect.BottomRight().x, xyVerDimEnd.y);
	else Dom.LineTo(rect.BottomLeft().x, xyVerDimEnd.y);
	Dom.MoveTo(xyVerDimEnd.x, xyVerDimEnd.y+800);
	Dom.LineTo(xyVerDimEnd.x+wB*iDir, xyVerDimEnd.y+800);
	for(i=0; i<7; i++)
	{
		Dom.DimArrowTo(xyVerDimEnd.x+wB/6*i*iDir, xyVerDimEnd.y+800, 800, 6);
	}
	CString str;
	double dUpperLoad = pBri->m_dLiveLoadAlways;
	str.Format("q = %.3f %s", pData->GetValueUnitType(dUpperLoad, UNIT_CGS_TONF_M2, pData->m_szTonf_M2), pData->m_szTonf_M2);
	Dom.SetTextAlignHorz(TA_CENTER);
	if(pBri->IsPileBase(nJijum) == FALSE) Dom.SetTextHeight(5);
	else Dom.SetTextHeight(6);
	Dom.TextOut(xyVerDimEnd.x+wB/2*iDir, xyVerDimEnd.y +1000, str);
	Dom.MoveTo(xyVerDimStt.x, xyVerDimStt.y);
	Dom.LineTo(xyVerDimEnd.x, xyVerDimEnd.y);

	Dom.SetCalcExtRect();
	rectVert = Dom.GetExtRect();


	*pDomP << Dom;
}

void CARcBridgeCalcFooting::WriteCalcFootingSupportCheck(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd)
	{
		WriteCalcFootingSupportCheck_Lsd(XLText, nIndexTitle, nIndexMiddle, bEarthquake);
		return;
	}

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
	if(pBoring == NULL) return;

	CDPoint vAng(0,1);
	double dStation = pBri->GetStationOnJijum(nJijum);
	double dWidth = toM(pBri->GetWidthSlabAct(dStation, vAng));
	double dLength = toM(GetLengthBox());

	// (ARCBRIDGE-2814) LSD���� ������ �߰��ϸ鼭 �λ���� ���ǿ� ���� ������ ������� �������̱� ������ ���̿����� �λ���� ������ ���Խ�Ű�� �ʴ´�.
	if(pBri->m_bExistProtectionRiseFooting)
	{
		dLength += toM(pBri->m_dPRF_W * 2);
	}

	double dPfDesign  = GetValueUnitChange(m_pStd->GetWheelLoadFront() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPrDesign  = GetValueUnitChange(m_pStd->GetWheelLoadRear() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPmDesign  = GetValueUnitChange(m_pStd->GetWheelLoadMiddle() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPsDesign  = GetValueUnitChange(m_pStd->GetWheelLoadPs() ,UNIT_CGS_TONF,pData->m_szTonf);
	double E  = m_pStd->m_pLoad->GetEForEffectiveWidth(pBri, 1);
	double Pr = (2*dPrDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
	double Pf = (2*dPfDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
	// (ARCBRIDGE-3214) DL ���߻����ÿ��� EffectiveWidth�� ������ �ʰ� 3.0���� ������.
	// double Ps = (dPsDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
	double Pm = (dPmDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/EFFECTIVE_WIDTH_DL;
	double Ps = (dPsDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/EFFECTIVE_WIDTH_DL;
	double dDistApply = GetValueUnitChange(m_pStd->m_pLoad->m_dDistributionApply ,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	double dLive1 = 0;
	if(pBri->m_dLengthBridge > 8400)
		dLive1 = (Pf + 2 * Pr) / dLength;
	else if(pBri->m_dLengthBridge > 4200)
		dLive1 = (Pm + Pr) / dLength;
	else
		dLive1 = Pr / dLength;

	double dLive2 = dDistApply + Ps / dLength;

	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h$a%d.%d ���� ������ ����$n", nIndexTitle, nIndexMiddle++);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	*XLText += 1;
	XLText->AddFormatText("$h%d) ���� ����$n$n", nIndexSmall++);

	long nIndexTiny = 1;
	XLText->AddFormatText("$h$r%s �ۿ�����$n", GetNumberString(nIndexTiny++, NUM_CIRCLE));

	XLText->AddFormatText("$h$r$r$e- ��ü����$r$r$r$c=$r$%s / %s$m+06$c=$r %s %s$n", 
		pData->GetStrDouble(m_dSelfWeight, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(dLength, UNIT_CGS_M, TRUE, -1, FALSE, 8),  pData->GetStrDouble(m_dSelfWeight / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	if(m_dProtectionRiseFooting > 0)
	{
		XLText->AddFormatText("$h$r$r$e- �λ������$r$r$r$c=$r%s / %s$m+06$c=$r %s %s$n", 
			pData->GetStrDouble(m_dProtectionRiseFooting, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8) , pData->GetStrDouble(dLength, UNIT_CGS_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(m_dProtectionRiseFooting / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}
	if(m_dMassWeight > 0)
	{
		XLText->AddFormatText("$h$r$r$e- Mass ����(DC) DC$r$r$r$r$c=$r%s / %s$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dMassWeight, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(m_dMassWeight / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}

	if(pData->IsRailDesign2017())
	{
		CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			// ���������� ��� ���� ���������� ������ �״�� �����մϴ�. 
			XLText->AddFormatText("$h$r$r$e- ��������$m+03$c=$r%.3f$m+06$c=$r %s %s$n", 
				GetValueUnitChange(pLoad->m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M,pData->m_szTonf_M), pData->GetStrDouble(m_dLiveLoad, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			m_dLiveLoad = pLoad->m_dLiveLoaUR_Pvl;
		}
		else
		{
			double B = pLoad->m_bSingleLane ? pLoad->m_dRail_WidthDistribute : pLoad->m_dRail_WidthDistribute + toM(pLoad->m_dRail_DistBetweenRail);
			double I = (pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? 0 : pLoad->m_dCoefficientImpact[0];
			double N = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? 1 : 2;	

			double Pd = Round(GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf)*N*(1+I)/B,3);
			double dW = Round(GetValueUnitChange(pData->m_DistributionLoadFront,UNIT_CGS_TONF_M,pData->m_szTonf_M)*N*(1+I)/B,3);

			if(pData->IsKRL_2012())
			{
				// �̵������� ��� 2x����P / ������ ���� + w �� ����˴ϴ�.
				XLText->AddFormatText("$h$r$r$e- ��������$m+03$c=$r(2 x %.3f) / %.3f + %.3f$m+09$c=$r %s %s$n", 
					Pd, dLength, dW, pData->GetStrDouble(m_dLiveLoad, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			}
			else
			{
				// �̵������� ��� ���߿� �ش��ϴ� ���� �ణ�Ÿ��� ������ �����մϴ�. 
				XLText->AddFormatText("$h$r$r$e- ��������$m+03$c=$r%.3f / %.3f$m+06$c=$r %s %s$n", 
					Pd, pData->m_RailDistWheel, pData->GetStrDouble(m_dLiveLoad, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			}
		}
	}
	else
	{
		if(m_dLiveLoad > 0)
		{
			if(pBri->m_dLengthBridge > 8400)
			{
				XLText->AddFormatText("$h$r$r$e- %s$r$r$r$c=$r(%s + 2 x %s) / %s$m+09$c=$r %s %s$n", pData->GetStringLoadDB(FALSE),
					pData->GetStrDouble(Pf, UNIT_CGS_TONF, TRUE, 3), pData->GetStrDouble(Pr, UNIT_CGS_TONF, TRUE, 3), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(dLive1, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			}
			else if(pBri->m_dLengthBridge > 4200)
			{
				XLText->AddFormatText("$h$r$r$e- %s$r$r$r$c=$r(%s + %s) / %s$m+09$c=$r %s %s$n", pData->GetStringLoadDB(FALSE),
					pData->GetStrDouble(Pm, UNIT_CGS_TONF, TRUE, 3), pData->GetStrDouble(Pr, UNIT_CGS_TONF, TRUE, 3), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(dLive1, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			}
			else
			{
				XLText->AddFormatText("$h$r$r$e- %s$r$r$r$c=$r%s / %s$m+09$c=$r %s %s$n", pData->GetStringLoadDB(FALSE),
					pData->GetStrDouble(Pr, UNIT_CGS_TONF, TRUE, 3), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(dLive1, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			}

			XLText->AddFormatText("$h$r$r$e- %s$r$r$r$c=$r%s + %s / %s$m+09$c=$r %s %s$n", pData->GetStringLoadDB(TRUE),
				pData->GetStrDouble(dDistApply, UNIT_CGS_TONF_M2), pData->GetStrDouble(Ps, UNIT_CGS_TONF), pData->GetStrDouble(dLength, UNIT_CGS_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(dLive2, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
			XLText->AddFormatText("$h$r$r$e- �� %s�� %s �� �ִ������� ����Ѵ�.$n", pData->GetStringLoadDB(FALSE), pData->GetStringLoadDB(TRUE));
		}
	}

	if(m_dInteriorWaterWeight > 0)
	{
		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dHeightWater = toM(pBri->m_dHeightInteriorWaterPress);

		XLText->AddFormatText("$h$r$r$e- ���μ���$r$r$r$c=$r%.3f x %s$m+09$c=$r %s %s$n", 
			dHeightWater, pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3),
			pData->GetStrDouble(m_dInteriorWaterWeight, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}


	XLText->AddFormatText("$h$n$r%s �ִ� ���� �ݷ�$n", GetNumberString(nIndexTiny++, NUM_CIRCLE));
	CString str1(_T("")), str2(_T("")), str3(_T("")), str4(_T("")), str5(_T(""));
	str1 = pData->GetStrDouble(m_dSelfWeight / dLength, UNIT_CGS_TONF_M2);
	
	if(m_dProtectionRiseFooting > 0)
	{
		str2.Format(_T(" + %s"), pData->GetStrDouble(m_dProtectionRiseFooting / dLength, UNIT_CGS_TONF_M2, TRUE));
	}	
	if(m_dLiveLoad > 0)
	{
		str3.Format(_T(" + %s"), pData->GetStrDouble(m_dLiveLoad, UNIT_CGS_TONF_M2, TRUE));
	}
	if(m_dMassWeight > 0)
	{
		str4.Format(_T(" + %s"), pData->GetStrDouble(m_dMassWeight / dLength, UNIT_CGS_TONF_M2, TRUE));
	}
	if(m_dInteriorWaterWeight > 0)
	{
		str5.Format(_T(" + %s"), pData->GetStrDouble(m_dInteriorWaterWeight, UNIT_CGS_TONF_M2, TRUE));
	}
	if(m_dProtectionRiseFooting > 0 || m_dLiveLoad > 0 || m_dMassWeight > 0 || m_dInteriorWaterWeight > 0)
	{
		XLText->AddFormatText("$h$r$r$iQmax$r$c=$r(%s%s%s%s%s)$n", str1, str2, str3, str4, str5);
		XLText->AddFormatText("$h$r$r$r$c=$r%s %s$n", pData->GetStrDouble(m_dQMax[0], UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);
	}
	else // �������߸� �ִ� ���
	{
		XLText->AddFormatText("$h$r$r$iQmax$r$c=$r%s %s$n",  pData->GetStrDouble(m_dQMax[0], UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);
	}
	*XLText += 1;

	WriteCalcSupport_Usd(XLText, nJijum, nIndexSmall, bEarthquake);
}

void CARcBridgeCalcFooting::WriteCalcFootingSupportCheck_Lsd(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
	if(pBoring == NULL) return;

	CDPoint vAng(0,1);
	double dStation = pBri->GetStationOnJijum(nJijum);
	double dWidth = pBri->GetWidthSlabAct(dStation, vAng);
	double dLength = GetLengthBox();

	long nLine = 0;
	long nSpan = 0;
	double dWidthBetweenCurb = pBri->GetWidthTotalRoadWay(dStation);
	double UW_Ascon  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	
 	double dLaneLoad = GetValueUnitChange(m_pStd->m_pLoad->m_dLaneLoad,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	// (ARCBRIDGE-2814) LSD���� ������ �߰��ϸ鼭 �λ���� ���ǿ� ���� ������ ������� �������̱� ������ ���̿����� �λ���� ������ ���Խ�Ű�� �ʴ´�.
	
	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h$a%d.%d ���� ������ ����$n", nIndexTitle, nIndexMiddle++);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	*XLText += 1;
	XLText->AddFormatText("$h%d) ���� ����$n$n", nIndexSmall++);

	long nIndexTiny = 1;
	XLText->AddFormatText("$h$r%s �ۿ�����$n", GetNumberString(nIndexTiny++, NUM_CIRCLE));

	XLText->AddFormatText("$h$r$r$e- ��ü���� DC$r$r$r$r$c=$r%s / %s$m+09$c=$r %s %s$n", 
		pData->GetStrDouble(m_dSelfWeight, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(m_dSelfWeight / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);

	if(m_dProtectionRiseFooting > 0)
	{
		XLText->AddFormatText("$h$r$r$e- �������� EV$r$r$r$r$c=$r%s / %s$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dProtectionRiseFooting, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8) , pData->GetStrDouble(dLength, UNIT_CGS_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(m_dProtectionRiseFooting / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}

	if(m_dAdditionalDC > 0)
	{
		XLText->AddFormatText("$h$r$r$e- ��Ÿ �������� DC$r$r$r$r$c=$r$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dAdditionalDC, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}
	if(m_dPaveWeight > 0)
	{
		XLText->AddFormatText("$h$r$r$e- �������� DW$r$r$r$r$c=$r(%.0f x %s) x %s / %s$m+09$c=$r %s %s$n", 
			pBri->m_dTP, pData->GetStrDouble(UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M), pData->GetStrDouble(dWidth, UNIT_CGS_M),
			pData->GetStrDouble(m_dPaveWeight, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}
	if(m_dMassWeight > 0)
	{
		XLText->AddFormatText("$h$r$r$e- Mass ����(DC) DC$r$r$r$r$c=$r%s / %s$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dMassWeight, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 8), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(m_dMassWeight / dLength, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}
	if(m_dAdditionalDW > 0)
	{
		XLText->AddFormatText("$h$r$r$e- ��Ÿ �������� DW$r$r$r$r$c=$r$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dAdditionalDW, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}
	if(m_dLiveLoad > 0)
	{
		if(dLength > 12000)
		{
			XLText->AddFormatText("$h$r$r$e- ǥ��Ʈ������$r$r$r$r$c=$r(%s + 2 x %s + %s) / %s$m+09$c=$r %s %s$n", 
				CommaC(m_dPf, 3), CommaC(m_dPm, 3), CommaC(m_dPr, 3), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(m_dLive1, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$h$r$r$e- ǥ��Ʈ������$r$r$r$r$c=$r(2 x %s + %s) / %s$m+09$c=$r %s %s$n", 
				CommaC(m_dPm, 3), CommaC(m_dPr, 3), pData->GetStrDouble(dLength, UNIT_CGS_M),  pData->GetStrDouble(m_dLive1, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
		}
		XLText->AddFormatText("$h$r$r$e- ǥ����������$r$r$r$r$c=$r%s x 0.75 + %s$m+09$c=$r %s %s$n", 
			pData->GetStrDouble(m_dLive1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dLaneLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dLive2, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$r$r$e- �� ǥ��Ʈ�����߰� ǥ��Ʈ������ x 0.75 + ǥ���������� �� �ִ� ���߿����� ����Ѵ�.$n");
	}
	if(m_dInteriorWaterWeight > 0)
	{
		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dHeightWater = pBri->m_dHeightInteriorWaterPress;

		XLText->AddFormatText("$h$r$r$e- ���μ���$r$r$r$r$c=$r%.0f x %s$m+09$c=$r %s %s$n", 
			dHeightWater, pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3),
			pData->GetStrDouble(m_dInteriorWaterWeight, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 8), pData->m_szTonf_M2);
	}


	XLText->AddFormatText("$h$n$r%s �ִ� ���� �ݷ�$n", GetNumberString(nIndexTiny++, NUM_CIRCLE));
	for(long iEnv=0; iEnv<eCountEnvTypeLsd-1; ++iEnv)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)iEnv;
		CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		XLText->AddFormatText("$h$r$r$e- %s$n", szTitle);

		CString str1(_T("")), str2(_T(""));
		for(long ix=0; ix<5; ++ix)
		{
			if(ix == 2 && m_dProtectionRiseFooting == 0)
				continue;
			if(ix == 4 && m_dInteriorWaterWeight == 0)
				continue;

			if(m_dLoadFactor[iEnv][ix] > 0)
			{
				if(str1.IsEmpty() == FALSE)
					str1 += _T(" + ");

				double dLoad(0);
				switch (ix)
				{
				case 0:		// DC
					dLoad = m_dSelfWeight/dLength + m_dMassWeight/dLength + m_dAdditionalDC;	break;
				case 1:		// DW
					dLoad = m_dPaveWeight + m_dAdditionalDW;	break;
				case 2:		// EV
					dLoad = m_dProtectionRiseFooting / dLength;	break;
				case 3:		// Ȱ����
					dLoad = m_dLiveLoad;	break;
				case 4:		// ���μ���
					dLoad = m_dInteriorWaterWeight;	break;
				default:
					dLoad = 0; break;
				}
				str2.Format(_T("%s x %.2f"), pData->GetStrDouble(dLoad, UNIT_CGS_TONF_M2), m_dLoadFactor[iEnv][ix]);

				str1 += str2;
			}
		}

		XLText->AddFormatText("$h$r$r$iQmax$r$c=$r%s$n", str1);
		XLText->AddFormatText("$h$r$r$r$c=$r%s %s$n", pData->GetStrDouble(m_dQMax[iEnv], UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);
	}
	
	*XLText += 1;

	WriteCalcSupport_Lsd(XLText, nJijum, nIndexSmall, bEarthquake);
}

void CARcBridgeCalcFooting::WriteCalcFootingBuoyancyCheck(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE || pBri->m_bApplyGroundWaterLevel == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double dUWWater = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h$a%d.%d �η� ����$n$n", nIndexTitle, nIndexMiddle++);

	XLText->AddFormatText("$h: �η¿� ���� ������ Fs = %.1f (R / B)$n", m_dSF_Buoyance);
	XLText->AddFormatText("$h  �η¿� ���� �������� ��ü���߰� ���鸶����, �������߸��� ���$n");
	XLText->AddFormatText("$h  (�ּ����� H = %s %s �� ���Ͽ� ����)$n$n", pData->GetStrDouble(0.0, UNIT_CGS_M), pData->m_szM);

	long nIndexSmall = 1;
	CString strName, strCalc, strValue;
	long nSize = m_sArrSelfWeight[0].GetSize();
	//////////////////////////////////////////////////////////////////////////
	XLText->AddFormatText("$h%d) ��ü ����%s", nIndexSmall++, bLsd ? _T("(DC)") : _T(""));
	for(long ix = 0; ix < nSize; ix++)
	{
		strName = m_sArrSelfWeight[0].GetAt(ix);
		strCalc = m_sArrSelfWeight[1].GetAt(ix);
		strValue = m_sArrSelfWeight[2].GetAt(ix);
		XLText->AddFormatText("$n$h$c'-$r%s$m+03$c:$r%s$m+11$c=$r$me2$i$f3%s$r$r$r %s", strName, strCalc, strValue, pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$-21");
	XLText->AddFormatText("$n$h$m+15��$r$c=$r$me2$i%s$r$r$r %s$n", pData->GetStrDouble(m_dSelfWeight, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);

	double dStation = pBri->GetStationOnJijum(0);
	double dHeightBox = bLsd? pBri->GetHeightBridgeBox(dStation) : toM(pBri->GetHeightBridgeBox(dStation));
	double hs_ = bLsd? (pBri->m_dGroundWaterLevel - pBri->m_dELSB) : toM(pBri->m_dGroundWaterLevel - pBri->m_dELSB);
	double hs = min(hs_, dHeightBox);

	//////////////////////////////////////////////////////////////////////////
	if(pBri->m_bExistProtectionRiseFooting)
	{
		double UW_BackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double UW_BackFill_W = GetValueUnitChange(pBri->m_dUnitWeigthBackFillWater, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double W  = bLsd? pBri->m_dPRF_W : toM(pBri->m_dPRF_W);
		double H1 = bLsd? pBri->m_dPRF_H1 : toM(pBri->m_dPRF_H1);
		double H2 = bLsd? pBri->m_dPRF_H2 : toM(pBri->m_dPRF_H2);
		double D1 = dHeightBox - hs;
		double D2 = hs - H1 - H2;
		double Ko = m_pStd->m_dCoefficientKo;
		double F1 = Ko*UW_BackFill*D1*D1*tan(ToRadian(pBri->m_dAngleFrictionBackFill * 2./3.));

		*XLText += 1;
		XLText->AddFormatText("$h%d) ���鸶����$n", nIndexSmall++);
		XLText->AddFormatText("$h$r$cF$r$c=$r2 �� C �� D + Ku �� �� x D�� x tan��$n");
		XLText->AddFormatText("$h$r���ı����� ���� ������ ���鿡 �����Ͽ� �����Ƿ� �������� ������ ��븶��������$n");
		XLText->AddFormatText("$h$r �����Ͽ� �� = 2/3 �� �� �� ����$n");
		//
		XLText->AddFormatText("$h$r'- ���ϼ��� �̻�$n");
		XLText->AddFormatText("$h$r$cF1$r$c=$rKu x rt x D1�� x tan��$n");
		XLText->AddFormatText("$h$r$r$c=$r$%.3f x %s x %s�� x tan(%.2f x 2/3)$n", 
			Ko, pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3, TRUE), pData->GetStrDouble(D1, UNIT_CGS_M), pBri->m_dAngleFrictionBackFill);
		XLText->AddFormatText("$h$r$r$c=$r$%s %s$n", pData->GetStrDouble(F1, UNIT_CGS_TONF_M, TRUE), pData->m_szTonf_M);
		//
		XLText->AddFormatText("$h$r'- ���ϼ��� ����$n");
		XLText->AddFormatText("$h$r$cF2$r$c=$rKu x (2 x rt x D1 x D2 + D2�� x rsub) x tan��$n");
		XLText->AddFormatText("$h$r$r$c=$r$%.3f x (2 x %s x %s x %s + %s��$n", 
			Ko, pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3, TRUE), pData->GetStrDouble(D1, UNIT_CGS_M), pData->GetStrDouble(D2, UNIT_CGS_M), pData->GetStrDouble(D2, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$r$rx %s) x tan(%.2f x 2/3)$n", pData->GetStrDouble(UW_BackFill_W, UNIT_CGS_TONF_M3, TRUE), pBri->m_dAngleFrictionBackFill);
		XLText->AddFormatText("$h$r$r$c=$r$%s %s$n", pData->GetStrDouble(m_dSideFriction - F1, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		//
		XLText->AddFormatText("$h$r�� �� = %s + %s = %s %s$n", pData->GetStrDouble(F1, UNIT_CGS_TONF_M), pData->GetStrDouble(m_dSideFriction - F1, UNIT_CGS_TONF_M), pData->GetStrDouble(m_dSideFriction, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$r�ر����� ��ü�� �ֺ� ��ä�� ����� ���� �������� �������� ���� ��Ȯ�Ǽ��� ����Ͽ�$n");
		XLText->AddFormatText("$h$r �����Ѵ�. [ö��������� 2004(�����, p 170]$n");

		*XLText += 1;
		XLText->AddFormatText("$h%d) %s", nIndexSmall++, bLsd ? _T("�������� (EV)") : _T("�λ������"));
		XLText->AddFormatText("$n$h$c'-$r%s$m+05$c:$r%s x %s x %s x 2 EA$m+09$c=$r$me2$i%s$r$r$r %s", _T("�� (���ϼ��� �̻�)"), 
			pData->GetStrDouble(dHeightBox - hs, UNIT_CGS_M), pData->GetStrDouble(W, UNIT_CGS_M), pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3, TRUE), 
			pData->GetStrDouble((dHeightBox - hs) * W * UW_BackFill * 2, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);
		XLText->AddFormatText("$n$h$c'-$r%s$m+05$c:$r%s x %s x %s x 2 EA$m+09$c=$r$me2$i%s$r$r$r %s", _T("�� (���ϼ��� ����)"), 
			pData->GetStrDouble(hs, UNIT_CGS_M), pData->GetStrDouble(W, UNIT_CGS_M), pData->GetStrDouble(UW_BackFill_W, UNIT_CGS_TONF_M3, TRUE), 
			pData->GetStrDouble(hs * W * UW_BackFill_W * 2, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);
		XLText->AddFormatText("$n$h$c'-$r%s$m+05$c:$r%s x %s x %s x 2 EA$m+09$c=$r$me2$i%s$r$r$r %s", _T("���ϼ�"), 
			pData->GetStrDouble(hs, UNIT_CGS_M), pData->GetStrDouble(W, UNIT_CGS_M), pData->GetStrDouble(dUWWater, UNIT_CGS_TONF_M3, TRUE), 
			pData->GetStrDouble(hs * W * dUWWater * 2, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);
		XLText->AddFormatText("$h$-21");
		XLText->AddFormatText("$n$h$m+15��$r$c=$r$me2$i%s$r$r$r %s$n", pData->GetStrDouble(m_dProtectionRiseFooting, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);
	}

	//////////////////////////////////////////////////////////////////////////
	double B = bLsd? GetLengthBox() : toM(GetLengthBox());
//	double B = toM(pBri->m_dLengthBridge);
	*XLText += 1;
	XLText->AddFormatText("$h%d) �η�$n", nIndexSmall++);
	XLText->AddFormatText("$h$r$cU$r$c=$rB �� hs �� ��w  (���⼭, hs = ���������� �� ���ǰ� - ���ϼ���)$n");
	XLText->AddFormatText("$h$r$r$c=$r%s x %s �� %s$n", pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(hs, UNIT_CGS_M), pData->GetStrDouble(dUWWater, UNIT_CGS_TONF_M3, TRUE));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(m_dBuoyance, UNIT_CGS_TONF_M, FALSE, -1, TRUE), pData->m_szTonf_M);

	//////////////////////////////////////////////////////////////////////////
	double dSF = (m_dSelfWeight + m_dSideFriction + m_dProtectionRiseFooting) / m_dBuoyance;
	BOOL bOK = dSF >= m_dSF_Buoyance ? TRUE : FALSE;
	*XLText += 1;
	XLText->AddFormatText("$h%d) ���� ����$n", nIndexSmall++);
	if(m_dSideFriction + m_dProtectionRiseFooting > 0)
	{
		CString str2 = _T(""), str3 = _T("");
		CString str1 = pData->GetStrDouble(m_dSelfWeight, UNIT_CGS_TONF_M);
		if(m_dSideFriction > 0)
		{
			str2.Format(_T(" + %s"), pData->GetStrDouble(m_dSideFriction, UNIT_CGS_TONF_M));
		}
		if(m_dProtectionRiseFooting > 0)
		{
			str3.Format(_T(" + %s"), pData->GetStrDouble(m_dProtectionRiseFooting, UNIT_CGS_TONF_M));
		}
		XLText->AddFormatText("$h$r$cR$r$c=$r%s%s%s$n", str1, str2, str3);
		XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(m_dSelfWeight + m_dSideFriction + m_dProtectionRiseFooting, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	}
	else
	{
		XLText->AddFormatText("$h$r$cR$r$c=$r%s %s$n", pData->GetStrDouble(m_dSelfWeight, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$r$cFs$r$c=$r%s / %s$n", pData->GetStrDouble(m_dSelfWeight + m_dSideFriction + m_dProtectionRiseFooting, UNIT_CGS_TONF_M), pData->GetStrDouble(m_dBuoyance, UNIT_CGS_TONF_M));
	XLText->AddFormatText("$h$r$r$c=$r$%.3f %s %.3f   ��  %s$n", dSF, bOK ? _T("��") :  _T("<"), m_dSF_Buoyance, bOK ? _T("O.K") :  _T("N.G"));

	if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
	{
		*XLText += 1;
		XLText->AddFormatText("$h�� �η¹��� ��Ŀ$n");
		strValue.Format(_T("��Ŀ �����  :  %.%s x 1.21 - %s  =  %s %s"), 
			pData->GetStrDouble(m_dBuoyance, UNIT_CGS_TONF_M), pData->GetStrDouble(m_dSelfWeight + m_dSideFriction, UNIT_CGS_TONF_M), 
			pData->GetStrDouble(m_dBuoyance*1.21 - m_dSelfWeight - m_dSideFriction, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$r%s $n", strValue);
	}
}

void CARcBridgeCalcFooting::CalculateSupportCheck_Usd(BOOL bEarthquake)
{
	m_sArrSupport.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
	if(pBoring == NULL) return;
	CExFootingApp *pExFoot = &pFooting->m_exFooting;

	long nType = pFooting->m_nTypeEquation_Qa;

	CDPoint vAng(0,1);
	double dStation = pBri->GetStationOnJijum(nJijum);
	double dWidth = toM(pBri->GetWidthSlabAct(dStation, vAng));
	double dLength = toM(GetLengthBox());
	if(pBri->m_bExistProtectionRiseFooting)
	{
		dLength += toM(pBri->m_dPRF_W * 2);
	}

	m_dVerTotal = (m_dSelfWeight + m_dProtectionRiseFooting) / dLength;
	
	double dUWConc = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dMassWeight = 0;
	if(pExFoot)
	{
		if(pExFoot->m_nType != EXFOOTING_TYPE_MASS)
			dMassWeight = 0;
		else if(pExFoot->m_nCountDan==0 || pExFoot->m_bFrontDanMass==FALSE)
			dMassWeight = toM(pExFoot->GetWidth()) * toM(pExFoot->GetMaxHeight()) * dUWConc;
		else
		{
			double dArea(0);
			double dHExt = pExFoot->GetMaxHeight();
			long nDan = pExFoot->m_nCountDan;
			long i=0; for(i=0; i<nDan; i++)
			{
				double dH = pExFoot->m_dEFH[i];
				double dW = pExFoot->m_dEFW[i];
				double dWs = pExFoot->m_dEFS[i];

				dArea += (dW * 2 + dWs) * dH / 2.;
				dHExt -= dH;
			}
			dArea += pExFoot->GetWidth() * dHExt;

			dMassWeight = toM2(dArea) * dUWConc;
		}
	}
	m_dMassWeight = dMassWeight;

	if(pData->IsRailDesign2017())
	{
		CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			// ���������� ��� ���� ���������� ������ �״�� �����մϴ�. 
			m_dLiveLoad = pLoad->m_dLiveLoaUR_Pvl;
		}
		else
		{
			double B = pLoad->m_bSingleLane ? pLoad->m_dRail_WidthDistribute : pLoad->m_dRail_WidthDistribute + toM(pLoad->m_dRail_DistBetweenRail);
			//				double I = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? pLoad->m_dCoefficientImpact : 0;
			double I = (pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? 0 : pLoad->m_dCoefficientImpact[0];
			double N = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? 1 : 2;	

			double Pd = Round(pData->m_RailLoadWheel*N*(1+I)/B*dWidth,3);
			double dW = Round(pData->m_DistributionLoadFront*N*(1+I)/B*dWidth,3);

			if(pData->IsKRL_2012())
			{
				// �̵������� ��� 2x����P / ������ ���� + w �� ����˴ϴ�.
				m_dLiveLoad = (2 * Pd) / dLength + dW;
			}
			else
			{
				// �̵������� ��� ���߿� �ش��ϴ� ���� �ణ�Ÿ��� ������ �����մϴ�. 
				m_dLiveLoad = Pd / pData->m_RailDistWheel;
			}
		}
	}
	else
	{
		double dPfDesign  = GetValueUnitChange(m_pStd->GetWheelLoadFront() ,UNIT_CGS_TONF,pData->m_szTonf);
		double dPrDesign  = GetValueUnitChange(m_pStd->GetWheelLoadRear() ,UNIT_CGS_TONF,pData->m_szTonf);
		double dPmDesign  = GetValueUnitChange(m_pStd->GetWheelLoadMiddle() ,UNIT_CGS_TONF,pData->m_szTonf);
		double dPsDesign  = GetValueUnitChange(m_pStd->GetWheelLoadPs() ,UNIT_CGS_TONF,pData->m_szTonf);
		double E  = m_pStd->m_pLoad->GetEForEffectiveWidth(pBri, 1);
		double Pr = (2*dPrDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
		double Pf = (2*dPfDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
		// (ARCBRIDGE-3214) DL ���߻����ÿ��� EffectiveWidth�� ������ �ʰ� 3.0���� ������.
		double Pm = (dPmDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/EFFECTIVE_WIDTH_DL;
		double Ps = (dPsDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/EFFECTIVE_WIDTH_DL;
		double dDistApply = GetValueUnitChange(m_pStd->m_pLoad->m_dDistributionApply ,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

		double dLive1 = 0;
		if(pBri->m_dLengthBridge > 8400)
			dLive1 = (Pf + 2 * Pr) / dLength;
		else if(pBri->m_dLengthBridge > 4200)
			dLive1 = (Pm + Pr) / dLength;
		else
			dLive1 = Pr / dLength;

		double dLive2 = dDistApply + Ps / dLength;
		m_dLiveLoad = MAX(dLive1, dLive2);
	}

	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
	{
		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dHeightWater = toM(pBri->m_dHeightInteriorWaterPress);

		m_dInteriorWaterWeight = dUW_Water * dHeightWater;

		m_dVerTotal += m_dInteriorWaterWeight;
	}

	m_dMrTotal = 0;
	m_dMoTotal = 0;
	m_dQaEquation = 0;
	m_dLoadFactor[0][0] = 1.0;
	m_dQMax[0] = m_dVerTotal + m_dLiveLoad;
	if(m_dMassWeight > 0)
		m_dQMax[0] += m_dMassWeight / dLength;

	double NValue = pBoring->m_nNValue;
	double pi = ToRadian(pFooting->GetFrictionAngleBaseGround(NValue));
	double L  = dLength;
	double B  = dWidth;
	double Df = pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
	double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
	double r1 = pData->GetValueUnitType(pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double r2 = pData->GetValueUnitType(pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

	// Terzaghi //////////////////////////////////////////////////
	if(nType == 0)
	{
		// �������鿡�� ���ϼ��������� ����(dH)�� ������ ��(B_)���� ������쿡�� r1�� r_�� ����Ѵ�			
		double q	= r2*Df; // ????
		////////////////////////////////////////////////
		double eB	= fabs(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
		double B_	= B - 2*fabs(eB);
		double alpha= 1.0 + 0.3*B_/L; // ���簢��
		double beta = 0.5 - 0.1*B_/L;
		double Kpr	= GetKprValue(pFooting->GetFrictionAngleBaseGround(pBoring->m_nNValue));
		double  a   = exp((0.75*ConstPi-pi/2)*tan(pi));
		double Nq	= pow(a,2) / (2*pow(cos(ConstPi/4+pi/2),2));
		double Nc	= (Nq-1)/tan(pi);
		double Nr	= tan(pi)/2 * (Kpr/pow(cos(pi),2) - 1);

		m_dQaEquation = ( alpha*c*Nc + q*Nq + beta*r1*B_*Nr );
	}
	// Meyerhof //////////////////////////////////////////////////
	// ��ȿ���ϸ����� ������� �ʴ´�.....
	if(nType==1)
	{
		double q	= r2*Df; // ????
		double B_	= B;
		double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
		double Nc	= (pi==0) ? 5.14 : (Nq-1)/tan(pi);
		double Nr	= (Nq-1)*tan(1.4*pi);

		m_dQaEquation = ( c*Nc + q*Nq + 0.5*r1*B_*Nr );
	}
	// Hansen   //////////////////////////////////////////////////
	if(nType == 2)
	{
		double beta = 0; // ���ݰ��..
		double eB	= fabs(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
		double B_	= B - 2*fabs(eB);
		double q	= r2*Df;	// ????
		double k    = (Df/B<=1) ? Df/B : atan(Df/B);
		//////////////////////////////////////////////////////////////////////////
		double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
		double Nc	= (Nq-1)/tan(pi);
		double Nr	= 1.5*(Nq-1)*tan(pi);
		double Sc	= 1 + (Nq/Nc) * (B/L);
		double Sq	= 1 + (B/L) * tan(pi);
		double Sr	= 1 - 0.4 * B/L;
		double Sc_	= 0.2 * B/L;
		double dc	= 1 + 0.4*k;
		double dq	= 1 + 2*tan(pi)*pow(1-sin(pi),2)*k;
//		double dr	= 1.;
		double dc_	= 0.4*k;
		double gc	= 1 - beta/147.0;
		double gq	= pow(1 - 0.5*tan(ToRadian(beta)),5);
		double gr	= gq;

		if(pi==0)
		{
			double ic_	= 1.;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double bc_	= 1.;	// ���������� ��簡 �����Ƿ� 1

			m_dQaEquation = (5.14*c*(1+Sc_+dc_-ic_-dc_-bc_)+q);
		}
		else
		{
			double dr	= 1.;
			double ic	= 1.;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double iq	= 1.;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double ir	= 1.;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double bc	= 1.;	// ���������� ��簡 �����Ƿ� 1
			double bq	= 1.;	// ���������� ��簡 �����Ƿ� 1
			double br	= 1.;	// ���������� ��簡 �����Ƿ� 1

			m_dQaEquation = (c*Nc*Sc*dc*ic*gc*bc + q*Nq*Sq*dq*iq*gq*bq + 0.5*r1*B_*Nr*Sr*dr*ir*gr*br);
		}
	}
}

void CARcBridgeCalcFooting::CalculateSupportCheck_Lsd( BOOL bEarthquake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
	if(pBoring == NULL) return;
	CExFootingApp *pExFoot = &pFooting->m_exFooting;

	ClearVectorPtr(m_pListLsdFootingSafe);

	CLsdFootingDesign *pLsdFooting = new CLsdFootingDesign;
	m_pListLsdFootingSafe.push_back(pLsdFooting);

	pLsdFooting->m_eDesignType = pData->IsDesignKDS2019() ? eDesignDoro2019 : eDesignDoro2015;

	SetLsdFooting(nJijum);
	pLsdFooting->AddLoad(eLSD_COMBINATION_LIMIT, _T(""), 100, 100, 100, 100);

	// (ARCBRIDGE-2596) ������ �ִ� �������並 �����ϸ� �����Ͱ� ��������.
	pLsdFooting->CalculateAll();

	CDPoint vAng(0,1);
	double dLength = GetLengthBox();
	// (ARCBRIDGE-2814) LSD���� ������ �߰��ϸ鼭 �λ���� ���ǿ� ���� ������ ������� �������̱� ������ ���̿����� �λ���� ������ ���Խ�Ű�� �ʴ´�.

	long nLine = 0;
	long nSpan = 0;
	double dStationBridge    = pBri->GetStationOnJijum(nSpan,nLine);
	double dWidthBetweenCurb = pBri->GetWidthTotalRoadWay(dStationBridge);
	double dWidthSlab        = pBri->GetWidthSlabAct(dStationBridge,vAng);
	double UW_Ascon  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_Conc  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dMassWeight = 0;
	
	if(pExFoot)
	{
		if(pExFoot->m_nType != EXFOOTING_TYPE_MASS)
			dMassWeight = 0;
		else if(pExFoot->m_nCountDan==0 || pExFoot->m_bFrontDanMass==FALSE)
			dMassWeight = pExFoot->GetWidth() * pExFoot->GetMaxHeight() * UW_Conc;
		else
		{
			double dArea(0);
			double dHExt = pExFoot->GetMaxHeight();
			long nDan = pExFoot->m_nCountDan;
			long i=0; for(i=0; i<nDan; i++)
			{
				double dH = pExFoot->m_dEFH[i];
				double dW = pExFoot->m_dEFW[i];
				double dWs = pExFoot->m_dEFS[i];

				dArea += (dW * 2 + dWs) * dH / 2.;
				dHExt -= dH;
			}
			dArea += pExFoot->GetWidth() * dHExt;

			dMassWeight = dArea * UW_Conc;
		}
	}
	
	m_dPaveWeight = pBri->m_dTP * UW_Ascon * dWidthBetweenCurb / dWidthSlab;
	m_dMassWeight = dMassWeight;

	m_dAdditionalDC = GetValueUnitChange(m_pStd->m_pLoad->m_DeadLoadDistribution[0],UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	m_dAdditionalDW = GetValueUnitChange(m_pStd->m_pLoad->m_DeadLoadDistribution[1],UNIT_CGS_TONF_M2,pData->m_szTonf_M2) - m_dPaveWeight;

	long nLane = m_pStd->m_pLoad->m_nCaseDesignLane[0];
	double dPmDesign  = GetValueUnitChange(m_pStd->GetWheelLoadMiddle() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPrDesign  = GetValueUnitChange(m_pStd->GetWheelLoadRear() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPfDesign  = GetValueUnitChange(m_pStd->GetWheelLoadFront() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dLaneLoad = GetValueUnitChange(m_pStd->m_pLoad->m_dLaneLoad,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double E  = frM(m_pStd->m_pLoad->GetEForEffectiveWidth(pBri, nLane));

	m_dPr = (2*nLane*dPrDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
	m_dPm = (2*nLane*dPmDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;
	m_dPf = (2*nLane*dPfDesign*(1+m_pStd->m_pLoad->m_dCoefficientImpact[0]))/E;

	if(dLength > 12000)
		m_dLive1 = (m_dPf + 2 * m_dPm + m_dPr) / dLength;			// (ARCBRIDGE-3466) (Pr + 2 * Pm + Pr) => (Pr + 2 * Pm + Pf) (ARCBRIDGE-2962)
	else
		m_dLive1 = (2 * m_dPm + m_dPr) / dLength;
	m_dLive2 = m_dLive1 * 0.75 + dLaneLoad;
	m_dLiveLoad = MAX(m_dLive1, m_dLive2);

	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
	{
		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dHeightWater = pBri->m_dHeightInteriorWaterPress;

		m_dInteriorWaterWeight = dUW_Water * dHeightWater;
	}

	// () �ִ����ݹݷ��� ����, �ش� ���  ��� ���� ���Ѵ�.
	m_dVerTotal = m_dSelfWeight / dLength;
	m_dMrTotal = 0;
	m_dMoTotal = 0;
	m_dQaEquation = 0;
	for(long iEnv=0; iEnv<eCountEnvTypeLsd-1; ++iEnv)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)iEnv;
		CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		m_dLoadFactor[iEnv][0] = m_dLoadFactor[iEnv][0] = m_dLoadFactor[iEnv][0] = 0;

		ELSDCombKind eCombinEnv = eLSD_COMBINE_LIMIT_I;
		if(iEnv == eEnvExtremeI)
			eCombinEnv = eLSD_COMBINE_EXTREME_I;
		else if(iEnv == eEnvExtremeII)
			eCombinEnv = eLSD_COMBINE_EXTREME_II;
		else if(iEnv == eEnvService)
			eCombinEnv = eLSD_COMBINE_SERVICE_I;


		std::vector<CLsdLoadCase *> vLoadCase = pBri->m_plsdManager->GetLsdLoadCaseArray(eCombinEnv, eLSD_LOADCASE_DIR_BRI);
		for(auto ite=vLoadCase.begin(); ite != vLoadCase.end(); ++ite)
		{
			m_dLoadFactor[iEnv][0] = (*ite)->GetLoadFactor(eLSD_LOADCASE_DC1);
			m_dLoadFactor[iEnv][1] = (*ite)->GetLoadFactor(eLSD_LOADCASE_DW1);
			m_dLoadFactor[iEnv][2] = (*ite)->GetLoadFactor(eLSD_LOADCASE_EV);
			m_dLoadFactor[iEnv][3] = (*ite)->GetLoadFactor(eLSD_LOADCASE_LL1);
			if(pBri->m_nSelectInteriorWaterPress == 0)
				m_dLoadFactor[iEnv][4] = (*ite)->GetLoadFactor(eLSD_LOADCASE_EVI);
			else if(pBri->m_nSelectInteriorWaterPress == 1)
				m_dLoadFactor[iEnv][4] = (*ite)->GetLoadFactor(eLSD_LOADCASE_BPI);
			else if(pBri->m_nSelectInteriorWaterPress == 2)
				m_dLoadFactor[iEnv][4] = (*ite)->GetLoadFactor(eLSD_LOADCASE_WA21);

			if(m_dLoadFactor[iEnv][0] > 0)
				break;
		}

		m_dQMax[iEnv] = (m_dVerTotal + m_dAdditionalDC) * m_dLoadFactor[iEnv][0] + (m_dPaveWeight + m_dAdditionalDW) * m_dLoadFactor[iEnv][1] + m_dProtectionRiseFooting/dLength * m_dLoadFactor[iEnv][1] + m_dLiveLoad * m_dLoadFactor[iEnv][3] + m_dInteriorWaterWeight * m_dLoadFactor[iEnv][4];
		if(m_dMassWeight > 0)
			m_dQMax[iEnv] += m_dMassWeight/dLength * m_dLoadFactor[iEnv][0];
	}

	sLsdFootingLoad *pLoad = pLsdFooting->GetLoad(0);
	if(pLsdFooting->IsRock())
	{
		if(pFooting->m_nTypeEquation_Qa == 0)
		{
			// Hoek & Brown ���Ƚ�
			m_dQaEquation = pLoad->sSafeResult.Qu_HB;
		}
		else if(pFooting->m_nTypeEquation_Qa == 1)
		{
			// AASHTO ���Ƚ�
			m_dQaEquation = pLoad->sSafeResult.Qu_AAS;
		}
		else if(pFooting->m_nTypeEquation_Qa == 2)
		{
			// Canadian Geotechnical Society
			m_dQaEquation = pLoad->sSafeResult.Qu_CGS;
		}
	}
	// ���
	else
	{
		m_dQaEquation = pLoad->sSafeResult.Qu_Soil;
	}
}

// �η°���
void CARcBridgeCalcFooting::CalculateBuoyancyCheck()
{
	m_sArrSelfWeight[0].RemoveAll(); // ��ġ
	m_sArrSelfWeight[1].RemoveAll(); // ���	
	m_sArrSelfWeight[2].RemoveAll(); // ��

	m_dSF_Buoyance = 1.20;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double dStation = pBri->GetStationOnJijum(0);
	double dHeightBox = toM(pBri->GetHeightBridgeBox(dStation));
	double dLenBridge = toM(GetLengthBox());
	double dWidBridge = toM(pBri->GetWidthSlabAct(dStation, CDPoint(0,1)));
	double dUWConc = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUWWater = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

	double hs = toM(min(pBri->m_dGroundWaterLevel - pBri->m_dELSB, pBri->GetHeightBridgeBox(dStation)));
	if(bLsd)
	{
		dHeightBox = frM(dHeightBox);
		dLenBridge = frM(dLenBridge);
		dWidBridge = frM(dWidBridge);
		hs = frM(hs);
	}

	CString str = _T("");
	//////////////////////////////////////////////////////////////////////////
	// ��ü����
	m_dSelfWeight = 0;
	double dWeight(0);
	double dThick(0);
	// ������
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(bLsd)
			dThick = iUD == iUPPER ? pBri->m_dTS : pBri->m_dTS_Lower;
		else
			dThick = iUD == iUPPER ? toM(pBri->m_dTS) : toM(pBri->m_dTS_Lower);

		str = iUD == iUPPER ? _T("��ν�����") : _T("�Ϻν�����");
		m_sArrSelfWeight[0].Add(str);
		str.Format(_T("%s x %s x %s"), pData->GetStrDouble(dLenBridge, UNIT_CGS_M), pData->GetStrDouble(dThick, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
		m_sArrSelfWeight[1].Add(str);
		dWeight = dLenBridge * dThick * dUWConc;
		str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
		m_sArrSelfWeight[2].Add(str);
		m_dSelfWeight += dWeight;
	}
	// ������ü
	for(long iLR = iLEFT; iLR <= iLOWER; iLR++)
	{
		BOOL bLeft = iLR == iLEFT ? TRUE : FALSE;
		if(bLsd)
			dThick = bLeft ? pBri->m_dWS : pBri->m_dWE;
		else
			dThick = bLeft ? toM(pBri->m_dWS) : toM(pBri->m_dWE);

		double dStation = pBri->GetStationOnJijum(bLeft ? 0 : pBri->m_nQtyJigan);
		double dHeight = pBri->GetHeightBridgeBox(dStation) - pBri->m_dTS - pBri->m_dTS_Lower;
		if(bLsd == FALSE)
			dHeight = toM(dHeight);

		str = bLeft ? _T("������ü") : _T("������ü");
		m_sArrSelfWeight[0].Add(str);
		str.Format(_T("%s x %s x %s"), pData->GetStrDouble(dHeight, UNIT_CGS_M), pData->GetStrDouble(dThick, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
		m_sArrSelfWeight[1].Add(str);
		dWeight = dHeight * dThick * dUWConc;
		str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
		m_sArrSelfWeight[2].Add(str);
		m_dSelfWeight += dWeight;
	}
	// �߰���ü
	for(long nJijum = 1; nJijum < pBri->m_nQtyJigan; nJijum++)
	{
		CWallApp *pWall = pBri->GetInWall(nJijum - 1);
		str.Format(_T("�߰����� %d"), nJijum);
		double dStation = pBri->GetStationOnJijum(nJijum);
		double dUpperGirH = pBri->GetGirderHeight(nJijum, TRUE);
		double dLowerGirH = pBri->GetGirderHeight(nJijum, TRUE);
		double dHeight = pBri->GetHeightBridgeBox(dStation) - dUpperGirH - dLowerGirH;
		if(bLsd == FALSE)
			dHeight = toM(dHeight);

		m_sArrSelfWeight[0].Add(str);
		if(pWall->m_bTopIsColumn)
		{
			long nEa = pWall->m_nCountColumn;
			double H = 0./*, B = 0.*/, R = 0.;
			if(pWall->m_Column.m_nType == COL_TYPE_CIRCLE || pWall->m_Column.m_nType == COL_TYPE_POLY)
			{
				R = bLsd? (pWall->m_Column.m_dB[iOUTTER][iUPPER] / 2) : toM(pWall->m_Column.m_dB[iOUTTER][iUPPER] / 2);
				str.Format(_T("(�� x %s��) x %s x %s x %d / %s"), 
					pData->GetStrDouble(R, UNIT_CGS_M), pData->GetStrDouble(dHeight, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE), nEa, pData->GetStrDouble(dWidBridge, UNIT_CGS_M));
				m_sArrSelfWeight[1].Add(str);
				dWeight = ConstPi * R * R * dHeight * dUWConc * nEa / dWidBridge;
			}
			else if(pWall->m_Column.m_nType == COL_TYPE_RECTANGLE)
			{
				double B = bLsd? pWall->m_Column.m_dB[iOUTTER][iUPPER] : toM(pWall->m_Column.m_dB[iOUTTER][iUPPER]);
				H = bLsd? pWall->m_Column.m_dH[iOUTTER][iUPPER] : toM(pWall->m_Column.m_dH[iOUTTER][iUPPER]);
				str.Format(_T("(%s x %s) x %s x %s x %d / %s"), 
					pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dHeight, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE), nEa, pData->GetStrDouble(dWidBridge, UNIT_CGS_M));
				m_sArrSelfWeight[1].Add(str);
				dWeight = ConstPi * R * R * dHeight * dUWConc * nEa / dWidBridge;
			}
			else if(pWall->m_Column.m_nType == COL_TYPE_TRACK)
			{
				R = bLsd? pWall->m_Column.m_dR[iOUTTER][iUPPER] : toM(pWall->m_Column.m_dR[iOUTTER][iUPPER]);
				H = bLsd? (pWall->m_Column.m_dH[iOUTTER][iUPPER] - R * 2) : toM(pWall->m_Column.m_dH[iOUTTER][iUPPER] - R * 2);
				str.Format(_T("(�� x %s�� + %s x %s) x %s x %s x %d / %s"),
					pData->GetStrDouble(R, UNIT_CGS_M), pData->GetStrDouble(R*2, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dHeight, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE), nEa, pData->GetStrDouble(dWidBridge, UNIT_CGS_M));
				m_sArrSelfWeight[1].Add(str);
				dWeight = ConstPi * R * R * dHeight * dUWConc * nEa / dWidBridge;
			}
		}
		else
		{
			double dThick = bLsd? pWall->m_dW : toM(pWall->m_dW);
			str.Format(_T("%s x %s x %s"), pData->GetStrDouble(dHeight, UNIT_CGS_M), pData->GetStrDouble(dThick, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
			m_sArrSelfWeight[1].Add(str);
			dWeight = dHeight * dThick * dUWConc;
		}
		str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
		m_sArrSelfWeight[2].Add(str);
		m_dSelfWeight += dWeight;
	}
	// �Ŵ�
	for(long nJijum = 1; nJijum < pBri->m_nQtyJigan; nJijum++)
	{
		CWallApp *pWall = pBri->GetInWall(nJijum - 1);
		if(pWall->m_bTopIsColumn == FALSE) continue;

		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			str.Format(_T("%s�Ŵ� %d"), bUpperSlab ? _T("���") : _T("�Ϻ�"), nJijum);
			double dGirH = pBri->GetGirderHeight(nJijum, bUpperSlab);
			double dGirW = pBri->GetGirderWidth(nJijum, bUpperSlab);
			if(bLsd == FALSE)
			{
				dGirH = toM(dGirH);
				dGirW = toM(dGirW);
			}
			m_sArrSelfWeight[0].Add(str);
			str.Format(_T("%s x %s x %s"), 
				pData->GetStrDouble(dGirH, UNIT_CGS_M), pData->GetStrDouble(dGirW, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
			m_sArrSelfWeight[1].Add(str);
			dWeight = dGirH * dGirW * dUWConc;
			str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
			m_sArrSelfWeight[2].Add(str);
			m_dSelfWeight += dWeight;
		}
	}
	// ��ġ : �簢�κ��� �Ŵ��� ���ԵǹǷ� ����... / �Ŵ��� ���� �簢�� �ִ� ��� ����...
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
		long nIdxHunch = 1;
		long nJijum = 0;
		long nEa = 0;
		double dHH[2], dHW[2];

		// ��������
		dHH[iSTT] = pBri->GetThickHunch(nJijum, FALSE, FALSE, bUpperSlab);
		dHW[iSTT] = pBri->GetWidthHunch(nJijum, FALSE, bUpperSlab);
		dHH[iEND] = pBri->GetThickHunch(pBri->m_nQtyJigan, TRUE, FALSE, bUpperSlab);
		dHW[iEND] = pBri->GetWidthHunch(pBri->m_nQtyJigan, TRUE, bUpperSlab);
		if(bLsd == FALSE)
		{
			dHH[iSTT] = toM(dHH[iSTT]);
			dHW[iSTT] = toM(dHW[iSTT]);
			dHH[iEND] = toM(dHH[iEND]);
			dHW[iEND] = toM(dHW[iEND]);
		}

		nEa = (Compare(dHH[iSTT], dHH[iEND], _T("=")) && Compare(dHW[iSTT], dHW[iEND], _T("="))) ? 2 : 1;

		for(long iSE = iSTT; iSE <= iEND; iSE++)
		{
			if(iSE == iEND && nEa > 1) break;
			if(dHH[iSE] <= 0 || dHW[iSE] <= 0) continue;

			str.Format(_T("%s��ġ %d"), bUpperSlab ? _T("���") : _T("�Ϻ�"), nIdxHunch++);
			m_sArrSelfWeight[0].Add(str);
			if(nEa > 1)
				str.Format(_T("%s x %s / 2 x %s x %d EA"), 
					pData->GetStrDouble(dHH[iSE], UNIT_CGS_M), pData->GetStrDouble(dHW[iSE], UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE), nEa);
			else
				str.Format(_T("%s x %s / 2 x %s"), 
					pData->GetStrDouble(dHH[iSE], UNIT_CGS_M), pData->GetStrDouble(dHW[iSE], UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
			m_sArrSelfWeight[1].Add(str);
			dWeight = dHH[iSE] * dHW[iSE] * dUWConc * nEa / 2.;
			str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
			m_sArrSelfWeight[2].Add(str);
			m_dSelfWeight += dWeight;
		}

		// �߰�������
		for(long nJijum = 1; nJijum < pBri->m_nQtyJigan; nJijum++)
		{
			dHH[iSTT] = pBri->GetThickHunch(nJijum, TRUE, FALSE, bUpperSlab);
			dHH[iEND] = pBri->GetThickHunch(nJijum, FALSE, FALSE, bUpperSlab);
			dHW[iSTT] = pBri->GetWidthHunch(nJijum, TRUE, bUpperSlab);
			dHW[iEND] = pBri->GetWidthHunch(nJijum, FALSE, bUpperSlab);
			if(bLsd == FALSE)
			{
				dHH[iSTT] = toM(dHH[iSTT]);
				dHW[iSTT] = toM(dHW[iSTT]);
				dHH[iEND] = toM(dHH[iEND]);
				dHW[iEND] = toM(dHW[iEND]);
			}

			nEa = (Compare(dHH[iSTT], dHH[iEND], _T("=")) && Compare(dHW[iSTT], dHW[iEND], _T("="))) ? 2 : 1;

			for(long iSE = iSTT; iSE <= iEND; iSE++)
			{
				if(iSE == iEND && nEa > 1) break;
				if(dHH[iSE] <= 0 || dHW[iSE] <= 0) continue;

				str.Format(_T("%s��ġ %d"), bUpperSlab ? _T("���") : _T("�Ϻ�"), nIdxHunch++);
				m_sArrSelfWeight[0].Add(str);
				if(nEa > 1)
					str.Format(_T("%s x %s / 2 x %s x %d EA"), 
						pData->GetStrDouble(dHH[iSE], UNIT_CGS_M), pData->GetStrDouble(dHW[iSE], UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE), nEa);
				else
					str.Format(_T("%s x %s / 2 x %s"), 
						pData->GetStrDouble(dHH[iSE], UNIT_CGS_M), pData->GetStrDouble(dHW[iSE], UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
				m_sArrSelfWeight[1].Add(str);
				dWeight = dHH[iSE] * dHW[iSE] * dUWConc * nEa / 2.;
				str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
				m_sArrSelfWeight[2].Add(str);
				m_dSelfWeight += dWeight;
			}
		}
	}

	// �����
	for(long iLR = iLEFT; iLR <= iLOWER; iLR++)
	{
		BOOL bLeft = iLR == iLEFT ? TRUE : FALSE;
		if(pBri->IsBracket(bLeft))
		{
			double dW(0), dH1(0), dH2(0), dH3(0);
			if(bLsd)
			{
				dW = bLeft ? pBri->m_dBWS : pBri->m_dBWE;
				dH1 = bLeft ? pBri->m_dBHS : pBri->m_dBHE;
				dH2 = bLeft ? pBri->m_dBHHS : pBri->m_dBHHE;
			}
			else
			{
				dW = bLeft ? toM(pBri->m_dWS) : toM(pBri->m_dWE);
				dH1 = bLeft ? toM(pBri->m_dBHS) : toM(pBri->m_dBHE);
				dH2 = bLeft ? toM(pBri->m_dBHHS) : toM(pBri->m_dBHHE);
			}
			dH3 = dH1 - dH2;

			str = bLeft ? _T("���������") : _T("���������");
			m_sArrSelfWeight[0].Add(str);
			str.Format(_T("%s x (%s + %s)/2 x %s"), pData->GetStrDouble(dW, UNIT_CGS_M), pData->GetStrDouble(dH1, UNIT_CGS_M), pData->GetStrDouble(dH3, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
			m_sArrSelfWeight[1].Add(str);
			dWeight = dW * (dH1 + dH3)/2 * dUWConc;
			str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
			m_sArrSelfWeight[2].Add(str);
			m_dSelfWeight += dWeight;
		}
	}

	//  ���鸶����, �λ������
	m_dSideFriction = 0;
	m_dProtectionRiseFooting = 0;
	if(pBri->m_bExistProtectionRiseFooting)
	{
		double H1 = bLsd? pBri->m_dPRF_H1 : toM(pBri->m_dPRF_H1);
		double H2 = bLsd? pBri->m_dPRF_H2 : toM(pBri->m_dPRF_H2);
		double W  = bLsd? pBri->m_dPRF_W : toM(pBri->m_dPRF_W);
		double Ko = m_pStd->m_dCoefficientKo;
		m_sArrSelfWeight[0].Add(_T("�λ������"));
		if(H2 > 0)
		{
			str.Format(_T("(%s + %s) / 2 x %s x %s x 2 EA"), 
				pData->GetStrDouble(H1, UNIT_CGS_M), pData->GetStrDouble(H2, UNIT_CGS_M), pData->GetStrDouble(W, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
		}
		else
		{
			str.Format(_T("%s x %s x %s x 2 EA"), pData->GetStrDouble(H1, UNIT_CGS_M), pData->GetStrDouble(W, UNIT_CGS_M), pData->GetStrDouble(dUWConc, UNIT_CGS_TONF_M3, TRUE));
		}
		m_sArrSelfWeight[1].Add(str);
		dWeight = (H1 + H2) / 2 * W * dUWConc * 2;
		str = pData->GetStrDouble(dWeight, UNIT_CGS_TONF_M);
		m_sArrSelfWeight[2].Add(str);
		m_dSelfWeight += dWeight;

		//
		double UW_BackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double UW_BackFill_W = GetValueUnitChange(pBri->m_dUnitWeigthBackFillWater, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dRadFriction = ToRadian(pBri->m_dAngleFrictionBackFill);

		double D1 = dHeightBox - hs;
		double D2 = hs - H1 - H2;
		m_dSideFriction = 0;
		m_dSideFriction += Ko * UW_BackFill * pow(D1, 2) * tan(dRadFriction * 2./3.);
		m_dSideFriction += Ko * (2 * D1 * D2 * UW_BackFill + pow(D2, 2) * UW_BackFill_W) * tan(dRadFriction * 2./3.);

		//
		m_dProtectionRiseFooting = 0;
		m_dProtectionRiseFooting += (dHeightBox - hs) * W * UW_BackFill * 2;
		m_dProtectionRiseFooting += hs * W * UW_BackFill_W * 2;
		m_dProtectionRiseFooting += hs * W * dUWWater * 2;
	}

	//////////////////////////////////////////////////////////////////////////
	// �η�
	m_dBuoyance = dLenBridge * dUWWater * hs;

	if(pBri->m_bApplyGroundWaterLevel == FALSE)
	{
		double UW_BackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		m_dSideFriction = 0;
		m_dBuoyance = 0;
		if(pBri->m_bExistProtectionRiseFooting)
			m_dProtectionRiseFooting = dHeightBox * UW_BackFill * 2;
	}
}

double CARcBridgeCalcFooting::GetDistMaxPile(long nIdxOrg)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) 
		return 0.;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return 0;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return 0;

	double dDistMax = 0;

	COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
	long i = 0; for(i = 0; i < pOrg->countSide; i++)
	{
		CPileAttr *pPileAttr = pPile->GetPileAttr(nIdxOrg, i);
		dDistMax = MAX(dDistMax, pPileAttr->dist);
	}

	return dDistMax;
}


void CARcBridgeCalcFooting::WriteCalcFootingPile(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;
	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	long nRow = XLText->GetRowPostion();
	XLText->AddFormatText("$s+20$h$a3) �� ���ҿ����� �ۿ����� �� ����$n$n");
	if(bLsd)
		MakeTablePileForceAndDisp_Lsd(XLText, COL_POS_2);
	else
		MakeTablePileForceAndDisp_Usd(XLText, COL_POS_2);

	XLText->GoPosition(NO_MOVE, COL_POS_1);
	XLText->AddFormatText("$s+40$h$a4) ������ ������ ����$n$n");
	XLText->AddFormatText("$h$r%s$r������ ��������� ����$n", GetNumberString(1, NUM_CIRCLE));
	double dDispMax[2];
	// (ARCBRIDGE-1649)
//	dDispMax[0] = MAX(m_dArrDispX[0].Max(), m_dArrDispZ[0].Max());
//	dDispMax[1] = MAX(MAX(m_dArrDispX[2].Max(), m_dArrDispZ[2].Max()), MAX(m_dArrDispX[3].Max(), m_dArrDispZ[3].Max()));
	if(bLsd)
	{
		if(bPileASD)
		{
			dDispMax[0] = toM(m_dArrDispX[0].Max());
			dDispMax[1] = toM(MAX(m_dArrDispX[2].Max(), m_dArrDispX[3].Max()));
		}
		else
		{
			dDispMax[0] = m_dArrDispX[0].Max();
			dDispMax[1] = MAX(m_dArrDispX[2].Max(), m_dArrDispX[3].Max());
		}
	}
	else
	{
		dDispMax[0] = m_dArrDispX[0].Max();
		dDispMax[1] = MAX(m_dArrDispX[2].Max(), m_dArrDispX[3].Max());
	}
	XLText->OutXL();
	nRow = XLText->GetRowPostion();
	nRow = pCalcPile->WritePileOutputSafeCheckReaction(XLText->GetExcelControl(), nRow, TRUE, dDispMax, TRUE);

	XLText->GoPosition(nRow+2,COL_POS_3);
	XLText->AddFormatText("$s+20$h%s) ���Һ� ����$n", (bLsd && bPileASD==FALSE)? _T("��") : _T("��"));
	if(bLsd)
		MakeTablePileReactionCheck_Lsd(XLText, COL_POS_4);
	else
		MakeTablePileReactionCheck_Usd(XLText, COL_POS_4);

	XLText->GoPosition(NO_MOVE, COL_POS_2);
	// ��Ÿ����
	if(pPile->m_nType==PILE_TYPE_HYUNJANG)
	{
		if(bLsd)
		{
			long nIndex = 2;
			for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
			{
				if(nTypeCase == PILE_CASE_USE)
					continue;
				if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
					continue;

				CString szCase = pCalcPile->GetStringCase(nTypeCase);
				
				XLText->GoPosition(NO_MOVE, COL_POS_2);
				XLText->AddFormatText("$s+10$h%s$r������ ��ü ���� : %s$n", GetNumberString(nIndex, 0), szCase);
				WriteHyuntaPileCheckPM_Lsd(XLText, COL_POS_3, nTypeCase);
				*XLText += 1;
				++nIndex;
			}
		}
		else
		{
			XLText->AddFormatText("$s+10$h$r%s$r������ ��ü ����$n", GetNumberString(2, 0));
			WriteHyuntaPileCheckPM_Usd(XLText, COL_POS_3);
		}
	}
	else if(!bLsd || (bLsd && bPileASD))
	{
		// (ARCBRIDGE-2603) �Ѱ���� �����϶��� ��������, PHCŸ��, PHC���� ��ü����� ���ܵǾ�� �Ѵ�.
		double dEaJoint    = (pCalcPile->m_dLengthPileMax<pPile->m_dLength) ? ::RoundDown((pPile->m_dLength/pCalcPile->m_dLengthPileMax),0) : 0;
		double dRateDeduct = pCalcPile->m_dRateDeductJoint + pCalcPile->m_dRateDeductDiaLen;

		XLText->AddFormatText("$s+40$h%s$r������ ���� ����$n", GetNumberString(2, 0));
		XLText->AddFormatText("$h$r��) ������ ������ ���� ���� ��������� ������ ���� $n");
		XLText->AddFormatText("$h$r%s$n",
			pData->IsRailDesign() ? "    :�����α�������� �ؼ�(�Ϻα�����)�� P223 ����" : "    :��ö���������(ö������) P620��");
		XLText->AddFormatText("$h$r$r$r��� PILE ���� ���� : %.2f m / ��$n", toM(pCalcPile->m_dLengthPileMax));
		XLText->AddFormatText("$h$r$rPILE ���� : %.2f m$n", toM(pPile->m_dLength));
		XLText->AddFormatText("$h$r$r�������Ҽ� : %.0f ����$n", dEaJoint);
		XLText->AddFormatText("$h$r$r���������� ���� ������� ������ : %.2f%% $n", pCalcPile->m_dRateDeductJoint*100);


		double dLofD = pCalcPile->m_dPileLength/pPile->m_dDia;
		if(dLofD<=pCalcPile->m_dLimitsDiaLen)
		{
			XLText->AddFormatText("$h$r$r$r���� (L/D) = %.2f/%.3f = %.3f < %.0f �̹Ƿ� ���� ���� ���� ����.$n",
				toM(pCalcPile->m_dPileLength),toM(pPile->m_dDia), dLofD, pCalcPile->m_dLimitsDiaLen);
			XLText->AddFormatText("$h$r$r$r���� ��������� �� �������� %.2f%% ����$n", dRateDeduct*100);
		}
		else if(dLofD>pCalcPile->m_dLimitsDiaLen)
		{
			XLText->AddFormatText("$h$r$r$r���� (L/D) = %.2f/%.3f = %.3f > %.0f �̹Ƿ� ���� ���� ������� ������ ����.$n",
				toM(pCalcPile->m_dPileLength),toM(pPile->m_dDia), dLofD, pCalcPile->m_dLimitsDiaLen);
			XLText->AddFormatText("$h$r$r$r������ ��(%%) = (L/D) - n = %.3f - %.0f = %.3f%%$n",
				dLofD, toM(pCalcPile->m_dLimitsDiaLen), pCalcPile->m_dRateDeductDiaLen*100);
			XLText->AddFormatText("$h$r$r$r���� ��������� �� �������� %.2f%% ����$n", dRateDeduct*100);
		}
		else
		{
			XLText->AddFormatText("$h$r$r$r���� ��������� ���Ҵ� ����.$n");
		}

		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText("$s+20$n$h$r��) ������ ������ ����$n");
		if(pPile->IsPHC())
		{
			if(bLsd)
				MakeTablePileStressCheckPHC_Lsd(XLText, COL_POS_4);
			else
				MakeTablePileStressCheckPHC_Usd(XLText, COL_POS_4);
		}
		else
		{
			if(bLsd)
				MakeTablePileStressCheck_Lsd(XLText, COL_POS_4);
			else
				MakeTablePileStressCheck_Usd(XLText, COL_POS_4);
		}

		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText("$s+20$h$r��) ������ �������� ����$n");
		if(bLsd)
			MakeTablePileShearStressCheck_Lsd(XLText, COL_POS_4);
		else
			MakeTablePileShearStressCheck_Usd(XLText, COL_POS_4);
	}
	else if(bLsd)
	{
		pCalcPile->SetIndexTitleSub(4);
		// ����Ⱦ���� ������
		XLText->OutXL();
		nRow = XLText->GetRowPostion();
		nRow = pCalcPile->WriteLsdPileOutputSafeCheckTransverse(XLText->GetExcelControl(), nRow, TRUE);
		XLText->GoPosition(nRow,COL_POS_3);
		
		// ������ ħ�Ϸ� ����
		XLText->OutXL();
		nRow = XLText->GetRowPostion();
		nRow = pCalcPile->WriteLsdPileOutputSafeCheckSettlement(XLText->GetExcelControl(), nRow, TRUE);
		XLText->GoPosition(nRow,COL_POS_3);

		if(pPile->m_nType==PILE_TYPE_STEEL || pPile->m_nType==PILE_TYPE_STEEL_DIGGING)
		{
			// �������� �Ѱ���� ��ü ����
			WritePileCheckMain_Lsd(XLText, COL_POS_2);
		}
	}
	XLText->OutXL();
}

void CARcBridgeCalcFooting::MakeTablePileForceAndDisp_Usd(CXLFormatText *pXLText, long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 4 : 3) : 2; // ���,���,�ر�,���

	//������ ���� ����
	//	double dDistPile = GetPileCtcSideUnitMeter();
	char *szTitle[] = { "�� ��", "Vi","Hi","Mi","��x(m)","��z(m)","��(rad)", "���"};
	long nColWidth[] = { 2, 3, 3, 3, 3, 3, 3, 3 };

	long nColTotal=0;
	long n = 0;
	for(n=0; n<8; n++) nColTotal += nColWidth[n];

	long i = 0, j = 0;
	for(i=0; i < nCase; i++) // �������(���,������), �������
	{
		pXLText->GoPosition(NO_MOVE,nPos);
		if(i==0)	
			pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(i+1, NUM_CIRCLE), _T("�������"));
		else if(i==1)	
			pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(i+1, NUM_CIRCLE), _T("�������"));
		else	
		{
			if(pBri->m_nSelectModelingCountEQ == 2 && nCase == 2)
				pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(i+1, NUM_CIRCLE), _T("�ر�����"));
			else if(pBri->m_nSelectModelingCountEQ == 2 && nCase == 3)
				pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(i+1, NUM_CIRCLE), _T("��ɼ���"));
			else
				pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(i+1, NUM_CIRCLE), _T("��������"));
		}

		pXLText->GoPosition(NO_MOVE,nPos);
		pXLText->AddTable(1 + nPileSu - 1, nColTotal-1, 1 ,2);

		// Title	
		for(n=0; n<8; n++)	
		{
			pXLText->AddFormatText("$me%d$c%s$m+%02d",nColWidth[n]-1,szTitle[n],nColWidth[n]);
		}
		pXLText->AddText("$n$h");		

		for(j=0; j<nPileSu; j++) 
		{
			CPileLoad *pLoad = NULL;
			if(i == 0)
				pLoad = pCalcPile->m_pListUse.GetAt(j);
			else if(i == 1) 
				pLoad = pCalcPile->m_pListUlt.GetAt(j);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * (i - 1) + j);

			double dHi = GetValueUnitChange(pLoad->m_dForceHori, UNIT_CGS_TONF, pData->m_szTonf);
			double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf);
			double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM);
			double dDispX = m_dArrDispX[i].GetAt(j);
			double dDispZ = m_dArrDispZ[i].GetAt(j);
			double dAlpha = m_dArrDispR[i].GetAt(j);

			pXLText->AddFormatText("$h$me1$c%d$m+%02d", j + 1, nColWidth[0]);
			pXLText->AddFormatText("$me2$c$f3%.2f$m+%02d", dVi, nColWidth[1]);
			pXLText->AddFormatText("$me2$c$f3%.2f$m+%02d", dHi, nColWidth[2]);
			pXLText->AddFormatText("$me2$c$f3%.2f$m+%02d", dMi, nColWidth[3]);
			pXLText->AddFormatText("$me2$c$f6%.6f$m+%02d", dDispX, nColWidth[4]);
			pXLText->AddFormatText("$me2$c$f6%.6f$m+%02d", dDispZ, nColWidth[5]);
			pXLText->AddFormatText("$me2$c$f6%.6f$m+%02d", dAlpha, nColWidth[6]);
			pXLText->AddFormatText("$me2$c%s$n", pLoad->m_sName); // ����
		}
		pXLText->AddText("$n");	
	}

	pXLText->AddText("$n");	
}

void CARcBridgeCalcFooting::MakeTablePileReactionCheck_Usd(CXLFormatText *pXLText,long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	double dDistPile = 0;
	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

	long nT = 0, n = 0;
	for(nT = 0; nT < nCase; nT++)
	{
		pXLText->GoPosition(NO_MOVE,nPos);
		BOOL bEarthQuake = (nT==0) ? FALSE : TRUE;
		if(nCase>1 && !bEarthQuake) pXLText->AddText("�� ���$n");
		if(nCase>1 && bEarthQuake) 
		{
			if(nCase == 3)
			{
				pXLText->AddText(nT == 1 ? "�� ������ : �ر�����$n" : "�� ������ : ��ɼ���$n");
			}
			else
			{
				pXLText->AddText("�� ������$n");
			}
		}

		pXLText->AddTable(nPileSu, 20, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(max)"));
		szTitle.Add(_T("V(min)"));
		szTitle.Add(_T("Ra(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("Pa(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		for(n=0; n<7; n++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(n));	
		pXLText->AddText("$n");
		for(n=0; n<nPileSu; n++)
		{
			CPileLoad *pLoad = NULL;
			if(nT == 0)
				pLoad = pCalcPile->m_pListUse.GetAt(n);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);

			dDistPile = toM(GetDistMaxPile(n));
			double dVmax = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dVmin = 0;

			double dRa = GetValueUnitChange(bEarthQuake ? pCalcPile->m_Ra[1] : pCalcPile->m_Ra[0],UNIT_CGS_TONF,pData->m_szTonf);
			double dPa = GetValueUnitChange(bEarthQuake ? pCalcPile->m_Pa[1] : pCalcPile->m_Pa[0],UNIT_CGS_TONF,pData->m_szTonf);

			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", n+1);	
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dVmax);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dVmin);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dRa);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dPa);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dDistPile);
			BOOL bOK = (dVmax > dRa || (dVmin<0 && fabs(dVmin)>fabs(dPa)))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::WriteHyuntaPileCheckPM_Usd(CXLFormatText *pXLText,long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h��) ���Һ� �ۿ�����$n");
	//////////////////////////////////////////////////////////////////////////
	CLongArray nArrJointID;
	char *szTitle[] = { "�� ��", "P", "M", "e", "C.T.C", "���"};
	long nColWidth[] = { 4, 3, 3, 3, 3, 3 };
	long nColTotal=0;
	long n = 0;
	for(n=0; n<6; n++) nColTotal += nColWidth[n];

	pXLText->GoPosition(NO_MOVE, nPos+1);
	pXLText->AddTable(1+nPileSu*nCase-1,nColTotal-1,1,4);

	// Title	
	for(n=0; n<5; n++)	pXLText->AddFormatText("$me%d$c%s$m+%02d",nColWidth[n]-1,szTitle[n],nColWidth[n]);
	pXLText->AddText("$n");		
	CString str = _T("");
	long i = 0, j = 0;
	double dCaseMaxP[2] = {0,0}, dCaseMaxM[2] = {0,0}, dCaseMaxE[2] = {0,0};
	for(i=0; i<nCase; i++) // ���, ������
	{		
		pXLText->GoPosition(NO_MOVE, nPos+1);
		if(i==0)		
		{
			pXLText->AddMergeText(nPileSu-1,2,"$h$c$j��  ��$m+04");
		}
		else 
		{
			if(nCase == 2)
				pXLText->AddMergeText(nPileSu-1,2,"$h$c$j������$m+04");	
			else
			{
				str.Format(_T("$h$c$j%s$m+04"), i == 1 ? "�ر�����" : "��ɼ���");
				pXLText->AddMergeText(nPileSu-1,2,str);	
			}
		}
		for(j=0; j<nPileSu; j++) 
		{
			CPileLoad *pLoad = NULL;
			if(i == 0)
				pLoad = pCalcPile->m_pListUlt.GetAt(j);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * i + j);

			double dDistPile = toM(GetDistMaxPile(j));
			double dVi = pLoad->m_dForceVert * dDistPile;
			double dMi = pLoad->m_dMoment * dDistPile;
			double dE = dMi/dVi;

			if(dCaseMaxP[0] < dVi) // ����ִ�
			{
				dCaseMaxP[0] = dVi;
				dCaseMaxP[1] = dMi;
			}
			if(dCaseMaxM[0] < dMi) // ���Ʈ�ִ�
			{
				dCaseMaxM[0] = dVi;
				dCaseMaxM[1] = dMi;
			}
			if(dCaseMaxE[1]/dCaseMaxE[1] < dE) // ����ִ�
			{
				dCaseMaxE[0] = dVi;
				dCaseMaxE[1] = dMi;
			}

			pXLText->GoPosition(NO_MOVE, nPos+3);
			pXLText->AddFormatText("$h$r$c%d$r", j + 1);
			pXLText->AddFormatText("$me2$c$f3%.2f$m+%02d", dVi, nColWidth[1]);
			pXLText->AddFormatText("$me2$c$f3%.2f$m+%02d", dMi, nColWidth[2]);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+%02d", dE, nColWidth[3]);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+%02d", dDistPile, nColWidth[4]);
			pXLText->AddFormatText("$me2$c $n");			
		}
	}
	pXLText->AddText("$n");
	//////////////////////////////////////////////////////////////////////////
	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$s+40$h��) ���� �ܸ����$n");

	CConcBasic *pConc = new CConcBasic;
	pConc->m_Fck = pPile->m_pHyunTa->m_dHP_Fck;
	pConc->m_Fy  = pPile->m_pHyunTa->m_dHP_Fy;
	pConc->m_nTypeUnit = pData->m_nTypeUnit;
	pConc->m_nTypeSection = SECTION_COLUMN;
	pConc->m_Ro = pPile->m_dDia/2;

	for(i=0; i<pPile->m_pHyunTa->m_nHP_MainRebarDanSu; i++)
	{
		pConc->m_RbT1C_dc[i] = pPile->m_pHyunTa->m_dHP_MainRebarDist[i];
		pConc->m_RbT1C_DIA[i]= pPile->m_pHyunTa->m_dHP_MainRebarDia[i];
		pConc->m_RbT1C_DIA[i]= pPile->GetDiaRebarMain(1000,i); // �ִ���Ʈ��ġ....
		pConc->m_RbT1C_RE[i] = pPile->m_pHyunTa->m_dHP_MainRebarEa[i];
	}
	pConc->m_sArrPM.RemoveAll();

	// �����׸�
	CColumnForce *pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxP[0]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxP[1]), pData->m_szTonf, UNIT_CGS_TONF);
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("����ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));
	//
	pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxM[0]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxM[1]), pData->m_szTonf, UNIT_CGS_TONF);
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("���Ʈ�ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));
	//
	pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxE[0]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxE[1]), pData->m_szTonf, UNIT_CGS_TONF);
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("����ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));

	CString szTilte;
	for(i=0; i<pConc->m_pArrForce.GetSize(); i++)
	{
		pXLText->AddFormatText("$s+40");
		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h%s %s$n$n", GetNumberString(i+1, 5), pConc->m_sArrPM.GetAt(i));
		CColumn pColumn(pConc, FALSE, pCalcPile->m_nSeclectPmin, i);				
		pColumn.MakeOutputExcel(pXLText, nPos+1, FALSE);
	}

	CColumn pColumn(pConc, FALSE, pCalcPile->m_nSeclectPmin, -1);	
	pColumn.CalculateAll(TRUE);
	// P-M�����
	double dScaleDim = 150;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim*3);

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h��) PM �����$n");
	CDrawPM DrawPm(m_pStd->m_pARoadOptionStd, FALSE);
	DrawPm.DrawPM(pDom, &pColumn, FALSE);

	long nSizeRow = 29;
	pXLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow, CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcFooting::MakeTablePileStressCheckPHC_Usd(CXLFormatText *pXLText,long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

	double Ae  = frCm2(pPile->m_dAe_PHC);
	double Ze  = frCm3(pPile->m_dZe_PHC);
	double fce = GetValueUnitChange(pPile->m_dFce2_PHC, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

	double dFca[2];
	double dFta[2];
	dFca[0] = GetValueUnitChange(pCalcPile->m_PHC_Fca[0], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	dFca[1] = GetValueUnitChange(pCalcPile->m_PHC_Fca[1], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	dFta[0] = GetValueUnitChange(pCalcPile->m_PHC_Fta[0], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	dFta[1] = GetValueUnitChange(pCalcPile->m_PHC_Fta[1], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

	if(pPile->m_nType == PILE_TYPE_PHC)
	{
		pXLText->GoPosition(NO_MOVE, nPos-1);
		pXLText->AddText(_T("����Ÿ���� ��Ÿ�� ���� ���ҸӸ� �ļ��̳� ���������� �ұյ� ������ ���Ͽ� �ε��� ���ҸӸ���$n"));
		pXLText->AddText(_T("�����ؾ��ϴ� ��찡 �ִ�. �̶� PC������ ��쿡�� ���ܵ� ��ġ�κ��� ������������$n"));
		pXLText->AddText(_T("�̹� ���Ե� ������Ʈ������ �����ϰ� �ȴ�.    ����������. �ѱ����ݰ���ȸ �� �� 7.5.2.2$n"));
	}

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h��� �ھ������� fca$m+06$c=$r%.3f %s (����)$n", dFca[0], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$m+06$c=$r%.3f %s (������)$n", dFca[1], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h��� ���������� fta$m+06$c=$r%.3f %s (����)$n", dFta[0], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$m+06$c=$r%.3f %s (������)$n", dFta[1], pData->m_szKgf_CM2);

	pXLText->AddFormatText("$h$cf$r$c=$rfce �� M/Ze + V/Ae$n");
	pXLText->AddFormatText("$h$r$r$c=$r%.3f �� M / %.1f + V / %.1f$n", fce, Ze, Ae);

	// Table ���
	CString str = _T("");
	long nT = 0, n = 0;
	for(nT=0; nT<nCase; nT++)
	{
		pXLText->GoPosition(NO_MOVE, nPos);
		BOOL bEarthQuake = (nT==0) ? FALSE : TRUE;
		if(nCase>1 && !bEarthQuake) pXLText->AddText("�� ���$n");
		if(nCase>1 && bEarthQuake) 
		{
			if(nCase == 3)
			{
				pXLText->AddText(nT == 1 ? "�� ������ : �ر�����$n" : "�� ������ : ��ɼ���$n");
			}
			else
			{
				pXLText->AddText("�� ������$n");
			}
		}

		pXLText->AddFormatText("$h");	
		pXLText->AddTable(nPileSu, 23, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("M(" + pData->m_szTonfM + ")"));
		szTitle.Add(_T("fc"));
		szTitle.Add(_T("fca"));
		szTitle.Add(_T("ft"));
		szTitle.Add(_T("fta"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		long dWidth[9] = { 2, 3, 3, 3, 3, 3, 3, 2, 2 };

		for(n=0; n<9; n++)	
			pXLText->AddFormatText("$me%d$c%s$m+0%d", dWidth[n]-1,szTitle.GetAt(n), dWidth[n]);	
		pXLText->AddText("$n");
		for(n=0; n<nPileSu; n++)
		{
			CPileLoad *pLoad = NULL;
			if(nT == 0)
				pLoad = pCalcPile->m_pListUse.GetAt(n);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);

			double dDistPile = toM(GetDistMaxPile(n));
			double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM) * dDistPile;

			double ft   = fce - dMi*1000000/Ze + dVi*1000/Ae;
			double fc   = fce + dMi*1000000/Ze + dVi*1000/Ae;
			double fca  = bEarthQuake? dFca[1] : dFca[0];
			double fta  = bEarthQuake? dFta[1] : dFta[0];

			pXLText->AddFormatText("$h$me1$c$f0%d$m+02", n+1);	
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dVi);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dMi);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", fc);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", fca);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", ft);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", fta);
			pXLText->AddFormatText("$me1$c$f3%.3f$m+02", dDistPile);
			BOOL bOK = (fabs(fc) > fabs(fca) || (ft<0 && ft<fta))? FALSE : TRUE;
			pXLText->AddFormatText("$me1$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::MakeTablePileStressCheck_Usd(CXLFormatText *pXLText,long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

	double dDia = toCm(pPile->m_dDia);
	double I33	= toM4(frCm4(pCalcPile->m_I33));
	double yIp  = toM(pPile->m_dDia)/2.0/I33;
	double Ap   = toM2(frCm2(pCalcPile->m_Ap));

	double dFsa[2];
	dFsa[0] = pCalcPile->GetFsa(FALSE);
	dFsa[1] = pCalcPile->GetFsa(TRUE);

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h$cfsa$r$c=$r%.3f %s (����)$n", dFsa[0], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$cfsa$r$c=$r%.3f %s (������)$n", dFsa[1], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$cf$r$c=$rV/Ap �� M x y/Io$n");

	if(pData->IsSI()) 
		pXLText->AddFormatText("$h$r$c=$rV/%.3f �� M x %.3f/2 / %.5f$n", Ap, toM(frCm(dDia)), I33);
	else 
		pXLText->AddFormatText("$h$r$c=$rV/%.3f �� M x %.1f/2 / %.1f$n", Ap, dDia, I33);

	// Table ���
	long nT = 0, n = 0;
	for(nT=0; nT<nCase; nT++)
	{
		pXLText->GoPosition(NO_MOVE, nPos);
		BOOL bEarthQuake = (nT==0) ? FALSE : TRUE;
		if(nCase>1 && !bEarthQuake) pXLText->AddText("�� ���$n");
		if(nCase>1 && bEarthQuake) 
		{
			if(nCase == 3)
			{
				pXLText->AddText(nT == 1 ? "�� ������ : �ر�����$n" : "�� ������ : ��ɼ���$n");
			}
			else
			{
				pXLText->AddText("�� ������$n");
			}
		}

		pXLText->AddTable(nPileSu, 20, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("M(" + pData->m_szTonfM + ")"));
		szTitle.Add(_T("fmax"));
		szTitle.Add(_T("fsa"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));


		for(n=0; n<7; n++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(n));	
		pXLText->AddText("$n");
		for(n=0; n<nPileSu; n++)
		{
			CPileLoad *pLoad = NULL;
			if(nT == 0)
				pLoad = pCalcPile->m_pListUse.GetAt(n);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);

			double dDistPile = toM(GetDistMaxPile(n));
			double dVi = pLoad->m_dForceVert * dDistPile;
			double dMi = pLoad->m_dMoment * dDistPile;

			double fmax = GetValueUnitChange((dVi/Ap + dMi*yIp)/10,UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			double fsa  = bEarthQuake? dFsa[1] : dFsa[0];

			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", n+1);	
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", GetValueUnitChange(dVi,UNIT_CGS_TONF,pData->m_szTonf));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", GetValueUnitChange(dMi,UNIT_CGS_TONF,pData->m_szTonf));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", fmax);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", fsa);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dDistPile);
			BOOL bOK = (fabs(fmax) > fabs(fsa))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::MakeTablePileShearStressCheck_Usd(CXLFormatText *pXLText,long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

	double D		= pPile->m_dDia;
	double T1	= pPile->m_dT1;
	double T2	= pPile->m_dT2;
	double D1	= D-T2*2;
	double d		= D-T1*2;
	double Ap	= pCalcPile->m_Ap;
	double alpha	= 4*(D1*D1+D1*d+d*d) / (3*(D1*D1+d*d));

	double dTsa[2];
	dTsa[0] = pCalcPile->GetTsa(FALSE);
	dTsa[1] = pCalcPile->GetTsa(TRUE);

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h$me1��sa$m+02$c=$r%.3f %s (����)$n", dTsa[0], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$me1��sa$m+02$c=$r%.3f %s (������)$n", dTsa[1], pData->m_szKgf_CM2);
	pXLText->AddFormatText("$h$me1$c��$m+02$c=$r�� x Ho / Ap$n");
	pXLText->AddFormatText("$h$me1$c��$m+02$c=$r4 x (D��+ D��d + d��) / (3 x (D��+ d��))$n");
	if(pData->IsSI())
		pXLText->AddFormatText("$h$m+02$c=$r4 x (%.1f��+ %.1f��%.1f + %.1f��) / (3 x (%.1f��+ %.1f��))$n", D1, D1, d, d, D1, d);
	else 
		pXLText->AddFormatText("$h$m+02$c=$r4 x (%.3f��+ %.3f��%.3f + %.3f��) / (3 x (%.3f��+ %.3f��))$n", D1, D1, d, d, D1, d);
	pXLText->AddFormatText("$h$m+02$c=$r%.3f : ����������$n", alpha);

	long nT = 0, n = 0;
	for(nT=0; nT<nCase; nT++)
	{
		pXLText->GoPosition(NO_MOVE, nPos);
		BOOL bEarthQuake = (nT==0) ? FALSE : TRUE;
		if(nCase>1 && !bEarthQuake) pXLText->AddText("�� ���$n");
		if(nCase>1 && bEarthQuake) 
		{
			if(nCase == 3)
			{
				pXLText->AddText(nT == 1 ? "�� ������ : �ر�����$n" : "�� ������ : ��ɼ���$n");
			}
			else
			{
				pXLText->AddText("�� ������$n");
			}
		}

		pXLText->AddTable(nPileSu, 17, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("H(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("��"));
		szTitle.Add(_T("��sa"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		for(n=0; n<6; n++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(n));	
		pXLText->AddText("$n");
		for(n=0; n<nPileSu; n++)
		{
			CPileLoad *pLoad = NULL;
			if(nT == 0)
				pLoad = pCalcPile->m_pListUse.GetAt(n);
			else
				pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);

			double dDistPile = toM(GetDistMaxPile(n));
			double Ho = pLoad->m_dForceHori * dDistPile;
			double Tmax = GetValueUnitChange((alpha * frTon(Ho)) / Ap, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);	// tonf/m2 -> kgf/cm2
			double Tsa	= bEarthQuake? dTsa[1] : dTsa[0];

			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", n+1);	
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", GetValueUnitChange(Ho,UNIT_CGS_TONF, pData->m_szTonf));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", Tmax);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", Tsa);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dDistPile);
			BOOL bOK = (fabs(Tmax) > fabs(Tsa))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::CheckFootingSafeLSD()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	HGDOUBLE dDivMeter = frM(UNIT_METER);
	HGDOUBLE dVert = 0, dHori = 0, dMo = 0, dMr = 0;
	ClearVectorPtr(m_pListLsdFootingSafe);

	long nCntJijum = pBri->GetCountJijum();
	for(long nJ=0; nJ<nCntJijum; ++nJ)
	{
		CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
		if(pFooting == NULL || pFooting->m_bIs == FALSE) continue;

		if((nJ==0 && !pBri->IsOutWall(TRUE)) || (nJ==nCntJijum-1 && !pBri->IsOutWall(FALSE))) continue;

		CLsdFootingDesign *pLsdFooting = new CLsdFootingDesign;
		m_pListLsdFootingSafe.push_back(pLsdFooting);

		pLsdFooting->m_eDesignType = pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eDesignDoro2019 : eDesignDoro2015;

		pLsdFooting->m_dL_Load = Round(pBri->GetWidthSlabActByGagak(pBri->GetStationOnJijum(nJ), pBri->GetAngleJijum(nJ)), 0);

		for(long ixEnv=0; ixEnv<eCountEnvTypeLsd; ++ixEnv)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnv;
			CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			// �������信�� ��뼺 ����� ������ ���� �ʴ´�.
			if(eEnvType == eEnvService)
				continue;

			ELSDLoadGroupKind eKind;
			switch(eEnvType)
			{
			case eEnvLimit:
				eKind = eLSD_COMBINATION_LIMIT;	break;
			case eEnvExtremeI:
			case eEnvExtremeII:
				eKind = eLSD_COMBINATION_EXTREME;	break;
			case eEnvService_Crack:
				eKind = eLSD_COMBINATION_SERVICE;	break;
			default:
				break;
			}

			SetLsdFooting(nJ);
			for(long ixCheck=0; ixCheck<eCountFootCheck; ++ixCheck)
			{
				CLoadFooting *pLoad =  GetLoadFootingLsd(nJ, eEnvType, ixCheck);
				if(pLoad == NULL) continue;

				dVert = pLoad->m_dVerTotal;
				dHori = pLoad->m_dHorTotal;
				dMo = pLoad->m_dMoTotal;
				dMr = pLoad->m_dMrTotal;

				pLsdFooting->AddLoad(eKind, pLoad->m_sCombo, dVert, dHori, dMo, dMr);
			}
		}
		pLsdFooting->CalculateAll();
	}
}

void CARcBridgeCalcFooting::SetLsdFooting(long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;
	CFootingApp *	pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if (pFooting == NULL)
		return;

	CLsdFootingDesign *pLsdFootDesign = GetLsdFootingDesign(nJijum);
	if(pLsdFootDesign == NULL) return;

	pLsdFootDesign->m_ixTypeGround = pFooting->m_nTypeBaseGround;
	pLsdFootDesign->m_dRatePoisson = pFooting->m_dBoringRatePoasong;
	pLsdFootDesign->m_dAngleFriction = pFooting->GetFrictionAngleBaseGround(pFooting->m_dNValueBaseGround);
	pLsdFootDesign->m_dDf = frM(pFooting->m_dDf);
	pLsdFootDesign->sCoffSF.Eo = GetValueUnitChange(pFooting->m_dBoringEo, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

	// ============== AddLoad �Ҷ� ���� ������ ����ϱ� ������ CalculateAll() �ؼ� �־� ��� �Ѵ�.
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile)
		pCalcPile->CalculateAll();

	pLsdFootDesign->m_bUnitLoad = TRUE;
	pLsdFootDesign->SetLsdManager(pData->m_plsdEnv);
	pLsdFootDesign->m_bDirBridge = TRUE;
	pLsdFootDesign->SetFootingData(static_cast<CFootingData*>(pFooting), pCalcPile, FALSE, !pBri->IsBoxType()); // Mr�� ���ʿ��ܿ��� ���ϹǷ� �߾������� �ƴ�...

	pLsdFootDesign->SetNValueBaseGround(pFooting->m_dNValueBaseGround);

	BOOL bEnd = (nJijum == pBri->m_nQtyJigan)? TRUE : FALSE;
	CTwinVector tvBase;
	pFooting->GetTvFront();
	pFooting->m_tvArrFront.GetTvByInfo(bEnd? NOTRANS("����������") : NOTRANS("���ʿ�����"), tvBase);

	CDPoint xyBase = tvBase.GetXyBottom();

	double A	 = pBri->GetCoefficientAccelerateEQ();	// ���ӵ����
	double S	 = pBri->GetCoefficientGround();			// ���ݰ��
	double Kh	 = A*S;

	pLsdFootDesign->m_bUseMassWeightSupportCheck = pBri->m_bApplyMassCalcSafe;
	pLsdFootDesign->sMassConc.SetCenMass(xyBase);
	pLsdFootDesign->sMassConc.SetKh(Kh);
}

void CARcBridgeCalcFooting::MakeTableLoadForFooting_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bAdd, BOOL bService1, BOOL bService2)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString szUnit_Moment = pData->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pData->GetStringUnitType(UNIT_CGS_TONF);
	EPartOfBridge ePart = (nJijum == 0)? ePartFootStt : ((nJijum == pBri->m_nQtyJigan)? ePartFootEnd : ePartInFoot);

	int nWidthJoint2[] = { 5, 4, 4, 4, 8 };
	CString strTemp2 = "�����("+szUnit_Force+")";
	CString strTemp  = "������("+szUnit_Force+")";
	CString strTemp1 = "���Ʈ("+szUnit_Moment+")";
	char *Title2[] = { "�� ��", strTemp.GetBuffer(strTemp.GetLength()), strTemp1.GetBuffer(strTemp1.GetLength()), strTemp2.GetBuffer(strTemp2.GetLength()), "�� ��" };

	long nCountEnvType(0);
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		if(eEnvType == eEnvService && !bService1)
			continue;
		if(eEnvType == eEnvService_Crack && !bService2)
			continue;

		++nCountEnvType;
	}

	// table�� ��ü row��

	long nCntRow = nCountEnvType * 5 + 1;
	XLText->AddTable(nCntRow-1, 25-1, 1, 4);

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
	
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		BOOL bService = (ix==eEnvService || ix==eEnvService_Crack);
		ETypeEnvComboLsd eEnvType = static_cast<ETypeEnvComboLsd>(ix);

		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;
		
		if(ix == eEnvService && bService1 == FALSE) continue;
		if(ix == eEnvService_Crack && bService2 == FALSE) continue;

		ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);

		for(long iCheck=0; iCheck<eCountFootCheck; ++iCheck)
		{
			CLoadFooting *pLoadFooting = GetLoadFootingLsd(nJijum, eEnvType, iCheck);
			if (pLoadFooting == NULL)
			{
				// ���� ����� �Ǵϱ� ��ĭ�� ����ְ� �ѱ���.
				for(j=0; j<5; j++)
				{
					XLText->AddMergeText(0, nWidthJoint2[j]-1, "$c$j-");
					XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
				}
				XLText->AddText("$n$h");
				continue;
			}
			
			CString szCombo = pLoadFooting->m_sCombo;

			Axial = bAdd ? pLoadFooting->m_dUpperLoadV_Add : pLoadFooting->m_dUpperLoadV;
			Shear =  bAdd ? pLoadFooting->m_dUpperLoadH_Add : pLoadFooting->m_dUpperLoadH;
			Moment = bAdd ? pLoadFooting->m_dUpperLoadM_Add : pLoadFooting->m_dUpperLoadM;

			for(j=0; j<5; j++)
			{
				if(j == 0)
				{
					str = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
					if(bService)
					{
						str += (eEnvType == eEnvService_Crack)?  _T("\n(��������)") : _T("\n(��뼺����)");
					}
				}
				if(j == 1) str.Format("$f3%.3f",Axial);
				if(j == 2) str.Format("$f3%.3f",Moment);
				if(j == 3) str.Format("$f3%.3f",Shear);
				if(j == 4) 
				{
					str = pBri->GetStringFootingCheckCase((EFootingCheckLsd)iCheck) + _T(" (") + szCombo + _T(")");
				}
				if(j==0 && (iCheck == 0)) 
					strOpt = " $md4";
				else 
					strOpt = "";
				XLText->AddMergeFormatText(0, nWidthJoint2[j]-1, "$c$j%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
			}
			XLText->AddText("$n$h");
		}
	}
	
	*XLText += 1;
}

void CARcBridgeCalcFooting::MakeTableLoadCombo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, BOOL bSafeCheck)
{
	CLsdManager *pLsdManager = pBri->m_plsdManager;

	const EPartOfBridge ePart = (nJijum == 0)? ePartFootStt : ((nJijum == pBri->m_nQtyJigan)? ePartFootEnd : ePartInFoot);
	const ELSDLoadCaseDir eDir = eLSD_LOADCASE_DIR_BRI;
	
	CString strLoad;
	std::vector<ELSDLoadCaseKind> vecLoadCaseKind;
	for(long ix=0; ix<eLSD_LOADCASE_SIZE; ++ix)
	{
		ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(ix);
		HGBOOL bAdd = FALSE;

		for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			if(bSafeCheck && eEnvType == eEnvService) continue;
			
			ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);
			for(long iCheck=0; iCheck<eCountFootCheck; ++iCheck)
			{
				CLoadFooting *pLoadFooting = GetLoadFootingLsd(nJijum, eEnvType, iCheck);
				if (pLoadFooting == NULL)
					continue;

				long nCombo = pLoadFooting->m_nCombo;
				if(IsUsedLoadCase(pLsdManager, eKindCombo, eDir, eCase, nCombo))
				{
					vecLoadCaseKind.push_back(eCase);
					
					bAdd = TRUE;
					break;
				}
			}

			if (bAdd)
			{
				break;
			}
		}
	}

	const HGINT32 icCol = 11;
	HGINT32 icDiv = vecLoadCaseKind.size() / icCol;
	for (HGINT32 ix = 0; ix < icDiv; ++ix)
	{
		std::vector<ELSDLoadCaseKind> vecTemp(vecLoadCaseKind.begin() + ix * icCol, vecLoadCaseKind.begin() + (ix+1) * icCol);
		MakeTableLoadCombo(XLText, pBri, nJijum, vecTemp, bSafeCheck);
	}

	HGINT32 icMod = vecLoadCaseKind.size() % icCol;
	if (icMod != 0)
	{
		std::vector<ELSDLoadCaseKind> vecTemp(vecLoadCaseKind.begin() + icDiv * icCol, vecLoadCaseKind.begin() + icDiv * icCol + icMod);
		MakeTableLoadCombo(XLText, pBri, nJijum, vecTemp, bSafeCheck);
	}

	XLText->AddText("$n$h");
}

void CARcBridgeCalcFooting::MakeTableLoadCombo(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, const std::vector<ELSDLoadCaseKind> &vecLoadCaseKind, BOOL bSafeCheck)
{
	CLsdManagerBridge *pLsdManager = pBri->m_plsdManager;
	const EPartOfBridge ePart = (nJijum == 0)? ePartFootStt : ((nJijum == pBri->m_nQtyJigan)? ePartFootEnd : ePartInFoot);
	const ELSDLoadCaseDir eDir = eLSD_LOADCASE_DIR_BRI;
	const long nWidCol = 2;

	const long nCntCol = vecLoadCaseKind.size() + 1;
	const long nCntRow = GetCountLoadFooting(pBri, bSafeCheck);
	
	XLText->AddTable(nCntRow, nCntCol*nWidCol - 1, 1, 1);

	CStringArray szArrTitle;
	szArrTitle.Add("����");
	for (auto iter = vecLoadCaseKind.begin(); iter != vecLoadCaseKind.end(); ++iter)
	{
		szArrTitle.Add(pLsdManager->GetDesignNameByIndex(*iter));
	}

	long nCntMerge = 0;
	long j=0; for(j=0; j<nCntCol; j++)
	{
		XLText->AddMergeFormatText(nCntMerge,nWidCol-1,_T("$c%s"), szArrTitle.GetAt(j));
		XLText->AddFormatText(_T("$m+0%d"), nWidCol);
	}
	XLText->AddText(_T("$n$h"));

	CString str;
	double dFactor = 0;
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;
		if(bSafeCheck && eEnvType == eEnvService) continue;
		
		ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);

		for(long iCheck=0; iCheck<eCountFootCheck; ++iCheck)
		{
			CLoadFooting *pLoadFooting = GetLoadFootingLsd(nJijum, eEnvType, iCheck);
			if (pLoadFooting == NULL)
			{
				// ���� ����� �Ǵϱ� ��ĭ�� ����ְ� �ѱ���.
				for(j=0; j<=(long)vecLoadCaseKind.size(); j++)
				{
					XLText->AddMergeText(0, nWidCol-1, "$c$j-");
					XLText->AddFormatText("$m+0%d",nWidCol);
				}
				XLText->AddText("$n$h");
				continue;
			}

			CString szCombo = pLoadFooting->m_sCombo;

			str = szCombo;

			XLText->AddMergeFormatText(0, nWidCol-1,_T("$c%s"), str);
			XLText->AddFormatText(_T("$m+0%d"), nWidCol);

			for(auto iter = vecLoadCaseKind.begin(); iter != vecLoadCaseKind.end(); ++iter)
			{
				ELSDLoadCaseKind eCase = *(iter);
				dFactor = pLsdManager->GetLoadFactor(-1, szCombo, eCase);
				if(dFactor > 0)
				{
					str.Format(_T("$f2%.2f"),dFactor);
					if(dFactor==0)	str = _T("");

					XLText->AddMergeFormatText(0, nWidCol-1,_T("$c%s"), str);
					XLText->AddFormatText(_T("$m+0%d"), nWidCol);
				}
				else
				{
					XLText->AddMergeFormatText(0, nWidCol-1,_T("$c%s"), "");
					XLText->AddFormatText(_T("$m+0%d"), nWidCol);
				}
			}

			XLText->AddText(_T("$n$h"));
		}
	}
}

HGBOOL CARcBridgeCalcFooting::IsUsedLoadCase(CLsdManager *pMng, ELSDCombKind eComboKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case, long nCombo)
{
	ELSDLoadGroupKind eGroupKind = pMng->GetLoadGroupKindByCombKind(eComboKind);
	if (pMng->IsValidColumn(eGroupKind, Dir, Case) == FALSE)
	{
		return FALSE;
	}
	
	HGDOUBLE dValue = pMng->GetLsdLoadCaseFactor(eComboKind, Dir, nCombo, Case);
	if (dValue != 0)
	{
		return TRUE;
	}

	return FALSE;
}

void CARcBridgeCalcFooting::MakeTableLoadFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, BOOL bSafeCheck)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	const HGINT32 icRow = GetCountLoadFooting(pBri, bSafeCheck);
	const HGINT32 icCol = 6;
	const HGINT32 icWidthCell = 4;
	XLText->AddTable(icRow+1, icCol*icWidthCell - 1, 2, 1);

	CString szUnit_Moment = pDataStd->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pDataStd->GetStringUnitType(UNIT_CGS_TONF);

	CString strHori = "�����("+szUnit_Force+")";
	CString strVert  = "������("+szUnit_Force+")";
	CString strMoment1 = "Mr("+szUnit_Moment+")";
	CString strMoment2 = "Mo("+szUnit_Moment+")";

	//Title
	XLText->AddFormatText(_T("$h$me3$md1$c����$m+04 $me3$md1$c%s$m+04 $me3$md1$c%s$m+04"), strVert, strHori);
	XLText->AddFormatText(_T("$me7$c���Ʈ$m+08 $me3$md1$c���$n"));
	XLText->AddFormatText(_T("$h$m+12$me3$c%s$m+04 $me3$c%s$n$h"), strMoment1, strMoment2);

	CString str;
	CString strOpt = "";
	CLoadFooting *pData = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;
		if(bSafeCheck && ix == eEnvService) continue;
		
		for (HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			
			for(long j=0; j<6; j++)
			{
				// ���� ����� �Ǵϱ� ��ĭ�� ����ְ� �ѱ���.
				if(pData == NULL)
					str = _T("-");
				else if(j == 0)
					str = pData->m_sCombo;
				else if(j == 1)
					str.Format("$f3%.3f",pData->m_dVerTotal);
				else if(j == 2)
					str.Format("$f3%.3f",pData->m_dHorTotal);
				else if(j == 3)
					str.Format("$f3%.3f",pData->m_dMrTotal);
				else if(j == 4)
					str.Format("$f3%.3f",pData->m_dMoTotal);
				else if(j == 5)
					str.Empty();
				
				XLText->AddMergeFormatText(0, icWidthCell-1, "$c%s%s", str, strOpt);
				XLText->AddFormatText("$m+0%d", icWidthCell);
			}

			XLText->AddText("$n$h");			
		}
	}

	XLText->AddText(_T("$n$h"));
}

HGINT32 CARcBridgeCalcFooting::GetCountLoadFooting(CRcBridgeRebar *pBri, BOOL bSafeCheck)
{
	long nCntRow = 0;
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		if(bSafeCheck && eEnvType == eEnvService) continue;

		nCntRow += eCountFootCheck;
	}

	return nCntRow;
}

void CARcBridgeCalcFooting::MakeTableLoadSubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	const HGINT32 icRow = GetCountLoadFooting(pBri, FALSE);

	XLText->GoPosition(NO_MOVE, 1);	//�� ���
	XLText->AddFormatText("$e$h$r%s$r���ݹݷ� ����$n$h", GetNumberString(nIndexCircle,NUM_CIRCLE));

	XLText->GoPosition(NO_MOVE, 2);
	XLText->AddTable(icRow, 20, 1, 1);
	XLText->AddFormatText(_T("$h$c$me2%s$m+03$c$me2V(%s)$m+03$c$me2M(%s)$m+03$c$me2e(%s)$m+03$c$me2Q2(%s)$m+03$c$me2Q1(%s)$m+03$c$me2X(%s)$n"),
		hggettext("�� ��"),pDataStd->m_szTonf, pDataStd->m_szTonfM, pDataStd->m_szM, pDataStd->m_szTonf, pDataStd->m_szTonf, pDataStd->m_szM);
	
	CString sV = _T(""), sM = _T(""), sE = _T(""), sQ2 = _T(""), sQ1 = _T(""), sX = _T("");
	HGBOOL	bTrapezoid	=	FALSE;
	HGBOOL	bTriangle	=	FALSE;

	CLoadFooting *pData = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;

		for(HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			if(pData == NULL)
				continue;

			sV = pDataStd->GetStrDouble(pData->m_dVerTotal, UNIT_CGS_TONF_M);
			sM = pDataStd->GetStrDouble(pData->m_dMrTotal - pData->m_dMoTotal, UNIT_CGS_TONFM);
			sE = pDataStd->GetStrDouble(pData->m_dE, UNIT_CGS_M);
			sQ2 = pDataStd->GetStrDouble(GetValueQ2(pData), UNIT_CGS_TONF_M);
			sQ1 = pDataStd->GetStrDouble(GetValueQ1(pData), UNIT_CGS_TONF_M);
			
			double dB = pData->m_pFooting->GetWidth();
			double dX = min(pData->m_dLx, dB);
			sX = pDataStd->GetStrDouble(dX, UNIT_CGS_M);
			
			XLText->AddFormatText(_T("$h$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$n"),
				pData->m_sCombo, sV, sM, sE, sQ2, sQ1, sX);

			double	Lx	=	3.0*(dB / 2 - pData->m_dE);
			if(Lx > dB)		//��ٸ��� ����
			{
				bTrapezoid	=	TRUE;
			}
			else						//�ﰢ�� ����
			{
				bTriangle	=	TRUE;
			}
		}
	}

	CString str;
	CFootingApp *	pFooting = pBri->GetFootingByJijumNumber(nJijum);
	const CString sWidth = pDataStd->GetStrDoubleUnitChange(toM(pFooting->GetWidth()), UNIT_CGS_M, pDataStd->m_szM);
	str.Format(_T("%s) B : %s (%s%s), M = Mr-Mo, e = B/2 - M/V"), hggettext("��"),hggettext("������ ����"), sWidth ,pDataStd->m_szM) ;
	XLText->AddFormatText(_T("$h%s$n"), str);

	if(bTrapezoid)
	{
		XLText->AddText(hggettext("$h$r- ������ �ۿ���ġ�� ���� �� ���� �ִ� ��� (��ٸ��� ����)$n"));
		XLText->AddFormatText(_T("$h$r$r$p[%s:Pic1]$n$n"),hggettext("�ִ��ּ����ݹݷ�"));
		
		str.Format(hggettext(": ����ݷ��� �ۿ���(x = 3*[B/2-e])�� B���� ū ���"));
		XLText->AddFormatText(_T("$h$r$r%s$n"), str);
	}

	if(bTriangle)
	{
		XLText->AddText(hggettext("$h$r- ������ �ۿ���ġ�� ���� �� �ܿ� �ִ� ��� (�ﰢ�� ����)$n"));
		XLText->AddFormatText(_T("$h$r$r$p[�ﰢ�����ݹݷ�:Pic1]$n$n"));
		
		str.Format(hggettext(": ����ݷ��� �ۿ���(x = 3*[B/2-e])�� B���� ���� ���"));
		XLText->AddFormatText(_T("$h$r$r%s$n"), str);
	}
	
	XLText->AddText("$n$h");

	XLText->OutXL();	// Ȱ��ȭ�� Pic1 ��Ʈ�� �⺻ ��Ʈ�� �������ش�.
}

void CARcBridgeCalcFooting::WriteSectionDesign_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle)
{
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	CLoadFooting *pData = GetLoadFootingLsd(nJijum, eEnvLimit, 0);
	if (pData == NULL)
		return;

	long nRow = (pData->m_bQTriangle)? 11:10;

	CDomyun *Dom = new CDomyun;
	pData->DrawSupportForce(Dom, -1, TRUE);
	XLText->AddFormatText("$s+11$n$h$t");
	XLText->InsertDomyun(Dom, m_pStd->m_pView, nRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	long nIndexSub = 1;
	HGBOOL bTrap   = (pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));

	if(!bTrap)
	{
		long nTypeFoot = pFooting->GetTypeByInputValue();
		long nCntCheckFront = (pData->m_bTwoUpperLoad && nTypeFoot!=0) ? (pBri->m_bCheckFrontFootTrap2 ? 3 : 2) : 1;
		long n=0; for(n=0; n<nCntCheckFront; n++)
		{
			pData->WriteSectionDesignFront_Lsd(XLText, n, nIndexSub++, FALSE);
			WriteSectionForceBySubgradeReaction(XLText, pBri, nJijum, TRUE, n);
			WriteDesignReaction_Direct(XLText, pBri, nJijum, TRUE, n);
		}
	}

	pData->WriteSectionDesignBack_Lsd(XLText, nIndexSub, FALSE);
	WriteSectionForceBySubgradeReaction(XLText, pBri, nJijum, FALSE, 0);
	WriteDesignReaction_Direct(XLText, pBri, nJijum, FALSE, 0);
}

void CARcBridgeCalcFooting::WriteSectionDesign_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, long nIndexCircle)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	CLoadFooting *pData = GetLoadFootingLsd(nJijum, eEnvLimit, 0);
	if (pData == NULL)
		return;

	if(pData->m_bEarthQuake && pBri->IsApplyEarthQuakeDesign() == FALSE)
		return;

	XLText->GoPosition(NO_MOVE, 1);	//�� ���
	XLText->AddFormatText("$e$h$r%s$r�ܸ�� ���$n$h", GetNumberString(nIndexCircle,NUM_CIRCLE));

	MakeTableLoad_Pile(XLText, pBri, nJijum, TRUE);

	long nIndexSub = 1;
	pData->WriteSectionDesign_PileText(XLText, nIndexSub, TRUE);

	long nCntCheckFront = (pData->m_bTwoUpperLoad)? (pBri->m_bCheckFrontFootTrap2 ? 3 : 2) : 1;
	long n=0; for(n=0; n<nCntCheckFront; n++)
	{
		pData->WriteSectionDesignFront_Pile(XLText, n, nIndexSub++, FALSE);
		WriteDesignReaction_Pile(XLText, pBri, nJijum, TRUE, n);
	}

	pData->WriteSectionDesign_PileText(XLText, nIndexSub, FALSE);
	MakeTableLoad_Pile(XLText, pBri, nJijum, FALSE);
	pData->WriteSectionDesignBack_Pile(XLText, FALSE);
	WriteDesignReaction_Pile(XLText, pBri, nJijum, FALSE, 0);
}


void CARcBridgeCalcFooting::MakeTableLoad_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	CLoadFooting *pData = GetLoadFootingLsd(nJijum, eEnvLimit, 0);
	if (pData == NULL)
		return;

	long nTypePile = bFront ? 0 : 7; // ��ü
	pData->GetPileForcArr(nTypePile, FALSE);

	CExPileApp *pPile = &pData->m_pFooting->m_exFooting.m_Pile;
	pPile->SetPileArray();

	const HGINT32 icRow = GetCountLoadFooting(pBri, FALSE);
	const HGINT32 icCol = pData->m_dArrPileFoce.GetSize() + 1;
	
	XLText->GoPosition(NO_MOVE, 2);
	XLText->AddTable(icRow, icCol * 3, 1, 1);

	int i=0; for(i=0; i<icCol; i++)
	{
		CString str;
		HGINT32 iWidth = 3;
		if (i == 0)
		{
			iWidth = 4;
			str = "�� ��";
		}
		else
		{
			str.Format("Ru%d", i);
		}

		XLText->AddMergeFormatText(0,iWidth-1,"$c%s", str);
		XLText->AddFormatText("$m+0%d",iWidth);
		
	}
	XLText->AddText("$n$h");

	double dForce = 0;
	double wFooting = toM(pData->m_pFooting->GetLength());
	long nEa = 0;
	
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;
		
		for(HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			if(pData == NULL)
			{
				// ���� ����� �Ǵϱ� ��ĭ�� ����ְ� �ѱ���.
				for(long j=0; j<icCol; j++)
				{
					HGINT32 iWidth = (j == 0)? 4 : 3;
					XLText->AddMergeText(0, iWidth-1, "$c$j-");
					XLText->AddFormatText("$m+0%d",iWidth);
				}
				XLText->AddText("$n$h");
				continue;
			}

			CExPileApp *pPile = &pData->m_pFooting->m_exFooting.m_Pile;
			pPile->SetPileArray();
			pData->GetPileForcArr(nTypePile, FALSE);

			for (HGINT32 ixCol = 0; ixCol < icCol; ++ixCol)
			{
				CString str;
				HGINT32 iWidth = 3;
				if (ixCol == 0)
				{
					iWidth = 4;
					str = pData->m_sCombo;
				}
				else
				{
					if (ixCol - 1 < pData->m_dArrPileFoce.GetCount())
					{
						nEa = (long)pPile->m_ArrayOrg[ixCol - 1][1];
						dForce = pData->m_dArrPileFoce.GetAt(ixCol - 1);

						str = pDataStd->GetStrDouble(dForce, UNIT_CGS_TONF_M);
					}
				}

				XLText->AddMergeFormatText(0,iWidth-1,"$c%s", str);
				XLText->AddFormatText("$m+0%d",iWidth);	
			}

			XLText->AddText("$n$h");
		}
	}
	
	XLText->AddText("$n$h");
}

void CARcBridgeCalcFooting::WriteDesignReaction_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	const HGINT32 icRow = GetCountLoadFooting(pBri, FALSE);
	XLText->GoPosition(NO_MOVE, 3);
	XLText->AddFormatText(_T("$s+%02d"), icRow+5);
	XLText->AddText(hggettext("$h - ����ܸ�� ����$n$h"));

	XLText->AddTable(icRow+1, 19, 2, 1);

	XLText->AddMergeText(1, 1, _T("$m+02"));
	XLText->AddText(hggettext("$h$c$me3�� ��$m+04$c$me7���ҹݷ¿����� �ܸ��$m+08$c$me7����ܸ��$n$h"));
	XLText->AddFormatText(_T("$h$m+04$c$me3Vb(%s) $m+04$c$me3Ma(%s) $m+04$c$me3Vu(%s) $m+04$c$me3Mu(%s)$n$h"),
		pDataStd->m_szTonf, pDataStd->m_szTonfM, pDataStd->m_szTonf, pDataStd->m_szTonfM);

	CString strName;
	CString sVb = _T(""), sMa = _T(""), sVu = _T(""), sMu = _T("");

	CLoadFooting *pData = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;
		
		for(HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			if(pData == NULL) 
			{
				// ���� ����� �Ǵϱ� ��ĭ�� ����ְ� �ѱ���.
				XLText->AddText(_T("$h$c$me3-$m+04$c$me3-$m+04$c$me3-$m+04$c$me3-$m+04$c$me3-$n$h"));
				continue;
			}

			HGDOUBLE dSq = 0;
			HGDOUBLE dMq = 0;
			HGDOUBLE dVu = 0;
			HGDOUBLE dMu = 0;
			if (bFront)
			{
				dSq = pData->m_SqF;
				dMq = pData->m_MqF;
				dVu = pData->m_dVuFront;
				if (n == 0)
				{
					dMu = pData->m_dMuFront;
				}
				else if (n == 1)
				{
					dMu = pData->m_dMuFront2;
				}
				else
				{
					dMu = pData->m_dMuFront3;
				}
			}
			else
			{
				dSq = pData->m_SqB;
				dMq = pData->m_MqB;
				dVu = pData->m_dVuBack;
				dMu = pData->m_dMuBack;
			}

			strName = pData->m_sCombo;
			sVb = pDataStd->GetStrDouble(dSq, UNIT_CGS_TONF);
			sMa = pDataStd->GetStrDouble(dMq, UNIT_CGS_TONFM);
			sVu = pDataStd->GetStrDouble(dVu, UNIT_CGS_TONF);
			sMu = pDataStd->GetStrDouble(dMu, UNIT_CGS_TONFM);

			XLText->AddFormatText(_T("$h$c$me3%s $m+04$c$me3%s $m+04$c$me3%s $m+04$c$me3%s $m+04$c$me3%s$n$h"),
				strName, sVb, sMa, sVu, sMu);
		}
	}
}


void CARcBridgeCalcFooting::WriteDesignReaction_Direct(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n)
{
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	const HGINT32 icRow = GetCountLoadFooting(pBri, FALSE);
	XLText->GoPosition(NO_MOVE, 3);
	XLText->AddFormatText(_T("$s+%02d"), icRow+(icRow / eCountFootCheck)*2+2);
	XLText->AddText(hggettext("$h - ����ܸ�� ����$n$h"));
	
	XLText->AddTable(icRow+(icRow / eCountFootCheck)*2+1, 22, 2, 1);

	XLText->AddMergeText(1, 1, _T("$m+02"));
	XLText->AddText(hggettext("$h$c$me1�� ��$m+02$c$me8���ݹݷ�$m+09$c$me5���ݹݷ¿����� �ܸ��$m+06$c$me5����ܸ��$n$h"));
	XLText->AddFormatText(_T("$h$m+02$c$me2%s(%s) $m+03$c$me2Qa(%s) $m+03$c$me2Qb(%s) $m+03$c$me1Vb(%s) $m+02$c$me3Ma(%s) $m+04$c$me1Vu(%s) $m+02$c$me3Mu(%s)$n$h"),
		bFront ? "Q1": "Q2",pDataStd->m_szTonf, pDataStd->m_szTonf, pDataStd->m_szTonf, pDataStd->m_szTonf, pDataStd->m_szTonfM, pDataStd->m_szTonf, pDataStd->m_szTonfM);

	CString strName;
	CString sQ = _T(""), sQa = _T(""), sQb = _T(""), sVb = _T(""), sMa = _T(""), sVu = _T(""), sMu = _T("");

	CLoadFooting *pData = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;

		for(HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pData = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			if(pData == NULL) continue;

			HGDOUBLE dQMaxMin = 0;
			HGDOUBLE dQm = 0;
			HGDOUBLE dQv = 0;
			HGDOUBLE dSq = 0;
			HGDOUBLE dMq = 0;
			HGDOUBLE dVu = 0;
			HGDOUBLE dMu = 0;
			if (bFront)
			{
				dQMaxMin = GetValueQ1(pData);
				dSq = pData->m_SqF;
				dMq = pData->m_MqF;
				dVu = pData->m_dVuFront;
				if (n == 0)
				{
					dQm = pData->m_dQmF;
					dMu = pData->m_dMuFront;
				}
				else if (n == 1)
				{
					dQm = pData->m_dQmF2;
					dMu = pData->m_dMuFront2;
				}
				else
				{
					dQm = pData->m_dQmF3;
					dMu = pData->m_dMuFront3;
				}
				dQv = pData->m_dQvF;
			}
			else
			{
				dQMaxMin = GetValueQ2(pData);
				dQm = pData->m_dQmB;
				dQv = pData->m_dQvB;
				dSq = pData->m_SqB;
				dMq = pData->m_MqB;
				dVu = pData->m_dVuBack;
				dMu = pData->m_dMuBack;
			}
			
			strName = pData->m_sCombo;
			sQ = pDataStd->GetStrDouble(dQMaxMin, UNIT_CGS_TONF_M);
			sQa = pDataStd->GetStrDouble(dQm, UNIT_CGS_TONF_M);
			sQb = pDataStd->GetStrDouble(dQv, UNIT_CGS_TONF_M);
			sVb = pDataStd->GetStrDouble(dSq, UNIT_CGS_TONF);
			sMa = pDataStd->GetStrDouble(dMq, UNIT_CGS_TONFM);
			sVu = pDataStd->GetStrDouble(dVu, UNIT_CGS_TONF);
			sMu = pDataStd->GetStrDouble(dMu, UNIT_CGS_TONFM);
			
			XLText->AddFormatText(_T("$h$c$me1%s $m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me3%s $m+04$c$me1%s $m+02$c$me3%s$n$h"),
				strName, sQ, sQa, sQb, sVb, sMa, sVu, sMu);
		}
	}

	HGINT32 ix = bFront ? 0 : 1;
	enum { eMaxVu, eMaxMu, eMinVu, eMinMu, eCount };
	std::vector<HGDOUBLE> vecMaxMin[eCountEnvTypeLsd];

	for (HGINT32 ixEnvType = eEnvLimit; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
	{
		vecMaxMin[ixEnvType].resize(eCount);
		vecMaxMin[ixEnvType].at(eMaxVu) = bFront ? m_dATensionLowerVLsd[nJijum][ixEnvType][ix] : m_dATensionUpperVLsd[nJijum][ixEnvType][ix];
		vecMaxMin[ixEnvType].at(eMaxMu) = bFront ? m_dATensionLowerMLsd[nJijum][ixEnvType][ix] : m_dATensionUpperMLsd[nJijum][ixEnvType][ix];
		vecMaxMin[ixEnvType].at(eMinVu) = bFront ? m_dATensionUpperVLsd[nJijum][ixEnvType][ix] : m_dATensionLowerVLsd[nJijum][ixEnvType][ix];
		vecMaxMin[ixEnvType].at(eMinMu) = bFront ? m_dATensionUpperMLsd[nJijum][ixEnvType][ix] : m_dATensionLowerMLsd[nJijum][ixEnvType][ix];
	}

	CString str = "-";
	XLText->AddColor(0,0,20);
	for (HGINT32 ixEnvType = 0; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE)
			continue;

		CString strPre;
		if (ixEnvType == eEnvLimit)
			strPre = "St";
		else if (ixEnvType == eEnvExtremeI)
			strPre = "Ex1";
		else if (ixEnvType == eEnvExtremeII)
			strPre = "Ex2";
		else if (ixEnvType == eEnvService)
			strPre = "Se";
		else if (ixEnvType == eEnvService_Crack)
			strPre = "Crack";

		for (HGINT32 ix = 0; ix < 2; ++ix)
		{
			const HGBOOL bMax = ix == 0;
			const CString strMax = bMax ? "Max" : "Min";

			CString strVu = pDataStd->GetStrDouble(bMax ? vecMaxMin[ixEnvType].at(eMaxVu) : vecMaxMin[ixEnvType].at(eMinVu), UNIT_CGS_TONF);
			CString strMu = pDataStd->GetStrDouble(bMax ? vecMaxMin[ixEnvType].at(eMaxMu) : vecMaxMin[ixEnvType].at(eMinMu), UNIT_CGS_TONFM);

			XLText->AddFormatText(_T("$h$c$me1$b034%s $m+02$c$me2$b034%s $m+03$c$me2$b034%s $m+03$c$me2$b034%s $m+03$c$me1$b034%s $m+02$c$me3$b034%s $m+04$c$me1$b034%s $m+02$c$me3$b034%s$n$h"),
				strPre + strMax, str, str, str, str, str, strVu, strMu);
		}
	}
	
	XLText->AddText(hggettext("$h��) ���ݹݷ� ������ ���߰���� ����Ǿ����Ƿ� ���ݹݷ¿� ���� �ܸ�¿���$n$h"));
	XLText->AddText(hggettext("$h$r���߰���� �������� �ʴ´�.$n$h"));

	XLText->AddText("$n$h");
}

HGDOUBLE CARcBridgeCalcFooting::GetCenterOfFigure(CRcBridgeRebar *pBri, HGINT32 ixJijum)
{
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(ixJijum);
	if(pFooting == NULL || pBri->IsPileBase(ixJijum)==FALSE) 
		return 0;

	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	BOOL bInWall = (ixJijum == 0 || ixJijum == pBri->GetCountJijum()-1)? FALSE:TRUE;
	pPile->SetPileArray();

	double dDist	= (bInWall)? toM(pFooting->GetWidth()):0;
	double dEaTot	= 0;
	double dCalcTot	= 0;
	
	for(HGINT32 ix = 0; ix < pPile->m_nCountOrg; ++ix)
	{
		if(!bInWall)
			dDist += toM(pPile->m_ArrayOrg[ix][0]);
		else
			dDist -= toM(pPile->m_ArrayOrg[ix][0]);
		
		double dEa	= pPile->m_ArrayOrg[ix][1];
		double dCalc = dDist * dEa;

		dEaTot	+= dEa;
		dCalcTot += dCalc;
	}
	
	const HGDOUBLE dDosim = dEaTot ? dCalcTot / dEaTot : 0;
	
	return dDosim;
}

CLoadFooting* CARcBridgeCalcFooting::GetLoadFootingLsd( long nJijum, ETypeEnvComboLsd eEnvCombo, long ixCheck)
{
	long nIndexEnv = eEnvCombo;
	
	long nSize = m_pListLoadLsd[nJijum][nIndexEnv].GetSize();
	if(nSize <= ixCheck)
		return NULL;

	return m_pListLoadLsd[nJijum][nIndexEnv].GetAt(ixCheck);
}

long CARcBridgeCalcFooting::GetCountLoadFootingLsd( long nJijum, ETypeEnvComboLsd eEnvCombo )
{
	long nIndexEnv = eEnvCombo;
	
	return m_pListLoadLsd[nJijum][nIndexEnv].GetSize();
}

CLsdFootingDesign* CARcBridgeCalcFooting::GetLsdFootingDesign( long nJijum )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLsdFootingDesign *pFootingDesing(NULL);

	long ixIndex(-1);
	
	if(pBri->IsBoxType())
	{
		ixIndex = 0;
	}
	else
	{
		long nCntJijum = pBri->GetCountJijum();
		for(long nJ=0; nJ<nCntJijum; ++nJ)
		{
			CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
			if(pFooting == NULL || pFooting->m_bIs == FALSE) continue;

			if((nJ==0 && !pBri->IsOutWall(TRUE)) || (nJ==nCntJijum-1 && !pBri->IsOutWall(FALSE))) continue;

			++ixIndex;

			if(nJijum == nJ)
				break;
		}
	}

	long nSize = m_pListLsdFootingSafe.size();
	if(ixIndex > -1 && ixIndex < nSize)
	{
		pFootingDesing = m_pListLsdFootingSafe[ixIndex];
	}

	return pFootingDesing;
}

void CARcBridgeCalcFooting::WriteCalcSupport_Usd( CXLFormatText *XLText, long nJijum, long &nIndexSmall, BOOL bEarthquake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
	if(pBoring == NULL) return;

	CDPoint vAng(0,1);
	double dStation = pBri->GetStationOnJijum(nJijum);
	double dWidth = toM(pBri->GetWidthSlabAct(dStation, vAng));
	double dLength = toM(GetLengthBox());
	if(pBri->m_bExistProtectionRiseFooting)
	{
		dLength += toM(pBri->m_dPRF_W * 2);
	}

	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	*XLText += 1;
	// Terzaghi //////////////////////////////////////////////////
	if(pFooting->m_nTypeEquation_Qa == 0)
	{
		double NValue = pBoring->m_nNValue;
		double pi = ToRadian(pFooting->GetFrictionAngleBaseGround(NValue));
		double piDeg = pFooting->GetFrictionAngleBaseGround(NValue);
		double L  = dLength; //toM(pFooting->GetLength());
		double B  = dWidth; //toM(pFooting->GetWidth());
		double Df = pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
		double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
		double r1 = pData->GetValueUnitType(pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
		double r2 = pData->GetValueUnitType(pFooting->m_dDfGroundUnitWeight, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

		// �������鿡�� ���ϼ��������� ����(dH)�� ������ ��(B_)���� ������쿡�� r1�� r_�� ����Ѵ�			
		double q	= r2*Df; // ????
		////////////////////////////////////////////////
		double eB	= fabs(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
		double B_	= B - 2*fabs(eB);
		double alpha= 1.0 + 0.3*B_/L; // ���簢��
		double beta = 0.5 - 0.1*B_/L;
		double Kpr	= GetKprValue(pFooting->GetFrictionAngleBaseGround(pBoring->m_nNValue));
		double  a   = exp((0.75*ConstPi-pi/2)*tan(pi));
		double Nq	= pow(a,2) / (2*pow(cos(ConstPi/4+pi/2),2));
		double Nc	= (Nq-1)/tan(pi);
		double Nr	= tan(pi)/2 * (Kpr/pow(cos(pi),2) - 1);
		double SF = bEarthquake ? 2 : 3;

		double Q_Normal	 = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Q_EarthQ	 = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Qa	= bEarthquake ? Q_EarthQ : Q_Normal;

		double Qe	= m_dQaEquation/SF;
		double Qa_	= MIN(Qa, Qe);
		BOOL bOkSupportForce = Qa_ >= m_dQMax[0] ? TRUE : FALSE;

		CString szPiEqu = pFooting->GetStrignFrictionAngleEquation();
		CString szComp1 = (Qe > Qa)? ">" : "<";
		CString szComp2 = bOkSupportForce ? ">" : "<";
		CString szOkNg = bOkSupportForce ? "O.K" : "N.G";

		XLText->AddFormatText("$h%d) ������ ��뿬�������� : Terzaghi�� ���$n$n", nIndexSmall++);
		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText("$h$cQu$r$c=$r$e[ ��xcxNc + qxNq + ��xr1xB_xNr ]$n$h");

		XLText->AddFormatText("$h$m+02$iQu$r$c:$r$e������ ����� ����� ������ ����������(%s)$n", pData->m_szTonf);
		XLText->AddFormatText("$h$m+02$ic$r$c:$r$e������ ������(%s)$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$iq$r$c:$r$e��������(%s),  q = r2 x Df$n", pData->m_szTonf_M2);
		XLText->AddFormatText("$h$m+02$ir1, r2$r$c:$r$e�������� �� ���������� �����߷�(%s)$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$i$r$c$r$e�ٸ�, ���ϼ������Ͽ����� ���ߴ����߷��� ����Ѵ�.$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$iB'$r$c:$r$e������ ����� ����� ������ ��ȿ������(m), B' = B - 2e$n");
		XLText->AddFormatText("$h$m+02$iB$r$c:$r$e������ ��(m)$n");
		XLText->AddFormatText("$h$m+02$ie$r$c:$r$e������ ��ɷ�(m)$n");
		XLText->AddFormatText("$h$m+02$iDf$r$c:$r$e������ ��ȿ���� ����(m)$n");
		XLText->AddFormatText("$h$m+02$iNc,Nq,Nr$r$c:$r$e������ ���$n");

		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ic$r$c=$r$e%.3f %s$m+05r1$r$c=$r$e%.3f %s$m+05r2$r$c=$r$e%.3f %s$n", c, pData->m_szTonf_M2, r1, pData->m_szTonf_M3, r2, pData->m_szTonf_M3);
		XLText->AddFormatText("$h$iDf$r$c=$r$e%.3f m$m+05B$r$c=$r$e%.3f m$m+05q$r$c=$r$e%.3f %s$n", Df,B, q, pData->m_szTonf_M2);

		if(szPiEqu.IsEmpty())
			XLText->AddFormatText("$h$i��$r$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", piDeg, NValue);
		else
			XLText->AddFormatText("$h$i��$r$c=$r$e%s$m+04$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", szPiEqu, piDeg, NValue);

		XLText->AddFormatText("$h$ia$r$c=$r$eexp((0.75*��-��/2) x tan(��))$m+08$c=$r$e$f3%.3f$me1$n", a);
		XLText->AddFormatText("$h$iNq$r$c=$r$ea��/(2xCos��(��/4+��/2))$m+08$c=$r$e$f3%.3f$me1$n", Nq);
		XLText->AddFormatText("$h$iNc$r$c=$r$e(Nq-1)/tan��$m+08$c=$r$e$f3%.3f$me1$n", Nc);
		XLText->AddFormatText("$h$iNr$r$c=$r$etan��/2 x (Kpr/Cos���� - 1)$m+08$c=$r$e$f3%.3f$me1$n", Nr);

		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", eB);
		XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(eB), B_);
		XLText->AddFormatText("$h$i��$r$c=$r$e1.0 + 0.3x(B'/L)$m+08$c=$r$e$f3%.3f$me1$n", alpha);
		XLText->AddFormatText("$h$i��$r$c=$r$e0.5 + 0.1x(B'/L)$m+08$c=$r$e$f3%.3f$me1$n", beta);

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e[ %.3f x %.3f x %.3f + %.3f x %.3f + %.3f x %.3f x %.3f x %.3f ]$n", alpha, c, Nc, q, Nq, beta, r1, B_, Nr);
		XLText->AddFormatText("$h$e$r$c=$r$e%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.3f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);

		// 		if(pBri->m_bApplyMassCalcSafe && pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
		// 		{
		// 			XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQMax+m_dQmaxMass, pData->m_szTonf_M2);
		// 		}
		// 		else
		{
			XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQMax[0], pData->m_szTonf_M2);
		}

		if(!bOkSupportForce) 
			XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
		else 
			XLText->AddFormatText("$e$a%s$n", szOkNg);
	}
	// Meyerhof //////////////////////////////////////////////////
	else if(pFooting->m_nTypeEquation_Qa == 1)
	{
		double NValue = pBoring->m_nNValue;
		double pi = ToRadian(pFooting->GetFrictionAngleBaseGround(NValue));
		double piDeg = pFooting->GetFrictionAngleBaseGround(NValue);
		double B  = toM(pFooting->GetWidth());
		double Df = pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
		double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
		double r1 = pData->GetValueUnitType(pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
		double r2 = pData->GetValueUnitType(pFooting->m_dDfGroundUnitWeight, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

		double eB	= fabs(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
		double q	= r2*Df; // ????
		double B_	= B;
		double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
		double Nc	= (pi==0) ? 5.14 : (Nq-1)/tan(pi);
		double Nr	= (Nq-1)*tan(1.4*pi);
		double SF = bEarthquake ? 2 : 3;

		double Q_Normal	 = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Q_EarthQ	 = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Qa	= bEarthquake? Q_EarthQ:Q_Normal;

		double Qe	= m_dQaEquation/SF;
		double Qa_	= MIN(Qa, Qe);
		BOOL bOkSupportForce = Qa_ >= m_dQMax[0] ? TRUE : FALSE;

		CString szPiEqu = pFooting->GetStrignFrictionAngleEquation();
		CString szComp1 = (Qe > Qa)? ">":"<";
		CString szComp2 = (bOkSupportForce)? ">":"<";
		CString szOkNg = (bOkSupportForce)? "O.K":"N.G";

		XLText->AddFormatText("$h%d) ������ ��뿬�������� : Meyerhof�� ���$n$n", nIndexSmall++);
		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText("$h$cQu$r$c=$r$e[ cxNc + qxNq + 0.5xr1xB_xNr ]$n$h");

		XLText->AddFormatText("$h$m+02$iQu$r$c:$r$e������ ����������(%s)$n", pData->m_szTonf);
		XLText->AddFormatText("$h$m+02$ic$r$c:$r$e������ ������(%s)$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$iq$r$c:$r$e��������(%s),  q = r2 x Df$n", pData->m_szTonf_M2);
		XLText->AddFormatText("$h$m+02$ir1, r2$r$c:$r$e�������� �� ���������� �����߷�(%s)$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$i$r$c$r$e�ٸ�, ���ϼ������Ͽ����� ���ߴ����߷��� ����Ѵ�.$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$iB'$r$c:$r$e������ ����� ����� ������ ��ȿ������(m), B' = B - 2e$n");
		XLText->AddFormatText("$h$m+02$iB$r$c:$r$e������ ��(m)$n");
		XLText->AddFormatText("$h$m+02$ie$r$c:$r$e������ ��ɷ�(m)$n");
		XLText->AddFormatText("$h$m+02$iDf$r$c:$r$e������ ��ȿ���� ����(m)$n");
		XLText->AddFormatText("$h$m+02$iNc,Nq,Nr$r$c:$r$e������ ���$n");

		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ic$r$c=$r$e%.3f %s$m+05r1$r$c=$r$e%.3f %s$m+05r2$r$c=$r$e%.3f %s$n", c, pData->m_szTonf_M2, r1, pData->m_szTonf_M3, r2, pData->m_szTonf_M3);
		XLText->AddFormatText("$h$iDf$r$c=$r$e%.3f m$m+05B$r$c=$r$e%.3f m$m+05q$r$c=$r$e%.3f %s$n", Df,B, q, pData->m_szTonf_M2);

		if(szPiEqu.IsEmpty())
			XLText->AddFormatText("$h$i��$r$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", piDeg, NValue);
		else
			XLText->AddFormatText("$h$i��$r$c=$r$e%s$m+04$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", szPiEqu, piDeg, NValue);

		XLText->AddFormatText("$h$iNq$r$c=$r$eexp(��xtan��) x tan��(��/4 + ��/2)$m+08$c=$r$e$f3%.3f$me1$n", Nc);
		XLText->AddFormatText("$h$iNc$r$c=$r$e %.3f$m+04$e, ��=0: 5.14  ��>0: (Nq-1)/tan��$n", Nq);
		XLText->AddFormatText("$h$iNr$r$c=$r$e(Nq-1) x tan(1.4x��)$m+08$c=$r$e$f3%.3f$me1$n", Nr);

		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", eB);
		XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(eB), B_);

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e[ %.3f x %.3f + %.3f x %.3f + 0.5 x %.3f x %.3f x %.3f ]$n", c, Nc, q, Nq, r1, B_, Nr);
		XLText->AddFormatText("$h$e$r$c=$r$e%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.3f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);
		// 		if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
		// 		{
		// 			XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
		// 		}
		//		else
		{
			XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQMax[0], pData->m_szTonf_M2);
		}
		if(!bOkSupportForce) 
			XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
		else 
			XLText->AddFormatText("$e$a%s$n", szOkNg);
	}
	// Hansen   //////////////////////////////////////////////////
	else if(pFooting->m_nTypeEquation_Qa == 2)
	{
		XLText->AddFormatText("$h%d) ������ ��뿬�������� : Hansen�� ���$n$n", nIndexSmall++);
		XLText->GoPosition(NO_MOVE, COL_POS_2);
		XLText->AddFormatText("$h$cQu$r$c=$r$ec Nc sc dc ic gc bc + q Nq sq dq iq gq bq + 1/2 r1 B' Nr sr dr ir gr br$n$h");
		XLText->AddFormatText("$h$r$e��=0 �϶�, Qu = 5.14c (1+ sc' + dc' - ic' - gc' -bc') + q$n");

		XLText->AddFormatText("$h$m+02$iQu$r$c:$r$e������ ����� ����� ������ ����������(%s)$n", pData->m_szTonf);
		XLText->AddFormatText("$h$m+02$ic$r$c:$r$e������ ������(%s)$n", pData->m_szTonf_M2);
		XLText->AddFormatText("$h$m+02$iA'$r$c:$r$e��ȿ���ϸ���(��)$n");
		XLText->AddFormatText("$h$m+02$iq$r$c:$r$e��������(%s),  q = r2 x Df$n", pData->m_szTonf_M2);
		XLText->AddFormatText("$h$m+02$ir1, r2$r$c:$r$e�������� �� ���������� �����߷�(%s)$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$i$r$c$r$e�ٸ�, ���ϼ������Ͽ����� ���ߴ����߷��� ����Ѵ�.$n", pData->m_szTonf_M3);
		XLText->AddFormatText("$h$m+02$iB'$r$c:$r$e������ ����� ����� ������ ��ȿ������(m), B' = B - 2e$n");
		XLText->AddFormatText("$h$m+02$iB$r$c:$r$e������ ��(m)$n");
		XLText->AddFormatText("$h$m+02$ie$r$c:$r$e������ ��ɷ�(m)$n");
		XLText->AddFormatText("$h$m+02$iDf$r$c:$r$e������ ��ȿ���� ����(m)$n");
		XLText->AddFormatText("$h$m+02$iNc,Nq,Nr$r$c:$r$e������ ���$n");
		XLText->AddFormatText("$h$m+02$isc,sq,sr,sc'$r$c:$r$e������ ������$n");
		XLText->AddFormatText("$h$m+02$idc,dq,dr,dc'$r$c:$r$e������ ���Ա��̰��$n");
		XLText->AddFormatText("$h$m+02$igc,gq,gr,gc'$r$c:$r$e���ݰ��$n");
		XLText->AddFormatText("$h$m+02$ibc,bq,br,bc'$r$c:$r$e�������� �����$n");
		XLText->AddFormatText("$h$m+02$r$e��) ���������� ���(��)�� �����Ƿ� bc,bq,br,bc'�� �������� �ʴ´�.$n");
		XLText->AddFormatText("$h$m+02$r$e    si�� ii�� �Բ� ������� �����Ƿ� ic,iq,ir,ic'�� �������� �ʴ´�.$n");

		double NValue = pBoring->m_nNValue;
		double pi = ToRadian(pFooting->GetFrictionAngleBaseGround(NValue));
		double piDeg = pFooting->GetFrictionAngleBaseGround(NValue);
		double L  = dLength; //toM(pFooting->GetLength());
		double B  = dWidth; //toM(pFooting->GetWidth());
		double Df = pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
		double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);		// ���������� ������
		double r1 = pData->GetValueUnitType(pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
		double r2 = pData->GetValueUnitType(pFooting->m_dDfGroundUnitWeight, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
		double beta = 0; // ���ݰ��..
		// Qu = A'[c*Nc*Sc*dc*ic*gc*bc + q*Nq*Sq*dq*iq*gq*bq + 1/2*r1*B'*Nr*Sr*dr*ir*gr*br]
		double eB	= fabs(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
		double B_	= B - 2*fabs(eB);
		double q	= r2*Df;	// ????
		double k    = (Df/B<=1) ? Df/B : atan(Df/B);
		//////////////////////////////////////////////////////////////////////////
		double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
		double Nc	= (Nq-1)/tan(pi);
		double Nr	= 1.5*(Nq-1)*tan(pi);
		double Sc	= 1 + (Nq/Nc) * (B/L);
		double Sq	= 1 + (B/L) * tan(pi);
		double Sr	= 1 - 0.4 * B/L;
		double Sc_	= 0.2 * B/L;
		double dc	= 1 + 0.4*k;
		double dq	= 1 + 2*tan(pi)*pow(1-sin(pi),2)*k;
		double dr	= 1;
		double dc_	= 0.4*k;
		double gc	= 1 - beta/147.0;
		double gq	= pow(1 - 0.5*tan(ToRadian(beta)),5);
		double gr	= gq;
		double gc_	= beta/147.0;
		double SF = (bEarthquake) ? 2 : 3;

		double Q_Normal = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Q_EarthQ = pData->GetValueUnitType(pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Qa	= (bEarthquake) ? Q_EarthQ : Q_Normal;

		double Qe	= m_dQaEquation / SF;
		double Qa_	= MIN(Qa, Qe);

		BOOL bOkSupportForce = Qa_ >= m_dQMax[0] ? TRUE : FALSE;
		CString szPiEqu = pFooting->GetStrignFrictionAngleEquation();
		CString szComp1 = (Qe > Qa) ? ">":"<";
		CString szComp2 = (bOkSupportForce)? ">" : "<";
		CString szOkNg = (bOkSupportForce)? "O.K" : "N.G";

		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ic$r$c=$r$e%.3f %s$m+05r1$r$c=$r$e%.3f %s$m+05r2$r$c=$r$e%.3f %s$n", c, pData->m_szTonf_M2, r1, pData->m_szTonf_M3, r2, pData->m_szTonf_M3);
		XLText->AddFormatText("$h$iDf$r$c=$r$e%.3f m$m+05B$r$c=$r$e%.3f m$m+05q$r$c=$r$e%.3f %s$n", Df,B, q, pData->m_szTonf_M2);
		XLText->AddFormatText("$h$iL$r$c=$r$e%.3f m$n", L);

		if(szPiEqu.IsEmpty())
			XLText->AddFormatText("$h$i��$r$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", piDeg, NValue);
		else
			XLText->AddFormatText("$h$i��$r$c=$r$e%s$m+04$c=$r$e%.3f,$m+03N$r$c=$r$e%.0f$n", szPiEqu, piDeg, NValue);

		XLText->AddFormatText("$h$iNq$r$c=$r$eexp(��tan��) tan��(45+��/2)$m+08$c=$r$e$f3%.3f$me1$n", Nq);
		XLText->AddFormatText("$h$iNc$r$c=$r$e(Nq-1) cot��$m+08$c=$r$e$f3%.3f$me1$n", Nc);
		XLText->AddFormatText("$h$iNr$r$c=$r$e1.5(Nq-1) tan��$m+08$c=$r$e$f3%.3f$me1$n", Nr);
		XLText->AddFormatText("$h$isc$r$c=$r$e1 + Nq/Nc x B/L$m+08$c=$r$e$f3%.3f$me1$n", Sc);
		XLText->AddFormatText("$h$isc'$r$c=$r$e0.2 B/L$m+08$c=$r$e$f3%.3f$me1$n", Sc_);
		XLText->AddFormatText("$h$isq$r$c=$r$e1 + B/L x tan��$m+08$c=$r$e$f3%.3f$me1$n", Sq);
		XLText->AddFormatText("$h$isr$r$c=$r$e1 - 0.4 B/L$m+08$c=$r$e%.3f$me1$n", Sr);
		XLText->AddFormatText("$h$idc$r$c=$r$e1 + 0.4k$m+04$c=$r$e%.3f$m+02, D/B>1 : k=tan-1(D/B), D/B��1 : k=D/B(rad)$n", dc);
		XLText->AddFormatText("$h$idc'$r$c=$r$e0.4k$m+08$c=$r$e%.3f$me1$n", dc_);
		XLText->AddFormatText("$h$idq$r$c=$r$e1 + 2tan��(1-sin��)��k$m+08$c=$r$e%.3f$me1$n", Sq);
		XLText->AddFormatText("$h$idr$r$c=$r$e$f3%.3f$me1$n", dr);
		XLText->AddFormatText("$h$igc$r$c=$r$e1 - ��/147$m+08$c=$r$e$f3%.3f$me1$n", gc);
		XLText->AddFormatText("$h$igc'$r$c=$r$e��/147$m+08$c=$r$e$f3%.3f$me1$n", gc_);
		XLText->AddFormatText("$h$igq$r$c=$r$e(1-0.5tan��)x(1-0.5tan��)��$m+08$c=$r$e$f3%.3f$me1$n", gq);
		XLText->AddFormatText("$h$igr$r$c=$r$e(1-0.5tan��)x(1-0.5tan��)��$m+08$c=$r$e$f3%.3f$me1$n", gr);

		XLText->AddFormatText("$s+15$n$h");
		XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", eB);
		XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(eB), B_);

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		*XLText += 1;
		XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e%.3f x %.3f x %.3f x %.3f x %.3f$n", c, Nc, Sc, dc, gc);
		XLText->AddFormatText("$h$e$r$r$e + %.3f x %.3f x %.3f x %.3f x %.3f$n", q, Nq, Sq, dq, gq);
		XLText->AddFormatText("$h$e$r$r$e + 1/2 x %.3f x %.3f x %.3f x %.3f x %.3f x %.3f$n", r1, B_, Nr, Sr, dr, gr);
		XLText->AddFormatText("$h$e$r$c=$r$c$f3%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);

		*XLText += 1;
		XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.0f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);

		XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQMax[0], pData->m_szTonf_M2);
		if(!bOkSupportForce) 
			XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
		else 
			XLText->AddFormatText("$e$a%s$n", szOkNg);
	}
}

void CARcBridgeCalcFooting::WriteCalcSupport_Lsd( CXLFormatText *XLText, long nJijum, long &nIndexSmall, BOOL bEarthquake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bEarthquake && pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CLsdFootingDesign* pFootingDesign = GetLsdFootingDesign(0);
	if(pFootingDesign == NULL) return;

	//���� ���� ���
	double dUltFactor(0), dExtFactor(0), dSerFactor(0);		

	dUltFactor = pFootingDesign->sCoffSF.supporResitCoff[pFootingDesign->m_ixTypeGround][eLSD_COMBINATION_LIMIT];
	dExtFactor = pFootingDesign->sCoffSF.supporResitCoff[pFootingDesign->m_ixTypeGround][eLSD_COMBINATION_EXTREME];
	dSerFactor = pFootingDesign->sCoffSF.supporResitCoff[pFootingDesign->m_ixTypeGround][eLSD_COMBINATION_SERVICE];

	XLText->AddFormatText("$h%d) ������ ��뿬��������$n$n", nIndexSmall++);
	pFootingDesign->WritelSupportForceCheck(XLText, COL_POS_1);

	BOOL bUserInput = !pFootingDesign->m_pFooting->m_bAutoCalcSupportForce;

	for(long iEnv=0; iEnv<eCountEnvTypeLsd-1; ++iEnv)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)iEnv;
		CString szTitle = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		long nCombo = eLSD_COMBINATION_LIMIT;
		double dFactor(0);
		if(eEnvType == eEnvLimit)
		{
			dFactor = dUltFactor;
			nCombo = 1;
		}
		else if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
		{
			dFactor = dExtFactor;
			nCombo = 2;
		}
		else if(eEnvType == eEnvService)
		{
			dFactor = dSerFactor;
			nCombo = 0;
		}

		double dQa = bUserInput ? pFootingDesign->sCoffSF.Qa_user[nCombo] : m_dQaEquation * dFactor;
		BOOL bOkSupportForce = dQa >= m_dQMax[iEnv] ? TRUE : FALSE;
		CString szComp2 = (bOkSupportForce)? ">" : "<";
		CString szOkNg = (bOkSupportForce)? "O.K" : "N.G";

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$e- %s$n", szTitle);
		if(bUserInput)
		{
			XLText->AddFormatText("$h$eQa$r$c=$r$e%s %s$m+09$c%s$m+02$eQmax = %s %s$m+07$c��$r", 
				pData->GetStrDouble(dQa, UNIT_CGS_TONF_M2), pData->m_szTonf_M2, szComp2, pData->GetStrDouble(m_dQMax[iEnv], UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$h$eQa$r$c=$r$e%s x %.2f$m+04$c=$r$e%s %s$m+05$c%s$m+02$eQmax = %s %s$m+07$c��$r", 
				pData->GetStrDouble(m_dQaEquation, UNIT_CGS_TONF_M2), dFactor, pData->GetStrDouble(dQa, UNIT_CGS_TONF_M2), pData->m_szTonf_M2, szComp2, pData->GetStrDouble(m_dQMax[iEnv], UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		}

		if(!bOkSupportForce) 
			XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
		else 
			XLText->AddFormatText("$e$a%s$n", szOkNg);
	}
	
	if(bUserInput == FALSE)
	{
		CString str(_T(""));
		XLText->AddText(hggettext("$h$r-  ��뿬��������(Qa) = ��b x Qu$n"));
		str.Format(hggettext(" ���װ��(��b) : ����Ѱ����(%.2f), �����Ѱ����(%.2f), �شܻ�Ȳ�Ѱ����(%.2f)"), dSerFactor,dUltFactor ,dExtFactor );
		XLText->AddFormatText(_T("$h$r%s$n"), str);

		str.Format(hggettext(" Qu : ������ ���� ������"));
		XLText->AddFormatText(_T("$h$r%s$n"), str);
	}
}

void CARcBridgeCalcFooting::WriteSectionForceBySubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum, HGBOOL bFront, long n)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	std::vector<CLoadFooting *> vecTriangle;
	std::vector<CLoadFooting *> vecBoth;

	CString strCombTriangle = "COMB ";
	CString strBoth = "COMB ";

	HGINT32 ixCombo = 1;
	CLoadFooting *pLoad = NULL;
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
			continue;

		for(HGINT32 jx = 0; jx < eCountFootCheck; ++jx)
		{
			pLoad = GetLoadFootingLsd(nJijum, (ETypeEnvComboLsd)ix, jx);
			if(pLoad == NULL)
				continue;

			double dQ1 = GetValueQ1(pLoad);
			double dQ2 = GetValueQ2(pLoad);
			if((dQ1 > dQ2) == bFront || pLoad->m_bQTriangle == FALSE)
			{
				// �ﰢ���� ��ٸ����� ���� ����� ���°��
				// ��ٸ����϶��� ������� ũ�� ����� ����.
				vecBoth.push_back(pLoad);
				strBoth += COMMA(ixCombo) + ",";
			}
			else if (pLoad->m_bQTriangle)
			{
				vecTriangle.push_back(pLoad);
				strCombTriangle += COMMA(ixCombo) + ",";
			}
			
			++ixCombo;
		}
	}

	strCombTriangle = strCombTriangle.Mid(0,strCombTriangle.GetLength()-1);
	strBoth = strBoth.Mid(0,strBoth.GetLength()-1);

	HGINT32 ixTitle = 0;
	CString arrTitle[] = {"i)", "ii)", "iii)"};

	XLText->AddFormatText("$s+07$h$c-$r$e���ݹݷ� ���� �ܸ��$n");
	if (bFront)
	{
		if (vecTriangle.empty() == FALSE)
		{
			pLoad = vecTriangle.at(0);
			double dL = pLoad->m_pFooting->GetWidth() - pLoad->m_dDistSf;

			XLText->AddFormatText("$h$r$c%s$r$eQ1 < Q2, �ﰢ�� �ݷ� ������ : %s$n", arrTitle[ixTitle++], strCombTriangle);

			XLText->AddFormatText("$h$r$iVq$r$c=$r$eQs / 2 x (��-%s)$n", pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQm / 2 x (��-%s)��/ 3$n", pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddText("$h$r���⼭, �ݷ��� �ۿ��� �� = 3[B/2 - e]$n$h");
		}

		if (vecBoth.empty() == FALSE)
		{
			pLoad = vecBoth.at(0);
			double dDistMftemp = (n==0)? pLoad->m_dDistMf : (n == 1 ? pLoad->m_dDistMf2 : pLoad->m_dDistMf3); 
			XLText->AddFormatText("$h$r$c%s$r$eQ1 > Q2 �Ǵ� ��ٸ��� �ݷ� ������ : %s$n", arrTitle[ixTitle++], strBoth);

			XLText->AddFormatText("$h$r$iVq$r$c=$r$eQ1 x (%s) + (Qs-Q1)/2 x %s$n", pData->GetStrDouble(pLoad->m_dDistSf, UNIT_CGS_M), pData->GetStrDouble(pLoad->m_dDistSf, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQ1 x %s��/2 + (Qm-Q1) x %s��/2 x 2/3$n", pData->GetStrDouble(dDistMftemp, UNIT_CGS_M), pData->GetStrDouble(dDistMftemp, UNIT_CGS_M));
		}

		*XLText += 1;
	}
	else
	{
		if (vecBoth.empty() == FALSE)
		{
			pLoad = vecBoth.at(0);
			XLText->AddFormatText("$h$r$c%s$r$eQ1 < Q2 �Ǵ� ��ٸ��� �ݷ� ������ : %s$n", arrTitle[ixTitle++], strBoth);

			XLText->AddFormatText("$h$r$iVq$r$c=$r$eQ2 x (%s) + (Qs-Q2)/2 x %s$n", pData->GetStrDouble(pLoad->m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(pLoad->m_dDistSb, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQ2 x %s��/2 + (Qm-Q2) x %s��/2 x 2/3$n", pData->GetStrDouble(pLoad->m_dDistMb, UNIT_CGS_M), pData->GetStrDouble(pLoad->m_dDistMb, UNIT_CGS_M));
		}

		if (vecTriangle.empty() == FALSE)
		{
			pLoad = vecTriangle.at(0);
			XLText->AddFormatText("$h$r$c%s$r$eQ1 > Q2, �ﰢ�� �ݷ� ������ : %s$n", arrTitle[ixTitle++], strCombTriangle);

			XLText->AddFormatText("$h$r$iVq$r$c=$r$eQ2 x (%s) + (Qs-Q2)/2 x %s$n", pData->GetStrDouble(pLoad->m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(pLoad->m_dDistSb, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQ2 x %s��/2 + (Qm-Q2) x %s��/2 x 1/3$n", pData->GetStrDouble(pLoad->m_dDistMb, UNIT_CGS_M), pData->GetStrDouble(pLoad->m_dDistMb, UNIT_CGS_M));
		}

		*XLText += 1;
	}
}

HGDOUBLE CARcBridgeCalcFooting::GetValueQ2(CLoadFooting *pData) const
{
	if (pData->m_bQmaxFront)
	{
		return pData->m_dQmin;
	}
	
	return pData->m_dQmax;
}

HGDOUBLE CARcBridgeCalcFooting::GetValueQ1(CLoadFooting *pData) const
{
	if (pData->m_bQmaxFront)
	{
		return pData->m_dQmax;
	}
	
	return pData->m_dQmin;
}

void CARcBridgeCalcFooting::MakeTablePileForceAndDisp_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	//������ ���� ����
	//	double dDistPile = GetPileCtcSideUnitMeter();
	char *szTitle[] = { "�� ��", "Vi","Hi","Mi","��x(mm)","��z(mm)","��(rad)", "���"};
	long nColWidth[] = { 2, 3, 3, 3, 3, 3, 3, 3 };

	long nColTotal=0;
	long n = 0;
	for(n=0; n<8; n++) nColTotal += nColWidth[n];

	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->GoPosition(NO_MOVE,nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");
		pXLText->GoPosition(NO_MOVE,nPos);
		pXLText->AddTable(1 + nPileSu - 1, nColTotal-1, 1 ,2);

		// Title	
		for(n=0; n<8; n++)	
		{
			pXLText->AddFormatText("$me%d$c%s$m+%02d",nColWidth[n]-1,szTitle[n],nColWidth[n]);
		}
		pXLText->AddText("$n$h");		

		for(long jx=0; jx<nPileSu; ++jx) 
		{
			long nIdxOrg = jx;
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, jx, TRUE, nIdxOrg);
			if(pLoad == NULL) continue;
			
			double dHi = GetValueUnitChange(pLoad->m_dForceHori, UNIT_CGS_TONF, pData->m_szTonf);
			double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf);
			double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM);
			double dDispX = m_dArrDispX[nTypeCase].GetAt(nIdxOrg);
			double dDispZ = m_dArrDispZ[nTypeCase].GetAt(nIdxOrg);
			double dAlpha = m_dArrDispR[nTypeCase].GetAt(nIdxOrg);

			pXLText->AddFormatText("$h$me1$c%d$m+%02d", jx + 1, nColWidth[0]);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+%02d", dVi, nColWidth[1]);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+%02d", dHi, nColWidth[2]);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+%02d", dMi, nColWidth[3]);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+%02d", dDispX, nColWidth[4]);
			pXLText->AddFormatText("$me2$c$f3%.3f$m+%02d", dDispZ, nColWidth[5]);
			pXLText->AddFormatText("$me2$c$f6%.6f$m+%02d", dAlpha, nColWidth[6]);
			pXLText->AddFormatText("$me2$c%s$n", pLoad->m_sName); // ����
		}
		pXLText->AddText("$n");	
	}

	pXLText->AddText("$n");	
}

void CARcBridgeCalcFooting::MakeTablePileReactionCheck_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;
	double dDistPile = 0;

	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->GoPosition(NO_MOVE,nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_GANA), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");
		pXLText->AddTable(nPileSu, 20, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(max)"));
		szTitle.Add(_T("V(min)"));
		if(bPileASD)
		{
			szTitle.Add(_T("Ra(" + pData->m_szTonf + ")"));
			szTitle.Add(_T("Pa(" + pData->m_szTonf + ")"));
		}
		else
		{
			szTitle.Add(_T("Qr(" + pData->m_szTonf + ")"));
			szTitle.Add(_T("Qu(" + pData->m_szTonf + ")"));
		}
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		for(long nx=0; nx<7; nx++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(nx));	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			dDistPile = GetDistMaxPile(nx);
			double dVmax = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dVmin = 0;

			// LSD �϶� Ra, Pa�� ���� ����� �ִ� ���´�.
			double dRa = pCalcPile->m_Ra[nTypeCase];
			double dPa = pCalcPile->m_Pa[nTypeCase];
			
			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", GetStrDouble(dVmax, 0, TRUE));
			pXLText->AddFormatText("$me2$c%s$m+03", GetStrDouble(dVmin, 0, TRUE));
			pXLText->AddFormatText("$me2$c%s$m+03", GetStrDouble(dRa, 0, TRUE));
			pXLText->AddFormatText("$me2$c%s$m+03", GetStrDouble(dPa, 0, TRUE));
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (dVmax > dRa || (dVmin<0 && fabs(dVmin)>fabs(dPa)))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::WriteHyuntaPileCheckPM_Lsd( CXLFormatText *pXLText, long nPos, long nTypeCase )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	BOOL bExtreme = (nTypeCase == PILE_CASE_EXTREME) ? TRUE : FALSE;

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$h��) ���Һ� �ۿ�����$n");
	//////////////////////////////////////////////////////////////////////////
	CLongArray nArrJointID;
	char *szTitle[] = { "�� ��", "P", "M", "V", "e", "C.T.C", "���"};
	long nColWidth[] = { 3, 3, 3, 3, 3, 3, 3 };
	long nColTotal=0;
	for(long nx=0; nx<6; nx++) nColTotal += nColWidth[nx];

	long nCase = 1;
	
	pXLText->GoPosition(NO_MOVE, nPos+1);
	pXLText->AddTable(1+nPileSu*nCase-1,nColTotal-1,1,3);

	// Title	
	for(long nx=0; nx<6; nx++)	pXLText->AddFormatText("$me%d$c%s$m+%02d",nColWidth[nx]-1,szTitle[nx],nColWidth[nx]);
	pXLText->AddText("$n");		
	CString str = _T("");
	double dCaseMaxP[3] = {0,0,0}, dCaseMaxM[3] = {0,0,0}, dCaseMaxE[3] = {0,0,0};
	long nIndex = 0;

	for(long jx=0; jx<nPileSu; ++jx) 
	{
		CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, jx, TRUE);
		if(pLoad == NULL) continue;

		// CPileLoad�� ����ִ� ������� tonf m ������ 1mm�� ���� ������̴� dDistPile�� mm������ �����Ѵ�.
		double dDistPile = GetDistMaxPile(jx);
		double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF, pData->m_szTonf) * dDistPile;
		double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM) * dDistPile;
		double dHi = GetValueUnitChange(pLoad->m_dForceHori,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
		double dE = dMi/dVi;

		if(dCaseMaxP[0] < dVi) // ����ִ�
		{
			dCaseMaxP[0] = dVi;
			dCaseMaxP[1] = dMi;
			dCaseMaxP[2] = dHi;
		}
		if(dCaseMaxM[0] < dMi) // ���Ʈ�ִ�
		{
			dCaseMaxM[0] = dVi;
			dCaseMaxM[1] = dMi;
			dCaseMaxM[2] = dHi;
		}
		if(jx==0 || dCaseMaxE[1]/dCaseMaxE[1] < dE) // ����ִ�
		{
			dCaseMaxE[0] = dVi;
			dCaseMaxE[1] = dMi;
			dCaseMaxE[2] = dHi;
		}

		pXLText->AddFormatText("$h$me2$c%d$m+%02d", jx + 1, nColWidth[0]);
		pXLText->AddFormatText("$me2$c%s$m+%02d", GetStrDouble(dVi, 0, TRUE), nColWidth[1]);
		pXLText->AddFormatText("$me2$c%s$m+%02d", GetStrDouble(dMi, 0, TRUE), nColWidth[2]);
		pXLText->AddFormatText("$me2$c%s$m+%02d", GetStrDouble(dHi, 0, TRUE), nColWidth[2]);
		pXLText->AddFormatText("$me2$c%s$m+%02d", GetStrDouble(dE, 3, TRUE), nColWidth[3]);
		pXLText->AddFormatText("$me2$c%.0f$m+%02d", dDistPile, nColWidth[4]);
		pXLText->AddFormatText("$me2 $n");			
	}

	pXLText->AddText("$n");
	//////////////////////////////////////////////////////////////////////////
	// ��� ��� �غ�
	CConcBasic *pConc = new CConcBasic(DESIGN_TYPE_LSD, 1, 1);
	pConc->m_Fck = pPile->m_pHyunTa->m_dHP_Fck;
	pConc->m_Fy  = pPile->m_pHyunTa->m_dHP_Fy;
	pConc->m_nTypeUnit = pData->m_nTypeUnit;
	pConc->m_nTypeSection = SECTION_COLUMN;
	pConc->m_Ro = pPile->m_dDia/2;

	for(long ix=0; ix<pPile->m_pHyunTa->m_nHP_MainRebarDanSu; ix++)
	{
		pConc->m_RbT1C_dc[ix] = pPile->m_pHyunTa->m_dHP_MainRebarDist[ix];
		pConc->m_RbT1C_DIA[ix]= pPile->m_pHyunTa->m_dHP_MainRebarDia[ix];
		pConc->m_RbT1C_DIA[ix]= pPile->GetDiaRebarMain(1000,ix); // �ִ���Ʈ��ġ....
		pConc->m_RbT1C_RE[ix] = pPile->m_pHyunTa->m_dHP_MainRebarEa[ix];
	}
	pConc->m_SvDia = pPile->m_pHyunTa->m_dHP_RoundRebarDia[0];
	pConc->m_SvCtc = pPile->m_pHyunTa->m_dHP_RoundRebarCTC1[0];
	pConc->m_SvEa = pPile->m_pHyunTa->m_nHP_MainRebarDanSu * 2;
	pConc->m_sArrPM.RemoveAll();

	pConc->m_Pi_C = bExtreme ? pCalcPile->m_dPI_C_Extreme : pCalcPile->m_dPI_C;
	pConc->m_Pi_S = bExtreme ? pCalcPile->m_dPI_S_Extreme : pCalcPile->m_dPI_S;
	pConc->m_Pi_V = bExtreme ? pCalcPile->m_dPI_V_Extreme : pCalcPile->m_dPI_V;
	if(bExtreme && pCalcPile->m_nSelectPmin_Hyunta_Extreme == 1)
	{
		pConc->m_nLsdComboKind = 5; // eLSD_COMBINE_EXTREME_I
	}

	// �����׸�
	CColumnForce *pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxP[1]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxP[0]), pData->m_szTonf, UNIT_CGS_TONF);
	pForce->m_Vu[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxP[2]), pData->m_szTonf, UNIT_CGS_TONF);;
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("����ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));
	//
	pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxM[1]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxM[0]), pData->m_szTonf, UNIT_CGS_TONF);
	pForce->m_Vu[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxM[2]), pData->m_szTonf, UNIT_CGS_TONF);;
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("���Ʈ�ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));
	//
	pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxE[1]), pData->m_szTonfM, UNIT_CGS_TONFM);
	pForce->m_Pu				= GetValueUnitChange(fabs(dCaseMaxE[0]), pData->m_szTonf, UNIT_CGS_TONF);
	pForce->m_Vu[CALC_DIR_X]	= GetValueUnitChange(fabs(dCaseMaxE[2]), pData->m_szTonf, UNIT_CGS_TONF);;
	pConc->m_pArrForce.Add(pForce);
	pConc->m_sArrPM.Add(_T("����ִ�"));
	pConc->m_xyArrPM.Add(CDPoint(pForce->m_M2_s[CALC_DIR_X],pForce->m_Pu));
	//////////////////////////////////////////////////////////////////////////

	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$s+29$h��) ���� �ܸ���� : PM �����$n");

	CColumn pColumn(pConc, FALSE, pCalcPile->m_nSeclectPmin);	
	pColumn.CalculateAll(TRUE);
	// P-M�����
	double dScaleDim = 150;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim*3);

	CDrawPM DrawPm(m_pStd->m_pARoadOptionStd, FALSE);
	DrawPm.DrawPM(pDom, &pColumn, FALSE);
	long nSizeRow = 29;
	pXLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow, CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	// �ܸ����
	CString szTilte;
	CString szIndex[5] = { _T("��"), _T("��"), _T("��"), _T("��"), _T("��") };
	for(long ix=0; ix<pConc->m_pArrForce.GetSize(); ++ix)
	{
		pXLText->AddFormatText("$s+40");
		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$s+29$h%s) ���� �ܸ���� : %s$n", szIndex[ix], pConc->m_sArrPM.GetAt(ix));
		
		CColumn pColumn(pConc, FALSE, pCalcPile->m_nSeclectPmin, ix);				
		pColumn.MakeOutputExcel(pXLText, nPos+1, FALSE, TRUE, TRUE, TRUE);
	}
}

void CARcBridgeCalcFooting::MakeTablePileStressCheckPHC_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	double Ae  = frCm2(pPile->m_dAe_PHC);
	double Ze  = frCm3(pPile->m_dZe_PHC);
	double fce = GetValueUnitChange(pPile->m_dFce2_PHC, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

	double dFca[PILE_CASE_COUNT];
	double dFta[PILE_CASE_COUNT];
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		dFca[nTypeCase] = GetValueUnitChange(pCalcPile->m_PHC_Fca[nTypeCase], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
		dFta[nTypeCase] = GetValueUnitChange(pCalcPile->m_PHC_Fta[nTypeCase], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	}
	
	if(pPile->m_nType == PILE_TYPE_PHC)
	{
		pXLText->GoPosition(NO_MOVE, nPos-1);
		pXLText->AddText(_T("����Ÿ���� ��Ÿ�� ���� ���ҸӸ� �ļ��̳� ���������� �ұյ� ������ ���Ͽ� �ε��� ���ҸӸ���$n"));
		pXLText->AddText(_T("�����ؾ��ϴ� ��찡 �ִ�. �̶� PC������ ��쿡�� ���ܵ� ��ġ�κ��� ������������$n"));
		pXLText->AddText(_T("�̹� ���Ե� ������Ʈ������ �����ϰ� �ȴ�.    ����������. �ѱ����ݰ���ȸ �� �� 7.5.2.2$n"));
	}

	pXLText->GoPosition(NO_MOVE, nPos);
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;
		
		pXLText->AddFormatText("$h%s$m+06$c=$r%s %s (%s)$n", 
			nTypeCase==0? _T("��� �ھ������� fca") : _T(""), pData->GetStrDouble(dFca[nTypeCase], UNIT_CGS_KGF_CM2, FALSE), pData->m_szKgf_CM2, pCalcPile->GetStringCase(nTypeCase));
	}
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->AddFormatText("$h%s$m+06$c=$r%s %s (%s)$n", 
			nTypeCase==0? _T("��� ���������� fta") : _T(""), pData->GetStrDouble(dFta[nTypeCase], UNIT_CGS_KGF_CM2, FALSE), pData->m_szKgf_CM2, pCalcPile->GetStringCase(nTypeCase));
	}
	
	pXLText->AddFormatText("$h$cf$r$c=$rfce �� M/Ze + V/Ae$n");
	pXLText->AddFormatText("$h$r$r$c=$r%.3f �� M / %.1f + V / %.1f$n", fce, Ze, Ae);

	// Table ���
	CString str = _T("");
	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->GoPosition(NO_MOVE, MIN(nPos, COL_POS_2));	// ���̺��� �������� �Ѿ�� �ʵ��� ó��
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");	
		pXLText->AddTable(nPileSu, 23, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("M(" + pData->m_szTonfM + ")"));
		szTitle.Add(_T("fc"));
		szTitle.Add(_T("fca"));
		szTitle.Add(_T("ft"));
		szTitle.Add(_T("fta"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		long dWidth[9] = { 2, 3, 3, 3, 3, 3, 3, 2, 2 };

		for(long nx=0; nx<9; ++nx)	
			pXLText->AddFormatText("$me%d$c%s$m+0%d", dWidth[nx]-1,szTitle.GetAt(nx), dWidth[nx]);	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			// (ARCBRIDGE-2937) ������������� ��������� �������ΰ���
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			// CPileLoad�� ����ִ� ������� tonf m ������ 1m�� ���� ������̴� dDistPile�� m������ �����Ѵ�.
			double dDistPile = GetDistMaxPile(nx);
			double dVi = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dMi = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM) * dDistPile;
			// ���⼭�� ������ mm�� ����� ������.

			//double ft   = fce - dMi*1000000/Ze + dVi*1000/Ae;
			//double fc   = fce + dMi*1000000/Ze + dVi*1000/Ae;
			double ft   = fce - dMi/Ze + dVi/Ae;
			double fc   = fce + dMi/Ze + dVi/Ae;
			double fca  = dFca[nTypeCase];
			double fta  = dFta[nTypeCase];

			pXLText->AddFormatText("$h$me1$c$f0%d$m+02", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dVi, UNIT_CGS_TONF, FALSE));
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dMi, UNIT_CGS_TONFM, FALSE));
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", fc);
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", fca);
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", ft);
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", fta);
			pXLText->AddFormatText("$me1$c$f0%.0f$m+02", dDistPile);
			BOOL bOK = (fabs(fc) > fabs(fca) || (ft<0 && ft<fta))? FALSE : TRUE;
			pXLText->AddFormatText("$me1$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::MakeTablePileStressCheck_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	double dDia = pPile->m_dDia;
	double I33	= toM4(frCm4(pCalcPile->m_I33));
	double yIp  = toM(pPile->m_dDia)/2.0/I33;
	double Ap   = toM2(frCm2(pCalcPile->m_Ap));

	double dFsa[PILE_CASE_COUNT];
	dFsa[0] = pCalcPile->GetFsa(FALSE);
	dFsa[1] = pCalcPile->GetFsa(TRUE);

	pXLText->GoPosition(NO_MOVE, nPos);
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->AddFormatText("$h$cfsa$r$c=$r%.0f %s (%s)$n", dFsa[nTypeCase == PILE_CASE_EXTREME? 1 : 0], pData->m_szKgf_CM2, pCalcPile->GetStringCase(nTypeCase));
	}
	
	pXLText->AddFormatText("$h$cf$r$c=$rV/Ap �� M x y/Io$n");
	pXLText->AddFormatText("$h$r$c=$rV/%s �� M x %.0f/2 / %s$n", pData->GetStrDoubleUnitChange(Ap, UNIT_CGS_M2, pData->m_szM2), dDia, pData->GetStrDoubleUnitChange(I33, UNIT_CGS_M4, pData->m_szM4));

	// Table ���
	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");
		pXLText->AddTable(nPileSu, 20, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("V(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("M(" + pData->m_szTonfM + ")"));
		szTitle.Add(_T("fmax"));
		szTitle.Add(_T("fsa"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));


		for(long nx =0; nx<7; nx++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(nx));	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			// (ARCBRIDGE-2937) ������������� ��������� �������ΰ���
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			double dDistPile = GetDistMaxPile(nx);
			double dVi = pLoad->m_dForceVert * dDistPile;
			double dMi = pLoad->m_dMoment * dDistPile;

			double fmax = GetValueUnitChange((dVi/Ap + dMi*yIp)/10,UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			double fsa  = GetValueUnitChange(pCalcPile->m_dArrFsa[nTypeCase].GetAt(nx),UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDoubleUnitChange(dVi,UNIT_CGS_TONF, pData->m_szTonf, FALSE));
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDoubleUnitChange(dMi,UNIT_CGS_TONFM, pData->m_szTonfM, FALSE));
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", fmax);
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", fsa);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (fabs(fmax) > fabs(fsa))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::MakeTablePileShearStressCheck_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	double D		= pPile->m_dDia;
	double T1	= pPile->m_dT1;
	double T2	= pPile->m_dT2;
	double D1	= D-T2*2;
	double d		= D-T1*2;
	double Ap	= pCalcPile->m_Ap;
	double alpha	= 4*(D1*D1+D1*d+d*d) / (3*(D1*D1+d*d));

	double dTsa[2];
	dTsa[0] = pCalcPile->GetTsa(FALSE);
	dTsa[1] = pCalcPile->GetTsa(TRUE);

	pXLText->GoPosition(NO_MOVE, nPos);
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->AddFormatText("$h$me1��sa$m+02$c=$r%s %s (%s)$n", pData->GetStrDouble(dTsa[nTypeCase == PILE_CASE_EXTREME? 1 : 0], UNIT_CGS_KGF_CM2, FALSE), pData->m_szKgf_CM2, pCalcPile->GetStringCase(nTypeCase));
	}
	
	pXLText->AddFormatText("$h$me1$c��$m+02$c=$r�� x Ho / Ap$n");
	pXLText->AddFormatText("$h$me1$c��$m+02$c=$r4 x (D��+ D��d + d��) / (3 x (D��+ d��))$n");
	if(pData->IsSI())
		pXLText->AddFormatText("$h$m+02$c=$r4 x (%.1f��+ %.1f��%.1f + %.1f��) / (3 x (%.1f��+ %.1f��))$n", D1, D1, d, d, D1, d);
	else 
		pXLText->AddFormatText("$h$m+02$c=$r4 x (%.3f��+ %.3f��%.3f + %.3f��) / (3 x (%.3f��+ %.3f��))$n", D1, D1, d, d, D1, d);
	pXLText->AddFormatText("$h$m+02$c=$r%.3f : ����������$n", alpha);

	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;

		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");
		pXLText->AddTable(nPileSu, 17, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("H(" + pData->m_szTonf + ")"));
		szTitle.Add(_T("��"));
		szTitle.Add(_T("��sa"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		for(long nx=0; nx<6; nx++)	pXLText->AddFormatText("$me2$c%s$m+03",szTitle.GetAt(nx));	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			// (ARCBRIDGE-2937) ���� �������°��䵵 �������� ��������.
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			double dDistPile = GetDistMaxPile(nx);
			double Ho = pLoad->m_dForceHori * dDistPile;
			double Tmax = GetValueUnitChange((alpha * frTon(Ho)) / Ap, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);	// tonf/m2 -> kgf/cm2
			double Tsa	= GetValueUnitChange(pCalcPile->m_dArrTsa[nTypeCase].GetAt(nx),UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

			pXLText->AddFormatText("$h$me2$c$f0%d$m+03", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDoubleUnitChange(Ho,UNIT_CGS_TONF, pData->m_szTonf, FALSE));
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", Tmax);
			pXLText->AddFormatText("$me2$c$f1%.1f$m+03", Tsa);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (fabs(Tmax) > fabs(Tsa))? FALSE : TRUE;
			pXLText->AddFormatText("$me2$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::WritePileCheckMain_Lsd( CXLFormatText *pXLText, long nPos )
{
	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText("$s+40$h%s$r������ ��ü ����$n", GetNumberString(2, 0));

	// ��) ������ ��¿� ���� ����
	WritePileOutputSafeCheck_AxialForce_Lsd(pXLText, nPos+1);
	// ��) ���� ������ ���� ���� ����
	WritePileOutputSafeCheck_Fa_Lsd(pXLText, nPos+1);
	// ��) ������ �������� ����
	WritePileOutputSafeCheck_ShaerStress_Lsd(pXLText, nPos+1);
}

void CARcBridgeCalcFooting::WritePileOutputSafeCheck_AxialForce_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;

	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;
	double lamda = 0;
	double dFy  = GetValueUnitChange(pPile->m_dYieldStress_SteelPile, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	double dAp = frCm2(pCalcPile->m_Ap);
	double dPn1 = pCalcPile->GetLsdAxialPn();

	CString strBook = pData->m_nTypeCheckSteelPile == 0 ? _T("[��.��(2015) 6.5.4.2, 6.9.2.1, 6.9.4.1]") : _T("[��.��(2014) 5.3]");
	double dFactor = pData->m_nTypeCheckSteelPile == 0 ? LSD_CHECK_AXIAL_COEF : 0.658;
	long nDec23 = pData->m_nTypeCheckSteelPile == 0 ? 2 : 3;
	pXLText->GoPosition(NO_MOVE, nPos);
	pXLText->AddFormatText(hggettext("��) ������ ��¿� ���� ����    %s$n"), strBook);

	strBook = pData->m_nTypeCheckSteelPile == 0 ? _T("��") : _T("(Fy/Fe)");
	pXLText->AddFormatText(hggettext("Pn = %s^%s��Fy��As = %s^%s x %s x %s = %s %s$n"), 
		GetStrDouble(dFactor, nDec23, TRUE), strBook, GetStrDouble(dFactor, nDec23, TRUE),  GetStrDouble(lamda, 0, TRUE),
		GetStrDouble(dFy, 0, TRUE), GetStrDouble(dAp, 0, TRUE), GetStrDouble(dPn1, 0, TRUE), pData->m_szTonf);	
	pXLText->AddText(hggettext("Pr = ��c��Pn > Pu$n$n"));

	// ���̺� ���
	CString str = _T("");
	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;
		if(!bPileASD && nTypeCase == PILE_CASE_USE)
			continue;

		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");	
		pXLText->AddTable(nPileSu, 18, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("Pu"));
		szTitle.Add(_T("Pn"));
		szTitle.Add(_T("��c"));
		szTitle.Add(_T("Pr"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		long dWidth[7] = { 2, 3, 3, 3, 3, 3, 2 };

		for(long nx=0; nx<7; ++nx)	
			pXLText->AddFormatText("$me%d$c%s$m+0%d", dWidth[nx]-1,szTitle.GetAt(nx), dWidth[nx]);	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			// CPileLoad�� ����ִ� ������� tonf m ������ 1m�� ���� ������̴� dDistPile�� m������ �����Ѵ�.
			double dDistPile = GetDistMaxPile(nx);
			double dPu = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dPn = pCalcPile->GetLsdAxialPn();
			double piC = pCalcPile->sLsdCoef.piC[nTypeCase];
			double dPr = pCalcPile->GetLsdAxialPr(nTypeCase);

			pXLText->AddFormatText("$h$me1$c$f0%d$m+02", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dPu, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dPn, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", piC);
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dPr, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (dPu > dPr)? FALSE : TRUE;
			pXLText->AddFormatText("$me1$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::WritePileOutputSafeCheck_Fa_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;
	
	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	pXLText->OutXL();
	long nRow = pXLText->GetRowPostion();
	nRow = pCalcPile->WriteTextLsdPileSafeCheck_Fa(pXLText->GetExcelControl(), nRow);
	pXLText->GoPosition(nRow,nPos);

	// ���̺� ���
	CString str = _T("");
	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;
		if(!bPileASD && nTypeCase == PILE_CASE_USE)
			continue;

		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");	
		pXLText->AddTable(nPileSu, 19, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("Pu"));
		szTitle.Add(_T("Pu/Pr"));
		szTitle.Add(_T("Mu"));
		szTitle.Add(_T("���"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		long dWidth[7] = { 2, 3, 3, 4, 3, 3, 2 };

		for(long nx=0; nx<7; ++nx)	
			pXLText->AddFormatText("$me%d$c%s$m+0%d", dWidth[nx]-1,szTitle.GetAt(nx), dWidth[nx]);	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			// (ARCBRIDGE-2937) ������������� ��������� �������ΰ���
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			// CPileLoad�� ����ִ� ������� tonf m ������ 1m�� ���� ������̴� dDistPile�� m������ �����Ѵ�.
			double dDistPile = GetDistMaxPile(nx);
			double dPu = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pData->m_szTonf) * dDistPile;
			double dMu = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pData->m_szTonfM) * dDistPile;
			double dPn = pCalcPile->GetLsdAxialPn();
			double piC = pCalcPile->sLsdCoef.piC[nTypeCase];
			double dPr = pCalcPile->GetLsdAxialPr(nTypeCase);
			double dMr = pCalcPile->GetLsdAxialMomentMr(nTypeCase);
			double dResult = 0;
			if(dPu / dPr < 0.2)
			{
				dResult = dPu / (2 * dPr) + dMu / dMr;
			}
			else
			{
				dResult = dPu / dPr + (8./9.) * dMu / dMr;
			}

			pXLText->AddFormatText("$h$me1$c$f0%d$m+02", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dPu, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dPu/dPr);
			pXLText->AddFormatText("$me3$c%s$m+04", pData->GetStrDouble(dMu, UNIT_CGS_TONFM));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dResult);
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (dResult > LSD_CHECK_AXIALMOMENT_COEF)? FALSE : TRUE;
			pXLText->AddFormatText("$me1$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::WritePileOutputSafeCheck_ShaerStress_Lsd( CXLFormatText *pXLText, long nPos )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = 0;
	CFootingApp *pFooting = pBri->GetFooting(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
	if(pCalcPile==NULL) return;

	long nPileSu = pPile->m_nCountOrg;
	if(nPileSu == 0) return;
	HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;

	pXLText->OutXL();
	long nRow = pXLText->GetRowPostion();
	nRow = pCalcPile->WriteTextLsdPileSafeCheck_ShaerStress(pXLText->GetExcelControl(), nRow, TRUE);
	pXLText->GoPosition(nRow,nPos);

	// ���̺� ���
	CString str = _T("");
	long nIndex = 0;
	for(long nTypeCase=0; nTypeCase<PILE_CASE_COUNT; ++nTypeCase)
	{
		if(pCalcPile->GetSizePileLoad(nTypeCase) == 0)
			continue;

		// (ARCBRIDGE-2604) ���Ը��� ����������� ����� �����Ѱ���� ��� ����.
		if(bPileASD && nTypeCase == PILE_CASE_ULTIMATE)
			continue;
		if(!bPileASD && nTypeCase == PILE_CASE_USE)
			continue;

		pXLText->GoPosition(NO_MOVE, nPos);
		pXLText->AddFormatText("$h$s+%02d$c%s$r%s$n",nPileSu + 2, GetNumberString(++nIndex, NUM_CIRCLE), pCalcPile->GetStringCase(nTypeCase));

		pXLText->AddFormatText("$h");	
		pXLText->AddTable(nPileSu, 18, 1, 0);
		CStringArray	szTitle;
		szTitle.Add(_T("�� ��"));
		szTitle.Add(_T("H (N)"));
		szTitle.Add(_T("Fcr"));
		szTitle.Add(_T("Vn"));
		szTitle.Add(_T("Vr"));
		szTitle.Add(_T("C.T.C"));
		szTitle.Add(_T("�� ��"));

		long dWidth[7] = { 2, 3, 3, 3, 3, 3, 2 };

		for(long nx=0; nx<7; ++nx)	
			pXLText->AddFormatText("$me%d$c%s$m+0%d", dWidth[nx]-1,szTitle.GetAt(nx), dWidth[nx]);	
		pXLText->AddText("$n");
		for(long nx=0; nx<nPileSu; ++nx)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypeCase, nx, TRUE);
			if(pLoad == NULL) continue;

			// CPileLoad�� ����ִ� ������� tonf m ������ 1m�� ���� ������̴� dDistPile�� m������ �����Ѵ�.
			double dDistPile = GetDistMaxPile(nx);
			double dH = GetValueUnitChange(pLoad->m_dForceHori * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
			double dFcr = pCalcPile->GetLsdShearFcr(nTypeCase, nx, TRUE);
			double dVn = pCalcPile->GetLsdShearVn(nTypeCase, nx, TRUE);
			double dVr = pCalcPile->GetLsdShearVr(nTypeCase, nx, TRUE);

			pXLText->AddFormatText("$h$me1$c$f0%d$m+02", nx+1);	
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dH, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c$f3%.3f$m+03", dFcr);
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dVn, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c%s$m+03", pData->GetStrDouble(dVr, UNIT_CGS_TONF));
			pXLText->AddFormatText("$me2$c$f0%.0f$m+03", dDistPile);
			BOOL bOK = (dH > dVr)? FALSE : TRUE;
			pXLText->AddFormatText("$me1$c%s$n", bOK ? "O.K" : "N.G");
		}
		pXLText->AddText("$n");
	}
}

void CARcBridgeCalcFooting::GetCheckValueHyuntaPileAxialForce( long nJijum, double *dPiPn, double *dAxial, BOOL bNormal )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL) return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	double dMmax(0), dDepth(0);
	if(pBri->IsBoxType() == FALSE)
	{
		long nIdxStr = bNormal ? 0 : 3;
		long nCase = 0;
		long nTypeCase = PILE_CASE_USE;
		if(bNormal == FALSE)
			nTypeCase = bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH;

		if(pCalcPile->m_sMatHyunta.GetRow() > nIdxStr)
		{
			*dAxial = Round(hgatof(pCalcPile->m_sMatHyunta(nIdxStr,  DESIGNPILE_HYUNTA_P)), 3);
			dMmax = hgatof(pCalcPile->m_sMatHyunta(nIdxStr,  DESIGNPILE_HYUNTA_M));
			dDepth = pPile->m_dH0 + hgatof(pCalcPile->m_sMatHyunta(nIdxStr+1, 6));
		}
		else
		{
			*dAxial = 0;
			dMmax = 0;
		}

		// *dAxial = pDataStd->GetValueUnitType(pCalcPile->GetMaxReaction(!bNormal, nCase, nTypeCase), UNIT_CGS_TONF, pDataStd->m_szTonf);
		// dMmax = pDataStd->GetValueUnitType(pCalcPile->GetMomentHyunta(nTypeCase, nCase, -1, FALSE), UNIT_CGS_TONFM, pDataStd->m_szTonfM);
	}
	else
	{
		long nJijum = 0;
		CFootingApp *pFooting = pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		CDesignPile *pCalcPile = m_pStd->GetDesignPile(nJijum);
		if(pCalcPile==NULL) return;

		long nPileSu = pPile->m_nCountOrg;
		if(nPileSu == 0) return;

		long nCase = pBri->IsApplyEarthQuakeDesign() ? (pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // ���,�ر�,���

		double dVmax = 0;
		for(long nT = 0; nT < nCase; nT++)
		{
			BOOL bNormalCase = (nT!=0) ? FALSE : TRUE;
			if(bNormal != bNormalCase) continue;

			for(long n=0; n<nPileSu; n++)
			{
				CPileLoad *pLoad = NULL; 
				if(nT == 0)
					pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, n, TRUE);
				else
				{
					if(bLsd)
						pLoad = pCalcPile->m_pListExtreme.GetAt(n);
					else
						pLoad = pCalcPile->m_pListUse.GetAt(nPileSu * nT + n);
				}

				double dDistPile = GetDistMaxPile(n);
				if(!bLsd)
					dDistPile = toM(dDistPile);

				double dV = GetValueUnitChange(pLoad->m_dForceVert,UNIT_CGS_TONF,pDataStd->m_szTonf) * dDistPile;
				double dM = GetValueUnitChange(pLoad->m_dMoment,UNIT_CGS_TONFM,pDataStd->m_szTonfM) * dDistPile;

				if(dV > dVmax)
				{
					dVmax = dV;
				}
				if(dM > dMmax)
				{
					dMmax = dM;
					dDepth = pCalcPile->GetPileDepthMaxMoment(n, 0, TRUE, bNormalCase? PILE_CASE_USE : PILE_CASE_EARTH);
				}
			}
		}
		*dAxial = Round(dVmax, 3);
	}

	CConcBasic *pConc = new CConcBasic;
	pCalcPile->SetConcDataHyunta(pConc, dDepth, !bNormal);
	
	CColumnForce *pForce = new CColumnForce;
	pForce->m_M2_s[CALC_DIR_X]		= fabs(dMmax);
	pForce->m_Pu					= fabs(*dAxial);
	pConc->m_pArrForce.Add(pForce);

	CColumn pColumn(pConc, FALSE, pCalcPile->m_nSeclectPmin, 0);
	pColumn.CalculateAll(FALSE, FALSE);

	*dPiPn = (bLsd) ? pColumn.GetPiPn() : GetValueUnitChange(fabs(pColumn.GetPiPn()), UNIT_CGS_TONF, pDataStd->m_szTonf);;
}

HGDOUBLE CARcBridgeCalcFooting::GetLengthBox()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);

	double dLength = pBri->m_dLengthBridge + pBri->m_dWS/vAngStt.y + pBri->m_dWE/vAngEnd.y;

	return dLength;
}
