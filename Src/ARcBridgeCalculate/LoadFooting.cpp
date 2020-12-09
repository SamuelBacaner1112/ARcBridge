// LoadFooting.cpp: implementation of the CLoadFooting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadFooting::CLoadFooting(CARcBridgeCalcStd *pStd, CRcBridgeRebar *pBri, long nJijum, long nIdex, BOOL bUseLoad, ETypeEnvComboLsd eEnvType)
{
	m_bEarthQuake = (nIdex%4 == 3)? TRUE:FALSE;			// �ӽ÷� ������.. ���߿� �ٽ� ������...
	m_bApplyHoriEarQ = pBri->m_bApplyFootingGForce;			// ������ �������߿� ���� ������ ���..
	m_pStd = pStd;
	m_pBri = pBri;
	m_nJijum = nJijum;
	m_nIndex = nIdex;
	m_bUseLoad = bUseLoad;
	m_pFooting = pBri->GetFootingByJijumNumber(nJijum);
	m_bPile = m_pFooting->m_exFooting.m_Pile.m_bIs;

	m_eEnvComboType = eEnvType;

	m_bQTriangle		= FALSE;
	m_dFactorDeadLoad	= 0;
	m_dFactorSoilHor	= 0;
	m_dFactorPave		= 0;
	m_dFactorLiveLoad	= 0;
	m_nCombo			= 0;
	m_dA				= 0;

	if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
	{
		pBri->GetStringFootingCheckCase((EFootingCheckLsd)nIdex);
		m_bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)? TRUE : FALSE;
	}
	else
	{
		switch(nIdex) 
		{
		case 0:
			m_sTitleSub = "����(��� �ִ�)";
			break;
		case 1:
			m_sTitleSub = "����(�θ��Ʈ �ִ�)";
			break;
		case 2:
			m_sTitleSub = "����(�����Ʈ �ִ�)";
			break;
		case 3:
			m_sTitleSub = "������";
			break;
		default:
			m_sTitleSub = "";
		}
	}

	m_dHightBackFill = 0;

	int i=0; for(i=0; i<VER_LOAD_COUNT; i++)
	{
		m_dVerLoad[i] = 0;
		m_dHorLoad[i] = 0;
		m_dXiVerLoad[i] = 0;
		m_dYiHorLoad[i] = 0;
		m_dMr[i] = 0;
		m_dMo[i] = 0;
	}

	m_dQmaxMass			= 0;

	m_dVerSmallTotal	= 0;			// �Ұ�
	m_dHorSmallTotal	= 0;
	m_dMrSmallTotal		= 0;
	m_dMoSmallTotal		= 0;
	
	m_dHightTosa		= 0;			// ������� ����
	
	m_dXiUpperLoadV		= 0;	
	m_dYiUpperLoadH		= 0;
	m_dXiUpperLoadV_Add		= 0;	
	m_dYiUpperLoadH_Add		= 0;
	
	// Mv, Mh : ������ �Ǵ� ����¿� ���� ���Ʈ
	m_dUpperLoadV	= 0;			// ����������� ������
	m_dUpperLoadH	= 0;			// ����������� ������
	m_dUpperLoadM	= 0;			// ����������� ���Ʈ

	m_dMrUpperV	= 0;
	m_dMoUpperV	= 0;
	m_dMrUpperH	= 0;
	m_dMoUpperH	= 0;
	m_dMrUpperM	= 0;
	m_dMoUpperM	= 0;

	m_bTwoUpperLoad		= FALSE;
	m_dUpperLoadV_Add	= 0;			// ����������� ������
	m_dUpperLoadH_Add	= 0;			// ����������� ������
	m_dUpperLoadM_Add	= 0;			// ����������� ���Ʈ
	m_nCombo_Add		= 0;
	
	m_dMrUpperV_Add	= 0;
	m_dMoUpperV_Add	= 0;
	m_dMrUpperH_Add	= 0;
	m_dMoUpperH_Add	= 0;
	m_dMrUpperM_Add	= 0;
	m_dMoUpperM_Add	= 0;
	
	// �Ѱ�
	m_dVerTotal			= 0;
	m_dHorTotal			= 0;
	m_dMoTotal			= 0;
	m_dMrTotal			= 0;

	m_dDistCenttoDosim	= 0;
	m_dPileLoadV		= 0;
	m_dPileLoadH		= 0;
	m_dPileLoadM		= 0;
	m_dPileLoadMCen		= 0;

	m_dE				= 0;
	m_dLx				= 0;
	m_dQmax				= 0;
	m_dQmin				= 0;
	m_dQa				= 0;
	m_dHu				= 0;					// ��� �����
	m_bOkSupportForce	= FALSE;
	m_bOkSliding		= FALSE;
	m_bOkOverturning	= FALSE;
	// �ձ�,�ޱ��� �ۿ�����(���� �ܸ��)
	m_dQmF				= 0;
	m_dQmF2				= 0;
	m_dQmB				= 0;
	m_dQvF				= 0;
	m_dQvB				= 0;

	m_dDistMf			= 0;
	m_dDistMb			= 0;
	m_dDistMf2			= 0;
	m_dDistSf			= 0;				// ���������κ����� �ձ� ���ܸ� �Ÿ�
	m_dDistSb			= 0;				// ���������κ����� �ޱ� ���ܸ� �Ÿ�
	m_bOutBoundFootFront	= 0;
	m_bOutBoundFootBack		= 0;
	m_dHgtSf			= 0;

	m_dVuFront			= 0;
	m_dVuBack			= 0;
	m_dMuFront			= 0;
	m_dMuBack			= 0;

	// �������
	m_dQaEquation		= 0;
	m_dA_Equation		= 0;

	m_sCombo = _T("");
}

CLoadFooting::~CLoadFooting()
{

}

void CLoadFooting::CalculateAll()
{
}

void CLoadFooting::CalculateBaseLoad()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;
 
	BOOL bLsd = pData->IsLsdDesign();
	BOOL bTrap = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP);
	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
	BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwAscon = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwBackFill = pData->GetValueUnitType(m_pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dHeightPave = toM(m_pBri->m_dUnderGroundHeightPavement);
	if(pData->IsRailDesign())
	{
		dHeightPave = 0;
	}

	if( m_nCombo < 0 ) return;

	if(bLsd)
	{
		// BOOL bStt = (m_nJijum == m_pBri->GetCountJijum()-1)? FALSE:TRUE;
		// (���õ� ���տ��� B, L, R �� ���� ������ ���� ������ ã�´�.
		BOOL bStt =(m_sCombo.Find(_T("R")) != -1) ? FALSE : TRUE;
		long nFind  = m_sCombo.Find(_T("R"));
		m_dFactorDeadLoad	= m_pBri->m_plsdManager->GetLoadFactor(m_eEnvComboType, m_sCombo, eLSD_LOADCASE_DC1);
		m_dFactorSoilHor	= m_pBri->m_plsdManager->GetLoadFactor(m_eEnvComboType, m_sCombo, eLSD_LOADCASE_EV);
		m_dFactorPave		= m_pBri->m_plsdManager->GetLoadFactor(m_eEnvComboType, m_sCombo, eLSD_LOADCASE_DW1);
		m_dFactorLiveLoad	= m_pBri->m_plsdManager->GetLoadFactor(m_eEnvComboType, m_sCombo, bStt? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2);
	}
	else
	{
		if(!m_bUseLoad)
		{
			long nTypeCombo = m_bEarthQuake ? LOAD_COMBINE_EQ : LOAD_COMBINE_ULT;
			long nCombo = m_nCombo;
			if(m_bEarthQuake == FALSE)
			{
				nCombo = m_pBri->m_lArrComboUlt.GetAt(m_nCombo);
			}

			m_dFactorDeadLoad = m_pBri->GetLoadFactor(nTypeCombo, nCombo, BRIDGE_LOAD_DEAD);		// �������� ���
			m_dFactorSoilHor = (m_pBri->m_bApplyLoad[BRIDGE_LOAD_SOIL])? m_pBri->GetLoadFactor(nTypeCombo, nCombo, BRIDGE_LOAD_SOIL) : m_dFactorDeadLoad;
			double FactorL1 = m_pBri->GetLoadFactor(nTypeCombo, nCombo, BRIDGE_LOAD_LIVE_FULL);
			double FactorL2 = m_pBri->GetLoadFactor(nTypeCombo, nCombo, BRIDGE_LOAD_LIVE_ECC);
			m_dFactorLiveLoad = MAX(FactorL1, FactorL2);
		}
		else
		{
			long nCombo = m_pBri->m_lArrComboUse.GetAt(m_nCombo);
			m_dFactorDeadLoad = m_pBri->GetLoadFactor(LOAD_COMBINE_USE, nCombo, BRIDGE_LOAD_DEAD);		// �������� ���
			m_dFactorSoilHor  = m_dFactorDeadLoad;
			m_dFactorLiveLoad = m_pBri->GetLoadFactor(LOAD_COMBINE_USE, nCombo, BRIDGE_LOAD_LIVE_FULL);
		}

		m_dFactorPave = m_dFactorSoilHor;
	}

	if((bTrap && (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
		||(bTrap2 && (m_nJijum==0 || m_nJijum==m_pBri->m_nQtyJigan)) || bArch)
	{
		double dA = 0;
		if(bTrap)
		{
			// (ARCBRIDGE-1559) FH �� FSH�� �ٲ����... FSH�� �Ʒ��� ���̶��.. ������ �־ ������ ���ϰ�...
			// FSW�� �������� ���̳�... �Ѥ�
			double dW = toM(m_pFooting->GetWidth());
			//	double dFH = toM(m_pFooting->m_dFH);
			//	double dFSH = toM(m_pFooting->GetHeight(FALSE));
			double dFSH = toM(m_pFooting->m_dFH);
			double dFH = toM(m_pFooting->GetHeight(FALSE));
			double dFEW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFERW) : toM(m_pFooting->m_dFELW);
			double dFSW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFSRW) : toM(m_pFooting->m_dFSLW);
			dFSW = dW - dFSW;

			dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
		}
		else
		{
			double dW = toM(m_pFooting->GetWidth());
			double dFH = toM(m_pFooting->GetHeight(FALSE));
			double dFSH = toM((m_nJijum==0)? m_pFooting->m_dFSRH : m_pFooting->m_dFSLH);
			double dFEW = toM((m_nJijum==0)? m_pFooting->m_dFERW : m_pFooting->m_dFELW);
			double dFSW = toM((m_nJijum==0)? m_pFooting->m_dFSRW : m_pFooting->m_dFSLW);
			
			dA = (dW * dFH) + ((dW - dFSW - dFEW) * dFSH) + (dFSW * dFSH /2);
		}

		m_dVerLoad[VER_LOAD_FOOTW] = GetValueUnitChange(dA, UNIT_CGS_M2, pData->m_szM2) * dUwConc;
	}
	else m_dVerLoad[VER_LOAD_FOOTW] = GetValueUnitChange(toM(m_pFooting->GetWidth()), UNIT_CGS_M, pData->m_szM)  * GetValueUnitChange(toM(m_pFooting->GetHeight()), UNIT_CGS_M, pData->m_szM) * dUwConc;

	if(m_nJijum==0 || m_nJijum==m_pBri->GetCountJijum()-1)
	{
		double dWidth = 0;
		double dElBackFill =0;
	
		CDPoint xyStt(0, 0);
		xyStt.x	= m_pBri->GetStationOnJijum(m_nJijum, 0);

		CWallApp Wall;
		m_pBri->MakeWallByOutWall(Wall, m_nJijum==0, FALSE);

		dWidth = (m_nJijum == 0)? m_pFooting->m_dFWL:m_pFooting->m_dFWR;

		if(m_pBri->m_nSelectModelingHeight==0)
		{
			dElBackFill = m_pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y+Wall.m_dELBDiff;
		}
		else
		{
			dElBackFill  = m_pBri->GetElMaxSlabUpper(xyStt.x, m_pBri->GetAngleJijum(m_nJijum)) + m_pBri->GetThickPave(FALSE);
		}

		if(m_pBri->m_bUnderGroundRahmen) dElBackFill += m_pBri->m_dUnderGroundDesignHeight;

		if(m_pBri->m_bUnderGroundRahmen)
		{
			double dUwPave = (m_pBri->m_nTypePavement==0)? dUwAscon : dUwConc;
			// ���� .. �ƽ��� + �����Ż
			if(m_pBri->m_bUnderGroundPaveDetail)
			{
				double dTickPave = 0;
				if(dElBackFill > m_pFooting->m_dEL)	// ǥ��
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightPyo < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightPyo;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightPyo), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// ����
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightGee < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightGee;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightGee), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// �߰���
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightMiddle < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightMiddle;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightMiddle), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// ��������
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightBozo < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightBozo;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightBozo), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ��������
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// ���������
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightDong < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightDong;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightDong), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// EPS��
				{
					if(dElBackFill - m_pBri->m_dUnderGroundPaveHeightEps < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->m_dUnderGroundPaveHeightEps;
						
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightEps), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// EPS��
					dElBackFill -= dTickPave;
				}
				if(dElBackFill > m_pFooting->m_dEL)	// ���
				{
					if(dElBackFill - m_pBri->GetUnderGroundPaveHeightTosa() < m_pFooting->m_dEL) dTickPave = (dElBackFill - m_pFooting->m_dEL);
					else dTickPave = m_pBri->GetUnderGroundPaveHeightTosa();
					
					m_dVerLoad[VER_LOAD_PAVE] += GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(dTickPave), UNIT_CGS_M, pData->m_szM) *
						pData->GetValueUnitType(toTon(m_pBri->m_dUnderGroundPaveWeightTosa), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// EPS��
					dElBackFill -= dTickPave;
				}
			}
			else
			{
				m_dVerLoad[VER_LOAD_PAVE] = GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) *
					(GetValueUnitChange(dHeightPave, UNIT_CGS_M, pData->m_szM) * dUwPave +
					GetValueUnitChange(toM(m_pBri->m_dUnderGroundHeightMortar), UNIT_CGS_M, pData->m_szM) * pData->GetValueUnitType(toTon(m_pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement), UNIT_CGS_TONF_M3, pData->m_szTonf_M3));
				dElBackFill -= (frM(dHeightPave) + m_pBri->m_dUnderGroundHeightMortar);
			}
			m_dVerLoad[VER_LOAD_LIVE] = (!m_bEarthQuake && m_dFactorLiveLoad>0) ? GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2):0;
			m_dVerLoad[VER_LOAD_DEAD] = GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * pData->GetValueUnitType(m_pBri->m_dDeadLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		}
		else
		{
			m_dVerLoad[VER_LOAD_PAVE]	= GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * GetValueUnitChange(toM(m_pBri->m_dTP), UNIT_CGS_M, pData->m_szM) * dUwAscon;
			m_dVerLoad[VER_LOAD_LIVE] = (!m_bEarthQuake && m_dFactorLiveLoad>0)? GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2):0;
			m_dVerLoad[VER_LOAD_DEAD] = GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * pData->GetValueUnitType(m_pBri->m_dDeadLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		}

		m_dHightBackFill = bLsd? dElBackFill-m_pFooting->m_dEL : toM(dElBackFill-m_pFooting->m_dEL);
		m_dVerLoad[VER_LOAD_BACKFILL] = GetValueUnitChange(toM(dWidth), UNIT_CGS_M, pData->m_szM) * m_dHightBackFill * dUwBackFill;
	}
	else
	{
		m_dVerLoad[VER_LOAD_BACKFILL]	= 0;
		m_dVerLoad[VER_LOAD_PAVE]	= 0;
		m_dVerLoad[VER_LOAD_LIVE]	= 0;
		m_dVerLoad[VER_LOAD_DEAD]	= 0;
	}

	if(!m_bUseLoad)
	{
		m_dVerLoad[VER_LOAD_LIVE]     = m_dVerLoad[VER_LOAD_LIVE] * m_dFactorLiveLoad;
		m_dVerLoad[VER_LOAD_DEAD]     = m_dVerLoad[VER_LOAD_DEAD] * m_dFactorDeadLoad;
		m_dVerLoad[VER_LOAD_FOOTW]    = m_dVerLoad[VER_LOAD_FOOTW] * m_dFactorDeadLoad;
		m_dVerLoad[VER_LOAD_PAVE]     =  m_dVerLoad[VER_LOAD_PAVE] * m_dFactorPave;
		m_dVerLoad[VER_LOAD_BACKFILL] = m_dVerLoad[VER_LOAD_BACKFILL] * m_dFactorSoilHor;
	}
	
	if(m_bEarthQuake && m_bApplyHoriEarQ)
	{
		BOOL bEnd = (m_nJijum == m_pBri->m_nQtyJigan)? TRUE : FALSE;
		m_dA = m_pStd->GetKhValue(FALSE, !bEnd, eGoalEQ_None);		// ��縻���� �ִ����� �Է��ؾ��ϳ� �ϴ� FALSE�� �Է�
	}
	m_dVerSmallTotal = m_dVerLoad[VER_LOAD_FOOTW] + m_dVerLoad[VER_LOAD_BACKFILL] + m_dVerLoad[VER_LOAD_PAVE] + m_dVerLoad[VER_LOAD_LIVE] + m_dVerLoad[VER_LOAD_DEAD];
}

void CLoadFooting::CalculateApplyLoad()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	m_bApplyHoriEarQ = m_pStd->m_pBri->m_bApplyFootingGForce;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bStt = (m_nJijum == m_pBri->GetCountJijum()-1)? FALSE:TRUE;
	BOOL bTrap = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP);
	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
	BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	int iDir = (m_nJijum == m_pBri->GetCountJijum()-1)? 1:-1;
	// (ARCBRIDGE-1532) ������1�� ������ �ι�° ������ �ݴ�� ��������� �Ѵ�.
	if(((bTrap || bTrap2) && m_nJijum==m_pBri->m_nQtyJigan-1))
		iDir  = 1;
	if(bArch && m_nJijum>m_pBri->m_nQtyJigan)
	{
		iDir = static_cast<int>((m_nJijum == m_pBri->m_nQtyJigan+1)? -1.0 : 1.0);
	}

	double dTickWallhalf = m_pBri->GetThickWall(m_nJijum)/2;
	double dWidthFront	 = (m_nJijum == m_pBri->GetCountJijum()-1)? m_pFooting->m_dFWL:m_pFooting->m_dFWR;
	double dWidthBack	 = (m_nJijum == m_pBri->GetCountJijum()-1)? m_pFooting->m_dFWR:m_pFooting->m_dFWL;

	CDPoint xyBase(0, 0);
	xyBase.x = m_pFooting->m_xyBottomCenter.x - m_pFooting->GetWidth()/2*iDir;
	xyBase.y = m_pFooting->m_xyBottomCenter.y;

	// Xi, Yi ����
	m_dXiVerLoad[VER_LOAD_LIVE]	= (!m_bEarthQuake && m_dVerLoad[VER_LOAD_LIVE] != 0) ? fabs(toM((m_pFooting->GetWidth() - dWidthBack/2) * iDir)):0;
	m_dXiVerLoad[VER_LOAD_DEAD]	= m_dXiVerLoad[VER_LOAD_LIVE];
	if((bTrap && (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
		||(bTrap2 && (m_nJijum==0 || m_nJijum==m_pBri->m_nQtyJigan)) || bArch)
	{
		m_dXiVerLoad[VER_LOAD_FOOTW] = 0;
	}
	else m_dXiVerLoad[VER_LOAD_FOOTW] = (m_dVerLoad[VER_LOAD_FOOTW] == 0)? 0:fabs(toM((m_pFooting->GetWidth()/2) * iDir));
	m_dXiVerLoad[VER_LOAD_PAVE]	= (m_dVerLoad[VER_LOAD_PAVE] == 0)? 0:fabs(toM((m_pFooting->GetWidth() - dWidthBack/2) * iDir));
	m_dXiVerLoad[VER_LOAD_BACKFILL]	= (m_dVerLoad[VER_LOAD_BACKFILL] == 0)? 0:fabs(toM((m_pFooting->GetWidth() - dWidthBack/2) * iDir));
	m_dXiUpperLoadV	= (m_dUpperLoadV == 0)? 0:fabs(toM((dWidthFront + dTickWallhalf) * iDir));
	
	double dVertDistLoadEarthquake = pData->m_nTypeJointPosFootingEQ == 0 ? m_pFooting->GetHeight() : m_pFooting->GetHeight()/2;
	m_dYiUpperLoadH	= toM((m_bEarthQuake == FALSE)? m_pFooting->GetHeight() : dVertDistLoadEarthquake);
	if(m_dUpperLoadV == 0) m_dYiUpperLoadH = 0;

	// ������ ������ ����
	if(m_bEarthQuake && m_bApplyHoriEarQ)
	{
		double dElBackFill = 0;
		CDPoint xyStt(0, 0);
		xyStt.x	= m_pBri->GetStationOnJijum(m_nJijum, 0);
		dElBackFill = m_pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y;
		if(m_pBri->m_bUnderGroundRahmen) dElBackFill += m_pBri->m_dUnderGroundDesignHeight;

		// (ARCBRIDGE-3627) ���� �ۿ�� ���� ���ݰ�� ���� �ɼ��߰�.
		m_dHorLoad[VER_LOAD_FOOTW] = m_dVerLoad[VER_LOAD_FOOTW] * m_dA;

		m_dYiHorLoad[VER_LOAD_FOOTW] = (m_dHorLoad[1] == 0)? 0:fabs(toM(dVertDistLoadEarthquake));
		m_dYiHorLoad[VER_LOAD_PAVE]	= (m_dHorLoad[2] == 0)? 0:fabs(toM(dElBackFill-m_pFooting->m_dEL)+toM(m_pFooting->GetHeight())+toM(m_pBri->m_dTP)/2);
		m_dYiHorLoad[VER_LOAD_BACKFILL]	= (m_dHorLoad[3] == 0)? 0:fabs(toM(dElBackFill-m_pFooting->m_dEL)/2+toM(m_pFooting->GetHeight()));
		
		m_dHorSmallTotal = m_dHorLoad[0] + m_dHorLoad[1] + m_dHorLoad[2] + m_dHorLoad[3];
	}
	//////////////////////////////////////////////////////////////////////////
	// �����ʽ� ���ʸ��� �� ���� ����
	if((bTrap && (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || m_pFooting->m_nType==FOOTING_TYPE_RIGHTSLOPE))
		||(bTrap2 && (m_nJijum==0 || m_nJijum==m_pBri->m_nQtyJigan)) || bArch)
	{
		double A[3];
		double Xi[3];
		double Yi[3];

		double dW, dFH, dFSH, dFEW, dFSW;

		if(bTrap)
		{
			// (ARCBRIDGE-1559) FH �� FSH�� �ٲ����... FSH�� �Ʒ��� ���̶��.. ������ �־ ������ ���ϰ�...
			// FSW�� �������� ���̳�... �Ѥ�
			dW = toM(m_pFooting->GetWidth());
			//	dFH = toM(m_pFooting->m_dFH);
			//	dFSH = toM(m_pFooting->GetHeight(FALSE));
			dFSH = toM(m_pFooting->m_dFH);
			dFH = toM(m_pFooting->GetHeight(FALSE));
			dFEW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFERW) : toM(m_pFooting->m_dFELW);
			dFSW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFSRW) : toM(m_pFooting->m_dFSLW);
			dFSW = dW - dFSW;

			CWallApp *pWall = m_pBri->GetInWall((m_nJijum==1)? 0 : m_pBri->GetCountInWall()-1);
			CTwinVector tvTemp = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");

			// ������� �ۿ���ġ
			m_dXiUpperLoadV	= (m_dUpperLoadV == 0)? 0 : toM(fabs(xyBase.x - tvTemp.m_v1.x));
			m_dYiUpperLoadH	= (m_dUpperLoadH != 0)? toM(fabs(tvTemp.m_v1.y -xyBase.y)): 0;
		}
		else
		{
			dW = toM(m_pFooting->GetWidth());
			dFH = toM(m_pFooting->GetHeight(FALSE));
			dFSH = toM((m_nJijum==0)? m_pFooting->m_dFSRH : m_pFooting->m_dFSLH);
			dFEW = toM((m_nJijum==0)? m_pFooting->m_dFERW : m_pFooting->m_dFELW);
			dFSW = toM((m_nJijum==0)? m_pFooting->m_dFSRW : m_pFooting->m_dFSLW);

			CWallApp *pWall;
			CTwinVector tvTemp;
			// ������� �ۿ���ġ
			// 1.������ ��ü
			double dFWF = toM((m_nJijum==0)? m_pFooting->m_dFWR : m_pFooting->m_dFWL);
			m_dXiUpperLoadV = dFEW + dFSW + dFWF + toM(dTickWallhalf);
			m_dYiUpperLoadH = toM(m_pFooting->GetHeight(TRUE));
			// 2. ��米��
			CDPoint xyAdd(0, 0);
			if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				CTwinVector tvTemp = m_pBri->m_tvArrVertSectionRibCen.GetAt(bStt? 0:m_pBri->m_tvArrVertSectionRibCen.GetSize()-1);
				xyAdd = (bStt)? tvTemp.m_v1 : tvTemp.m_v2;
			}
			else
			{
				m_pBri->SyncBridge(0, FALSE);
				pWall = m_pBri->GetInWall((m_nJijum==0)? 0 : m_pBri->GetCountInWall()-1);
				tvTemp = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
				xyAdd = tvTemp.m_v1;
			}
			
			m_dXiUpperLoadV_Add	= (m_dUpperLoadV_Add == 0)? 0 : toM(fabs(xyBase.x - xyAdd.x));
			m_dYiUpperLoadH_Add	= (m_dUpperLoadH_Add != 0)? toM(fabs(xyAdd.y -xyBase.y)) : 0;
		}
		if(m_bEarthQuake && pData->m_nTypeJointPosFootingEQ == 1)
		{
			m_dYiUpperLoadH		= m_dYiUpperLoadH/2;
			m_dYiUpperLoadH_Add	= m_dYiUpperLoadH_Add/2;
		}
		
		A[0] = (dW - dFSW - dFEW) * dFSH;
		A[1] = (dFSW * dFSH /2);
		A[2] = (dW * dFH);
		
		Xi[0] = dFEW + dFSW + (dW - dFSW - dFEW)/2;
		Xi[1] = dFEW + dFSW*2/3;
		Xi[2] = dW/2;
		
		Yi[0] = dFH + dFSH/2;
		Yi[1] = dFH + dFSH/3;
		Yi[2] = dFH/2;
		
		double dA = A[0] + A[1] + A[2];
		double dXi = (A[0]*Xi[0] + A[1]*Xi[1] + A[2]*Xi[2]) / dA;
		double dYi = (A[0]*Yi[0] + A[1]*Yi[1] + A[2]*Yi[2]) / dA;

		m_dXiVerLoad[VER_LOAD_FOOTW] = dXi;
		m_dYiHorLoad[VER_LOAD_FOOTW] = dYi;
	}

	if(bLsd)
	{
		for(long ix=0; ix<VER_LOAD_COUNT; ++ix)
		{
			m_dXiVerLoad[ix] = frM(m_dXiVerLoad[ix]);
			m_dYiHorLoad[ix] = frM(m_dYiHorLoad[ix]);
		}

		m_dXiUpperLoadV	= frM(m_dXiUpperLoadV);
		m_dYiUpperLoadH	= frM(m_dYiUpperLoadH);
		m_dXiUpperLoadV_Add	= frM(m_dXiUpperLoadV_Add);
		m_dYiUpperLoadH_Add	= frM(m_dYiUpperLoadH_Add);
	}

	//////////////////////////////////////////////////////////////////////////
	// Mv, Mh ���
	m_dMr[VER_LOAD_LIVE]	 = m_dVerLoad[VER_LOAD_LIVE] * m_dXiVerLoad[VER_LOAD_LIVE];
	m_dMr[VER_LOAD_DEAD]	 = m_dVerLoad[VER_LOAD_DEAD] * m_dXiVerLoad[VER_LOAD_DEAD];
	m_dMr[VER_LOAD_FOOTW]  	 = m_dVerLoad[VER_LOAD_FOOTW] * m_dXiVerLoad[VER_LOAD_FOOTW];
	m_dMr[VER_LOAD_PAVE]	 = m_dVerLoad[VER_LOAD_PAVE] * m_dXiVerLoad[VER_LOAD_PAVE];
	m_dMr[VER_LOAD_BACKFILL] = m_dVerLoad[VER_LOAD_BACKFILL] * m_dXiVerLoad[VER_LOAD_BACKFILL];

	m_dMo[VER_LOAD_FOOTW]	 = m_dHorLoad[VER_LOAD_FOOTW] * m_dYiHorLoad[VER_LOAD_FOOTW];
	m_dMo[VER_LOAD_BACKFILL] = m_dHorLoad[VER_LOAD_BACKFILL] * m_dYiHorLoad[VER_LOAD_BACKFILL];

	m_dMrSmallTotal = m_dMr[0] + m_dMr[1] + m_dMr[2] + m_dMr[3] + m_dMr[4];
	m_dMoSmallTotal = m_dMo[0] + m_dMo[1] + m_dMo[2] + m_dMo[3] + m_dMo[4];
	
	// (ARCBRIDGE-3025) ��ȣ�� ���� Mr Mo �� �����ϰ� �־��µ� ��� �ۿ���� ��� Mo�� ��ȣ ����� �ش�.
	// (ARCBRIDGE-3025) �������� ����
	m_dMrUpperV	= (m_dUpperLoadV > 0)? fabs(m_dUpperLoadV * m_dXiUpperLoadV):0;
	m_dMoUpperV	= (m_dUpperLoadV < 0)? fabs(m_dUpperLoadV * m_dXiUpperLoadV):0;
	// m_dMrUpperH	= (m_dUpperLoadH < 0)? fabs(m_dUpperLoadH * m_dYiUpperLoadH):0;
	// m_dMoUpperH	= (m_dUpperLoadH > 0)? fabs(m_dUpperLoadH * m_dYiUpperLoadH):0;
	// m_dMrUpperM = (m_dUpperLoadM <0)? fabs(m_dUpperLoadM):0;
	// m_dMoUpperM = (m_dUpperLoadM >0)? fabs(m_dUpperLoadM):0;

	// m_dMrUpperV	= 0;
	// m_dMoUpperV	= m_dUpperLoadV * m_dXiUpperLoadV * -1;
	m_dMrUpperH	= 0;
	m_dMoUpperH	= m_dUpperLoadH * m_dYiUpperLoadH;
	m_dMrUpperM = 0;
	m_dMoUpperM = m_dUpperLoadM;

	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (m_nJijum==0 || m_nJijum==m_pBri->m_nQtyJigan) || bArch)
	{
		m_dMrUpperV_Add	= (m_dUpperLoadV_Add > 0)? fabs(m_dUpperLoadV_Add * m_dXiUpperLoadV_Add):0;
		m_dMoUpperV_Add	= (m_dUpperLoadV_Add < 0)? fabs(m_dUpperLoadV_Add * m_dXiUpperLoadV_Add):0;
		m_dMrUpperH_Add	= 0;
		m_dMoUpperH_Add	= m_dUpperLoadH_Add * m_dYiUpperLoadH_Add;
		m_dMrUpperM_Add = 0;
		m_dMoUpperM_Add = m_dUpperLoadM_Add;
	}
	else
	{
		m_dMrUpperV_Add	= 0;
		m_dMoUpperV_Add	= 0;
		m_dMrUpperH_Add	= 0;
		m_dMoUpperH_Add	= 0;
		m_dMrUpperM_Add = 0;
		m_dMoUpperM_Add = 0;
	}

	// �Ѱ�
	m_dVerTotal		= m_dVerSmallTotal + m_dUpperLoadV + m_dUpperLoadV_Add;
	m_dHorTotal		= m_dHorSmallTotal + m_dUpperLoadH + m_dUpperLoadH_Add;
	
	m_dMrTotal	= m_dMrSmallTotal + m_dMrUpperV + m_dMrUpperH + m_dMrUpperM + m_dMrUpperV_Add + m_dMrUpperH_Add + m_dMrUpperM_Add;
	m_dMoTotal	= m_dMoSmallTotal + m_dMoUpperV + m_dMoUpperH + m_dMoUpperM + m_dMoUpperV_Add + m_dMoUpperH_Add + m_dMoUpperM_Add;

	double dB = bLsd? m_pFooting->GetWidth() : toM(m_pFooting->GetWidth());
	// (ARCBRIDGE-3025) ��� ���Ҷ� ���Ʈ ��ȣ ����� ����ϵ��� ����
	m_dE = dB/2. - (m_dMrTotal-m_dMoTotal)/m_dVerTotal;
}

void CLoadFooting::MakeTableOtherLoad(CXLFormatText *XLText)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CString str = _T("");
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h");
	
	long nIndexTiny = 1;
	int nCntRow = (pData->IsRailDesign() && m_dVerLoad[VER_LOAD_DEAD]>0) ? VER_LOAD_COUNT+2 : VER_LOAD_COUNT+1;
	int nCntCol = 8;
	long nStt = (m_bEarthQuake) ? 1 : 0;

	if(!m_bEarthQuake) 
		XLText->AddFormatText("$s+06$h$e%s$r$e�� �� ��$n$h", GetNumberString(nIndexTiny,NUM_CIRCLE));
	else 
		XLText->AddFormatText("$s+06$h$e%s$r$e�� �� ��$n$h", GetNumberString(nIndexTiny+1,NUM_CIRCLE));
	XLText->AddTable(nCntRow-1-nStt, 24-1, 1, 1);

	if(m_bEarthQuake && m_bApplyHoriEarQ)		// ������ �̰� ������ �����϶�
	{
		nCntCol = 9;
		// Title
		int nWidthJoin1[] = { 3, 3, 2, 3, 2, 2, 3, 3, 3 };
		char *Title1[] = { "�� ��", "V", "A", "H", "Xi", "Yi", "Mr", "Mo", "�� ��" };
		if(pData->m_nTypeEarthQuakeApply != 0)
			Title1[2] = _T("S");

		char *sItem[] = { (m_pBri->m_bUnderGroundRahmen && !m_pBri->m_bCalcOpen)? "����Ȱ����":"��� Ȱ����", "�� �� �� ��", "��      ��", "�� �� �� ��", "��      ��" , ""};
		if((pData->IsRailDesign() && m_dVerLoad[VER_LOAD_DEAD]>0))
		{
			sItem[0] = "���� Ȱ����";
			sItem[4] = "�����������";
			sItem[5] = "��      ��";
		}
		
		int i=0; for(i=0; i<nCntCol; i++)
		{		
			XLText->AddMergeFormatText(0,nWidthJoin1[i]-1,"$c%s", Title1[i]);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
		}
		XLText->AddText("$n$h");
		
		// Data
		for(i=nStt; i<nCntRow-1; i++)
		{	
			int j=0; for(j=0; j<nCntCol; j++)
			{
				double A = (m_dHorLoad[i] == 0  || i-nStt == VER_LOAD_FOOTW)? 0 : m_dA;
				if(j == 0) str.Format("%s", sItem[i]);
				else if(j == 1 && i < nCntRow-2) str.Format("$f3%.3f", m_dVerLoad[i]);
				else if(j == 1 && i == nCntRow-2) str.Format("$f3%.3f", m_dVerSmallTotal);
				else if(j == 2 && i < nCntRow-2) str.Format("$f3%.3f", A);
				else if(j == 3 && i == nCntRow-2) str.Format("$f3%.3f", m_dHorSmallTotal);
				else if(j == 3 && i < nCntRow-2) str.Format("$f3%.3f", m_dHorLoad[i]);
				else if(j == 4 && i < nCntRow-2) str.Format("$f3%.3f", m_dXiVerLoad[i]);
				else if(j == 5 && i < nCntRow-2) str.Format("$f3%.3f", m_dYiHorLoad[i]);
				else if(j == 6 && i < nCntRow-2) str.Format("$f3%.3f", m_dMr[i]);
				else if(j == 6 && i == nCntRow-2) str.Format("$f3%.3f", m_dMrSmallTotal);
				else if(j == 7 && i < nCntRow-2) str.Format("$f3%.3f", m_dMo[i]);
				else if(j == 7 && i == nCntRow-2) str.Format("$f3%.3f", m_dMoSmallTotal);
				else str = "";
				
				if(str == "$f30.000") str = "";
				
				XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s", str);
				XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
				
			}
			XLText->AddText("$n$h");
		}
	}
	else
	{
		nCntCol = 8;
		// Title
		int nWidthJoin1[] = { 3, 3, 3, 3, 3, 3, 3, 3 };
		char *Title1[] = { "�� ��", "V", "H", "Xi", "Yi", "Mr", "Mo", "���" };
		char *sItem[] = { (m_pBri->m_bUnderGroundRahmen)? "����Ȱ����":"��� Ȱ����", "�� �� �� ��", "��      ��", "�� �� �� ��", "��      ��" , "" };
		if((pData->IsRailDesign() && m_dVerLoad[VER_LOAD_DEAD]>0))
		{
			sItem[0] = "���� Ȱ����";
			sItem[4] = "�����������";
			sItem[5] = "��      ��";
		}
		
		int i=0; for(i=0; i<nCntCol; i++)
		{		
			XLText->AddMergeFormatText(0,nWidthJoin1[i]-1,"$c%s", Title1[i]);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
		}
		XLText->AddText("$n$h");
		
		// Data
		for(i=nStt; i<nCntRow-1; i++)
		{	
			int j=0; for(j=0; j<nCntCol; j++)
			{
				if(j == 0) str.Format("%s", sItem[i]);
				else if(j == 1 && i < nCntRow-2) str.Format("$f3%.3f", m_dVerLoad[i]);
				else if(j == 1 && i == nCntRow-2) str.Format("$f3%.3f", m_dVerSmallTotal);
				else if(j == 2 && i < nCntRow-2) str.Format("$f3%.3f", m_dHorLoad[i]);
				else if(j == 2 && i == nCntRow-2) str.Format("$f3%.3f", m_dHorSmallTotal);
				else if(j == 3 && i < nCntRow-2) str.Format("$f3%.3f", m_dXiVerLoad[i]);
				else if(j == 4 && i < nCntRow-2) str.Format("$f3%.3f", m_dYiHorLoad[i]);
				else if(j == 5 && i < nCntRow-2) str.Format("$f3%.3f", m_dMr[i]);
				else if(j == 5 && i == nCntRow-2) str.Format("$f3%.3f", m_dMrSmallTotal);
				else if(j == 6 && i < nCntRow-2) str.Format("$f3%.3f", m_dMo[i]);
				else if(j == 6 && i == nCntRow-2) str.Format("$f3%.3f", m_dMoSmallTotal);
				else str = "";
				
				if(str == "$f30.000") str = "";
				
				XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s", str);
				XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
				
			}
			XLText->AddText("$n$h");
		}
	}
}

void CLoadFooting::MakeTableOtherLoadLsd(CXLFormatText *XLText)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE, COL_POS_2);

	long nIndexTiny = 1;
	if(!m_bEarthQuake) 
		XLText->AddFormatText("$s+06$h$e%s$r$e�� �� ��$n$h", GetNumberString(nIndexTiny,NUM_CIRCLE));
	else 
		XLText->AddFormatText("$s+06$h$e%s$r$e�� �� ��$n$h", GetNumberString(nIndexTiny+1,NUM_CIRCLE));
	
	XLText->AddTable(5, 23, 2, 6);
	//Ÿ��Ʋ
	XLText->AddMergeText(1, 5, _T("$m+06"));
	XLText->AddMergeText(1, 2, _T("$m+03"));
	XLText->AddMergeText(1, 2, _T("$m+03"));
	
	XLText->AddFormatText(_T("$h$c$me5�� ��$m+06$c$me2������(%s)$m+03$c$me2�����(%s)$m+03$c$me3�ۿ�Ÿ�$m+04$c$me7���Ʈ$n$h"), pData->m_szTonf, pData->m_szTonf);
	XLText->AddFormatText(_T("$h$me5$m+06$me2$m+03$me2$m+03$c$me1X(%s)$m+02$c$me1Y(%s)$m+02$c$me3Mr(%s)$m+04$c$me3Mo(%s)$n$h"), pData->m_szM, pData->m_szM, pData->m_szTonfM, pData->m_szTonfM);

	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1) ? TRUE : FALSE;

	CString strAdd = m_bEarthQuake ? "(�شܥ�)" : "";
	CString strES = (m_nJijum != m_pBri->GetCountJijum()-1) ? "ESL" : "ESR";
	CString strItem[] = { "1.��������" + strAdd, "2.������" + strAdd, "3.����" + strAdd, "4.��ä�� ��������" };
	CString strKind[] = { "DC", "EV", "DW", strES };
	CString	strVert	=	_T("$f[-]0");
	CString	strHori	=	_T("$f[-]0");
	CString	strX	=	_T("$f[-]0");
	CString	strY	=	_T("$f[-]0");
	CString	strMr	=	_T("$f[-]0");
	CString	strMo	=	_T("$f[-]0");
	CString strName	=	_T("");

	// ���⼭�� Factor�� ���� �ȵ� ���� ������ �ϹǷ� Factor�� ��������.
	double dLoadFactor[4] = { m_dFactorDeadLoad, m_dFactorSoilHor, m_dFactorPave, m_dFactorLiveLoad };


	HGINT32 iArrRow[] = {VER_LOAD_FOOTW, VER_LOAD_BACKFILL, VER_LOAD_PAVE, VER_LOAD_LIVE};

	for (HGINT32 ix = 0; ix < 4; ++ix)
	{
		HGINT32 iRow = iArrRow[ix];

		strName = strItem[ix];
		if (m_dVerLoad[iRow] != 0)
		{
			strVert = pData->GetStrDouble(m_dVerLoad[iRow] / dLoadFactor[ix], UNIT_CGS_TONF_M);
		}

		if (m_dHorLoad[iRow] != 0)
		{
			strHori = pData->GetStrDouble(m_dHorLoad[iRow] / dLoadFactor[ix], UNIT_CGS_TONF_M);
		}

		if (m_dXiVerLoad[iRow] != 0)
		{
			strX = pData->GetStrDouble(m_dXiVerLoad[iRow], UNIT_CGS_M);
		}

		if (m_dYiHorLoad[iRow] != 0)
		{
			strY = pData->GetStrDouble(m_dYiHorLoad[iRow], UNIT_CGS_M);
		}

		if (m_dMr[iRow] != 0)
		{
			strMr = ::GetStrDouble(m_dMr[iRow] / dLoadFactor[ix], 0, TRUE);
		}

		if (m_dMo[iRow] != 0)
		{
			strMo = ::GetStrDouble(m_dMo[iRow] / dLoadFactor[ix], 0, TRUE);
		}
		
		XLText->AddFormatText(_T("$h$e$me4%s$m+05"), strName);
		XLText->AddFormatText(_T("$c%s$r"), strKind[ix]);
		XLText->AddFormatText(_T("$c$me2%s$m+03"),  strVert);
		XLText->AddFormatText(_T("$c$me2%s$m+03"),  strHori);
		XLText->AddFormatText(_T("$c$me1%s$m+02$c$me1%s$m+02"), strX, strY);
		XLText->AddFormatText(_T("$c$me3%s$m+04"), strMr);
		XLText->AddFormatText(_T("$c$me3%s$n$h"), strMo);

		strVert	=	_T("$f[-]0");	strHori	=	_T("$f[-]0");	strX	=	_T("$f[-]0");	strY	=	_T("$f[-]0");	
		strMr	=	_T("$f[-]0");	strMo	=	_T("$f[-]0");	strName	=	_T("");
	}

	XLText->AddFormatText(_T("$n$h"));
}

void CLoadFooting::MakeTableLoadAll(CXLFormatText *XLText, long nIndexTiny, BOOL bTable)
{
	if( m_nCombo < 0 )
		return;

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	BOOL bLsd = pData->IsLsdDesign();
	CStringArray sArrLoad;
	sArrLoad.Add("��������");
	sArrLoad.Add("�������");
	sArrLoad.Add("Ȱ����");
	sArrLoad.Add("���� Ȱ����");
	sArrLoad.Add("��ũ �� Ư�� ����������");
	sArrLoad.Add("�����");
	sArrLoad.Add("�����");
	sArrLoad.Add("�µ�����(+)");
	sArrLoad.Add("�µ�����(-)");
	sArrLoad.Add("��������");
	sArrLoad.Add("����ħ��");
	sArrLoad.Add("�õ� �� ��������");
	sArrLoad.Add("��뷹�� ������");
	sArrLoad.Add("��������");
	sArrLoad.Add("������ Ȱ����");

	CString str = _T("");
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	
	if (bLsd)
	{
		CString strComboEnvType = m_pBri->GetStringComboEnvType_Lsd(m_eEnvComboType, FALSE);
		XLText->AddFormatText("$s+15$h$e%s$r%s(%s) : %s$n$h", GetNumberString(nIndexTiny+1,NUM_CIRCLE), strComboEnvType, m_sTitleSub, m_sCombo);
	}
	else
	{
		XLText->AddFormatText("$s+15$h$e%s$r%s : COMBO %d$n", GetNumberString(nIndexTiny+1,NUM_CIRCLE), m_sTitleSub, m_nCombo+1);
	}
	
	if (bLsd == FALSE)
	{
		//
		CString strTemp = _T("");
		for(long k=0; k<BRIDGE_LOAD_BASIC; k++)
		{
			long nCombo = m_bUseLoad ? m_pBri->m_lArrComboUse.GetAt(m_nCombo) : m_pBri->m_lArrComboUlt.GetAt(m_nCombo);
			double dFactor = m_pBri->GetLoadFactor((m_bEarthQuake)? LOAD_COMBINE_EQ: (m_bUseLoad ? LOAD_COMBINE_USE : LOAD_COMBINE_ULT), nCombo, k);
			
			if(dFactor > 0)
			{
				strTemp.Format("%.2fx%s", dFactor, sArrLoad.GetAt(k));
				if(str != _T("")) str += "+";
				str += strTemp;
			}
		}

		XLText->AddFormatText("$h$r$tx07%s$n$h", str);
	}

///
	if(!bTable) return;

	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (m_nJijum==0 || m_nJijum==m_pBri->m_nQtyJigan));
	BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	int nCntRow = (bTrap2 || bArch)? 9 : 6;
	int nCntCol = 8;
	XLText->AddTable(nCntRow-1, 24-1, 1, 1);
	
	// Title
	int nWidthJoin1[] = { 4, 3, 3, 3, 3, 3, 3, 2 };
	char *Title1[] = { "�� ��", "V", "H", "Xi", "Yi", "Mr", "Mo", "�� ��" };
	char *sItem[] = { "�� Ÿ �� ��", "��      ��" };
	
	int i=0; for(i=0; i<8; i++)
	{		
		XLText->AddMergeFormatText(0,nWidthJoin1[i]-1,"$c%s", Title1[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$h");
	
	// Data
	for(i=0; i<nCntRow-1; i++)
	{	
		int j=0; for(j=0; j<nCntCol; j++)
		{
			str.Empty();

			if(j == 0)
			{
				if(i < nCntRow-3) str = _T("");
				else str.Format("%s", (i==nCntRow-2)? sItem[1] : sItem[0]);
			}
			else if(j == 1 && i == 0) str.Format("$f3%.3f", m_dUpperLoadV);
			else if(j == 1 && ((bTrap2||bArch) && i==3)) str.Format("$f3%.3f", m_dUpperLoadV_Add);
			else if(j == 1 && i == nCntRow-3) str.Format("$f3%.3f", m_dVerSmallTotal);
			else if(j == 1 && i == nCntRow-2) str.Format("$f3%.3f", m_dVerTotal);
			else if(j == 2 && i == 1) str.Format("$f3%.3f", m_dUpperLoadH);
			else if(j == 2 && ((bTrap2||bArch) && i==4)) str.Format("$f3%.3f", m_dUpperLoadH_Add);
			else if(j == 2 && i == nCntRow-3) str.Format("$f3%.3f", m_dHorSmallTotal);
			else if(j == 2 && i == nCntRow-2) str.Format("$f3%.3f", m_dHorTotal);
			
			if (bLsd)
			{
				if(j == 3 && i == 0) str = ::GetStrDouble(m_dXiUpperLoadV, 0, TRUE);
				else if(j == 3 && ((bTrap2||bArch) && i==3)) str = ::GetStrDouble(m_dXiUpperLoadV_Add, 0, TRUE);
				else if(j == 4 && i == 1) str = ::GetStrDouble(m_dYiUpperLoadH, 0, TRUE);
				else if(j == 5 && i == 0) str = ::GetStrDouble(m_dMrUpperV, 0, TRUE);
				else if(j == 5 && i == 1) str = ::GetStrDouble(m_dMrUpperH, 0, TRUE);
				else if(j == 5 && i == 2) str = ::GetStrDouble(m_dMrUpperM, 0, TRUE);
				else if(j == 5 && ((bTrap2||bArch) && i==3)) str = ::GetStrDouble(m_dMrUpperV_Add, 0, TRUE);
				else if(j == 5 && ((bTrap2||bArch) && i==4)) str = ::GetStrDouble(m_dMrUpperH_Add, 0, TRUE);
				else if(j == 5 && ((bTrap2||bArch) && i==5)) str = ::GetStrDouble(m_dMrUpperM_Add, 0, TRUE);
				else if(j == 5 && i == nCntRow-3) str = ::GetStrDouble(m_dMrSmallTotal, 0, TRUE);
				else if(j == 5 && i == nCntRow-2) str = ::GetStrDouble(m_dMrTotal, 0, TRUE);
				else if(j == 6 && i == 0) str = ::GetStrDouble(m_dMoUpperV, 0, TRUE);
				else if(j == 6 && i == 1) str = ::GetStrDouble(m_dMoUpperH, 0, TRUE);
				else if(j == 6 && i == 2) str = ::GetStrDouble(m_dMoUpperM, 0, TRUE);
				else if(j == 6 && ((bTrap2||bArch) && i==3)) str = ::GetStrDouble(m_dMoUpperV_Add, 0, TRUE);
				else if(j == 6 && ((bTrap2||bArch) && i==4)) str = ::GetStrDouble(m_dMoUpperH_Add, 0, TRUE);
				else if(j == 6 && ((bTrap2||bArch) && i==5)) str = ::GetStrDouble(m_dMoUpperM_Add, 0, TRUE);
				else if(j == 6 && i == nCntRow-3) str = ::GetStrDouble(m_dMoSmallTotal, 0, TRUE);
				else if(j == 6 && i == nCntRow-2) str = ::GetStrDouble(m_dMoTotal, 0, TRUE);

				if(str == "0") str = "";
			}
			else
			{
				if(j == 3 && i == 0) str.Format("$f3%.3f", m_dXiUpperLoadV);
				else if(j == 3 && ((bTrap2||bArch) && i==3)) str.Format("$f3%.3f", m_dXiUpperLoadV_Add);
				else if(j == 4 && i == 1) str.Format("$f3%.3f", m_dYiUpperLoadH);
				else if(j == 4 && ((bTrap2||bArch) && i==4)) str.Format("$f3%.3f", m_dYiUpperLoadH_Add);
				else if(j == 5 && i == 0) str.Format("$f3%.3f", m_dMrUpperV);
				else if(j == 5 && i == 1) str.Format("$f3%.3f", m_dMrUpperH);
				else if(j == 5 && i == 2) str.Format("$f3%.3f", m_dMrUpperM);
				else if(j == 5 && ((bTrap2||bArch) && i==3)) str.Format("$f3%.3f", m_dMrUpperV_Add);
				else if(j == 5 && ((bTrap2||bArch) && i==4)) str.Format("$f3%.3f", m_dMrUpperH_Add);
				else if(j == 5 && ((bTrap2||bArch) && i==5)) str.Format("$f3%.3f", m_dMrUpperM_Add);
				else if(j == 5 && i == nCntRow-3) str.Format("$f3%.3f", m_dMrSmallTotal);
				else if(j == 5 && i == nCntRow-2) str.Format("$f3%.3f", m_dMrTotal);
				else if(j == 6 && i == 0) str.Format("$f3%.3f", m_dMoUpperV);
				else if(j == 6 && i == 1) str.Format("$f3%.3f", m_dMoUpperH);
				else if(j == 6 && i == 2) str.Format("$f3%.3f", m_dMoUpperM);
				else if(j == 6 && ((bTrap2||bArch) && i==3)) str.Format("$f3%.3f", m_dMoUpperV_Add);
				else if(j == 6 && ((bTrap2||bArch) && i==4)) str.Format("$f3%.3f", m_dMoUpperH_Add);
				else if(j == 6 && ((bTrap2||bArch) && i==5)) str.Format("$f3%.3f", m_dMoUpperM_Add);
				else if(j == 6 && i == nCntRow-3) str.Format("$f3%.3f", m_dMoSmallTotal);
				else if(j == 6 && i == nCntRow-2) str.Format("$f3%.3f", m_dMoTotal);
			}
			
			if(str == "$f30.000") str = "";
			
			if(j == 0 && (i >=0 && i<nCntRow-3))
			{
				if(i==0 || i==3)
				{
					if(i==0) XLText->AddMergeFormatText(2, 1,"$c%s", "���\n����\n����\n(��ü)");
					else XLText->AddMergeFormatText(2, 1,"$c%s%s", "���\n����\n����", bTrap2? "\n(���)" : "\n(����)");
					str = "������";
				}
				else if(i==1 || i==4) str = "�����";
				else if(i==2 || i==5) str = "���Ʈ";
				
				XLText->AddFormatText("$m+0%d",2);
				XLText->AddMergeFormatText(0, 1, "$b034$c%s", str);
				XLText->AddFormatText("$m+0%d", 2);
			}
			else
			{
				XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s", str);
				XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
			}	
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");

}

void CLoadFooting::CalulateQaMax()
{
	long nType = m_pFooting->m_nTypeEquation_Qa;
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);

	double NValue = pBoring->m_nNValue;
	double pi = ToRadian(m_pFooting->GetFrictionAngleBaseGround(NValue));
	double L  = toM(m_pFooting->GetLength());
	double B  = toM(m_pFooting->GetWidth());
	double Df = m_pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
	double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
	double r1 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double r2 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

	// Terzaghi //////////////////////////////////////////////////
	if(nType == 0)
	{
		// �������鿡�� ���ϼ��������� ����(dH)�� ������ ��(B_)���� ������쿡�� r1�� r_�� ����Ѵ�			
		double q	= r2*Df; // ????
		////////////////////////////////////////////////
		double eB	= m_dE;
		double B_	= B - 2*fabs(eB);
		double A_	= B_*L;
		double alpha= 1.0 + 0.3*B_/L; // ���簢��
		double beta = 0.5 - 0.1*B_/L;
		double Kpr	= GetKprValue(m_pFooting->GetFrictionAngleBaseGround(pBoring->m_nNValue));
		double  a   = exp((0.75*ConstPi-pi/2)*tan(pi));
		double Nq	= pow(a,2) / (2*pow(cos(ConstPi/4+pi/2),2));
		double Nc	= (Nq-1)/tan(pi);
		double Nr	= tan(pi)/2 * (Kpr/pow(cos(pi),2) - 1);
		
		m_dA_Equation = A_;
		m_dQaEquation = ( alpha*c*Nc + q*Nq + beta*r1*B_*Nr );
	}
	// Meyerhof //////////////////////////////////////////////////
	// ��ȿ���ϸ����� ������� �ʴ´�.....
	if(nType==1)
	{
		double q	= r2*Df; // ????
		double A_	= L*B;
		double B_	= B;
		double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
		double Nc	= (pi==0) ? 5.14 : (Nq-1)/tan(pi);
		double Nr	= (Nq-1)*tan(1.4*pi);
		
		m_dA_Equation = A_;
		m_dQaEquation = ( c*Nc + q*Nq + 0.5*r1*B_*Nr );
	}
	// Hansen   //////////////////////////////////////////////////
	if(nType==2)
	{
		double beta = 0; // ���ݰ��..
		// Qu = A'[c*Nc*Sc*dc*ic*gc*bc + q*Nq*Sq*dq*iq*gq*bq + 1/2*r1*B'*Nr*Sr*dr*ir*gr*br]
		double eB	= m_dE;
		double B_	= B - 2*fabs(eB);
		double A_	= B_*L;
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
		double dc_	= 0.4*k;
		double gc	= 1 - beta/147.0;
		double gq	= pow(1 - 0.5*tan(ToRadian(beta)),5);
		double gr	= gq;
		
		m_dA_Equation = A_;
		if(pi==0)
		{
			double ic_	= 1;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double bc_	= 1;	// ���������� ��簡 �����Ƿ� 1

			m_dQaEquation = (5.14*c*(1+Sc_+dc_-ic_-dc_-bc_)+q);
		}
		else
		{
			double dr	= 1;
			double ic	= 1;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double iq	= 1;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double ir	= 1;	// ������(Si�� �����Ͽ� ����� �� �����Ƿ� �������� �ʴ´�
			double bc	= 1;	// ���������� ��簡 �����Ƿ� 1
			double bq	= 1;	// ���������� ��簡 �����Ƿ� 1
			double br	= 1;	// ���������� ��簡 �����Ƿ� 1

			m_dQaEquation = (c*Nc*Sc*dc*ic*gc*bc + q*Nq*Sq*dq*iq*gq*bq + 0.5*r1*B_*Nr*Sr*dr*ir*gr*br);
		}
	}
	// QaMax //////////////////////////////////////////////////
}

void CLoadFooting::CalulatePileCentroidLoad()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();

	double dDistCen = GetPileFootingCentroid(m_nJijum);
	//CJijum *pJ = m_pBri->GetJijum(m_nJijum);
	double wSlab = toM(Round(m_pBri->GetWidthSlabActByGagak(m_pBri->GetStationOnJijum(m_nJijum), m_pBri->GetAngleJijum(m_nJijum)), 0)); // m_pStd->GetSlabWidth(m_nJijum)/vAng.y;
	m_dDistCenttoDosim = toM(m_pFooting->GetWidth()/2) - dDistCen;

	// (ARCBRIDGE-3163) ���ʾ�������� ���缭 ���������� ����
	// (ARCBRIDGE-3504) ���ʾ������並 ������������ ����
	// double wFooting = toM(Round(m_pFooting->GetLength(), 0));
	if(bLsd)
	{
		dDistCen = frM(dDistCen);
		m_dDistCenttoDosim = frM(m_dDistCenttoDosim);
		wSlab = frM(wSlab);
		// wFooting = frM(wFooting);
	}

	m_dPileLoadV = m_dVerTotal * wSlab;
	m_dPileLoadH = m_dHorTotal * wSlab;
	m_dPileLoadMCen = m_dPileLoadV * m_dE; // �����߾��ۿ� ���Ʈ
//	m_dPileLoadM = m_dPileLoadV * (m_dPileLoadMCen/m_dPileLoadV-m_dDistCenttoDosim); // ���ҵ����ۿ� ���Ʈ
	// (ARCBRIDGE-3663) ��� E�� RCModule\LsdFootingDesign.cpp�� �ִ� LSD�������� ������ �Ҽ����Ʒ��� ���� Round�� �ؼ� ���Ʈ ���� ���� �ٸ��� ���ǰ� �ִ�. ���缭 Round�� ���ش�.
	double dE = dDistCen-(m_dMrTotal-m_dMoTotal)/m_dVerTotal;
	if(bLsd) dE = Round(dE, 0);
	m_dPileLoadM = m_dPileLoadV * dE;// ���ҵ����ۿ� ���Ʈ , ������ // (ARCBRIDGE-3174) ���� �������信 ���缭 ���Ʈ ��ȣ ����
}

void CLoadFooting::CalculateSupportForce()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	BOOL bLsd = pData->IsLsdDesign();
	double dBase	 = bLsd? m_pFooting->GetWidth() : toM(m_pFooting->GetWidth());
	double Q_Normal	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Q_EarthQ	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Qa		 = 0;
	double QaEquation= 0;
	double Q1		 = 0;
	double Q2		 = 0;
	double e		 = 0;
	double d		 = 0;
	double M		 = 0;
	double V		 = 0;
	double Lx		 = 0;
	double SF		 = 0;
	
	BOOL bPreCalcQa = FALSE;
	if(m_pFooting->m_Qa_Calculate[0] > 0 && m_pFooting->m_Qa_Calculate[1] >0)
	{
		// ���� ���� �ִ� ���� ��갪�� ����Ѵ�.....  ����������� �������� �ʴ´�.
		Q_Normal = pData->GetValueUnitType(m_pFooting->m_Qa_Calculate[0], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		Q_EarthQ = pData->GetValueUnitType(m_pFooting->m_Qa_Calculate[1], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);

		bPreCalcQa = TRUE;
	}
	
	
	M	    = m_dMrTotal - m_dMoTotal;
	V	    = m_dVerTotal;
	e		= dBase/2 - fabs(M/V);
	Lx		= 3.0*(dBase/2-fabs(e));
	d		= dBase/2 - e;

	if(e < dBase/6 && dBase/3 < d && d < dBase/2)
	{
		m_bQTriangle = FALSE;
		m_bQmaxFront = TRUE;
	}
	else if(e == dBase/6 && d == dBase/3)
	{
		m_bQTriangle = TRUE;
		m_bQmaxFront = TRUE;
	}
	else if(e > dBase/6 && d < dBase/3)
	{
		m_bQTriangle = TRUE;
		m_bQmaxFront = TRUE;
	}
	else if(e == 0 && d == dBase/2)
	{
		m_bQTriangle = FALSE;
		m_bQmaxFront = TRUE;
	}
	else if(fabs(e) < dBase/6 && dBase/2 < d && d < 2*dBase/3)
	{
		m_bQTriangle = FALSE;
		m_bQmaxFront = FALSE;
	}
	else
	{
		m_bQTriangle = TRUE;
		m_bQmaxFront = FALSE;
	}

	if(!m_bQTriangle) // ��ٸ��� ����
	{
		Q1 = V/dBase * (1+6*fabs(e)/dBase);
		Q2 = V/dBase * (1-6*fabs(e)/dBase);
	}
	else	// �ﰢ�� ����
	{
		Q1 = 2.0*V/Lx;
		Q2 = 0;
	}
	
	Qa = (m_bEarthQuake) ? Q_EarthQ : Q_Normal;
	SF = (m_bEarthQuake) ? 2 : 3;
	
	QaEquation = m_dQaEquation/SF;
	if(!bPreCalcQa) Qa = MIN(QaEquation,Qa);
	
	m_dLx	= Lx;
	m_dQmax = (Q1>Q2)? Q1:Q2;
	m_dQmin = (Q1>Q2)? Q2:Q1;
	m_dQa	= Qa;
	
	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
	{
		m_bOkSupportForce = ((m_dQmax + m_dQmaxMass) <= Qa) ? TRUE : FALSE;
	}
	else m_bOkSupportForce = (m_dQmax <= Qa) ? TRUE : FALSE;
}

void CLoadFooting::CalculateSlidingCheck()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);

	//////////////////////////////////////////////////////////////////////////
	double NValue	= pBoring->m_nNValue;
	double Pi		= m_pFooting->GetFrictionAngleBaseGround(NValue);
	double PiB		= 2.0/3.0 * Pi;
	double tanPiB	= (pBoring->GetBoringType() == TOSA) ? tan(ToRadian(PiB)) : 0.6;
	double Cb		= pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double V		= 0;
	double H		= 0;
	double Hu		= 0;
	double SF_EarQ	= m_pBri->m_SafeFactorSliding[1];
	double SF_Norm	= m_pBri->m_SafeFactorSliding[0];
	double SF_Apply = 0;
	
	V	     = m_dVerTotal;
	H		 = fabs(m_dHorTotal);
	Hu		 = Cb*m_dA_Equation + tanPiB*V;

	m_dHu	= Hu;
	SF_Apply = (m_bEarthQuake) ? SF_EarQ : SF_Norm;

	m_bOkSliding = ((Hu/H)>=SF_Apply) ? TRUE : FALSE;
}

void CLoadFooting::CalculateOverturningCheck()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	//////////////////////////////////////////////////////////////////////////

	double dBase	 = toM(m_pFooting->GetWidth());
	double e_Normal	 = dBase/6;
	double e_EarthQ	 = dBase/3;
	double e_Apply	 = 0;	// ������
	double e_Load	 = 0;	// ���������� ���
	double M_V		 = 0;
	double Moment	 = 0;
	double Mr		 = 0;
	double Mo		 = 0;
	double SF		 = 0;

	Mr	    = m_dMrTotal;
	Mo	    = m_dMoTotal;
	Moment  = Mr - Mo;
	M_V     = Moment/m_dVerTotal;
	e_Load  = dBase/2 - M_V;
	
	e_Apply = (m_bEarthQuake) ? e_EarthQ : e_Normal;
	SF		= (m_bEarthQuake) ? m_pBri->m_SafeFactorOverturning[1] : m_pBri->m_SafeFactorOverturning[0];
	
	// (ARCBRIDGE-3034) Mo�� - ���ϼ� �����Ƿ� SF�� ���밪���� ǥ��
	m_bOkOverturning = (fabs(e_Load)<=fabs(e_Apply) && SF<=fabs(Mr/Mo)) ? TRUE : FALSE;
}

// nType = 0: ��ü, 1: �ձ�, 2: �ޱ� 3: �ձ�������ġ, 4: �ޱ�������ġ, 5: �ձ�2, 6 : �ձ�3, 7 : ��ü (bMaxApply = FALSE)
void CLoadFooting::GetPileForcArr(long nType, BOOL bUnitM)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;
	
	m_dArrDistPile.RemoveAll();
	m_dArrPileFoce.RemoveAll();
	m_szAddPileNo.RemoveAll();
	m_dAddPileFoce.RemoveAll();
	m_szAddPileFoceString.RemoveAll();

	BOOL bLsd = pData->IsLsdDesign();
	CDesignPile *pDesignPile = m_pStd->GetDesignPile(m_nJijum);
	CExPileApp *pPile = &m_pFooting->m_exFooting.m_Pile;
	pPile->SetPileArray();

	// �Ѱ���� �϶��� m������ �������.
	BOOL bStt = (m_nJijum == 0)? TRUE:FALSE;
	double dDistMf = bLsd? toM(m_dDistMf) : m_dDistMf;
	double dDistMf2 = bLsd? toM(m_dDistMf2) : m_dDistMf2;
	double dDistMf3 = bLsd? toM(m_dDistMf3) : m_dDistMf3;
	double dDistMb = bLsd? toM(m_dDistMb) : m_dDistMb;
	double dDistSf = bLsd? toM(m_dDistSf) : m_dDistSf;				// ���������κ����� �ձ� ���ܸ� �Ÿ�
	double dDistSb = bLsd? toM(m_dDistSb) : m_dDistSb;				// ���������κ����� �ޱ� ���ܸ� �Ÿ�
	double dp = toM(pPile->m_dDia);
	double B = toM(m_pFooting->GetWidth());
	CDPoint vAng = m_pBri->GetAngleJijum(m_nJijum);
	double wFooting = bLsd? m_pFooting->GetLength() : toM(m_pFooting->GetLength());
	
	BOOL bAllPile = (nType == 0 || nType == 7) ? TRUE : FALSE;
	double dForce	= 0;
	double dForce_	= 0;
	double dReacFix	= 0;
	double dReacHinge = 0;
	double dReacApply = 0;
	double dDistRet = 0;
	double dDist	= (!bStt)? B:0;		// ������... ���ʺ��� ����ϱ� ������ ����,������ �ٸ��� ����..
	CString str		= _T("");

	long nCntPileCol = pPile->m_nCountOrg;
// 	long nPos;
	BOOL bMaxApply = (nType==2 || nType==4 || nType==7) ? FALSE : TRUE; // �ޱ�����ô� �ݷ��� ���� ���� �Ҹ�
	if(m_dPileLoadM<0 && (nType==2 || nType==4 || nType==7)) 
	{
		bMaxApply = !bMaxApply;		// 31625 : ���ҵ��� �ۿ���Ʈ�� �θ��Ʈ�� ��� �б������ �ݷ��� �������� �Ҹ�, 
	}

	long i=0; for(i=0; i<nCntPileCol; i++)
	{
		long nPos = i;
		if(bStt)
		{
			dDist += toM(pPile->m_ArrayOrg[nPos][0]);
			str.Format("R%d", i+1);
			dDistRet = dDist;
		}
		else 
		{
			double dDistSum =0;
			long j=0; for(j=0; j<(nCntPileCol-nPos); j++)
				dDistSum += toM(pPile->m_ArrayOrg[j][0]);

			dDist = B - dDistSum;
			str.Format("R%d", i+1);
			dDistRet += toM(pPile->m_ArrayOrg[nPos][0]);
		}

		if(nType == 1 && dDistRet > dDistMf) continue;
		if(nType == 2 && dDistRet < dDistMb) continue;
		if(nType == 3 && dDistRet > dDistSf+dp/2) continue;
		if(nType == 4 && dDistRet < dDistSb-dp/2) continue;
		if(nType == 5 && dDistRet > dDistMf2) continue;
		if(nType == 6 && dDistRet > dDistMf3) continue;

		long nIndexOffset(0);
		long nTypeCase = m_bUseLoad ? PILE_CASE_USE : PILE_CASE_ULTIMATE;
		if(bLsd)
		{
			switch(m_eEnvComboType)
			{
			case eEnvLimit:
				nTypeCase = PILE_CASE_ULTIMATE; break;
			case eEnvExtremeI:
			case eEnvExtremeII:
				nTypeCase = PILE_CASE_EXTREME; break;
			case eEnvService:
			case eEnvService_Crack:
				nTypeCase = PILE_CASE_USE; break;
			}

			if(m_eEnvComboType == eEnvService)
			{
				nIndexOffset = eCountFootCheck;
			}
		}
		dReacFix = pDesignPile->GetValueMaxMinPileHead(m_nIndex + nIndexOffset, PILE_FORCE_AXIAL, nPos, TRUE, TRUE, nTypeCase, FALSE);
		dReacHinge = pDesignPile->GetValueMaxMinPileHead(m_nIndex + nIndexOffset, PILE_FORCE_AXIAL, nPos, FALSE, TRUE, nTypeCase, FALSE);

		// - (36661) ���� ���ö��� ���밪�� ū�� �����ؾ� �Ѵ�.
		if(bMaxApply)
			dReacApply = (fabs(dReacFix) > fabs(dReacHinge))? dReacFix : dReacHinge;
		else
			dReacApply = (fabs(dReacFix) < fabs(dReacHinge))? dReacFix : dReacHinge;

		if(bUnitM) 
			dForce	= GetValueUnitChange(dReacApply, UNIT_CGS_TONF, pData->m_szTonf) * pPile->m_ArrayOrg[i][1] / wFooting;
		else 
			dForce	= GetValueUnitChange(dReacApply, UNIT_CGS_TONF, pData->m_szTonf);

		double d1 = 0;
		BOOL bAdd = FALSE;
		if((bAllPile || nType == 3) && dDistRet < dDistSf+dp/2 && dDistRet > dDistSf-dp/2)
		{
			d1 = (dDistSf+dp/2) - dDistRet;
			dForce_ = dForce * d1/dp;
			if(bAllPile) bAdd = TRUE;
			else dForce = dForce_;
		}
		else if((bAllPile || nType == 4) && dDistRet < dDistSb+dp/2 && dDistRet > dDistSb-dp/2)
		{
			d1 = dDistRet - (dDistSb-dp/2);
			dForce_ = dForce * d1/dp;
			if(bAllPile) bAdd = TRUE;
			else dForce = dForce_;
		}
		
		if(bAdd)
		{
			m_szAddPileNo.Add(str);
			m_dAddPileFoce.Add(dForce_);
			
			if(bLsd)
				str.Format("%.0f / %.0f", frM(d1), frM(dp));
			else
				str.Format("%.3f / %.3f", d1, dp);
			m_szAddPileFoceString.Add(str);
		}
		
		m_dArrDistPile.Add(bLsd? frM(dDistRet) : dDistRet);
		m_dArrPileFoce.Add(dForce);
	}
}

void CLoadFooting::CalculateSectionDesign()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;

	BOOL bLsd = pData->IsLsdDesign();
	long nPosTypeFront = m_pBri->m_nApplyCalcShearDistFront;
	long nPosTypeBack = m_pBri->m_nApplyCalcShearDistBack;

	BOOL bTrap = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (m_nJijum == 0 || m_nJijum == m_pBri->m_nQtyJigan));
	BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwBackFill = pData->GetValueUnitType(m_pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dLiveLoadAlways = pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);

	double B = bLsd? m_pFooting->GetWidth() : toM(m_pFooting->GetWidth());
	double H = bLsd? m_pFooting->GetHeight() : toM(m_pFooting->GetHeight());
	double Hf = bLsd? m_pFooting->GetHeight(FALSE) : toM(m_pFooting->GetHeight(FALSE));
	double wF = bLsd? ((bStt)? m_pFooting->m_dFWR:m_pFooting->m_dFWL) : toM((bStt)? m_pFooting->m_dFWR:m_pFooting->m_dFWL);	// �ձ� ����
	double wB = bLsd? ((bStt)? m_pFooting->m_dFWL:m_pFooting->m_dFWR) : toM((bStt)? m_pFooting->m_dFWL:m_pFooting->m_dFWR);	// �ޱ� ����
	double dF = Hf - (bLsd? m_pFooting->m_dCoverLower[0] : toM(m_pFooting->m_dCoverLower[0]));
	double dB = H - (bLsd? m_pFooting->m_dCoverUpper[0] : toM(m_pFooting->m_dCoverUpper[0]));

	m_ScF = 0;
	m_ScB = 0;
	m_McF = 0;
	m_McF2 = 0;
	m_McF3 = 0;
	m_McB = 0;
	m_SqF = 0;
	m_SqB = 0;
	m_MqF = 0;
	m_MqF2 = 0;
	m_MqF3 = 0;
	m_MqB = 0;
	m_SsB = 0;
	m_MsB = 0;
	m_SpB = 0;
	m_MpB = 0;
	m_SlB = 0;
	m_MlB = 0;			
	
	// ���������� ���� ���� �ܸ� ������ �Ÿ�
	if(bTrap)
	{
		m_dDistMf = 0;
		m_dDistMb = bLsd? frM(GetDistSlopeBack()) : GetDistSlopeBack();
		m_dDistSf = m_dDistMf;
		
		if(nPosTypeBack == 0) m_dDistSb = m_dDistMb;
		else if(nPosTypeBack == 1) m_dDistSb = m_dDistMb+dB;
		else if(nPosTypeBack == 2) m_dDistSb = m_dDistMb+dB/2;
	}
	else if(bTrap2 || bArch)
	{
		GetDistFrontTrap2AndArch(m_dDistMf, m_dHgtSf);
		m_dDistMf2 = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE || m_pFooting->m_nType==FOOTING_TYPE_BASIC_LEFTSLOPE)? toM(m_pFooting->m_dFELW) : toM(m_pFooting->m_dFERW);
		if(bLsd)
		{
			m_dDistMf = frM(m_dDistMf);
			m_dHgtSf = frM(m_dHgtSf);
			m_dDistMf2 = frM(m_dDistMf2);
		}

		dF = m_dHgtSf - (bLsd? m_pFooting->m_dCoverLower[0] : toM(m_pFooting->m_dCoverLower[0]));
		if(nPosTypeFront == 0) m_dDistSf = m_dDistMf;
		else if(nPosTypeFront == 1) m_dDistSf = m_dDistMf-dF;
		else if(nPosTypeFront == 2) m_dDistSf = m_dDistMf-dF/2;

		double dWallThick = bLsd? (m_pFooting->m_dML + m_pFooting->m_dMR) : toM(m_pFooting->m_dML + m_pFooting->m_dMR);
		m_dDistMf3 = B - (wB + dWallThick);

		m_dDistMb = B-wB;
		if(nPosTypeBack == 0) m_dDistSb = m_dDistMb;
		else if(nPosTypeBack == 1) m_dDistSb = m_dDistMb+dB;
		else if(nPosTypeBack == 2) m_dDistSb = m_dDistMb+dB/2;
	}
	else
	{
		m_dDistMf = wF;
		if(nPosTypeFront == 0) m_dDistSf = m_dDistMf;
		else if(nPosTypeFront == 1) m_dDistSf = m_dDistMf-dF;
		else if(nPosTypeFront == 2) m_dDistSf = m_dDistMf-dF/2;
		
		m_dDistMb = B-wB;
		if(nPosTypeBack == 0) m_dDistSb = m_dDistMb;
		else if(nPosTypeBack == 1) m_dDistSb = m_dDistMb+dB;
		else if(nPosTypeBack == 2) m_dDistSb = m_dDistMb+dB/2;
	}

	m_bOutBoundFootFront = (m_dDistSf < 0)? TRUE : FALSE;
	m_bOutBoundFootBack	 = (m_dDistSb > B)? TRUE : FALSE;

	double B_ = (m_bQTriangle)? m_dLx:B;
	// �� ���� �ܸ鿡���� q
	if(m_bQmaxFront)
	{
		m_dQmF		= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistMf)/B_));
		m_dQmF2	= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistMf2)/B_));
		m_dQmF3	= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistMf3)/B_));
		m_dQmB		= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistMb)/B_));
		m_dQvF		= m_bOutBoundFootFront? 0 : MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistSf)/B_));
		m_dQvB		= m_bOutBoundFootBack? 0 : MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((B_-m_dDistSb)/B_));
	}
	else if(!m_bQmaxFront)
	{
		double dC = B-B_;
		m_dQmF		= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistMf-dC)/B_));
		m_dQmF2	= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistMf2-dC)/B_));
		m_dQmF3	= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistMf3-dC)/B_));
		m_dQmB		= MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistMb-dC)/B_));
		m_dQvF		= m_bOutBoundFootFront? 0 : MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistSf-dC)/B_));
		m_dQvB		= m_bOutBoundFootBack? 0 : MAX(0, m_dQmin + (m_dQmax-m_dQmin)*((m_dDistSb-dC)/B_));
	}

	// ���� ���߿� ���� �ܸ��
	m_ScF  = m_dDistSf * Hf * dUwConc * m_dFactorDeadLoad;
	m_ScB  = (B-m_dDistSb) * H * dUwConc * m_dFactorDeadLoad;
	m_McF  = m_dDistMf * Hf * dUwConc * m_dFactorDeadLoad * m_dDistMf/2;
	m_McF2 = m_dDistMf2 * Hf * dUwConc * m_dFactorDeadLoad * m_dDistMf2/2;
	m_McF3 = m_dDistMf3 * Hf * dUwConc * m_dFactorDeadLoad * m_dDistMf3/2;
	m_McB = (B-m_dDistMb) * H * dUwConc * m_dFactorDeadLoad * (B-m_dDistMb)/2;
	// ��ä�� ��� �� ���忡 ���� �ܸ��
	if(m_nJijum == 0 || m_nJijum == m_pBri->GetCountJijum()-1)
	{
		m_SpB = (B-m_dDistSb)*((m_dFactorPave==0) ? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave)/(B-m_dDistMb)) * m_dFactorPave;
		m_MpB = (pow(B-m_dDistMb, 2)/2*((m_dFactorPave==0)? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave)/(B-m_dDistMb))) * m_dFactorPave;

		m_SsB = ((B-m_dDistSb)*m_dHightBackFill*dUwBackFill) * m_dFactorSoilHor;
		m_MsB = (pow(B-m_dDistMb, 2)/2*m_dHightBackFill*dUwBackFill) * m_dFactorSoilHor;
	}
	// ���Ȱ���߿� ���� �ܸ��
	if(!m_bEarthQuake && (m_nJijum == 0 || m_nJijum == m_pBri->GetCountJijum()-1))
	{
		m_SlB = dLiveLoadAlways * (B-m_dDistSb) * m_dFactorLiveLoad;
		m_MlB = dLiveLoadAlways * pow(B-m_dDistMb, 2)/2 * m_dFactorLiveLoad;
	}
	// ���� �ݷ¿� ���� �ܸ��
	if(m_pBri->IsPileBase(m_nJijum))
	{
		long nCnt = 0;
		double dj = 0;
		double dForce = 0;
		
		
		GetPileForcArr(1);
		nCnt = m_dArrPileFoce.GetSize();
		long i=0; for(i=0; i<nCnt; i++)
		{
			dForce = m_dArrPileFoce.GetAt(i);
			dj = m_dDistMf - m_dArrDistPile.GetAt(i);
			m_MqF += dForce * dj;
		}
		
		GetPileForcArr(2);
		nCnt = m_dArrPileFoce.GetSize();
		for(i=0; i<nCnt; i++)
		{
			dForce = m_dArrPileFoce.GetAt(i);
			dj = m_dArrDistPile.GetAt(i) - m_dDistMb;
			m_MqB += dForce * dj;
		}
		
		GetPileForcArr(3);
		nCnt = m_dArrPileFoce.GetSize();
		for(i=0; i<nCnt; i++)
		{
			dForce = m_dArrPileFoce.GetAt(i);
			m_SqF += dForce;
		}
		
		GetPileForcArr(4);
		nCnt = m_dArrPileFoce.GetSize();
		for(i=0; i<nCnt; i++)
		{
			dForce = m_dArrPileFoce.GetAt(i);
			m_SqB += dForce;
		}

		// �ձ��ּҴܸ� ���� �߰���
		if(m_bTwoUpperLoad)
		{
			GetPileForcArr(5);
			nCnt = m_dArrPileFoce.GetSize();
			long i=0; for(i=0; i<nCnt; i++)
			{
				dForce = m_dArrPileFoce.GetAt(i);
				dj = m_dDistMf2 - m_dArrDistPile.GetAt(i);
				m_MqF2 += dForce * dj;

				dj = m_dDistMf3 - m_dArrDistPile.GetAt(i);
				m_MqF3 += dForce * dj;
			}
		}
	}
	else
	{
		// ������ġ�� ���ݹݷ� �ۿ���ġ �ۿ� ������� ���̳ʽ��� ������ �ȵȴ�. �׳� 0���ش�..
		if(m_bQmaxFront)
		{
			m_SqF = MAX(0, (m_dQvF*m_dDistSf + (m_dQmax-m_dQvF)*m_dDistSf/2));
			m_MqF = (m_dQmF*pow(m_dDistMf, 2)/2 + (m_dQmax-m_dQmF)*pow(m_dDistMf, 2)/2*(2.0/3));
			m_MqF2 = (m_dQmF2*pow(m_dDistMf2, 2)/2 + (m_dQmax-m_dQmF2)*pow(m_dDistMf2, 2)/2*(2.0/3));
			m_MqF3 = (m_dQmF3*pow(m_dDistMf3, 2)/2 + (m_dQmax-m_dQmF3)*pow(m_dDistMf3, 2)/2*(2.0/3));
			if(m_bQTriangle)
			{
				m_SqB = MAX(0, m_dQvB*(m_dLx-m_dDistSb)/2);
				m_MqB = m_dQmB*pow((m_dLx-m_dDistMb),2)/2*(1.0/3);
			}
			else
			{
				m_SqB = MAX(0, (m_dQmin*(B-m_dDistSb) + (m_dQvB-m_dQmin)*(B-m_dDistSb)/2));
				m_MqB = (m_dQmin*pow((B-m_dDistMb), 2)/2 + (m_dQmB-m_dQmin)*pow((B-m_dDistMb), 2)/2*(1.0/3));
			}
		}
		else
		{
			m_SqB = MAX(0, (m_dQvB*(B-m_dDistSb) + (m_dQmax-m_dQvB)*(B-m_dDistSb)/2));
			m_MqB = (m_dQmB*pow((B-m_dDistMb), 2)/2 + (m_dQmax-m_dQmB)*pow((B-m_dDistMb), 2)/2*(2.0/3));

			if(m_bQTriangle)
			{
				m_SqF = MAX(0, m_dQvF*(m_dLx-(B-m_dDistSf))/2);
				m_MqF = m_dQmF*pow(m_dLx-(B-m_dDistMf), 2)/2*(1.0/3);
				m_MqF2 = m_dQmF2*pow(m_dLx-(B-m_dDistMf2), 2)/2*(1.0/3);
				m_MqF3 = m_dQmF3*pow(m_dLx-(B-m_dDistMf3), 2)/2*(1.0/3);
			}
			else
			{
				m_SqF = MAX(0, (m_dQmin*m_dDistSf + (m_dQvF-m_dQmin)*m_dDistSf/2));	
				m_MqF = (m_dQmin*pow(m_dDistMf, 2)/2 + (m_dQmF-m_dQmin)*pow(m_dDistMf, 2)/2*(1.0/3));
				m_MqF2 = (m_dQmin*pow(m_dDistMf2, 2)/2 + (m_dQmF2-m_dQmin)*pow(m_dDistMf2, 2)/2*(1.0/3));
				m_MqF3 = (m_dQmin*pow(m_dDistMf3, 2)/2 + (m_dQmF3-m_dQmin)*pow(m_dDistMf3, 2)/2*(1.0/3));
			}
		}
	}
	
	// ���� �ܸ�� ����
	m_dVuFront	= m_bOutBoundFootFront? 0 : m_SqF - m_ScF;
	m_dMuFront	= m_MqF - m_McF;
	m_dMuFront2	= (m_bTwoUpperLoad)? (m_MqF2 - m_McF2) : 0;
	m_dMuFront3	= (m_bTwoUpperLoad)? (m_MqF3 - m_McF3) : 0;
	m_dVuBack	= m_bOutBoundFootBack? 0 : m_ScB + m_SsB + m_SpB + m_SlB - m_SqB;
	m_dMuBack	= m_McB + m_MsB + m_MpB + m_MlB - m_MqB;
	//////////////////////////////////////////////////////////////////////////

}

void CLoadFooting::WriteCheckOverturning(CXLFormatText *XLText, long nIndexSub, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+05$h$i%s$r������ ���� ����$n$h", GetNumberString(nIndexSub,NUM_GANA));
	if(bFirst)
	{
		XLText->AddFormatText("$h$c-$r$e�������ʿ� �ۿ��ϴ� ������ �շ��� �ۿ��ϴ� ��ġ�� ���ÿ��� �ٴ��� �߽����κ��� �ٴ���$n$h");
		XLText->AddFormatText("$h$r$e���� 1/6 �̳�, �����ÿ� ��ǳ�ÿ��� �ٴ��� ���� 1/3�̳��� �־�� �Ѵ�.$n$h");
		XLText->AddFormatText("$h$r$e[���α�������� 5.6.2]$n$h");
		XLText->AddFormatText("$h$c-$r$e���� ������ ���� ���׸��Ʈ�� �������Ʈ�� 2.0�� �̻��̾�� �Ѵ�.$n$h");
		XLText->AddFormatText("$h$c-$r$e������ ������ ���� ���׸��Ʈ�� �������Ʈ�� 1.5�� �̻��̾�� �Ѵ�.$n$h");
	}
	double dB = toM(m_pFooting->GetWidth());
	double dM = m_dMrTotal - m_dMoTotal;
	double dEa = (m_bEarthQuake) ? dB/3 : dB/6;
	double dSF = fabs(m_dMrTotal/m_dMoTotal);
	double dSFa = (m_bEarthQuake)? m_pBri->m_SafeFactorOverturning[1] : m_pBri->m_SafeFactorOverturning[0];
	CString szEa = (m_bEarthQuake) ? "B/3" : "B/6";
	CString szOkNgE = (fabs(m_dE) <= dEa)? "O.K":"N.G";
	CString szOkNgSF = (dSF >= dSFa)? "O.K":"N.G";
	CString szCompE = (fabs(m_dE) <= dEa)? "<":">";
	CString szCompSF = (dSF >= dSFa)? ">":"<";
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$c��M$r$c=$r$e��Mr - ��Mo$m+03$c=$r$e%.3f - %.3f$m+06$c=$r$e$f3%.3f$me2$m+02%s$n", m_dMrTotal, m_dMoTotal, dM, pData->m_szTonfM);
	XLText->AddFormatText("$h$ce$r$c=$r$eB/2 - ��M/��V$m+04$c=$r$e%.3f/2 - %.3f/%.3f$n", dB, dM, m_dVerTotal);
	XLText->AddFormatText("$h$r$c=$r$e$f3%.3f$me1$m+01m$m+02$c|e|$r$c%s$m+02$e%s$m+02$c=$r$e$f3%.3f$me1$m+03$c��$r", m_dE, szCompE, szEa, dEa);
	if(fabs(m_dE) > dEa) XLText->AddFormatText("$e$a$#03%s$n", szOkNgE);
	else XLText->AddFormatText("$e$a%s$n", szOkNgE);
	// (ARCBRIDGE-3034) Mo�� - ���ϼ� �����Ƿ� SF�� ���밪���� ǥ��
	XLText->AddFormatText("$h$cS.F$r$c=$r$e��Mr/��Mo$m+04$c=$r$e%.3f/%.3f$m+05$c=$r$e$f1%.1f$me1$m+02$c%s$m+02$e$f1%.1f $m+02$c��$r", m_dMrTotal, m_dMoTotal, dSF, szCompSF, dSFa, szOkNgSF);
	// if(m_dMoTotal > 0) XLText->AddFormatText("$h$cS.F$r$c=$r$e��Mr/��Mo$m+04$c=$r$e%.3f/%.3f$m+05$c=$r$e$f1%.1f$me1$m+02$c%s$m+02$e$f1%.1f $m+02$c��$r", m_dMrTotal, m_dMoTotal, dSF, szCompSF, dSFa, szOkNgSF);
	// else XLText->AddFormatText("$h$cS.F$r$c=$r$e��Mr/��Mo$m+04$c=$r$e%.3f/%.3f$m+05$c=$r$e��$m+02%s$m+02$e$f1%.1f $m+02$c��$r", m_dMrTotal, m_dMoTotal, szCompSF, dSFa);
	if(dSF < dSFa) XLText->AddFormatText("$e$a$#03%s$n", szOkNgSF);
	else XLText->AddFormatText("$e$a%s$n", szOkNgSF);
	XLText->AddFormatText("$n$h");
}

void CLoadFooting::WriteCheckSliding(CXLFormatText *XLText, long nIndexSub, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+05$h$i%s$e$rȰ���� ���� ����$n$h", GetNumberString(nIndexSub,NUM_GANA));
	if(bFirst)
	{
		XLText->AddFormatText("$h$c-$r$e�������� �ٴڸ鿡���� �������ݹݷ��� �ٴڸ� �Ʒ� ���������� ����������׷� �̳���$n$h");
		XLText->AddFormatText("$h$r$e�־�� �Ѵ�. [���α�������� 5.6.2]$n$h");
	}
	XLText->AddFormatText("$h$r$p[FOOTING_SLID]$n");
	if(bFirst)
	{
		XLText->AddFormatText("$h$r$eHu$r$c:$r$e��������� ���ݰ��� ���̿� �ۿ��ϴ� ���� ���׷�(%s)$n$h", pData->m_szTonf);
		XLText->AddFormatText("$h$r$eCb$r$c:$r$e��������� ���ݰ��� ������(%s)$n$h", pData->m_szTonf_M2);
		XLText->AddFormatText("$h$r$e��b$r$c:$r$e��������� ���ݰ��� ������(degree)$n$h");
		XLText->AddFormatText("$h$r$eA'$r$c:$r$e���������� ��ȿ���ϸ���(��)$n$h");
		XLText->AddFormatText("$h$r$eV$r$c:$r$e�������鿡 �ۿ��ϴ� ��������(%s), �ٸ� �η��� �� ������ �Ѵ�.$n$h", pData->m_szTonf);
		XLText->AddFormatText("$n$h");
	}

	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);
	//////////////////////////////////////////////////////////////////////////
	double NValue	= pBoring->m_nNValue;
	double PiB		= 2.0/3.0 * m_pFooting->GetFrictionAngleBaseGround(NValue);
	double tanPiB	= (pBoring->GetBoringType() == TOSA) ? tan(ToRadian(PiB)) : 0.6;
	double Cb		= pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Hu		= m_dHu;
	double SF		= Hu/fabs(m_dHorTotal);
	double SF_Apply = (m_bEarthQuake)? m_pBri->m_SafeFactorSliding[1]:m_pBri->m_SafeFactorSliding[0];

	CString szEque = m_pFooting->GetStrignFrictionAngleEquation();
	CString szOkNg = (m_bOkSliding)? "O.K":"N.G";
	CString szComp = (m_bOkSliding)? ">":"<";
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	if(szEque.IsEmpty())
		XLText->AddFormatText("$h$e��b$r$c=$r$e%.3f x 2/3$m+03$c=$r$e$f3%.3f, $m+03$eN = %.0f$n", m_pFooting->GetFrictionAngleBaseGround(NValue), PiB, NValue);
	else
		XLText->AddFormatText("$h$e��b$r$c=$r$e(%s) x 2/3$m+05$c=$r$e$f3%.3f, $m+03$eN = %.0f$n", szEque, PiB, NValue);

	XLText->AddFormatText("$h$etan��b$m+02$c=$r$etan(%.3f)$m+04$c=$r$e%.3f, $m+03$cCb$r$c=$r$e$f3%.3f$me1$n", PiB, tanPiB, Cb);
	if(Cb == 0) XLText->AddFormatText("$h$eHu$r$c=$r$e$f3%.3f x %.3f$m+05$c=$r$e%.3f$me1$m+02%s$n", m_dVerTotal, tanPiB, Hu, pData->m_szTonf);
	else XLText->AddFormatText("$h$eHu$r$c=$r$e%.3f x %.3f + %.3f x %.3f$m+09$c=$r$e%.3f$me1$m+02%s$n", Cb, m_dA_Equation, m_dVerTotal, tanPiB, Hu, pData->m_szTonf);

	XLText->AddFormatText("$h$eS.F$r$c=$r$eHu / H$m+03$c=$r$e%.3f / %.3f$m+05$c=$e$r%.2f$me1$m+02$c%s$m+02$e$f1%.1f$m+02$c��$r", Hu, fabs(m_dHorTotal), SF, szComp, SF_Apply);
	if(!m_bOkSliding) XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
	else XLText->AddFormatText("$e$a%s$n", szOkNg);
	XLText->AddText("$n$h");
}

void CLoadFooting::WriteCheckSupportForce(CXLFormatText *XLText, long nIndexSub, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	long nType = m_pFooting->m_nTypeEquation_Qa;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+05$h$i%s$r�����¿� ���� ����$n$h", GetNumberString(nIndexSub,NUM_GANA));
	if(bFirst)
	{
		XLText->AddFormatText("$h$c-$r$e�������� �ٴڸ鿡���� �������ݹݷ��� �ٴھƷ� ���������� ��뿬�������� �̳���$n$h");
		XLText->AddFormatText("$h$r$e�־�� �Ѵ�.[���α�������� 5.6.2]$n$h");
	}

	double B = toM(m_pFooting->GetWidth());

	CString szTemp = (m_bQTriangle)? "�ﰢ�� ����":"��ٸ��� ����";
	CString szComp = (m_bQTriangle)? "<":">";
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$cX$r$c=$r$e3 x [B/2-|e|]$n");
	XLText->AddFormatText("$h$r$c=$r$e3 x [%.3f/2 - %.3f]$m+06$c=$r$e%.3f$me1$m+02m$c$r%s$r$eB = %.3f m$m+04$c��$r$e%s$n", B, fabs(m_dE), m_dLx, szComp, B, szTemp);
	//////////////////////////////////////////////////////////////////////////
	// �׸� �־�ߵ�...
	long nRow = (m_bQTriangle)? 11:10;
	CDomyun *Dom = new CDomyun;
	DrawSupportForce(Dom, -1, FALSE);
	XLText->AddFormatText("$s+09$n$h$r");
	XLText->InsertDomyun(Dom, m_pStd->m_pView, nRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	XLText->AddFormatText("$u$u$u$u$u");
	if(m_bQTriangle)
	{
		XLText->AddFormatText("$h$t$t$r$r$r$p[FOOTING_QMAX2]$n$n$n$n$n");
		XLText->AddFormatText("$h$cqmax$r$c=$r$e(2 x %.3f) / (%.3f x %.3f)$m+09$c=$r$e%.3f$me2$m+03%s$n", m_dVerTotal, UNIT_METER, m_dLx, m_dQmax, pData->m_szTonf_M2);
	}
	else
	{
		
		XLText->AddFormatText("$h$t$t$r$r$r$p[FOOTING_QMAX1]$n$n$n$n$n");
		XLText->AddFormatText("$h$cqmax$r$c=$r$e%.3f/%.3f x (1 + (6 x %.3f)/%.3f)$m+11$c=$r$e%.3f$me2$m+03%s$n", m_dVerTotal, B, fabs(m_dE), B, m_dQmax, pData->m_szTonf_M2);
		XLText->AddFormatText("$h$cqmin$r$c=$r$e%.3f/%.3f x (1 - (6 x %.3f)/%.3f)$m+11$c=$r$e%.3f$me2$m+03%s$n", m_dVerTotal, B, fabs(m_dE), B, m_dQmin, pData->m_szTonf_M2);
	}
	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
	{
		XLText->AddFormatText("$h$c-$r$eMASS������ �ۿ���� ����Ͽ� Qmax = %.3f + %.3f = %.3f %s$n", m_dQmax, m_dQmaxMass, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
	}


	if(m_pFooting->m_Qa_Calculate[0] > 0 && m_pFooting->m_Qa_Calculate[1] >0)
	{
		// ���� ���� �ִ� ���� ��갪�� ����Ѵ�.....  ����������� �������� �ʴ´�.
		double Q_Normal = pData->GetValueUnitType(m_pFooting->m_Qa_Calculate[0], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Q_EarthQ = pData->GetValueUnitType(m_pFooting->m_Qa_Calculate[1], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		double Qa	= (m_bEarthQuake)? Q_EarthQ:Q_Normal;

		CString szComp2 = (m_bOkSupportForce)? ">":"<";
		CString szOkNg = (m_bOkSupportForce)? "O.K":"N.G";

		if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
		{
			XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa, pData->m_szTonf_M2, szComp2, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
		}
		else XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa, pData->m_szTonf_M2, szComp2, m_dQmax, pData->m_szTonf_M2);
		
		if(!m_bOkSupportForce) XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
		else XLText->AddFormatText("$e$a%s$n", szOkNg);

		XLText->AddFormatText("$h$c-$r������ ��뿬�������� ������ ����÷�ڷ᡻�� ����.$n");
	}
	else
	{
		// Terzaghi //////////////////////////////////////////////////
		if(nType == 0)
		{
			WriteCalcQubyTerzaghi(XLText, bFirst);
		}
		// Meyerhof //////////////////////////////////////////////////
		// ��ȿ���ϸ����� ������� �ʴ´�.....
		if(nType==1)
		{
			WriteCalcQubyMeyerhof(XLText, bFirst);
		}
		// Hansen   //////////////////////////////////////////////////
		if(nType==2)
		{
			WriteCalcQubyHansen(XLText, bFirst);
		}
		XLText->AddText("$n$h");
	}
}

void CLoadFooting::WriteCalcQubyHansen(CXLFormatText *XLText, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);
	//////////////////////////////////////////////////////////////////////////

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$n$h$c-$r$e������ ��뿬�������� : Hansen�� ���$n$h");
	XLText->GoPosition(NO_MOVE,4);
	XLText->AddFormatText("$h$cQu$r$c=$r$eA'[c Nc sc dc ic gc bc + q Nq sq dq iq gq bq + 1/2 r1 B' Nr sr dr ir gr br]$n$h");
	XLText->AddFormatText("$h$r$e��=0 �϶�, Qu = A' [5.14c (1+ sc' + dc' - ic' - gc' -bc') + q]$n");

	if(bFirst)
	{
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
	}

	double NValue = pBoring->m_nNValue;
	double pi = ToRadian(m_pFooting->GetFrictionAngleBaseGround(NValue));
	double piDeg = m_pFooting->GetFrictionAngleBaseGround(NValue);
	double L  = toM(m_pFooting->GetLength());
	double B  = toM(m_pFooting->GetWidth());
	double Df = m_pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
	double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);		// ���������� ������
	double r1 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double r2 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double beta = 0; // ���ݰ��..
	// Qu = A'[c*Nc*Sc*dc*ic*gc*bc + q*Nq*Sq*dq*iq*gq*bq + 1/2*r1*B'*Nr*Sr*dr*ir*gr*br]
	double eB	= m_dE;
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
	double SF = (m_bEarthQuake) ? 2 : 3;

	double Q_Normal	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Q_EarthQ	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Qa	= (m_bEarthQuake)? Q_EarthQ:Q_Normal;
	
	double Qe	= m_dQaEquation/SF;
	double Qa_	= MIN(Qa, Qe);
	CString szPiEqu = m_pFooting->GetStrignFrictionAngleEquation();
	CString szComp1 = (Qe > Qa)? ">":"<";
	CString szComp2 = (m_bOkSupportForce)? ">":"<";
	CString szOkNg = (m_bOkSupportForce)? "O.K":"N.G";

	if(m_nIndex == 0)
	{
		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$ic$r$c=$r$e%.3f %s$m+05r1$r$c=$r$e%.3f %s$m+05r2$r$c=$r$e%.3f %s$n", c, pData->m_szTonf_M2, r1, pData->m_szTonf_M3, r2, pData->m_szTonf_M3);
		XLText->AddFormatText("$h$iDf$r$c=$r$e%.3f m$m+05B$r$c=$r$e%.3f m$m+05q$r$c=$r$e%.3f %s$n", Df,B, q, pData->m_szTonf_M2);

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
	}
	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", m_dE);
	XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(m_dE), B_);

	XLText->GoPosition(NO_MOVE,COL_POS_3);	
	XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e[ %.3f x %.3f x %.3f x %.3f x %.3f$n", c, Nc, Sc, dc, gc);
	XLText->AddFormatText("$h$e$r$r$e + %.3f x %.3f x %.3f x %.3f x %.3f$n", q, Nq, Sq, dq, gq);
	XLText->AddFormatText("$h$e$r$r$e + 1/2 x %.3f x %.3f x %.3f x %.3f x %.3f x %.3f$n", r1, B_, Nr, Sr, dr, gr);
	XLText->AddFormatText("$h$e$r$c=$r$e%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);
	XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.0f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);
	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
	{
		XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
	}
	else XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax, pData->m_szTonf_M2);
	if(!m_bOkSupportForce) XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
	else XLText->AddFormatText("$e$a%s$n", szOkNg);
}
void CLoadFooting::WriteCalcQubyTerzaghi(CXLFormatText *XLText, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);
	//////////////////////////////////////////////////////////////////////////
	double NValue = pBoring->m_nNValue;
	double pi = ToRadian(m_pFooting->GetFrictionAngleBaseGround(NValue));
	double piDeg = m_pFooting->GetFrictionAngleBaseGround(NValue);
	double L  = toM(m_pFooting->GetLength());
	double B  = toM(m_pFooting->GetWidth());
	double Df = m_pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
	double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
	double r1 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double r2 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

	// �������鿡�� ���ϼ��������� ����(dH)�� ������ ��(B_)���� ������쿡�� r1�� r_�� ����Ѵ�			
	double q	= r2*Df; // ????
	////////////////////////////////////////////////
	double eB	= m_dE;
	double B_	= B - 2*fabs(eB);
	double alpha= 1.0 + 0.3*B_/L; // ���簢��
	double beta = 0.5 - 0.1*B_/L;
	double Kpr	= GetKprValue(m_pFooting->GetFrictionAngleBaseGround(pBoring->m_nNValue));
	double  a   = exp((0.75*ConstPi-pi/2)*tan(pi));
	double Nq	= pow(a,2) / (2*pow(cos(ConstPi/4+pi/2),2));
	double Nc	= (Nq-1)/tan(pi);
	double Nr	= tan(pi)/2 * (Kpr/pow(cos(pi),2) - 1);
	double SF = (m_bEarthQuake) ? 2 : 3;
	
	double Q_Normal	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Q_EarthQ	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Qa	= (m_bEarthQuake)? Q_EarthQ:Q_Normal;
	
	double Qe	= m_dQaEquation/SF;
	double Qa_	= MIN(Qa, Qe);
	CString szPiEqu = m_pFooting->GetStrignFrictionAngleEquation();
	CString szComp1 = (Qe > Qa)? ">":"<";
	CString szComp2 = (m_bOkSupportForce)? ">":"<";
	CString szOkNg = (m_bOkSupportForce)? "O.K":"N.G";

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$n$h$c-$r$e������ ��뿬�������� : Terzaghi�� ���$n$h");
	XLText->GoPosition(NO_MOVE,4);
	XLText->AddFormatText("$h$cQu$r$c=$r$e[ ��xcxNc + qxNq + ��xr1xB_xNr ]$n$h");
	

	if(bFirst)
	{
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
	}
	
	if(m_nIndex == 0)
	{
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
	}

	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", m_dE);
	XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(m_dE), B_);
	XLText->AddFormatText("$h$i��$r$c=$r$e1.0 + 0.3x(B'/L)$m+08$c=$r$e$f3%.3f$me1$n", alpha);
	XLText->AddFormatText("$h$i��$r$c=$r$e0.5 + 0.1x(B'/L)$m+08$c=$r$e$f3%.3f$me1$n", beta);

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e[ %.3f x %.3f x %.3f + %.3f x %.3f + %.3f x %.3f x %.3f x %.3f ]$n", alpha, c, Nc, q, Nq, beta, r1, B_, Nr);
	XLText->AddFormatText("$h$e$r$c=$r$e%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);
	XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.3f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);

	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
	{
		XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
	}
	else XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax, pData->m_szTonf_M2);

	if(!m_bOkSupportForce) XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
	else XLText->AddFormatText("$e$a%s$n", szOkNg);
}//m_dQaEquation = ( alpha*c*Nc + q*Nq + beta*r1*B_*Nr );

void CLoadFooting::WriteCalcQubyMeyerhof(CXLFormatText *XLText, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;
	CBoringData *pBoring = m_pBri->GetBoringDataJijumBase(m_nJijum);
	
	double NValue = pBoring->m_nNValue;
	double pi = ToRadian(m_pFooting->GetFrictionAngleBaseGround(NValue));
	double piDeg = m_pFooting->GetFrictionAngleBaseGround(NValue);
	double B  = toM(m_pFooting->GetWidth());
	double Df = m_pBri->m_dValidDepthBaseGround;	// ������ ��ȿ���Ա���
	double c  = pData->GetValueUnitType(pBoring->m_c, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);						// ������ ������
	double r1 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.
	double r2 = pData->GetValueUnitType(m_pFooting->m_dUnitWeigthBaseGround, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);	// ���������� �����߷�, �� ���ϼ��� ���Ͽ����� ���ߴ����߷��� ����Ѵ�.

	double q	= r2*Df; // ????
	double B_	= B;
	double Nq	= exp(ConstPi*tan(pi)) * pow(tan(ConstPi/4+pi/2),2);
	double Nc	= (pi==0) ? 5.14 : (Nq-1)/tan(pi);
	double Nr	= (Nq-1)*tan(1.4*pi);
	double SF = (m_bEarthQuake) ? 2 : 3;
	
	double Q_Normal	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Q_EarthQ	 = pData->GetValueUnitType(m_pFooting->m_QaMax_BaseGround*1.5, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double Qa	= (m_bEarthQuake)? Q_EarthQ:Q_Normal;
	
	double Qe	= m_dQaEquation/SF;
	double Qa_	= MIN(Qa, Qe);
	CString szPiEqu = m_pFooting->GetStrignFrictionAngleEquation();
	CString szComp1 = (Qe > Qa)? ">":"<";
	CString szComp2 = (m_bOkSupportForce)? ">":"<";
	CString szOkNg = (m_bOkSupportForce)? "O.K":"N.G";

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$n$h$c-$r$e������ ��뿬�������� : Meyerhof�� ���$n$h");
	XLText->GoPosition(NO_MOVE,4);
	XLText->AddFormatText("$h$cQu$r$c=$r$e[ cxNc + qxNq + 0.5xr1xB_xNr ]$n$h");

	if(bFirst)
	{
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
	}
	if(m_nIndex == 0)
	{
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
	}
	
	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$h$ie$r$c=$r$e%.3f m$n", m_dE);
	XLText->AddFormatText("$h$iB'$r$c=$r$eB - 2e$m+02$c=$r$e%.3f - 2 x %.3f$m+05$c=$r$e%.3f m$n", B, fabs(m_dE), B_);
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$s+07$h$eQu$r$c=$r$e[ %.3f x %.3f + %.3f x %.3f + 0.5 x %.3f x %.3f x %.3f ]$n", c, Nc, q, Nq, r1, B_, Nr);
	XLText->AddFormatText("$h$e$r$c=$r$e%.3f$me2$m+03%s$n", m_dQaEquation, pData->m_szTonf_M2);
	XLText->AddFormatText("$h$eQa$r$c=$r$e%.3f / %.3f$m+04$c=$r$e%.3f %s$m+05$c%s$r$r$e%.3f %s$n", m_dQaEquation, SF, Qe, pData->m_szTonf_M2, szComp1, Qa, pData->m_szTonf_M2);
	if(m_pStd->m_pBri->m_bApplyMassCalcSafe && m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
	{
		XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax+m_dQmaxMass, pData->m_szTonf_M2);
	}
	else XLText->AddFormatText("$h$eQa'$r$c=$r$e%.3f %s$m+05$c%s$m+02$eQmax = %.3f %s$m+07$c��$r", Qa_, pData->m_szTonf_M2, szComp2, m_dQmax, pData->m_szTonf_M2);
	if(!m_bOkSupportForce) XLText->AddFormatText("$e$a$#03%s$n", szOkNg);
	else XLText->AddFormatText("$e$a%s$n", szOkNg);

}		// m_dQaEquation = ( c*Nc + q*Nq + 0.5*r1*B_*Nr )

void CLoadFooting::WriteSectionDesign(CXLFormatText *XLText, BOOL bUse, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	long nIndexSub = 1;
	
	//////////////////////////////////////////////////////////////////////////
	// �ܸ� ���
	BOOL bTrap   = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	double B = toM(m_pFooting->GetWidth());

	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+09$i$h%s$r���ݹݷ��� ����$n$h", GetNumberString(nIndexSub++,NUM_GANA));

	CString szTemp = (m_bQTriangle)? "�ﰢ�� ����":"��ٸ��� ����";
	CString szComp = (m_bQTriangle)? "<":">";
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$c��M$r$c=$r$e��Mr - ��Mo$m+04$c=$r$e%.3f - %.3f$m+07$c=$r$e$f3%.3f %s/m$n", m_dMrTotal, m_dMoTotal, m_dMrTotal-m_dMoTotal, pData->m_szTonfM);
	XLText->AddFormatText("$h$ce$r$c=$r$eB/2 - ��M/��V$m+04$c=$r$e%.3f/2 - %.3f/%.3f$m+07$c=$r$e$f3%.3f m$n", B, m_dMrTotal-m_dMoTotal, m_dVerTotal, m_dE);
	
	XLText->AddFormatText("$h$c��$r$c=$r$e3 x [B/2-|e|]$n");
	XLText->AddFormatText("$h$r$c=$r$e3 x [%.3f/2 - %.3f]$m+06$c=$r$e%.3f$me1$m+02m$c$r%s$r$eB = %.3f m$m+05$c��$r$e%s$n", B, fabs(m_dE), m_dLx, szComp, B, szTemp);
	
	if(m_bQTriangle)
	{
		XLText->AddFormatText("$h$p[FOOTING_QMAX2]$n$n");
		XLText->AddFormatText("$h$cqmax$r$c=$r$e(2 x %.3f) / (%.3f x %.3f)$m+11$c=$r$e%.3f %s$n", m_dVerTotal, UNIT_METER, m_dLx, m_dQmax, pData->m_szTonf_M2);
	}
	else
	{
		
		XLText->AddFormatText("$h$p[FOOTING_QMAX1]$n$n$n");
		XLText->AddFormatText("$h$iqmax$r$c=$r$e%.3f/%.3f x (1 + (6 x %.3f)/%.3f)$m+11$c=$r$e%.3f %s$n", m_dVerTotal, B, fabs(m_dE), B, m_dQmax, pData->m_szTonf_M2);
		XLText->AddFormatText("$h$iqmin$r$c=$r$e%.3f/%.3f x (1 - (6 x %.3f)/%.3f)$m+11$c=$r$e%.3f %s$n", m_dVerTotal, B, fabs(m_dE), B, m_dQmin, pData->m_szTonf_M2);
	}
	//////////////////////////////////////////////////////////////////////////
	long nRow = (m_bQTriangle)? 11:10;
	CDomyun *Dom = new CDomyun;
	DrawSupportForce(Dom, -1, TRUE);
	XLText->AddFormatText("$s+11$n$h$t");
	XLText->InsertDomyun(Dom, m_pStd->m_pView, nRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
	
	if(!bTrap)
	{
		long nTypeFoot = m_pFooting->GetTypeByInputValue();
		long nCntCheckFront = (m_bTwoUpperLoad && nTypeFoot!=0) ? (m_pBri->m_bCheckFrontFootTrap2 ? 3 : 2) : 1;
		long n=0; for(n=0; n<nCntCheckFront; n++)
		{
			WriteSectionDesignFront(XLText, n, nIndexSub++, bUse);
		}
	}

	WriteSectionDesignBack(XLText, nIndexSub, bUse);
}

void CLoadFooting::WriteSectionDesign_Pile(CXLFormatText *XLText, BOOL bUse, BOOL bFirst)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	if(m_bEarthQuake && m_pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	long nIndexSub = 1;
	WriteSectionDesign_PileText(XLText, nIndexSub, TRUE);

	long nCntCheckFront = (m_bTwoUpperLoad)? (m_pBri->m_bCheckFrontFootTrap2 ? 3 : 2) : 1;
	long n=0; for(n=0; n<nCntCheckFront; n++)
	{
		WriteSectionDesignFront_Pile(XLText, n, nIndexSub++, bUse);
	}

	WriteSectionDesign_PileText(XLText, nIndexSub, FALSE);
	WriteSectionDesignBack_Pile(XLText, bUse);
}

void CLoadFooting::WriteSectionDesign_PileText(CXLFormatText * XLText, long nIndexSub, HGBOOL bFront)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	//////////////////////////////////////////////////////////////////////////
	CString str = _T("");
	long nRow = bFront ? ((m_bQTriangle)? 11:10) : ((m_bQTriangle) ? 10 : 9);
	if (bFront)
	{
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h");
		XLText->AddFormatText("$h$i%s$r�ޱ��� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA));
	}

	CDomyun *DomFront = new CDomyun;
	long nTypePile = bFront ? 0 : 7; // ��ü
	DrawSupportForce(DomFront, nTypePile, TRUE);

	CDRect rt = DomFront->GetExtRect();
	if(nRow * (rt.Width()/rt.Height()) > 13) nRow = long(RoundDown(13*(rt.Height()/rt.Width()),0));

	XLText->AddFormatText("$s+10$h$r");
	XLText->InsertDomyun(DomFront, m_pStd->m_pView, nRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	XLText->AddFormatText("$u$u$u$u$u$u$u$u");
	GetPileForcArr(nTypePile, FALSE);
	
	CExPileApp *pPile = &m_pFooting->m_exFooting.m_Pile;
	pPile->SetPileArray();

	double dForce = 0;
	double wFooting = toM(m_pFooting->GetLength());
	long nEa = 0, i=0, nCntMin=0;
	long nCnt = m_dArrPileFoce.GetSize();
	nCntMin += nCnt;
	XLText->AddFormatText("$h$t$t$r$c*$r$e���� ������ : %s %s$n", pData->GetStrDoubleUnitChange(wFooting, UNIT_CGS_M, pData->m_szM), pData->m_szM);
	for(i=0; i<nCnt; i++)
	{
		nEa = (long)pPile->m_ArrayOrg[i][1];
		dForce = m_dArrPileFoce.GetAt(i);
		if (bLsd)
			XLText->AddFormatText("$h$t$t$r$cR%d$r$c=$r$eRu%d x %2d / %s$n", i+1, i+1, nEa, pData->GetStrDoubleUnitChange(wFooting, UNIT_CGS_M, pData->m_szM));
		else
			XLText->AddFormatText("$h$t$t$r$cR%d$r$c=$r$e%8.3f x %2d / %5.3f$m+06$c=$r$e%.3f %s$n", i+1, dForce, nEa, wFooting, dForce*nEa/wFooting, pData->m_szTonf_M);
	}

	nCnt = m_dAddPileFoce.GetSize();
	nCntMin += nCnt;
	for(i=0; i<nCnt; i++)
	{
		str = m_szAddPileNo.GetAt(i);
		dForce = m_dAddPileFoce.GetAt(i);
		CString strTemp = str.Mid(1);
		CString szAdd = m_szAddPileFoceString.GetAt(i);
		long nTemp = atoi(strTemp)-1;
		nEa = (long)pPile->m_ArrayOrg[nTemp][1];
		XLText->AddFormatText("$h$t$t$r$c%s'$r$c=$r$e%s x d��/dp$n$h", str, str);
		if (bLsd)
		{
			XLText->AddFormatText("$h$t$t$r$r$c=$r$e%s x %s$n", str, szAdd);
		}
		else
		{
			XLText->AddFormatText("$h$t$t$r$r$c=$r$e%s x %s$m+06$c=$r$e%.3f %s$n", str, szAdd, dForce*nEa/wFooting, pData->m_szTonf_M);
		}
	}

	if(nCnt != 0)
	{
		XLText->AddFormatText("$h$t$t$r$r$c:$r$ed��= ���ܰ���� ����ܸ鿡 ���� ��������$n");
		XLText->AddFormatText("$h$t$t$r$r$r$edp = ������ ������ ��������$n");
		if(pData->m_nTypeKciCode==0)		// KCI 2003
			XLText->AddFormatText("$h$t$t$r$r$r$e����ũ��Ʈ ����������ء�12.3.2,(3)$n");
		else		// KCI 2007
			XLText->AddFormatText("$h$t$t$r$r$r$e����ũ��Ʈ ����������ء�12.3.2,(4)$n");
		nCntMin += 3;
	}

	*XLText +=(nCntMin < 9)? 9-nCntMin:0;
}

void CLoadFooting::WriteSectionDesignFront_Pile(CXLFormatText * XLText, long n, long nIndexSub, BOOL bUse)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	double dDistTemp = (n==0)? m_dDistMf : (n == 1 ? m_dDistMf2 : m_dDistMf3);
	double Hf = toM(m_pFooting->GetHeight(FALSE));
	double wF = m_dDistMf;		// toM((bStt)? m_pFooting->m_dFWR:m_pFooting->m_dFWL);	// �ձ� ����
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dForce = 0;
	long nCnt = 0;
	long i = 0;
	CString str;

	if(bLsd)
	{
		Hf = frM(Hf);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$n$h");
	if(n == 0)
		str = _T("�ձ� (A-A)");
	else if(n == 1)
		str = _T("�ձ�2 (B-B)");
	else if(n == 2)
		str = _T("�ձ�3 (C-C)");
	XLText->AddFormatText("$s+03$h$i%s$r%s�� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA), str);

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		XLText->AddFormatText("$h$r$iVc$r$c=$r$e(%s-%s) x %s x %s x %0.2f$m+11$c=$r$e%s$me2$m+03%s$n", 
			pData->GetStrDouble(wF, UNIT_CGS_M), pData->GetStrDouble(wF-m_dDistSf, UNIT_CGS_M), pData->GetStrDouble(Hf, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), 
			m_dFactorDeadLoad, pData->GetStrDouble(m_ScF, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%s x %s x %s x %s/2 x %0.2f$m+11$c=$r$e$%s$me2$m+03%s/%s$n", 
		pData->GetStrDouble(dDistTemp, UNIT_CGS_M), pData->GetStrDouble(Hf, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3),
		pData->GetStrDouble(dDistTemp, UNIT_CGS_M), m_dFactorDeadLoad, pData->GetStrDouble((n==0)? m_McF:(n == 1 ? m_McF2 : m_McF3), UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

	XLText->AddFormatText("$s+03$h$c-$r$e���ҹݷ� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		XLText->AddFormatText("$h$r$iVq$r$c=$r$e");
		GetPileForcArr(3);
		nCnt = m_dArrPileFoce.GetSize();
		if(nCnt > 1)
		{
			for(i=0; i<nCnt; i++)
			{
				if(i%3 == 0 && i>2) XLText->AddFormatText("$n$h$r$r$r");
				dForce = m_dArrPileFoce.GetAt(i);
				if(i != 0) XLText->AddFormatText("$c+$r");
				XLText->AddFormatText("$e%.3f$me1$m+02", dForce);
			}
			XLText->AddFormatText("$c=$r$e%.3f %s$n", m_SqF, pData->m_szTonf_M);
		}
		else XLText->AddFormatText("$e%.3f %s$n", m_SqF, pData->m_szTonf_M);
	}

	XLText->AddFormatText("$h$r$iMq$r$c=$r$e");
	GetPileForcArr((n==0)? 1 : (n == 1 ? 5 : 6));
	nCnt = m_dArrPileFoce.GetSize();
	for(i=0; i<nCnt; i++)
	{
		if(i%3 == 0 && i>2) XLText->AddFormatText("$n$h$r$r$r");
		dForce = m_dArrPileFoce.GetAt(i);
		double dj = m_dDistMf - m_dArrDistPile.GetAt(i);
		if(n==1)
			dj = m_dDistMf2 - m_dArrDistPile.GetAt(i);
		else if(n==2)
			dj = m_dDistMf3 - m_dArrDistPile.GetAt(i);
		if(i != 0) XLText->AddFormatText("$c+$r");
		XLText->AddFormatText("$e%.3f x %s$me3$m+04", dForce, pData->GetStrDouble(dj, UNIT_CGS_M));
	}
	XLText->AddFormatText("$c=$r$e%s %s/%s$n", pData->GetStrDouble((n==0)? m_MqF:(n == 1 ? m_MqF2 : m_MqF3), UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

	if (bLsd == FALSE)
	{
		XLText->AddFormatText("$s+05$h$c-$r$���� �ܸ�� ����$n");
		if(bUse == FALSE)
		{
			XLText->AddFormatText("$h$r$iVu$r$c=$r$eVq - Vc$m+03$c=$r$e%.3f - %.3f$m+05$c=$r$e%.3f %s$n", m_SqF, m_ScF, m_dVuFront, pData->m_szTonf_M);
		}
		XLText->AddFormatText("$h$r$iMu$r$c=$r$eMq - Mc$m+03$c=$r$e%.3f - %.3f$m+05$c=$r$e%.3f %s/m$n", (n==0)? m_MqF:(n == 1 ? m_MqF2 : m_MqF3), (n==0)? m_McF:(n == 1 ? m_McF2 : m_McF3), (n==0)? m_dMuFront:(n == 1 ? m_dMuFront2 : m_dMuFront3), pData->m_szTonfM);
	}
}

void CLoadFooting::WriteSectionDesignBack_Pile(CXLFormatText *XLText, BOOL bUse)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	BOOL bIsOutWall = (m_pBri->IsOutsideWall());
	BOOL bOutWall = (bIsOutWall && m_nJijum == 0 || m_nJijum == m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	double B = toM(m_pFooting->GetWidth());
	double H = toM(m_pFooting->GetHeight());
	double wB = toM((bStt)? m_pFooting->m_dFWL:m_pFooting->m_dFWR);	// �ޱ� ����
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwBackFill = pData->GetValueUnitType(m_pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	if(bLsd)
	{
		B = frM(B);
		H = frM(H);
		wB = frM(wB);
	}

	// (37131) ���ʰ��信���� ���߶���� ����Ȱ���� ���� ������.
	// double dLiveLoadAlways = pData->GetValueUnitType((m_pBri->m_bUnderGroundRahmen)? m_pBri->m_dUnderGroundDesignLiveAlways:m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double dLiveLoadAlways = pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double dForce = 0;
	long i = 0;
	long nCnt = 0;

	CExPileApp *pPile = &m_pFooting->m_exFooting.m_Pile;
	pPile->SetPileArray();
	
	//
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		if(wB-(B-m_dDistSb) > 0) 
		{
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e(%s-%s) x %s x %s x %0.2f$m+11$c=$r$e%s$me2$m+03%s$n", 
				pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(wB-(B-m_dDistSb), UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), 
				m_dFactorDeadLoad, pData->GetStrDouble(m_ScB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
		else 
		{
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e%s x %s x %s x %0.2f$m+11$c=$r$e%s$me2$m+03%s$n", 
				pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), 
				m_dFactorDeadLoad, pData->GetStrDouble(m_ScB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
	}
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%s x %s x %s x %s/2 x %0.2f$m+11$c=$r$e%s$me2$m+03%s/%s$n", 
		pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), 
		pData->GetStrDouble(wB, UNIT_CGS_M), m_dFactorDeadLoad, pData->GetStrDouble(m_McB, UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

	if(bOutWall)
	{
		XLText->AddFormatText("$s+03$h$c-$r$e��ä�� ��翡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			XLText->AddFormatText("$h$r$iVs$r$c=$r$e[%s x %s x %s] x %0.2f = %s %s$n", 
				pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(m_dHightBackFill, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3), 
				m_dFactorSoilHor, pData->GetStrDouble(m_SsB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
		XLText->AddFormatText("$h$r$iMs$r$c=$r$e[%s x %s x %s x %s/2]x%0.2f$n", 
			pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(m_dHightBackFill, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3), 
			pData->GetStrDouble(wB, UNIT_CGS_M), m_dFactorSoilHor);
		XLText->AddFormatText("$h$m+02$c=$r$e %s %s/%s$n", pData->GetStrDouble(m_MsB, UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

		XLText->AddFormatText("$s+03$h$c-$r$e���忡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			XLText->AddFormatText("$h$r$iVp$r$c=$r$e[%s x %s / %s] x %0.2f = %s %s$n", 
				pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(m_dFactorPave==0? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave), UNIT_CGS_TONF), 
				pData->GetStrDouble(B-m_dDistMb, UNIT_CGS_M), m_dFactorPave, pData->GetStrDouble(m_SpB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
		XLText->AddFormatText("$h$r$iMp$r$c=$r$e[%s x %s / %s x %s/2]x%0.2f$n", 
			pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(m_dFactorPave==0? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave), UNIT_CGS_TONF), 
			pData->GetStrDouble(B-m_dDistMb, UNIT_CGS_M), pData->GetStrDouble(wB, UNIT_CGS_M), m_dFactorPave);
		XLText->AddFormatText("$h$m+02$c=$r$e %s %s/%s$n", pData->GetStrDouble(m_MpB, UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

		if(!m_bEarthQuake)
		{
			CString strLoad = bLsd ? _T("��������") : _T("��� Ȱ����");
			XLText->AddFormatText("$s+03$h$c-$r$e%s�� ���� �ܸ��$n", strLoad);
			if(bUse == FALSE)
			{
				XLText->AddFormatText("$h$r$iVl$r$c=$r$e%s x %s x %0.2f = %s %s$n", 
					pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), m_dFactorLiveLoad, 
					pData->GetStrDouble(m_SlB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
			}
			XLText->AddFormatText("$h$r$iMl$r$c=$r$e%s x %s��/2 x %0.2f = %s %s/%s$n", 
				pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetStrDouble(wB, UNIT_CGS_M), m_dFactorLiveLoad, 
				pData->GetStrDouble(m_MlB, UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	XLText->AddFormatText("$s+03$h$c-$r$e���ҹݷ� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		XLText->AddFormatText("$h$r$iVq$r$c=$r$e");
		GetPileForcArr(4);
		nCnt = m_dArrDistPile.GetSize();
		if(nCnt > 1)
		{
			for(i=0; i<nCnt; i++)
			{
				if(i%3 == 0 && i>2) XLText->AddFormatText("$n$h$r$r$r");
				dForce = m_dArrPileFoce.GetAt(i);
				if(i != 0) XLText->AddFormatText("$c+$r");
				XLText->AddFormatText("$e%.3f$me1$m+02", dForce);
			}
			XLText->AddFormatText("$c=$r$e%.3f %s$n", m_SqB, pData->m_szTonf_M);
		}
		else XLText->AddFormatText("$e%.3f %s$n", m_SqB, pData->m_szTonfM);
	}

	XLText->AddFormatText("$h$r$iMq$r$c=$r$e");
	GetPileForcArr(2);
	nCnt = m_dArrDistPile.GetSize();
	for(i=0; i<nCnt; i++)
	{
		if(i%3 == 0 && i>2) XLText->AddFormatText("$n$h$r$r$r");
		dForce = m_dArrPileFoce.GetAt(i);
		double dj = m_dArrDistPile.GetAt(i) - m_dDistMb;
		if(i != 0) XLText->AddFormatText("$c+$r");
		XLText->AddFormatText("$e%.3f x %s$me3$m+04", dForce, pData->GetStrDouble(dj, UNIT_CGS_M));
	}
	XLText->AddFormatText("$c=$r$e%s %s/%s$n", pData->GetStrDouble(m_MqB, UNIT_CGS_TONFM), pData->m_szTonfM, pData->m_szM);

	if (bLsd == FALSE)
	{
		XLText->AddFormatText("$s+05$h$c-$r$���� �ܸ�� ����$n");
		if(bOutWall)
		{
			if(bUse == FALSE)
			{
				XLText->AddFormatText("$h$r$iVu$r$c=$r$eVc + Vs + Vp + Vl - Vq$n");
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f + %.3f + %.3f + %.3f - %.3f$m+12$c=$r$e%.3f %s$n", m_ScB, m_SsB, m_SpB, m_SlB, m_SqB, m_dVuBack, pData->m_szTonf_M);
			}
			XLText->AddFormatText("$h$r$iMu$r$c=$r$eMc + Ms + Mp + Ml - Mq$n");
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f + %.3f + %.3f + %.3f - %.3f$m+12$c=$r$e%.3f %s/m$n", m_McB, m_MsB, m_MpB, m_MlB, m_MqB, m_dMuBack, pData->m_szTonfM);
		}
		else
		{
			if(bUse == FALSE)
			{
				XLText->AddFormatText("$h$r$iVu$r$c=$r$eVc - Vq$n");
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f - %.3f$m+12$c=$r$e%.3f %s$n", m_ScB, m_SqB, m_dVuBack, pData->m_szTonf_M);
			}
			XLText->AddFormatText("$h$r$iMu$r$c=$r$eMc - Mq$n");
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f - %.3f$m+12$c=$r$e%.3f %s/m$n", m_McB, m_MqB, m_dMuBack, pData->m_szTonfM);

		}
	}
	
	XLText->AddFormatText("$n$h");
}

void CLoadFooting::WriteForceCenter_Pile(CXLFormatText *XLText)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	long nIndexSub = 1;
	CDPoint vAng = m_pBri->GetAngleJijum(m_nJijum);
	double dAng = vAng.GetAngleDegree();
	double wSlab = m_pStd->GetSlabWidth(m_nJijum);
	double wSlabByAng = wSlab/vAng.y;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$i$h%s$r��� ����$n$h", GetNumberString(nIndexSub++,NUM_GANA));
	XLText->AddFormatText("$h$r$ee$r$c=$r$eB/2 - (��M / ��V)$m+05$c=$r$e%.3f / 2 - ( %.3f / %.3f )$n", toM(m_pFooting->GetWidth()), m_dMrTotal-m_dMoTotal, m_dVerTotal);
	XLText->AddFormatText("$h$r$r$c=$r$e$f3%.3f$me1$m+02m$n", m_dE);	

	XLText->AddFormatText("$i$h%s$r�߾� �ۿ�����$n$h", GetNumberString(nIndexSub++,NUM_GANA));
	XLText->AddFormatText("$h$r$e���� ������$m+03$c:$r$e%.3f / (sin %.3f)  = %.3f %s$n", wSlab, dAng, wSlabByAng, "m");
	XLText->AddFormatText("$h$r������$m+02$c=$r$e%.3f$me2$m+03$cx$r$e$f3%.3f$me1$m+02$c=$r$e$f3%.3f$me2$m+03%s$n", m_dVerTotal, wSlabByAng, m_dPileLoadV, pData->m_szTonf);
	XLText->AddFormatText("$h$r�����$m+02$c=$r$e%.3f$me2$m+03$cx$r$e$f3%.3f$me1$m+02$c=$r$e$f3%.3f$me2$m+03%s$n", m_dHorTotal, wSlabByAng,m_dPileLoadH, pData->m_szTonf);
	XLText->AddFormatText("$h$r���Ʈ$m+02$c=$r$e%.3f$me2$m+03$cx$r$e$f3%.3f$me1$m+02$c=$r$e$f3%.3f$me2$m+03%s$n", m_dPileLoadV, m_dE, m_dPileLoadMCen, pData->m_szTonfM);
	XLText->AddText("$n$h");
}

void CLoadFooting::DrawSupportForce(CDomyun *pDomP, long nTypePile , BOOL bSection)
{
	CDomyun Dom(pDomP);
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bTrap = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (m_nJijum == 0 || m_nJijum == m_pBri->m_nQtyJigan));
	BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	BOOL bSlopeR = (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE && bTrap);
	BOOL bSlopeL = (m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE && bTrap);
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	BOOL bInWall = (m_nJijum==0 || m_nJijum==m_pBri->GetCountJijum()-1)? FALSE:TRUE;
	long iDir = (bStt)? -1:1;

	Dom.SetScaleDim(80);
	Dom.SetTextHeight(5);

	CDPoint xyStt(0,0);
	CDPoint xyFront;
	CDPoint xyBack;
	CDPoint xyTemp;

	double B = m_pFooting->GetWidth();
	double H = m_pFooting->GetHeight();
	double wF = (bStt)? m_pFooting->m_dFWR:m_pFooting->m_dFWL;	// �ձ� ����
	double wB = (bStt)? m_pFooting->m_dFWL:m_pFooting->m_dFWR;	// �ޱ� ����
	double BW = B-wF-wB;
	double dFH = m_pFooting->m_dFH;
	double dFSH = m_pFooting->GetHeight(FALSE);
	double dFEW = 0;
	double dFSW = 0;
	

	if(bTrap && (bSlopeR || bSlopeL))
	{
		CWallApp *pWall = m_pBri->GetInWall(m_nJijum-1);
		iDir = (bSlopeR)? -1 : 1;
		B = m_pFooting->GetWidth();
		H = m_pFooting->GetHeight();
		wF = m_pFooting->m_dFW;
		wB = (bSlopeL)? m_pFooting->m_dFSRW : m_pFooting->m_dFSLW;
		BW = 0;
		double dTWall = m_pBri->GetThickWall(m_nJijum);
		double dAng = pWall->GetAngleWallByHor();

		CTwinVector  tvSlop, tvTemp, tvCut;

		xyTemp = xyStt;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x +=  B*iDir;
		Dom.LineTo(xyTemp);
		xyTemp.y += H;
		Dom.LineTo(xyTemp);
		xyTemp.x -= wB*iDir;
		Dom.LineTo(xyTemp);
		tvSlop.m_v1 = xyTemp;
		xyTemp.x -= wF*iDir;
		xyTemp.y -= dFH;
		Dom.LineTo(xyTemp);
		tvSlop.m_v2 = xyTemp;
		xyTemp.y -= dFSH;
		Dom.LineTo(xyTemp);

		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
		CDPoint vSlope = tvSlop.GetXyDir();
		tvTemp.m_v1 = tvSlop.GetXyMid();
		tvTemp.m_v2 = tvTemp.m_v1 + 2000 * ToDPointFrDegree(dAng);
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
		
		tvCut.m_v1 = tvTemp.m_v2 + (dTWall + 200) * ToDPointFrDegree(dAng+90);
		tvCut.m_v2 = tvTemp.m_v2 - (dTWall + 200) * ToDPointFrDegree(dAng+90);
		Dom.LineTo(tvCut.m_v1, tvCut.m_v2);

		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		Offset(tvTemp.m_v1, tvTemp.m_v2, -dTWall*iDir/2);
		GetXyMatchSegAndLine(tvSlop.m_v1, tvSlop.m_v2, tvTemp.m_v2, tvTemp.GetXyDir(), tvTemp.m_v1);
		xyFront = tvTemp.m_v1;
		
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
		Offset(tvTemp.m_v1, tvTemp.m_v2, dTWall*iDir);
		GetXyMatchSegAndLine(tvSlop.m_v1, tvSlop.m_v2, tvTemp.m_v2, tvTemp.GetXyDir(), tvTemp.m_v1);
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
	}
	else if(bTrap2 || bArch)
	{
		CWallApp *pWall = m_pBri->GetInWall((m_nJijum==0)? 0 : m_pBri->GetCountInWall()-1);
		double dAng = pWall->GetAngleWallByHor();
		double dTWall = m_pBri->GetThickWall((m_nJijum==0)? 0 : m_pBri->m_nQtyJigan);
		double dTWall_Add = 0;
		if(bTrap2) dTWall_Add = m_pBri->GetThickWall((m_nJijum==0)? 1 : m_pBri->m_nQtyJigan-1);
		else dTWall_Add = m_pStd->GetThickArchRib(m_pBri->GetStationRibBySpan((m_nJijum==0)? 0:m_pBri->m_nQtyJigan-1, (m_nJijum==0)? 0:2));

		dTWall += ((m_nJijum==0) ? m_pBri->m_dWSHW : m_pBri->m_dWEHW);

		BW = dTWall;
		double dW = m_pFooting->GetWidth();
		double dFH = m_pFooting->GetHeight(FALSE);
		dFSH = (m_nJijum==0)? m_pFooting->m_dFSRH : m_pFooting->m_dFSLH;
		dFEW = (m_nJijum==0)? m_pFooting->m_dFERW : m_pFooting->m_dFELW;
		dFSW = (m_nJijum==0)? m_pFooting->m_dFSRW : m_pFooting->m_dFSLW;
		double dFWF = (m_nJijum==0)? m_pFooting->m_dFWR : m_pFooting->m_dFWL;
		double dFWB = (m_nJijum==0)? m_pFooting->m_dFWL :m_pFooting->m_dFWR;

		CTwinVector  tvSlop, tvTemp, tvCut;
		
		xyTemp = xyStt;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x +=  dW*iDir;
		Dom.LineTo(xyTemp);
		xyTemp.y += (dFH + dFSH);
		Dom.LineTo(xyTemp);
		xyTemp.x -= dFWB*iDir;
		xyBack = xyTemp;
		Dom.LineTo(xyTemp);
		xyTemp.y += 2000;
		Dom.LineTo(xyTemp);
		xyTemp.x -= dTWall*iDir;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y -= 2000;
		Dom.LineTo(xyTemp);
		xyTemp.x -= dFWF*iDir;
		Dom.LineTo(xyTemp);
		tvSlop.m_v1 = xyTemp;
		xyTemp.x -= dFSW*iDir;
		xyTemp.y -= dFSH;
		Dom.LineTo(xyTemp);
		tvSlop.m_v2 = xyTemp;
		xyTemp.x -= dFEW*iDir;
		Dom.LineTo(xyTemp);
		xyTemp.y -= dFH;
		Dom.LineTo(xyTemp);

		CDPoint xyBase(0, 0);
		xyBase.x = m_pFooting->m_xyBottomCenter.x - m_pFooting->GetWidth()/2 * iDir;
		xyBase.y = m_pFooting->m_xyBottomCenter.y;
		

		CTwinVector tvBW;
		if(bTrap2)
		{
			tvBW = pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
		}
		else
		{
			long nSizetv = m_pBri->m_tvArrVertSectionRibCen.GetSize();
			tvBW = m_pBri->m_tvArrVertSectionRibCen.GetAt((bStt)? 0 : nSizetv-1);
			CDPoint vAng = bStt? tvBW.GetXyDir() : tvBW.GetXyDir()*-1;
			dAng = vAng.GetAngleDegree();
		}
		
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
		CDPoint vSlope = tvSlop.GetXyDir();
		tvTemp.m_v1 = CDPoint((tvBW.m_v1.x - xyBase.x), (tvBW.m_v1.y - xyBase.y));
		tvTemp.m_v2 = tvTemp.m_v1 + 2000 * ToDPointFrDegree(dAng);
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
		
		tvCut.m_v1 = tvTemp.m_v2 + (dTWall_Add + 200) * ToDPointFrDegree(dAng+90);
		tvCut.m_v2 = tvTemp.m_v2 - (dTWall_Add + 200) * ToDPointFrDegree(dAng+90);
		Dom.LineTo(tvCut.m_v1, tvCut.m_v2);
		
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		Offset(tvTemp.m_v1, tvTemp.m_v2, -dTWall_Add*iDir/2);
		GetXyMatchSegAndLine(tvSlop.m_v1, tvSlop.m_v2, tvTemp.m_v2, tvTemp.GetXyDir(), tvTemp.m_v1);
		xyFront = tvTemp.m_v1;
		
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
		Offset(tvTemp.m_v1, tvTemp.m_v2, dTWall_Add*iDir);
		GetXyMatchSegAndLine(tvSlop.m_v1, tvSlop.m_v2, tvTemp.m_v2, tvTemp.GetXyDir(), tvTemp.m_v1);
		Dom.LineTo(tvTemp.m_v1, tvTemp.m_v2);
	}
	else
	{
		xyTemp = xyStt;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x +=  B*iDir;
		Dom.LineTo(xyTemp);
		xyTemp.y += H;
		Dom.LineTo(xyTemp);
		xyTemp.x -= wB*iDir;
		Dom.LineTo(xyTemp);
		xyBack = xyTemp;
		xyTemp.y += 2000;
		Dom.LineTo(xyTemp);
		xyTemp.x -= BW*iDir;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y -= 2000;
		Dom.LineTo(xyTemp);
		xyFront = xyTemp;
		xyTemp.x -= wF*iDir;
		Dom.LineTo(xyTemp);
		xyTemp.y -= H;
		Dom.LineTo(xyTemp);
	}
	
	if(m_nJijum ==0 || m_nJijum == m_pBri->GetCountJijum()-1)
	{
		xyTemp = xyStt;
		xyTemp.x += B*iDir;
		xyTemp.y += H;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y += 2000;
		Dom.LineTo(xyTemp);
	}
	else if(!bTrap)
	{
		xyTemp = xyStt;
		xyTemp.x += (B - wB);
		xyTemp.y += (H+2000);
	}
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER_LINE);
	if(bInWall && !bTrap)
	{
		xyTemp.x = xyBack.x - 200;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x = xyFront.x + 200;
		Dom.LineTo(xyTemp);
	}
	else if(!bTrap)
	{
		xyTemp.x += 200 * iDir;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x = xyBack.x + -(BW+200) * iDir;
		Dom.LineTo(xyTemp);
	}
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);

	xyTemp = xyStt;
	xyTemp.y += H;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	if(bTrap2 || bArch)
	{
		Dom.DimLineTo(dFEW*iDir, 0, COMMA(dFEW));
		Dom.DimLineTo(dFSW*iDir, 0, COMMA(dFSW));
	}
	Dom.DimLineTo(wF*iDir, 0, COMMA(wF));
	Dom.DimLineTo(BW*iDir, 0, COMMA(BW));
	Dom.DimLineTo(wB*iDir, 0, COMMA(wB));

	if(bStt && !(bTrap && bSlopeL)) Dom.SetDirection("RIGHT");
	else Dom.SetDirection("LEFT");
	xyTemp = xyStt;
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	if((bTrap && (bSlopeL || bSlopeR)) || bTrap2 || bArch)
	{
		Dom.DimLineTo(dFH, 0, COMMA(dFH));
		Dom.DimLineTo(dFSH, 0, COMMA(dFSH));
	}
	else Dom.DimLineTo(H, 0, COMMA(H));
	Dom.SetTextAlignHorz(1);

	BOOL bPile = (m_pBri->IsPileBase(m_nJijum))? TRUE:FALSE; 
	if(bPile) 
	{
		GetPileForcArr(nTypePile);

		double dHMax = m_dArrPileFoce.Max(FALSE);
		double dHMin = m_dArrPileFoce.Min(TRUE);
		double nLenFactor = 0;
		if(dHMax <= 0 && dHMin <= 0) nLenFactor = 0;
		else if(fabs(dHMax) > fabs(dHMin)) nLenFactor = 2000/dHMax;
		else nLenFactor = 2000/fabs(m_dArrPileFoce.Min(TRUE));
		
		CString str = _T("");
		long nCnt = m_dArrPileFoce.GetSize();
		long i=0; for(i=0; i<nCnt; i++)
		{
			xyTemp = xyStt;
			xyTemp.x += (bLsd? m_dArrDistPile.GetAt(i) : frM(m_dArrDistPile.GetAt(i)))*iDir;
			xyTemp.y -= m_dArrPileFoce.GetAt(i) * nLenFactor;
			str.Format("R%d", i+1);
			Dom.TextOut(xyTemp.x, xyTemp.y, str);
			Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dArrPileFoce.GetAt(i) * nLenFactor, 2);
		}
	}
	else
	{
		double nLenFactor = (m_dQmax > 0)? 2000/m_dQmax:0;
		if(m_bQmaxFront)
		{
			xyTemp = xyStt;
			xyTemp.y -= m_dQmax*nLenFactor;
			Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmax*nLenFactor, 2);
			Dom.TextOut(xyTemp.x, xyTemp.y-400, bLsd ? "Q1" : "Qmax");
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			if(!m_bQTriangle)
			{
				xyTemp = xyStt;
				xyTemp.x += B*iDir;
				xyTemp.y -= m_dQmin*nLenFactor;
				Dom.LineTo(xyTemp);
				if(m_dQmin > 0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmin*nLenFactor, 2);
				else Dom.DimArrowTo(xyTemp.x, xyTemp.y, -m_dQmin*nLenFactor, 6);
				Dom.TextOut(xyTemp.x, xyTemp.y-400, bLsd ? "Q2" : "Qmin");
			}
			else
			{
				xyTemp = xyStt;
				xyTemp.x += (bLsd? m_dLx : frM(m_dLx))*iDir;
				Dom.LineTo(xyTemp);
			}
		}
		else
		{
			xyTemp = xyStt;
			xyTemp.x += B*iDir;
			xyTemp.y -= m_dQmax*nLenFactor;
			Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmax*nLenFactor, 2);
			Dom.TextOut(xyTemp.x, xyTemp.y-400, bLsd ? "Q2" : "Qmax");
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			if(!m_bQTriangle)
			{
				xyTemp = xyStt;
				xyTemp.y -= m_dQmin*nLenFactor;
				Dom.LineTo(xyTemp);
				if(m_dQmin > 0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmin*nLenFactor, 2);
				else Dom.DimArrowTo(xyTemp.x, xyTemp.y, -m_dQmin*nLenFactor, 6);
				Dom.TextOut(xyTemp.x, xyTemp.y-400, bLsd ? "Q1" : "Qmin");
			}
			else
			{
				xyTemp = xyStt;
				xyTemp.x += (B-(bLsd? m_dLx : frM(m_dLx)))*iDir;
				Dom.LineTo(xyTemp);
			}
		}
	}
	
	if(bSection)
	{
		double nLenFactor = (m_dQmax > 0)? 2000/m_dQmax:0;
		long nPosTypeFront = m_pBri->m_nApplyCalcShearDistFront;
		long nPosTypeBack = m_pBri->m_nApplyCalcShearDistBack;
		
		double dF = (bTrap2 || bArch)? (bLsd? m_dHgtSf : frM(m_dHgtSf))-m_pFooting->m_dCoverLower[0] : H-m_pFooting->m_dCoverLower[0];
		double dB = H-m_pFooting->m_dCoverUpper[0];
		double dDistFront = 0;
		double dDistBack = 0;
		
		if(nPosTypeFront == 0) dDistFront = 0;
		else if(nPosTypeFront == 1) dDistFront = dF;
		else if(nPosTypeFront == 2) dDistFront = dF/2;
		
		if(nPosTypeBack == 0) dDistBack = 0;
		else if(nPosTypeBack == 1) dDistBack = dB;
		else if(nPosTypeBack == 2) dDistBack = dB/2;

		if(!bTrap)
		{
			xyTemp = xyFront;
			xyTemp.y = -m_dQmF*nLenFactor;
			if(!bPile && m_dQmF>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmF*nLenFactor, 2);
			if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qm");
			if(nPosTypeFront == 0 && !bPile) Dom.TextOut(xyTemp.x, xyTemp.y-700, "Qs");
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
			xyTemp.y = 0;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y = (m_bTwoUpperLoad)? xyFront.y : dFH;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			if(m_bTwoUpperLoad && m_dDistMf2 > 0)
			{
				xyTemp.x = (bLsd? m_dDistMf2 : frM(m_dDistMf2))*iDir;
				xyTemp.y = -m_dQmF2*nLenFactor;
				if(!bPile && m_dQmF2>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmF2*nLenFactor, 2);
				if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qm");
				xyTemp.y = 0;
				Dom.MoveTo(xyTemp.x, xyTemp.y);
				xyTemp.y = dFH;
				Dom.LineTo(xyTemp.x, xyTemp.y);
			}

			if(m_bTwoUpperLoad && m_dDistMf3 > 0)
			{
				xyTemp.x = (bLsd? m_dDistMf3 : frM(m_dDistMf3))*iDir;
				xyTemp.y = -m_dQmF3*nLenFactor;
				if(!bPile && m_dQmF3>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmF3*nLenFactor, 2);
				if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qm");
				xyTemp.y = 0;
				Dom.MoveTo(xyTemp.x, xyTemp.y);
				xyTemp.y = dFH;
				Dom.LineTo(xyTemp.x, xyTemp.y);
			}
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		}
		
		xyTemp.x = xyBack.x;
		xyTemp.y = -m_dQmB*nLenFactor;
		if(!bPile && m_dQmB>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQmB*nLenFactor, 2);
		if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qm");
		if(nPosTypeBack == 0 && !bPile) Dom.TextOut(xyTemp.x, xyTemp.y-700, "Qs");
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
		xyTemp.y = 0;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y = xyBack.y;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		
		if(nPosTypeFront != 0 && !bTrap && !m_bOutBoundFootFront)
		{
			double dHmin = m_pFooting->GetHeight(FALSE);
			xyTemp.x = xyFront.x - (dDistFront * iDir);
			xyTemp.y = -m_dQvF*nLenFactor;
			if(!bPile && m_dQvF>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQvF*nLenFactor, 2);
			if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qs");
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
			xyTemp.y = 0;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y = dHmin;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		}
		if(nPosTypeBack != 0 && !m_bOutBoundFootBack)
		{
			xyTemp.x = xyBack.x + (dDistBack * iDir);
			xyTemp.y = -m_dQvB*nLenFactor;
			if(!bPile && m_dQvF>0) Dom.DimArrowTo(xyTemp.x, xyTemp.y, m_dQvB*nLenFactor, 2);
			if(!bPile) Dom.TextOut(xyTemp.x, xyTemp.y-400, "Qs");
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
			xyTemp.y = 0;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y = H;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		}
		
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyStt.x, xyStt.y-(m_dQmax*nLenFactor+400), 0, "");
		if(bPile)
		{
			double dDistPrev = 0; 
			double dDistCurr = 0; 
			long nSizePile = m_dArrDistPile.GetSize();
			long i=0; for(i=0; i<nSizePile; i++)
			{
				dDistCurr = bLsd? m_dArrDistPile.GetAt(i) : frM(m_dArrDistPile.GetAt(i));
				Dom.DimLineToExtend((dDistCurr-dDistPrev)*iDir, 0, COMMA(dDistCurr-dDistPrev), "");
				dDistPrev = dDistCurr;
			}
			Dom.DimLineToExtend((B-dDistCurr)*iDir, 0, COMMA(B-dDistCurr), "");
		}
		else
		{
			double dTemp = 0;

			if(bTrap)
			{
				double dDistTemp = fabs(xyStt.x - xyBack.x);
				Dom.DimLineToExtend((dDistTemp)*iDir, 0, COMMA(dDistTemp), "");
				dTemp += dDistTemp;
				Dom.DimLineToExtend((dDistBack)*iDir, 0, COMMA(dDistBack), "");
				dTemp += dDistBack;
				Dom.DimLineToExtend((B-dTemp)*iDir, 0, COMMA(B-dTemp), "");
			}
			else if(bTrap2 || bArch)
			{
				double dDistTemp = bLsd? m_dDistMf : frM(m_dDistMf);
				Dom.DimLineToExtend((dDistTemp)*iDir, 0, COMMA(dDistTemp), "");
				dTemp += dDistTemp;
				dDistTemp = bLsd? fabs(m_dDistMb - m_dDistMf) : fabs(frM(m_dDistMb - m_dDistMf));
				Dom.DimLineToExtend((dDistTemp)*iDir, 0, COMMA(dDistTemp), "");
				dTemp += dDistTemp;
				Dom.DimLineToExtend((B-dTemp)*iDir, 0, COMMA(B-dTemp), "");
			}
			else
			{
				Dom.DimLineToExtend((wF-dDistFront)*iDir, 0, COMMA(wF-dDistFront), "");
				dTemp += wF-dDistFront;
				Dom.DimLineToExtend((dDistFront)*iDir, 0, "",COMMA(dDistFront));
				dTemp = wF;
				Dom.DimLineToExtend(((B-wB)-dTemp)*iDir, 0, COMMA((B-wB)-dTemp),"");
				dTemp = B-wB;
				Dom.DimLineToExtend((dDistBack)*iDir, 0, "",COMMA(dDistBack));
				dTemp += dDistBack;
				Dom.DimLineToExtend((B-dTemp)*iDir, 0, COMMA(B-dTemp), "");
			}
		}

		CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
		xyTemp.x = (bLsd? m_dDistMf : frM(m_dDistMf))*iDir;
		xyTemp.y = dFH + 3000;
		Sym.ArrowSection(&Dom, "A", xyTemp, 0,CDPoint(-1,0), 2);

		xyTemp.x = (bLsd? m_dDistMf : frM(m_dDistMf))*iDir;
		xyTemp.y = -4000;
		Sym.ArrowSection(&Dom, "A", xyTemp, 0,CDPoint(-1,0), 1);

		if(!bInWall && bTrap2)
		{
			if(m_bTwoUpperLoad)
			{
				xyTemp.x = (bLsd? m_dDistMf2 : frM(m_dDistMf2))*iDir;
				xyTemp.y = dFH + 3000;
				Sym.ArrowSection(&Dom, "B", xyTemp, 0,CDPoint(-1,0), 2);

				xyTemp.x = (bLsd? m_dDistMf2 : frM(m_dDistMf2))*iDir;
				xyTemp.y = -4000;
				Sym.ArrowSection(&Dom, "B", xyTemp, 0,CDPoint(-1,0), 1);
			}

			if(m_bTwoUpperLoad)
			{
				xyTemp.x = (bLsd? m_dDistMf3 : frM(m_dDistMf3))*iDir;
				xyTemp.y = dFH + 3000;
				Sym.ArrowSection(&Dom, "C", xyTemp, 0,CDPoint(-1,0), 2);

				xyTemp.x = (bLsd? m_dDistMf3 : frM(m_dDistMf3))*iDir;
				xyTemp.y = -4000;
				Sym.ArrowSection(&Dom, "C", xyTemp, 0,CDPoint(-1,0), 1);
			}
		}
	}
	*pDomP << Dom;
}

double CLoadFooting::GetPileFootingCentroid(long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting == NULL || pBri->IsPileBase(nJijum)==FALSE) return 0;
	
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	pPile->SetPileArray();
//	COrgInfo *pOrg;
	
	long nRowCnt = pPile->m_nCountOrg;
	double dDist	= 0;
	double dEa		= 0;
	double dCalc	= 0;
	double dDosim	= 0;
	double dEaTot	= 0;
	double dCalcTot	= 0;
	// Data
	long i=0; for(i=0; i<nRowCnt; i++)
	{
		if(i>=0 && i<nRowCnt)
		{
//			pOrg = pPile->m_pArrOrgInfo.GetAt(i);
			
			dDist += toM(pPile->m_ArrayOrg[i][0]);
			dEa	= pPile->m_ArrayOrg[i][1];
			dCalc = dDist * dEa;
			
			dEaTot	+= dEa;
			dCalcTot += dCalc;
		}
		
		if(i == nRowCnt-1) dDosim = dCalcTot / dEaTot;
	}
	
	return dDosim;
}

void CLoadFooting::MakeTableFootCenter(CXLFormatText *XLText)
{
	if((m_pFooting->m_nType < FOOTING_TYPE_LEFTSLOPE))
		return;

	CString str = _T("");
	int nCntRow = 5;
	int nCntCol = 6;
	XLText->AddFormatText("$h* �������� ���� �� ���� ����ٰ�$n$h");
	XLText->AddTable(nCntRow-1, 18-1, 1, 1);
	
	double dW, dFH, dFSH, dFEW, dFSW;

	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP)
	{
		// (ARCBRIDGE-1559) FH �� FSH�� �ٲ����... FSH�� �Ʒ��� ���̶��.. ������ �־ ������ ���ϰ�...
		// FSW�� �������� ���̳�... �Ѥ�
		dW = toM(m_pFooting->GetWidth());
//		dFH = toM(m_pFooting->m_dFH);
//		dFSH = toM(m_pFooting->GetHeight(FALSE));
		dFH = toM(m_pFooting->GetHeight(FALSE));
		dFSH = toM(m_pFooting->m_dFH);
		dFEW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFERW) : toM(m_pFooting->m_dFELW);
		dFSW = (m_pFooting->m_nType==FOOTING_TYPE_LEFTSLOPE)? toM(m_pFooting->m_dFSRW) : toM(m_pFooting->m_dFSLW);
		dFSW = dW - dFSW;
	}
	else
	{
		dW = toM(m_pFooting->GetWidth());
		dFH = toM(m_pFooting->GetHeight(FALSE));
		dFSH = toM((m_nJijum==0)? m_pFooting->m_dFSRH : m_pFooting->m_dFSLH);
		dFEW = toM((m_nJijum==0)? m_pFooting->m_dFERW : m_pFooting->m_dFELW);
		dFSW = toM((m_nJijum==0)? m_pFooting->m_dFSRW : m_pFooting->m_dFSLW);
	}

	double A[3];
	double Xi[3];
	double Yi[3];

	A[0] = (dW * dFH);
	A[1] = dFSW * dFSH /2;
	A[2] = (dW - dFSW - dFEW) * dFSH;

	Xi[0] = dW/2;
	Xi[1] = dFEW + dFSW*2/3;
	Xi[2] = (dW - dFEW - dFSW)/2 + dFEW + dFSW;

	Yi[0] = dFH/2;
	Yi[1] = dFH + dFSH/3;
	Yi[2] = dFH + dFSH/2;

	double dA = A[0] + A[1] + A[2];
	double dXi = (A[0]*Xi[0] + A[1]*Xi[1] + A[2]*Xi[2]) / dA;
	double dYi = (A[0]*Yi[0] + A[1]*Yi[1] + A[2]*Yi[2]) / dA;
	// Title
	int nWidthJoin1[] = { 3, 3, 3, 3, 3, 3 };
	char *Title1[] = { "�� ��", "B", "H", "A", "Xi", "Yi" };
	
	int i=0; for(i=0; i<6; i++)
	{		
		XLText->AddMergeFormatText(0,nWidthJoin1[i]-1,"$c%s", Title1[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin1[i]);
	}
	XLText->AddText("$n$h");
	
	// Data
	for(i=0; i<nCntRow-1; i++)
	{	
		int j=0; for(j=0; j<nCntCol; j++)
		{
			if(j == 0 && i < 3) str = GetNumberString(i+1, NUM_CIRCLE);
			else if(j == 0 && i == 3) str = _T("��");
			else if(j == 1 && i == 0) str.Format("$f3%.3f", dW);
			else if(j == 1 && i == 1) str.Format("$f3%.3f", dFSW);
			else if(j == 1 && i == 2) str.Format("$f3%.3f", dW-dFSW-dFEW);
			else if(j == 2 && i == 0) str.Format("$f3%.3f", dFH);
			else if(j == 2 && i == 1) str.Format("$f3%.3f/2", dFSH);
			else if(j == 2 && i == 2) str.Format("$f3%.3f", dFSH);
			else if(j == 3 && i < 3) str.Format("$f3%.3f", A[i]);
			else if(j == 3 && i == 3) str.Format("$f3%.3f", dA);
			else if(j == 4 && i < 3) str.Format("$f3%.3f", Xi[i]);
			else if(j == 4 && i == 3) str.Format("$f3%.3f", dXi);
			else if(j == 5 && i < 3) str.Format("$f3%.3f", Yi[i]);
			else if(j == 5 && i == 3) str.Format("$f3%.3f", dYi);
			else str = "";
			
			if(str == "$f30.000") str = "";
			
			XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s", str);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);	
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

double CLoadFooting::GetDistSlopeBack()
{
	double dDist = 0;
	CWallApp *pWall = m_pBri->GetInWall(m_nJijum-1);
	double dAng = pWall->GetAngleWallByHor();
	double dTWall = toM(m_pBri->GetThickWall(m_nJijum));
	double dFH = toM(m_pFooting->m_dFH);
	double dFW = toM(m_pFooting->m_dFW);
	
	if(dAng < 90) dAng = 180 - dAng;
	double dTemp = fabs(dTWall/2. /sin(ToRadian(dAng)));
	CDPoint xyP(0, 0);
	GetXyMatchSegAndLine(CDPoint(0, 0), CDPoint(dFW, dFH), CDPoint(dTemp, 0), ToDPointFrDegree(dAng), xyP);
	
	dDist = xyP.x + dFW/2;

	return dDist;
}

void CLoadFooting::GetDistFrontTrap2AndArch(double &dDist, double &dHeight)
{
	BOOL bTrap2 = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
	CWallApp *pWall = m_pBri->GetInWall((m_nJijum==0)? 0 : m_pBri->GetCountInWall()-1);
	
	m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	m_pBri->SyncBridge();

	CDPoint xyStt, xyFront;
	CString str = _T("");
	BOOL bStt = (m_nJijum==0)? TRUE : FALSE;

	CTwinVector tvBW, tvFoot;
	if(bTrap2)
	{
		str.Format("��ü%s��", bStt? "����" : "����");
		tvBW = pWall->m_tvArrFront.GetTvByInfo(str);
		xyFront = tvBW.m_v1;
	}
	else
	{
		long nSizetv = m_pBri->m_tvArrVertSectionRib[iLOWER].GetSize();
		tvBW = m_pBri->m_tvArrVertSectionRib[iLOWER].GetAt((bStt)? 0 : nSizetv-1);
		xyFront = bStt? tvBW.m_v1 : tvBW.m_v2;
	}

	str.Format("����%s��", bStt? "����" : "����");
	tvFoot = m_pFooting->m_tvArrFront.GetTvByInfo(str);
	xyStt = bStt? tvFoot.m_v2 : tvFoot.m_v1;

	m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);
	dDist = toM(fabs(xyStt.x - xyFront.x));
	dHeight = toM(fabs(xyFront.y - xyStt.y));
	m_pBri->SyncBridge();
}

CString CLoadFooting::GetStringVertLoad( HGINT32 iIndexLoad, HGBOOL bMark/*=TRUE*/ )
{
	CString szName = _T("");
	
	switch (iIndexLoad)
	{
	case VER_LOAD_LIVE:
		szName = bMark? _T("L") : _T("��� Ȱ����"); break;
	case VER_LOAD_FOOTW:
		szName = bMark? _T("SW") : _T("��������"); break;
	case VER_LOAD_PAVE:
		szName = bMark? _T("Pav") : _T("����"); break;
	case VER_LOAD_BACKFILL:
		szName = bMark? _T("Sh") : _T("������"); break;
	case VER_LOAD_DEAD:
		szName = bMark? _T("D") : _T("��������"); break;
	default:
		szName = _T(""); break;
	}

	return szName;
}

void CLoadFooting::WriteSectionDesignFront(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse)
{
	if(n == 1 && m_pFooting->m_dFELW==0.0 && m_pFooting->m_dFERW==0.0) return;

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double B = toM(m_pFooting->GetWidth());
	double B_ = (m_bQTriangle)? m_dLx:B;
	double dC = B-B_;
	double Hf = toM(m_pFooting->GetHeight(FALSE));
	double wF = toM((bStt)? (m_pFooting->m_dFWR + m_pFooting->m_dFSRW):(m_pFooting->m_dFWL + m_pFooting->m_dFSLW));	// �ձ� ����

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$n$h");
	CString str = _T("");
	if(n == 0)
		str = _T("�ձ� (A-A)");
	else if(n == 1)
		str = _T("�ձ�2 (B-B)");
	else if(n == 2)
		str = _T("�ձ�3 (C-C)");
	XLText->AddFormatText("$s+06$h$i%s$r%s�� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA), str);

	XLText->GoPosition(NO_MOVE,COL_POS_3);

	// ������ġ�� ������ ������ �ۿ� �ִٸ� �ܸ���� ���̳ʽ��� ���� �ʰ� 0�̵ȴ�.
	double dDistMftemp = (n==0)? m_dDistMf : (n == 1 ? m_dDistMf2 : m_dDistMf3); 
	double X2 = (m_bQmaxFront)? B_-dDistMftemp:dDistMftemp-dC;
	if(X2 > 0)
	{
		XLText->AddFormatText("$h$iQm$r$c=$r$eQmin + (Qmax-Qmin) x ����/��$m+09$c$n");
		XLText->AddFormatText("$h$c$r$c=$r$e%.3f + (%.3f - %.3f) x (%.3f/%.3f)$m+12$c=$r$e$f3%.3f$me2$m+03%s$n", m_dQmin, m_dQmax, m_dQmin, X2, B_, (n==0)? m_dQmF:(n == 1 ? m_dQmF2 : m_dQmF3), pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$iQm$r$c=$r0$r%s$m+03(������ġ�� ������ �������� �ۿ�����)$n", pData->m_szTonf_M2);
	}

	X2 = (m_bQmaxFront)? B_-m_dDistSf : m_dDistSf-dC;

	if(m_bOutBoundFootFront)
	{
		XLText->AddFormatText("$h$iQs$r$c=$r0$r%s$m+03(������ġ�� ���ʸ� ������Ƿ� �������� ����)$n", pData->m_szTonf_M2);
	}
	else if(X2 > 0)
	{
		XLText->AddFormatText("$h$iQs$r$c=$r$eQmin + (Qmax-Qmin) x ����/��$m+09$c$n");
		XLText->AddFormatText("$h$c$r$c=$r$e%.3f + (%.3f - %.3f) x (%.3f/%.3f)$m+12$c=$r$e$f3%.3f$me2$m+03%s$n", m_dQmin, m_dQmax, m_dQmin, X2, B_, m_dQvF, pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$iQs$r$c=$r0$r%s$m+03(������ġ�� ������ �������� �ۿ�����)$n", pData->m_szTonf_M2);
	}

	*XLText += 1;
	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");

	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootFront) 
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e%.3f x %.3f x %.3f x %0.2f$m+11$c=$r$e$f3%.3f$me2$m+03%s$n", m_dDistSf, Hf, dUwConc, m_dFactorDeadLoad, m_ScF, pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%.3f x %.3f x %.3f x %.3f/2 x %0.2f$m+11$c=$r$e$f3%.3f$me2$m+03%s/m$n", dDistMftemp, Hf, dUwConc, dDistMftemp, m_dFactorDeadLoad, (n==0)? m_McF:(n == 1 ? m_McF2 : m_McF3), pData->m_szTonfM);


	XLText->AddFormatText("$s+07$h$c-$r$e���� �ݷ¿� ���� �ܸ��$n");

	if(m_bQmaxFront)
	{
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootFront)
			{
				XLText->AddFormatText("$h$r$iVq$r$c=$r$eQs x (%.3f-%.3f) + (Qmax-Qs)/2 x (%.3f-%.3f)$n", wF, wF-m_dDistSf, wF, wF-m_dDistSf);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f + (%.3f-%.3f)/2 x %.3f$n", m_dQvF, m_dDistSf, m_dQmax, m_dQvF, m_dDistSf);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqF, pData->m_szTonf_M);
			}
		}
		XLText->AddFormatText("$h$r$iMq$r$c=$r$eQm x %.3f��/2+ (Qmax-Qm)/2 x %.3f��x 2/3$n", dDistMftemp, dDistMftemp);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2+ (%.3f-%.3f)/2 x %.3f��x 2/3$n", (n==0)? m_dQmF:(n == 1 ? m_dQmF2 : m_dQmF3), dDistMftemp, m_dQmax, (n==0)? m_dQmF:(n == 1 ? m_dQmF2 : m_dQmF3), dDistMftemp);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", m_MqF, pData->m_szTonfM);
	}
	else
	{
		if(m_bQTriangle)
		{
			if(bUse == FALSE)
			{
				if(!m_bOutBoundFootFront)
				{
					XLText->AddFormatText("$h$r$iVq$r$c=$r$eQs x (%.3f-%.3f)/2$n", m_dLx, B-m_dDistSf);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f / 2$n", m_dQvF, m_dLx-(B-m_dDistMf));
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqF, pData->m_szTonf_M);
				}
			}
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQm x %.3f��/2 x 2/3$n", m_dLx-(B-m_dDistMf));
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2 x 2/3$n", (n==0)? m_dQmF:(n == 1 ? m_dQmF2 : m_dQmF3), m_dLx-(B-dDistMftemp));
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", (n==0)? m_MqF:(n == 1 ? m_MqF2 : m_MqF3), pData->m_szTonfM);

		}
		else
		{
			if(bUse == FALSE)
			{
				if(!m_bOutBoundFootFront)
				{
					XLText->AddFormatText("$h$r$iVq$r$c=$r$eQmin x (%.3f) + (Qs-Qmin)/2 x %.3f$n", m_dDistSf, m_dDistSf);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f + (%.3f-%.3f)/2 x %.3f$n", m_dQmin, m_dDistSf, m_dQvF, m_dQmin, m_dDistSf);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqF, pData->m_szTonf_M);
				}
			}
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQmin x %.3f��/2 + (Qm-Qmin) x %.3f��/2 x 1/3$n", dDistMftemp, dDistMftemp);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2 + (%.3f-%.3f) x %.3f��/2 x 1/3$n", m_dQmin, dDistMftemp, (n==0)? m_dQmF:(n == 1 ? m_dQmF2 : m_dQmF3), m_dQmin, dDistMftemp);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", (n==0)? m_MqF:(n == 1 ? m_MqF2 : m_MqF3), pData->m_szTonfM);
		}
	}

	
	XLText->AddFormatText("$s+03$h$c-$r$���� �ܸ�� ����$n");
	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootFront) 
			XLText->AddFormatText("$h$r$iVu$r$c=$r$eVq - Vc$m+02$c=$r$e%.3f - %.3f$m+05$c=$r$e%.3f %s$n", m_SqF, m_ScF, m_dVuFront, pData->m_szTonf_M);
		else 
			XLText->AddFormatText("$h$r$iVu$r$c=$r$e%.3f %s$n", m_dVuFront, pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$r$iMu$r$c=$r$eMq - Mc$m+02$c=$r$e%.3f - %.3f$m+05$c=$r$e%.3f %s/m$n", (n==0)? m_MqF:(n == 1 ? m_MqF2 : m_MqF3), (n==0)? m_McF:(n == 1 ? m_McF2 : m_McF3), (n==0)? m_dMuFront:(n == 1 ? m_dMuFront2 : m_dMuFront3), pData->m_szTonfM);
}

void CLoadFooting::WriteSectionDesignBack(CXLFormatText * XLText, long nIndexSub, BOOL bUse)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	// (37131) ���ʰ��信���� ���߶���� ����Ȱ���� ���� ������.
	// double dLiveLoadAlways = pData->GetValueUnitType((m_pBri->m_bUnderGroundRahmen && !m_pBri->m_bCalcOpen)? m_pBri->m_dUnderGroundDesignLiveAlways:m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	BOOL bIsOutWall = (m_pBri->IsOutsideWall());
	BOOL bOutWall = (bIsOutWall && m_nJijum == 0 || m_nJijum == m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	BOOL bTrap   = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	const HGBOOL bLsd = pData->IsLsdDesign();

	double B = toM(m_pFooting->GetWidth());
	double H = toM(m_pFooting->GetHeight());
	double wB = toM((bStt)? (m_pFooting->m_dFWL + m_pFooting->m_dFSLW):(m_pFooting->m_dFWR + m_pFooting->m_dFSRW));	// �ޱ� ����
	if(bTrap) wB = B - m_dDistMb;

	double dLiveLoadAlways = pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwBackFill = pData->GetValueUnitType(m_pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double B_ = (m_bQTriangle)? m_dLx:B;
	double dC = B-B_;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$s+06$h$i%s$r�ޱ��� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA));
	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	
	double X2 = (m_bQmaxFront)? B-m_dDistMb:m_dDistMb-dC;
	if(X2 > 0)
	{
		XLText->AddFormatText("$h$iQm$r$c=$r$eQmin + (Qmax-Qmin) x ����/��$m+09$c$n");
		XLText->AddFormatText("$h$c$r$c=$r$e%.3f + (%.3f - %.3f) x (%.3f/%.3f)$m+12$c=$r$e$f3%.3f$me2$m+03%s$n", m_dQmin, m_dQmax, m_dQmin, X2, B_, m_dQmB, pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$iQm$r$c=$r0$r%s$m+03(������ġ�� ������ �������� �ۿ�����)$n", pData->m_szTonf_M2);
	}

	X2 = (m_bQmaxFront)? B-m_dDistSb : m_dDistSb-dC;

	if(m_bOutBoundFootBack)
	{
		XLText->AddFormatText("$h$iQs$r$c=$r0$r%s$m+03(������ġ�� ���ʸ� ������Ƿ� �������� ����)$n", pData->m_szTonf_M2);
	}
	else if(X2 > 0)
	{
		XLText->AddFormatText("$h$iQs$r$c=$r$eQmin + (Qmax-Qmin) x ����/��$m+09$c$n");
		XLText->AddFormatText("$h$c$r$c=$r$e%.3f + (%.3f - %.3f) x (%.3f/%.3f)$m+12$c=$r$e$f3%.3f$me2$m+03%s$n", m_dQmin, m_dQmax, m_dQmin, X2, B, m_dQvB, pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$iQs$r$c=$r0$r%s$m+03(������ġ�� ������ �������� �ۿ�����)$n", pData->m_szTonf_M2);
	}

	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootBack) 
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e%.3f x %.3f x %.3f x %0.2f$m+11$c=$r$e$f3%.3f$me2$m+03%s$n", B-m_dDistSb, H, dUwConc, m_dFactorDeadLoad, m_ScB, pData->m_szTonf_M);
	}
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%.3f x %.3f x %.3f x %.3f/2 x %0.2f$m+11$c=$r$e$f3%.3f$me2$m+03%s/m$n", B-m_dDistMb, H, dUwConc, B-m_dDistMb, m_dFactorDeadLoad, m_McB, pData->m_szTonfM);

	if (bOutWall)
	{
		XLText->AddFormatText("$s+03$h$c-$r$e��ä�� ��翡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootBack)
			{
				XLText->AddFormatText("$h$r$iVs$r$c=$r$e(%.3f x %.3f x %.3f) x %0.2f$n", (B-m_dDistSb), m_dHightBackFill, dUwBackFill, m_dFactorSoilHor);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f$me2$m+03%s$n", m_SsB, pData->m_szTonf_M);
			}
		}
		XLText->AddFormatText("$h$r$iMs$r$c=$r$e(%.3f x %.3f x %.3f x %.3f/2) x %0.2f$n", wB, m_dHightBackFill, dUwBackFill, wB, m_dFactorSoilHor);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f$me2$m+03%s/m$n", m_MsB, pData->m_szTonfM);

		XLText->AddFormatText("$s+03$h$c-$r$e���忡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootBack)
			{
				XLText->AddFormatText("$h$r$iVp$r$c=$r$e(%.3f x %.3f / %.3f) x %0.2f$n", (B-m_dDistSb), (m_dFactorPave==0)? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave), wB, m_dFactorPave);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f$me2$m+03%s$n", m_SpB, pData->m_szTonf_M);
			}
		}
		XLText->AddFormatText("$h$r$iMp$r$c=$r$e(%.3f x %.3f / %.3f x %.3f/2) x %0.2f$n", wB, (m_dFactorPave==0)? 0 : (m_dVerLoad[VER_LOAD_PAVE]/m_dFactorPave), wB, wB, m_dFactorPave);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f$me2$m+03%s/m$n", m_MpB, pData->m_szTonfM);
	}

	if(bOutWall && m_pBri->m_bUnderGroundRahmen)
	{
		if(!m_bEarthQuake)
		{
			XLText->AddFormatText("$s+03$h$c-$r$e���� Ȱ���߿� ���� �ܸ��$n");
			if(!m_bOutBoundFootBack) XLText->AddFormatText("$h$r$iVl$r$c=$r$e%.3f x %.3f x %0.2f$m+07$c=$r$e$f3%.3f$me2$m+03%s$n", (B-m_dDistSb), dLiveLoadAlways, m_dFactorLiveLoad, m_SlB, pData->m_szTonf_M);
			XLText->AddFormatText("$h$r$iMl$r$c=$r$e(%.3f x %.3f��/2) x %0.2f$m+07$c=$r$e$f3%.3f$me2$m+03%s/m$n", dLiveLoadAlways, wB, m_dFactorLiveLoad, m_MlB, pData->m_szTonfM);
		}
	}
	else if(bOutWall)
	{
		if(!m_bEarthQuake)
		{
			CString strLoad = bLsd ? _T("��������") : _T("��� Ȱ����");
			XLText->AddFormatText("$s+03$h$c-$r$e%s�� ���� �ܸ��$n", strLoad);
			if(!m_bOutBoundFootBack) XLText->AddFormatText("$h$r$iVl$r$c=$r$e%.3f x %.3f x %0.2f$m+07$c=$r$e$f3%.3f$me2$m+03%s$n", (B-m_dDistSb), dLiveLoadAlways, m_dFactorLiveLoad, m_SlB, pData->m_szTonf_M);
			XLText->AddFormatText("$h$r$iMl$r$c=$r$e(%.3f x %.3f��/2) x %0.2f$m+07$c=$r$e$f3%.3f$me2$m+03%s/m$n", dLiveLoadAlways, wB, m_dFactorLiveLoad, m_MlB, pData->m_szTonfM);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	XLText->AddFormatText("$s+07$h$c-$r$e���ݹݷ� ���� �ܸ��$n");

	if(m_bQmaxFront)
	{	
		if(m_bQTriangle)
		{
			if(bUse == FALSE)
			{
				if(!m_bOutBoundFootBack)
				{
					XLText->AddFormatText("$h$r$iVq$r$c=$r$eQs x (%.3f-%.3f) / 2$n", m_dLx, m_dDistSb);
					XLText->AddFormatText("$h$r$r$c=$r$e[%.3f x (%.3f-%.3f) / 2]$n", m_dQvB, m_dLx, m_dDistSb);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqB, pData->m_szTonf_M);
				}
			}
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQm x %.3f��/2 x 1/3$n", m_dLx-m_dDistMb);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2 x 1/3$n", m_dQmB, m_dLx-m_dDistMb);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", m_MqB, pData->m_szTonfM);
		}
		else
		{
			if(bUse == FALSE)
			{
				if(!m_bOutBoundFootBack)
				{
					XLText->AddFormatText("$h$r$iVq$r$c=$r$eQmin x (%.3f-%.3f) + (Qs-Qmin)/2 x (%.3f-%.3f)$n", B, m_dDistSb, B, m_dDistSb);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f + (%.3f-%.3f)/2 x %.3f$n", m_dQmin, B-m_dDistSb, m_dQvB, m_dQmin, B-m_dDistSb);
					XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqB, pData->m_szTonf_M);
				}
			}
			XLText->AddFormatText("$h$r$iMq$r$c=$r$eQmin x %.3f��/2 + (Qm-Qmin) x %.3f��/2 x 1/3$n", B-m_dDistMb, B-m_dDistMb);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2 + (%.3f-%.3f) x %.3f��/2 x 1/3$n", m_dQmin, B-m_dDistMb, m_dQmB, m_dQmin, B-m_dDistMb);
			XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", m_MqB, pData->m_szTonfM);
		}
	}
	else
	{
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootBack)
			{
				XLText->AddFormatText("$h$r$iVq$r$c=$r$eQs x (%.3f-%.3f) + (Qmax-Qs)/2 x (%.3f-%.3f)$n", B, m_dDistSb, B, m_dDistSb);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f + (%.3f-%.3f)/2 x %.3f$n", m_dQvB, B-m_dDistSb, m_dQmax, m_dQvB, B-m_dDistSb);
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s$n", m_SqB, pData->m_szTonf_M);
			}
		}
		XLText->AddFormatText("$h$r$iMq$r$c=$r$eQm x %.3f��/2 + (Qmax-Qm) x %.3f��/2 x 2/3$n", B-m_dDistMb, B-m_dDistMb);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f x %.3f��/2 + (%.3f-%.3f) x %.3f��/2 x 2/3$n", m_dQmB, B-m_dDistMb, m_dQmax, m_dQmB, B-m_dDistMb);
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f %s/m$n", m_MqB, pData->m_szTonfM);
	}
	
	XLText->AddFormatText("$s+03$h$c-$r$���� �ܸ�� ����$n");
	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootBack)
		{
			if(bOutWall) 
			{
				XLText->AddFormatText("$h$r$iVu$r$c=$r$eVc + Vs + Vp + Vl - Vq$n");
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f + %.3f + %.3f + %.3f - %.3f$m+12$c=$r$e%.3f %s$n", m_ScB, m_SsB, m_SpB, m_SlB, m_SqB, m_dVuBack, pData->m_szTonf_M);
			}
			else 
			{
				XLText->AddFormatText("$h$r$iVu$r$c=$r$eVc - Vq$n");
				XLText->AddFormatText("$h$r$r$c=$r$e%.3f - %.3f$m+12$c=$r$e%.3f %s$n", m_ScB, m_SqB, m_dVuBack, pData->m_szTonf_M);
			}
		}
		else 
			XLText->AddFormatText("$h$r$iVu$r$c=$r$e%.3f %s$n", m_dVuBack, pData->m_szTonf_M);
	}

	if(bOutWall) 
	{
		XLText->AddFormatText("$h$r$iMu$r$c=$r$eMc + Ms + Mp + Ml - Mq$n");
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f + %.3f + %.3f + %.3f - %.3f$m+12$c=$r$e%.3f %s/m$n", m_McB, m_MsB, m_MpB, m_MlB, m_MqB, m_dMuBack, pData->m_szTonfM);
	}
	else
	{
		XLText->AddFormatText("$h$r$iMu$r$c=$r$eMc - Mq$n");
		XLText->AddFormatText("$h$r$r$c=$r$e%.3f - %.3f$m+12$c=$r$e%.3f %s/m$n", m_McB, m_MqB, m_dMuBack, pData->m_szTonfM);
	}
	
	XLText->AddFormatText("$n$h");
}


void CLoadFooting::WriteSectionDesignFront_Lsd(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse)
{
	if(n == 1 && m_pFooting->m_dFELW==0.0 && m_pFooting->m_dFERW==0.0) return;

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double B = m_pFooting->GetWidth();
	double Hf = m_pFooting->GetHeight(FALSE);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$n$h");
	CString str = _T("");
	if(n == 0)
		str = _T("�ձ� (A-A)");
	else if(n == 1)
		str = _T("�ձ�2 (B-B)");
	else if(n == 2)
		str = _T("�ձ�3 (C-C)");
	XLText->AddFormatText("$s+06$h$i%s$r%s�� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA), str);

	XLText->GoPosition(NO_MOVE,COL_POS_3);

	// ������ġ�� ������ ������ �ۿ� �ִٸ� �ܸ���� ���̳ʽ��� ���� �ʰ� 0�̵ȴ�.
	double dDistMftemp = (n==0)? m_dDistMf : (n == 1 ? m_dDistMf2 : m_dDistMf3); 

	*XLText += 1;
	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");

	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootFront) 
		{
			HGDOUBLE dScF = m_dDistSf * Hf * dUwConc;
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e%s x %s x %s$m+11$c=$r$e$f%d%s$me2$m+03%s$n", 
			pData->GetStrDouble(m_dDistSf, UNIT_CGS_M), pData->GetStrDouble(Hf, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dScF, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
	}

	HGDOUBLE dMcF = dDistMftemp * Hf * dUwConc * dDistMftemp / 2;
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%s x %s x %s x %s/2$m+11$c=$r$e%s$me2$m+03%s/mm$n",
		pData->GetStrDouble(dDistMftemp, UNIT_CGS_M), pData->GetStrDouble(Hf, UNIT_CGS_M), 
		pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dDistMftemp, UNIT_CGS_M), pData->GetStrDouble(dMcF, UNIT_CGS_TONF), pData->m_szTonfM);
}

void CLoadFooting::WriteSectionDesignBack_Lsd(CXLFormatText * XLText, long nIndexSub, BOOL bUse)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	// (37131) ���ʰ��信���� ���߶���� ����Ȱ���� ���� ������.
	// double dLiveLoadAlways = pData->GetValueUnitType((m_pBri->m_bUnderGroundRahmen && !m_pBri->m_bCalcOpen)? m_pBri->m_dUnderGroundDesignLiveAlways:m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	BOOL bIsOutWall = (m_pBri->IsOutsideWall());
	BOOL bOutWall = (bIsOutWall && m_nJijum == 0 || m_nJijum == m_pBri->GetCountJijum()-1)? TRUE:FALSE;
	BOOL bTrap   = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (m_pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || m_pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	BOOL bStt = (m_nJijum != m_pBri->GetCountJijum()-1)? TRUE:FALSE;

	double B = m_pFooting->GetWidth();
	double H = m_pFooting->GetHeight();
	double wB = (bStt)? (m_pFooting->m_dFWL + m_pFooting->m_dFSLW):(m_pFooting->m_dFWR + m_pFooting->m_dFSRW);	// �ޱ� ����
	if(bTrap) wB = B - m_dDistMb;

	double dLiveLoadAlways = pData->GetValueUnitType(m_pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	double dUwConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dUwBackFill = pData->GetValueUnitType(m_pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$s+06$h$i%s$r�ޱ��� �ܸ�� ���$n$h", GetNumberString(nIndexSub,NUM_GANA));
	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_3);

	XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");
	if(bUse == FALSE)
	{
		if(!m_bOutBoundFootBack) 
		{
			HGDOUBLE dScB = (B-m_dDistSb) * H * dUwConc;
			XLText->AddFormatText("$h$r$iVc$r$c=$r$e%s x %s x %s$m+11$c=$r$e$f%d%s$me2$m+03%s$n", 
			pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3), pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dScB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		}
	}

	HGDOUBLE dMcB = (B-m_dDistMb) * H * dUwConc * (B-m_dDistMb) / 2;
	XLText->AddFormatText("$h$r$iMc$r$c=$r$e%s x %s x %s x %s/2$m+11$c=$r$e%s$me2$m+03%s/mm$n",
		pData->GetStrDouble(B-m_dDistMb, UNIT_CGS_M), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(dUwConc, UNIT_CGS_TONF_M3),
		pData->GetStrDouble(B-m_dDistMb, UNIT_CGS_M), pData->GetStrDouble(dMcB, UNIT_CGS_TONF), pData->m_szTonfM);

	if (bOutWall)
	{
		XLText->AddFormatText("$s+03$h$c-$r$e��ä�� ��翡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootBack)
			{
				HGDOUBLE dSsB = (B-m_dDistSb) * m_dHightBackFill * dUwBackFill;
				XLText->AddFormatText("$h$r$iVs$r$c=$r$e%s x %s x %s$n",
					pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(m_dHightBackFill, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3));
				XLText->AddFormatText("$h$r$r$c=$r$e$f%d%s$me2$m+03%s$n", pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dSsB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
			}
		}

		HGDOUBLE dMsB = wB * m_dHightBackFill * dUwBackFill * wB / 2;
		XLText->AddFormatText("$h$r$iMs$r$c=$r$e%s x %s x %s x %s/2$n",
			pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(m_dHightBackFill, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(wB, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$r$c=$r$e%s$me2$m+03%s/mm$n", pData->GetStrDouble(dMsB, UNIT_CGS_TONF), pData->m_szTonfM);

		XLText->AddFormatText("$s+03$h$c-$r$e���忡 ���� �ܸ��$n");
		if(bUse == FALSE)
		{
			if(!m_bOutBoundFootBack)
			{
				HGDOUBLE dSpB = (B-m_dDistSb) * m_dVerLoad[VER_LOAD_PAVE] / m_dFactorPave / wB;
				XLText->AddFormatText("$h$r$iVp$r$c=$r$e%s x %s / %s$n",
					pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(m_dVerLoad[VER_LOAD_PAVE] / m_dFactorPave, UNIT_CGS_TONF_M3), pData->GetStrDouble(wB, UNIT_CGS_M));
				XLText->AddFormatText("$h$r$r$c=$r$e$f%d%s$me2$m+03%s$n", pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dSpB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
			}
		}

		HGDOUBLE dMpB = wB * m_dVerLoad[VER_LOAD_PAVE] / m_dFactorPave / wB * wB / 2;
		XLText->AddFormatText("$h$r$iMp$r$c=$r$e%s x %s / %s x %s/2$n",
			pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(m_dVerLoad[VER_LOAD_PAVE] / m_dFactorPave, UNIT_CGS_TONF_M3), pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(wB, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$r$c=$r$e%s$me2$m+03%s/mm$n", pData->GetStrDouble(dMpB, UNIT_CGS_TONF), pData->m_szTonfM);

	}

	if(bOutWall && m_pBri->m_bUnderGroundRahmen)
	{		
		if(!m_bEarthQuake)
		{
			XLText->AddFormatText("$s+03$h$c-$r$e�������߿� ���� �ܸ��$n");
			if(!m_bOutBoundFootBack) 
			{
				HGDOUBLE dSlB = (B-m_dDistSb) * dLiveLoadAlways;
				XLText->AddFormatText("$h$r$iVl$r$c=$r$e%s x %s$m+07$c=$r$e$f%d%s$me2$m+03%s$n",
				pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dSlB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
			}

			HGDOUBLE dMlB = dLiveLoadAlways * pow(wB, 2) / 2;
			XLText->AddFormatText("$h$r$iMl$r$c=$r$e%s x %s��/2$m+07$c=$r$e%s$me2$m+03%s/mm$n",
				pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(dMlB, UNIT_CGS_TONF), pData->m_szTonfM);
		}
	}
	else if(bOutWall)
	{
		if(!m_bEarthQuake)
		{
			CString strLoad = bLsd ? _T("��������") : _T("��� Ȱ����");
			XLText->AddFormatText("$s+03$h$c-$r$e%s�� ���� �ܸ��$n", strLoad);
			if(!m_bOutBoundFootBack) 
			{
				HGDOUBLE dSlB = (B-m_dDistSb) * dLiveLoadAlways;
				XLText->AddFormatText("$h$r$iVl$r$c=$r$e%s x %s$m+07$c=$r$e$f%d%s$me2$m+03%s$n",
				pData->GetStrDouble(B-m_dDistSb, UNIT_CGS_M), pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetDecimal(UNIT_CGS_TONF_M), pData->GetStrDouble(dSlB, UNIT_CGS_TONF_M), pData->m_szTonf_M);
			}

			HGDOUBLE dMIB = dLiveLoadAlways * pow(wB, 2) / 2;
			XLText->AddFormatText("$h$r$iMl$r$c=$r$e%s x %s��/2$m+07$c=$r$e%s$me2$m+03%s/mm$n", 
				pData->GetStrDouble(dLiveLoadAlways, UNIT_CGS_TONF_M2), pData->GetStrDouble(wB, UNIT_CGS_M), pData->GetStrDouble(dMIB, UNIT_CGS_TONF), pData->m_szTonfM);
		}
	}

	XLText->AddFormatText("$n$h");
}
