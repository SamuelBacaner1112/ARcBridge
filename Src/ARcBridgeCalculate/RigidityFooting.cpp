// RigidityFooting.cpp: implementation of the CRigidityFooting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRigidityFooting::CRigidityFooting(CFootingApp *pFooting, CBoringData *pBoring, long nJijum)
{
	m_pFooting  = pFooting;
	m_pBoring   = pBoring;
	m_nJijum	= nJijum;

	// �����ʱ�ȭ....
	m_B	= 0;
	m_D	= 0;
	m_L	= 0;
	m_Es_Base= 0;
	m_Es_Wall= 0;
	m_v	= 0;
	m_G	= 0;
	m_bCombineValue	= FALSE;
	long i=0; for(i=0; i<RIGIDITY_COUNT; i++)
	{
		m_Ro[i]		= 0;
		m_Ko[i]		= 0;
		m_alpha[i]	= 0;
		m_beta[i]	= 0;
		m_ValueK[i]	= 0;
		m_CombK[i]	= 0;
	}
	m_KsBackFill	= 0;
	m_KrBackFill	= 0;
	m_dHeightBackFill	= 0;
	m_dWidthBackFill	= 0;
	m_dH1	= 0;
}

CRigidityFooting::~CRigidityFooting()
{

} 

void CRigidityFooting::CalculateRigidity()
{
//	if(m_pStd->m_pBri->GetTypeRigidFootingEQ() return;
//	��ü�� ������������ ����ؾ���......

	if(m_pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_PILE)
	{
		CalculateRigidityPileBase();
	}
	else
	{
		CalculateRigidityDirectBase();
	}
}

void CRigidityFooting::CalculateRigidityPileBase()
{
	double B = m_pFooting->GetWidth();
	double L = m_pFooting->GetLength();
	m_B   = toM(B)/2;
	m_L   = toM(L)/2;
	m_D   = toM(m_pFooting->GetHeight());;
	m_Es_Base  = m_pFooting->m_dBoringEo * 10;			// (m_pBoring==NULL) ? 0 : m_pBoring->m_dEo*10;	// kgf/cm2 -> tonf/m2
	m_Es_Wall  = (m_pStd->m_pBri->m_dEo_CalcWallSpringEQ>0) ? m_pStd->m_pBri->m_dEo_CalcWallSpringEQ*10 : m_Es_Base;
	m_v   = (m_pBoring==NULL) ? 0 : m_pBoring->m_RatePoasong;
	m_G   = m_Es_Base/(2*(1+m_v));

	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	CDesignPile *pCalcPile = new CDesignPile;
	pCalcPile->SetTypeUnit(m_pStd->m_pBri->m_pARcBridgeDataStd->m_nTypeUnit);
	pCalcPile->m_nIndexJijum = m_nJijum;
	pCalcPile->m_bBridgeDir  = TRUE;
	m_pStd->SetCalcPileData(pCalcPile);

	CPileLoad *pLoadNull = new CPileLoad; // ����� ���� ��¥����...
	pLoadNull->m_dWCaseApply = LOAD_PILE_EQ;

	CExPileApp *pPile = pCalcPile->m_pPile;

	if(bLsd)
		pCalcPile->m_pListExtreme.Add(pLoadNull);
	else
		pCalcPile->m_pListUlt.Add(pLoadNull);
	pCalcPile->SetTypeUnit();
	pCalcPile->CalculateCondition();
	pCalcPile->CalculatePileCenter();
	pCalcPile->CalculatePileAttribute();
	pCalcPile->CalculatePileDisplacementAndReaction();

	double N1  = pPile->GetPileSu(TRUE);
	double N2  = pPile->GetPileSu(FALSE);

	double K1 = pCalcPile->m_K1_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH]; // ������������, t/m
	double K4 = pCalcPile->m_K4_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH]; // t.m/rad
	double Kv = pCalcPile->m_Kv;

	double SumXi  = 0;
	double SumYi  = 0;
	double CenXi  = pCalcPile->m_dCenX;
	double CenYi  = pCalcPile->m_dCenY;
	double DistP  = 0; 
	double EaPile = 0; 
	double dDist = 0;
	double dDistSum = 0;
	long i=0; for(i=0; i<N1; i++)
	{
		dDist = pPile->GetPileDist(i,TRUE);
		dDistSum += dDist;
		DistP  = toM(fabs(CenXi - dDistSum));
		EaPile = pPile->GetPileSu(i,TRUE);
		SumXi += pow(DistP,2)*N2;
	}
	dDistSum = 0;
	for(i=0; i<N2; i++)
	{
		dDist = pPile->GetPileDist(i,FALSE);
		dDistSum += dDist;
		DistP  = toM(fabs(CenYi - dDistSum));
		EaPile = pPile->GetPileSu(i,FALSE);
		SumYi += pow(DistP,2)*EaPile;
	}

	long nQtyPileSum = pPile->GetPileSu();

	CString szTonf = m_pStd->m_pBri->m_pARcBridgeDataStd->m_szTonf;
	m_ValueK[RIGIDITY_DISPX]	= nQtyPileSum*K1;
	m_ValueK[RIGIDITY_DISPY]	= nQtyPileSum*K1;
	m_ValueK[RIGIDITY_DISPZ]	= nQtyPileSum*Kv;
	m_ValueK[RIGIDITY_ROTATEX]	= Kv*SumYi + nQtyPileSum*K4;
	m_ValueK[RIGIDITY_ROTATEY]	= Kv*SumXi + nQtyPileSum*K4;
	m_ValueK[RIGIDITY_ROTATEZ]	= K1*(SumXi+SumYi);
	
	delete pCalcPile;
}

void CRigidityFooting::CalculateRigidityDirectBase()
{
	//////////////////////////////////////////////////////////////////////////
	// ���ʰ�������
	double B = m_pFooting->GetWidth();
	double L = m_pFooting->GetLength();
	m_B   = toM(B)/2;
	m_L   = toM(L)/2;
	m_D   = toM(m_pFooting->GetHeight());;
	m_Es_Base  = m_pFooting->m_dBoringEo * 10;			// (m_pBoring==NULL) ? 0 : m_pBoring->m_dEo*10;	// kgf/cm2 -> tonf/m2(CGS), kN/m��(SI)
	m_Es_Wall  = (m_pStd->m_pBri->m_dEo_CalcWallSpringEQ>0) ? m_pStd->m_pBri->m_dEo_CalcWallSpringEQ*10 : m_Es_Base;

	m_v   = (m_pBoring==NULL) ? 0 : m_pBoring->m_RatePoasong;
	m_G   = Round(m_Es_Base/(2*(1+m_v)),3);

	long i = RIGIDITY_DISPX;
	// �������� X
	m_Ro[i]		= Round(sqrt(4*m_B*m_L/ConstPi),3);
	m_Ko[i]		= 8*m_G*m_Ro[i]/(2-m_v);
	m_alpha[i]  = m_pFooting->m_dEQShapeFactHorX;
	m_beta[i]   = m_pFooting->m_dEQSettleFactHorXY;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];
	// �������� Y
	i = RIGIDITY_DISPY;
	m_Ro[i]		= Round(m_Ro[RIGIDITY_DISPX],3);
	m_Ko[i]		= m_Ko[RIGIDITY_DISPX];
	m_alpha[i]  = m_pFooting->m_dEQShapeFactHorY;
	m_beta[i]   = m_pFooting->m_dEQSettleFactHorXY;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];
	// �������� Z
	i = RIGIDITY_DISPZ;
	m_Ro[i]		 = Round(m_Ro[RIGIDITY_DISPX],3);
	m_Ko[i]		 = 4*m_G*m_Ro[i]/(1-m_v);
	m_alpha[i]	 = m_pFooting->m_dEQShapeFactVerZ;
	m_beta[i]	 = m_pFooting->m_dEQSettleFactVerZ;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];

	// ȸ������ X
	i = RIGIDITY_ROTATEX;
	m_Ro[i]	    = Round(pow(2*m_B*pow(2*m_L,3)/(3*ConstPi),0.25),3);
	m_Ko[i]	    = 8*m_G*m_Ro[i]*m_Ro[i]*m_Ro[i]/(3*(1-m_v));
	m_alpha[i]  = m_pFooting->m_dEQShapeFactRotX;
	m_beta[i]   = m_pFooting->m_dEQSettleFactRotX;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];
	// ȸ������ Y
	i = RIGIDITY_ROTATEY;
	m_Ro[i]		= Round(pow(pow(2*m_B,3)*2*m_L/(3*ConstPi),0.25),3);
	m_Ko[i]		= 8*m_G*m_Ro[i]*m_Ro[i]*m_Ro[i]/(3*(1-m_v));
	m_alpha[i]  = m_pFooting->m_dEQShapeFactRotY;
	m_beta[i]   = m_pFooting->m_dEQSettleFactRotY;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];
	// ȸ������ Z
	i = RIGIDITY_ROTATEZ;
	m_Ro[i]		= Round(pow(4*m_B*m_L*(4*m_B*m_B+4*m_L*m_L)/(6*ConstPi),0.25),3);
	m_Ko[i]		= 16*m_G*m_Ro[i]*m_Ro[i]*m_Ro[i]/3.0;
	m_alpha[i]  = m_pFooting->m_dEQShapeFactTorZ;
	m_beta[i]   = m_pFooting->m_dEQSettleFactTorZ;
	m_ValueK[i] = m_alpha[i]*m_beta[i]*m_Ko[i];
}

// dHeightBackFill : m
// dWidthBackFill  : m
void CRigidityFooting::CalculateBackFillAndCombine(double dHeightBackFill, double dWidthBackFill)
{
	long i=0; for(i=0; i<RIGIDITY_COUNT; i++)
	{
		m_CombK[i] = m_ValueK[i];
	}
	if(Compare(dHeightBackFill,0.0,"=") || Compare(dWidthBackFill,0.0,"=")) return;

	BOOL bLsd = m_pStd->m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	// �������� ������ ��쿡�� ���....
	m_bCombineValue = TRUE;

	m_dHeightBackFill = dHeightBackFill;
	m_dWidthBackFill  = dWidthBackFill;

	// (ARCBRIDGE-2663) �Ѱ�����϶��� ��ü���� ������ mm�� �Ǵµ� ������ kN/m �϶��� ���� ���⶧���� ��ü���̸� �����ϸ� ���� Ŀ����.
	// �տ����� ����� ������ �ְ� �Ǹ� ��ü ������ȯ�� �ٲ� �ִ°� ������ ��ü�� �ٲܼ� ������ ��갪�� �ٲ��ְ� ������ �״�� �������.
	m_KsBackFill = 0.425 * m_Es_Wall * m_dWidthBackFill; // �����⺯������
	m_KrBackFill = 0.072 * m_Es_Wall * m_dWidthBackFill * pow(m_dHeightBackFill,2); // Ⱦ����ȸ������

	m_dH1 = Round(0.37*m_dHeightBackFill,3);

	if(m_pStd->m_pBri->GetTypeRigidFootingEQ()!=0) return;

	// ��ä�򰭼��� ���ʰ����� ����
	// (ARCBRIDGE-2663) �Ѱ�����϶� ���̴����� mm�̱� ������ ��ü ������ 1000�� �����ش�. (1000�� �ƴ϶� ��ü ���� ��ü�� �����ִ°� ���� �ʳ�?)
	i = RIGIDITY_DISPX;
	m_CombK[i] = m_KsBackFill + m_ValueK[i];

	i = RIGIDITY_ROTATEY;
	m_CombK[i] = pow(m_dH1,2)*m_KsBackFill + m_KrBackFill + m_ValueK[i];
}
