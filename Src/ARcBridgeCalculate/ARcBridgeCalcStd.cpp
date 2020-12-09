// ARcBridgeCalcStd.cpp: implementation of the CARcBridgeCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcStd::CARcBridgeCalcStd(CRcBridgeRebar *pRcBridge)
{
	m_pBri = pRcBridge;
	
	m_pLoad		= new CARcBridgeCalcLoad;
	m_pLoadEQ	= new CARcBridgeCalcEarthQuake;
	m_pFrame	= new CARcBridgeDesignFrame;
	m_pShell	= new CARcBridgeDesignShell;
	m_pApsStt[0]	= new CARcBridgeCalcAps(&m_pBri->m_apsStt[0], TRUE);
	m_pApsEnd[0]	= new CARcBridgeCalcAps(&m_pBri->m_apsEnd[0], FALSE);
	m_pApsStt[1]	= NULL;
	m_pApsEnd[1]	= NULL;
	if(m_pBri->m_apsStt[1].m_bIs) m_pApsStt[1]	= new CARcBridgeCalcAps(&m_pBri->m_apsStt[1], TRUE);
	if(m_pBri->m_apsEnd[1].m_bIs) m_pApsEnd[1]	= new CARcBridgeCalcAps(&m_pBri->m_apsEnd[1], FALSE);

	m_pMeshCalc	= new CARcBridgeMeshCalc;
	m_pCorner[iUPPER]	= new CARcBridgeCalcCornerAndSupport;
	m_pCorner[iLOWER]	= new CARcBridgeCalcCornerAndSupport;
	m_pFooting	= new CARcBridgeCalcFooting;
	long i=0; for(i=0; i<4; i++)
	{
		m_pWing[i] = new CARcBridgeCalcWing;
		m_pWing[i]->m_pStd = this;
	}
	m_pColGir  = new CARcBridgeCalcColAndGir;
	m_pFatigue = new CARcBridgeCalcFatigue;
	m_pSummary = new CARcBridgeCalcSummary;
	m_pArchRib = new CARcBridgeCalcArchRib;
	m_pCantilever = new CCalcCantilever;

	m_pModulusFoundation = new CModulusFoundation;
	m_pStm	= new CARcBridgeCalcStm(this);

	m_pLoad->m_pStd		= this;
	m_pLoadEQ->m_pStd	= this;
	m_pFrame->m_pStd	= this;
	m_pShell->m_pStd	= this;
	m_pApsStt[0]->m_pStd	= this;
	m_pApsEnd[0]->m_pStd	= this;
	if(m_pApsStt[1]) m_pApsStt[1]->m_pStd	= this;
	if(m_pApsEnd[1]) m_pApsEnd[1]->m_pStd	= this;
	m_pMeshCalc->m_pStd = this;
	m_pCorner[iUPPER]->m_pStd   = this;
	m_pCorner[iLOWER]->m_pStd   = this;
	m_pFooting->m_pStd	= this;
	m_pColGir->m_pStd	= this;
	m_pFatigue->m_pStd	= this;
	m_pSummary->m_pStd	= this;
	m_pArchRib->m_pStd	= this;
	m_pCantilever->m_pStd = this;
	m_pModulusFoundation->m_pStd = this;
	
	m_pPileEx = new CExPileData;

	for(long nJijum=0; nJijum<m_pBri->GetCountJijum(); nJijum++)
	{
		if(m_pBri->IsPileBase(nJijum)==FALSE) continue;
		if(m_pBri->IsBoxType() && nJijum > 0) break;

		CDesignPile *pCalcPile = new CDesignPile(m_pBri->m_pARcBridgeDataStd->m_nTypeUnit, m_pBri->m_pARcBridgeDataStd->IsRailDesign() ? 1 : 0);
		pCalcPile->m_nIndexJijum = nJijum;
		pCalcPile->m_bBridgeDir  = TRUE;
		SetCalcPileData(pCalcPile);
		m_pArrPileCalc.Add(pCalcPile);
	}
}

CARcBridgeCalcStd::~CARcBridgeCalcStd()
{	
	delete m_pLoad;
	delete m_pLoadEQ;
	delete m_pFrame;
	delete m_pShell;
	delete m_pApsStt[0];
	delete m_pApsEnd[0];
	if(m_pApsStt[1]) delete m_pApsStt[1];
	if(m_pApsEnd[1]) delete m_pApsEnd[1];
	delete m_pMeshCalc;
	delete m_pCorner[iUPPER];
	delete m_pCorner[iLOWER];
	delete m_pFooting;
	long i=0; for(i=0; i<4; i++)
	{
		delete m_pWing[i];
	}
	delete m_pColGir;
	delete m_pFatigue;
	delete m_pSummary;
	delete m_pArchRib;
	delete m_pModulusFoundation;
	delete m_pStm;
	delete m_pCantilever;

	AhTPADelete(&m_pArrPileCalc, (CDesignPile*)0);

	delete m_pPileEx;
}

// 모델링지간중 가장 작은 값을 구함.
double CARcBridgeCalcStd::GetLengthModelSpanMin()
{
	double dLengthSpanMin = 0;
	double dLengthSpanMax = 0;
	double dLengthSpanAvg = 0;
	GetLengthModelSpanMinMax(dLengthSpanMin, dLengthSpanMax, dLengthSpanAvg);

	return dLengthSpanMin;
}

// 모델링지간중 가장 긴 값을 구함.
double CARcBridgeCalcStd::GetLengthModelSpanMax()
{
	double dLengthSpanMin = 0;
	double dLengthSpanMax = 0;
	double dLengthSpanAvg = 0;
	GetLengthModelSpanMinMax(dLengthSpanMin, dLengthSpanMax, dLengthSpanAvg);
	return dLengthSpanMax;
}

double CARcBridgeCalcStd::GetLengthModelSpanAvg()
{
	double dLengthSpanMin = 0;
	double dLengthSpanMax = 0;
	double dLengthSpanAvg = 0;
	GetLengthModelSpanMinMax(dLengthSpanMin, dLengthSpanMax, dLengthSpanAvg);
	return dLengthSpanAvg;
}

void CARcBridgeCalcStd::GetLengthModelSpanMinMax(HGDOUBLE &dMin, HGDOUBLE &dMax, HGDOUBLE &dAvg)
{
//	double dLengthSpan = 0;

	// 0 - 구조물의 내측을 기준으로 모델링
	// 1 - 구조물의 중심축선을 기준으로 모델링
	//long nTypeModeling = 0;

	CDPoint vAngStt = m_pBri->GetAngleJijum(0);
	CDPoint vAngEnd = m_pBri->GetAngleJijum(m_pBri->m_nQtyJigan);

	double dSumLength(0);
	double dLengthSpanMin = 0;
	double dLengthSpanMax = 0;
	for(long nSpan=0; nSpan<m_pBri->m_nQtyJigan; nSpan++)
	{
		double dLengthSpan = m_pBri->GetLengthJigan(nSpan);
		if(nSpan==m_pBri->m_nQtyJigan-1)
		{
			CJijum* pJijum = m_pBri->GetJijum(nSpan+1);
			if(m_pBri->m_bSelectSpanLengthApplyShoe && pJijum && pJijum->m_nQtyShoe[0] > 0)
			{
				dLengthSpan -= (pJijum->m_dLengthUgan[0] + pJijum->m_dLengthSlabToShoe[0]);
			}
			else dLengthSpan += m_pBri->m_dWE/2/vAngEnd.y;
		}
		if(nSpan==0)
		{
			CJijum* pJijum = m_pBri->GetJijum(0);
			if(m_pBri->m_bSelectSpanLengthApplyShoe && pJijum && pJijum->m_nQtyShoe[1] > 0)
			{
				dLengthSpan -= (pJijum->m_dLengthUgan[1] + pJijum->m_dLengthSlabToShoe[1]);
			}
			else dLengthSpan += m_pBri->m_dWS/2/vAngStt.y;
		}

		dLengthSpanMin = (nSpan==0) ? dLengthSpan : MIN(dLengthSpanMin,dLengthSpan);
		dLengthSpanMax = (nSpan==0) ? dLengthSpan : MAX(dLengthSpanMax,dLengthSpan);
		dSumLength += dLengthSpan;
	}

	dMin = dLengthSpanMin;
	dMax = dLengthSpanMax;
	dAvg = dSumLength / m_pBri->m_nQtyJigan;
}

void CARcBridgeCalcStd::CalculateAll()
{
	if(m_pBri==NULL) return;
	
	BOOL bArch = FALSE;
	if(m_pBri)
	{
		// TRAP2는 모델링은 수평으로 되있지만 기초 계산시 기초 형상 때문에 경사로 그려놓고 계산을 하는게 맞는듯..
		// 혹시라도 문제가 생긴다면 빼고 기초 하중 구하는 부분에서만 바꿔 주어야 하겠다..
		// BOX Tpye일때는 하단경사가 없다면, 수평으로 맞추는게 아니라 그대로 해줘야...
		bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH || m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
		if(m_pBri->IsBoxType() && !m_pBri->m_bSlopeLowerSlab) bArch = TRUE;

		m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bArch? 1 : 0);
		m_pBri->SyncBridge();
		m_pBri->SyncBridgeRebar();
	}
	CalculateCofficient();
	
	m_pFrame->MakeXyCoordinates(FALSE);
	m_pFrame->MakeArrayThick();

	m_pModulusFoundation->CalculateAll();

	m_pLoad->CalculateAll();
	m_pLoadEQ->CalculateAll();

	m_pColGir->CalculateAll();

	m_pFooting->CalculateAll();

	m_pApsStt[0]->CalculateAll();
	m_pApsEnd[0]->CalculateAll();
	if(m_pApsStt[1]) m_pApsStt[1]->CalculateAll();
	if(m_pApsEnd[1]) m_pApsEnd[1]->CalculateAll();
	m_pCorner[iUPPER]->CalculateAll(TRUE);
	if(m_pBri->IsBoxType())
	{
		m_pCorner[iLOWER]->CalculateAll(FALSE);
	}

	m_pWing[0]->CalculateAll(TRUE,TRUE);
	m_pWing[1]->CalculateAll(TRUE,FALSE);
	m_pWing[2]->CalculateAll(FALSE,TRUE);
	m_pWing[3]->CalculateAll(FALSE,FALSE);
	m_pFatigue->CalculateAll();
	if(m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		m_pBri->TrimRibByFootingUpper();
		m_pArchRib->CalculateAll();
	}

	if(m_pBri)
	{
		m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, bArch? 1 : 0);
	}

	m_pCantilever->CalculateAll();
}

double CARcBridgeCalcStd::GetDesignRoadWidth()
{
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double dWidth(0);
	if(bLsd)
	{
		dWidth = (pBri->m_nSelectDesignRoadWidth==0) ? 3000 : pData->m_dDesignRoadWidth;
	}
	else
	{
		double dWidthBetweenCurb = m_pBri->GetWidthTotalRoadWay();
		 
		long nCountLane = GetCountLane(dWidthBetweenCurb);
		dWidth = (pBri->m_nSelectDesignRoadWidth==0) ? 3000 : Round(MIN(3600, dWidthBetweenCurb/(double)(nCountLane)),3);
	}

	return dWidth;
}

// 설계차로수 : 도로교 설계기준 2.1.3,(2)
long CARcBridgeCalcStd::GetCountLane(double dWidth)
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	// 차도가 없는 경우에는 "0"차선을 리턴...
	BOOL bExistLane = FALSE;
	long nQtyHDan = m_pBri->GetQtyHDan();
	for(int nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC *pGW = m_pBri->GetGuardWallByHDan(nHDan);
		if(pGW->IsTypeHDanOnlyNone())
		{
			bExistLane = TRUE;
			break;
		}
	}
	if(bExistLane==FALSE) return 0;

	
	//////////////////////////////////////////////////////////////////////////
	double dWidthBetweenCurb = (dWidth==0) ? m_pBri->GetWidthTotalRoadWay() : dWidth;	// mm

	long nLaneCount = 1;
	if(pData->IsLsdDesign())
	{
		nLaneCount = (long)(RoundDown(dWidthBetweenCurb / pData->m_dDesignRoadWidth, 0));
	}
	else
	{
		if(Round(dWidthBetweenCurb, 0)>= 6000)	nLaneCount =  2;
		if(Round(dWidthBetweenCurb, 0)>= 9100)	nLaneCount =  3;
		if(Round(dWidthBetweenCurb, 0)>=12800)	nLaneCount =  4;
		if(Round(dWidthBetweenCurb, 0)>=16400)	nLaneCount =  5;
		if(Round(dWidthBetweenCurb, 0)>=20100)	nLaneCount =  6;
		if(Round(dWidthBetweenCurb, 0)>=23800)	nLaneCount =  7;
		if(Round(dWidthBetweenCurb, 0)>=27400)	nLaneCount =  8;
		if(Round(dWidthBetweenCurb, 0)>=31100)	nLaneCount =  9;
		if(Round(dWidthBetweenCurb, 0)>=34700)	nLaneCount = 10;
	}

	return nLaneCount;
}

double CARcBridgeCalcStd::GetWheelLoadFront()
{
	double dWheelLoad = 2.400; // tonf

	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) dWheelLoad = 1.800;
	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) dWheelLoad = 1.350;
	
	return dWheelLoad;
}

double CARcBridgeCalcStd::GetWheelLoadMiddle()
{
	double dWheelLoad = 6.750; // tonf

	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) dWheelLoad = 5.0625;
	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) dWheelLoad = 3.7969;

	return dWheelLoad;
}

double CARcBridgeCalcStd::GetWheelLoadRear()
{
	double dWheelLoad = 9.600; // tonf

	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) dWheelLoad = 7.200;
	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) dWheelLoad = 5.400;

	return dWheelLoad;
}

double CARcBridgeCalcStd::GetWheelLoadPm()
{
	double Pm = 10.80; // tonf

	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) Pm = 8.100;
	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) Pm = 6.080;

	return Pm;
}

double CARcBridgeCalcStd::GetWheelLoadPs()
{
	double Ps = 15.60; // tonf

	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) Ps = 11.70;
	if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) Ps = 8.780;

	return Ps;
}

double CARcBridgeCalcStd::GetWheelLoadDistribution()
{
	double w = 1.270; // tonf

	if (m_pBri->m_pARcBridgeDataStd->IsLsdDesign() )
	{
		if (m_pBri->m_nLoadStandardLane == 0)
		{
			const HGDOUBLE dRate = 0.75;

			if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) w = w * dRate;
			if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) w = w * dRate * dRate;
		}
	}
	else
	{
		if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) w = 0.950;
		if(m_pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) w = 0.710;
	}

	return w;
}

// double CARcBridgeCalcStd::GetCoefficient_Rankine()
// {
// 	return m_dCoefficient_Rankine;
// }

double CARcBridgeCalcStd::GetCoefficient_coulomb()
{
	return m_dCoefficient_Coulomb;
}

double CARcBridgeCalcStd::GetCoefficient_MononobeOkabe(long nIdxPos)
{
	// 함형이 아닌경우 => 시점, 종점
	// 함형인 경우 => 기능수행, 붕괴방지, 즉시복구, 장기복구
	return m_dCoefficient_MononobeOkabe[nIdxPos];
}

void CARcBridgeCalcStd::CalculateCofficient_coulomb()
{
	double pi	 = ToRadian(m_pBri->m_dAngleFrictionBackFill);
	double alpha = 0;
	double beta	 = 0;	// 교대배면의 수직에 대한 각 [도.시 798]
	double delta = 0;

	double dValueUpper  = pow(cos(pi-beta),2);
	double dValueLower1 = pow(cos(beta),2) * cos(beta+delta);
	double dValueLower2 = sin(pi+delta)*sin(pi-alpha);
	double dValueLower3 = cos(beta+delta)*cos(beta-alpha);

	m_dCoefficient_Coulomb = Round(dValueUpper / (dValueLower1 * pow(1+sqrt(dValueLower2/dValueLower3),2)),3);
}

void CARcBridgeCalcStd::CalculateCofficient_Rankine()
{
	double pi	 = ToRadian(m_pBri->m_dAngleFrictionBackFill);
	double alpha = 0;

	double dValueUpper = cos(alpha)-sqrt(cos(alpha)*cos(alpha)-cos(pi)*cos(pi));
	double dValueLower = cos(alpha)+sqrt(cos(alpha)*cos(alpha)-cos(pi)*cos(pi));

	m_dCoefficient_Rankine = Round(cos(alpha)*dValueUpper/dValueLower,3);
}

void CARcBridgeCalcStd::CalculateCofficient_MononobeOkabe()
{
	BOOL bAngledPile = FALSE;

	for(int n=0; n<eGoalEarthQuakeCount; n++)
	{
		EGoalOfEarthQuake eGolaEq = m_pBri->IsBoxType() ? static_cast<EGoalOfEarthQuake>(n) : eGoalEQ_None;
		if(m_pBri->IsBoxType())
		{
			if(m_pBri->m_bGoalEarthQuake[n] == FALSE) continue;
		}
		else
		{
			if(n > 1)	break;
		}

		BOOL bStt = (n == 0) ? TRUE : FALSE;
		double Kh = GetKhValue(bAngledPile, bStt, eGolaEq);
		double Kv	 = 0;
		double pi	 = ToRadian(m_pBri->m_dAngleFrictionBackFill);
		double beta	 = 0;	// 교대배면의 수직에 대한 각 [도.시 798]
		double theta = atan(Kh/(1-Kv));	// tan-1(Kh/(1-Kv));
		double delta = 0;	// 흙과 교대사이의 마찰각
		double i	 = 0;	// 뒷채움흙의 경사각
		
		double dValueUpper  = pow(cos(pi-theta-beta),2);
		double dValueLower1 = cos(theta)*pow(cos(beta),2) * cos(delta+beta+theta);
		double dValueLower2 = sin(pi+delta)*sin(pi-theta-i);
		double dValueLower3 = cos(delta+beta+theta)*cos(i-beta);
		m_dCoefficient_MononobeOkabe[n]  = Round(dValueUpper / (dValueLower1 * pow(1+sqrt(dValueLower2/dValueLower3),2)),3);
	}
}

double CARcBridgeCalcStd::GetSlabWidth(long nJijum)
{
	double SlabWidth = 0;
	if(nJijum == -1)
	{
		long nLine   = 0;

		nJijum  = 0;
		CDPoint vAng = CDPoint(0,1);
		double Station   = m_pBri->GetStationOnJijum(nJijum,nLine);
		SlabWidth = toM(m_pBri->GetWidthSlabAct(Station,vAng));	// 슬래브폭원(교량폭,벽체폭)
	}
	else
	{
		CDPoint vAng = m_pBri->GetAngleJijum(nJijum);
		SlabWidth = toM(m_pBri->GetWidthSlabActByGagak(m_pBri->GetStationOnJijum(nJijum), vAng) * vAng.y);
	}

	return SlabWidth; // m
}

// 모델링구간내의 동륜하중 바퀴수를 구한다...
long CARcBridgeCalcStd::GetQtyRailWheel()
{
	if(!m_pBri->m_pARcBridgeDataStd->IsRailDesign()) return 0;

	double SpanLength = toM(m_pBri->GetLengthModelSpan(-1));
	double LsLoad	  = m_pBri->m_pARcBridgeDataStd->m_RailLoadWheel;
	long   nQtyWheel  = 0;

	long nSizeLoad = m_pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetSize();

	double DistSum = 0;
	long i=0; for(i=0; i<nSizeLoad; i++)
	{
		double dLoadWheel = m_pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetAt(i);
		double dDistNext  = m_pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetAt(i);

		if(dLoadWheel==LsLoad)
		{
			if(DistSum<=SpanLength) 
				nQtyWheel++;
			else
				break;
		}

		DistSum += dDistNext;
	}

	return nQtyWheel;
}

void CARcBridgeCalcStd::CalculateCofficient()
{
	double dAngleRad    = ToRadian(m_pBri->m_dAngleFrictionBackFill);
	m_dCoefficientKo = Round(1 - sin(dAngleRad),3);
	CalculateCofficient_coulomb();
	CalculateCofficient_Rankine();
	CalculateCofficient_MononobeOkabe();
}


// 모델링중심을 기준으로한 두께를 구한다...
// 헌치두께가 시종점방향으로 다른 경우는 두꺼운 것을 기준으로 적용한다.....
double CARcBridgeCalcStd::GetThickSlabByDist(long nJijum, double DistFromJijum, BOOL bUpperSlab)
{
	double SlabThick = bUpperSlab ? m_pBri->m_dTS : m_pBri->m_dTS_Lower;
	if(m_pBri->m_bSelectHunchModeling == FALSE) return SlabThick;

	double HunchThickStt = m_pBri->GetThickHunch(nJijum, TRUE , FALSE, bUpperSlab);
	double HunchThickEnd = m_pBri->GetThickHunch(nJijum, FALSE, FALSE, bUpperSlab);
	double HunchWidthStt = m_pBri->GetWidthHunch(nJijum, TRUE, bUpperSlab);
	double HunchWidthEnd = m_pBri->GetWidthHunch(nJijum, FALSE, bUpperSlab);
	if(HunchWidthStt == 0) HunchThickStt = 0;
	if(HunchWidthEnd == 0) HunchThickEnd = 0;
	double HunchThick    = MAX(HunchThickStt,HunchThickEnd);

	if(m_pBri->IsRcSlab() && (nJijum!=0 && nJijum!=m_pBri->m_nQtyJigan))
	{
		if(HunchThickStt>0)
		{
			return Round((SlabThick + HunchThickStt), 0);
		}
		if(HunchThickEnd>0 && HunchThickStt!=HunchThickEnd)
		{
			return Round((SlabThick + HunchThickEnd), 0);
		}
		else return Round(SlabThick, 0);
	}
	else if(nJijum==0)
	{
		CDPoint vAng = m_pBri->GetAngleJijum(0);
		double dW = (m_pBri->IsPiRahmen() || m_pBri->IsRcSlab())? m_pBri->m_dWS/vAng.y : m_pBri->m_dWS/(2*vAng.y);
		if(DistFromJijum<=dW) 
			return Round((HunchThick+SlabThick), 0);
		else if(DistFromJijum>=dW+HunchWidthEnd)
			return Round(SlabThick, 0);
		else
			return Round(SlabThick + HunchThick*(HunchWidthEnd+dW-DistFromJijum)/HunchWidthEnd, 0);
	}
	else if(nJijum==m_pBri->m_nQtyJigan)
	{
		CDPoint vAng = m_pBri->GetAngleJijum(nJijum);
		double dW = (m_pBri->IsPiRahmen() || m_pBri->IsRcSlab())? m_pBri->m_dWE/vAng.y : m_pBri->m_dWE/(2*vAng.y);
		if(DistFromJijum>=-dW) 
			return Round((HunchThick+SlabThick), 0);
		else if(-DistFromJijum>=dW+HunchWidthStt)
			return Round(SlabThick, 0);
		else
			return Round(SlabThick + HunchThick*(HunchWidthStt+dW+DistFromJijum)/HunchWidthStt, 0);
	}
	else
	{
		long nUD = bUpperSlab ? iUPPER : iLOWER;
		long nIdxInWall = nJijum - 1;
		long nIdxHunchLeft  = m_pBri->GetIdxHunchInWall(nIdxInWall,TRUE);

		double WL  = m_pBri->m_dWL[nIdxHunchLeft][nUD];
		double WR  = m_pBri->m_dWR[nIdxHunchLeft][nUD];

		if(DistFromJijum>=-WL && DistFromJijum<=WR)
			return Round((HunchThick+SlabThick), 0);
		else if(DistFromJijum<-(WL+HunchWidthStt) || DistFromJijum>(WR+HunchWidthEnd))
			return Round(SlabThick, 0);
		else
		{
			if(DistFromJijum<0)
			{
				return Round(SlabThick + HunchThick*(WL+HunchWidthStt+DistFromJijum)/HunchWidthStt, 0);
			}
			else
			{
				return Round(SlabThick + HunchThick*(WR+HunchWidthEnd-DistFromJijum)/HunchWidthEnd, 0);
			}
		}
	}

	return 0;
}

double CARcBridgeCalcStd::GetThickWallByDist(long nJijum, double DistFromSlab, BOOL bFrBottom)
{
	double WallThick  = m_pBri->GetThickWall(nJijum);
	if(m_pBri->m_bSelectHunchModeling == FALSE) return WallThick;

	if(bFrBottom)
	{
		if(nJijum>0 && nJijum<m_pBri->m_nQtyJigan) return WallThick;

		double dHunchWidth = (nJijum==0) ? m_pBri->m_dWSHW : m_pBri->m_dWEHW;
		double dHunchHeight = (nJijum==0) ? m_pBri->m_dWSHH : m_pBri->m_dWEHH;

		if(dHunchWidth > dHunchHeight/3) dHunchWidth = dHunchHeight/3;

		if(DistFromSlab>=dHunchHeight) return Round(WallThick, 0);
		else if(DistFromSlab > 0) return Round(WallThick + dHunchWidth*(dHunchHeight-DistFromSlab)/dHunchHeight, 0);
		else return Round(WallThick + dHunchWidth, 0);
	}
	else
	{
		double HunchWidthStt = m_pBri->GetWidthHunch(nJijum,TRUE);
		double HunchWidthEnd = m_pBri->GetWidthHunch(nJijum,FALSE);
		double dThickHunchStt = m_pBri->GetThickHunch(nJijum,TRUE,FALSE);
		double dThickHunchEnd = m_pBri->GetThickHunch(nJijum,FALSE,FALSE);
		double SlabThick     = m_pBri->m_dTS;
		double SlabHunch     = m_pBri->m_dHHS;	
		
		if(HunchWidthStt > dThickHunchStt/3) HunchWidthStt = dThickHunchStt/3;
		if(HunchWidthEnd > dThickHunchEnd/3) HunchWidthEnd = dThickHunchEnd/3;
		
		if(DistFromSlab>=SlabThick/2+SlabHunch) 
			return WallThick;
		else if(DistFromSlab<=SlabThick/2) 
			return WallThick+HunchWidthStt+HunchWidthEnd;
		else
		{
			double ThickHunch = HunchWidthStt + HunchWidthEnd;
			return WallThick + ThickHunch*(SlabThick/2+SlabHunch-DistFromSlab)/SlabHunch;
		}
	}

	return 0;
}

double CARcBridgeCalcStd::GetKhValue(BOOL bFixdMovement, BOOL bStt, EGoalOfEarthQuake eGoalEq)
{
	// 설계선택사향에서 자동을 선택했을경우 인자로 받은 bFixdMovement에 의해서 결정되고
	// 특정 값을 선택했을때는 그 값으로...   0:자동, 1:0.5, 2:1.0, 3:1.5
	long nSelFactor = (bStt)? m_pBri->m_nSelFactorKhStt : m_pBri->m_nSelFactorKhEnd;

	double dDeductFactor = m_pBri->m_dCoffKh_Sei / 100.0;
	double A = m_pBri->GetCoefficientAccelerateEQ(eGoalEq);
	double S = m_pBri->GetCoefficientGround();

	if(m_pBri->IsBoxType())
	{
		return A * 1.0 * dDeductFactor;
	}
	else
	{
		if(nSelFactor == 0)
		{
			if(bFixdMovement)
				return A * 1.5 * dDeductFactor;
			else
				return A * 0.5 * dDeductFactor;
		}
		else if(nSelFactor == 1) return A * 0.5 * dDeductFactor;
		else if(nSelFactor == 2) return A * 1.0 * dDeductFactor;
		else if(nSelFactor == 3) return A * 1.5 * dDeductFactor;
		else if(nSelFactor == 4) return A * 0.5 * dDeductFactor * S;
		else if(nSelFactor == 5) return A * 1.0 * dDeductFactor * S;
		else if(nSelFactor == 6) return A * 1.5 * dDeductFactor * S;

		else return 0;
	}
}

BOOL CARcBridgeCalcStd::IsSkew()
{
	CDPoint vAngle = m_pBri->GetAngleJijum(0);

	double AngDegree = Round(vAngle.GetAngleDegree(),3);

	return !Compare(AngDegree,90.0,"=");
}

CDesignPile* CARcBridgeCalcStd::GetDesignPile(long nJijum)
{
	long nSize = m_pArrPileCalc.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CDesignPile	*pCalcPile = m_pArrPileCalc.GetAt(i);
		if(pCalcPile->m_nIndexJijum==nJijum) return pCalcPile;
	}

	return NULL;
}

void CARcBridgeCalcStd::SetCalcPileData(CDesignPile *pCalcPile)
{
	if(pCalcPile==NULL) return;
	long nJijum = pCalcPile->m_nIndexJijum;
	CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting==NULL) return;

	CARcBridgeDataStd* pData = m_pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();
	BOOL bInWall = (nJijum == 0 || nJijum == m_pBri->m_nQtyJigan)? FALSE : TRUE;
	CBoringDataEx *pBoringEx = m_pBri->GetBoringDataExByJijumNumber(nJijum);
	CBoringData   *pBoring   = m_pBri->GetBoringDataJijumBase(nJijum);
	CString szExcelFileName =AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory") + "\\HgExcelBase.dll";

	if(pBoringEx == NULL || pBoring == NULL)
		return;

	EPartOfBridge ePart = bInWall? ePartInFoot : ((nJijum == 0)? ePartFootStt : ePartFootEnd);
	if(m_pBri->IsBoxType())
		ePart = ePartLowerSlab;

	pCalcPile->SetTypeMethodDesign(bLsd? 2 : 0);
	pCalcPile->SetDesingKDS(pData->IsDesignKDS2019() || pData->IsRailDesignKDS2018());
	pCalcPile->SetTypeCheckSteelPile(pData->m_nTypeCheckSteelPile);
	pCalcPile->m_pPile = &pFooting->m_exFooting.m_Pile;
	pCalcPile->m_pPile->SetPileArray();

	pCalcPile->m_pBasicConcInfo			= m_pBri->m_pARcBridgeDataStd->m_pBasicConcInfo;
	pCalcPile->m_pView					= m_pView;
	pCalcPile->m_pARoadOptionStd			= m_pARoadOptionStd;
	pCalcPile->m_bApplyAllowInc			= m_pBri->m_pARcBridgeDataStd->m_bApplyAllowInc; // 말뚝응력 검토시 허용응력 증가계수 적용
	pCalcPile->m_bApplyBoredPileASD			= (m_pBri->m_pARcBridgeDataStd->m_nApplyBoredPileASD == 1);		// 매입말뚝의 허용응력설계법 적용 
	pCalcPile->m_bApplyPhcPileASD			= (m_pBri->m_pARcBridgeDataStd->m_nApplyPhcPileASD == 1);
	pCalcPile->m_EL_Boring					= pBoringEx->m_EL;
	pCalcPile->m_EL_PileStt				= toM(pFooting->m_dEL - pFooting->GetHeight()); 
	pCalcPile->m_dNValue_Hori				= pFooting->m_dNValue_Eo_Hori;
	pCalcPile->m_dNValue_Vert				= pFooting->m_dNValue_Eo_Vert;
	pCalcPile->m_dEo_UserInput				= pFooting->m_dEo_Input_Hori;
	pCalcPile->m_dPileLength				= pCalcPile->m_pPile->m_dLength - 100;
	pCalcPile->m_SupportFactor				= pFooting->m_dFactorPile_qd;
//	pCalcPile->m_dNValueBaseGround			= pBoring->m_nNValue;
//	pCalcPile->m_dFrictionAngleBaseGround	= pFooting->GetFrictionAngleBaseGround(pBoring->m_nNValue);
	pCalcPile->m_dUnitWeigthBaseGround		= pFooting->m_dUnitWeigthBaseGround;
	pCalcPile->m_dC_BaseGround				= pBoring->m_c;
	pCalcPile->m_dLengthPileMax			= pFooting->m_exFooting.m_Pile.m_dLengthPileMax;	/// 입력
	pCalcPile->m_dCoverFooting				= pFooting->m_dCoverLower[0];
	pCalcPile->m_dHeightFooting			= m_pBri->IsBoxType() ? m_pBri->m_dTS_Lower : pFooting->GetHeight();
	pCalcPile->m_dEc_Footing				= m_pBri->m_pARcBridgeDataStd->m_Ec;
	pCalcPile->m_dEs_Footing				= m_pBri->m_pARcBridgeDataStd->m_Es;
	pCalcPile->m_bMinAvCheckHyunTa			= m_pBri->m_bApplyMinAvHyunTa;

	if(bLsd)
	{
		CLsdResistCoefMatCtl *pResistCoefCtl = &(pData->m_plsdEnv->GetLsdResistCoefMatCtl());
		pCalcPile->m_dPI_C					= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_LIMIT_I, eResistCoefMaterial_Concrete);
		pCalcPile->m_dPI_V					= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_LIMIT_I, eResistCoefMaterial_PsSteel);
		pCalcPile->m_dPI_S					= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_LIMIT_I, eResistCoefMaterial_Ferro);
		pCalcPile->m_dPI_C_Extreme			= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_EXTREME_I, eResistCoefMaterial_Concrete);
		pCalcPile->m_dPI_V_Extreme			= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_EXTREME_I, eResistCoefMaterial_PsSteel);
		pCalcPile->m_dPI_S_Extreme			= pResistCoefCtl->GetCoefficient(eLSD_COMBINE_EXTREME_I, eResistCoefMaterial_Ferro);
		//
		pCalcPile->m_nSelectPmin_Hyunta_Extreme = pData->m_nSelectPmin_Hyunta_Extreme;		
	}
	else
	{
		pCalcPile->m_dPI_C = m_pBri->m_pARcBridgeDataStd->m_Pi_C;
		pCalcPile->m_dPI_V = m_pBri->m_pARcBridgeDataStd->m_Pi_V;
	}
	pCalcPile->m_nSeclectPmin = 2;	 // m_pBri->m_nSelectPminColumn 안쓰나?

	pCalcPile->m_Fck				= m_pBri->GetValueFck(ePart);
	pCalcPile->m_Fy					= m_pBri->GetValueFy(ePart, FALSE);
	pCalcPile->m_szFontPrint		= m_pBri->m_pARcBridgeDataStd->m_szFontPrint;
	pCalcPile->m_szFileNameExcel	= szExcelFileName;
	pCalcPile->m_nFontSizePrint		= (long)m_pBri->m_pARcBridgeDataStd->m_dFontSizePrint;
	pCalcPile->m_Ep					= m_pBri->m_pARcBridgeDataStd->m_Es_Steel;

	pCalcPile->m_pBoring = pBoringEx;

	double dAlpha	= pFooting->m_dFactorAlpha[0];
	double dAlphaEq	= pFooting->m_dFactorAlpha[1];

	if(bLsd)
	{
		pCalcPile->m_alpha[0] = dAlpha;
		pCalcPile->m_alpha[1] = dAlpha;
		pCalcPile->m_alpha[2] = dAlphaEq;

		pCalcPile->m_Pa_Calculate[0] = pFooting->m_Pa_Calculate[0];	/// 입력
		pCalcPile->m_Pa_Calculate[1] = pFooting->m_Pa_Calculate[1];	/// 입력
		pCalcPile->m_Pa_Calculate[2] = pFooting->m_Pa_Calculate[2];	/// 입력
		pCalcPile->m_Qa_Calculate[0] = pFooting->m_Qa_Calculate[0];	/// 입력
		pCalcPile->m_Qa_Calculate[1] = pFooting->m_Qa_Calculate[1];	/// 입력
		pCalcPile->m_Qa_Calculate[2] = pFooting->m_Qa_Calculate[2];	/// 입력
	}
	else
	{
		pCalcPile->m_alpha[0] = dAlpha;
		pCalcPile->m_alpha[1] = dAlphaEq;

		pCalcPile->m_Pa_Calculate[0] = pFooting->m_Pa_Calculate[0];	/// 입력
		pCalcPile->m_Pa_Calculate[1] = pFooting->m_Pa_Calculate[1];	/// 입력
		pCalcPile->m_Qa_Calculate[0] = pFooting->m_Qa_Calculate[0];	/// 입력
		pCalcPile->m_Qa_Calculate[1] = pFooting->m_Qa_Calculate[1];	/// 입력
	}

	if(bLsd)
	{
		// Lsd Factor
		long nTypeIntrusionToSupport = pCalcPile->m_pPile->m_nTypeIntrusionToSupport; // 양호,불량

		ECoefficientBaseIndex eIndex[PILE_CASE_COUNT] = {eResistCoefficient_Service, eResistCoefficient_Ultimate, eResistCoefficient_Extreme};
		CLsdResistCoefBaseCtl *pResistCoefCtl = &(pData->m_plsdEnv->GetLsdResistCoefBaseCtl());
		// 	if(pResistCoefCtl && pResistCoefCtl->GetResistCoefSize() >= eRESISTCOEF_MAXCOUNT)
		if(pResistCoefCtl)
		{
			for(long i = 0; i < PILE_CASE_COUNT; i++)
			{
				pCalcPile->sLsdCoef.piC[i] = pResistCoefCtl->GetCoefficient(nTypeIntrusionToSupport == 0 ? eRESISTCOEF_IntruStateGood : eRESISTCOEF_IntruStateBad, eIndex[i]); // // 관입상태 양호
				pCalcPile->sLsdCoef.piF[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Flexure, eIndex[i]);
				pCalcPile->sLsdCoef.piV[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Shear, eIndex[i]);
				//
				pCalcPile->sLsdCoef.piC_AM[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_NonDrivenPile_Pipe, eIndex[i]); // 축압축 + 모멘트
				pCalcPile->sLsdCoef.piF_AM[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_NonDrivenPile_Flexure, eIndex[i]);
				//
				pCalcPile->sLsdCoef.piU_ST[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Type, eIndex[i]); // 인발저항계수
				pCalcPile->sLsdCoef.piU_HT[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Drill, eIndex[i]);
				//
				pCalcPile->sLsdCoef.piR_m[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_AxialCompress_Steel, eIndex[i]);
				pCalcPile->sLsdCoef.piR_s[i] = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePileCompress_Type, eIndex[i]);		
			}
		}
	}
}

double CARcBridgeCalcStd::GetScaleDiagram(long nRow, long nType)
{
	double dHeight = nRow * CXLFormatText::GetRowHeight() * 25/19;
	
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CFEMManage *pFEM;
	if(nType == 0) pFEM = &pBri->m_FemNormal;
	else if(nType == 1) pFEM = &pBri->m_FemEQ[0];
	
	CForceDiagram FDiagram(pFEM);
	FDiagram.SetCWnd(m_pView);
	
	FDiagram.DrawFrameModel(FALSE,FALSE,FALSE);
	
	FDiagram.SetCalcExtRect();
	CDRect rect = FDiagram.GetExtRect();
	double dR  = rect.Width() / rect.Height();			
	long nCWidth = (long)MIN(XFL_PAGEWIDTH,dHeight*dR);	
	
	double dScale = rect.Width() / (double)nCWidth * 3;
	return dScale;
}

// BOOL CARcBridgeCalcStd::IsDistributionLoadSlab(CString strLoad)
// {
// 	if(strLoad=="D")		return TRUE;
// 	if(strLoad=="D-EQ")		return TRUE;
// 	if(strLoad=="VH")		return TRUE;
// 
// 	return FALSE;
// }

CDRect CARcBridgeCalcStd::DrawDiagramModeling(CDomyun *pDomP, CFEMManage *pFEM, BOOL bDrawNLLink)
{
	CDomyun Dom(pDomP);
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	// 부재
	long nSizeEle = pFEM->GetElementSize();
	CDPoint xyI(0,0),xyJ(0,0);
	long i=0; for(i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		CJoint  *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint  *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);
		xyI = CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z);
		xyJ = CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xyI = frM(xyI);
			xyJ = frM(xyJ);
		}

		Dom.LineTo(xyI,xyJ);
	}

	// NLLINK
	if(bDrawNLLink)
	{
		CDPoint vX(1,0), vDir(0,0);
		long nSizeNLLink = pFEM->GetNLLinkSize();
		for(long ix = 0; ix < nSizeNLLink; ix++)
		{
			CNullLLink *pNL = pFEM->GetNLLink((unsigned short)ix);
			CJoint  *pJntI = pFEM->GetJoint(pNL->m_nJStt);
			CJoint  *pJntJ = pFEM->GetJoint(pNL->m_nJEnd);

			xyI = CDPoint(pJntI->m_vPoint.x, pJntI->m_vPoint.z);
			xyJ = CDPoint(pJntJ->m_vPoint.x, pJntJ->m_vPoint.z);
			if(bLsd == FALSE)
			{
				xyI = frM(xyI);
				xyJ = frM(xyJ);
			}
			vDir = (xyI - xyJ).Unit();

			Dom.LineTo(xyI, xyJ - 500 * vDir);
			Dom.LineTo(xyJ - 500 * vDir + 150 * vDir.Rotate90(), xyJ - 500 * vDir - 150 * vDir.Rotate90());
		}
	}

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();

	*pDomP << Dom;

	return rect;
}

void CARcBridgeCalcStd::DimModelingSlab(CDomyun *pDomP, CFEMManage *pFEM, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge, BOOL bUpperSlab)
{
	if(bUpperSlab == FALSE && m_pBri->IsBoxType() == FALSE) return;

	CDomyun Dom(pDomP);

	CDPointArray xyArr;
	CStringArray sArr1,sArr2;

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nFlag = bUpperSlab ? JOINT_UPPERSLAB : JOINT_LOWERSLAB;

	long nSizeJoint = pFEM->GetJointSize();
	CDPoint xy(0,0), xyDimStt(0,0);
	BOOL bFirstJoint = TRUE;
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint  *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]!=nFlag) continue;

		xy = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xy = frM(xy);
		}

		xyArr.Add(xy);
		if(bJointMark)
		{
			if(bUpperSlab)
				Dom.TextOut(xy.x, xy.y + Dom.GetTextHeight()/2,COMMA(pJnt->m_nIdx+1));
			else
				Dom.TextOut(xy.x, xy.y - Dom.GetTextHeight()*1.5,COMMA(pJnt->m_nIdx+1));
			Dom.Circle(xy,Dom.GetScaleDim()*0.75,TRUE);
		}
		if(bFirstJoint)
		{
			xyDimStt = xy;
			bFirstJoint = FALSE;
		}
		if(bUpperSlab) 
		{
			xyDimStt.y = max(xyDimStt.y, xy.y);
		}
	}

	BOOL bBlank = FALSE;
	long nSizeEle = pFEM->GetElementSize();
	long nSizeEleSlab = 0;

	nFlag = bUpperSlab ? ELEMENT_UPPERSLAB : ELEMENT_LOWERSLAB;
	for(i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag!=nFlag) continue;
		nSizeEleSlab++;
	}

	double sw = bUpperSlab ? 1 : -1;
	CDPoint xyI(0,0),xyJ(0,0),vY(0,1), xyPre(0, 0), xyCur(0, 0);
	for(i=0; i<nSizeEle; i++)
	{
		if(!bEleMark) break;
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag!=nFlag) continue;
		CJoint  *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint  *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);
		xyI = CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z);
		xyJ = CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xyI = frM(xyI);
			xyJ = frM(xyJ);
		}
		xyI += (Dom.GetTextHeight() + Dom.Always(3)) * sw * vY;
		xyJ += (Dom.GetTextHeight() + Dom.Always(3)) * sw * vY;
		
		if(bMerge && i!=0 && i!=nSizeEleSlab-1 && fabs(xyI.x-xyJ.x)<600)
		{
			bBlank = TRUE;
		}
		else
		{
			xyCur.x = (xyI.x+xyJ.x)/2;
			xyCur.y = (xyI.y+xyJ.y)/2;
			if(bBlank) Dom.TextOut((xyPre.x+xyCur.x)/2, (xyPre.y+xyCur.y)/2, "∼");
			Dom.TextCircle(xyCur.x, xyCur.y, COMMA(pEle->m_nIndex+1), 3, FALSE, TRUE);
			xyPre.x = (xyI.x+xyJ.x)/2;
			xyPre.y = (xyI.y+xyJ.y)/2;
			bBlank = FALSE;
		}
	}

	if(bDim)
	{
		long nDimDan = 0;
		double dMinMargine = -1;
		CHgBaseDrawStd DrawStd;
		DrawStd.AutoDimCont(&Dom,xyDimStt,xyArr,sArr1,sArr2,nDimDan, bUpperSlab ? _T("TOP") : _T("BOTTOM"),dMinMargine,FALSE,TRUE,TRUE,FALSE);
	}

	*pDomP << Dom;
}

void CARcBridgeCalcStd::DimModelingWall(CDomyun *pDomP, CFEMManage *pFEM, long nJijum, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge)
{
	CDomyun Dom(pDomP);

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nJijumStt = (nJijum!=-1) ? nJijum : 0;
	long nJijumEnd = (nJijum!=-1) ? nJijum : m_pBri->m_nQtyJigan;

	long nJ=0; for(nJ=nJijumStt; nJ<=nJijumEnd; nJ++)
	{
		CDPointArray xyArr;
		CStringArray sArr1,sArr2;

		BOOL bLeftDir = (nJ==0) ? TRUE : FALSE;

		long nSizeJoint = pFEM->GetJointSize();
		CDPoint xy(0,0);
		long i=0; for(i=0; i<nSizeJoint; i++)
		{
			CJoint  *pJnt = pFEM->GetJoint((USHORT)i);
			if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]!=nJ) continue;

			xy = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
			if(bLsd == FALSE)
			{
				xy = frM(xy);
			}

			xyArr.Add(xy);
			if(bJointMark && pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT]==FALSE)
			{
				if(bLeftDir)
					Dom.TextOut(xy.x-Dom.GetTextHeight(),xy.y,COMMA(pJnt->m_nIdx+1));
				else
					Dom.TextOut(xy.x+Dom.GetTextHeight(),xy.y,COMMA(pJnt->m_nIdx+1));
				Dom.Circle(xy,Dom.GetScaleDim()*0.75,TRUE);
			}
		}

		BOOL bBlank = FALSE;
		long nSizeEle = pFEM->GetElementSize();
		long nSizeEleWall = 0;
		long nCnt = 0;
		
		for(i=0; i<nSizeEle; i++)
		{
			CElement *pEle = pFEM->GetElement((USHORT)i);
			if(pEle->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJ)
				nSizeEleWall++;
		}

		CDPoint xyI(0,0),xyJ(0,0),vX(1,0), xyPre(0, 0), xyCur(0, 0);
		for(i=0; i<nSizeEle; i++)
		{
			if(!bEleMark) break;
			CElement *pEle = pFEM->GetElement((USHORT)i);
			if(pEle->m_nFlag==ELEMENT_UPPERSLAB) continue;
			if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO]!=nJ) continue;
			nCnt++;

			CJoint  *pJntI = pFEM->GetJoint(pEle->m_nJStt);
			CJoint  *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);

			xyI = CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z);
			xyJ = CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z);
			if(bLsd == FALSE)
			{
				xyI = frM(xyI);
				xyJ = frM(xyJ);
			}

			if(bLeftDir)
			{
				xyI -= (Dom.GetTextHeight() + Dom.Always(3))*vX;
				xyJ -= (Dom.GetTextHeight() + Dom.Always(3))*vX;
			}
			else
			{
				xyI += (Dom.GetTextHeight() + Dom.Always(3))*vX;
				xyJ += (Dom.GetTextHeight() + Dom.Always(3))*vX;
			}

			if(bMerge && nCnt!=1 && nCnt!=nSizeEleWall && fabs(xyI.y-xyJ.y)<600)
			{
				bBlank = TRUE;
			}
			else
			{
				xyCur.x = (xyI.x+xyJ.x)/2;
				xyCur.y = (xyI.y+xyJ.y)/2;
				if(bBlank) Dom.TextOut((xyPre.x+xyCur.x)/2, (xyPre.y+xyCur.y)/2, "∼");
				Dom.TextCircle((xyI.x+xyJ.x)/2,(xyI.y+xyJ.y)/2,COMMA(pEle->m_nIndex+1),3,FALSE,TRUE);
				xyPre.x = (xyI.x+xyJ.x)/2;
				xyPre.y = (xyI.y+xyJ.y)/2;
				bBlank = FALSE;
			}
		}

		// 시종점벽체만 치수를 기입한다...
		if(bDim && m_pBri->IsOutsideWall() && (nJ==0 || nJ==m_pBri->m_nQtyJigan))
		{
			xyArr.Sort(FALSE);
			CDPoint xyStt = xyArr.GetAt(0);
			long nDimDan = 1;
			double dMinMargine = 0;
			CHgBaseDrawStd DrawStd;
			DrawStd.AutoDimCont(&Dom,xyStt,xyArr,sArr1,sArr2,nDimDan,bLeftDir?"LEFT":"RIGHT",dMinMargine,FALSE,TRUE,TRUE,FALSE);
		}
	}

	*pDomP << Dom;
}

void CARcBridgeCalcStd::DrawModelingSpringAndFix(CDomyun *pDomP, CFEMManage *pFEM)
{
	if(m_pBri->IsBoxType())
	{
		DrawModelingSpringAndNLLink(pDomP, pFEM);
		return;
	}
	CDomyun Dom(pDomP);

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nSizeJoint = pFEM->GetJointSize();
	CDPoint xy(0,0);

	BOOL bUseHCAD = CHgBaseDrawStd::IsUseHCadBlock();
	CHgBaseDrawStd::SetUseHCadBlock(FALSE);
	double Scale = Dom.GetScaleDim();
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint  *pJnt = pFEM->GetJoint((USHORT)i);
		if(!pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] && pJnt->m_vSpring.IsNull() && pJnt->m_vSpringR.IsNull()) continue;
		
		xy = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xy = frM(xy);
		}
		
		if(pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX])
		{
			// 기초하단의 고정단
			CHgBaseDrawStd::InsertHCadBlock(&Dom, "FIXMARK", xy, Scale, 0, TRUE);
			Dom.LineTo(xy.x-12.867*0.5*Scale,xy.y,xy.x+12.867*0.5*Scale,xy.y); // 12.867 -> 심볼의 길이 HCAD_SYMBOL
		}
		else if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING])
		{
			// 기초하단의 스프링
			CDomyun DomBlock(pDomP);
			CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns1, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns1, "TEXT2", "");
			Dom << DomBlock;

			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns2, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns2, "TEXT2", "");
			DomBlock.Rotate(xy,CDPoint(cos(-ConstPi*0.5),sin(-ConstPi*0.5)));
			Dom << DomBlock;

			CObInsert *pIns3 = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale,0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns3, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns3, "TEXT2", "");
			DomBlock.Rotate(xy,CDPoint(cos(ConstPi*0.75),sin(ConstPi*0.75)));
			Dom << DomBlock;
		}
		else if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==0)
		{
			// 시점측벽체 측벽스프링
			CDomyun DomBlock(pDomP);
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns, "TEXT2", "");
			DomBlock.Rotate(ConstPi*1.5,xy.x,xy.y);
			Dom << DomBlock;
		}
		else
		{
			// 종점측벽체 측벽스프링
			CDomyun DomBlock(pDomP);
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns, "TEXT2", "");
			DomBlock.Rotate(xy,CDPoint(cos(ConstPi*0.5),sin(ConstPi*0.5)));
			Dom << DomBlock;
		}

	}
	*pDomP << Dom;

	CHgBaseDrawStd::SetUseHCadBlock(bUseHCAD);
}

void CARcBridgeCalcStd::DrawModelingSpringAndNLLink(CDomyun *pDomP, CFEMManage *pFEM)
{
	if(m_pBri->IsBoxType() == FALSE) return;
	CDomyun Dom(pDomP);

	long nSizeJoint = pFEM->GetJointSize();
	CDPoint xy(0,0);

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bUseHCAD = CHgBaseDrawStd::IsUseHCadBlock();
	CHgBaseDrawStd::SetUseHCadBlock(FALSE);
	double Scale = Dom.GetScaleDim();
	for(long i=0; i<nSizeJoint; i++)
	{
		CJoint  *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] == FALSE) continue;
		if(pJnt->m_vSpring.IsNull() && pJnt->m_vSpringR.IsNull()) continue;

		xy = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xy = frM(xy);
		}

		// 기초하단의 스프링
		if(pJnt->m_vSpring.z != 0)
		{
			CDomyun DomBlock(pDomP);
			CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns1, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns1, "TEXT2", "");
			Dom << DomBlock;
		}

		if(pJnt->m_vSpring.x != 0)
		{
			CDomyun DomBlock(pDomP);
			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(&DomBlock, "HGSPRING", xy, Scale, 0, FALSE);
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns2, "TEXT1", "");
			CHgBaseDrawStd::SetAttrib(&DomBlock, pIns2, "TEXT2", "");
			DomBlock.Rotate(xy,CDPoint(cos(-ConstPi*0.5),sin(-ConstPi*0.5)));
			Dom << DomBlock;
		}
	}
	*pDomP << Dom;

	CHgBaseDrawStd::SetUseHCadBlock(bUseHCAD);
}

double CARcBridgeCalcStd::GetRebarAreaSuportBar(CConcBasic *pConc, BOOL bTension, BOOL bCompression, BOOL bUnitCm2)
{
	double dAreaRebar = 0;
	for(long nDan=0; nDan<MAX_REBAR_DAN; nDan++)
	{
		if(bTension && (pConc->m_TSupportBarDia[nDan] != 0))
		{
			dAreaRebar += GetRebarArea_Fix(pConc->m_TSupportBarDia[nDan],1000.0/pConc->m_TSupportBarCTC[nDan]);
		}
		if(bCompression && (pConc->m_CSupportBarDia[nDan] != 0))
		{
			dAreaRebar += GetRebarArea_Fix(pConc->m_CSupportBarDia[nDan],1000.0/pConc->m_CSupportBarCTC[nDan]);
		}
	}

	return bUnitCm2 ? dAreaRebar : frCm2(dAreaRebar); // cm2
}

// bUpperSlab = TRUE  : 상부슬래브 분포하중
// bUpperSlab = FALSE : 하부슬래브 분포하중 : 연직수압(부력)
void CARcBridgeCalcStd::DrawDiagramSlabDistribution(CDomyun *pDomP, CFEMManage *pFEM, double dLoadValue, double dOffset, double dLen, BOOL bUpperSlab, BOOL bHeightHalf, BOOL bAll)
{
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(bUpperSlab == FALSE && pBri->IsBoxType() ==FALSE) return;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJointStt = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabStt(pFEM, bUpperSlab))->m_nJStt;
	long nIdxJointEnd = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabEnd(pFEM, bUpperSlab))->m_nJEnd;

	BOOL bLoadDirUp = ((bUpperSlab && dLoadValue > 0) || (!bUpperSlab && dLoadValue < 0));
	CDomyun Dom(pDomP);

	CJoint *pJnt1 = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJnt2 = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyStt = CDPoint(pJnt1->m_vPoint.x, pJnt1->m_vPoint.z);
	CDPoint xyEnd = CDPoint(pJnt2->m_vPoint.x, pJnt2->m_vPoint.z);

	if(bUpperSlab == FALSE && bAll == FALSE)
	{
		long nIdxJointStt2 = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabStt(pFEM, TRUE))->m_nJStt;
		long nIdxJointEnd2 = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabEnd(pFEM, TRUE))->m_nJEnd;
		CJoint *pJnt1_2 = pFEM->GetJoint((USHORT)nIdxJointStt2);
		CJoint *pJnt2_2 = pFEM->GetJoint((USHORT)nIdxJointEnd2);
		CDPoint xyStt2 = CDPoint(pJnt1_2->m_vPoint.x, pJnt1_2->m_vPoint.z);
		CDPoint xyEnd2 = CDPoint(pJnt2_2->m_vPoint.x, pJnt2_2->m_vPoint.z);

		GetXyMatchSegAndLine(xyStt, xyEnd, xyStt2, CDPoint(0,1), xyStt);
		GetXyMatchSegAndLine(xyStt, xyEnd, xyEnd2, CDPoint(0,1), xyEnd);
	}

	if(bLsd == FALSE)
	{
		xyStt = frM(xyStt);
		xyEnd = frM(xyEnd);
	}

	double Scale = Dom.GetScaleDim();
	CDPoint vX(1,0), vY(0,1);
	if(dOffset > 0 && ~(xyEnd-xyStt) > dOffset)
	{
		xyStt += dOffset*vX;
	}
	if(dLen > 0 && dLen < ~(xyEnd-xyStt))
	{
		xyEnd = xyStt + dLen*vX;
	}
	CDPoint xy(0,0);
	double dLenVert = bLoadDirUp ? 1500 : -1500;
	if(bHeightHalf)
		dLenVert /= 2;

	long nDivide = (long)(~(xyEnd-xyStt)/1000.0);
	long i=0; for(i=0; i <= nDivide; i++)
	{
		xy = xyStt + ((xyEnd.x - xyStt.x)/(double)nDivide)*i*vX + ((xyEnd.y - xyStt.y)/(double)nDivide)*i*vY;

		CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, bLoadDirUp ? _T("HGMODARD") : _T("HGMODARU"), xy, Scale/1.5, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "");
		Dom.LineTo(xy.x, xy.y, xy.x, xy.y+dLenVert);
	}

	CString str = _T("");
	if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
		str.Format("W = %.6f %s", fabs(dLoadValue),pData->m_szTonf_M);
	else
		str.Format("W = %.3f %s", fabs(dLoadValue),pData->m_szTonf_M);

	if(bLoadDirUp)
		Dom.TextOut((xyStt.x+xyEnd.x)/2, xyStt.y + dLenVert + Dom.Always(1), str);
	else
		Dom.TextOut((xyStt.x+xyEnd.x)/2, xyStt.y + dLenVert - Dom.GetTextHeight() - Dom.Always(1), str);
	Dom.LineTo(xyStt.x, xyStt.y + dLenVert, xyEnd.x, xyEnd.y + dLenVert);

	*pDomP << Dom;
}

double CARcBridgeCalcStd::GetThickArchRib(double dSta)
{
	CRcBridgeRebar *pBri = m_pBri;
	double dThick = 0;

	double dThickCrown = Round(Round(pBri->GetThickArchRib(0, TRUE), 0), -1);
	double dDistStt = fabs(pBri->m_tvArrVertSectionRibCen.GetAt(0).m_v1.x - pBri->m_pbRibCen.m_xyCrown.x);
	double dDistEnd = fabs(pBri->m_tvArrVertSectionRibCen.GetAt(pBri->m_tvArrVertSectionRibCen.GetSize()-1).m_v2.x - pBri->m_pbRibCen.m_xyCrown.x);

	if((dDistStt/dThickCrown)<1.5 || (dDistEnd/dThickCrown)<1.5)
	{
		dThick = dThickCrown;
	}
	else
	{
		double dDist = fabs(dSta - pBri->m_pbRibCen.m_xyCrown.x);
		dThick = Round(Round(pBri->GetThickArchRib(dDist, TRUE), 0), -1);
	}

	return dThick;
}

CDPoint CARcBridgeCalcStd::GetAngleArchRibBySta(double dSta)
{
	CRcBridgeRebar *pBri = m_pBri;
	CDPoint vAng(0, 0);

	CTwinVectorArray tvArrCenter;
	CTwinVector tv;

	tvArrCenter = pBri->m_tvArrVertSectionRibCen;

	double dDist = fabs(dSta - pBri->m_pbRibCen.m_xyCrown.x);
	vAng = pBri->m_pbRibCen.GetXyDirTanByDist(-dDist);

	return vAng;
}

void CARcBridgeCalcStd::DimModelingArchRib(CDomyun *pDomP, CFEMManage *pFEM, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge)
{
	CDomyun Dom(pDomP);
	
	CDPointArray xyArr;
	CStringArray sArr1,sArr2;
	
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nSizeJoint = pFEM->GetJointSize();
	CDPoint xy(0,0);
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint  *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]!=JOINT_ARCHRIB) continue;
		
		xy = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xy = frM(xy);
		}

		xyArr.Add(xy);
		if(bJointMark)
		{
			Dom.TextOut(xy.x,xy.y+Dom.GetTextHeight()/2,COMMA(pJnt->m_nIdx+1));
			Dom.Circle(xy,Dom.GetScaleDim()*0.75,TRUE);
		}
	}
	
	BOOL bBlank = FALSE;
	long nSizeEle = pFEM->GetElementSize();
	long nSizeEleRib = 0;
	
	for(i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag!=ELEMENT_UPPERSLAB) break;
		nSizeEleRib++;
	}
	
	CDPoint xyI(0,0),xyJ(0,0),vY(0,1), xyPre(0, 0), xyCur(0, 0);
	BOOL bFirst = TRUE;
	long nJntNumStt = 0;
	for(i=0; i<nSizeEle; i++)
	{
		if(!bEleMark) break;
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag!=ELEMENT_ARCHRIB) continue;
		CJoint  *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint  *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);
		xyI = CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z);
		xyJ = CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xyI = frM(xyI);
			xyJ = frM(xyJ);
		}
		xyI += (Dom.GetTextHeight() + Dom.Always(3))*vY;
		xyJ += (Dom.GetTextHeight() + Dom.Always(3))*vY;
		
		if(bMerge && !bFirst && i!=nSizeEleRib-1 && fabs(xyI.x-xyJ.x)<600)
		{
			bBlank = TRUE;
		}
		else
		{
			xyCur.x = (xyI.x+xyJ.x)/2;
			xyCur.y = (xyI.y+xyJ.y)/2;
			if(bBlank) Dom.TextOut((xyPre.x+xyCur.x)/2, (xyPre.y+xyCur.y)/2, "∼");
			Dom.TextCircle(xyCur.x, xyCur.y, COMMA(pEle->m_nIndex+1), 3, FALSE, TRUE);
			xyPre.x = (xyI.x+xyJ.x)/2;
			xyPre.y = (xyI.y+xyJ.y)/2;
			bBlank = FALSE;
		}
		if(bFirst)
		{
			nJntNumStt = pEle->m_nJStt;
			bFirst = FALSE;
		}
	}
	
	if(bDim)
	{
		CJoint *pJnt  = pFEM->GetJoint((USHORT)nJntNumStt);
		CDPoint xyStt = CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z);
		if(bLsd == FALSE)
		{
			xyStt = frM(xyStt);
		}
		long nDimDan = 0;
		double dMinMargine = -1;
		CHgBaseDrawStd DrawStd;
		DrawStd.AutoDimCont(&Dom,xyStt,xyArr,sArr1,sArr2,nDimDan,"BOTTOM",dMinMargine,FALSE,TRUE,TRUE,FALSE);
	}
	
	*pDomP << Dom;
}

double CARcBridgeCalcStd::GetLenSpanMaxOut(BOOL bVert)
{
	CRcBridgeRebar *pBri = m_pBri;
	
	double dMaxLen = 0;
	long nSpan = pBri->m_nQtyJigan;
	long i=0; for(i=0; i<nSpan; i++)
	{
		double dLenCul = 0;
		double dThickWall1 = (i==0 || i==nSpan)? pBri->GetThickWall(i) : pBri->GetThickWall(i)/2;
		double dThickWall2 = (i+1==0 || i+1==nSpan)? pBri->GetThickWall(i+1) : pBri->GetThickWall(i+1)/2;
		double dAngle  = bVert? 1 : pBri->GetAngleJijum(i).y;
		dLenCul += dThickWall1 / dAngle;
		dLenCul += dThickWall2 / dAngle;
		dLenCul += pBri->GetLengthJigan(i, 0, bVert);
		dMaxLen = MAX(dLenCul, dMaxLen);
	}

	return dMaxLen;
}

double CARcBridgeCalcStd::GetElDiffModelingEnd()
{
	double dDiff = 0;

	double dHStt = toM(m_pFrame->m_vArrSlab[iUPPER].GetAt(0).z);
	double dHEnd = toM(m_pFrame->m_vArrSlab[iUPPER].GetAt(m_pFrame->m_vArrSlab[iUPPER].GetSize()-1).z);

	dDiff = dHEnd - dHStt;

	return dDiff;
}

void CARcBridgeCalcStd::SetDomyunScale( CDomyun *pDomP, long nRows )
{
	pDomP->SetCalcExtRect();
	CDRect rect = pDomP->GetExtRect();		
	double dR  = rect.Width() / rect.Height();	

	double dHeight = nRows * CXLFormatText::GetRowHeight() * 25/19;
	long nCWidth = (long)(MIN(XFL_PAGEWIDTH,dHeight*dR));	

	double dScale = rect.Width() / (double)nCWidth * 3;
	
	pDomP->SetScaleDim(dScale);
	return;
}

void CARcBridgeCalcStd::WriteCalcCifficientKo( long nTypeMethod, CXLFormatText *XLText, long nIdxTitle, long nColStt, BOOL bTitle/*=FALSE*/ )
{
	XLText->GoPosition(NO_MOVE,nColStt);
	if(bTitle)
	{
		CString szTitle(_T(""));

		switch(nTypeMethod)
		{
		case 0:	szTitle = _T("정지토압");	break;
		case 1:	szTitle = _T("RANKINE");	break;
		case 2:	szTitle = _T("Coulomb");	break;
		default:
			break;
		}

		XLText->AddFormatText("$n$h %d)$r토압계수 (%s)$n",nIdxTitle, szTitle);
	}

	double Ko = GetCofficientForAlwaysLiveLoad(nTypeMethod);

	if(nTypeMethod == 0)
	{
		XLText->AddFormatText("$h$rKo$r$c=$r1.0 - sinΦ = 1.0 - sin%.1f˚ = %.3f$n", m_pBri->m_dAngleFrictionBackFill,Ko);
	}
	else
	{
		CString szObj = (nTypeMethod == 1)? _T("RANKINE주동") : _T("평상시주동토압계수");
		XLText->AddFormatText("$h$r$r$r$p[%s]$n", szObj);
		XLText->AddFormatText("$h$r$p[K_A]$r$c=$n$n");
		XLText->AddFormatText("$h$r$r$c=$r'%.3f$n",Ko);
		XLText->AddFormatText("$h$m+02여기서,$r$r$cΦ$r$c=$r뒷채움 흙의 내부마찰각( = %.2f 도)$n", m_pBri->m_dAngleFrictionBackFill);
		XLText->AddFormatText("$h$m+04$cα$r$c=$r지표면과 수평면이 이루는 각( = 0 도)$n");
		XLText->AddFormatText("$h$m+04$cβ$r$c=$r벽배면과 연직면이 이루는 각( = 0 도)$n");
		XLText->AddFormatText("$h$m+04$cδ$r$c=$r벽배면과 뒷채움흙사이의 벽면마찰각( = 0 도)$n");
	}
}

double CARcBridgeCalcStd::GetCofficientForAlwaysLiveLoad(long nTypeMethod)
{
	double dK0(0);

	const long nType = nTypeMethod < 0 ? m_pBri->m_nSelectKindCoefficientKo : nTypeMethod;
	switch(nType)
	{
	case 0 :					// 정지토압
		dK0 = m_dCoefficientKo;	break;
	case 1:						// RANKINE
		dK0	= m_dCoefficient_Rankine;	break;
	case 2:
		dK0	= m_dCoefficient_Coulomb;	break;
	default:
		break;
	}

	return dK0;
}

void CARcBridgeCalcStd::MakeStmFrameSection(CStmModel *pStm)
{
	pStm->ClearSection();

	SStmPropSection sSection;
	sSection.id_material = 0;
	sSection.area = 1.0E-10;
	sSection.inertia22 = 1.0E-10;
	sSection.inertia33 = 1.0E-10;
	sSection.torsion = 1.0E-10;
	sSection.area_shear22 = 1.0E-10;
	sSection.area_shear33 =  1.0E-10;

	SStmPropElement sElement;
	sElement.id_section = 0;
	sElement.ic_segment = 2;
	sElement.angle = 0.0;

	// element 생성이전에는 1개의 Frame Section만 생성, element 생성시 필요
	CString strSection = _T("FSEC");
	if(pStm->GetCountElement() == 0)
	{
		pStm->AddSection(strSection, sSection);
		std::map<HGINT32,CStmObjElement*>::iterator posElement;
		std::map<HGINT32,CStmObjElement*> mpEle = pStm->GetElements();
		for(posElement = mpEle.begin(); posElement != mpEle.end(); ++posElement)
		{
			if(!posElement->second->IsEnable()) continue;
			posElement->second->SetProperty(sElement);
		}
		return;
	}

	// element 생성이후에는 부재 개수만큼 Frame Section을 생성
	// STAD 2D 해석결과가 있는 경우는 해석결과를 적용 : 
	long nIdSection = 0;
	double dMinArea = 0.00000001;
	double dAxialForce = 0;
	std::map<HGINT32,CStmObjElement*>::iterator posElement;
	std::map<HGINT32,CStmObjElement*> mpEle = pStm->GetElements();
	for(posElement = mpEle.begin(); posElement != mpEle.end(); ++posElement)
	{
		if(!posElement->second->IsEnable()) continue;

		dAxialForce = posElement->second->GetResultAxialForce(0);
		strSection.Format(_T("SEC%02d"), posElement->second->GetId() + 1);

		sSection.id_material = dAxialForce > 0 ? 2 : 1;
		sSection.area = posElement->second->GetAreaSection();
		if(fabs(sSection.area) < dMinArea)
		{
			sSection.area = dMinArea;
			sSection.id_material = 0;
		}

		pStm->AddSection(strSection, sSection);
		sElement.id_section = nIdSection++;
		posElement->second->SetProperty(sElement);
	}
}

void CARcBridgeCalcStd::DrawDiagramProtectionRiseFooting( CDomyun *pDomP, CFEMManage *pFEM, double dLoadValueLeft, double dLoadValueRight, double dLen/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) return;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJointStt = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabStt(pFEM, TRUE))->m_nJStt;
	long nIdxJointEnd = pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabEnd(pFEM, TRUE))->m_nJEnd;
	CJoint *pJntStt = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyUpperStt = bLsd? CDPoint(pJntStt->m_vPoint.x, pJntStt->m_vPoint.z) : frM(CDPoint(pJntStt->m_vPoint.x, pJntStt->m_vPoint.z));
	CDPoint xyUpperEnd = bLsd? CDPoint(pJntEnd->m_vPoint.x, pJntEnd->m_vPoint.z) : frM(CDPoint(pJntEnd->m_vPoint.x, pJntEnd->m_vPoint.z));

	pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabStt(pFEM, FALSE))->m_nJStt;
	pFEM->GetElement((USHORT)m_pFrame->GetNumberElementSlabEnd(pFEM, FALSE))->m_nJEnd;

	CDomyun Dom(pDomP);

	double dLenVert = dLen;
	double Scale = Dom.GetScaleDim();
	CDPoint xyStt(0,0), xyEnd(0,0), vX(1,0);
	for(long ix = 0; ix < pFEM->GetElementSize(); ix++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)ix);
		if(pEle->m_nFlag != ELEMENT_LOWERSLAB) continue;
		pJntStt = pFEM->GetJoint((USHORT)pEle->m_nJStt);
		pJntEnd = pFEM->GetJoint((USHORT)pEle->m_nJEnd);
		xyStt = bLsd? CDPoint(pJntStt->m_vPoint.x, pJntStt->m_vPoint.z) : frM(CDPoint(pJntStt->m_vPoint.x, pJntStt->m_vPoint.z));
		xyEnd = bLsd? CDPoint(pJntEnd->m_vPoint.x, pJntEnd->m_vPoint.z) : frM(CDPoint(pJntEnd->m_vPoint.x, pJntEnd->m_vPoint.z));
		if(Compare(xyEnd.x , xyUpperStt.x, _T("<=")) || Compare(xyStt.x , xyUpperEnd.x, _T(">="))) 
		{
			long nDivide = 2;
			for(long kk=0; kk <= nDivide; kk++)
			{
				CDPoint xy = xyStt + ((xyEnd.x - xyStt.x)/(double)nDivide)*kk*vX;

				CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, _T("HGMODARD"), xy, Scale/1.5, 0, TRUE);
				CHgBaseDrawStd::SetAttrib(&Dom, pIns, "RB_NO", "");
				Dom.LineTo(xy.x, xy.y, xy.x, xy.y + dLenVert);
			}

			Dom.LineTo(xyStt.x, xyStt.y + dLenVert, xyEnd.x, xyEnd.y + dLenVert);
			if(Compare(xyEnd.x , xyUpperStt.x, _T("=")) || Compare(xyStt.x , xyUpperEnd.x, _T("="))) 
			{
				BOOL bLeft = Compare(xyEnd.x , xyUpperStt.x, _T("=")) ? TRUE : FALSE;
				CString str = _T("");
				str.Format("W = %s %s", pData->GetStrDouble(bLeft? dLoadValueLeft : dLoadValueRight, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
				Dom.TextOut((xyStt.x + xyEnd.x)/2, xyStt.y + dLenVert + Dom.Always(1), str);
			}
		}
	}

	*pDomP << Dom;
}

void CARcBridgeCalcStd::DrawDiagramJointLoad( CDomyun *pDomP, CFEMManage *pFEM, long nJoint, CVector vLoad, BOOL bMoment )
{
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bLsd = pData->IsLsdDesign();
	CDomyun Dom(pDomP);
	double Scale = Dom.GetScaleDim();

	long nDec03 = bLsd ? 0 : 3;
	CString str(_T(""));

	CJoint *pJnt = pFEM->GetJoint((USHORT)nJoint);
	CDPoint xyPos = CDPoint(pJnt->m_vPoint.x, pJnt->m_vPoint.z);
	if(bLsd == FALSE)
	{
		xyPos = frM(xyPos);
	}

	if(bMoment)
	{
		// Moment
		if(vLoad.x != 0)
		{
			str.Format(_T("%.3f"), vLoad.x);
			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(&Dom, _T("HGMODMOMENT2"), xyPos, Dom.GetScaleDim(), 0, TRUE);	
			CHgBaseDrawStd::SetAttrib(&Dom, pIns2, _T("RB_NO"), str);
		}
		// Moment
		if(vLoad.y != 0)
		{
			str = GetStrDouble(fabs(vLoad.y), nDec03, bLsd);
			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(&Dom, (vLoad.y>0)? _T("HGMODMOMENT") : _T("HGMODMOMENTR"), xyPos, Dom.GetScaleDim(), 0, TRUE);	
			CHgBaseDrawStd::SetAttrib(&Dom, pIns2, _T("RB_NO"), str);
		}
		// Moment
		if(vLoad.z != 0)
		{
			str = GetStrDouble(vLoad.z, nDec03, bLsd);
			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(&Dom, _T("HGMODMOMENT"), xyPos, Dom.GetScaleDim(), 0, TRUE);	
			CHgBaseDrawStd::SetAttrib(&Dom, pIns2, _T("RB_NO"), str);
		}
	}
	else
	{
		// x축
		if(vLoad.x != 0)
		{
			str = GetStrDouble(vLoad.x, nDec03, bLsd);
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, (vLoad.x > 0)? _T("HGMODARR"):_T("HGMODARL"), xyPos, Scale, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, _T("RB_NO"), str);
		}
		// y축
		if(vLoad.y != 0)
		{
			str = GetStrDouble(vLoad.y, nDec03, bLsd);
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, _T("HGMODARS"), xyPos, Scale, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, _T("RB_NO"), str);
		}
		// z축
		if(vLoad.z != 0)
		{
			str = GetStrDouble(vLoad.z, nDec03, bLsd);
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, (vLoad.z > 0)? _T("HGMODARU"):_T("HGMODARD"), xyPos, Scale, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, _T("RB_NO"), str);
		}
	}

	*pDomP << Dom;
}

void CARcBridgeCalcStd::DrawDiagramElementLoad( CDomyun *pDomP, CFEMManage *pFEM, long nElement, long nDir, long nTypeLoad, double dDist1, double dDist2, double dLoad1, double dLoad2 )
{
	CRcBridgeRebar *pBri = m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CElement *pEle = pFEM->GetElement((unsigned short)nElement);
	if(pEle == NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	CDomyun Dom(pDomP);
	double Scale = Dom.GetScaleDim();

	CString str(_T(""));
	long nIdxJointStt = pEle->m_nJStt;
	long nIdxJointEnd = pEle->m_nJEnd;

	double dDistMove = Dom.Always(10.0);
	CString strBlock = (nDir==0)? ((dLoad1 > 0)? _T("HGMODARR"):_T("HGMODARL")) : ((nDir==1)? _T("HGMODARS"):_T("HGMODARD"));
	CJoint *pJnt1 = pFEM->GetJoint((unsigned short)nIdxJointStt);
	CJoint *pJnt2 = pFEM->GetJoint((unsigned short)nIdxJointEnd);
	CDPoint xyStt(0,0), xyEnd(0,0);
	if(bLsd)
	{
		xyStt = CDPoint(pJnt1->m_vPoint.x,pJnt1->m_vPoint.z);
		xyEnd = CDPoint(pJnt2->m_vPoint.x,pJnt2->m_vPoint.z);
	}
	else
	{
		xyStt = frM(CDPoint(pJnt1->m_vPoint.x,pJnt1->m_vPoint.z));
		xyEnd = frM(CDPoint(pJnt2->m_vPoint.x,pJnt2->m_vPoint.z));
	}

	CDPoint xyDir = (xyEnd-xyStt).Unit();
	CDPoint ixy(0, 0);
	long nDec03 = bLsd ? 3 : 3;
	if(nTypeLoad == 0)		// 집중하중
	{
		ixy = bLsd ? xyStt + frM(dDist1) * xyDir : xyStt + dDist1 * xyDir;
		str = GetStrDouble(dLoad1, nDec03, bLsd);
		CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, strBlock, ixy, Scale, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, _T("RB_NO"), str);
	}
	else if(nTypeLoad == 1)		// 분포하중
	{
		// (ARCBRIDGE-3449) Dist는 0~1인 상대거리다. 계산해서 넣어줘야 겠다.
		double dLenEle = ~(xyEnd - xyStt);
		dDist1 *= dLenEle;
		dDist2 *= dLenEle;

		long nLoadDir = (nDir!=0 || dLoad1 > 0)? 1 : -1;
		if(nDir == 0)
		{
			Dom.LineTo(xyStt.x-dDistMove*nLoadDir, xyStt.y, xyEnd.x-dDistMove*nLoadDir, xyEnd.y);
		}
		else if(nDir == 1)
		{
			Dom.LineTo(xyStt.x-dDistMove * cos(ConstPi/4),xyStt.y-dDistMove*sin(ConstPi/4),
				xyEnd.x-dDistMove * cos(ConstPi/4),xyEnd.y-dDistMove*sin(ConstPi/4));
		}
		else
		{
			Dom.LineTo(xyStt.x, xyStt.y+dDistMove, xyEnd.x, xyEnd.y+dDistMove);
		}

		double dOffset = bLsd ? dDist1 : frM(dDist1);
		double dLen = bLsd ? (dDist2 - dDist1) : frM(dDist2 - dDist1);
		if(dOffset > 0 && ~(xyEnd-xyStt) > dOffset)
		{
			xyStt += dOffset*xyDir;
		}
		if(dLen > 0 && dLen < ~(xyEnd-xyStt))
		{
			xyEnd = xyStt + dLen*xyDir;
		}

		double dDivide = MAX(1, Round(~(xyEnd-xyStt)/1000.0, 0));
		long i=0; for(i=0; i<=dDivide; i++)
		{
			CDPoint xy = xyStt + (~(xyEnd-xyStt)/(double)dDivide)*i*xyDir;
			CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, strBlock, xy, Scale/2.5, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(&Dom, pIns, _T("RB_NO"), _T(""));
			Dom.LineTo(xy.x, xy.y, xy.x, xy.y+dDistMove);
		}

		// 수직 부재와 수평부재는 표시 위치가 다르다.
		BOOL bVertElement = (fabs(xyEnd.y - xyStt.y) > (xyEnd.x - xyStt.x))? TRUE : FALSE;
		if(dLoad1 == dLoad2)
		{
			dDistMove = Dom.Always(7.5);
			Dom.SetTextAlignHorz((nDir==0)? TA_RIGHT : TA_CENTER);

			str.Format(_T("W = %s %s"), GetStrDouble(fabs(dLoad1), nDec03, bLsd), pData->m_szTonf_M);
			if(nDir == 0) Dom.TextOut(xyStt.x-(dDistMove+Dom.Always(1)) * nLoadDir, (xyStt.y+xyEnd.y)/2,str);
			else if(nDir == 1) Dom.TextOut(xyStt.x-dDistMove+Dom.Always(1), (xyStt.y+xyEnd.y)/2,str);
			else
			{
				if(bVertElement) Dom.TextOut(xyStt.y+dDistMove+Dom.Always(1), (xyStt.y+xyEnd.y)/2,str);
				else Dom.TextOut((xyStt.x+xyEnd.x)/2, xyStt.y+dDistMove+Dom.Always(3),str);
			}

		}
		else
		{
			dDistMove = Dom.Always(7.5);
			Dom.SetTextAlignHorz(TA_RIGHT);
			str.Format(_T("W = %s %s"), GetStrDouble(fabs(dLoad1), nDec03, bLsd), pData->m_szTonf_M);
			Dom.TextOut(xyStt, str);
			str.Format(_T("W = %s %s"), GetStrDouble(fabs(dLoad2), nDec03, bLsd), pData->m_szTonf_M);
			Dom.TextOut(xyEnd, str);
		}

		
	}

	*pDomP << Dom;
}
