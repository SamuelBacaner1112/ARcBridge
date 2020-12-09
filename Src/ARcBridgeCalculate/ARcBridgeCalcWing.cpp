// ARcBridgeCalcWing.cpp: implementation of the CARcBridgeCalcWing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STR_BRACKET_STT		m_bLsd? _T("(") : _T("")
#define STR_BRACKET_END		m_bLsd? _T(")") : _T("")
#define STR_SIMBOL_ETA		m_bLsd? _T(" x ηi") : _T("")

#define AREA_A		0
#define AREA_B		1
#define AREA_C		2
#define AREA_D		3
#define AREA_A_		4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcWing::CARcBridgeCalcWing()
{
	m_bStt	= TRUE;
	m_bLeft	= TRUE;

	m_bIsA	= TRUE;
	m_bIsD	= FALSE;
	m_bIsA_	= FALSE;
	m_bIsA_Calc = TRUE;
	m_xyA	= CDPoint(0,0);
	m_xyB	= CDPoint(0,0);
	m_xyC	= CDPoint(0,0);
	m_xyD	= CDPoint(0,0);
	m_xyE	= CDPoint(0,0);
	m_xyF	= CDPoint(0,0);
	m_xyA_	= CDPoint(0,0);
	m_xyE_	= CDPoint(0,0);

	m_dLoadAlways	= 0;
	m_dLoadAlways_LS	= 0;
	m_dWeightBackFill	= 0;
	m_dPi			= 0;
	m_dK0			= 0;
	m_dHs			= 0;
	
	m_bLsd	= FALSE;
	for(long ix = 0; ix < WING_CASE_COUNT; ix++)
	{
		m_dFactorToap[ix]	= 0;
		m_dFactorLive[ix]	= 0;
		m_dEta[ix] = 1.0;
		m_szCombo[ix] = _T("");
	}
	
	long i=0; for(i=0; i<5; i++)
	{
		m_S[i]		= 0;
		m_M[i]		= 0;
		m_Vu[i]		= 0;
		m_Mu[i]		= 0;

		m_S_Use[i]	= 0;
		m_M_Use[i]	= 0;
		m_Vu_Use[i]	= 0;
		m_Mu_Use[i]	= 0;

		m_S_Use_Judge[i]	= 0;
		m_M_Use_Judge[i]	= 0;
		m_Vu_Use_Judge[i]	= 0;
		m_Mu_Use_Judge[i]	= 0;
	}
}

CARcBridgeCalcWing::~CARcBridgeCalcWing()
{

}

void CARcBridgeCalcWing::CalculateAll(BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) return;
	if(pBri->IsPiRahmen())   return;
	m_pWing = m_pStd->m_pBri->GetWingWall(bStt,bLeft);
	if(m_pWing==NULL) return;
	if(m_pWing->m_bExist==FALSE) return;

	m_bStt	= bStt;
	m_bLeft	= bLeft;

	int se = m_bStt ? -1 : 1;
	int nLeft = m_bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
	
	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection	= pBri->m_tvArrVertSection;
	pBri->m_nTypeJongdanForRebar = 2;
	pBri->SyncBridge(nLeft, TRUE);

	m_bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	MakeArea();
	if(m_bIsD)
	{
		Calculate_BlockD();
	}
	if(m_bIsA)
	{
		if(m_bIsA_Calc) 
			Calculate_BlockA();
		Calculate_BlockA_Add();
		Calculate_BlockB();
		Calculate_BlockC();
	}
	SetConcData();

	pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebarOrg;
	pBri->m_tvArrVertSection		= tvArrVertSection;

	pBri->SyncBridge(0, TRUE);
}

void CARcBridgeCalcWing::Calculate_BlockA()
{
	double h1 = m_bLsd? (m_xyBaseTop.y-m_xyA_.y) : toM(m_xyBaseTop.y-m_xyA_.y);
	double dL = m_bLsd? (fabs(m_xyA.x-m_xyE.x)) : toM(fabs(m_xyA.x-m_xyE.x));
	BOOL bCutAreaD = (m_xyA_.y > m_xyF.y)? TRUE:FALSE;

	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dc = m_dK0 * (m_dWeightBackFill*h1);			// 토압
	double dc_ = m_dK0 * (m_dWeightBackFill*m_dHs);		// 토피에의한 토압

	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLeft && i > WING_FACTOR_USE)
			break;

		long nTypeFactor = i;
		BOOL bUse = (nTypeFactor==WING_FACTOR_ULT)? FALSE : TRUE;
		BOOL bJudge = (nTypeFactor==WING_FACTOR_USE_JUDGE)? TRUE : FALSE;
		double dFactorToap = m_dFactorToap[nTypeFactor];
		double dFactorLive = m_dFactorLive[nTypeFactor];

		double Sd = (bUse)? (bJudge? m_S_Use_Judge[AREA_D] : m_S_Use[AREA_D]):m_S[AREA_D];
		double Md = (bUse)? (bJudge? m_M_Use_Judge[AREA_D] : m_M_Use[AREA_D]):m_M[AREA_D];

		if(bCutAreaD)
		{
			double Sd_ = GetValueAreaDCut(h1, 0, bUse);
			double Md_ = GetValueAreaDCut(h1, 1, bUse);
			Sd -= Sd_;
			Md -= Md_;
		}

		double Pa = (dFactorToap * (1/2. * dc * h1 + dc_ * h1) + (dFactorLive * dq[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor] * dq[LIVE_LOAD_LS]) * h1) * m_dEta[nTypeFactor];
		double Sa = Sd + Pa * dL;
		double Ma = Md + Sd*dL + 1/2.*(Pa*pow(dL, 2));

		if(i==WING_FACTOR_ULT)
		{
			m_S[AREA_A]		= Sa;
			m_M[AREA_A]		= Ma;
			m_Vu[AREA_A]	= Sa/h1;
			m_Mu[AREA_A]	= Ma/h1;
		}
		else if(i==WING_FACTOR_USE)
		{
			m_S_Use[AREA_A]		= Sa;
			m_M_Use[AREA_A]		= Ma;
			m_Vu_Use[AREA_A]	= Sa/h1;
			m_Mu_Use[AREA_A]	= Ma/h1;
		}
		else
		{
			m_S_Use_Judge[AREA_A]		= Sa;
			m_M_Use_Judge[AREA_A]		= Ma;
			m_Vu_Use_Judge[AREA_A]	= Sa/h1;
			m_Mu_Use_Judge[AREA_A]	= Ma/h1;
		}
	}
}

void CARcBridgeCalcWing::Calculate_BlockB()
{
	double h1 = m_xyBaseTop.y-m_xyB.y;
	double h2 = m_xyBaseTop.y-(m_xyB.y+m_xyC.y)/2;
	double dL = fabs(m_xyA.x-m_xyE.x);
	if(m_bLsd == FALSE)
	{
		h1 = toM(h1);
		h2 = toM(h2);
		dL = toM(dL);
	}

	BOOL bCutAreaD = (m_xyB.y > m_xyF.y)? TRUE: FALSE;
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 중간부 토압

	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLeft && i > WING_FACTOR_USE)
			break;

		BOOL bUse = (i==WING_FACTOR_USE)? TRUE : FALSE;
		BOOL bJudge = (i==WING_FACTOR_USE_JUDGE)? TRUE : FALSE;
		long nTypeFactor = i;
		double dFactorToap = m_dFactorToap[nTypeFactor];
		double dFactorLive = m_dFactorLive[nTypeFactor];

		double P1 = (dFactorToap*dq1 + (dFactorLive*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor] * dq[LIVE_LOAD_LS])) * m_dEta[nTypeFactor];
		double P2 = (dFactorToap*dq2 + (dFactorLive*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor] * dq[LIVE_LOAD_LS])) * m_dEta[nTypeFactor];
		
		double Sb = P1 * dL;
		double Mb = (1/2.) * P1 * pow(dL, 2);
		double Sb_ = P2 * dL/2;
		double Mb_ = (1/2.) * P2 * pow(dL/2, 2);
		if(bCutAreaD)
		{
			double dHd_ = m_bLsd? (m_xyB.y - m_xyF.y) : toM(m_xyB.y - m_xyF.y);
			double Sd_ = GetValueAreaDCut(h1, 0, bUse, bJudge)/dHd_;
			double Md_ = GetValueAreaDCut(h1, 1, bUse, bJudge)/dHd_;
			Sb += Sd_;
			Mb += (Md_+Sd_*dL);
			Sb_ += Sd_;
			Mb_ += (Md_+Sd_*dL);
		}

		if(i==WING_FACTOR_ULT)
		{
			m_S[AREA_B]		= MAX(Sb, Sb_);
			m_M[AREA_B]		= MAX(Mb, Mb_);
			m_Vu[AREA_B]	= MAX(Sb, Sb_);
			m_Mu[AREA_B]	= MAX(Mb, Mb_);
		}
		else if(i==WING_FACTOR_USE)
		{
			m_S_Use[AREA_B]		= MAX(Sb, Sb_);
			m_M_Use[AREA_B]		= MAX(Mb, Mb_);
			m_Vu_Use[AREA_B]	= MAX(Sb, Sb_);
			m_Mu_Use[AREA_B]	= MAX(Mb, Mb_);
		}
		else
		{
			m_S_Use_Judge[AREA_B]		= MAX(Sb, Sb_);
			m_M_Use_Judge[AREA_B]		= MAX(Mb, Mb_);
			m_Vu_Use_Judge[AREA_B]	= MAX(Sb, Sb_);
			m_Mu_Use_Judge[AREA_B]	= MAX(Mb, Mb_);
		}
	}
}

void CARcBridgeCalcWing::Calculate_BlockC()
{
	double h1 = m_xyBaseTop.y-m_xyB.y;
	double h2 = m_xyBaseTop.y-(m_xyB.y+m_xyC.y)/2;
	double h3 = m_xyBaseTop.y-m_xyC.y;
	double dL = fabs(m_xyA.x-m_xyE.x);
	if(m_bLsd == FALSE)
	{
		h1 = toM(h1);
		h2 = toM(h2);
		h3 = toM(h3);
		dL = toM(dL);
	}

	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 중간부 토압
	double dq3 = m_dK0 * (m_dWeightBackFill*(h3+m_dHs));			// 하단부 토압

	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLeft && i > WING_FACTOR_USE)
			break;

		long nTypeFactor = i;
		double dFactorToap = m_dFactorToap[nTypeFactor];
		double dFactorLiveES = m_dFactorLive[nTypeFactor];
		double dFactorLiveLS = m_dFactorLive_LS[nTypeFactor];

		double P1 = (dFactorToap*dq1 + (dFactorLiveES * dq[LIVE_LOAD_ES] + dFactorLiveLS * dq[LIVE_LOAD_LS])) * m_dEta[nTypeFactor];
		double P2 = (dFactorToap*dq2 + (dFactorLiveES * dq[LIVE_LOAD_ES] + dFactorLiveLS * dq[LIVE_LOAD_LS])) * m_dEta[nTypeFactor];
		double P3 = (dFactorToap*dq3 + (dFactorLiveES * dq[LIVE_LOAD_ES] + dFactorLiveLS * dq[LIVE_LOAD_LS])) * m_dEta[nTypeFactor];

		double Sc = (P1+P3)/2 * dL;
		double Mc = (1/6.) * (2*P1 + P3) * pow(dL, 2);
		double Sc_ = (P2+P3)/2 * dL/2;
		double Mc_ = (1/6.) * (2*P2 + P3) * pow(dL/2, 2);

		if(i==WING_FACTOR_ULT)
		{
			m_S[AREA_C]		= MAX(Sc, Sc_);
			m_M[AREA_C]		= MAX(Mc, Mc_);
			m_Vu[AREA_C]	= MAX(Sc, Sc_);
			m_Mu[AREA_C]	= MAX(Mc, Mc_);
			
		}
		else if(i==WING_FACTOR_USE)
		{
			m_S_Use[AREA_C]		= MAX(Sc, Sc_);
			m_M_Use[AREA_C]		= MAX(Mc, Mc_);
			m_Vu_Use[AREA_C]	= MAX(Sc, Sc_);
			m_Mu_Use[AREA_C]	= MAX(Mc, Mc_);
		}
		else
		{
			m_S_Use_Judge[AREA_C]		= MAX(Sc, Sc_);
			m_M_Use_Judge[AREA_C]		= MAX(Mc, Mc_);
			m_Vu_Use_Judge[AREA_C]	= MAX(Sc, Sc_);
			m_Mu_Use_Judge[AREA_C]	= MAX(Mc, Mc_);
		}
	}		
}

void CARcBridgeCalcWing::Calculate_BlockD()
{
	double h2(0);
	double Sh_(0);
	double Mh_(0);
	double Sl[2] = { 0, 0 };
	double Ml[2] = { 0, 0 };
	double Sh(0);
	double Mh(0);

	if(m_pWing->m_bMatchBottom)
	{
		double dL1 = fabs(m_xyE_.x - m_xyF.x);
		double dL2 = fabs(m_xyF.x - m_xyE.x);
		double h1 = m_dFH;	// m_pWing->m_dHL;
		if(m_bLsd == FALSE)
		{
			dL1 = toM(dL1);
			dL2 = toM(dL2);
			h1 = toM(h1);
		}

		double sw = m_pWing->m_dSW;
		double h3 = dL1/sw;
		h2 = h1 + h3;

		// 1 : 상단 사각형(h1 x (L1 + L2)), 2 : 경사부 삼각형(h2 x L1 / 2), 3 : 평면부 사각형(h2 x L2)
		// 상재 하중에 의한 단면력
		//상재하중 : ES, LS
		for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
		{
			Sl[nLive] = Ml[nLive] = 0;
			if(IsLiveLoad(nLive == LIVE_LOAD_ES))
			{
				double dLoadAlways = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);
				Sl[nLive] = m_dK0 * dLoadAlways * (h1*(dL1 + dL2) + dL1*h3/2. + h3*dL2);
				Ml[nLive] = m_dK0 * dLoadAlways * ((h1*pow(dL1+dL2, 2))/2 + (dL1*h3/2.) * (dL2 + dL1/3.) + h3*pow(dL2, 2)/2);
			}
		}

		// 토압에 의한 단면력
		Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*(dL1 + dL2) + (h1*h3*dL1 + pow(h3, 2)/3.*dL1) + h1*2*h3*dL2);
		Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL1 + dL2, 2))/2 + (h1*h3*dL1 + pow(h3, 2)/3.*dL1) * (dL2 + dL1/3.) + h1*h3*pow(dL2, 2));

		// 토피에 의한 단면력
		if(m_dHs > 0)
		{
			Sh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * (h1*(dL1 + dL2) + dL1*h3/2. + h3*dL2);
			Mh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * ((h1*pow(dL1+dL2, 2))/2 + (dL1*h3/2.) * (dL2 + dL1/3.) + h3*pow(dL2, 2)/2);
		}
	}
	else
	{
		double dL = fabs(m_xyE_.x - m_xyE.x);
		double h1 = m_dFH; // m_pWing->m_dHL;
		if(m_bLsd == FALSE)
		{
			dL = toM(dL);
			h1 = toM(h1);
		}

		double sw = m_pWing->m_dSW;
		h2 = h1+dL/sw;

		// 상재 하중에 의한 단면력
		//상재하중 : ES, LS
		for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
		{
			Sl[nLive] = Ml[nLive] = 0;
			if(IsLiveLoad(nLive == LIVE_LOAD_ES))
			{
				double dLoadAlways = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);
				Sl[nLive] = m_dK0 * dLoadAlways * (h1*dL + pow(dL, 2)/(2*sw));
				Ml[nLive] = m_dK0 * dLoadAlways * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
			}
		}

		// 토압에 의한 단면력
		Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*dL + h1*pow(dL, 2)/sw + pow(dL, 3)/(3*pow(sw, 2)));
		Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL, 2))/2 + (h1*pow(dL, 3))/(3*sw) + pow(dL, 4)/(12*pow(sw, 2)));

		// 토피에 의한 단면력
		if(m_dHs > 0)
		{
			Sh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * (h1*dL + pow(dL, 2)/(2*sw));
			Mh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
		}
	}

	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLeft && i > WING_FACTOR_USE)
			break;

		long nTypeFactor = i;
		double dFactorToap = m_dFactorToap[nTypeFactor];
		double dFactorLiveES = m_dFactorLive[nTypeFactor];
		double dFactorLiveLS = m_dFactorLive_LS[nTypeFactor];

		double Sd = (dFactorToap*(Sh+Sh_) + dFactorLiveES*Sl[LIVE_LOAD_ES] + dFactorLiveLS*Sl[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];
		double Md = (dFactorToap*(Mh+Mh_) + dFactorLiveES*Ml[LIVE_LOAD_ES] + dFactorLiveLS*Ml[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];

		if(i==WING_FACTOR_ULT)
		{
			m_S[AREA_D]		= Sd;
			m_M[AREA_D]		= Md;
			m_Vu[AREA_D]	= h2 > 0 ? Sd/h2 : 0.0;
			m_Mu[AREA_D]	= h2 > 0 ? Md/h2 : 0.0;
		}
		else if(i==WING_FACTOR_USE)
		{
			m_S_Use[AREA_D]	= Sd;
			m_M_Use[AREA_D]	= Md;
			m_Vu_Use[AREA_D]	= h2 > 0 ? Sd/h2 : 0.0;
			m_Mu_Use[AREA_D]	= h2 > 0 ? Md/h2 : 0.0;
		}
		else
		{
			m_S_Use_Judge[AREA_D]	= Sd;
			m_M_Use_Judge[AREA_D]	= Md;
			m_Vu_Use_Judge[AREA_D]	= h2 > 0 ? Sd/h2 : 0.0;
			m_Mu_Use_Judge[AREA_D]	= h2 > 0 ? Md/h2 : 0.0;
		}
	}
}

void CARcBridgeCalcWing::Calculate_BlockA_Add()
{
	double h1 = m_xyBaseTop.y-m_xyA_.y;			// 상단 H
	double h2 = m_xyBaseTop.y-m_xyB.y;			// 하단 H
	double hl = m_xyA_.y-m_xyB.y;
	double dL = fabs(m_xyA.x-m_xyE.x);
	if(m_bLsd == FALSE)
	{
		h1 = toM(h1);
		h2 = toM(h2);
		hl = toM(hl);
		dL = toM(dL);
	}

	BOOL bCutAreaD1 = (m_xyA_.y > m_xyF.y)? TRUE: FALSE;
	BOOL bCutAreaD2 = (m_xyB.y > m_xyF.y)? TRUE: FALSE;
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 하단 토압
	
	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLeft && i > WING_FACTOR_USE)
			break;

		BOOL bUse = (i==WING_FACTOR_USE)? TRUE : FALSE;
		BOOL bJudge = (i==WING_FACTOR_USE_JUDGE)? TRUE : FALSE;
		long nTypeFactor = i;
		double dFactorToap = m_dFactorToap[nTypeFactor];
		double dFactorLiveES = m_dFactorLive[nTypeFactor];
		double dFactorLiveLS = m_dFactorLive_LS[nTypeFactor];

		double P1 = (dFactorToap * (1/2. * (dq1 + dq2) * hl) + (dFactorLiveES * dq[LIVE_LOAD_ES] + dFactorLiveLS * dq[LIVE_LOAD_LS]) * hl) * m_dEta[nTypeFactor];
		
		double Sa_ = P1 * dL;
		double Ma_ = (1/2.) * P1 * pow(dL, 2);
	
		if(bCutAreaD1)
		{
			double dHd_ = m_bLsd? (m_xyA_.y - m_xyB.y) : toM(m_xyA_.y - m_xyB.y);
			double Sd_ = GetValueAreaDCut(h1, 0, bUse, bJudge);
			double Md_ = GetValueAreaDCut(h1, 1, bUse, bJudge);
			if(bCutAreaD2)
			{
				Sd_ -= GetValueAreaDCut(h2, 0, bUse, bJudge);
				Md_ -= GetValueAreaDCut(h2, 1, bUse, bJudge);
			}
			
			Sa_ += Sd_/dHd_;
			Ma_ += (Md_+Sd_*dL)/dHd_;
		}

		if(i==WING_FACTOR_ULT)
		{
			m_S[AREA_A_]	= Sa_;
			m_M[AREA_A_]	= Ma_;
			m_Vu[AREA_A_]	= Sa_ > 0 ? Sa_/hl : 0;
			m_Mu[AREA_A_]	= Ma_ > 0 ? Ma_/hl : 0;
		}
		else if(i==WING_FACTOR_USE)
		{
			m_S_Use[AREA_A_]	= Sa_;
			m_M_Use[AREA_A_]	= Ma_;
			m_Vu_Use[AREA_A_]	= Sa_ > 0 ? Sa_/hl : 0;
			m_Mu_Use[AREA_A_]	= Ma_ > 0 ? Ma_/hl : 0;
		}
		else
		{
			m_S_Use_Judge[AREA_A_]	= Sa_;
			m_M_Use_Judge[AREA_A_]	= Ma_;
			m_Vu_Use_Judge[AREA_A_]	= Sa_ > 0 ? Sa_/hl : 0;
			m_Mu_Use_Judge[AREA_A_]	= Ma_ > 0 ? Ma_/hl : 0;
		}
	}
}

double CARcBridgeCalcWing::GetValueAreaDCut(double dh, long nType, BOOL bUse, BOOL bJudge)		// nType 0: Sd', 1: Md'
{
	double hLow = m_xyE.y - m_xyF.y;
	double dHl	= m_dFH;	// m_pWing->m_dHL;
	double dL1	= fabs(m_xyE_.x - m_xyE.x);
	if(m_bLsd == FALSE)
	{
		hLow	= toM(hLow);
		dHl		= toM(dHl);
		dL1		= toM(dL1);
	}

	double h1 = (dHl < dh)? 0:(dHl-dh);
	double sw = m_pWing->m_dSW;
	double dL = MIN((hLow-dh) * sw, dL1);
	
	if(hLow < dh) return 0;
	
	// 상재 하중에 의한 단면력
	double Sl[LIVE_LOAD_COUNT], Ml[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		Sl[nLive] = Ml[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			double dLoadAlways = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);
			Sl[nLive] = m_dK0 * dLoadAlways * (h1*dL + pow(dL, 2)/(2*sw));
			Ml[nLive] = m_dK0 * dLoadAlways * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
		}
	}
	
	// 토압에 의한 단면력
	double Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*dL + h1*pow(dL, 2)/sw + pow(dL, 3)/(3*pow(sw, 2)));
	double Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL, 2))/2 + (h1*pow(dL, 3))/(3*sw) + pow(dL, 4)/(12*pow(sw, 2)));
	
	// 토피에 의한 단면력
	double Sh_ = 0;
	double Mh_ = 0;
	if(m_dHs+dh > 0)
	{
		Sh_ = m_dK0 * ((m_dHs+dh) * m_dWeightBackFill) * (h1*dL + pow(dL, 2)/(2*sw));
		Mh_ = m_dK0 * ((m_dHs+dh) * m_dWeightBackFill) * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
	}
	
	long nTypeFactor = bUse? (bJudge? WING_FACTOR_USE_JUDGE : WING_FACTOR_USE) : WING_FACTOR_ULT;
	double Sd = (m_dFactorToap[nTypeFactor]*(Sh+Sh_) + m_dFactorLive[nTypeFactor]*Sl[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor]*Sl[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];
	double Md = (m_dFactorToap[nTypeFactor]*(Mh+Mh_) + m_dFactorLive[nTypeFactor]*Ml[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor]*Ml[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];

	double dRet = (nType == 0)? Sd : Md;
	
	return dRet;
}

void CARcBridgeCalcWing::MakeArea()
{
	CRcBridgeApp *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CTwinVectorArray tvArr;
	CTwinVector tv;

	int se = m_bStt ? -1 : 1;
	
	pBri->GetTvWingWallFront(m_bStt, m_bLeft, 1, tvArr, TRUE, FALSE, TRUE, FALSE);

	double dW = 0;
	double dL1 = 0;

	tv = tvArr.GetTvByInfo("날개벽정면하단");
	dW = tv.GetHorLength();
	m_xyC = tv.m_v2;
	m_xyD = tv.m_v1;
	m_bIsA = (m_pWing->m_dL2 > 0)? TRUE:FALSE;
	
	//부상방지저판이 있을경우 날개벽 정면 하단의 범위가 다름.
	if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
	{
		double dWall = m_bStt ? pBri->m_dWS : pBri->m_dWE;
		CDPoint xySta = pBri->GetXySectionSttEnd(m_bStt, m_bStt == m_bLeft ? 1 : -1);
		xySta.x += dWall * se;
		CDPoint xyBottom = pBri->GetXyLowerSlabBottomByOffsetFrStation(xySta.x, 0, m_bStt == m_bLeft ? 1 : -1);
		xyBottom.y += pBri->m_dPRF_H1 + pBri->m_dPRF_H2;
		m_xyC = CDPoint(xySta.x, xyBottom.y);


		dW  = fabs(m_xyD.x - m_xyC.x);
	}

	tv = tvArr.GetTvByInfo("날개벽정면끝단상부");
	CDPoint xyWingEnd = tv.m_v1;
	m_xyE_ = xyWingEnd;
	m_xyE = CDPoint(m_xyD.x, xyWingEnd.y);
	m_xyA = CDPoint(m_xyC.x, xyWingEnd.y);
	dL1 = m_pWing->m_dL1;//fabs(m_xyE_.x - m_xyE.x);

	tv = tvArr.GetTvByInfo("날개벽정면노리");
	m_xyF = tv.m_v2;
	m_xyB = m_xyC+CDPoint(0, dW);
	m_xyB.y = MIN(m_xyB.y, m_xyA.y);
	m_bIsA_Calc = (m_bIsA && (m_xyA.y - m_xyB.y) > 350.0)? TRUE : FALSE;
	
	m_bIsD  = (dL1 > 0)? TRUE:FALSE;
	m_bIsA_ = (m_bIsA_Calc && m_pWing->m_nDirHunch == 1 && dL1 != m_pWing->m_dT2)? TRUE:FALSE;
	if(m_bIsA_) m_xyA_ = m_xyA - CDPoint(0, m_pWing->m_dLT + (m_pWing->m_dT2 - m_pWing->m_dT1)*m_pWing->m_dSH);
	if(!m_bIsA_ || m_xyA_.y < m_xyB.y)
	{
		m_bIsA_ = FALSE;
		m_xyA_ = m_xyB;
	}

	// 조건변수
	m_dLoadAlways = GetValueUnitChange(pData->IsRailDesign()? (1.5+3.5) : pBri->m_dLiveLoadAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
	m_dLoadAlways_LS = 0;
	m_dWeightBackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	m_dPi = pBri->m_dAngleFrictionBackFill;
	m_dK0 = m_pStd->m_dCoefficientKo;
	m_dHs = MAX(toM(m_pWing->m_dHeightSoil),0);
	if(m_bLsd)
	{
		m_dHs = frM(m_dHs);
	}

	m_xyBaseTop = pBri->m_nApplyWingWallHeightBasePos == 0 ? pBri->GetXySectionWingWallStt(m_bStt, TRUE) : xyWingEnd;

	m_dFH		=	m_pWing->m_dHL - (xyWingEnd.y - m_xyBaseTop.y);
	
	if(m_bLsd)
	{
		CLsdManagerBridge *pLsdManager = pBri->m_plsdManager;
		for(long i = 0; i < WING_CASE_COUNT; i++)
		{
			CLsdLoadCase *pLoadCase = pBri->m_plsdManager->GetLsdLoadCaseWingWallOrAps(m_dEta[i], m_szCombo[i], i != WING_FACTOR_ULT, i == WING_FACTOR_USE_JUDGE, TRUE, m_bStt);
			if(!pLoadCase) continue;

			m_dFactorToap[i] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_EH);
			m_dFactorLive[i] =	max(pLoadCase->GetLoadFactor(eLSD_LOADCASE_ES), pLoadCase->GetLoadFactor(eLSD_LOADCASE_ES2));
			m_dFactorLive_LS[i] =	max(pLoadCase->GetLoadFactor(eLSD_LOADCASE_LS), pLoadCase->GetLoadFactor(eLSD_LOADCASE_LS2));
		}

		// 상재활하중 ES
		if(IsLiveLoad_ES() == FALSE)
		{
			m_dLoadAlways = 0;
		}

		// 상재활하중 LS
		if(IsLiveLoad_LS())
		{
			// (ARCBRIDGE-29710) 선형중심에서의 벽체 높이에서 날개벽 높이로 변경
			double dHeightWall = pBri->GetXySectionWingWallStt(m_bStt, TRUE).y -  pBri->GetXySectionWingWallStt(m_bStt, FALSE).y;;
			double dEquivalentHeight = GetEquivalentHeightWall(dHeightWall, pBri->m_nLiveLoadDistToBackWall);
			// (ARCBRIDGE-3009) 도로의 종류와 상관없이 다차로재하계수는 적용하지 않는것으로 변경
			double dRoadWaysFactor = 1.0; // m_pStd->m_pLoad->GetCalculateRoadWayFactor();
			double dCorrFactor = pBri->m_bCorrectionFactor_LiveLoadAlways ? pBri->m_dCorrectionFactor_LiveLoadAlways : 1.00;

			m_dLoadAlways_LS = m_dWeightBackFill * dEquivalentHeight * dCorrFactor * dRoadWaysFactor; // 차로에 대한 factor은 미적용
		}
	}
	else
	{
		m_dEta[WING_FACTOR_USE] = m_dEta[WING_FACTOR_ULT] = 1.0;
		m_dFactorToap[WING_FACTOR_USE]	= 1.0;
		m_dFactorLive[WING_FACTOR_USE]	= 1.0;
		m_dFactorToap[WING_FACTOR_ULT] = pBri->m_dFactorToap;
		m_dFactorLive[WING_FACTOR_ULT] = pBri->m_dFactorLive;
		m_dFactorLive_LS[WING_FACTOR_USE]	= 0;
		m_dFactorLive_LS[WING_FACTOR_ULT]	= 0;
	}
}

void CARcBridgeCalcWing::DrawWingWallArea(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);
	CTwinVectorArray tvArr;
	CTwinVector tv;

	long iDir = (m_bStt)? 1:-1;
	CDPoint xyTemp(0, 0);
	double dL1 = fabs(m_xyE_.x - m_xyE.x);
	double dW = fabs(m_xyA.x - m_xyE.x);	
	double dSpare2 = 0.0;
	int se = m_bStt ? -1 : 1;
	int lr = m_bLeft ? -1 : 1;
	int nLeft = m_bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.

	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection	= pBri->m_tvArrVertSection;
	pBri->m_nTypeJongdanForRebar = 2;
	pBri->SyncBridge(nLeft, TRUE);

	double h1 = m_dFH;	// m_pWing->m_dHL;
	double h2 = (m_bIsA_)? m_xyBaseTop.y - m_xyA_.y:0;
	double h3 = (m_bIsA)? m_xyBaseTop.y - m_xyB.y:0;
	double h5 = m_xyBaseTop.y - m_xyC.y;
	double h4 = (m_bIsA)? h3 + (h5-h3)/2:0;
	if(m_pWing->m_bMatchBottom && m_pWing->m_dH2 > 0)
		h2 = m_xyBaseTop.y - m_xyF.y;
	
	double q0 = m_dK0 * m_dWeightBackFill * m_dHs;
	double q1 = m_dK0 * m_dWeightBackFill * ((m_bLsd? h1 : toM(h1))+m_dHs);
	double q2 = m_dK0 * m_dWeightBackFill * ((m_bLsd? h2 : toM(h2))+m_dHs);
	double q3 = m_dK0 * m_dWeightBackFill * ((m_bLsd? h3 : toM(h3))+m_dHs);
	double q4 = m_dK0 * m_dWeightBackFill * ((m_bLsd? h4 : toM(h4))+m_dHs);
	double q5 = m_dK0 * m_dWeightBackFill * ((m_bLsd? h5 : toM(h5))+m_dHs);

	// 정면도
	CDomyun DomFront(pDomP);
	DomFront.SetScaleDim(150);

	m_pStd->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, m_bStt, m_bLeft, 1, FALSE, FALSE, FALSE);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, m_bStt, m_bLeft);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
	DomFront.SetDimExo(5);
	DomFront.SetDimVLen(5);
	DomFront.SetDimLevelDis(5);

	DomFront.SetDirection("TOP");
	DomFront.DimMoveTo(m_xyA.x, m_xyA.y, 0);
	DomFront.DimLineTo(-dW*iDir, 0, COMMA(dW));
	DomFront.DimLineTo(-dL1*iDir, 0, COMMA(dL1));
	if(m_bIsA)
	{
		DomFront.DimMoveTo(m_xyA.x, m_xyA.y, 1);
		DomFront.DimLineTo(-(dW+dL1)*iDir, 1, COMMA(dW+dL1));
	}
	
	DomFront.SetDirection(m_bStt? "LEFT":"RIGHT");
	if(dL1 > 0)
	{
		double dLenSlope = m_xyE_.y - m_xyF.y - m_pWing->m_dHL;
		DomFront.DimMoveTo(m_xyE.x-(dL1+2500)*iDir, m_xyBaseTop.y, 0);
		DomFront.DimLineTo(-m_dFH, 0, COMMA(m_dFH));
		DomFront.DimLineTo(-dLenSlope, 0, COMMA(dLenSlope));
		DomFront.DimLineTo(-(m_xyF.y-m_xyD.y), 0, COMMA(m_xyF.y-m_xyD.y));
	}
	DomFront.DimMoveTo(m_xyE.x-(dL1+2500)*iDir, m_xyBaseTop.y, (dL1>0)? 1:0);
	DomFront.DimLineTo(m_xyC.y-m_xyBaseTop.y, (dL1>0)? 1:0, COMMA(m_xyBaseTop.y-m_xyC.y));
	if(pBri->m_bUnderGroundRahmen && m_dHs > 0)
	{
		DomFront.DimMoveTo(m_xyE.x-(dL1+2500)*iDir, m_xyE.y, 0);
		DomFront.DimLineToExtend(m_bLsd? m_dHs : frM(m_dHs), 0, COMMA(m_bLsd? m_dHs : frM(m_dHs)), "Ht");
	}

	DomFront.SetDirection(!m_bStt? "LEFT":"RIGHT");
	DomFront.DimMoveTo(m_xyA.x+2000*iDir, m_xyA.y, 0);
	DomFront.DimLineTo(m_xyA_.y-m_xyA.y, 0, COMMA(m_xyA.y-m_xyA_.y));
	DomFront.DimLineTo(m_xyB.y-m_xyA_.y, 0, COMMA(m_xyA_.y-m_xyB.y));
	DomFront.DimLineTo(m_xyC.y-m_xyB.y, 0, COMMA(m_xyB.y-m_xyC.y));

	if(m_bIsA)
	{
		if(m_bIsA_Calc) DomFront.TextOut(m_xyA.x+250*iDir, m_xyA.y, "a");
		DomFront.TextOut(m_xyB.x+250*iDir, m_xyB.y, "b");
		DomFront.TextOut(m_xyB.x+250*iDir, (m_xyB.y+m_xyC.y)/2, "b'");
		DomFront.TextOut(m_xyC.x+250*iDir, m_xyC.y, "c");
		DomFront.TextOut((m_xyC.x+m_xyD.x)/2, m_xyC.y-250, "c'");
		DomFront.TextOut(m_xyD.x-250*iDir, m_xyD.y, "d");
	}

	DomFront.TextOut(m_xyE.x-250*iDir, m_xyE.y, "e");
	if(dL1 > 0) DomFront.TextOut(m_xyF.x-250*iDir, m_xyF.y-250, "f");
	if(dL1 > 0) DomFront.TextOut(m_xyE.x-dL1/2*iDir, m_xyE.y-m_pWing->m_dHL, "D 구간");
	
	if(m_bIsA)
	{
		if(m_bIsA_) DomFront.TextOut((m_xyA.x+m_xyE.x)/2, (m_xyA.y+m_xyA_.y)/2, "A 구간");
		else DomFront.TextOut((m_xyA.x+m_xyE.x)/2, (m_xyA.y+m_xyB.y)/2, "A 구간");
		DomFront.TextOut(m_xyD.x+fabs(m_xyC.x-m_xyD.x)*(2/3.)*iDir, m_xyB.y-(m_xyB.y-m_xyC.y)*(1/4.), "B 구간");
		DomFront.TextOut(m_xyD.x+fabs(m_xyC.x-m_xyD.x)*(1/3.)*iDir, m_xyB.y-(m_xyB.y-m_xyC.y)*(3/4.), "C 구간");
		
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DASH);
		xyTemp = m_xyC;
		DomFront.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x = m_xyD.x;
		xyTemp.y = m_xyB.y;
		DomFront.LineTo(xyTemp.x, xyTemp.y);
		xyTemp = m_xyB;
		DomFront.LineTo(xyTemp.x, xyTemp.y);
		
		xyTemp = m_xyE;
		DomFront.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp = m_xyF;
		DomFront.LineTo(xyTemp.x, xyTemp.y);
		
		if(m_bIsA_)
		{
			xyTemp = m_xyA_;
			DomFront.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x = m_xyD.x;
			DomFront.LineTo(xyTemp.x, xyTemp.y);
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
			DomFront.SetDimExo(5);
			DomFront.SetDimVLen(5);
			
			DomFront.TextOut(m_xyA_.x+200*iDir, m_xyA_.y, "a'");
			DomFront.TextOut((m_xyA_.x+m_xyE.x)/2, (m_xyA_.y+m_xyB.y)/2, "A' 구간");
		}
		
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
		DomFront.SetDimExo(5);
		DomFront.SetDimVLen(5);
	}
	//////////////////////////////////////////////////////////////////////////
	long nDistx = (long)(m_xyE.x + (dL1+800) * -iDir);
	double dFactor = 2000/q5;
	double dLenArrow = 0;
	long ArrowDir = (iDir == 1)? 0:4;

	xyTemp.x = nDistx; 
	xyTemp.y = m_xyBaseTop.y;
	DomFront.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= h5;
	DomFront.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= q0*dFactor*iDir; 
	xyTemp.y = m_xyBaseTop.y;
	DomFront.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= (q5-q0)*dFactor*iDir;
	xyTemp.y -= h5;
	DomFront.LineTo(xyTemp.x, xyTemp.y);

	if(pBri->m_bUnderGroundRahmen && m_dHs > 0)
	{
		xyTemp.x = nDistx; 
		xyTemp.y = m_xyBaseTop.y+(m_bLsd? m_dHs : frM(m_dHs));
		DomFront.MoveTo(xyTemp.x-(q5*dFactor*iDir), xyTemp.y);
		xyTemp.x = m_xyE.x - dL1 *iDir;
		DomFront.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x = nDistx - (q5*dFactor*iDir)/2;
		CHgBaseDrawStd::InsertHCadBlock(&DomFront, "CGEARTH", xyTemp, DomFront.GetScaleDim());
	}

	if(q0 > 0)
	{
		dLenArrow = q0*dFactor;
		xyTemp.x = nDistx - (dLenArrow * iDir);
		xyTemp.y = m_xyBaseTop.y;
		DomFront.DimArrowTo(xyTemp.x, xyTemp.y, dLenArrow, ArrowDir);
	}
	
	CString str = _T("");
	double dArrH[5] = { h1, h2, h3, h4, h5 };
	double dArrQ[5] = { q1, q2, q3, q4, q5 };
	long n=0; for(n=0; n<5; n++)
	{
		if(dArrH[n] != 0)
		{
			dLenArrow = dArrQ[n]*dFactor;
			xyTemp.x = nDistx - (dLenArrow * iDir);
			xyTemp.y = m_xyBaseTop.y - dArrH[n];
			str.Format("q%d", n+1);
			DomFront.TextOut(xyTemp.x-230*iDir, xyTemp.y, str);
			DomFront.DimArrowTo(xyTemp.x, xyTemp.y, dLenArrow, ArrowDir);
			str.Format("h%d", n+1);
			xyTemp.x += dLenArrow * iDir;
			DomFront.TextOut(xyTemp.x+230*iDir, xyTemp.y, str);
		}
	}

	CDRect rcFront = DomFront.GetExtRect();
	
	*pDomP << DomFront;
	
	// 단면도
	CDomyun DomSect(pDomP);
	DomSect.SetScaleDim(150);

	m_pStd->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, m_bStt, m_bLeft);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_DIML);
	DomSect.SetDimExo(5);
	DomSect.SetDimVLen(5);
	DomSect.SetDimLevelDis(5);
	
	DBStd.m_pDrawWingWall->DimGenSection(&DomSect, m_bStt, m_bLeft, FALSE);
	
	CDRect rcSect = DomSect.GetExtRect();
	
	if(se*lr>0)
		DomSect.Move((rcFront.Width()+rcSect.Width()+dSpare2)*se, 0);
	else
		DomSect.Move((rcFront.Width()+dSpare2)*se, 0);
	
	pBri->m_tvArrVertSection		= tvArrVertSection;
	pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebarOrg;
	*pDomP << DomSect;
}

void CARcBridgeCalcWing::WriteDesignWingWall(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nTypeCalc = pBri->m_nSelectWingCalc;// 관용, 혼합, 3D

	BOOL bManual = nTypeCalc != 2 ? TRUE : FALSE;
	BOOL bCalc3D = nTypeCalc != 0 ? TRUE : FALSE;

	long nIndexSmall = 1;
	long nIndexSub	= 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	CString szTitle = _T("");
	szTitle += (m_bStt)? "시점부":"종점부";
	szTitle += (m_bLeft)? " 좌측 날개벽":" 우측 날개벽";
	if(nIndexMiddle != 1) XLText->AddFormatText("$s+99");
	XLText->AddFormatText("$e$h$a%d.%d %s$n$n", nIndexTitle, nIndexMiddle++, szTitle);

	// (ARCBRIDGE-2659) 날개벽 단면 삽도는 무조건 출력하도록 요청.
	if(TRUE)		// (bManual)
	{
		if(bManual)
		{
			CString strTail = (bCalc3D) ? _T(" : 관용법") : _T("");
			XLText->AddFormatText("$e$h %d)$r단면력 산정%s$n", nIndexSmall++,strTail);
		}
		else
		{
			XLText->AddFormatText("$e$h %d)$r단면력 산정 : 3D Plate 해석$n$n", nIndexSmall++);
		}

		CDomyun *pDom = new CDomyun;

		DrawWingWallArea(pDom);

		double dDomHight = pDom->GetExtHeight();
		double dDomWidth = pDom->GetExtWidth();

		long nRowDom = (long)MIN(17, 28 * (dDomHight/dDomWidth));  
		*XLText += 1;
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW | CXLFormatText::INSERT_DOM_CENTER);
		*XLText += 1;

		if(m_bLsd)
		{
			CString str(_T(""));
			for(long ix = 0; ix < WING_CASE_COUNT; ix++)
			{
				long nCASE = ix;
				CString strLS, strES;
				if(IsLiveLoad(TRUE))
				{
					strES.Format(_T(" + %.3f·ES"), m_dFactorLive[nCASE]);
				}
				if(IsLiveLoad(FALSE))
				{
					strLS.Format(_T(" + %.3f·LS"), m_dFactorLive_LS[nCASE]);
				}
				str.Format(_T("%s  = %.3f·EH%s%s, ηi = %.3f : %s"), m_szCombo[nCASE], m_dFactorToap[nCASE], strES, strLS, m_dEta[nCASE], GetStringTypeCase(nCASE));
				XLText->AddFormatText(_T("$h$r%s$n"), str);
			}
		}
		else
		{
			XLText->AddFormatText("$h$r$cU$r=$r$e%.3f x H + %.3f x L$n", m_dFactorToap[WING_FACTOR_ULT], m_dFactorLive[WING_FACTOR_ULT]);
		}
		
		XLText->AddFormatText("$h$r$e정지토압계수$m+04Ko$r$c=$r$e1 - sinΦ  = 1 - sin(%.1f) = %.3f$n", m_dPi, m_dK0);
		*XLText += 1;

		if(m_bLsd)
		{
			WriteCalcAlwaysLiveLoad(XLText);
		}

		if(bManual)
		{
			if(m_bIsD)
			{
				if(m_pWing->m_bMatchBottom)
					WriteForceAreaD_Flat(XLText, nIndexSub++);
				else
					WriteForceAreaD(XLText, nIndexSub++);
			}
			if(m_bIsA)
			{
				if(m_bIsA_Calc) WriteForceAreaA(XLText, nIndexSub++);
				if(m_bIsA_) WriteForceAreaA_Add(XLText, nIndexSub++);
				WriteForceAreaB(XLText, nIndexSub++);
				WriteForceAreaC(XLText, nIndexSub++);
			}
			WriteForceSummary(XLText, nIndexSub++,FALSE,FALSE);
		}
	}

	if(bCalc3D)
	{
		nIndexSub = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		if(bManual)
		{
			XLText->AddText(_T("$s+20"));
			XLText->AddFormatText("$e$h %d)$r단면력 산정 : 3D Plate 해석$n$n", nIndexSmall++);
		}
		else
			XLText->AddText(_T("$s+10"));

		WriteForce3DPlate(XLText, nIndexSub++);
		WriteForceSummary(XLText, nIndexSub++,TRUE,FALSE);
		*XLText += 1;
	}

	if(bManual && bCalc3D)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		WriteForceSummary(XLText, nIndexSmall++, TRUE, TRUE);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	WriteCheckSection(XLText, nIndexSmall);
	
}

void CARcBridgeCalcWing::WriteForce3DPlate(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = pBri->GetFemWingWall(m_bStt, m_bLeft);
	if(!pFEM)	return;

	XLText->GoPosition(NO_MOVE, 2);
	CString	str = _T("");
	str.Format(_T("%s %s$n"), GetNumberString(nIndexSub++, NUM_CIRCLE),hggettext("해석 모델"));
	XLText->AddFormatText(_T("$h%s$h"), str);

	CDomyun *pDom = new CDomyun;
	pDom->SetScaleDim(50);
	pDom->SetLTScale(100*5);
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom, HCAD_CONC);

	POSITION pos = pFEM->m_ShellList.GetHeadPosition();
//	CShell *pShell = NULL;
	CJoint *pJoint = NULL;
	CDPoint	xyPos(0, 0), xyStt(0, 0);
	CDPointArray	xyArr;
	long nNum = 1;
	while(pos)		
	{
		xyArr.RemoveAll();
		CShell *pShell = (CShell*)pFEM->m_ShellList.GetNext(pos);
		if(!pShell)	continue;

		long j = 0; for(j = 0; j < 4; j++)
		{
			pJoint = pFEM->GetJoint(pShell->m_nJ[j]);
			if(!pJoint)	continue;

			xyPos	=	m_bLsd? CDPoint(pJoint->m_vPoint.x, pJoint->m_vPoint.z) : CDPoint(frM(pJoint->m_vPoint.x), frM(pJoint->m_vPoint.z));
			xyArr.Add(xyPos);
			if(j == 0)
			{
				pDom->MoveTo(xyPos);
				xyStt = xyPos;
			}
			else
			{
				pDom->LineTo(xyPos);
				if(j == 3)
					pDom->LineTo(xyStt);
			}
		}

		xyPos = CDPoint(0, 0);
		long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
			xyPos += xyArr.GetAt(i);

		xyPos	=	xyPos / 4;
		str.Format(_T("%d"), nNum++);
		pDom->TextOut(xyPos, str);
	}

	// 그림이 잘리는 것을 막기위해 약간의 영역을 주자
	pDom->SetCalcExtRect();
	CDRect rc = pDom->GetExtRect();
	double dMargin = rc.Width() * 0.01;
	pDom->Point(rc.left - dMargin, rc.top - dMargin);
	pDom->Point(rc.right + dMargin, rc.bottom + dMargin);
	
	XLText->InsertDomyun(pDom, m_pStd->m_pView, 18, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_VCENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	XLText->AddText(_T("$n"));
}


void CARcBridgeCalcWing::WriteForceAreaA(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	double h1 = m_xyBaseTop.y-m_xyA_.y;
	double dL = fabs(m_xyA.x-m_xyE.x);
	if(m_bLsd == FALSE)
	{
		h1 = toM(h1);
		dL = toM(dL);
	}

	BOOL bCutAreaD = (m_xyA_.y > m_xyF.y)? TRUE:FALSE;
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dc = m_dK0 * (m_dWeightBackFill*h1);			// 토압
	double dc_ = m_dK0 * (m_dWeightBackFill*m_dHs);		// 토피에의한 토압
	
	double Sd = 0;
	double Md = 0;
	double Sd_ = 0;
	double Md_ = 0;
	if(bCutAreaD)
	{
		Sd_ = GetValueAreaDCut(h1, 0);
		Md_ = GetValueAreaDCut(h1, 1);
		Sd = m_S[AREA_D] - Sd_;
		Md = m_M[AREA_D] - Md_;
	}
	else
	{
		Sd = m_S[AREA_D];
		Md = m_M[AREA_D];
	}
	
	double Pa = (m_dFactorToap[WING_FACTOR_ULT] * (1/2. * dc * h1 + dc_ * h1) + (m_dFactorLive[WING_FACTOR_ULT] * dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT] * dq[LIVE_LOAD_LS]) * h1) * m_dEta[WING_FACTOR_ULT];
	double Sa = Sd + Pa * dL;
	double Ma = Md + Sd*dL + 1/2.*(Pa*pow(dL, 2));

	CString szDWbackFill(_T("")), szValueEta(_T(""));
	szDWbackFill = pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3);
	if(m_bLsd)
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$e$h%s$r단면 A$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	WriteCalcLoad_qq(XLText);
	
	if(m_dHs > 0)
	{
		XLText->AddFormatText("$h$eq0'$r$c=$r$eKo x γt x 토피$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, szDWbackFill, pData->GetStrDouble(m_dHs, UNIT_CGS_M), pData->GetStrDouble(dc_, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x (h3+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, szDWbackFill, pData->GetStrDouble(h1+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dc+dc_, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		*XLText += 1;

		CString str(_T(""));
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			str.Format("$h$ePa$r$c=$r$e%s%.3f x ½ x (q0'+q%d) x h%d + (%.3f x q0 + %.3f x q0') x h%d%s%s$n",
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_bIsA_? 2 : 3, m_bIsA_? 2 : 3, 
				m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT],  m_bIsA_? 2 : 3, STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText(_T("$h$e%s$n"), str);

			str.Format("$r$c=$r$e%s%.3f x ½ x (%s+%s) x %s + (%.3f x %s + %.3f x %s) x %s%s%s$n", 
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dc, UNIT_CGS_TONF_M2), pData->GetStrDouble(dc_, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), 
				m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_ES], UNIT_CGS_TONF_M2), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_LS], UNIT_CGS_TONF_M2), 
				pData->GetStrDouble(h1, UNIT_CGS_M), STR_BRACKET_END, szValueEta);
			XLText->AddFormatText(_T("$h$e%s$n"), str);
		}
		else
		{
			long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
			double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
			str.Format("$h$ePa$r$c=$r$e%s%.3f x ½ x q%d x h%d + %.3f x q0 x h%d%s%s$n",
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_bIsA_? 2 : 3, m_bIsA_? 2 : 3, dFactorLive, m_bIsA_? 2 : 3, STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText(_T("$h$e%s$n"), str);

			str.Format("$r$c=$r$e%s%.3f x ½ x (%s+%s) x %s + %.3f x %s x %s%s%s$n", 
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dc, UNIT_CGS_TONF_M2), pData->GetStrDouble(dc_, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), 
				dFactorLive, pData->GetStrDouble(dq[nTypeLive], UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), STR_BRACKET_END, szValueEta);
			XLText->AddFormatText(_T("$h%s$n"), str);
		}
	}
	else
	{
		if(m_bIsA_)
		{
			XLText->AddFormatText("$h$eq2$r$c=$r$eKo x γt x h2$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
				m_dK0, szDWbackFill, pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dc, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x h3$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
				m_dK0, szDWbackFill, pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dc, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		}
		*XLText += 1;
		
		CString str(_T(""));
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			str.Format("$h$ePa$r$c=$r$e%s%.3f x ½ x q%d x h2 + (%.3f x q0 + %.3f x q0') x h2%s%s",
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_bIsA_? 2 : 3, m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText(_T("$h$e%s$n"), str);

			str.Format("$r$c=$r$e%s%.3f x ½ x %s x %s + (%.3f x %s + %.3f x %s) x %s%s%s", 
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dc+dc_, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), 
				m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_ES], UNIT_CGS_TONF_M2), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_LS], UNIT_CGS_TONF_M2), 
				pData->GetStrDouble(h1, UNIT_CGS_M), STR_BRACKET_END, szValueEta);
			XLText->AddFormatText(_T("$h$e%s$n"), str);
		}
		else
		{
			long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
			double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
			str.Format("$h$ePa$r$c=$r$e%s%.3f x ½ x q%d x h2 + %.3f x q0 x h2%s%s",
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_bIsA_? 2 : 3, dFactorLive, STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText(_T("$h$e%s$n"), str);

			str.Format("$r$c=$r$e%s%.3f x ½ x %s x %s + %.3f x %s x %s%s%s", 
				STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dc+dc_, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), 
				dFactorLive, pData->GetStrDouble(dq[nTypeLive], UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), STR_BRACKET_END, szValueEta);
			XLText->AddFormatText(_T("$h%s$n"), str);
		}
	}
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->m_szTonf);

	if(m_bIsD && bCutAreaD)
	{
//		pXLText->AddFormatText("$r* D구간 추가 부분 산정$n$h");

		XLText->AddFormatText("$h* D구간 추가 부분 산정시 공제 부분$n$h");
		WriteCalcLoad_Dminus(XLText, 3, h1, FALSE);
	}

	if(m_bIsD) XLText->AddFormatText("$h$eSa$r$c=$r$eSd + Pa x L$n");
	else XLText->AddFormatText("$h$eSa$r$c=$r$ePa x L$n");

	if(!m_bIsD)
	{
		XLText->AddFormatText("$h$r$c=$r$e%s x %s$n", 
			pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	else if(bCutAreaD)
	{
		XLText->AddFormatText("$h$r$c=$r$e(%s - %s) + %s x %s$n", 
			pData->GetStrDouble(Sd+Sd_, UNIT_CGS_TONF), pData->GetStrDouble(Sd_, UNIT_CGS_TONF), pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	else
	{
		XLText->AddFormatText("$h$r$c=$r$e%s + %s x %s$n", 
			pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sa, UNIT_CGS_TONF), pData->m_szTonf);

	if(m_bIsD) XLText->AddFormatText("$h$eMa$r$c=$r$eMd + Sd x L + ½ x Pa x L²$n");
	else XLText->AddFormatText("$h$eMa$r$c=$r$e½ x Pa x L²$n");

	if(!m_bIsD)
	{
		XLText->AddFormatText("$h$r$c=$r$e½ x %s x %s²$n", 
			pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	else if(bCutAreaD)
	{
		XLText->AddFormatText("$h$r$c=$r$e(%s - %s) +  (%s - %s) x %s + ½ x %s x %s²$n", 
			pData->GetStrDouble(Md+Md_, UNIT_CGS_TONFM), pData->GetStrDouble(Md_, UNIT_CGS_TONFM) , 
			pData->GetStrDouble(Sd+Sd_, UNIT_CGS_TONF), pData->GetStrDouble(Sd_, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M), 
			pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	else
	{
		XLText->AddFormatText("$h$r$c=$r$e%s + %s x %s + ½ x %s x %s²$n", 
			pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M), 
			pData->GetStrDouble(Pa, UNIT_CGS_TONF), pData->GetStrDouble(dL, UNIT_CGS_M));
	}
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Ma, UNIT_CGS_TONFM), pData->m_szTonfM);

	*XLText += 1;
	if(m_bIsA_) XLText->AddFormatText("$h$ea-a' 단면에 단위폭으로 등분포 시키면$n");
	else XLText->AddFormatText("$h$ea-b 단면에 단위폭으로 등분포 시키면$n");
	XLText->AddFormatText("$h$cVu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Sa, UNIT_CGS_TONF), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(m_Vu[AREA_A], UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$cMu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Ma, UNIT_CGS_TONFM), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(m_Mu[AREA_A], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

void CARcBridgeCalcWing::WriteForceAreaB(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double h1 = m_xyBaseTop.y-m_xyB.y;
	double h2 = m_xyBaseTop.y-(m_xyB.y+m_xyC.y)/2;
	double dL = fabs(m_xyA.x-m_xyE.x);
	BOOL bCutAreaD = (m_xyB.y > m_xyF.y)? TRUE: FALSE;
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		h1 = toM(h1);
		h2 = toM(h2);
		dL = toM(dL);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 중간부 토압
	
	double P1 = (m_dFactorToap[WING_FACTOR_ULT]*dq1 + m_dFactorLive[WING_FACTOR_ULT]*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*dq[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	double P2 = (m_dFactorToap[WING_FACTOR_ULT]*dq2 + m_dFactorLive[WING_FACTOR_ULT]*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*dq[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	
	double Sb = P1 * dL;
	double Mb = (1/2.) * P1 * pow(dL, 2);
	double Sb_ = P2 * dL/2;
	double Mb_ = (1/2.) * P2 * pow(dL/2, 2);
	double Sd_ = 0;
	double Md_ = 0;
	double dHd_ = 0;
	if(bCutAreaD)
	{
		dHd_ = m_bLsd? (m_xyB.y - m_xyF.y) : toM(m_xyB.y - m_xyF.y);
		Sd_ = GetValueAreaDCut(h1, 0)/dHd_;
		Md_ = GetValueAreaDCut(h1, 1)/dHd_;
		Sb += Sd_;
		Mb += (Md_+Sd_*dL);
		Sb_ += Sd_;
		Mb_ += (Md_+Sd_*dL);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+10$e$h%s$r단면 B$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$c*$r$eb-b' 및 b'-c의 두구간으로 분할하여 불리한 단면력으로 설계$n");
	WriteCalcLoad_qq(XLText);

	if(m_dHs > 0)
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x (h3+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq4$r$c=$r$eKo x γt x (h4+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	else 
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x h3$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq4$r$c=$r$eKo x γt x h4$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	*XLText += 1;

	CString strqq(_T(""));
	if(pBri->m_nSelectWingLoad_LS_ES == 2)
	{
		strqq.Format(_T("%.3f x %s + %.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_ES], UNIT_CGS_TONF_M2), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_LS], UNIT_CGS_TONF_M2));
	}
	else
	{
		long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
		double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
		strqq.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(dq[nTypeLive], UNIT_CGS_TONF_M2));
	}

	XLText->AddFormatText("$h$eP1$r$c=$r$e%s%.3f x %s + %s%s%s$n", 
		STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), strqq, STR_BRACKET_END, szValueEta);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	XLText->AddFormatText("$h$eP2$r$c=$r$e%s%.3f x %s + %s%s%s$n", 
		STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), strqq, STR_BRACKET_END, szValueEta);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	*XLText += 1;

	if(m_bIsD && bCutAreaD)
	{
		XLText->AddFormatText("$s+09$h* D구간 추가 부분 산정$n$h");

		WriteCalcLoad_Dminus(XLText, 3, h1, FALSE);
	}

	if(bCutAreaD)
	{
		XLText->AddFormatText("$s+02$h$eSd'$r$c=$r$eVd' / hd$m+06$c=$r$e%s / %s = %s %s$n", 
			pData->GetStrDouble(Sd_*dHd_, UNIT_CGS_TONF), pData->GetStrDouble(dHd_, UNIT_CGS_M), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$s+02$h$eMd'$r$c=$r$eMd' / hd$m+06$c=$r$e%s / %s = %s %s$n", 
			pData->GetStrDouble(Md_*dHd_, UNIT_CGS_TONFM), pData->GetStrDouble(dHd_, UNIT_CGS_M), pData->GetStrDouble(Md_, UNIT_CGS_TONFM), pData->m_szTonfM);
		*XLText += 1;

		XLText->AddFormatText("$s+14$h$eSb$r$c=$r$eP1 x L + Sd'$m+06$c=$r$e%s x %s + %s$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sb, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMb$r$c=$r$e½ x P1 x L²+ (Md' + Sd' x L)$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %s x %s²+ (%s + %s x %s)$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M), 
			pData->GetStrDouble(Md_, UNIT_CGS_TONFM), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/m$n", pData->GetStrDouble(Mb, UNIT_CGS_TONFM), pData->m_szTonfM);
		*XLText += 1;
		XLText->AddFormatText("$h$eSb'$r$c=$r$eP2 x L + Sd'$m+06$c=$r$e%s x %s + %s$n", 
			pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sb_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMb'$r$c=$r$e½ x P2 x L²+ (Md' + Sd' x L)$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %s x %s²+ (%s + %s x %s)$n", 
			pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M), 
			pData->GetStrDouble(Md_, UNIT_CGS_TONFM), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/m$n", pData->GetStrDouble(Mb_, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	else
	{
		XLText->AddFormatText("$h$eSb$r$c=$r$eP1 x L$m+05$c=$r$e%s x %s$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", 
			pData->GetStrDouble(Sb, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMb$r$c=$r$e½ x P1 x L²$m+05$c=$r$e½ x %s x %s²$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/m$n", 
			pData->GetStrDouble(Mb, UNIT_CGS_TONFM), pData->m_szTonfM);
		*XLText += 1;
		XLText->AddFormatText("$h$eSb'$r$c=$r$eP2 x L$m+05$c=$r$e%s x %s$n", 
			pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", 
			pData->GetStrDouble(Sb_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMb'$r$c=$r$e½ x P2 x L²$m+05$c=$r$e½ x s x s²$n", 
			pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/m$n", 
			pData->GetStrDouble(Mb_, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	*XLText += 1;

	XLText->AddFormatText("$h$eVu$r$c=$r$e%s %s$n", 
		pData->GetStrDouble(m_Vu[AREA_B], UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$eMu$r$c=$r$e%s %s/m$n", 
		pData->GetStrDouble(m_Mu[AREA_B], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

void CARcBridgeCalcWing::WriteForceAreaC(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double h1 = m_xyBaseTop.y-m_xyB.y;
	double h2 = m_xyBaseTop.y-(m_xyB.y+m_xyC.y)/2;
	double h3 = m_xyBaseTop.y-m_xyC.y;
	double dL = fabs(m_xyA.x-m_xyE.x);
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		h1	= toM(h1);
		h2	= toM(h2);
		h3	= toM(h3);
		dL	= toM(dL);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 중간부 토압
	double dq3 = m_dK0 * (m_dWeightBackFill*(h3+m_dHs));			// 하단부 토압
	
	double P1 = (m_dFactorToap[WING_FACTOR_ULT]*dq1 + m_dFactorLive[WING_FACTOR_ULT]*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*dq[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	double P2 = (m_dFactorToap[WING_FACTOR_ULT]*dq2 + m_dFactorLive[WING_FACTOR_ULT]*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*dq[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	double P3 = (m_dFactorToap[WING_FACTOR_ULT]*dq3 + m_dFactorLive[WING_FACTOR_ULT]*dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*dq[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	
	double Sc = (P1+P3)/2 * dL;
	double Mc = (1/6.) * (2*P1 + P3) * pow(dL, 2);
	double Sc_ = (P2+P3)/2 * dL/2;
	double Mc_ = (1/6.) * (2*P2 + P3) * pow(dL/2, 2);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+15$e$h%s$r단면 C$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));
	
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$c*$r$ec-c' 및 c'-d의 두구간으로 분할하여 불리한 단면력으로 설계$n");
	WriteCalcLoad_qq(XLText);

	if(m_dHs > 0)
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x (h3+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq4$r$c=$r$eKo x γt x (h4+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq5$r$c=$r$eKo x γt x (h5+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h3+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq3, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	else 
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x h3$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq4$r$c=$r$eKo x γt x h4$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		XLText->AddFormatText("$h$eq5$r$c=$r$eKo x γt x h5$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h3, UNIT_CGS_M), pData->GetStrDouble(dq3, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	*XLText += 1;
	
	CString strqq(_T(""));
	if(pBri->m_nSelectWingLoad_LS_ES == 2)
	{
		strqq.Format(_T("%.3f x %s + %.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_ES], UNIT_CGS_TONF_M2), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_LS], UNIT_CGS_TONF_M2));
	}
	else
	{
		long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
		double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
		strqq.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(dq[nTypeLive], UNIT_CGS_TONF_M2));
	}
	XLText->AddFormatText("$h$eP1$r$c=$r$e%s%.3f x %s + %s%s%s$n", 
		STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), 
		strqq, STR_BRACKET_END, szValueEta);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);

	XLText->AddFormatText("$h$eP2$r$c=$r$e%s%.3f x %s + %s%s%s$n", 
		STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), 
		strqq, STR_BRACKET_END, szValueEta);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	
	XLText->AddFormatText("$h$eP3$r$c=$r$e%s%.3f x %s + %s%s%s$n", 
		STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq3, UNIT_CGS_TONF_M2), 
		strqq, STR_BRACKET_END, szValueEta);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P3, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	*XLText += 1;

	XLText->AddFormatText("$s+10$h$eSc$r$c=$r$e(P1+P3)/2 x L$m+05$c=$r$e(%s+%s)/2 x %s$n", 
		pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(P3, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sc, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$eMc$r$c=$r$e(1/6) x (2P1+P3) x L²$m+05$c=$r$e(1/6) x (2 x %s+%s) x %s²$n", 
		pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(P3, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$c=$r$e%s %s/m$n", pData->GetStrDouble(Mc,UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
	XLText->AddFormatText("$h$eSc'$r$c=$r$e(P2+P3)/2 x L$m+05$c=$r$e(%s+%s)/2 x %s$n", 
		pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(P3, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sc_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$eMc'$r$c=$r$e(1/6) x (2P2+P3) x L²$m+05$c=$r$e(1/6) x (2 x %s+%s) x %s²$n", 
		pData->GetStrDouble(P2, UNIT_CGS_TONF_M2), pData->GetStrDouble(P3, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL/2, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$c=$r$e%s %s/%s$n", pData->GetStrDouble(Mc_, UNIT_CGS_TONFM), pData->m_szTonfM, m_bLsd? _T("mm") : _T("m"));
	*XLText += 1;

	XLText->AddFormatText("$h$eVu$r$c=$r$e%s %s$n", pData->GetStrDouble(m_Vu[AREA_C], UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$eMu$r$c=$r$e%s %s/m$n", pData->GetStrDouble(m_Mu[AREA_C], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

void CARcBridgeCalcWing::WriteForceAreaD(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dL = fabs(m_xyE_.x - m_xyE.x);
	double h1 = m_dFH;	// m_pWing->m_dHL;
	double sw = m_pWing->m_dSW;
	double h2 = h1+dL/sw;
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		h1	= toM(h1);
		h2	= toM(h2);
		dL	= toM(dL);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}
	
	// 상재 하중에 의한 단면력
	double Sl[LIVE_LOAD_COUNT], Ml[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		Sl[nLive] = Ml[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			double dLoadAlways = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);
			Sl[nLive] = m_dK0 * dLoadAlways * (h1*dL + pow(dL, 2)/(2*sw));
			Ml[nLive] = m_dK0 * dLoadAlways * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
		}
	}
	
	// 토압에 의한 단면력
	double Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*dL + h1*pow(dL, 2)/sw + pow(dL, 3)/(3*pow(sw, 2)));
	double Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL, 2))/2. + (h1*pow(dL, 3))/(3*sw) + pow(dL, 4)/(12*pow(sw, 2)));
	
	// 토피에 의한 단면력
	double Sh_ = 0;
	double Mh_ = 0;
	if(m_dHs > 0 && pBri->m_bUnderGroundRahmen)
	{
		Sh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * (h1*dL + pow(dL, 2)/(2*sw));
		Mh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));
	}

	// 극한 단면력
	double Sd = (m_dFactorToap[WING_FACTOR_ULT]*(Sh+Sh_) + m_dFactorLive[WING_FACTOR_ULT]*Sl[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*Sl[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	double Md = (m_dFactorToap[WING_FACTOR_ULT]*(Mh+Mh_) + m_dFactorLive[WING_FACTOR_ULT]*Ml[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*Ml[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$e$h%s$r단면 D$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));

	// 상재 하중에 의한 단면력
	for(long nLive = LIVE_LOAD_ES; nLive <= LIVE_LOAD_LS; nLive++)
	{
		if(IsLiveLoad(nLive == LIVE_LOAD_ES) == FALSE) continue;
		double dAlwaysLoad = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);

		CString sLoadMark = _T("");
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			sLoadMark = (nLive == LIVE_LOAD_ES) ? _T("(ES)") : _T("(LS)");
		}

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$c-$r$e상재하중에 의한 단면력%s$n", sLoadMark);
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddFormatText("$h$e위치에 따른 날개벽 높이$n");
		if(m_pWing->m_bSWHor)
		{
			XLText->AddFormatText("$h$ih(x)$r$c=$r$eh1$n", sw);
		}
		else
		{
			XLText->AddFormatText("$h$ih(x)$r$c=$r$eh1 + x/n$m+05n$r$e:$r$e날개벽 경사$m+03( 1:%.1f )$n", sw);
		}
		XLText->AddFormatText("$h$ip(x)$r$c=$r$eKo x q x h(x)$m+05q$r$e:$r$e상재하중$m+03( %s %s )$n", 
			pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2, FALSE, 1), pData->m_szTonf_M2);
		*XLText += 1;

		if(m_pWing->m_bSWHor)
		{
			XLText->AddFormatText("$h$e작용 단면력$n");
			XLText->AddFormatText("$h$cSl$r$c=$r$e∫p(x)dx = Ko x q x ∫(h1)dx = Ko x q x (h1xL)$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x (%s x %s)$n", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sl[nLive], UNIT_CGS_TONF), pData->m_szTonf);

			XLText->AddFormatText("$h$cMl$r$c=$r$e∫(L-x)p(x)dx = Ko x q x ∫(L-x)(h1)dx$n");
			XLText->AddFormatText("$h$r$c=$r$eKo x q x ((h1xL²)/2)$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x ((%s x %s²)/2)$n", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Ml[nLive], UNIT_CGS_TONFM), pData->m_szTonfM);
		}
		else
		{
			XLText->AddFormatText("$h$e작용 단면력$n");
			XLText->AddFormatText("$h$cSl$r$c=$r$e∫p(x)dx = Ko x q x ∫(h1+x/n)dx = Ko x q x (h1xL + L²/(2n))$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x (%s x %s + %s²/(2 x %.3f))$n", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sl[nLive], UNIT_CGS_TONF), pData->m_szTonf);

			XLText->AddFormatText("$h$cMl$r$c=$r$e∫(L-x)p(x)dx = Ko x q x ∫(L-x)(h1+x/n)dx$n");
			XLText->AddFormatText("$h$r$c=$r$eKo x q x ((h1xL²)/2 + L³/(6n))$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x ((%s x %s²)/2 + %s³/(6 x %.3f))$n", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Ml[nLive], UNIT_CGS_TONFM), pData->m_szTonfM);
		}
		*XLText += 1;
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$h$c-$r$e토압 의한 단면력$n");
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$e위치에 따른 작용토압$n");
	XLText->AddFormatText("$h$ip(x)$r$c=$r$e½ x Ko x r x h(x)²$n");
	*XLText += 1;
	XLText->AddFormatText("$h$e작용 단면력$n");
	if(m_pWing->m_bSWHor)
	{
		XLText->AddFormatText("$h$cSh$r$c=$r$e∫p(x)dx = ½ x Ko x r x ∫h(x)²dx = ½ x Ko x r x ∫(h1)²dx$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x (h1²x L)$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x (%s²x %s)$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh, UNIT_CGS_TONF), pData->m_szTonf);
		*XLText += 1;

		XLText->AddFormatText("$h$cMh$r$c=$r$e∫(L-x)p(x)dx = ½ x Ko x r x ∫(L-x)(h1)dx$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x (h1²x L²)/2$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x (%s²x %s²)/2$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n",  pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	else
	{
		XLText->AddFormatText("$h$cSh$r$c=$r$e∫p(x)dx = ½ x Ko x r x ∫h(x)²dx = ½ x Ko x r x ∫(h1 + x/n)²dx$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x (h1²x L + (h1xL²)/n + L³/(3n²))$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x (%s²x %s + (%sx%s²)/n$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M),
			pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
		XLText->AddFormatText("$h$r$r$e+ %s³/(3 x %.1f²))$n", pData->GetStrDouble(dL, UNIT_CGS_M), sw);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh, UNIT_CGS_TONF), pData->m_szTonf);
		*XLText += 1;

		XLText->AddFormatText("$h$cMh$r$c=$r$e∫(L-x)p(x)dx = ½ x Ko x r x ∫(L-x)(h1+x/n)dx$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x ((h1²x L²)/2 + (h1 x L³)/(3n) + L⁴/(12n²))$n");
		XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x ((%s²x %s²)/2$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$r$e+ (%s x %s³)/(3 x %.1f) + %s⁴/(12x %.1f²))$n", 
			pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw, pData->GetStrDouble(dL, UNIT_CGS_M), sw);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n",  pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->m_szTonfM);
	}

	if(m_dHs > 0 && pBri->m_bUnderGroundRahmen)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+10$h$c-$r$e토피에 의한 단면력$n");
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddFormatText("$h$ip(x)$r$c=$r$eKo x r x ht x h(x)$n");
		*XLText += 1;
		XLText->AddFormatText("$h$e작용 단면력$n");

		if(m_pWing->m_bSWHor)
		{
			XLText->AddFormatText("$h$cSh'$r$c=$r$e∫p(x)dx = Ko x r x ht x ∫(h1)dx = Ko x r x ht x (h1xL)$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x (%s x %s)$n", 
				m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh_, UNIT_CGS_TONF), pData->m_szTonf);
			XLText->AddFormatText("$h$cMh'$r$c=$r$e∫(L-x)p(x)dx = Ko x r x ht x ∫(L-x)(h1)dx$n");
			XLText->AddFormatText("$h$r$c=$r$eKo x r x ht x (h1xL²)/2$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x (%s x %s²)/2$n", 
				m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), pData->m_szTonfM);	
			*XLText += 1;
		}
		else
		{
			XLText->AddFormatText("$h$cSh'$r$c=$r$e∫p(x)dx = Ko x r x ht x ∫(h1+x/n)dx = Ko x r x ht x (h1xL + L²/(2n))$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x (%s x %s + %s²/(2 x %.3f))$n", 
				m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh_, UNIT_CGS_TONF), pData->m_szTonf);
			XLText->AddFormatText("$h$cMh'$r$c=$r$e∫(L-x)p(x)dx = Ko x r x ht x ∫(L-x)(h1+x/n)dx$n");
			XLText->AddFormatText("$h$r$c=$r$eKo x r x ht x ((h1xL²)/2 + L³/(6n))$n");
			XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x ((%s x %s²)/2 + %s³/(6 x %.3f))$n", 
				m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), pData->m_szTonfM);	
			*XLText += 1;
		}
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$h$c-$r$e극한 단면력$n");
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strSl(_T("")), strMl(_T("")), strSl1(_T("")), strMl1(_T(""));
	if(pBri->m_nSelectWingLoad_LS_ES == 2)
	{
		strSl.Format(_T("%.3f x %s + %.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Sl[LIVE_LOAD_ES], UNIT_CGS_TONF), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(Sl[LIVE_LOAD_LS], UNIT_CGS_TONF));
		strSl1.Format(_T("%.3f x SI + %.3f x SI'"), m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT]);
		strMl.Format(_T("%.3f x %s + %.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Ml[LIVE_LOAD_ES], UNIT_CGS_TONFM), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(Ml[LIVE_LOAD_LS], UNIT_CGS_TONFM));
		strMl1.Format(_T("%.3f x MI + %.3f x MI'"), m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT]);
	}
	else
	{
		long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
		double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
		strSl.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(Sl[nTypeLive], UNIT_CGS_TONF));
		strSl1.Format(_T("%.3f x SI"), dFactorLive);
		strMl.Format(_T("%.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Ml[nTypeLive], UNIT_CGS_TONFM));
		strMl1.Format(_T("%.3f x MI"), m_dFactorLive_LS[WING_FACTOR_ULT]);
	}

	if(m_dHs > 0 && pBri->m_bUnderGroundRahmen)
	{
		XLText->AddFormatText("$h$cSd$r$c=$r$e%s%.3f x (Sh + Sh') + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strSl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x (%s + %s) + %s%s%s$m+15$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Sh, UNIT_CGS_TONF), pData->GetStrDouble(Sh_, UNIT_CGS_TONF), strSl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->m_szTonf);
		XLText->AddFormatText("$h$cMd$r$c=$r$e%s%.3f x (Mh + Mh') + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strMl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x (%s + %s) + %s%s%s$m+15$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), strMl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->m_szTonf);
	}
	else
	{
		XLText->AddFormatText("$h$cSd$r$c=$r$e%s%.3f x Sh + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strSl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x %s + %s%s%s$m+15$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Sh, UNIT_CGS_TONF), strSl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->m_szTonf);
		XLText->AddFormatText("$h$cMd$r$c=$r$e%s%.3f x Mh + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strMl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x %s + %s%s%s$m+15$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Mh, UNIT_CGS_TONFM), strMl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	*XLText += 1;
	XLText->AddFormatText("$h$e e-f 단면에 단위폭으로 등분포 시키면$n");
	XLText->AddFormatText("$h$cVu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(m_Vu[AREA_D], UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$cMu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(m_Mu[AREA_D], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

/*
	BOX 타입이고 부상방지턱이 없을때 D구간 하단이 평평한모양 하중계산
*/
void CARcBridgeCalcWing::WriteForceAreaD_Flat( CXLFormatText *XLText, long nIndexSub )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dL1 = fabs(m_xyE_.x - m_xyF.x);
	double dL2 = fabs(m_xyF.x - m_xyE.x);
	double h1 = m_dFH;	// m_pWing->m_dHL;
	double sw = m_pWing->m_dSW;
	double h3 = dL1/sw;
	double h2 = h1+h3;
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		h1	= toM(h1);
		h2	= toM(h2);
		h3	= toM(h3);
		dL1	= toM(dL1);
		dL2	= toM(dL2);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}

	// 1 : 상단 사각형(h1 x (L1 + L2)), 2 : 경사부 삼각형(h2 x L1 / 2), 3 : 평면부 사각형(h2 x L2)
	// 상재 하중에 의한 단면력
	double Sl[LIVE_LOAD_COUNT], Ml[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		Sl[nLive] = Ml[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			double dLoadAlways = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);
			Sl[nLive] = m_dK0 * dLoadAlways * (h1*(dL1 + dL2) + dL1*h3/2. + h3*dL2);
			Ml[nLive] = m_dK0 * dLoadAlways * ((h1*pow(dL1+dL2, 2))/2 + (dL1*h3/2.) * (dL2 + dL1/3.) + h3*pow(dL2, 2)/2);
		}
	}
	
	// 토압에 의한 단면력
	double Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*(dL1 + dL2) + (h1*h3*dL1 + pow(h3, 2)/3.*dL1) + h1*2*h3*dL2);
	double Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL1 + dL2, 2))/2 + (h1*h3*dL1 + pow(h3, 2)/3.*dL1) * (dL2 + dL1/3.) + h1*h3*pow(dL2, 2));

	// 토피에 의한 단면력
	double Sh_ = 0;
	double Mh_ = 0;
	if(m_dHs > 0)
	{
		Sh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * (h1*(dL1 + dL2) + dL1*h3/2. + h3*dL2);
		Mh_ = m_dK0 * (m_dHs * m_dWeightBackFill) * ((h1*pow(dL1+dL2, 2))/2 + (dL1*h3/2.) * (dL2 + dL1/3.) + h3*pow(dL2, 2)/2);
	}

	// 극한 단면력
	double Sd = (m_dFactorToap[WING_FACTOR_ULT]*(Sh+Sh_) + m_dFactorLive[WING_FACTOR_ULT]*Sl[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*Sl[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];
	double Md = (m_dFactorToap[WING_FACTOR_ULT]*(Mh+Mh_) + m_dFactorLive[WING_FACTOR_ULT]*Ml[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT]*Ml[LIVE_LOAD_LS]) * m_dEta[WING_FACTOR_ULT];

	CString szL1 = pData->GetStrDouble(dL1, UNIT_CGS_M);
	CString szL2 = pData->GetStrDouble(dL2, UNIT_CGS_M);
	CString szH1 = pData->GetStrDouble(h1, UNIT_CGS_M);
	CString szH2 = pData->GetStrDouble(h3, UNIT_CGS_M);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$e$h%s$r단면 D$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));

	for(long nLive = LIVE_LOAD_ES; nLive <= LIVE_LOAD_LS; nLive++)
	{
		if(IsLiveLoad(nLive == LIVE_LOAD_ES) == FALSE) continue;
		double dAlwaysLoad = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);

		CString sLoadMark = _T("");
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			sLoadMark = (nLive == LIVE_LOAD_ES) ? _T("(ES)") : _T("(LS)");
		}

		XLText->AddFormatText("$h$c-$r$e상재하중에 의한 단면력%s$n", sLoadMark);
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddFormatText("$h$e위치에 따른 날개벽 높이$n");
		XLText->AddFormatText("$h$ih(x)$r$c=$r$eh1 + x/n$m+05n$r$e:$r$e날개벽 경사$m+03( 1:%.1f )$n", sw);
		XLText->AddFormatText("$h$ip(x)$r$c=$r$eKo x q x h(x)$m+05q$r$e:$r$e상재하중$m+03( %s %s )$n", 
			pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2, FALSE, 1), pData->m_szTonf_M2);
		*XLText += 1;
		XLText->AddFormatText("$h$e작용 단면력$n");
		XLText->AddFormatText("$h$cSl$r$c=$r$e∫p(x)dx = Ko x q x ∫(h1+x/n)dx = Ko x q x (h1 x (L1+L2) + L1 x h2 / 2 + h2 x L2)$n");
		XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x (%s x (%s + %s) + %s x %s / 2 + %s x %s)$n", 
			m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), szH1, szL1, szL2, szL1, szH2, szH2, szL2);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sl[nLive], UNIT_CGS_TONF), pData->m_szTonf);

		XLText->AddFormatText("$h$cMl$r$c=$r$e∫(L-x)p(x)dx = Ko x q x ∫(L-x)(h1+x/n)dx$n");
		XLText->AddFormatText("$h$r$c=$r$eKo x q x ((h1 x (L1 + L2)²)/2 + (L1 x h2 /2) x (L2 + L1/3) + h2 x L2²/2)$n");
		XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x ((%s x (%s + %s)²)/2$n", 
			m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), szH1, szL1, szL2);
		XLText->AddFormatText("$h$r$r$e + (%s x %s /2) x (%s + %s/3) + %s x %s²/2)$n",
			szL1, szH2, szL2, szL1, szH2, szL2);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Ml[nLive], UNIT_CGS_TONFM), pData->m_szTonfM);
		*XLText += 1;
	}
	
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$h$c-$r$e토압 의한 단면력$n");
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddFormatText("$h$e위치에 따른 작용토압$n");
	XLText->AddFormatText("$h$ip(x)$r$c=$r$e½ x Ko x r x h(x)²$n");
	*XLText += 1;
	XLText->AddFormatText("$h$e작용 단면력$n");
//	Sh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*(dL1 + dL2) + (h1*h3*dL1 + pow(h3, 2)/3.*L1) + h1*2*h3*dL2);
	XLText->AddFormatText("$h$cSh$r$c=$r$e∫p(x)dx = ½ x Ko x r x ∫h(x)²dx = ½ x Ko x r x ∫(h1 + x/n)²dx$n");
	XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x (h1²x (dL1 + dL2) + (h1 x h2 x L1 + h2²/3 x L1) + h1 x 2 x h2 x L2)$n");
	XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x (%s²x (%s + %s)$n", 
		m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), szH1, szL1, szL2);
	XLText->AddFormatText("$h$r$r$e + (%s x %s x %s + %s²/3 x %s) + %s x 2 x %s x %s)$n",
		szH1, szH2, szL1, szH2, szL1, szH1, szH2, szL2);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh, UNIT_CGS_TONF), pData->m_szTonf);
	*XLText += 1;
// Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL1 + dL2, 2))/2 + (h1*h3*dL1 + pow(h3, 2)/3.*L1) * (dL2 + dL1/3.) + h1*h3*pow(dL2, 2));
	XLText->AddFormatText("$h$cMh$r$c=$r$e∫(L-x)p(x)dx = ½ x Ko x r x ∫(L-x)(h1+x/n)dx$n");
	XLText->AddFormatText("$h$r$c=$r$e½ x Ko x r x ((h1²x (L1 + L2)²)/2$n");
	XLText->AddFormatText("$h$r$r$e + (h1 x h2 x L1 + h2²/3 x L1) x (L2 + L1/3) + h1 x h2 x L2²)$n");
	XLText->AddFormatText("$h$r$c=$r$e½ x %.3f x %s x ((%s²x (%s + %s)²) / 2$n", 
		m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), szH1, szL1, szL2);
	XLText->AddFormatText("$h$r$r$e+ (%s x %s x %s + %s²/3 x %s) x (%s + %s/3)$n", 
		szH1, szH2, szL1, szH2, szL1, szL2, szL1);
	XLText->AddFormatText("$h$r$r$e+ %s x %s x %s²)$n", 
		szH1, szH2, szL2);
	XLText->AddFormatText("$h$r$c=$r$e%s %s$n",  pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->m_szTonfM);

	if(m_dHs > 0 && pBri->m_bUnderGroundRahmen)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+10$h$c-$r$e토피에 의한 단면력$n");
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddFormatText("$h$ip(x)$r$c=$r$eKo x r x ht x h(x)$n");
		*XLText += 1;
		XLText->AddFormatText("$h$e작용 단면력$n");
		XLText->AddFormatText("$h$cSh'$r$c=$r$e∫p(x)dx = Ko x r x ht x ∫(h1+x/n)dx = Ko x r x ht x (h1 x (L1+L2) + L1 x h2 / 2 + h2 x L2)$n");
		XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x (%s x (%s + %s) + %s x %s / 2 + %s x %s)$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), szH1, szL1, szL2, szL1, szH2, szH2, szL2);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sh_, UNIT_CGS_TONF), pData->m_szTonf);
		XLText->AddFormatText("$h$cMh'$r$c=$r$e∫(L-x)p(x)dx = Ko x r x ht x ∫(L-x)(h1+x/n)dx$n");
		XLText->AddFormatText("$h$r$c=$r$eKo x r x ht x ((h1 x (L1 + L2)²)/2 + (L1 x h2 /2) x (L2 + L1/3) + h2 x L2²/2)$n");
		XLText->AddFormatText("$h$r$c=$r$e%.3f x %s x %s x ((%s x (%s + %s)²)/2$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(m_dHs, UNIT_CGS_M), szH1, szL1, szL2);
		XLText->AddFormatText("$h$r$r$e + (%s x %s /2) x (%s + %s/3) + %s x %s²/2)$n",
			szL1, szH2, szL2, szL1, szH2, szL2);
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), pData->m_szTonfM);	
		*XLText += 1;
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+16$h$c-$r$e극한 단면력$n");
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strSl(_T("")), strMl(_T("")), strSl1(_T("")), strMl1(_T(""));
	if(pBri->m_nSelectWingLoad_LS_ES == 2)
	{
		strSl.Format(_T("%.3f x %s + %.3f x %s"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Sl[LIVE_LOAD_ES], UNIT_CGS_TONF), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(Sl[LIVE_LOAD_LS], UNIT_CGS_TONF));
		strSl1.Format(_T("%.3f x SI + %.3f x SI'"), m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT]);
	}
	else
	{
		long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
		double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
		strSl.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(Sl[nTypeLive], UNIT_CGS_TONF));
		strSl1.Format(_T("%.3f x SI"), dFactorLive);
	}

	if(m_dHs > 0 && pBri->m_bUnderGroundRahmen)
	{
		XLText->AddFormatText("$h$cSd$r$c=$r$e%s%.3f x (Sh + Sh') + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strSl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x (%s + %s) + %s%s%s$m+12$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Sh, UNIT_CGS_TONF), pData->GetStrDouble(Sh_, UNIT_CGS_TONF), strSl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->m_szTonf);
		XLText->AddFormatText("$h$cMd$r$c=$r$e%s%.3f x (Mh + Mh') + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strMl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x (%s + %s) + %s%s%s$m+12$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), strMl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->m_szTonf);
	}
	else
	{
		XLText->AddFormatText("$h$cSd$r$c=$r$e%s%.3f x Sh + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strSl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x %s + %s%s%s$m+12$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Sh, UNIT_CGS_TONF), strSl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->m_szTonf);
		XLText->AddFormatText("$h$cMd$r$c=$r$e%s%.3f x Mh + %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], strMl1, STR_BRACKET_END, STR_SIMBOL_ETA);
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x %s + %s%s%s$m+12$c=$r$e%s %s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(Mh, UNIT_CGS_TONFM), strMl, STR_BRACKET_END, szValueEta, 
			pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	*XLText += 1;
	XLText->AddFormatText("$h$e e-f 단면에 단위폭으로 등분포 시키면$n");
	XLText->AddFormatText("$h$cVu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Sd, UNIT_CGS_TONF), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(m_Vu[AREA_D], UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$cMu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(m_Mu[AREA_D], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

void CARcBridgeCalcWing::WriteForceAreaA_Add(CXLFormatText *XLText, long nIndexSub)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double h1 = m_xyBaseTop.y-m_xyA_.y;			// 상단 H
	double h2 = m_xyBaseTop.y-m_xyB.y;			// 하단 H
	double hl = m_xyA_.y-m_xyB.y;
	double dL = fabs(m_xyA.x-m_xyE.x);
	BOOL bCutAreaD1 = (m_xyA_.y > m_xyF.y)? TRUE: FALSE;
	BOOL bCutAreaD2 = (m_xyB.y > m_xyF.y)? TRUE: FALSE;
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		h1	= toM(h1);
		h2	= toM(h2);
		hl	= toM(hl);
		dL	= toM(dL);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}
	
	double dq[LIVE_LOAD_COUNT];
	for(long nLive = LIVE_LOAD_ES; nLive < LIVE_LOAD_COUNT; nLive++)
	{
		dq[nLive] = 0;
		if(IsLiveLoad(nLive == LIVE_LOAD_ES))
		{
			dq[nLive] = m_dK0 * GetLiveLoadAlways(nLive == LIVE_LOAD_ES);					// 상재하중
		}
	}
	double dq1 = m_dK0 * (m_dWeightBackFill*(h1+m_dHs));			// 상단 토압
	double dq2 = m_dK0 * (m_dWeightBackFill*(h2+m_dHs));			// 하단 토압
	
	double P1 = (m_dFactorToap[WING_FACTOR_ULT] * (1/2. * (dq1 + dq2) * hl) + (m_dFactorLive[WING_FACTOR_ULT] * dq[LIVE_LOAD_ES] + m_dFactorLive_LS[WING_FACTOR_ULT] * dq[LIVE_LOAD_LS]) * hl) * m_dEta[WING_FACTOR_ULT];
	
	double Sa_ = P1 * dL;
	double Ma_ = (1/2.) * P1 * pow(dL, 2);
	
	double Sd_ = 0;
	double Md_ = 0;
	if(bCutAreaD1)
	{
		double dHd_ = m_bLsd? (m_xyA_.y - m_xyB.y) : toM(m_xyA_.y - m_xyB.y);
		Sd_ = GetValueAreaDCut(h1, 0)/dHd_;
		Md_ = GetValueAreaDCut(h1, 1)/dHd_;
		if(bCutAreaD2)
		{
			Sd_ = (Sd_*dHd_ - GetValueAreaDCut(h2, 0))/dHd_;
			Md_ = (Md_*dHd_ - GetValueAreaDCut(h2, 1))/dHd_;
		}
		
		Sa_ += Sd_;
		Ma_ += (Md_+Sd_*dL);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+12$e$h%s$r단면 A'$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	WriteCalcLoad_qq(XLText);

	if(m_dHs > 0) 
	{
		XLText->AddFormatText("$h$eq2$r$c=$r$eKo x γt x (h2+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$eq2$r$c=$r$eKo x γt x h2$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2); 
	}

	if(m_dHs > 0) 
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x (h3+ht)$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2+m_dHs, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$eq3$r$c=$r$eKo x γt x h3$m+05$c=$r$e%.3f x %s x %s$m+06$c=$r$e%s %s$n", 
			m_dK0, pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(h2, UNIT_CGS_M), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->m_szTonf_M2); 
	}
	*XLText += 1;

	if(pBri->m_nSelectWingLoad_LS_ES == 2)
	{
		XLText->AddFormatText("$h$eP1$r$c=$r$e%s%.3f x ½ x (q2 + q3) x (h3-h2) + (%.3f x q0 + %.3f x q0') x (h3-h2)%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_dFactorLive[WING_FACTOR_ULT], m_dFactorLive_LS[WING_FACTOR_ULT], STR_BRACKET_END, STR_SIMBOL_ETA);	
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x ½ x (%s + %s) x %s + (%.3f x %s + %.3f x %s) x %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->GetStrDouble(hl, UNIT_CGS_M), 
			m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_ES], UNIT_CGS_TONF_M2), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(dq[LIVE_LOAD_LS], UNIT_CGS_TONF_M2), 
			pData->GetStrDouble(hl, UNIT_CGS_M), STR_BRACKET_END, STR_SIMBOL_ETA);	
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);	
	}
	else
	{
		long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
		double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];

		XLText->AddFormatText("$h$eP1$r$c=$r$e%s%.3f x ½ x (q2 + q3) x (h3-h2) + %.3f x q0 x (h3-h2)%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], m_dFactorLive[WING_FACTOR_ULT], STR_BRACKET_END, STR_SIMBOL_ETA);	
		XLText->AddFormatText("$h$r$c=$r$e%s%.3f x ½ x (%s + %s) x %s + %.3f x %s x %s%s%s$n", 
			STR_BRACKET_STT, m_dFactorToap[WING_FACTOR_ULT], pData->GetStrDouble(dq1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dq2, UNIT_CGS_TONF_M2), pData->GetStrDouble(hl, UNIT_CGS_M), 
			dFactorLive, pData->GetStrDouble(dq[nTypeLive], UNIT_CGS_TONF_M2), pData->GetStrDouble(hl, UNIT_CGS_M), STR_BRACKET_END, STR_SIMBOL_ETA);	
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);	
	}
	*XLText += 1;

	if(m_bIsD && bCutAreaD1)
	{
		XLText->AddFormatText("$s+09$h* D구간 추가 부분 산정$n$h");
		WriteCalcLoad_Dminus(XLText, 3, h1, FALSE);
		
		if(bCutAreaD2)
		{
			XLText->AddFormatText("$h* D구간 추가 부분 산정시 공제 부분$n$h");
			WriteCalcLoad_Dminus(XLText, 3, h2, FALSE);
		}
	}

	if(bCutAreaD1)
	{
		XLText->AddFormatText("$h$eSa'$r$c=$r$eP1 x L + Sd'$m+07$c=$r$e%s x %s + %s$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n",  pData->GetStrDouble(Sa_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMa'$r$c=$r$e½ x P1 x L²+ (Md' + Sd'xL)$m+08$c=$r$e½ x %s x %s²+(%s + %s x %s)$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(Md_, UNIT_CGS_TONFM), pData->GetStrDouble(Sd_, UNIT_CGS_TONF_M), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/%s$n", pData->GetStrDouble(Ma_, UNIT_CGS_TONFM), pData->m_szTonfM, m_bLsd? _T("mm") : _T("m"));
	}
	else
	{
		XLText->AddFormatText("$h$eSa'$r$c=$r$eP1 x L$m+05$c=$r$e%s x %s$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s$n", pData->GetStrDouble(Sa_, UNIT_CGS_TONF_M), pData->m_szTonf_M);
		XLText->AddFormatText("$h$eMa'$r$c=$r$e½ x P1 x L²$m+05$c=$r$e½ x %s x %s²$n", 
			pData->GetStrDouble(P1, UNIT_CGS_TONF_M2), pData->GetStrDouble(dL, UNIT_CGS_M));
		XLText->AddFormatText("$h$r$c=$r$e%s %s/%s$n", pData->GetStrDouble(Ma_, UNIT_CGS_TONFM), pData->m_szTonfM, m_bLsd? _T("mm") : _T("m"));
	}
	
	*XLText += 1;

	XLText->AddFormatText("$h$e a'-b 단면에 단위폭으로 등분포 시키면$n");
	XLText->AddFormatText("$h$cVu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Sa_, UNIT_CGS_TONF_M), pData->GetStrDouble(hl, UNIT_CGS_M), pData->GetStrDouble(m_Vu[AREA_A_], UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$cMu$r$c=$r$e%s / %s$m+06$c=$r$e%s %s$n", 
		pData->GetStrDouble(Ma_, UNIT_CGS_TONFM), pData->GetStrDouble(hl, UNIT_CGS_M), pData->GetStrDouble(m_Mu[AREA_A_], UNIT_CGS_TONFM), pData->m_szTonfM);
	*XLText += 1;
}

void CARcBridgeCalcWing::WriteForceSummary(CXLFormatText *XLText, long nIndexSub, BOOL bCalc3D, BOOL bMax)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nPos = bMax ? COL_POS_1 : COL_POS_2;

	XLText->GoPosition(NO_MOVE, nPos);
	if(bMax)
		XLText->AddFormatText("$e$h %d)$r설계 단면력 집계$n", nIndexSub);
	else
		XLText->AddFormatText("$s+12$e$h%s$r단면력 집계$n$h", GetNumberString(nIndexSub,NUM_CIRCLE));

	XLText->GoPosition(NO_MOVE, nPos);
	CString str = _T("");
	long i=0; for(i=0; i<WING_CASE_COUNT; i++)
	{
		if(!m_bLsd && i > WING_FACTOR_USE)
			break;

		BOOL bUse = (i==WING_FACTOR_USE)? TRUE : FALSE;
		BOOL bJudge = (i==WING_FACTOR_USE_JUDGE)? TRUE : FALSE;
		*XLText += 1;
		if(m_bLsd)
		{
			str = GetStringTypeCase(i) + hggettext(" 단면력 집계");
		}
		else
		{
			str = bUse ? hggettext("사용하중 단면력") : hggettext("계수하중 단면력");
		}

		XLText->AddFormatText("$h$c-$r$e%s$n$h", str);

		long nColCnt = (m_bIsA)? 4 : 1;
		long nRowCnt = 3;

		if(m_bIsA_ && m_bIsA_Calc) nColCnt++;
		if(m_bIsD) nColCnt++;
	
		int nWidthJoinTitle = (nColCnt == 4 || !m_bIsA)? 6:4;
		
		int nWidthJoinItem = 0;
		switch(nColCnt) {
		case 4: nWidthJoinItem = 6; break;
		case 5: nWidthJoinItem = 5; break;
		case 6:	nWidthJoinItem = 4; break;
		default:
			nWidthJoinItem = 6;
		}

		XLText->AddTable(nRowCnt-1, (nWidthJoinTitle + nWidthJoinItem * (nColCnt-1))-1, 1, 1);

		char *Title1[] = { "구 분", "단면 D", "단면 A", "단면 A'", "단면 B", "단면 C" };
		long nCntMerge = 0;
		long nWidthJoin = 0;
		long j=0; for(j=0; j<6; j++)
		{
			nWidthJoin = (j==0)? nWidthJoinTitle:nWidthJoinItem;
			
			if(j == 1 && !m_bIsD) continue;
			else if(j > 1 && !m_bIsA) continue;
			else if(j == 2 && !m_bIsA_Calc) continue;
			else if(j == 3 && !m_bIsA_) continue;
			
			XLText->AddMergeFormatText(nCntMerge, nWidthJoin-1,"$c%s",Title1[j]);
			XLText->AddFormatText("$m+%02d", nWidthJoin);
		}
		XLText->AddText("$n$h");

		double dValue = 0;
		long k=0; for(k=0; k<2; k++)
		{
			for(j=0; j<6; j++)
			{
				nWidthJoin = (j==0)? nWidthJoinTitle:nWidthJoinItem;

				long nIndex = 0;
				switch(j) {
				case 1: nIndex = AREA_D; break;
				case 2: nIndex = AREA_A; break;
				case 3: nIndex = AREA_A_; break;
				case 4: nIndex = AREA_B; break;
				case 5: nIndex = AREA_C; break;
				default: nIndex = 0; break;
				}

				if(j==0)
				{
					if(k==0) str.Format("모멘트(%s)", pData->m_szTonfM);
					else str.Format("전단력(%s)", pData->m_szTonf);
				}
				else if(j==1 && !m_bIsD) continue;
				else if(j > 1 && !m_bIsA) continue;
				else if(j == 2 && !m_bIsA_Calc) continue;
				else if(j==3 && !m_bIsA_) continue;
				else if(j>0)
				{
					if(bCalc3D == FALSE && bMax == FALSE)
					{
						if(i==WING_FACTOR_USE) 
							dValue = (k==0) ? m_Mu_Use[nIndex] : m_Vu_Use[nIndex];
						else if(i==WING_FACTOR_ULT)
							dValue = (k==0) ? m_Mu[nIndex] : m_Vu[nIndex];
						else if(i==WING_FACTOR_USE_JUDGE) 
							dValue = (k==0) ? m_Mu_Use_Judge[nIndex] : m_Vu_Use_Judge[nIndex];
					}
					else if(bCalc3D && bMax == FALSE)
					{
						if(i==WING_FACTOR_USE) 
							dValue = (k==0) ? m_pWing->m_dMomentUsePlate[nIndex] : m_pWing->m_dShearUsePlate[nIndex];
						else if(i==WING_FACTOR_ULT)
							dValue = (k==0) ? m_pWing->m_dMomentUltPlate[nIndex] : m_pWing->m_dShearUltPlate[nIndex];
						else if(i==WING_FACTOR_USE_JUDGE) 
							dValue = (k==0) ? m_pWing->m_dMomentUseJudge_Plate[nIndex] : 0;
					}
					else // 적용단면력
					{
						if(i==WING_FACTOR_USE) 
							dValue = (k==0) ? MAX(m_pWing->m_dMomentUsePlate[nIndex],m_Mu_Use[nIndex]) : MAX(m_pWing->m_dShearUsePlate[nIndex],m_Vu_Use[nIndex]);
						else if(i==WING_FACTOR_ULT)
							dValue = (k==0) ? MAX(m_pWing->m_dMomentUltPlate[nIndex],m_Mu[nIndex]) : MAX(m_pWing->m_dShearUltPlate[nIndex],m_Vu[nIndex]);
						else if(i==WING_FACTOR_USE_JUDGE) 
							dValue = (k==0) ? MAX(m_pWing->m_dMomentUseJudge_Plate[nIndex],m_Mu_Use_Judge[nIndex]) : 0;
					}


					str = pData->GetStrDouble(dValue, (k==0)? UNIT_CGS_TONFM : UNIT_CGS_TONF_M);
					if(i==WING_FACTOR_USE_JUDGE && k==1)
						str = _T("-");
				}
				
				XLText->AddMergeFormatText(nCntMerge, nWidthJoin-1,"$c%s", str);
				XLText->AddFormatText("$m+%02d", nWidthJoin);
			}
			XLText->AddText("$n$h");
		}
	}
}

void CARcBridgeCalcWing::SetConcData()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nTypeCalc = pBri->m_nSelectWingCalc;// 관용, 혼합, 3D
	BOOL bManual = nTypeCalc != 2 ? TRUE : FALSE;
	BOOL bCalc3D = nTypeCalc != 0 ? TRUE : FALSE;

	double dMoment = 0;
	double dShear = 0;
	double dMomentUse = 0;
	double dMomentUse_Judge = 0;

	for(long nA = AREA_A; nA <= AREA_A_; nA++)
	{
		CConcBasic *pConc = pBri->GetConcDataWingWall(m_bStt, m_bLeft, nA);
		if(pConc)
		{
			if(bManual && bCalc3D)
			{
				dMoment = MAX(m_Mu[nA], m_pWing->m_dMomentUltPlate[nA]);
				dShear = MAX(m_Vu[nA], m_pWing->m_dShearUltPlate[nA]);
				dMomentUse = MAX(m_Mu_Use[nA], m_pWing->m_dMomentUsePlate[nA]);
				dMomentUse_Judge = MAX(m_Mu_Use_Judge[nA], m_pWing->m_dMomentUseJudge_Plate[nA]);
			}
			else if(bManual)
			{
				dMoment = m_Mu[nA];
				dShear = m_Vu[nA];
				dMomentUse = m_Mu_Use[nA];
				dMomentUse_Judge = m_Mu_Use_Judge[nA];
			}
			else
			{
				dMoment = m_pWing->m_dMomentUltPlate[nA];
				dShear = m_pWing->m_dShearUltPlate[nA];
				dMomentUse = m_pWing->m_dMomentUsePlate[nA];
				dMomentUse_Judge = m_pWing->m_dMomentUseJudge_Plate[nA];
			}

			if(m_bLsd)
			{
				// 한계상태에서 부재력은 mm 단위로 집계되지만 계산은 단위폭(1m)로 계산되므로 부재력에 x1000을 해준다.
				dMoment		= dMoment * 1000;
				dShear		= dShear * 1000;
				dMomentUse	= dMomentUse * 1000;
				dMomentUse_Judge	= dMomentUse_Judge * 1000;
			}

			// 값이 CGS단위가 아닌 것은 변환을 하여 저장하여 줌.....
			pConc->m_Moment		= GetValueUnitChange(dMoment, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_Shear		= GetValueUnitChange(dShear, pData->m_szTonf, UNIT_CGS_TONF);
			pConc->m_MomentUse	= GetValueUnitChange(dMomentUse, pData->m_szTonfM, UNIT_CGS_TONFM);
			pConc->m_MomentCrack	= GetValueUnitChange(dMomentUse_Judge, pData->m_szTonfM, UNIT_CGS_TONFM);
		}
	}
}

void CARcBridgeCalcWing::WriteCheckSection(CXLFormatText *XLText, long nIndexSmall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	long nIndexSub =1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h %d)$r단면검토$n$n", nIndexSmall++);
	CConcBasic *pConc;

	CString szStatus = m_bLsd ? _T(" : 극한한계상태") : _T("");

	long nArrArea[5] = { AREA_D, AREA_A, AREA_B, AREA_C, AREA_A_ };
	for(long ix=0; ix<5; ++ix)
	{
		long nArea = nArrArea[ix];
		if(nArea == AREA_D && !m_bIsD) continue;
		else if(nArea == AREA_A_ && !m_bIsA_) continue;
		else if(nArea < AREA_D && !m_bIsA) continue;
		else if(nArea == AREA_A && !m_bIsA_Calc) continue;

		pConc = pBri->GetConcDataWingWall(m_bStt, m_bLeft, nArea);
		if(pConc && pConc->m_Moment > 0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+40$h$r$e%s$r%s%s$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle, szStatus);

			if(m_bLsd)
			{
				if(pConc->m_strDesignDegree == _T("A"))
				{
					XLText->AddFormatText(_T("$h$r설계등급 A는 비균열 상태 및 균열폭 한계상태 검토대상이 아니므로 균열 검토를 진행하지 않습니다.$n$h"));
					continue;
				}

				long nMatType = pBri->GetLsdMatrialTypePos(m_bStt? ePartWingWallStt : ePartWingWallEnd, FALSE);

				long nIndexStatus = 0;
				CLsdBeam Beam(pConc, LSD_ULS_DESIGN, nIndexStatus);
				Beam.SetMatrialTypePos(nMatType);
				Beam.MakeOutputDesignExcelFile(XLText, 2, TRUE, FALSE, FALSE);
			}
			else
			{
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				pBeam.MakeOutputDesignExcelFile(XLText, 2, TRUE,FALSE);
			}
		}
	}
	
	nIndexSub =1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h %d)$r사용성 검토$n$n", nIndexSmall++);

	szStatus = m_bLsd ? _T(" : 사용한계상태") : _T("");

	for(long ix=0; ix<5; ++ix)
	{
		long nArea = nArrArea[ix];
		if(nArea == AREA_D && !m_bIsD) continue;
		else if(nArea == AREA_A_ && !m_bIsA_) continue;
		else if(nArea < AREA_D && !m_bIsA) continue;
		else if(nArea == AREA_A && !m_bIsA_Calc) continue;

		pConc = pBri->GetConcDataWingWall(m_bStt, m_bLeft, nArea);
		if(pConc && pConc->m_MomentUse > 0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+22$h$r$e%s$r%s%s$n$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle, szStatus);

			if(m_bLsd)
			{
				if(pConc->m_strDesignDegree == _T("A"))
				{
					XLText->AddFormatText(_T("$h$r설계등급 A는 비균열 상태 및 균열폭 한계상태 검토대상이 아니므로 균열 검토를 진행하지 않습니다.$n$h"));
					continue;
				}

				CString sDesign= _T(""), sKind=_T(""), sExtra = _T("");
				double dCrackWitdh = pBri->GetCrackLimitWidth_LSD(m_bStt? ePartWingWallStt : ePartWingWallEnd, FALSE, sKind, sDesign);

				BOOL bUseWingCrack = pBri->IsUseSelectLoadCaseWingOrApsMatchDesignRating(pConc->m_strDesignDegree , m_bStt? ePartWingWallStt : ePartWingWallEnd);
				if(pBri->IsCheckPosEtaUseLoad(m_bStt? ePartWingWallStt : ePartWingWallEnd) == FALSE || bUseWingCrack == FALSE)
				{
					XLText->AddFormatText(_T("$h ※ 노출 환경에 따른 적용 설계 등급 : %s %s$n$h"), sDesign, sExtra);
					XLText->AddFormatText(_T("$h$r사용하중 조합에서 한계상태 검증을 위한 하중조합을 미검토하였으므로 $n사용성 검토는 진행하지 않는다.$n$n$h"));
					continue;
				}

				if(sDesign != _T("A"))
					XLText->AddFormatText(_T("$h$r설계등급 %s 이므로 균열폭 한계상태는 %s, 표면 한계균열폭은 %.1fmm를 적용.$n$h"), sDesign,sKind,dCrackWitdh );	

				long nMatType = pBri->GetLsdMatrialTypePos(m_bStt? ePartWingWallStt : ePartWingWallEnd, FALSE);

				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.SetMatrialTypePos(nMatType);
				Beam.MakeOutputUsabilityExcelFile(XLText, 2, FALSE);
				XLText->AddText(_T("$n"));
			}
			else
			{
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				pBeam.MakeOutputCrackExcelFile(XLText, 2, FALSE);
			}
		}
	}

	nIndexSub =1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$e$h %d)$r수평철근 검토$n$n", nIndexSmall++);

	szStatus = m_bLsd ? _T(" : 극한한계상태") : _T("");

	for(long ix=0; ix<5; ++ix)
	{
		long nArea = nArrArea[ix];
		if(nArea == AREA_D && !m_bIsD) continue;
		else if(nArea == AREA_A_ && !m_bIsA_) continue;
		else if(nArea < AREA_D && !m_bIsA) continue;
		else if(nArea == AREA_A && !m_bIsA_Calc) continue;

		pConc = pBri->GetConcDataWingWall(m_bStt, m_bLeft, nArea);
		if(pConc)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+22$h$r$e%s$r%s%s$n$n", GetNumberString(nIndexSub++,NUM_CIRCLE), pConc->m_sTitle, szStatus);

			if(m_bLsd)
			{
				long nIndexPrint = 0;
				CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
				Beam.CalculateDryingShrinkage();
				Beam.MakeOutputDryingShrinkageExcelFile(XLText, 2, nIndexPrint);
				XLText->AddText(_T("$n"));
			}
			else
			{
				WriteSupportCheckWingWall(XLText, pConc, 2);
			}
		}
	}
}

void CARcBridgeCalcWing::WriteSupportCheckWingWall(CXLFormatText *XLText, CConcBasic *pConc, long nColStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	XLText->GoPosition(NO_MOVE, nColStt);

//	double dRatio = 0;
	double dAreaReq = 0;
	//double dAreaReqSect = 0;
	double dAreaUseC = 0;
	double dAreaUseT = 0;
	double dAreaUseTot = 0;
//	double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");
	CString szTitle = _T("");

	dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConc, FALSE, TRUE, !bSI);
	dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConc, TRUE, FALSE, !bSI);
	dAreaUseTot = dAreaUseC + dAreaUseT;

	BOOL bStt = pConc->m_sTitle.Find(_T("시점")) != -1 ? TRUE : FALSE;
	
	XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
	XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
	long i=0; for(i=0; i<MAX_REBAR_DAN; i++)
	{
		if(pConc->m_CSupportBarDia[i] == 0) continue;
		if(i != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConc->m_CSupportBarDia[i], pBri->GetValueFy(bStt? ePartWingWallStt : ePartWingWallEnd)), pConc->m_CSupportBarCTC[i]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);
	
	XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
	for(i=0; i<MAX_REBAR_DAN; i++)
	{
		if(pConc->m_TSupportBarDia[i] == 0) continue;
		if(i != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConc->m_TSupportBarDia[i], pBri->GetValueFy(bStt? ePartWingWallStt : ePartWingWallEnd)), pConc->m_TSupportBarCTC[i]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);
	
	XLText->AddFormatText("$u$h$m+06$-07$n$h");
	if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
	else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
	XLText->AddFormatText("$n$h");
	
	if(pBri->m_bCheckSuppoerBarWing15hs)
	{
		double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
		double dThick = MIN(dMaxH, pConc->m_Ho);
		double dB = pConc->m_Bo;
		double dReqOrg = (bSI) ? 0.0015 * dThick * dB: toCm2(0.0015 * dThick * dB);
		
		XLText->AddFormatText("$h$r$c-$r$e0.0015hs  [도로교설계기준 4.3.9]$n$h");
		if(pData->IsDoro2010())
			XLText->AddFormatText("$h$r$r$eh : 부재두께$m+04b : 부재 폭$n$h");
		else
			XLText->AddFormatText("$h$r$r$eh : 부재두께 최대 %.0f$m+07b : 부재 폭$n$h", dMaxH);
		
		XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");

		if(pData->m_nTypeKciCode != 0)	// Kci 2007
		{
			if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ㎟  (≤1800)$n$h", dThick, dB, dReqOrg);
			else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ㎠  (≤1800)$n$h", toCm(dThick), toCm(dB), dReqOrg);
			
			if(dReqOrg > 1800 && pData->IsDoro2010())
			{
				XLText->AddFormatText(_T("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n"));//zzzz줄맞추기
				XLText->AddFormatText(_T("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$eO.K$n$h"));
			}

			dAreaReq = MIN(dReqOrg, (bSI? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+12$c∴$r %.1f ㎟$m+03$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
			else XLText->AddFormatText("$h$m+12$c∴$r %.3f ㎠$m+03$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
		}
		else
		{
			if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ㎟", dThick, dB, dAreaReq);
			else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ㎠", toCm(dThick), toCm(dB), dAreaReq);
			
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
		}
		
		if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
	}
	
	if(pBri->m_bCheckSuppoerBarWingArea)
	{
		double dAreaSection = pConc->m_Ho * pConc->m_Bo;
		double dRatio = pBri->m_dCheckSuppoerBarWingArea;
		dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
		//dAreaReqSect = dAreaReq;
		
		if (pData->m_nTypeKciCode == 0 || pData->m_nTypeKciCode == 1)
		{
			XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 P.287]$n$h");
		}
		else // Kci2012
		{
			XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 11.3(2)]$n$h");
		}
		XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
		XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", pBri->m_dCheckSuppoerBarWingArea);
		if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", pBri->m_dCheckSuppoerBarWingArea, pConc->m_Bo, pConc->m_Ho, dAreaReq);
		else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", pBri->m_dCheckSuppoerBarWingArea, toCm(pConc->m_Bo), toCm(pConc->m_Ho), dAreaReq);
		
		if(pData->m_nTypeKciCode != 0)
			dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
		szComp = (dAreaReq > dAreaUseTot)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
		if(pData->m_nTypeKciCode != 0 && dAreaSection * dRatio >= 1800)	// Kci 2007
		{
			if(bSI)
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
				XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다.(KCI 2007) [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
			}
			else
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
				XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다.(KCI 2007) [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
			}
		}
		else
		{
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}
		XLText->AddFormatText("$n$h");
	}
	
	if(pBri->m_bCheckSuppoerBarWing300)
	{
		dAreaReq = (bSI)? 300:toCm2(300);
		
		XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
		XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
		else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);
		
		double dAreaUseComp = MIN(dAreaUseC, dAreaUseT);
		szComp = (dAreaReq > dAreaUseComp)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
		if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
		else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);
		
		if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
	}
	
/*	if(pBri->m_bCheckSuppoerBarWingArea)
	{
		dAreaUseComp = (pConc->m_sTitle == "중간부")? dAreaUseT:dAreaUseC;
		
		XLText->AddFormatText("$h$r$c-$r$e노출면 배치 철근량 검토$n$h");
		XLText->AddFormatText("$h$r$r$e노출면에 1/2이상, 2/3이하 배치$n$h");
		
		if(bSI) XLText->AddFormatText("$h$m+02$c:$r$e%.1f / 2 = %.1f ㎟$m+06", dAreaReqSect, dAreaReqSect/2);
		else XLText->AddFormatText("$h$m+02$c:$r$e%.3f / 2 = %.3f ㎠$m+06", dAreaReqSect, dAreaReqSect/2);
		szComp = (dAreaReqSect/2 < dAreaUseComp)? "<":">";
		if(bSI) XLText->AddFormatText("$c%s$r$e%.1f ㎟$m+03", szComp, dAreaUseComp);
		else XLText->AddFormatText("$c%s$r$e%.3f ㎠$m+03", szComp, dAreaUseComp);
		szComp = (dAreaReqSect*2/3 > dAreaUseComp)? "<":">";
		if(bSI) XLText->AddFormatText("$c%s$r$e%.1f x 2/3 = %.1f ㎟$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);
		else XLText->AddFormatText("$c%s$r$e%.3f x 2/3 = %.3f ㎠$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);
		
		szOkNg = (dAreaReqSect/2 < dAreaUseComp && dAreaReqSect*2/3 > dAreaUseComp)? "O.K":"N.G";
		if(szOkNg == "N.G") XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
	}*/
}

void CARcBridgeCalcWing::WriteCalcLoad_Dminus(CXLFormatText *pXLText, long nColStt, double dh, BOOL bUse)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double hLow = m_xyE.y - m_xyF.y;
	double dHL = m_dFH;	// m_pWing->m_dHL;
	double dL1 = m_pWing->m_dL1;
	CString szValueEta(_T(""));
	if(m_bLsd == FALSE)
	{
		hLow = toM(hLow);
		dHL = toM(dHL);
		dL1 = toM(dL1);
	}
	else
	{
		szValueEta.Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
	}

	double h1 = (dHL < dh)? 0:(dHL-dh);
	double sw = m_pWing->m_bSWHor ? 999999.0 : m_pWing->m_dSW;
	double dL = MIN((hLow-dh) * sw, dL1);
	
	
	if(hLow < dh) return;
	double dHs = 0; // 상부에 작용하는 토사 높이

	CString str = "";
	double Vl[LIVE_LOAD_COUNT], Ml[LIVE_LOAD_COUNT], Vh(0), Mh(0), Vh_(0), Mh_(0), Vd(0), Md(0);
	pXLText->GoPosition(NO_MOVE, nColStt);
	pXLText->AddText("$s+09");

	// 상재 하중에 의한 단면력
	for(long nLive=0; nLive<LIVE_LOAD_COUNT; ++nLive)
	{
		if(IsLiveLoad(nLive == LIVE_LOAD_ES) == FALSE)
		{
			Vl[nLive] = 0;
			Ml[nLive] = 0;
			continue;
		}
		double dAlwaysLoad = GetLiveLoadAlways(nLive == LIVE_LOAD_ES);

		CString sLoadMark = _T("");
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			sLoadMark = (nLive == LIVE_LOAD_ES) ? _T("(ES)") : _T("(LS)");
		}

		if(dAlwaysLoad > 0)
		{
			Vl[nLive] = m_dK0 * dAlwaysLoad * (h1*dL + pow(dL, 2)/(2*sw));
			Ml[nLive] = m_dK0 * dAlwaysLoad * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));

			pXLText->AddFormatText("$h$r- 상재하중에 의한 단면력%s$n$h", sLoadMark);
			str.Format("Vl  = ∫p(x)dx = Ko·q·∫(h1 + x/n)dx = Ko·q·(h1·L + L²/(2n))");
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("    = %.3f x %s x (%s x %s + %s²/ (2 x %.3f))", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("    = %s %s", pData->GetStrDouble(Vl[nLive], UNIT_CGS_TONF), pData->m_szTonf);
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("Ml  = ∫(L - x)P(x)dx = Ko·q·∫(L - x)(h1 + x/n)dx");
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("    = Ko·q·((h1·L²)/2 + L³/(6n))");
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("    = %.3f x %s x ((%s x %s²)/2 + %s³/ (6 x %.3f))", 
				m_dK0, pData->GetStrDouble(dAlwaysLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
			pXLText->AddFormatText("$h$r%s$n$h", str);
			str.Format("    = %s %s", pData->GetStrDouble(Ml[nLive], UNIT_CGS_TONFM), pData->m_szTonfM);
			pXLText->AddFormatText("$h$r%s$n$n$h", str);
		}
	}

	// 토압에 의한 단면력
	Vh = 1/2. * m_dK0 * m_dWeightBackFill * (pow(h1, 2)*dL + h1*pow(dL, 2)/sw + pow(dL, 3)/(3*pow(sw, 2)));
	Mh = 1/2. * m_dK0 * m_dWeightBackFill * ((pow(h1, 2)*pow(dL, 2))/2 + (h1*pow(dL, 3))/(3*sw) + pow(dL, 4)/(12*pow(sw, 2)));
	
	pXLText->AddText("$h$r- 토압에 의한 단면력$n$h");
	str.Format("Vh  = ∫p(x)dx = ½·Ko·r·∫(h1'+h2')·(h2'-h1')·dx");
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = ½·Ko·r·((h1·L²)/n + L³/(3n²))");
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = ½ x %.3f x %s x ((%s x %s²)/%.3f + %s³/(3 x %.3f²))", 
		m_dK0,  pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M2),  pData->GetStrDouble(h1, UNIT_CGS_M),  pData->GetStrDouble(dL, UNIT_CGS_M), sw,  pData->GetStrDouble(dL, UNIT_CGS_M), sw);
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = %s %s",  pData->GetStrDouble(Vh, UNIT_CGS_TONF), pData->m_szTonf);
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("Mh  = ∫(L - x)p(x)dx = ½·Ko·r·∫(L - x)(2h1/n·x + x²/n)dx");
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = ½·Ko·r·(L⁴/ 12n + h1/3n·L³)");
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = ½ x %.3f x %s x (%s⁴/ (12 x %.3f) + %s / (3 x %.3f) x %s³)", 
		m_dK0,  pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M2),  pData->GetStrDouble(dL, UNIT_CGS_M), sw,  pData->GetStrDouble(h1, UNIT_CGS_M), sw,  pData->GetStrDouble(dL, UNIT_CGS_M));
	pXLText->AddFormatText("$h$r%s$n$h", str);
	str.Format("    = %s %s", pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->m_szTonfM);
	pXLText->AddFormatText("$h$r%s$n$n$h", str);

	// 토피에 의한 단면력
	if(dHs+dh > 0)
	{
		Vh_ = m_dK0 * ((dHs+dh) * m_dWeightBackFill) * (h1*dL + pow(dL, 2)/(2*sw));
		Mh_ = m_dK0 * ((dHs+dh) * m_dWeightBackFill) * ((h1*pow(dL, 2))/2 + pow(dL, 3)/(6*sw));

		pXLText->AddText("$h$r- 토피에 의한 단면력$n$h");
		str.Format("Vh' = ∫p(x)dx = Ko·q·∫(h1 + x/n)dx = Ko·q·(h1·L + L²/(2n))");
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("    = %.3f x (%s x %s) x (%s x %s + %s²/ (2 x %.3f))", 
			m_dK0, pData->GetStrDouble((dHs+dh), UNIT_CGS_M), pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("    = %s %s", pData->GetStrDouble(Vh_, UNIT_CGS_TONF), pData->m_szTonf);
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("Mh' = ∫(L - x)P(x)dx = Ko·q·∫(L - x)(h1 + x/n)dx");
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("    = Ko·q·((h1·L²)/2 + L³/(6n))");
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("    = %.3f x (%s x %s) x ((%s x %s²)/2 + %s³/ (6 x %.3f))", 
			m_dK0, pData->GetStrDouble((dHs+dh), UNIT_CGS_M), pData->GetStrDouble(m_dWeightBackFill, UNIT_CGS_TONF_M2), pData->GetStrDouble(h1, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), sw);
		pXLText->AddFormatText("$h$r%s$n$h", str);
		str.Format("    = %s %s", pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), pData->m_szTonfM);
		pXLText->AddFormatText("$h$r%s$n$n$h", str);
	}

	if(bUse && !m_bLsd)
	{
		Vd = Vh + Vh_ + Vl[LIVE_LOAD_ES];
		Md = Mh + Mh_ + Ml[LIVE_LOAD_ES];

		pXLText->AddFormatText("$rVd' = %.3f + %.3f + %.3f = %.3f %s$n$h", Vl[LIVE_LOAD_ES], Vh, Vh_, Vd, pData->m_szTonf);
		pXLText->AddFormatText("$rMd' = %.3f + %.3f + %.3f = %.3f %s$n$h", Ml[LIVE_LOAD_ES], Mh, Mh_, Md, pData->m_szTonfM);
	}
	else
	{
		long nTypeFactor = bUse? WING_FACTOR_USE : WING_FACTOR_ULT;
		Vd = (m_dFactorToap[nTypeFactor]*(Vh+Vh_) + m_dFactorLive[nTypeFactor]*Vl[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor]*Vl[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];
		Md = (m_dFactorToap[nTypeFactor]*(Mh+Mh_) + m_dFactorLive[nTypeFactor]*Ml[LIVE_LOAD_ES] + m_dFactorLive_LS[nTypeFactor]*Ml[LIVE_LOAD_LS]) * m_dEta[nTypeFactor];
		CString szSl(_T("")), szMl(_T(""));
		if(pBri->m_nSelectWingLoad_LS_ES == 2)
		{
			szSl.Format(_T("(%.3f x %s + %.3f x %s)"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Vl[LIVE_LOAD_ES], UNIT_CGS_TONF), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(Vl[LIVE_LOAD_LS], UNIT_CGS_TONF));
			szMl.Format(_T("(%.3f x %s + %.3f x %s)"), m_dFactorLive[WING_FACTOR_ULT], pData->GetStrDouble(Ml[LIVE_LOAD_ES], UNIT_CGS_TONF), m_dFactorLive_LS[WING_FACTOR_ULT], pData->GetStrDouble(Ml[LIVE_LOAD_LS], UNIT_CGS_TONF));
		}
		else
		{
			long nTypeLive = IsLiveLoad_LS()? LIVE_LOAD_LS : LIVE_LOAD_ES;
			double dFactorLive = IsLiveLoad_LS()? m_dFactorLive_LS[WING_FACTOR_ULT] : m_dFactorLive[WING_FACTOR_ULT];
			szSl.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(Vl[nTypeLive], UNIT_CGS_TONF));
			szMl.Format(_T("%.3f x %s"), dFactorLive, pData->GetStrDouble(Ml[nTypeLive], UNIT_CGS_TONFM));
		}

		pXLText->AddFormatText("$rVd' = %s%s + %.3f x (%s + %s)%s%s = %s %s$n$h", 
			STR_BRACKET_STT, szSl, m_dFactorToap[nTypeFactor], pData->GetStrDouble(Vh, UNIT_CGS_TONF), pData->GetStrDouble(Vh_, UNIT_CGS_TONF), STR_BRACKET_END, szValueEta, pData->GetStrDouble(Vd, UNIT_CGS_TONF), pData->m_szTonf);
		pXLText->AddFormatText("$rMd' = %s%s + %.3f x (%s + %s)%s%s$n$h", 
			STR_BRACKET_STT, szMl, m_dFactorToap[nTypeFactor], pData->GetStrDouble(Mh, UNIT_CGS_TONFM), pData->GetStrDouble(Mh_, UNIT_CGS_TONFM), STR_BRACKET_END, szValueEta);
		pXLText->AddFormatText("$r    = %s %s$n$h", pData->GetStrDouble(Md, UNIT_CGS_TONFM), pData->m_szTonfM);
	}
	*pXLText += 1;
}

BOOL CARcBridgeCalcWing::IsLiveLoad_ES()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		return TRUE;

	return pBri->m_nSelectWingLoad_LS_ES != 0;
}

BOOL CARcBridgeCalcWing::IsLiveLoad_LS()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		return FALSE;

	return pBri->m_nSelectWingLoad_LS_ES != 1;
}

BOOL CARcBridgeCalcWing::IsLiveLoad( BOOL bES )
{
	if(bES)
	{
		return IsLiveLoad_ES();
	}
	else
	{
		return IsLiveLoad_LS();
	}
}

void CARcBridgeCalcWing::WriteCalcLoad_qq( CXLFormatText *pXLText )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLSD = pData->IsLsdDesign();
	CString str, str1;

	// pData->IsRailDesign() 일때 수식표현 다르게 해주는건???
	if(IsLiveLoad_ES())
	{
		str.Format(_T("q0  = Ko x q"));
		pXLText->AddFormatText(_T("$h%s$m+06"), str);
		str.Format(_T(" = %.3f x %s"), m_dK0, pData->GetStrDouble(m_dLoadAlways, UNIT_CGS_TONF_M2));
		str1.Format(_T(" = %s %s"), pData->GetStrDouble(m_dK0*m_dLoadAlways, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		pXLText->AddFormatText(_T("%s$m+07%s$n$h"), str, str1);
	}
	if(IsLiveLoad_LS())
	{
		str = (pBri->m_nSelectWingLoad_LS_ES == 2) ? _T("q0' = Ko x q'") : _T("q0  = Ko x q");
		pXLText->AddFormatText(_T("$h%s$m+06"), str);
		str.Format(_T(" = %.3f x %s"), m_dK0, pData->GetStrDouble(m_dLoadAlways_LS, UNIT_CGS_TONF_M2));
		str1.Format(_T(" = %s %s"), pData->GetStrDouble(m_dK0*m_dLoadAlways_LS, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
		pXLText->AddFormatText(_T("%s$m+07%s$n$h"), str, str1);
	}
}

double CARcBridgeCalcWing::GetLiveLoadAlways( BOOL bES )
{
	return bES ? m_dLoadAlways : m_dLoadAlways_LS;
}

void CARcBridgeCalcWing::WriteCalcAlwaysLiveLoad( CXLFormatText *pXLText )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	// 상재활하중 LS
	if(IsLiveLoad_LS() == FALSE)
		return;

	CString str;
	// (ARCBRIDGE-29710) 선형중심에서의 벽체 높이에서 날개벽 높이로 변경
	double dHeightCalc = pBri->GetXySectionWingWallStt(m_bStt, TRUE).y -  pBri->GetXySectionWingWallStt(m_bStt, FALSE).y;
	double dHeq	= GetEquivalentHeightWall(dHeightCalc, pBri->m_nLiveLoadDistToBackWall);

	str.Format(_T("* 상재활하중 산정 (LS)"));
	pXLText->AddFormatText(_T("$h$r%s$n$h"), str);

	str.Format(_T("  - 차량하중에 대한 흙의 등가높이"));
	pXLText->AddFormatText(_T("$h$r%s$n$h"), str);
	str.Format(_T("벽체 높이 :       H   = %.3f mm"), dHeightCalc);
	pXLText->AddFormatText(_T("$h$r$r%s$n$h"), str);
	str.Format(_T("흙의 등가 높이 : Heq = %s mm (%s)"), CommaC(dHeq, 0), pBri->m_nLiveLoadDistToBackWall == 0 ? _T("차량으로부터 거리 0mm") : _T("차량으로부터 거리 300mm 이상"));
	pXLText->AddFormatText(_T("$h$r$r%s$n$h"), str);

	str.Format(_T("참고문헌: ASSHTO Table 3.11.6.4-2"));
	pXLText->AddFormatText(_T("$h$r$r%s$n$h"), str);

	//ASSHTO Table 삽입
	pXLText->GoPosition(NO_MOVE, 4);
	pXLText->AddTable(4, 14, 2, 0);
	pXLText->AddText(hggettext("$h$me6$md1$c벽체 높이(m) $m+07$me7$c차량으로부터 벽체 배면까지 거리 $n$h"));
	pXLText->AddText(hggettext("$h$m+07$me3$c0.0mm $m+04$me3$c300mm 이상$n$h"));
	pXLText->AddFormatText(_T("$h$me6$c1.5 $m+07$me3$c%s $m+04$me3$c%s$n$h"),  _T("1,500"), _T("600"));
	pXLText->AddFormatText(_T("$h$me6$c%s $m+07$me3$c%s $m+04$me3$c%s$n$h"), _T("'3.0"), _T("1,050"), _T("600"));
	pXLText->AddFormatText(_T("$h$me6$c≥6.0 $m+07$me3$c%s $m+04$me3$c%s$n$h"), _T("600"), _T("600"));

	CString szCorr = _T("");
	if(pBri->m_bCorrectionFactor_LiveLoadAlways)
	{
		double dCorrFactor = pBri->m_bCorrectionFactor_LiveLoadAlways ? pBri->m_dCorrectionFactor_LiveLoadAlways : 1.00;
		str.Format(_T("ASSHTO 표준차량과 축중비를 고려하여 보정계수 %.2f(192kN/144N)을 적용"), dCorrFactor);
		pXLText->AddFormatText(_T("%s$n$h"), str);

		szCorr.Format(_T(" x %.2f"), dCorrFactor);
	}
	pXLText->AddFormatText(_T("$n$h"));
	pXLText->GoPosition(NO_MOVE, 2);

	str.Format(_T("  - 상재활하중에 의한 수평력"));
	pXLText->AddFormatText(_T("$h$r%s$n$h"), str);

	double dRoadWaysFactor = m_pStd->m_pLoad->GetCalculateRoadWayFactor();
	double	dr	=	GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dLiveLoad = GetLiveLoadAlways(FALSE);

	CString sQ = (pBri->m_nSelectWingLoad_LS_ES == 2) ? _T("q'") : _T("q");
	CString szRoadWaysFactor = (pData->m_nLSDRoadType == 0) ? _T("") : _T(" x ") + CommaC(dRoadWaysFactor, 2, FALSE);

	// (ARCBRIDGE-3009) 도로의 종류와 상관없이 다차로재하계수는 적용하지 않는것으로 변경
	if(pBri->m_bCorrectionFactor_LiveLoadAlways)
	{
		str.Format(_T("%s = %.6f x %.0f x %.2f = %.6f N/mm²"), sQ, dr, dHeq, pBri->m_dCorrectionFactor_LiveLoadAlways, dLiveLoad);
	}
	else
	{
		str.Format(_T("%s = %.6f x %.0f = %.6f N/mm²"), sQ, dr, dHeq, dLiveLoad);
	}
	pXLText->AddFormatText(_T("$h$r$r%s$n$h"), str);
}

double CARcBridgeCalcWing::GetEquivalentHeightWall( double dHeightWall, long nLiveLoadDistToBackWall )
{
	double dEquivalentHeight(0);

	if(nLiveLoadDistToBackWall == 1)
	{
		return 600;
	}

	// (ARCBRIDGE-2955) 이 테이블을 기준으로 직선보간법으로 산정
	if(dHeightWall >= 6000)
		dEquivalentHeight = 600;
	else if(dHeightWall < 1500)
		dEquivalentHeight = 1500;
	else if(dHeightWall >= 3000)
	{
		// 1.050 - 0.15 x (h - 3)
		dEquivalentHeight = 1050 - 0.15 * (dHeightWall - 3000);
	}
	else
	{
		// 1.050 + 0.3 x (3 - h)
		dEquivalentHeight = 1050 + 0.3 * (3000 - dHeightWall);
	}

	return dEquivalentHeight;
}

CString CARcBridgeCalcWing::GetStringTypeCase( long nTypeCase )
{
	CString strName(_T(""));

	switch (nTypeCase)
	{
	case WING_FACTOR_ULT:
		strName = _T("극한한계상태");	break;
	case WING_FACTOR_USE:
		strName = _T("사용한계상태-검토");	break;
	case WING_FACTOR_USE_JUDGE:
		strName = _T("사용한계상태-판정");	break;
	default:
		break;
	}
	
	return strName;
}
