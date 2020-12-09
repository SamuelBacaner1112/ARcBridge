// ModulusFoundation.cpp: implementation of the CModulusFoundation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModulusFoundation::CModulusFoundation()
{
	for(long jx = 0; jx < MODULUS_POS_MAX; jx++)
	{
		m_B[jx] = 0;
		m_L[jx] = 0;

		for(long ix = 0; ix < MODULUS_FOUNDATION_MAX; ix++)
		{
			m_dAlpah[ix] = 0;
			m_Bv[jx][ix] = 0;
			for(long kk = 0; kk < MODULUS_BORING_MAX; kk++)
			{
				m_Kvo[jx][ix][kk] = 0;
				m_KvBL[jx][ix][kk] = 0;
				m_KvBB[jx][ix][kk] = 0;
				m_Ks[jx][ix][kk] = 0;
			}
		}
	}
}

CModulusFoundation::~CModulusFoundation()
{
} 

void CModulusFoundation::CalculateAll()
{
	if(m_pStd->m_pBri == NULL) return;
	if(m_pStd->m_pBri->IsBoxType() == FALSE) return;

	InitVariables();
	Calculate(MODULUS_FOUNDATION_NORMAL);
	Calculate(MODULUS_FOUNDATION_EQ);
}

void CModulusFoundation::Calculate(long nTypeCalc)
{
	long nJijum = 0;
	CRcBridgeApp *pBri = m_pStd->m_pBri;
	CFootingApp *pFooting = &pBri->m_footingBox;
	CBoringData *pBoringBase = pBri->GetBoringDataJijumBase(nJijum);
	CBoringDataEx *pBoringEx = pBri->GetBoringDataExByJijumNumber(nJijum);
	if(pBoringBase == NULL || pBoringEx == NULL) return;

	m_KvPowFactor = -3./4.; // ������� �������� ȥ�����ΰ��
	if(pBri->m_nVerticalFactor == 0) // ���������� �켼�� ���
	{
		m_KvPowFactor = -1./2.;
	}
	else if(pBri->m_nVerticalFactor == 1) // ���������� �켼�� ���
	{
		m_KvPowFactor = -1.0;
	}

	long nSizeBorig = pBoringEx->m_pArrBoring.GetSize();

	double dElevationBase = toM(pBri->m_dELSB + pBri->m_dTS_Lower / 2.);
	double dElevationTop = dElevationBase + toM(max(pBri->GetHeightModelJijum(0), pBri->GetHeightModelJijum(pBri->m_nQtyJigan)));

	long nIdxBoring = 0;
	m_lArrIndex.RemoveAll();
	for(long ix = nSizeBorig-1; ix >= 0; ix--)
	{
		if(nTypeCalc == MODULUS_FOUNDATION_NORMAL) break; // ������������ ���

		CBoringData *pBoringCurr = pBoringEx->GetBoringByIndex(ix); // �Ʒ���
		CBoringData *pBoringNext = (ix - 1 >= 0) ? pBoringEx->GetBoringByIndex(ix - 1) : NULL; // ����
		double dEleBot = pBoringEx->m_EL - pBoringCurr->m_dDepth;
		double dEleTop = pBoringNext ? pBoringEx->m_EL - pBoringNext->m_dDepth : pBoringEx->m_EL;
		if(dEleTop < dElevationBase) continue;
		if(dEleBot >= dElevationTop) break;
		if(nIdxBoring >= MODULUS_BORING_MAX) break;

		for(long px=0; px<MODULUS_POS_MAX; px++)
		{
			CalculateValue(px, nTypeCalc, nIdxBoring, pBoringCurr);
		}

		m_lArrIndex.Add(ix);
		m_dArrElevationTop.Add(dEleTop);
		m_dArrElevationBottom.Add(dEleBot);

		nIdxBoring++;
	}

	if(nIdxBoring == 0)
	{
		// ������ �϶��� ������ ��ü�� �� ����ϰ� ����϶��� �Ϻν����길 �������� ���� ����ϸ�ȴ�.
		CalculateValue(MODULUS_POS_LOWERSLAB, nTypeCalc, nIdxBoring, pBoringBase);
		nIdxBoring++;
	}
	else
	{
		// ��ܺ��� EL�� �𵨸� ��ܱ��� ����
		double dEleTop = m_dArrElevationTop.GetAt(m_dArrElevationTop.GetSize() - 1);
		m_dArrElevationTop.SetAt(nIdxBoring - 1, max(dEleTop, dElevationTop));
		// ��ܺ��� EL�� �𵨸� �ϴܱ��� ����
		double dEleBot = m_dArrElevationBottom.GetAt(0);
		m_dArrElevationBottom.SetAt(0, min(dEleBot, dElevationBase));
	}

	m_nQtyCalc = nIdxBoring;
}

// dElevation : M
long CModulusFoundation::GetIndexBoringByElevation(double dElevation)
{
	if(m_nQtyCalc == 1) return 0;

	for(long ix = 0; ix < m_nQtyCalc; ix++)
	{
		double dEleBot = m_dArrElevationBottom.GetAt(ix);
		double dEleTop = m_dArrElevationTop.GetAt(ix);
		if(dElevation > dEleBot && dElevation <= dEleTop)
		{
			return ix;
		}
		if(ix == 0 && dElevation <= dEleBot)
		{
			return ix;
		}
		if(ix == m_nQtyCalc - 1 && dElevation >= dEleTop)
		{
			return ix;
		}
	}

	return -1;
}


void CModulusFoundation::CalculateValue(long nPos, long nTypeCalc, long nIdxBoring, CBoringData *pBoring)
{
	long nIdx = nTypeCalc;
	CRcBridgeApp *pBri = m_pStd->m_pBri;
	CFootingApp *pFooting = &pBri->m_footingBox;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	double L = m_L[nPos];
	double B = m_B[nPos];
	double alpha = m_dAlpah[nIdx];
	double dLBase = bLsd? 300 : 0.3;

	// �������ݸ��� �ݷ°��
	double NValue = nPos == MODULUS_POS_LOWERSLAB ? pFooting->m_dNValueBaseGround : pBoring->m_nNValue;
	double Eo = GetValueUnitChange(pFooting->m_dNValue_Eo_Vert * 10, UNIT_CGS_TONF_M2, pData->m_szTonf_M2)  * NValue;
	if(pFooting->m_dEo_Input_Vert > 0)
	{
		Eo = GetValueUnitChange(pFooting->m_dEo_Input_Vert * 10, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	}

	m_Kvo[nPos][nIdx][nIdxBoring] = 1. / dLBase * alpha * Eo;  // Kvo = 1/0.3 x �� x Eo

	// L < B : �ɼǰ� ������� ����
	if(L < B)
	{
		m_Bv[nPos][nIdx] = sqrt(B * L);
		m_KvBL[nPos][nIdx][nIdxBoring] = m_Kvo[nPos][nIdx][nIdxBoring] * pow((m_Bv[nPos][nIdx] / dLBase) , m_KvPowFactor);
	}
	else
	{
		long nSelect = pBri->m_nGroundFactorKV;
		if(nSelect == 0 || nSelect == 1) // Terzaghi ������, Terzaghi ������ ����� �Է�
		{
			m_TerzaghiFactor = (nSelect == 0) ? (1 + 0.5 * B / L) / 1.5 : pBri->m_dGroundFactorKV;

			m_Bv[nPos][nIdx] = sqrt(B * B);
			m_KvBB[nPos][nIdx][nIdxBoring] = m_Kvo[nPos][nIdx][nIdxBoring] * pow((m_Bv[nPos][nIdx] / dLBase) , m_KvPowFactor);
			m_KvBL[nPos][nIdx][nIdxBoring] = m_TerzaghiFactor * m_KvBB[nPos][nIdx][nIdxBoring];
		}
		else // ����������� : L < B�� ���� ������ ������ ����ϸ� �ٸ� Bv ������ ��( B �� L ) �� ��( B �� B )�� �����ϰ� L > B �� ǥ�⸸ �մϴ�.
		{
			m_Bv[nPos][nIdx] = sqrt(B * B);
			m_KvBL[nPos][nIdx][nIdxBoring] = m_Kvo[nPos][nIdx][nIdxBoring] * pow((m_Bv[nPos][nIdx] / dLBase) , m_KvPowFactor); // (L <= B)�� ����
		}
	}

	m_Ks[nPos][nIdx][nIdxBoring] = pBri->m_nEquivalentApplyKsb == 0 ? m_KvBL[nPos][nIdx][nIdxBoring] / pBri->m_dEquivalentApplyKsb : 0;
}


void CModulusFoundation::InitVariables()
{
	CRcBridgeApp *pBri = m_pStd->m_pBri;

	// ARCBRIDGE-3251 : �������̸� ����ϹǷ� ��ü�β��� ������ �ݿ��Ͽ� ó��..
	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	if(pBri->m_nGroundFactorBv == 0) // B ; ��������
	{
		m_B[MODULUS_POS_LOWERSLAB] = toM(pBri->GetLengthBridge(FALSE) + pBri->m_dWE/vAngStt.y + pBri->m_dWS/vAngEnd.y);
		if(pBri->IsExtLine())
			m_L[MODULUS_POS_LOWERSLAB] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge/2, CDPoint(0,1)));
		else
			m_L[MODULUS_POS_LOWERSLAB] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);

		m_B[MODULUS_POS_STTWALL] = toM(pBri->GetHeightModelJijum(0));
		if(pBri->IsExtLine())
			m_L[MODULUS_POS_STTWALL] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1)));
		else
			m_L[MODULUS_POS_STTWALL] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);

		m_B[MODULUS_POS_ENDWALL] = toM(pBri->GetHeightModelJijum(pBri->m_nQtyJigan));
		if(pBri->IsExtLine())
			m_L[MODULUS_POS_ENDWALL] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge, CDPoint(0,1)));
		else
			m_L[MODULUS_POS_ENDWALL] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);
	}
	else
	{
		m_L[MODULUS_POS_LOWERSLAB] = toM(pBri->GetLengthBridge(FALSE) + pBri->m_dWE/vAngStt.y + pBri->m_dWS/vAngEnd.y);
		if(pBri->IsExtLine())
			m_B[MODULUS_POS_LOWERSLAB] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge/2, CDPoint(0,1)));
		else
			m_B[MODULUS_POS_LOWERSLAB] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);

		m_L[MODULUS_POS_STTWALL] = toM(pBri->GetHeightModelJijum(0));
		if(pBri->IsExtLine())
			m_B[MODULUS_POS_STTWALL] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0,1)));
		else
			m_B[MODULUS_POS_STTWALL] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);

		m_L[MODULUS_POS_ENDWALL] = toM(pBri->GetHeightModelJijum(pBri->m_nQtyJigan));
		if(pBri->IsExtLine())
			m_B[MODULUS_POS_ENDWALL] = toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge, CDPoint(0,1)));
		else
			m_B[MODULUS_POS_ENDWALL] = toM(pBri->m_dWidthSlabRight - pBri->m_dWidthSlabLeft);
	}

	if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
	{
		for(long ix=0; ix<MODULUS_POS_MAX; ++ix)
		{
			m_L[ix] = frM(m_L[ix]);
			m_B[ix] = frM(m_B[ix]);
		}
	}

	m_dAlpah[MODULUS_FOUNDATION_NORMAL]		= pBri->GetFooting(0)->m_dFactorAlpha[MODULUS_FOUNDATION_NORMAL];
	m_dAlpah[MODULUS_FOUNDATION_EQ]			= pBri->GetFooting(0)->m_dFactorAlpha[MODULUS_FOUNDATION_EQ];
}
