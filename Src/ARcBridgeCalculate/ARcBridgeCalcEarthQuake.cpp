// ARcBridgeCalcEarthQuake.cpp: implementation of the CARcBridgeClacEarthQuake class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DIAGRAM_ROW_SIZE	13
CARcBridgeCalcEarthQuake::CARcBridgeCalcEarthQuake()
{
	m_UW_Conc		= 0;
	m_dWidthSlab	= 0;
	m_dWeightSlab	= 0;
	m_dWeightWall	= 0;
	m_dWeightHunch	= 0;
	m_dArrWeightWall.RemoveAll();
	m_dArrWeightHunch.RemoveAll();
	m_sArrWeightWall.RemoveAll();
	m_sArrWeightHunch.RemoveAll();
	m_dWeightUpper	= 0;
	m_DeadLoadSoil	= 0;

	long i=0; for(i=0; i<CALC_DIR; i++)
	{
		m_alpha[i]	= 0;
		m_beta[i]	= 0;
		m_gamma[i]	= 0;
		m_T[i]		= 0;
		m_Cs[i]		= 0;
		m_Pe[i]		= 0;
	}
	m_ArrPe.RemoveAll();
	for(i=0; i<2; i++)
	{
		m_ToapEQ[i]		= 0;
		m_ToapNormal[i]	= 0;
		m_HeightToap[i]	= 0;
		m_HeightWall[i] = 0;
	}
}

CARcBridgeCalcEarthQuake::~CARcBridgeCalcEarthQuake()
{
	AhTPADelete(&m_pListRighty,(CRigidityFooting*)0);
}

void CARcBridgeCalcEarthQuake::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) return;

	AhTPADelete(&m_pListRighty,(CRigidityFooting*)0);

	long nRound = pData->IsSI() ? 4 : 3;
	m_UW_Conc  = Round(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete),nRound);

	if(pBri->IsBoxType() == FALSE)
	{
		// 기초에 대한 강성계산
		long nJ=0; for(nJ=0; nJ<=pBri->m_nQtyJigan; nJ++)
		{
			if((nJ==0 || nJ==pBri->m_nQtyJigan) && !pBri->IsOutsideWall())
			{
				continue;
			}
			if(nJ==0)
				CalculateRigidityAbut(TRUE);
			else if(nJ==pBri->m_nQtyJigan)
				CalculateRigidityAbut(FALSE);
			else
				CalculateRigidityPier(nJ);
		}

		if(pBri->m_FemEQ[0].IsOutputLoaded()==FALSE) return;

		ReadSapOutUnitDisplacement();

		// 등가정적지진하중의 산정 : 단위하중재하
		CalculateUnitLoad();
		CalculateCoefficientXDir();
		CalculateCoefficientYDir();

		// 지진시 토압하중 산정
		CalculateSoilPressure();
	}
	else
	{
		// 고정하중
		if(m_pStd->m_pLoad->m_DeadLoadDistribution[0]==0 && m_pStd->m_pLoad->m_DeadLoadDistribution[1]==0)
		{
			// 계산이 안되어있는 상태에서 CalculateDeadLoad()만 호출하면 토피등이 계산이 안된상태이므로 안된다.
			m_pStd->m_pLoad->m_bRailDesign = pBri->m_pARcBridgeDataStd->IsRailDesign();
			pBri->SetDataInitPaveDetailWidth();

			m_pStd->m_pLoad->CalculateAll();
		}

		double dApplyWidth = m_pStd->GetSlabWidth(-1);

		m_DeadLoadDistribution = (m_pStd->m_pLoad->m_DeadLoadDistribution[0] + m_pStd->m_pLoad->m_DeadLoadDistribution[1]) * dApplyWidth;
		m_DeadLoadDistributionLsd[0] = m_pStd->m_pLoad->m_DeadLoadDistribution[0] * dApplyWidth;
		m_DeadLoadDistributionLsd[1] = m_pStd->m_pLoad->m_DeadLoadDistribution[1] * dApplyWidth;

		// 구체관성력 : 두께 x 콘크리트
		for(long nCase = 0; nCase < eGoalEarthQuakeCount; nCase++)
		{
			EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);
			double Kh = m_pStd->GetKhValue(FALSE, TRUE, eGoalEq);

			m_InertialForce_Slab[nCase][iUPPER] = Kh * m_UW_Conc * toM(pBri->m_dTS) * dApplyWidth;
			m_InertialForce_Slab[nCase][iLOWER] = Kh * m_UW_Conc * toM(pBri->m_dTS_Lower) * dApplyWidth;
			m_InertialForce_Wall[nCase][iLEFT]  =  Kh * m_UW_Conc * toM(pBri->m_dWS) * dApplyWidth;
			m_InertialForce_Wall[nCase][iRIGHT] =  Kh * m_UW_Conc * toM(pBri->m_dWE) * dApplyWidth;
		}
		// 지진시 토압하중 산정
		CalculateSoilPressure();
	}
}

void CARcBridgeCalcEarthQuake::CalculateRigidityAbut(BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijum = bStt ? 0 : pBri->m_nQtyJigan;
	CDPoint vAng = CDPoint(0,1);
	double Hw = toM(pBri->GetHeightModelJijum(nJijum));	// 벽체높이
	double Bs = m_pStd->GetSlabWidth(nJijum);	// 슬래브폭원(교량폭,벽체폭)

	if(pBri->GetTypeJointPosFootingEQ()==1)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
		double FH = pFooting->GetHeight();
		Hw += toM(FH)/2;
	}

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	CBoringData *pBoring  = pBri->GetBoringDataJijumBase(nJijum);

	if(pFooting==NULL) return;

	// 기초강성산정
	CRigidityFooting *pRigity = new CRigidityFooting(pFooting,pBoring,nJijum);
	pRigity->m_pStd = m_pStd;
	pRigity->CalculateRigidity();
	pRigity->CalculateBackFillAndCombine(Hw,Bs);
	m_pListRighty.Add(pRigity);
}

void CARcBridgeCalcEarthQuake::CalculateRigidityPier(long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
	CBoringData *pBoring  = pBri->GetBoringDataJijumBase(nJijum);

	if(pFooting==NULL) return;

	// 기초강성산정
	CRigidityFooting *pRigity = new CRigidityFooting(pFooting,pBoring,nJijum);
	pRigity->m_pStd = m_pStd;
	pRigity->CalculateRigidity();
	pRigity->CalculateBackFillAndCombine(0,0); // 교각위치는 계산하지 않음...
	m_pListRighty.Add(pRigity);
}

void CARcBridgeCalcEarthQuake::CalculateUnitLoad()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGBOOL bLSD = pData->IsLsdDesign();
	if(m_pStd->m_pLoad->m_DeadLoadDistribution[0]==0 && m_pStd->m_pLoad->m_DeadLoadDistribution[1]==0)
	{
		// 계산이 안되어있는 상태에서 CalculateDeadLoad()만 호출하면 토피등이 계산이 안된상태이므로 안된다.
		m_pStd->m_pLoad->m_bRailDesign = pBri->m_pARcBridgeDataStd->IsRailDesign();
		pBri->SetDataInitPaveDetailWidth();

		m_pStd->m_pLoad->CalculateAll();
	}

	long nJijum  = 0;
	long nLine   = 0;
	CDPoint vAng = CDPoint(0,1);
	double Station = pBri->GetStationOnJijum(nJijum,nLine);
	m_dWidthSlab = toM(pBri->GetWidthSlabAct(Station,vAng));	// 슬래브폭원(교량폭,벽체폭)

	// 포장,보도,방호벽,중분대,난간등의 하중을 재하.....
	// 구체는 SAP에서 자동으로 재하
	long nRound = pData->IsSI() ? 4 : 3;
	m_UW_Conc  = Round(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete),nRound);
	m_DeadLoadDistribution = (m_pStd->m_pLoad->m_DeadLoadDistribution[0] + m_pStd->m_pLoad->m_DeadLoadDistribution[1]) * m_dWidthSlab;
	m_DeadLoadDistributionLsd[0] = m_pStd->m_pLoad->m_DeadLoadDistribution[0] * m_dWidthSlab;
	m_DeadLoadDistributionLsd[1] = m_pStd->m_pLoad->m_DeadLoadDistribution[1] * m_dWidthSlab;
	m_dWeightSlab	= Round(toM(pBri->m_dTS)*m_dWidthSlab*m_UW_Conc,nRound);
	m_DeadLoadSoil	= m_pStd->m_pLoad->m_dSoilPressureVert*m_dWidthSlab;
	m_dWeightHunch	= 0;
	// 헌치
	m_dArrWeightHunch.RemoveAll();
	m_sArrWeightHunch.RemoveAll();
// 	double HW_m=0.,HW_l=0.,HW_r=0.,HH=0.,HunchWeight=0.;
	BOOL bSttHUnch = TRUE;
	CString strHunch;
	double LengthModel = toM(pBri->GetLengthModelSpan(-1)); // 모델링상의 전체길이
	for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		bSttHUnch = (nJijum==0) ? FALSE : TRUE;

		double dLengthModel = LengthModel;
		double HH = toM(pBri->GetThickHunch(nJijum,bSttHUnch,TRUE));
		double HW_l = toM(pBri->GetWidthHunch(nJijum,TRUE));
		double HW_r = toM(pBri->GetWidthHunch(nJijum,FALSE));
		double HW_m = 0;
		double dWidthSlab = GetValueUnitChange(m_dWidthSlab, UNIT_CGS_M, pData->m_szM);		// 단위변환시에만 사용, 계산때는 사용하지 않는다.

		if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
		{
			HW_m = toM(pBri->m_dWL[nJijum-1][iUPPER]+pBri->m_dWR[nJijum-1][iUPPER]);
		}
		double HunchWeight = Round((HW_m+HW_m+HW_l+HW_r)/2*HH*m_dWidthSlab*m_UW_Conc/LengthModel,nRound);
		// 단위 변환
		dLengthModel = bLSD==TRUE ? pBri->GetLengthModelSpan(-1) : toM(pBri->GetLengthModelSpan(-1)); // 모델링상의 전체길이
		HH = bLSD==TRUE ? pBri->GetThickHunch(nJijum,bSttHUnch,TRUE) : toM(pBri->GetThickHunch(nJijum,bSttHUnch,TRUE));
		HW_l = bLSD==TRUE ? pBri->GetWidthHunch(nJijum,TRUE) : toM(pBri->GetWidthHunch(nJijum,TRUE));
		HW_r = bLSD==TRUE ? pBri->GetWidthHunch(nJijum,FALSE) : toM(pBri->GetWidthHunch(nJijum,FALSE));

		if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
			HW_m = bLSD==TRUE ? (pBri->m_dWL[nJijum-1][iUPPER]+pBri->m_dWR[nJijum-1][iUPPER]) : toM(pBri->m_dWL[nJijum-1][iUPPER]+pBri->m_dWR[nJijum-1][iUPPER]);

		if(HW_m==0)
			strHunch.Format("(%s x %s)/2 x %s x %s / %s",pData->GetStrDouble(HW_m+HW_l+HW_r, UNIT_CGS_M), pData->GetStrDouble(HH, UNIT_CGS_M),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), pData->GetStrDouble(GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), UNIT_CGS_TONF_M3),pData->GetStrDouble(dLengthModel, UNIT_CGS_M));
		else
			strHunch.Format("(%s+%s)/2 x %s x %s x %s / %s", pData->GetStrDouble(HW_m, UNIT_CGS_M) , pData->GetStrDouble(HW_m+HW_l+HW_r, UNIT_CGS_M),pData->GetStrDouble(HH, UNIT_CGS_M),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), pData->GetStrDouble(GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3), UNIT_CGS_TONF_M3),pData->GetStrDouble(dLengthModel, UNIT_CGS_M));
		if(HunchWeight>0)
		{
			m_dArrWeightHunch.Add(HunchWeight);
			m_sArrWeightHunch.Add(strHunch);
		}
	}

	// 벽체의 중량
	m_dArrWeightWall.RemoveAll();
	m_sArrWeightWall.RemoveAll();
	if(pBri->GetTypeUpperWeightEQ()==1)
	{
		CString strWall;
		double dHeightWall=0,dThickWall=0,dLengthWall=0,WallWeight=0;
		for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
		{
			if(nJijum==0)
			{
				dThickWall  = toM(pBri->m_dWS);
				dHeightWall = toM(pBri->GetHeightModelJijum(nJijum)) - toM(pBri->m_dTS)/2;
				dLengthWall = m_dWidthSlab/pBri->GetAngleJijum(nJijum).y;
			}
			else if(nJijum==pBri->m_nQtyJigan)
			{
				dThickWall  = toM(pBri->m_dWE);
				dHeightWall = toM(pBri->GetHeightModelJijum(nJijum)) - toM(pBri->m_dTS)/2;
				dLengthWall = m_dWidthSlab/pBri->GetAngleJijum(nJijum).y;
			}
			else
			{
				long nIdxInWall = nJijum - 1;
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				dThickWall  = toM(Round(pWall->m_dW,0));	// 벽체의 두께
				dLengthWall = m_dWidthSlab/pBri->GetAngleJijum(nJijum).y;
				dHeightWall = toM(Round(pWall->GetLength(),0));
			}
			long nTypeColumn = pBri->GetTypeColumnByJijum(nJijum);
			if(nTypeColumn==-1)
			{
				WallWeight = Round(dThickWall*dHeightWall*dLengthWall*m_UW_Conc/LengthModel/2,nRound);
				strWall.Format("%.3f x %.3f x %.3f x %.3f / %.3f / 2",dThickWall,dHeightWall,dLengthWall,GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3),LengthModel);
			}
			else
			{
				long nIdxInWall = nJijum - 1;
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				//#define COL_TYPE_CIRCLE		0
				//#define COL_TYPE_RECTANGLE	1
				//#define COL_TYPE_TRACK		2
				//#define COL_TYPE_POLY			3
				double dAreaColumn = pWall->m_Column.GetColumnArea();
				long nEaColumn = pWall->m_nCountColumn;
				WallWeight = Round(dAreaColumn*dLengthWall*nEaColumn*m_UW_Conc/LengthModel/2,nRound);
				strWall.Format("%.3f x %.3f x %dEA x %.3f / %.3f / 2",dAreaColumn,dHeightWall,nEaColumn,GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3),LengthModel);
			}
			if(WallWeight>0)
			{
				m_dArrWeightWall.Add(WallWeight);
				m_sArrWeightWall.Add(strWall);
			}
		}
	}
	//
	m_dWeightHunch = 0;
	long i=0; for(i=0; i<m_dArrWeightHunch.GetSize(); i++)
	{
		m_dWeightHunch += m_dArrWeightHunch.GetAt(i);
	}
	m_dWeightWall = 0;
	for(i=0; i<m_dArrWeightWall.GetSize(); i++)
	{
		m_dWeightWall += m_dArrWeightWall.GetAt(i);
	}
	m_dWeightUpper  = m_DeadLoadDistribution + m_dWeightSlab + m_dWeightHunch + m_DeadLoadSoil + m_dWeightWall; // 상부의 전체중량 + 벽체 50%
}

// 종방향계수값 계산
void CARcBridgeCalcEarthQuake::CalculateCoefficientXDir()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double Wx  = m_dWeightUpper;
	double Vsx = bLsd? toM(GetMaxDisplacementUpper(TRUE)) : GetMaxDisplacementUpper(TRUE);
	double dx  = toM(pBri->GetLengthModelSpan(-1)); // 모델링상의 전체길이
	double Po  = 1.00; // 단위하중
	double g   = pBri->GetGravity(); 
	double A   = pBri->GetCoefficientAccelerateEQ(); 
	double S   = pBri->GetCoefficientGround();

	long i = CALC_DIR_X;

	m_alpha[i] = Vsx * dx;
	m_beta[i]  = Wx * Vsx * dx;
	m_gamma[i] = Wx * pow(Vsx,2) * dx;

	if(pData->IsSI())
		m_T[i]  = 2 * ConstPi * sqrt(m_gamma[i]*10/(Po*g*m_alpha[i]));
	else
		m_T[i]  = 2 * ConstPi * sqrt(m_gamma[i]/(Po*g*m_alpha[i]));

	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		m_Cs[i] = MIN(2.5*A, 1.2*A*S/pow(m_T[i],2.0/3.0));
	}
	else
	{
		m_Cs[i] = GetSa(pBri, m_T[i])/g;
	}

	m_Pe[i] = (m_beta[i] * m_Cs[i] / m_gamma[i]) * Wx * Vsx;
}

// 횡방향계수값 계산
void CARcBridgeCalcEarthQuake::CalculateCoefficientYDir()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	double Wy  = m_dWeightUpper;
	double Po  = 1.00; // 단위하중
	double g   = pBri->GetGravity(); 
	double A   = pBri->GetCoefficientAccelerateEQ(); 
	double S   = pBri->GetCoefficientGround();

	m_ArrPe.RemoveAll();
	long nDir = CALC_DIR_Y;
	long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0]);
	long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0]);
	long nJoint	= 0;
	for(nJoint=nIdxJointStt; nJoint<nIdxJointEnd; nJoint++)
	{
		double Vsy = GetDisplacement(nJoint,FALSE);
		double Iy  = m_pStd->m_pFrame->GetLengthBetweenJoint(&pBri->m_FemEQ[0],nJoint,nJoint+1); // 해당절점이 차지하는 길이

		if(bLsd)
		{
			Vsy = toM(Vsy);
			Iy = toM(Iy);
		}
		m_alpha[nDir] += Vsy * Iy;
		m_beta[nDir]  += Wy * Vsy * Iy;
		m_gamma[nDir] += Wy * pow(Vsy,2) * Iy;
	}
	if(pData->IsSI())
		m_T[nDir]  = 2 * ConstPi * sqrt(m_gamma[nDir]*10/(Po*g*m_alpha[nDir]));
	else
		m_T[nDir]  = 2 * ConstPi * sqrt(m_gamma[nDir]/(Po*g*m_alpha[nDir]));

	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		m_Cs[nDir] = MIN(2.5*A, 1.2 * A * S / pow(m_T[nDir],2.0/3.0));
	}
	else
	{
		m_Cs[nDir] = GetSa(pBri, m_T[nDir])/g;
	}
	
	for(nJoint=nIdxJointStt; nJoint<nIdxJointEnd; nJoint++)
	{
		double Vsy = GetDisplacement(nJoint,FALSE);
		if(Vsy==0) // 해당절점에 변위가 없는 경우
		{
			m_ArrPe.Add(0);
			continue;
		}
		if(bLsd)
		{
			Vsy = toM(Vsy);
		}
		double Pe = (m_beta[nDir] * m_Cs[nDir] / m_gamma[nDir]) * Wy * Vsy;
		m_ArrPe.Add(Pe);
	}
}


void CARcBridgeCalcEarthQuake::CalculateSoilPressure()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(!pBri->IsOutsideWall()) return;

	CDPoint vAng = CDPoint(0,1);

	if(pBri->IsBoxType() == FALSE)
	{
		double r = pBri->m_dUnitWeigthBackFill;
		for(long nA=0; nA<2; nA++)
		{
			long nJijum = (nA==0) ? 0 : pBri->m_nQtyJigan;

			double Kae = m_pStd->GetCoefficient_MononobeOkabe(nA);
			double Ka  = m_pStd->GetCoefficient_coulomb();

			double H   = toM(pBri->GetHeightModelJijum(nJijum));
			double Bs  = m_pStd->GetSlabWidth();	// 슬래브폭원(교량폭,벽체폭)
			if(pBri->GetTypeJointPosFootingEQ()==1)
			{
				CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
				double FH = pFooting->GetHeight();
				H += toM(FH)/2;
			}

			m_ToapEQ[nA]	 = 0.5*Kae*r*H*H*Bs;
			m_ToapNormal[nA] = 0.5*Ka*r*H*H*Bs;
			m_HeightWall[nA] = H;
			m_HeightToap[nA] = ( (m_ToapNormal[nA]*H/3) + ((m_ToapEQ[nA]-m_ToapNormal[nA])*0.6*H) ) / m_ToapEQ[nA];
		}
	}
	else
	{
		// 높이별 토압
		CARcBridgeDesignFrame *pFrame = m_pStd->m_pFrame;
		pFrame->MakeXyCoordinates(TRUE);

		for(long ix = 0; ix < 2; ix++)
		{
			m_dArrSoilPressure[eGoalEQ_Collapse][ix].RemoveAll();
			m_dArrSoilPressure[eGoalEQ_Function][ix].RemoveAll();
			m_dArrSoilPressure[eGoalEQ_Immediately][ix].RemoveAll();
			m_dArrSoilPressure[eGoalEQ_LongTerm][ix].RemoveAll();
			m_dArrSoilHeight[ix].RemoveAll();
		}

		long nSizeSlab = pFrame->m_vArrSlab[iUPPER].GetSize();
		for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
		{
			for(long nCase = 0; nCase <= eGoalEarthQuakeCount; nCase++)
			{
				BOOL bSttWall = iLR == iLEFT ? TRUE : FALSE;
				EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);
				if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;

				long nJijum = bSttWall ? 0 : pBri->m_nQtyJigan;;
				long nSize = pFrame->m_vArrWall[nJijum].GetSize();

				double dHStt = bSttWall ? toM(pFrame->m_vArrSlab[iUPPER].GetAt(0).z) : toM(pFrame->m_vArrSlab[iUPPER].GetAt(nSizeSlab - 1).z);

				double Height = 0;
				double Value  = m_pStd->m_pLoad->GetLoadSoilPressure(Height, iLR == iLEFT, FALSE, TRUE, eGoalEq);
				m_dArrSoilHeight[iLR].Add(Height);
				m_dArrSoilPressure[nCase][iLR].Add(Value);

				for(long ix = 0; ix < nSize; ix++)
				{
					Height = fabs(dHStt - toM(pFrame->m_vArrWall[nJijum].GetAt(ix).z));
					Value  = m_pStd->m_pLoad->GetLoadSoilPressure(Height, iLR == iLEFT, FALSE, TRUE, eGoalEq);
					m_dArrSoilHeight[iLR].Add(Height);
					m_dArrSoilPressure[nCase][iLR].Add(Value);
				}

				double dHEnd = toM(pFrame->m_vArrSlab[iLOWER].GetAt(0).z);
				Height = fabs(dHStt - dHEnd);
				Value  = m_pStd->m_pLoad->GetLoadSoilPressure(Height, iLR == iLEFT, FALSE, TRUE, eGoalEq);
				m_dArrSoilHeight[iLR].Add(Height);
				m_dArrSoilPressure[nCase][iLR].Add(Value);
			}
		}
	}
}

CRigidityFooting* CARcBridgeCalcEarthQuake::GetRigidityFooting(long nJijum)
{
	CRigidityFooting *pRigid(NULL);

	long nSizeRigid = m_pListRighty.GetSize();

	long n=0; for(n=0; n<nSizeRigid; n++)
	{
		pRigid = m_pListRighty.GetAt(n);
		if(pRigid->m_nJijum==nJijum)
			return pRigid;
	}

	return NULL;
}

void CARcBridgeCalcEarthQuake::ReadSapOutUnitDisplacement()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
 
	long nSizeRow = pBri->m_FemEQ[0].GetJointSize();
	long nSizeCol = CALC_DIR;

	m_MatDispUnit.Resize(nSizeRow,nSizeCol);
	CFEMManage *pFEM = &pBri->m_FemEQ[0];

	CString strLoadX = "LOAD-X";
//	CString strLoadY = "LOAD-Y";
	for(long nJoint=0; nJoint<nSizeRow; nJoint++)
	{
		CJoint *pJ = pFEM->GetJoint((USHORT)nJoint);
		long nLoadCaseX = pFEM->GetLoadCaseByString(strLoadX);
//		long nLoadCaseY = pFEM->GetLoadCaseByString(strLoadY);
		CResultForce *pResultX = pJ->GetResultForce(nLoadCaseX);
//		CResultForce *pResultY = pJ->GetResultForce(nLoadCaseY);
		CVector vDispX = pResultX ? pResultX->m_Disp : CVector(0,0,0);
//		CVector vDispY = pResultY->m_Disp;

		m_MatDispUnit(nJoint,CALC_DIR_X) = Round(vDispX.x,ROUND_POS_DISP);
		m_MatDispUnit(nJoint,CALC_DIR_Y) = 0;
//		m_MatDispUnit(nJoint,CALC_DIR_Y) = Round(vDispY.y,ROUND_POS_DISP);
	}
}

// 절대값을 기준으로 리턴
double CARcBridgeCalcEarthQuake::GetMaxDisplacementUpper(BOOL bDirBridge, BOOL bAvg)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	double MaxDisp = 0.;
	double AvgDisp = 0.;
	double SumDisp = 0.;
	long icDisp = 0;

	if(pBri->m_bApplyUserDispMax)
	{
		MaxDisp = pBri->m_dUnitLoadDispMaxUser;
		return MaxDisp;
	}

	long nSizeRow = m_MatDispUnit.GetRow();
	long nDir = bDirBridge ? CALC_DIR_X : CALC_DIR_Y;

	long nRowStt = m_pStd->m_pFrame->GetNumberElementSlabStt(&pBri->m_FemEQ[0]);
	long nRowEnd = m_pStd->m_pFrame->GetNumberElementSlabEnd(&pBri->m_FemEQ[0]);

	for(long nRow=nRowStt; nRow<=nRowEnd; nRow++)
	{
		if(nRow==-1 || nRow>=nSizeRow) break;
		double Disp = fabs(m_MatDispUnit(nRow,nDir));
		MaxDisp = MAX(Disp,MaxDisp);

		SumDisp += Disp;
		++icDisp;
	}
	AvgDisp = SumDisp / icDisp;

	return bAvg? AvgDisp : MaxDisp;
}

double CARcBridgeCalcEarthQuake::GetDisplacement(long nJoint, BOOL bDirBridge)
{
	double Disp = 0;
	
	if(nJoint<0 || nJoint>=m_MatDispUnit.GetRow()) return Disp;

	long nDir = bDirBridge ? CALC_DIR_X : CALC_DIR_Y;

	Disp = fabs(m_MatDispUnit(nJoint,nDir));

	return Disp;
}

void CARcBridgeCalcEarthQuake::WriteCalcFootingSpring(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle)
{
	long nJijum = 0;
	CFootingApp *pFooting = NULL;

	CString str = _T("");
	// 시종점기초
	for(long nA=0; nA<2; nA++)
	{
		nJijum = (nA==0) ? 0 : pBri->GetCountJijum()-1;
		str    = (nA==0) ? "시점측" : "종점측";
		XLText->GoPosition(NO_MOVE,COL_POS_1);

		pFooting = pBri->GetFootingByJijumNumber(nJijum);
		if(pFooting==NULL) continue;
		if(pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_PILE) 
		{
			XLText->AddFormatText("$s+45$h %d)$rSPRING 계수 산정 : %s 기초$n",nIndexTitle++,str);
			*XLText += 1;
			WriteCalcFootingSpring_Pile(XLText,pBri,nJijum);
		}
		else 
		{
			XLText->AddFormatText("$s+17$h %d)$rSPRING 계수 산정 : %s 기초$n",nIndexTitle++,str);
			*XLText += 1;
			WriteCalcFootingSpring_Direct(XLText,pBri,nJijum);
		}
		*XLText += 1;
	}

	// 중간기초
	for(nJijum=1; nJijum<pBri->GetCountJijum()-1; nJijum++)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);

		pFooting = pBri->GetFootingByJijumNumber(nJijum);
		if(pFooting==NULL) continue;
		if(pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_PILE) 
		{
			XLText->AddFormatText("$s+45$h %d)$rSPRING 계수 산정 : 중간지점부 %d 기초$n",nIndexTitle++,nJijum);
			*XLText += 1;
			WriteCalcFootingSpring_Pile(XLText,pBri,nJijum);
		}
		else 
		{
			XLText->AddFormatText("$s+17$h %d)$rSPRING 계수 산정 : 중간지점부 %d 기초$n",nIndexTitle++,nJijum);
			*XLText += 1;
			WriteCalcFootingSpring_Direct(XLText,pBri,nJijum);
		}
		*XLText += 1;
	}
}

void CARcBridgeCalcEarthQuake::WriteCalcFootingSpring_Wall(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pData  = pBri->m_pARcBridgeDataStd;
	CRigidityFooting  *pRigid = GetRigidityFooting(nJijum);
	if(pRigid==NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	CString szUnitRot = pData->m_szTonfM + "/rad";
	double Es = GetValueUnitChange(pRigid->m_Es_Wall,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	// (ARCBRIDGE-2663) 단위는 KN/m 가 맞지만 한계상태 일때 길이단위가 들어가기 때문에 1/1000이 되어야 한다. 강제로 KN/m2으로 단위변환한다.
	double Ks = GetValueUnitChange(pRigid->m_KsBackFill,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double Kr = GetValueUnitChange(pRigid->m_KrBackFill,UNIT_CGS_TONF,pData->m_szTonf);
	double dH1 = bLsd ? frM(pRigid->m_dH1) : pRigid->m_dH1;
	double dWidthBackFill = bLsd ? frM(pRigid->m_dWidthBackFill) : pRigid->m_dWidthBackFill;
	double dHeightBackFill = bLsd ? frM(pRigid->m_dHeightBackFill) : pRigid->m_dHeightBackFill;

	long nIndexTitle = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$s+04$h$i%s$r종방향(교축방향 : X-DIR) 변위강성$n",GetNumberString(nIndexTitle++,NUM_GANA));
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$r%s x Eo x B$n", (bLsd? _T("0.000425") : _T("0.425")));
	XLText->AddFormatText("$h$r$r$c=$r%s x %s x %s$n", (bLsd? _T("0.000425") : _T("0.425")), pData->GetStrDouble(Es, UNIT_CGS_TONF_M2, FALSE, 0), pData->GetStrDouble(dWidthBackFill, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_M, FALSE, 0), pData->m_szTonf_M);
	//
	XLText->AddFormatText("$s+04$h$i%s$r횡방향(교축직각방향 : Y-DIR) 회전강성$n",GetNumberString(nIndexTitle++,NUM_GANA));
	XLText->AddFormatText("$h$r$p[K_R]$r$c=$r%s x Eo x B x H²$n", (bLsd? _T("0.000072") : _T("0.072")));
	XLText->AddFormatText("$h$r$r$c=$r%s x %s x %s x %s²$n", (bLsd? _T("0.000072") : _T("0.072")),
		pData->GetStrDouble(Es, UNIT_CGS_TONF_M2, FALSE, 0), pData->GetStrDouble(dWidthBackFill, UNIT_CGS_M), pData->GetStrDouble(dHeightBackFill, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Kr, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, TRUE),szUnitRot);
	//
	XLText->AddFormatText("$h$i%s$rY축,Z축으로의 변위강성과 X축,Z축에 대한 회전강성은 무시한다.$n",GetNumberString(nIndexTitle++,NUM_GANA));
	//
	if(pBri->GetTypeRigidFootingEQ()==0)
	{
		XLText->AddFormatText("$s+05$h$i%s$r기초상단으로 위치 전환$n",GetNumberString(nIndexTitle++,NUM_GANA));
		XLText->AddFormatText("$h$r$ch1$r$c=$r0.37 x H = 0.37 x %s = %s %s$n", pData->GetStrDouble(dHeightBackFill, UNIT_CGS_M), pData->GetStrDouble(dH1, UNIT_CGS_M), pData->m_szM);

		char *Title[] = { "δx", "θy","δx", "θy"};
		int nWidthJoint[] = {5, 5, 5, 5};
		long nSizeRow = 3;
		long nSizeCol = 4;
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddTable(nSizeRow-1,20-1,1,0);
		long j=0; for(j=0; j<nSizeCol; j++)
		{
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title[j]);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
		long i=0; for(i=1; i<nSizeRow; i++)
		{
			CString str = _T("");
			for(j=0; j<nSizeCol; j++)
			{
				if(i==1)
				{
					if(j==0) str.Format("Ks");
					if(j==1) str.Format("h1 x Ks");
					if(j==2) str = pData->GetStrDouble(Ks, UNIT_CGS_TONF_M, FALSE, 0);
					if(j==3) str = pData->GetStrDouble(Ks*dH1, UNIT_CGS_TONF, FALSE, 0, FALSE, -1, TRUE);
				}
				if(i==2)
				{
					if(j==0) str.Format("h1 x Ks");
					if(j==1) str.Format("h1²x Ks + Kr");
					if(j==2) str = pData->GetStrDouble(Ks*dH1, UNIT_CGS_TONF, FALSE, 0, FALSE, -1, TRUE);
					if(j==3) str = pData->GetStrDouble(Ks*dH1*dH1 + Kr, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, TRUE);
				}
				if(j==0)	XLText->AddText("$n$h");
				XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcEarthQuake::WriteCalcFootingSpring_Combine(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pData  = pBri->m_pARcBridgeDataStd;
	CRigidityFooting  *pRigid = GetRigidityFooting(nJijum);
	if(pRigid==NULL) return;
	if(pBri->GetTypeRigidFootingEQ()!=0) return;

	XLText->GoPosition(NO_MOVE,COL_POS_3);

	CString szUnitRot = pData->m_szTonfM + "/rad";
	// (ARCBRIDGE-2663) 단위는 KN.m 가 맞지만 한계상태 일때 길이단위가 들어가기 때문에 1/1000이 되어야 한다. 강제로 KN으로 단위변환한다.
	// 하지만 여기서는 길이단위 1m가 들어가줘야 하기 때문에 원래대로 변환한다.
	double dValueWall = GetValueUnitChange(pow(pRigid->m_dH1,2)*pRigid->m_KsBackFill + pRigid->m_KrBackFill,UNIT_CGS_TONFM,pData->m_szTonfM);

	// (ARCBRIDGE-2663) 단위는 KN/m 가 맞지만 한계상태 일때 길이단위가 들어가기 때문에 1/1000이 되어야 한다. 강제로 KN/m2으로 단위변환한다.
	XLText->AddFormatText("$h$iKδx$r$c=$r%s(벽체) + %s(기초)$m+10$c=$r'%s %s$n"
		,pData->GetStrDouble(GetValueUnitChange(pRigid->m_KsBackFill,UNIT_CGS_TONF_M,pData->m_szTonf_M), UNIT_CGS_TONF_M, FALSE, -1, FALSE, 10)
		,pData->GetStrDouble(GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPX],UNIT_CGS_TONF_M,pData->m_szTonf_M), UNIT_CGS_TONF_M, FALSE, 0, FALSE, 10)
		,pData->GetStrDouble(GetValueUnitChange(pRigid->m_CombK[RIGIDITY_DISPX],UNIT_CGS_TONF_M,pData->m_szTonf_M), UNIT_CGS_TONF_M, FALSE, 0, FALSE, 10)
		,pData->m_szTonf_M);
	XLText->AddFormatText("$h$iKθy$r$c=$r%s(벽체) + %s(기초)$m+10$c=$r'%s %s$n"
		,pData->GetStrDouble(dValueWall, UNIT_CGS_TONFM, FALSE, 0, FALSE, 10, TRUE)
		,pData->GetStrDouble(GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_ROTATEY],UNIT_CGS_TONFM,pData->m_szTonfM), UNIT_CGS_TONFM, FALSE, 0, FALSE, 10, TRUE)
		,pData->GetStrDouble(GetValueUnitChange(pRigid->m_CombK[RIGIDITY_ROTATEY],UNIT_CGS_TONFM,pData->m_szTonfM), UNIT_CGS_TONFM, FALSE, 0, FALSE, 10, TRUE)
		,szUnitRot);

	char *Title[] = { "Kδx", "Kδy","Kδz","Kθx", "Kθy","Kθz"};
	int nWidthJoint[] = {3, 3, 3, 3, 3, 3};
	long nSizeRow = 7;
	long nSizeCol = 6;
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddTable(nSizeRow-1,18-1,1,0);
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
	}
	long i=0; for(i=1; i<nSizeRow; i++)
	{
		CString str = _T("");
		for(j=0; j<nSizeCol; j++)
		{
			str = _T("-");
			if(i==1 && j==0)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_DISPX], UNIT_CGS_TONF_M,pData->m_szTonf_M, FALSE, 0, TRUE, -1, TRUE);
			if(i==1 && j==4)
				str = pData->GetStrDoubleUnitChange(pRigid->m_KsBackFill*pRigid->m_dH1,UNIT_CGS_TONF_M,pData->m_szTonf_M, FALSE, 0, TRUE, -1, TRUE);
			if(i==2 && j==1)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_DISPY],UNIT_CGS_TONF_M,pData->m_szTonf_M, FALSE, 0, TRUE, -1, TRUE);
			if(i==3 && j==2)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_DISPZ],UNIT_CGS_TONF_M,pData->m_szTonf_M, FALSE, 0, TRUE, -1, TRUE);
			if(i==4 && j==3)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_ROTATEX],UNIT_CGS_TONFM,pData->m_szTonfM, FALSE, 0, TRUE, -1, TRUE);
			if(i==5 && j==4)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_ROTATEY],UNIT_CGS_TONFM,pData->m_szTonfM, FALSE, 0, TRUE, -1, TRUE);
			if(i==5 && j==0)
				str = pData->GetStrDoubleUnitChange(pRigid->m_KsBackFill*pRigid->m_dH1,UNIT_CGS_TONFM,pData->m_szTonfM, FALSE, 0, TRUE, -1, TRUE);
			if(i==6 && j==5)
				str = pData->GetStrDoubleUnitChange(pRigid->m_CombK[RIGIDITY_ROTATEZ],UNIT_CGS_TONFM,pData->m_szTonfM, FALSE, 0, TRUE, -1, TRUE);
			if(j==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcEarthQuake::WriteCalcFootingSpring_Direct(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pData  = pBri->m_pARcBridgeDataStd;
	CRigidityFooting  *pRigid = GetRigidityFooting(nJijum);
	if(pRigid==NULL) return;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	long nIndexTitle = 1;
	BOOL bLsd = pData->IsLsdDesign();
	BOOL bAbutWall = (nJijum==0 || nJijum==pBri->GetCountJijum()-1) ? TRUE : FALSE;
	if(bAbutWall)
	{
		XLText->AddFormatText("$s+05$h%s$r벽체의 변위와 회전에 대한 강성$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		WriteCalcFootingSpring_Wall(XLText,pBri,nJijum);
		*XLText += 1;
	}

	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	long nNumberType = bAbutWall ? NUM_GANA : NUM_CIRCLE;
	if(bAbutWall)
	{
		XLText->AddFormatText("$s+15$h%s$r기초의 변위와 회전에 대한 강성$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
	}
	if(pBri->GetTypeRigidFootingEQ()==2) 
	{
		XLText->AddFormatText("$h$r$r: 안전측으로하여 무한강성으로 한다.$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		return; // 기초를 고정으로 처리하는 경우
	}

	XLText->AddFormatText("$h$r얕은 사각형 기초의 동적거동에 관한 스프링 계수는 반무한체 표면에 구속된$n");
	XLText->AddFormatText("$h$r원형기초의 계수로 수정하여 구한다.$n");
	XLText->AddFormatText("$h$r$r$p[EQ_SPRING]");
	*XLText += 8;

	double Es = GetValueUnitChange(pRigid->m_Es_Base,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double G  = GetValueUnitChange(pRigid->m_G ,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double dB = bLsd? frM(pRigid->m_B) : pRigid->m_B;
	double dL = bLsd? frM(pRigid->m_L) : pRigid->m_L;
	double dD = bLsd? frM(pRigid->m_D) : pRigid->m_D;

	CString str = _T("");
	CString szUnitRot = pData->m_szTonfM + "/rad";
	str.Format("$h$r$c2B$r$c=$r%s %s$m+05(교축방향)", 
		pData->GetStrDouble(dB*2, UNIT_CGS_M, FALSE, -1, FALSE, 7), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format("$h$r$c2L$r$c=$r%s %s$m+05(교축직각방향)", 
		pData->GetStrDouble(dL*2, UNIT_CGS_M, FALSE, -1, FALSE, 7), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format("$h$r$cD$r$c=$r%s %s$m+05(기초높이)",
		pData->GetStrDouble(dD, UNIT_CGS_M, FALSE, -1, FALSE, 7), pData->m_szM);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format("$h$r$cEs$r$c=$r%s %s$m+05(흙의 탄성계수)", 
		pData->GetStrDouble(Es, UNIT_CGS_TONF_M2, FALSE, 1),pData->m_szTonf_M2);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format("$h$r$cυ$r$c=$r'%.3f$m+05(흙의 포아슨비)",pRigid->m_v);
	XLText->AddFormatText("$h$r%s$n",str);
	str.Format("$h$r$cG$r$c=$rEs / (2 x (1 + υ)) = %s %s(흙의 전단탄성계수)",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2, FALSE, -1, FALSE, 8), pData->m_szTonf_M2);
	XLText->AddFormatText("$h$r%s$n",str);

	long nIndexMiddle = 1;
	CString strHead = bAbutWall ? "$h$i" : "$h";
	long i = RIGIDITY_DISPX;
	double dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	double Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$s+14%s%s$r변위에 대한 강성 : X축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ro$r$md1$c=$r$p[FOOT_DISP_R0]$m+05Ro : 유효 기초 반지름 (%s)$n$n", pData->m_szM);
	XLText->AddFormatText("$h$r$r$c=$r√(4 x %s x %s / π)$n", 
		pData->GetStrDouble(dB, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(dRo, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_DISP_K0]$m+05Ko : 유효 반지름으로 계산된 강성 (%s)$n$n",pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$r$c=$r(8 x %s x %s) / (2 - %.3f)$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M), pRigid->m_v);
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$m+05Ks : 수정된 기초의 변위강성 (%s)$n",pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i],pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONF_M, FALSE, 0));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",pData->GetStrDouble(Ks, UNIT_CGS_TONF_M, FALSE, 0), pData->m_szTonf_M);
	//
	i = RIGIDITY_DISPY;
	dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$s+10%s%s$r변위에 대한 강성 : Y축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_DISP_K0]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r(8 x %s x %s) / (2 - %.3f)$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M), pRigid->m_v);
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$n");
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i],pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONF_M, FALSE, 0));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_TONF_M, FALSE, 0), pData->m_szTonf_M);
	//
	i = RIGIDITY_DISPZ;
	dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$s+10%s%s$r변위에 대한 강성 : Z축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_DISP_K0Z]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r(4 x %s x %s) / (1 - %.3f)$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M), pRigid->m_v);
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONF_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$n");
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i],pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONF_M, FALSE, 0));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_TONF_M, FALSE, 0), pData->m_szTonf_M);
	//
	i = RIGIDITY_ROTATEX;
	dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	XLText->AddFormatText("$s+15%s%s$r회전에 대한 강성 : X축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ro$r$md1$c=$r$p[FOOT_ROTATE_RX]$m+05Ro : 유효 기초 반지름 (%s)$n$n", pData->m_szM);
	XLText->AddFormatText("$h$r$r$c=$r｛(2 x %s) x (2 x %s)³/ 3π｝^ ¼$n", pData->GetStrDouble(dB, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(dRo, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_ROTATE_KX]$m+05Ko : 유효 반지름으로 계산된 강성 (%s)$n$n",pData->m_szTonfM);
	XLText->AddFormatText("$h$r$r$c=$r(8 x %s x %s³) /｛3 x (1 - %.3f)｝$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M), pRigid->m_v);
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, -1, FALSE, -1, bLsd), pData->m_szTonfM);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$m+05Ks : 수정된 기초의 변위강성 (%s)$n",pData->m_szTonfM);
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i],pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, bLsd));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, TRUE), szUnitRot);
	//
	i = RIGIDITY_ROTATEY;
	dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	XLText->AddFormatText("$s+15%s%s$r회전에 대한 강성 : Y축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ro$r$md1$c=$r$p[FOOT_ROTATE_RY]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r｛(2 x %s)³x (2 x %s)/ 3π｝^ ¼$n", pData->GetStrDouble(dB, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(dRo, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_ROTATE_KY]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r(8 x %s x %s³) /｛3 x (1 - %.3f)｝$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M), pRigid->m_v);
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, -1, FALSE, -1, bLsd), pData->m_szTonfM);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$m+05Ks : 수정된 기초의 변위강성 (%s)$n",pData->m_szTonfM);
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i], pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, bLsd));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, TRUE), szUnitRot);
	//
	i = RIGIDITY_ROTATEZ;
	dRo = bLsd? frM(pRigid->m_Ro[i]) : pRigid->m_Ro[i];
	Ko =GetValueUnitChange(pRigid->m_Ko[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	Ks =GetValueUnitChange(pRigid->m_ValueK[i],UNIT_CGS_TONFM,pData->m_szTonfM);
	XLText->AddFormatText("$s+15%s%s$r회전에 대한 강성 : Z축방향$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$r$md1Ro$r$md1$c=$r$p[FOOT_ROTATE_RZ]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r｛(4 x %s x %s) x (4 x %s²+ 4 x %s²) / 6π｝^ ¼$n",
		pData->GetStrDouble(dB, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), pData->GetStrDouble(dL, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(dRo, UNIT_CGS_M), pData->m_szM);
	XLText->AddFormatText("$h$r$md1Ko$r$md1$c=$r$p[FOOT_ROTATE_KZ]$n$n");
	XLText->AddFormatText("$h$r$r$c=$r(16 x %s x %s³) / 3$n",
		pData->GetStrDouble(G, UNIT_CGS_TONF_M2), pData->GetStrDouble(dRo, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n",
		pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, -1, FALSE, -1, bLsd), pData->m_szTonfM);
	XLText->AddFormatText("$h$r$iL/B$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rα = %.3f (기초의 형상계수)$n",
		pData->GetStrDouble(dL, UNIT_CGS_M), pData->GetStrDouble(dB, UNIT_CGS_M), dL/dB, pRigid->m_alpha[i]);
	XLText->AddFormatText("$h$r$iD/R$r$c=$r%s / %s$m+05$c=$r'%.3f$r$r→$rβ = %.3f (기초의 근입계수)$n",
		pData->GetStrDouble(dD, UNIT_CGS_M), pData->GetStrDouble(dRo, UNIT_CGS_M), dD/dRo, pRigid->m_beta[i]);
	XLText->AddFormatText("$h$r$p[K_S]$r$c=$rα x β x Ko$m+05Ks : 수정된 기초의 변위강성 (%s)$n",pData->m_szTonfM);
	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f x %s$n",pRigid->m_alpha[i],pRigid->m_beta[i], pData->GetStrDouble(Ko, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, bLsd));
	XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(Ks, UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, TRUE), szUnitRot);

	*XLText += 1;
	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(bAbutWall && pBri->GetTypeRigidFootingEQ()==0)
	{
		XLText->AddFormatText("$s+10$h%s$r벽체강성과 기초강성의 조합$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		WriteCalcFootingSpring_Combine(XLText,pBri,nJijum);
	}
}

void CARcBridgeCalcEarthQuake::WriteCalcFootingSpring_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	CARcBridgeDataStd *pData  = pBri->m_pARcBridgeDataStd;
	CRigidityFooting  *pRigid = GetRigidityFooting(nJijum);
	if(pRigid==NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	// 가상으로 말뚝을 계산..... => 특성치만 사용..
	CDesignPile *pCalcPile = new CDesignPile(pData->m_nTypeUnit, pData->IsRailDesign() ? 1 : 0);
	pCalcPile->m_nIndexJijum = nJijum;
	pCalcPile->m_bBridgeDir  = TRUE;
	m_pStd->SetCalcPileData(pCalcPile);

	CPileLoad *pLoadNull = new CPileLoad; // 계산을 위한 가짜하중...
	pLoadNull->m_dWCaseApply = LOAD_PILE_EQ;

	if(bLsd)
		pCalcPile->m_pListExtreme.Add(pLoadNull);
	else
		pCalcPile->m_pListUlt.Add(pLoadNull);
	pCalcPile->SetTypeUnit();
	pCalcPile->CalculateCondition();
	pCalcPile->CalculatePileAttribute();
	pCalcPile->CalculatePileDisplacementAndReaction();
	//////////////////////////////////////////////////////////////////////////

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	long nIndexTitle = 1;
	BOOL bAbutWall = (nJijum==0 || nJijum==pBri->GetCountJijum()-1) ? TRUE : FALSE;
	if(bAbutWall)
	{
		XLText->AddFormatText("$h%s$r벽체의 변위와 회전에 대한 강성$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		WriteCalcFootingSpring_Wall(XLText,pBri,nJijum);
		*XLText += 1;
	}

	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	long nNumberType = bAbutWall ? NUM_GANA : NUM_CIRCLE;
	if(bAbutWall)
	{
		XLText->AddFormatText("$h%s$r기초의 변위와 회전에 대한 강성$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
	}	
	if(pBri->GetTypeRigidFootingEQ()==2) 
	{
		XLText->AddFormatText("$h$r$r: 안전측으로하여 무한강성으로 한다.$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		delete pCalcPile;
		return; // 기초를 고정으로 처리하는 경우
	}
	XLText->OutXL();

	double dScaleDim = 100;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim*5);
	DrawFootingPileBase(pDom,nJijum);
	*pCalcPile->m_pDomPlan << *pDom;

	CXLControl *pXL = XLText->GetExcelControl();	
	long nRowExcel = CXLFormatText::GetRowPostion();
	nRowExcel = pCalcPile->WritePileCondition(pXL,nRowExcel,COL_POS_1,TRUE,nNumberType);
	nRowExcel = pCalcPile->WritePileAttribute(pXL,nRowExcel,COL_POS_1,TRUE,nNumberType);	
	XLText->GoPosition(nRowExcel,COL_POS_2);

		// K1 - // 수평스프링정수, t/m
		// K2 - // t.m/m
		// K3 - // t.m/m
		// K4 - // t.m/rad
	long nIndexMiddle = 5;
	double K1 = GetValueUnitChange(pCalcPile->m_K1_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH],UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double K2 = GetValueUnitChange(pCalcPile->m_K2_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH],UNIT_CGS_TONF,pData->m_szTonf);
	double K3 = GetValueUnitChange(pCalcPile->m_K3_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH],UNIT_CGS_TONF,pData->m_szTonf);
	double K4 = GetValueUnitChange(pCalcPile->m_K4_Fix[bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH],UNIT_CGS_TONFM,pData->m_szTonfM);
	CString strHead = bAbutWall ? "$h$i" : "$h";
	XLText->AddFormatText("$s+05%s%s$r축직각방향 스프링정수 : 말뚝머리 고정인 경우$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$rK1$r$c=$r4EIβ³$m+06$c=$r%s %s$n"  , 
		pData->GetStrDouble(K1, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 8, TRUE), pData->m_szTonf_M);
	XLText->AddFormatText("$h$rK2$r$c=$r2EIβ²$m+06$c=$r%s %s/rad$n",
		pData->GetStrDouble(K2, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 8, TRUE), pData->m_szTonf);
	XLText->AddFormatText("$h$rK3$r$c=$r2EIβ²$m+06$c=$r%s %s/rad$n",
		pData->GetStrDouble(K3, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 8, TRUE), pData->m_szTonf);
	XLText->AddFormatText("$h$rK4$r$c=$r2EIβ  $m+06$c=$r%s %s/rad$n",
		pData->GetStrDouble(K4, UNIT_CGS_TONFM, FALSE, 0, FALSE, 8, TRUE), pData->m_szTonfM);
	//
	double dX = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPX]  ,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dY = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPY]  ,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dZ = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPZ]  ,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double rX = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_ROTATEX],UNIT_CGS_TONFM,pData->m_szTonfM);
	double rY = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_ROTATEY],UNIT_CGS_TONFM,pData->m_szTonfM);
	double rZ = GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_ROTATEZ],UNIT_CGS_TONFM,pData->m_szTonfM);
	XLText->AddFormatText("$s+07%s%s$r말뚝기초의 스프링 강성$n",strHead,GetNumberString(nIndexMiddle++,nNumberType));
	XLText->AddFormatText("$h$rKxt$r$c=$r∑K1$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(dX, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 12), pData->m_szTonf);
	XLText->AddFormatText("$h$rKyt$r$c=$r∑K1$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(dY, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 12), pData->m_szTonf);
	XLText->AddFormatText("$h$rKzt$r$c=$r∑Kv$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(dZ, UNIT_CGS_TONF_M, FALSE, 0, FALSE, 12), pData->m_szTonf);
	XLText->AddFormatText("$h$rKxr$r$c=$rKv x ∑yi²+ ∑K4$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(rX, UNIT_CGS_TONFM, FALSE, 0, FALSE, 12, TRUE), pData->m_szTonfM);
	XLText->AddFormatText("$h$rKyr$r$c=$rKv x ∑xi²+ ∑K4$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(rY, UNIT_CGS_TONFM, FALSE, 0, FALSE, 12, TRUE), pData->m_szTonfM);
	XLText->AddFormatText("$h$rKzr$r$c=$rK1 x (∑xi²+ ∑yi²)$m+06$c=$r%s %s/m$n",
		pData->GetStrDouble(rZ, UNIT_CGS_TONFM, FALSE, 0, FALSE, 12, TRUE), pData->m_szTonfM);

	*XLText += 1;
	//////////////////////////////////////////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(bAbutWall && pBri->GetTypeRigidFootingEQ()==0)
	{
		XLText->AddFormatText("$s+10$h%s$r벽체강성과 기초강성의 조합$n",GetNumberString(nIndexTitle++,NUM_CIRCLE));
		WriteCalcFootingSpring_Combine(XLText,pBri,nJijum);
	}

	delete pCalcPile;
	delete pDom;
}

void CARcBridgeCalcEarthQuake::WriteCalcLoadDirX(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	HGBOOL bLSD = pData->IsLsdDesign();
	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+45$h$a%d.%d 종방향 지진하중$n",nIndexTitle,nIndexMiddle);

	//////////////////////////////////////////////////////////////////////////
	double WeightUpper = GetValueUnitChange(m_dWeightUpper,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	if(pBri->GetTypeUpperWeightEQ()==0)
		XLText->AddFormatText("$n$h %d)$r상부구조 자중 계산$n",nIndexSmall++);
	else
		XLText->AddFormatText("$n$h %d)$r상부구조 자중 계산 (하부중량 1/2 포함)$n",nIndexSmall++);
	WriteCalcLoadUpperWeight(XLText,pBri,nIndexTiny);

	//////////////////////////////////////////////////////////////////////////
	double Vsx = GetMaxDisplacementUpper(TRUE);
	CString strVsx(_T("")), strVsxUnit(_T(""));
	strVsx.Format(bLSD==TRUE ? _T("%.6f") : _T("%.9f"), Vsx);
	strVsxUnit.Format(bLSD==TRUE ? _T("mm") : _T("m"));

	double dLengthModel = bLSD==TRUE ? pBri->GetLengthModelSpan(-1) : toM(pBri->GetLengthModelSpan(-1));
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r단위하중재하 및 변위계산$n",nIndexSmall++);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	CDomyun *pDom = new CDomyun;
	long nSizeRow = 11;
	DrawDiagramUnitLoad(pDom,nSizeRow,TRUE);	
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$r→ 종방향 최대변위 V(x) = %s %s$n$n",strVsx, strVsxUnit);
	//
	CString strAlpha(_T("")), strBeta(_T("")), strGamma(_T("")), strPe(_T(""));
	CString strM2Unit(_T(""));
	double alpha = GetValueUnitChange(m_alpha[CALC_DIR_X], UNIT_CGS_M2, pData->m_szM2);
	double beta  = GetValueUnitChange(m_beta[CALC_DIR_X],UNIT_CGS_TONFM,pData->m_szTonfM);
	double gamma = GetValueUnitChange(m_gamma[CALC_DIR_X],UNIT_CGS_TONFM,pData->m_szTonfM);
	if(bLSD==TRUE) gamma *= 1000;
	double Pe    = GetValueUnitChange(m_Pe[CALC_DIR_X],UNIT_CGS_TONF_M,pData->m_szTonf_M);

	strAlpha.Format(bLSD==TRUE ? _T("%.6f") : _T("%.9f"), alpha);
	strBeta.Format(bLSD==TRUE ? _T("%.6f") : _T("%.9f"), beta);
	strGamma.Format(bLSD==TRUE ? _T("%.6f") : _T("%.9f"), gamma);

	strM2Unit.Format(bLSD==TRUE ? _T("mm") : _T("m²"));

	XLText->AddFormatText("$h %d)$rα,β,γ 계수값 산정$n",nIndexSmall++);
	XLText->AddFormatText("$h$r$iW(x)$r$c=$r$me2$f3$i%s$m+03%s$n",pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	
	XLText->AddFormatText("$h$r$iα$r$c=$r$p[EQ_ALPHA_X]$m+05$c=$r%s x %s$m+08$c=$r%s %s$n$n",strVsx, pData->GetStrDouble(dLengthModel, UNIT_CGS_M), strAlpha, strM2Unit);
	XLText->AddFormatText("$h$r$iβ$r$c=$r$p[EQ_BETA_X]$m+05$c=$r%s x %s x %s$m+08$c=$r%s %s$n$n",pData->GetStrDouble(WeightUpper,UNIT_CGS_TONF_M),strVsx,
		pData->GetStrDouble(dLengthModel,UNIT_CGS_M),strBeta,pData->m_szTonfM);
	XLText->AddFormatText("$h$r$iγ$r$c=$r$p[EQ_GAMMA_X]$m+05$c=$r%s x %s²x %s$m+08$c=$r%s %s.%s$n$n",pData->GetStrDouble(WeightUpper,UNIT_CGS_TONF_M), strVsx,
		pData->GetStrDouble(dLengthModel,UNIT_CGS_M),strGamma,pData->m_szTonf, strM2Unit);

	//
	double Po  = 1.00; // 단위하중
	double g   = (bLSD==TRUE) ? frM(pBri->GetGravity()) : pBri->GetGravity(); 

	CString strUnitg = bLSD==TRUE ? _T("m/sec") : _T("mm/sec");
	XLText->AddFormatText("$n$h %d)$r교량의 주기 계산$n",nIndexSmall++);
	//	XLText->AddFormatText("$h$r$r$r$p[EQ_T]$m+06;$r g  : 중력가속도 (%.3f m/sec)",g);
	
	XLText->AddFormatText("$h$r$r$r$p[EQ_T]$m+06;$r g  : 중력가속도 (%s %s)", pData->GetStrDouble(g,UNIT_CGS_M), strUnitg);

//	XLText->AddFormatText("$h$r$md1$cT$r$md1$c=$m+08 Po : 가상 정적 단위하중 (%.3f %s)$n$n",Po,pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$md1$cT$r$md1$c=$m+08 Po : 가상 정적 단위하중 (%s %s)$n$n",pData->GetStrDouble(Po,UNIT_CGS_TONF_M), pData->m_szTonf_M);

//	XLText->AddFormatText("$h$r$r$c=$r2π x √( %.9f / (%.3f x %.3f x %.9f) )$n",gamma,Po,g,m_alpha[CALC_DIR_X]);
	XLText->AddFormatText("$h$r$r$c=$r2π x √( %s / (%s x %s x %s) )$n",strGamma, pData->GetStrDouble(Po, UNIT_CGS_TONF_M), pData->GetStrDouble(g,UNIT_CGS_M), strAlpha);
	XLText->AddFormatText("$h$r$r$c=$r%.3f (sec)$n$n", m_T[CALC_DIR_X]);
	//
	double A   = pBri->GetCoefficientAccelerateEQ(); 
	double S   = pBri->GetCoefficientGround();
	XLText->AddFormatText("$s+15$h %d)$r탄성지진 응답계수 계산$n",nIndexSmall++);
	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		XLText->AddFormatText("$h$r$r$r$p[EQ_CS]$m+06;$r A  : 가속도 계수 (%.3f x %.3f = %.3f)",pBri->GetCoefficientDangerEQ(),pBri->GetCoefficientRegionEQ(), A);  // 단위 없음 - LSD 구분없음

		XLText->AddFormatText("$h$r$md1$cCs$r$md1$c=$m+08 S : 토질 형상 특성에 대한 무차원 계수$n$n");
		XLText->AddFormatText("$h$r$r$c=$r1.2 x %.3f x %.3f / %.3f^⅔$n",A,S,m_T[CALC_DIR_X]);
		XLText->AddFormatText("$h$r$r$c=$r%.3f (≤ 2.5 x A), ∴ Cs = %.3f 적용$n$n",1.2*A*S/pow(m_T[CALC_DIR_X],2.0/3.0),m_Cs[CALC_DIR_X]);
	}
	else
	{
		WriteResponseFactor_KDS17(XLText, pBri, TRUE);
	}
	//
	XLText->AddFormatText("$h %d)$r등가 정적 지진하중 계산$n",nIndexSmall++);
	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		XLText->AddFormatText("$h$r$r$r$p[EQ_PE]");
		XLText->AddFormatText("$h$r$md1$cPe$r$md1$c=$n$n");
		//	XLText->AddFormatText("$h$r$r$c=$r%.3f x %.3f / %.9f x %.3f x %.9f$n$n",beta,m_Cs[CALC_DIR_X],gamma,WeightUpper,Vsx);
		XLText->AddFormatText("$h$r$r$c=$r%s x %.3f / %s x %s x %s$n$n",pData->GetStrDouble(beta, UNIT_CGS_TONFM),m_Cs[CALC_DIR_X],strGamma,pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M),strVsx);
		XLText->AddFormatText("$h$r$r$c=$r%s %s$n$n",pData->GetStrDouble(Pe, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	}
	else
	{
		XLText->AddFormatText(_T("$h$rPe(x)$m+02$c=$rβ·%s / γ x ω(x)vs(x)$n$h"), _T("Sa"));
		XLText->AddFormatText("$h$m+03$c=$r%s x %.3f / %s x %s x %s$n",pData->GetStrDouble(beta, UNIT_CGS_TONFM), m_Cs[CALC_DIR_X],strGamma,pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M),strVsx);
		XLText->AddFormatText("$h$m+03$c=$r%s %s$n$n",pData->GetStrDouble(Pe, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	}
}

void CARcBridgeCalcEarthQuake::WriteCalcLoadDirY(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$a%d.%d 횡방향 지진하중",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$m+06 : 횡방향 강성이 매우 크므로 생략한다.$n$n",nIndexSmall++);
	return;

	HGBOOL bLSD = pData->IsLsdDesign();
	//////////////////////////////////////////////////////////////////////////
	double WeightUpper = GetValueUnitChange(m_dWeightUpper,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$n$h %d)$r상부구조 자중 계산$n",nIndexSmall++);
	WriteCalcLoadUpperWeight(XLText,pBri,nIndexTiny);
	//
	XLText->AddFormatText("$n$h %d)$r단위하중재하 및 변위계산$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	CDomyun *pDom = new CDomyun;
	long nSizeRow = 11;
	DrawDiagramUnitLoad(pDom,nSizeRow,FALSE);	
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	//
	double alpha = bLSD==TRUE ? frM2(m_alpha[CALC_DIR_Y]) : m_alpha[CALC_DIR_Y];
	double beta  = GetValueUnitChange(m_beta[CALC_DIR_Y],UNIT_CGS_TONF,pData->m_szTonf);
	double gamma = GetValueUnitChange(m_gamma[CALC_DIR_Y],UNIT_CGS_TONF,pData->m_szTonf);

	CString strUnit = bLSD==TRUE ? _T("mm") : _T("m²");
	CString strAlpha(_T("")), strBeta(_T("")), strGamma(_T(""));
	strAlpha.Format(bLSD==TRUE ? _T("%.7f") : _T("%.10f"), alpha);
	strBeta.Format(bLSD==TRUE ? _T("%.7f") : _T("%.10f"), beta);
	strGamma.Format(bLSD==TRUE ? _T("%.7f") : _T("%.10f"), gamma);

	XLText->AddFormatText("$n$h %d)$rα,β,γ 계수값 산정$n",nIndexSmall++);
	XLText->AddFormatText("$h$r$iW(y)$r$c=$r$me1$f3$i%s$r$r%s$n",pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$iα$r$c=$r$p[EQ_ALPHA_Y]$m+05$c=$r%s %s$n$n",strAlpha, strUnit);
	XLText->AddFormatText("$h$r$iβ$r$c=$r$p[EQ_BETA_Y]$m+05$c=$r%s %s$n$n",strBeta,pData->m_szTonfM);
	XLText->AddFormatText("$h$r$iγ$r$c=$r$p[EQ_GAMMA_Y]$m+05$c=$r%s %s.m²$n$n",strGamma,pData->m_szTonf);

	//
	double Po  = GetValueUnitChange(1.00, UNIT_CGS_TONF_M, pData->m_szTonf_M)  ; // 단위하중
	double g   = bLSD==TRUE ? frM(pBri->GetGravity()) : pBri->GetGravity(); 
	
	strUnit = bLSD==TRUE ? _T("mm/sec") : _T("m/sec");
	XLText->AddFormatText("$n$h %d)$r교량의 주기 계산$n",nIndexSmall++);
	XLText->AddFormatText("$h$r$r$r$p[EQ_T]$m+06;$r g  : 중력가속도 (%s %s)$n", pData->GetStrDouble(g, UNIT_CGS_M),strUnit);
	XLText->AddFormatText("$h$r$cT$r$c=$m+08 Po : 가상 정적 단위하중 (%s %s)$n$n",pData->GetStrDouble(Po,UNIT_CGS_TONF_M),pData->m_szTonf_M);
	XLText->AddFormatText("$h$r$r$c=$r2π x √｛ %s /（%s x %s x %s）｝$n",strGamma,pData->GetStrDouble(Po,UNIT_CGS_TONF_M),pData->GetStrDouble(g, UNIT_CGS_M),strAlpha);
	XLText->AddFormatText("$h$r$r$c=$r%.3f (sec)$n$n",m_T[CALC_DIR_Y]);

	//
	double A   = pBri->GetCoefficientAccelerateEQ(); 
	double S   = pBri->GetCoefficientGround();
	XLText->AddFormatText("$h %d)$r탄성지진 응답계수 계산$n",nIndexSmall++);
	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		XLText->AddFormatText("$h$r$r$r$p[EQ_CS]$m+06;$r A  : 가속도 계수 (%.3f x %.3f = %.3f)$n",pBri->GetCoefficientDangerEQ(),pBri->GetCoefficientRegionEQ(), A);
		XLText->AddFormatText("$h$r$cCs$r$c=$m+08 S : 토질 형상 특성에 대한 무차원 계수$n$n");
		XLText->AddFormatText("$h$r$r$c=$r1.2 x %.3f x %.3f / %.3f^⅔$n",A,S,m_T[CALC_DIR_Y]);
		XLText->AddFormatText("$h$r$r$c=$r%.3f (≤ 2.5 x A), ∴ Cs = %.3f 적용$n$n",1.2*A*S/pow(m_T[CALC_DIR_Y],2.0/3.0),m_Cs[CALC_DIR_Y]);
	}
	else
	{
		WriteResponseFactor_KDS17(XLText, pBri, TRUE);
	}

	//
	double dCalc = GetValueUnitChange((beta*m_Cs[CALC_DIR_Y]/gamma)*WeightUpper,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$s+10$h %d)$r등가 정적 지진하중 계산$n",nIndexSmall++);
	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		XLText->AddFormatText("$h$r$r$r$p[EQ_PE]$n");
		XLText->AddFormatText("$h$r$cPe$r$c=$n$n");
		XLText->AddFormatText("$h$r$r$c=$r%s x %.3f / %s x %s x Vs(y)$n",strBeta,m_Cs[CALC_DIR_Y],strGamma,pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$r$r$c=$r%s  x Vs(y) %s$n$n",pData->GetStrDouble(dCalc, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	}
	else
	{
		XLText->AddFormatText(_T("$h$rPe$r$c=$rβ·%s / γ x ω(y)Vs(y)$n$h"), pData->m_nTypeEarthQuakeApply == 0 ? _T("Cs") : _T("Sa"));
		XLText->AddFormatText("$h$r$r$c=$r%s x %.3f / %s x %s x Vs(y)$n", strBeta,m_Cs[CALC_DIR_Y],strGamma,pData->GetStrDouble(WeightUpper, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$r$r$c=$r%s x Vs(y) %s$n$n",pData->GetStrDouble(dCalc, UNIT_CGS_TONF_M),pData->m_szTonf_M);
	}
	//
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h※ 횡방향 하중에 대한 변위 및 등가 정적 지진하중$n");
	char *Title[] = { "절점", "Vy", "Iy", "Vy×Iy", "Vy×Iy×Wy", "Iy×Wy×Vy²", "Pe" };
	int nWidthJoint[] = { 2, 4, 3, 4, 4, 4, 3 };
	long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0]);
	long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0]);

	nSizeRow = nIdxJointEnd - nIdxJointStt + 2; // 타이틀,계
	long nSizeCol = 7;
	XLText->AddTable(nSizeRow-1,24-1,1,1);
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
	}
	long i=0; for(i=1; i<nSizeRow; i++)
	{
		CString str = _T("");
		for(j=0; j<nSizeCol; j++)
		{
			if(i==nSizeRow-1)
			{
				if(j==0) str = "계";
				if(j==1) str = "'-";
//				if(j==2) str = "$f3=SUM({IYS}:{IYE})";
				if(j==2) str.Format("$f3%.3f",toM(pBri->GetLengthModelSpan(-1)));
				if(j==3) str.Format("$f[0.0000000000]%s",strAlpha);
				if(j==4) str.Format("$f[0.0000000000]%s",strBeta);
				if(j==5) str.Format("$f[0.0000000000]%s",strGamma);
				if(j==6) str = "'-";
			}
			else
			{
				long nJoint = nIdxJointStt + i -1;
				double Wy  = WeightUpper;
				double Vsy = GetDisplacement(nJoint,FALSE);
				double Iy  = m_pStd->m_pFrame->GetLengthBetweenJoint(&pBri->m_FemEQ[0],nJoint,nJoint+1);// 해당절점이 차지하는 길이
				double Pe  = GetValueUnitChange(m_ArrPe.GetAt(i-1),UNIT_CGS_TONF,pData->m_szTonf);
				if(j==0) str.Format("%d",nJoint+1);
				if(j==1) str.Format(bLSD==TRUE ? "$f[0.0000000000]%.7f" : "$f[0.0000000000]%.10f",Vsy);
				//				if(j==2) str.Format(i==1 ? "${IYS}$f3%.3f" : i==nSizeRow-2 ? "${IYE}$f3%.3f" : "$f3%.3f",Iy);
				if(j==2) str.Format("$f3%.3f",pData->GetStrDouble(Iy,UNIT_CGS_M));
				if(j==3) str.Format(bLSD==TRUE ? "$f[0.0000000000]%.7f" : "$f[0.0000000000]%.10f",Vsy*Iy);
				if(j==4) str.Format(bLSD==TRUE ? "$f[0.0000000000]%.7f" : "$f[0.0000000000]%.10f",Vsy*Iy*Wy);
				if(j==5) str.Format(bLSD==TRUE ? "$f[0.0000000000]%.7f" : "$f[0.0000000000]%.10f",Vsy*Vsy*Iy*Wy);
				if(j==6) str.Format("$f3%s", pData->GetStrDouble(Pe,UNIT_CGS_TONF));
			}
			if(j==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
	}
}

void CARcBridgeCalcEarthQuake::WriteCalcLoadInertialForce(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	if(pBri->IsBoxType() == FALSE) return;

	long nSizeEq = pBri->GetCountEarthQuakeModel();

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+%d$h$a%d.%d 구체 관성력$n$n", nSizeEq < 2 ? 10 : 20, nIndexTitle,nIndexMiddle);

	// 구체관성력 : 두께 x 콘크리트
	XLText->AddFormatText("$h fi = Ai × rcon'c × Kh$n");

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	HGBOOL bLSD = pData->IsLsdDesign();

	double dUW = GetValueUnitChange(m_UW_Conc, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dThick[4], dIner[4];
	dThick[0] = bLSD==TRUE ? pBri->m_dTS : toM(pBri->m_dTS);
	dThick[1] = bLSD==TRUE ? pBri->m_dTS_Lower : toM(pBri->m_dTS_Lower);
	dThick[2] = bLSD==TRUE ? pBri->m_dWS : toM(pBri->m_dWS);
	dThick[3] = bLSD==TRUE ? pBri->m_dWE : toM(pBri->m_dWE);

	double dApplyWidth = m_pStd->GetSlabWidth(-1);

	char *Title1[] = { "구분", "산식", "관성력(kN/m)"};
	if ( bLSD == TRUE )
		Title1[2] = "관성력(N/mm)";
	char *Title2[] = { "상부슬래브", "하부슬래브", "시점벽체", "종점벽체"};
	long nWidthTitle[] = {5, 9, 5};
	long nSizeRow = 5;
	long nSizeCol = 3;
	for(long nCase = 0; nCase < eGoalEarthQuakeCount; nCase++)
	{
		EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);
		if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;

		double Kh = m_pStd->GetKhValue(FALSE, TRUE, eGoalEq);
		double dWidth = bLSD==TRUE ? frM(m_pStd->GetSlabWidth(-1)) : dApplyWidth;

		*XLText += 1;
		XLText->GoPosition(NO_MOVE, 1);
		if(nSizeEq > 1)
		{
			XLText->AddFormatText("$h %d) %s$n", nCase + 1, pBri->GetStringGoalEarthQuake(eGoalEq));
		}

		dIner[0] = GetValueUnitChange(m_InertialForce_Slab[nCase][iUPPER], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		dIner[1] = GetValueUnitChange(m_InertialForce_Slab[nCase][iLOWER], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		dIner[2] = GetValueUnitChange(m_InertialForce_Wall[nCase][iLEFT], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		dIner[3] = GetValueUnitChange(m_InertialForce_Wall[nCase][iRIGHT], UNIT_CGS_TONF_M, pData->m_szTonf_M);

		XLText->GoPosition(NO_MOVE, 2);
		XLText->AddTable(nSizeRow-1, 19-1, 1, 0);
		for(long j=0; j <nSizeCol; j++)
		{
			XLText->AddMergeFormatText(0, nWidthTitle[j]-1, "$c%s", Title1[j]);
			XLText->AddFormatText("$m+0%d" ,nWidthTitle[j]);
		}

		for(long ix = 0; ix < 4 ; ix++)
		{
			XLText->AddText("$n");
			XLText->GoPosition(NO_MOVE, 2);
			XLText->AddMergeFormatText(0, nWidthTitle[0]-1, "$c%s", Title2[ix]);
			XLText->AddFormatText("$m+%02d",nWidthTitle[0]);

			XLText->AddMergeFormatText(0, nWidthTitle[1]-1,"$c%s ⅹ %s ⅹ %.3f x %s", pData->GetStrDouble(dThick[ix],UNIT_CGS_M), pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3), 
				Kh, pData->GetStrDouble(dWidth, UNIT_CGS_M));
			XLText->AddFormatText("$m+%02d", nWidthTitle[1]);

			XLText->AddMergeFormatText(0,nWidthTitle[2]-1,"$c$f3%s", pData->GetStrDouble(dIner[ix], UNIT_CGS_TONF_M));
			XLText->AddFormatText("$m+%02d",nWidthTitle[2]);

		}
		*XLText += 1;
	}
	*XLText += 1;
}


void CARcBridgeCalcEarthQuake::WriteCalcLoadSoilPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	HGBOOL bLSD = pData->IsLsdDesign();

	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	double A     = pBri->GetCoefficientAccelerateEQ();
	double Kae1   = m_pStd->GetCoefficient_MononobeOkabe(0);
	double Kae2   = m_pStd->GetCoefficient_MononobeOkabe(1);
	double Ka    = m_pStd->GetCoefficient_coulomb();
	double Kh1	 = m_pStd->GetKhValue(FALSE, TRUE, eGoalEQ_None);
	double Kh2	 = m_pStd->GetKhValue(FALSE, FALSE, eGoalEQ_None);
	double Kv	 = 0;
	double theta1 = atan(Kh1/(1-Kv));	// tan-1(Kh/(1-Kv));
	double theta2 = atan(Kh2/(1-Kv));	// tan-1(Kh/(1-Kv));
	double r     = GetValueUnitChange(pBri->m_dUnitWeigthBackFill,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double H1    = toM(pBri->GetHeightModelJijum(0));
	double H2    = toM(pBri->GetHeightModelJijum(pBri->m_nQtyJigan));
	double Bs    = GetValueUnitChange(m_pStd->GetSlabWidth(), UNIT_CGS_M, pData->m_szM);	// 슬래브폭원(교량폭,벽체폭)
	if(pBri->GetTypeJointPosFootingEQ()==1)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);
		double FH = pFooting->GetHeight();
		H1 += toM(FH)/2;

		pFooting = pBri->GetFootingByJijumNumber(pBri->m_nQtyJigan);
		FH = pFooting->GetHeight();
		H2 += toM(FH)/2;
	}

	if( bLSD==TRUE )
	{
		H1 = GetValueUnitChange(H1, UNIT_CGS_M, pData->m_szM);
		H2 = GetValueUnitChange(H2, UNIT_CGS_M, pData->m_szM);
	}

	double dS = pBri->GetCoefficientGround();
	CString strUnit = bLSD==TRUE ? _T("mm") : _T("m");
	CString strMarkA = pData->m_nTypeEarthQuakeApply == 0 ? _T("A") : _T("S");
	CString strKh = _T("");
	CString strCoefGround(_T("")), strValueCoefGround(_T(""));
	if(dS != 1.0 && pBri->m_nSelFactorKhStt > 3)
	{
		strCoefGround = pData->m_nTypeEarthQuakeApply == 0 ? _T(" x S") : _T(" x 지반보정계수");
		strValueCoefGround.Format(_T(", %s=%.2f"), pData->m_nTypeEarthQuakeApply == 0 ? _T("S") : _T("지반보정계수"), dS);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+20$h$a%d.%d 지진시 토압$n",nIndexTitle,nIndexMiddle);
	//
	XLText->AddFormatText("$n$h %d)$r지진시 주동토압$n",nIndexSmall++);
	nIndexTiny = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_2); 
	XLText->AddFormatText("$h%s$r지진시 주동토압계수(Mononobe-Okabe식 적용)$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	if(pBri->m_nSelFactorKhStt != pBri->m_nSelFactorKhEnd) XLText->AddFormatText("$h$r%s$r시점측 벽체 주동토압계수$n",GetNumberString(1, NUM_GANA));
	
	if(pBri->m_nSelFactorKhStt == 1 || pBri->m_nSelFactorKhStt == 0 || pBri->m_nSelFactorKhStt == 4) strKh.Format(_T("%s/2%s (변위를 허용한 교대)"), strMarkA, strCoefGround);
	else if(pBri->m_nSelFactorKhStt == 2 || pBri->m_nSelFactorKhStt == 5) strKh.Format(_T("%s x 1.0%s"), strMarkA, strCoefGround);
	else if(pBri->m_nSelFactorKhStt == 3 || pBri->m_nSelFactorKhStt == 6) strKh.Format(_T("%s x 1.5%s"), strMarkA, strCoefGround);
	
	XLText->AddFormatText("$h$r$r$r$p[지진시주동토압계수]$n");
	XLText->AddFormatText("$h$r$p[K_AE]$r$c=$n$n");
	XLText->AddFormatText("$h$r$r$c=$r'%.3f$n",Kae1);
	XLText->AddFormatText("$h$m+02여기서,$r$r$cΦ$r$c=$r뒷채움 흙의 내부마찰각( = %.2f 도)$n",pBri->m_dAngleFrictionBackFill);
	XLText->AddFormatText("$h$m+04$cα$r$c=$r지표면과 수평면이 이루는 각( = 0 도)$n");
	XLText->AddFormatText("$h$m+04$cβ$r$c=$r벽배면과 연직면이 이루는 각( = 0 도)$n");
	XLText->AddFormatText("$h$m+04$cδ$r$c=$r벽배면과 뒷채움흙사이의 벽면마찰각( = 0 도)$n");
	XLText->AddFormatText("$h$m+04$cKh$r$c=$r수평지진계수 = %s, %s=%.3f%s$n", strKh, pData->m_nTypeEarthQuakeApply == 0 ? _T("A") : _T("S"), A, strValueCoefGround);
	XLText->AddFormatText("$h$m+04$cθ$r$c=$rtan-1[Kh/(1-Kv)] = %.3f, Kv = %.3f$n",ToDegree(theta1),Kv);
	
	// 설계선택사항에서 시점측과 종점측의 계수 선택이 다를때 시점과 종점을 따로 찍어준다.
	if(pBri->m_nSelFactorKhStt != pBri->m_nSelFactorKhEnd)
	{
		if(dS != 1.0 && pBri->m_nSelFactorKhEnd > 3)
		{
			strCoefGround = pData->m_nTypeEarthQuakeApply == 0 ? _T(" x S") : _T(" x 지반보정계수");
			strValueCoefGround.Format(_T(", %s=%.2f"), pData->m_nTypeEarthQuakeApply == 0 ? _T("S") : _T("지반보정계수"), dS);
		}
		else
		{
			strCoefGround = _T("");
			strValueCoefGround = _T("");
		}

		if(pBri->m_nSelFactorKhEnd == 1 || pBri->m_nSelFactorKhEnd == 0 || pBri->m_nSelFactorKhEnd == 4) strKh.Format(_T("%s/2%s (변위를 허용한 교대)"), strMarkA, strCoefGround);
		else if(pBri->m_nSelFactorKhEnd == 2 || pBri->m_nSelFactorKhEnd == 5) strKh.Format(_T("%s x 1.0%s"), strMarkA, strCoefGround);
		else if(pBri->m_nSelFactorKhEnd == 3 || pBri->m_nSelFactorKhEnd == 6) strKh.Format(_T("%s x 1.5%s"), strMarkA, strCoefGround);

		XLText->AddFormatText("$h$r%s$r종점측 벽체 주동토압계수$n",GetNumberString(2, NUM_GANA));
		XLText->AddFormatText("$h$r$r$r$p[지진시주동토압계수]$n");
		XLText->AddFormatText("$h$r$p[K_AE]$r$c=$n$n");
		XLText->AddFormatText("$h$r$r$c=$r'%.3f$n",Kae2);
		XLText->AddFormatText("$h$m+02여기서,$r$r$cΦ$r$c=$r뒷채움 흙의 내부마찰각( = %.0f 도)$n",pBri->m_dAngleFrictionBackFill);
		XLText->AddFormatText("$h$m+04$cα$r$c=$r지표면과 수평면이 이루는 각( = 0 도)$n");
		XLText->AddFormatText("$h$m+04$cβ$r$c=$r벽배면과 연직면이 이루는 각( = 0 도)$n");
		XLText->AddFormatText("$h$m+04$cδ$r$c=$r벽배면과 뒷채움흙사이의 벽면마찰각( = 0 도)$n");
		XLText->AddFormatText("$h$m+04$cKh$r$c=$r수평지진계수 = %s, %s=%.3f%s$n", strKh, pData->m_nTypeEarthQuakeApply == 0 ? _T("A") : _T("S"), A, strValueCoefGround);
		XLText->AddFormatText("$h$m+04$cθ$r$c=$rtan-1[Kh/(1-Kv)] = %.3f, Kv = %.3f$n", ToDegree(theta2),Kv);
	}

	if(pBri->m_nSelectModelingCountEQ==2)
	{
		double Pae1 = GetValueUnitChange(m_ToapEQ[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Pae2 = GetValueUnitChange(m_ToapEQ[1],UNIT_CGS_TONF,pData->m_szTonf);
		XLText->AddFormatText("$h%s$r지진시 주동토압 : 시점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$p[P_AE]$r$c=$r½ x Kae x r x H²x B$n");
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",Kae1, pData->GetStrDouble(r,UNIT_CGS_TONF_M3),
			pData->GetStrDouble(H1,UNIT_CGS_M) ,pData->GetStrDouble(Bs, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDouble(Pae1, UNIT_CGS_TONF),pData->m_szTonf);
		//
		XLText->AddFormatText("$h%s$r지진시 주동토압 : 종점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$p[P_AE]$r$c=$r½ x Kae x r x H²x B$n");
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",Kae2, pData->GetStrDouble(r,UNIT_CGS_TONF_M3) ,
			pData->GetStrDouble(H2, UNIT_CGS_TONF_M) ,pData->GetStrDouble(Bs, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n$n", pData->GetStrDouble(Pae2, UNIT_CGS_TONF),pData->m_szTonf);
	}
	else
	{
		BOOL bSttWall = (pBri->m_nSelectModelingCountEQ==0) ? TRUE : FALSE;
		long nIdxWall = bSttWall ? 0 : 1;
		double Pae = GetValueUnitChange(m_ToapEQ[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		CString szWallPos = bSttWall ? "시점측 벽체" : "종점측 벽체";
		XLText->AddFormatText("$h%s$r지진시 주동토압 : %s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),szWallPos);
		XLText->AddFormatText("$h$r$p[P_AE]$r$c=$r½ x Kae x r x H²x B$n");
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",bSttWall ? Kae1 : Kae2,
			pData->GetStrDouble(r, UNIT_CGS_TONF_M3), bSttWall ? pData->GetStrDouble(H1,UNIT_CGS_M) : pData->GetStrDouble(H2, UNIT_CGS_TONF_M),
			pData->GetStrDouble(Bs, UNIT_CGS_TONF_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n$n",pData->GetStrDouble(Pae, UNIT_CGS_TONF),pData->m_szTonf);
	}

	//
	XLText->GoPosition(NO_MOVE,COL_POS_1); 
	XLText->AddFormatText("$h %d)$r평상시 주동토압$n",nIndexSmall++);
	nIndexTiny = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_2); 
	XLText->AddFormatText("$h%s$r평상시 주동토압계수(Coulomb식 적용)$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	XLText->AddFormatText("$h$r$r$r$p[평상시주동토압계수]$n");
	XLText->AddFormatText("$h$r$p[K_A]$r$c=$n$n");
	XLText->AddFormatText("$h$r$r$c=$r'%.3f$n",Ka);
	if(pBri->m_nSelectModelingCountEQ==2)
	{
		double Pa1 = GetValueUnitChange(m_ToapNormal[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Pa2 = GetValueUnitChange(m_ToapNormal[1],UNIT_CGS_TONF,pData->m_szTonf);
		double dCalcH1 = H1/3.0;				// m
		double dCalcH2 = H2/3.0;				// m

		XLText->AddFormatText("$s+04$h%s$r시점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$p[P_A]$r$c=$r½ x Ka$r x r x H²x B$n");		// ATest 작업 후 수정 예정 - 2015-07-06 : Ka가 가려짐
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",Ka,
			pData->GetStrDouble(r,UNIT_CGS_TONF_M3) ,pData->GetStrDouble(H1,UNIT_CGS_M), pData->GetStrDouble(Bs,UNIT_CGS_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n", pData->GetStrDouble(Pa1, UNIT_CGS_TONF),pData->m_szTonf);
		XLText->AddFormatText("$h$r$i h$r$c=$r1/3 x %s$m+06$c=$r%s %s$n", pData->GetStrDouble(H1,UNIT_CGS_M, FALSE, 2), pData->GetStrDouble(dCalcH1, UNIT_CGS_M), strUnit);
		XLText->AddFormatText("$s+04$h%s$r종점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$p[P_A]$r$c=$r½ x Ka$r x r x H²x B$n");
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",Ka,pData->GetStrDouble(r,UNIT_CGS_TONF_M3),pData->GetStrDouble(H2, UNIT_CGS_M),
			pData->GetStrDouble(Bs,UNIT_CGS_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n$n",pData->GetStrDouble(Pa2, UNIT_CGS_TONF),pData->m_szTonf);
		XLText->AddFormatText("$h$r$i h$r$c=$r1/3 x %s$m+06$c=$r%s %s$n", pData->GetStrDouble(H2, UNIT_CGS_M, FALSE, 2), pData->GetStrDouble(dCalcH2, UNIT_CGS_M), strUnit);
	}
	else
	{
		BOOL bSttWall = (pBri->m_nSelectModelingCountEQ==0) ? TRUE : FALSE;
		long nIdxWall = bSttWall ? 0 : 1;
		double Pa = GetValueUnitChange(m_ToapNormal[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		CString szWallPos = (pBri->m_nSelectModelingCountEQ==0) ? "시점측 벽체" : "종점측 벽체";
		XLText->AddFormatText("$s+06$h%s$r평상시 주동토압 : %s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),szWallPos);
		XLText->AddFormatText("$h$r$p[P_A]$r$c=$r½ x Ka x r x H²x B$n");
		XLText->AddFormatText("$h$m+02$c=$r½ x %.3f x %s x %s²x %s$n",Ka,pData->GetStrDouble(r,UNIT_CGS_TONF_M3),bSttWall ? pData->GetStrDouble(H1,UNIT_CGS_M) : pData->GetStrDouble(H2,UNIT_CGS_M),
			pData->GetStrDouble(Bs,UNIT_CGS_M));
		XLText->AddFormatText("$h$m+02$c=$r%s %s$n$n",pData->GetStrDouble(Pa, UNIT_CGS_TONF),pData->m_szTonf);
		double dHE = (bSttWall ? H1 : H2) / 3.0;
		XLText->AddFormatText("$h$r$i h$r$c=$r1/3 x %s$m+06$c=$r%s %s$n", (bSttWall ? pData->GetStrDouble(H1,UNIT_CGS_M, FALSE, 2) : pData->GetStrDouble(H2,UNIT_CGS_M, FALSE, 2)), 
			pData->GetStrDouble(dHE,UNIT_CGS_M), strUnit);
	}
	//
	nIndexTiny = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1); 
	XLText->AddFormatText("$s+%02d$h %d)$r지진시 주동토압의 작용점 산정$n",(pBri->m_nSelectModelingCountEQ==2)?20:10,nIndexSmall++);
	if(pBri->m_nSelectModelingCountEQ==2)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2); 
		double Pae1 = GetValueUnitChange(m_ToapEQ[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Pae2 = GetValueUnitChange(m_ToapEQ[1],UNIT_CGS_TONF,pData->m_szTonf);
		double Pa1  = GetValueUnitChange(m_ToapNormal[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Pa2  = GetValueUnitChange(m_ToapNormal[1],UNIT_CGS_TONF,pData->m_szTonf);
		double dHeightToap0 = GetValueUnitChange(m_HeightToap[0],UNIT_CGS_M,pData->m_szM);
		double dHeightToap1 = GetValueUnitChange(m_HeightToap[1],UNIT_CGS_M,pData->m_szM);

		XLText->AddFormatText("$h%s$r평상시 주동토압 : 시점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$i$p[D_P_AE]$r$c=$r%s - %s$m+07$c=$r%s %s$n",pData->GetStrDouble(Pae1, UNIT_CGS_TONF),
			pData->GetStrDouble(Pa1, UNIT_CGS_TONF),pData->GetStrDouble(Pae1-Pa1, UNIT_CGS_TONF), pData->m_szTonf);
		if(pBri->GetTypeSoilPressureEQ()==0)
		{
			XLText->AddFormatText("$h$r$md1$ch$r$md1$c=$r$p[P_AE_H]$n$n");
			XLText->AddFormatText("$h$r$r$c=$r（(%s x %s / 3) + (%s x 0.6 x %s)）/ %s$n", pData->GetStrDouble(Pa1, UNIT_CGS_TONF), pData->GetStrDouble(H1, UNIT_CGS_M),
				pData->GetStrDouble(Pae1-Pa1, UNIT_CGS_TONF), pData->GetStrDouble(H1, UNIT_CGS_M), pData->GetStrDouble(Pae1, UNIT_CGS_TONF));
			XLText->AddFormatText("$h$r$r$c=$r%s %s$n",pData->GetStrDouble(dHeightToap0,UNIT_CGS_M), strUnit);
		}
		else
		{
			HGDOUBLE dH = 0.6*H1;
			XLText->AddFormatText("$h$r$ch$r$c=$r0.6 x %s$m+07$c=$r%s %s$n",pData->GetStrDouble(H1, UNIT_CGS_M), pData->GetStrDouble(dH, UNIT_CGS_M), strUnit);
		}
		//
		XLText->AddFormatText("$h%s$r평상시 주동토압 : 종점측 벽체$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$r$i$p[D_P_AE]$r$c=$r%s - %s$m+07$c=$r%s %s$n", pData->GetStrDouble(Pae2,UNIT_CGS_TONF) ,pData->GetStrDouble(Pa2,UNIT_CGS_TONF),
			pData->GetStrDouble(Pae2-Pa2,UNIT_CGS_TONF) ,pData->m_szTonf);
		if(pBri->GetTypeSoilPressureEQ()==0)
		{
			XLText->AddFormatText("$h$r$md1$ch$r$md1$c=$r$p[P_AE_H]$n$n");
			XLText->AddFormatText("$h$r$r$c=$r（(%s x %s / 3) + (%s x 0.6 x %s)）/ %s$n",pData->GetStrDouble(Pa2,UNIT_CGS_TONF),pData->GetStrDouble(H2,UNIT_CGS_TONF),
				pData->GetStrDouble(Pae2-Pa2,UNIT_CGS_TONF), pData->GetStrDouble(H2,UNIT_CGS_M), pData->GetStrDouble(Pae2,UNIT_CGS_TONF));
			XLText->AddFormatText("$h$r$r$c=$r%s %s$n", pData->GetStrDouble(dHeightToap1,UNIT_CGS_M), strUnit);
		}
		else
		{
			double dH2 = 0.6*H2;
			XLText->AddFormatText("$h$r$ch$r$c=$r0.6 x %s$m+07$c=$r%s %s$n",pData->GetStrDouble(H2,UNIT_CGS_M),pData->GetStrDouble(dH2,UNIT_CGS_M), strUnit);
		}
	}
	else
	{
		BOOL bSttWall = (pBri->m_nSelectModelingCountEQ==0) ? TRUE : FALSE;
		long nIdxWall = bSttWall ? 0 : 1;
		double Pae = GetValueUnitChange(m_ToapEQ[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		double Pa  = GetValueUnitChange(m_ToapNormal[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		double dHeightToap = GetValueUnitChange(m_HeightToap[nIdxWall],UNIT_CGS_M,pData->m_szM);
		double H   = bSttWall ? H1 : H2;
		XLText->AddFormatText("$h$r$i$p[D_P_AE]$r$c=$r%s - %s$m+07$c=$r%s %s$n",pData->GetStrDouble(Pae,UNIT_CGS_TONF),pData->GetStrDouble(Pa,UNIT_CGS_TONF),pData->GetStrDouble(Pae-Pa,UNIT_CGS_TONF),
			pData->m_szTonf);
		if(pBri->GetTypeSoilPressureEQ()==0)
		{
			XLText->AddFormatText("$h$r$md1$ch$r$md1$c=$r$p[P_AE_H]$n$n");
			XLText->AddFormatText("$h$r$r$c=$r（(%s x %s / 3) + (%s x 0.6 x %s)）/ %s$n",pData->GetStrDouble(Pa,UNIT_CGS_TONF),pData->GetStrDouble(H,UNIT_CGS_M),
				pData->GetStrDouble(Pae-Pa,UNIT_CGS_TONF), pData->GetStrDouble(H,UNIT_CGS_M), pData->GetStrDouble(Pae,UNIT_CGS_TONF));
			XLText->AddFormatText("$h$r$r$c=$r%s %s$n",pData->GetStrDouble(dHeightToap,UNIT_CGS_M), strUnit);
		}
		else
		{
			double dH = GetValueUnitChange(0.6*toM(H), UNIT_CGS_M, pData->m_szM);
			XLText->AddFormatText("$h$r$ch$r$c=$r0.6 x %s$m+07$c=$r%s %s$n",pData->GetStrDouble(H,UNIT_CGS_M),pData->GetStrDouble(dH,UNIT_CGS_M), strUnit);
		}
	}
}

double CARcBridgeCalcEarthQuake::GetScaleDiagram(long nRow)
{
	double dHeight = nRow * CXLFormatText::GetRowHeight() * 25/19;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = &pBri->m_FemEQ[0];

	CForceDiagram FDiagram(pFEM);
	FDiagram.SetCWnd(m_pStd->m_pView);
	
	FDiagram.DrawFrameModel(FALSE,FALSE,FALSE);

	FDiagram.SetCalcExtRect();
	CDRect rect = FDiagram.GetExtRect();		
	double dR  = rect.Width() / rect.Height();			
	long nCWidth = (long)(MIN(XFL_PAGEWIDTH,dHeight*dR));	

	double dScale = rect.Width() / (double)nCWidth * 3;
	return dScale;
}

void CARcBridgeCalcEarthQuake::DrawFootingPileBase(CDomyun *pDomP, long nJijum)
{
	CRcBridgeRebar	  *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	// GetFooting의 인자는 실로 존재하는 인덱스이므로 nJijum을 쓰면 파이형 이나 경사교각에서 잘못된 결과를 가져온다.
//	long nIdxFoot = (pBri->IsFooting(TRUE))? nJijum : nJijum -1;
//	CFootingApp* pFoot = pBri->GetFooting(nIdxFoot);
	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(pFoot==NULL) return;

	int nCountJijum = pBri->GetCountJijum();
	BOOL bInWall = nJijum!=0 && nJijum!=nCountJijum-1;

	CExPileApp *pPile = &(pFoot->m_exFooting.m_Pile);
	CDrawExPile drawExPile(pPile, m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	// 기초 평면
	pFoot->GetTvPlan();
	DBStd.m_pDrawFooting->DrawPlan(&Dom, pFoot, FALSE);
	DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFoot->m_exFooting);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	if(bInWall)
	{
		// 벽체 평면
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		if(pWall!=NULL)
		{
			DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, TRUE);
			if(pWall->m_bTopIsColumn)
				DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
		}
	}
	else
	{
		// 날개벽 평면
		CTwinVectorArray tvArr;
		pBri->GetTvWingWallPlane(nJijum==0, FALSE);
		CTwinVectorArray tvArrPlane;
		tvArrPlane = pFoot->m_tvArrplan;
		tvArrPlane.Sort();
		DBStd.GetTvArrTrimByPolygon(pBri->m_tvArrPlaneWingWall, tvArrPlane, TRUE, tvArr);
		DBStd.DrawTvArray(&Dom, tvArr);
	}

	CDPoint xyJijum = pBri->GetXyOnJijum(nJijum);
	CDPoint vAng = pBri->GetAngleAzimuthJijum(nJijum);
	vAng = CDPoint(Round(vAng.x, 5), Round(vAng.y, 5));
	if(nJijum==0) vAng *= -1;
	drawExPile.DimPlanSide(&Dom, 0, 0, FALSE, TRUE);
	drawExPile.DimPlanOrg(&Dom, 0, FALSE, TRUE);
	Dom.Rotate(xyJijum, vAng.GetMirrorVert());

	DBStd.m_pDrawExFooting->MarkFootPlaneCenterOfRoad(&Dom, nJijum);	// 도로 중심 마크

	Dom.Move(-xyJijum.x, -xyJijum.y);

	if(vAng!=CDPoint(0, 1) && !(pFoot->m_bRAngLeft && pFoot->m_bRAngRight))
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rc.right, rc.top-Dom.Always(5), _T("* () 안의 치수는 사거리 치수임."));
	}

	*pDomP << Dom;
}

void CARcBridgeCalcEarthQuake::DrawDiagramUnitLoad(CDomyun *pDomP, long nSizeRow, BOOL bDirBri)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = &pBri->m_FemEQ[0];

	CForceDiagram FDiagram(pFEM);
	FDiagram.SetCWnd(m_pStd->m_pView);

	m_pStd->m_pARoadOptionStd->SetEnvType(&FDiagram,HCAD_DIML);

	double dScaleDim = GetScaleDiagram(nSizeRow);
	FDiagram.SetScaleDim(dScaleDim);
	FDiagram.SetLTScale(dScaleDim*5);				
	FDiagram.DrawFrameModel(TRUE,FALSE,FALSE);

	// 단위하중 표기
	double dLengthModel = toM(pBri->GetLengthModelSpan(-1));
	double dUnitLoad    = 1.000;
	CString str = _T("");
	str.Format("%.3f %s",dUnitLoad,pData->m_szTonf_M);

	if(bDirBri)
	{
		double dLengthText = 4;
		double TextY = pFEM->GetJoint(0)->m_vPoint.z + FDiagram.Always(4);
		long nDirLoad = (pBri->m_nSelectModelingCountEQ==1) ? 0 : 4;
		if(pBri->m_nSelectModelingCountEQ==1)
			FDiagram.GiSiArrowExt(dLengthModel/2-dLengthText/2,TextY,0,dLengthText,nDirLoad,str);
		else
			FDiagram.GiSiArrowExt(dLengthModel/2+dLengthText/2,TextY,dLengthText,dLengthText,nDirLoad,str);
	}
	else
	{
		long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0]);
		long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0]);
		double dLengthText = 4;
		for(long nJoint=nIdxJointStt; nJoint<=nIdxJointEnd; nJoint++)
		{
			CJoint *pJnt = pFEM->GetJoint((USHORT)nJoint);
			CDomyun Dom(&FDiagram);
			Dom.GiSiArrowExt(pJnt->m_vPoint.x,pJnt->m_vPoint.z,dLengthText,dLengthText,4,str);
			Dom.Rotate(ConstPi/4,pJnt->m_vPoint.x,pJnt->m_vPoint.z);
			FDiagram << Dom;
		}
	}

	FDiagram.SetCalcExtRect();
	CDRect rect = FDiagram.GetExtRect();
	m_pStd->m_pARoadOptionStd->SetEnvType(&FDiagram,HCAD_HIDD);
	FDiagram.Point(rect.left,rect.bottom+FDiagram.Always(5));

	*pDomP << FDiagram;
}

void CARcBridgeCalcEarthQuake::WriteCalcLoadUpperWeight(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTiny)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGBOOL bLSD = pData->IsLsdDesign();

	double WeightDist  = GetValueUnitChange(m_pStd->m_pLoad->m_DeadLoadDistribution[0]+m_pStd->m_pLoad->m_DeadLoadDistribution[1],UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double UW_Conc     = GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dWidthSlab = GetValueUnitChange(m_dWidthSlab, UNIT_CGS_M, pData->m_szM);
	double WeightEtc   = GetValueUnitChange(m_DeadLoadDistribution,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double WeightSlab  = GetValueUnitChange(m_dWeightSlab,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double WeightSoilUint  = GetValueUnitChange(m_pStd->m_pLoad->m_dSoilPressureVert,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double WeightSoil = GetValueUnitChange(m_DeadLoadSoil,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double WeightUpper = GetValueUnitChange(m_dWeightUpper,UNIT_CGS_TONF_M,pData->m_szTonf_M);

	double dTS = bLSD==TRUE ? pBri->m_dTS : toM(pBri->m_dTS);

	long nSizeHunch = m_sArrWeightHunch.GetSize();
	XLText->GoPosition(NO_MOVE,COL_POS_2);
//	XLText->AddFormatText("$h%s$r슬래브$m+03$c:$r%.3f x %.3f x %.3f$m+13$c=$r$me2$f3$i%.3f$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),toM(pBri->m_dTS),m_dWidthSlab,UW_Conc,WeightSlab,pData->m_szTonf_M);
	XLText->AddFormatText("$h%s$r슬래브$m+03$c:$r%s x %s x %s$m+13$c=$r$me2$f3$i%s$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),
		pData->GetStrDouble(dTS, UNIT_CGS_M), pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3),
		pData->GetStrDouble(WeightSlab, UNIT_CGS_TONF_M), pData->m_szTonf_M);

	long i=0; for(i=0; i<nSizeHunch; i++)
	{
		double WeightHunch = GetValueUnitChange(m_dArrWeightHunch.GetAt(i),UNIT_CGS_TONF_M,pData->m_szTonf_M);

//		XLText->AddFormatText("$n$h%s$r헌치%2d$m+03$c:$r%s$m+13$c=$r$me2$f3$i%.3f$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),i+1,m_sArrWeightHunch.GetAt(i),WeightHunch,pData->m_szTonf_M);
		XLText->AddFormatText("$n$h%s$r헌치%2d$m+03$c:$r%s$m+13$c=$r$me2$f3$i%s$m+03%s", GetNumberString(nIndexTiny++,NUM_CIRCLE),i+1,m_sArrWeightHunch.GetAt(i),
			pData->GetStrDouble(WeightHunch,UNIT_CGS_TONF_M), pData->m_szTonf_M);
	}
//	XLText->AddFormatText("$n$h%s$r기타하중$m+03$c:$r%.3f x %.3f$m+13$c=$r$me2$f3$i%.3f$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),WeightDist,m_dWidthSlab,WeightEtc,pData->m_szTonf_M);
	XLText->AddFormatText("$n$h%s$r기타하중$m+03$c:$r%s x %s$m+13$c=$r$me2$f3$i%s$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),pData->GetStrDouble(WeightDist,UNIT_CGS_TONF_M2), 
		pData->GetStrDouble(dWidthSlab, UNIT_CGS_M),pData->GetStrDouble(WeightEtc,UNIT_CGS_TONF_M),pData->m_szTonf_M);
	if(WeightSoil > 0) 
	{
		XLText->AddFormatText("$n$h%s$r토  압$m+03$c:$r%s x %s$m+13$c=$r$me2$f3$i%s$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),
			pData->GetStrDouble(WeightSoilUint,UNIT_CGS_TONF_M2),pData->GetStrDouble(dWidthSlab,UNIT_CGS_M),pData->GetStrDouble(WeightSoil,UNIT_CGS_TONF_M, TRUE),pData->m_szTonf_M);
	}
	if(pBri->GetTypeUpperWeightEQ()==1)
	{
		if(m_dArrWeightWall.GetSize()>0)
		{
			long nSizeWall = m_sArrWeightWall.GetSize();
			long i=0; for(i=0; i<nSizeWall; i++)
			{
				double WeightWall = GetValueUnitChange(m_dArrWeightWall.GetAt(i),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				XLText->AddFormatText("$n$h%s$r벽체%2d$m+03$c:$r%s$m+13$c=$r$me2$f3$i%s$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),i+1,m_sArrWeightWall.GetAt(i),
					pData->GetStrDouble(WeightWall,UNIT_CGS_TONF_M, TRUE),pData->m_szTonf_M);

//				XLText->AddFormatText("$n$h%s$r벽체%2d$m+03$c:$r%s$m+13$c=$r$me2$f3$i%.3f$m+03%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),i+1,m_sArrWeightWall.GetAt(i),WeightWall,pData->m_szTonf_M);
			}
		}
	}
	XLText->AddFormatText("$h$-22");
	XLText->AddFormatText("$n$h$r합 계$m+03$c:$r$m+13$c=$r$me2$f3$i%s$m+03%s$n",pData->GetStrDouble(WeightUpper,UNIT_CGS_TONF_M) ,pData->m_szTonf_M);
}

CRigidityFooting* CARcBridgeCalcEarthQuake::GetRigidityFootingByJijumNumber(long nJijum)
{
	long nSizeRigid = m_pListRighty.GetSize();
	long i=0; for(i=0; i<nSizeRigid; i++)
	{
		CRigidityFooting *pRigid = m_pListRighty.GetAt(i);
		if(pRigid->m_nJijum==nJijum) return pRigid;
	}

	return NULL;
}

void CARcBridgeCalcEarthQuake::WriteLoadDiagram(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CFEMManage *pFem = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFem == NULL) return;

	CForceDiagram FDiagram(pFem);

	m_dDimScaleDiagram = (pBri->m_nQtyJigan==1) ? toM(pBri->m_dLengthBridge)*12 : toM(pBri->m_dLengthBridge)*6;

	FDiagram.SetCWnd(m_pStd->m_pView);		
	FDiagram.SetScaleDim(m_dDimScaleDiagram);
	FDiagram.SetLTScale(m_dDimScaleDiagram*5);	

	long nIndexSmall = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$a%d.%d 지 진 시$n$n",nIndexTitle,nIndexMiddle);

	CString strComment = _T("");
	if(pBri->IsBoxType())
	{
		// 고정하중
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "고정하중";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		m_pStd->m_pLoad->DrawDiagramDeadLoad(XLText, TRUE);
		*XLText += 1;

		// 지진시 관성력
		for(long nCase = 0; nCase < eGoalEarthQuakeCount; nCase++)
		{
			if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;
			EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);
			
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment.Format(_T("%s (%s)"), _T("지진시 관성력"), pBri->GetStringGoalEarthQuake(eGoalEq, TRUE));
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramLoadInertialForce(XLText, eGoalEq);
			*XLText += 1;		
		}

		// 지진시 토압
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH])
		{
			for(long nCase = 0; nCase < eGoalEarthQuakeCount; nCase++)
			{
				if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;
				EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);

				for(long nCaseW = 0; nCaseW < 2; nCaseW++)
				{
					if(nCaseW == 1 && pBri->m_bApplyGroundWaterLevel == FALSE) break;
					XLText->GoPosition(NO_MOVE,COL_POS_1);
					strComment.Format(_T("%s (%s)"), _T("양토압"), pBri->GetStringGoalEarthQuake(eGoalEq, TRUE));
					if(pBri->m_bApplyGroundWaterLevel)
					{
						if(nCaseW == 0)
							strComment += _T(" : 지하수가 없는 경우");
						else
							strComment += _T(" : 지하수가 있는 경우");
					}
					XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
					m_pStd->m_pLoad->DrawDiagramHoriPressure(XLText, TRUE, TRUE, nCaseW == 1, TRUE, eGoalEq);
					*XLText += 1;
				}
			}
		}
		// 수압
		if(pBri->m_bApplyGroundWaterLevel && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = "수평 수압";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramHoriPressure(XLText, FALSE, TRUE, TRUE, TRUE, pBri->GetEarthQuakeGoalByIndex(0));
			*XLText += 1;
		}
		if(pBri->m_bApplyGroundWaterLevel && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = "연직 수압";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramWaterPressureVertical(XLText);
			*XLText += 1;
		}

		// 앵커
		if(pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = "앵커력";
			XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramEarthAnchor(XLText, TRUE);

			*XLText += 1;
		}

	}
	else
	{
		// 고정하중
		strComment = "고정하중";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramDeadLoad(XLText);
		*XLText += 1;

		// 종방향 지진하중
		strComment = "종방향 지진하중";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramLoadEQ(XLText);
		*XLText += 1;

		// 활하중
		if(pBri->m_bApplyLiveLoadEQ && pBri->m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL])
		{
			if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				strComment = "활하중";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);
				m_pStd->m_pLoad->DrawDiagramLiveLoadUnderGroundRahmen(XLText, TRUE);
			}
			*XLText += 1;
		}

		if(pBri->IsOutsideWall())
		{
			if(pBri->m_nSelectModelingCountEQ==2)
			{
				// 지진시 토압
				strComment = "지진시 토압 : 시점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,TRUE);
				*XLText += 1;

				// 지진시 토압
				strComment = "지진시 토압 : 종점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,FALSE);
				*XLText += 1;
			}
			else
			{
				BOOL bSttWall = (pBri->m_nSelectModelingCountEQ==0) ? TRUE : FALSE;
				// 지진시 토압
				strComment = bSttWall ? "지진시 토압 : 시점측벽체" : "지진시 토압 : 종점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,bSttWall);
				*XLText += 1;
			}
		}
	}
}


void CARcBridgeCalcEarthQuake::DrawDiagramDeadLoad(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, TRUE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	double dLoadValue = GetValueUnitChange(m_DeadLoadDistribution+m_DeadLoadSoil,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	m_pStd->DrawDiagramSlabDistribution(pDom,pFEM,dLoadValue);

	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	XLText->AddFormatText("$hWd = %.3f %s$n",dLoadValue,pData->m_szTonf_M);
	XLText->AddFormatText("$h구체 자중은 PROGRAM내 자동고려$n");
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcEarthQuake::DrawDiagramLoadEQ(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, TRUE);

	BOOL bLsd = pData->IsLsdDesign();
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	double dLoadValue = GetValueUnitChange(m_Pe[CALC_DIR_X],UNIT_CGS_TONF_M,pData->m_szTonf_M);

	long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0]);
	long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0]);
	CJoint *pJntStt = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd = pFEM->GetJoint((USHORT)nIdxJointEnd);
	double dLengthModel = bLsd? (pJntEnd->m_vPoint.x - pJntStt->m_vPoint.x) : frM(pJntEnd->m_vPoint.x - pJntStt->m_vPoint.x);
	double dCountDraw   = 12;
	double dLengthArrow = dLengthModel/dCountDraw/2;

	CDPoint vX(1,0),vY(0,1);
	CDPoint xyStt = (bLsd? CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z) : CDPoint(frM(pJntStt->m_vPoint.x),frM(pJntStt->m_vPoint.z))) + dLengthArrow*vX;
	CDPoint xyDraw(0,0);
	long i=0; for(i=0; i<(long)dCountDraw; i++)
	{
		if(pBri->m_nSelectModelingCountEQ==1)
		{
			xyDraw = xyStt + (i-1)*(dLengthArrow*2)*vX + dLengthArrow/2*vY;
			pDom->GiSiArrowExt(xyDraw.x,xyDraw.y,dLengthArrow,dLengthArrow,0,_T(""));
		}
		else
		{
			xyDraw = xyStt + i*(dLengthArrow*2)*vX + dLengthArrow/2*vY;
			pDom->GiSiArrowExt(xyDraw.x,xyDraw.y,dLengthArrow,dLengthArrow,4,_T(""));
		}
	}

	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	XLText->AddFormatText("$hPe = %.3f %s$n",dLoadValue,pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}


void CARcBridgeCalcEarthQuake::DrawDiagramLoadInertialForce(CXLFormatText *XLText, EGoalOfEarthQuake eGoalEq)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE);

	BOOL bLsd = pData->IsLsdDesign();
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0], iUD == iUPPER);
		long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0], iUD == iUPPER);
		CJoint *pJntStt = pFEM->GetJoint((USHORT)nIdxJointStt);
		CJoint *pJntEnd = pFEM->GetJoint((USHORT)nIdxJointEnd);

		CDPoint xyStt(bLsd? pJntStt->m_vPoint.x : frM(pJntStt->m_vPoint.x), (bLsd? pJntStt->m_vPoint.z : frM(pJntStt->m_vPoint.z)) + 300);
		CDPoint xyEnd(bLsd? pJntEnd->m_vPoint.x : frM(pJntEnd->m_vPoint.x), (bLsd? pJntEnd->m_vPoint.z : frM(pJntEnd->m_vPoint.z)) + 300);
		double dLengthModel = ~(xyEnd - xyStt);
		double dCountDraw   = 12;
		double dLengthArrow = dLengthModel / dCountDraw / 2;

		CDPoint vDir = (xyEnd - xyStt).Unit();
		CDPoint xySlab = xyStt + dLengthArrow * vDir;

		CDPoint xyDraw(0,0);
		for(long i=0; i<(long)dCountDraw; i++)
		{
			xyDraw = xySlab + i*(dLengthArrow*2)*vDir;
			pDom->GiSiArrowExt(xyDraw.x,xyDraw.y,dLengthArrow,dLengthArrow,4,_T(""));
		}
	}

	for(long iSE = iSTT; iSE <= iEND; iSE++)
	{
		long nIdxJointUp = iSE == iSTT ? m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0], TRUE) : m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0], TRUE);
		long nIdxJointDn = iSE == iSTT ? m_pStd->m_pFrame->GetNumberJointSlabStt(&pBri->m_FemEQ[0], FALSE) : m_pStd->m_pFrame->GetNumberJointSlabEnd(&pBri->m_FemEQ[0], FALSE);

		CJoint *pJntUp = pFEM->GetJoint((USHORT)nIdxJointUp);
		CJoint *pJntDn = pFEM->GetJoint((USHORT)nIdxJointDn);

		CDPoint xyUp(bLsd? pJntUp->m_vPoint.x : frM(pJntUp->m_vPoint.x), bLsd? pJntUp->m_vPoint.z : frM(pJntUp->m_vPoint.z));
		CDPoint xyDn(bLsd? pJntDn->m_vPoint.x : frM(pJntDn->m_vPoint.x), bLsd? pJntDn->m_vPoint.z : frM(pJntDn->m_vPoint.z));
		double dLengthModel = ~(xyUp - xyDn);
		double dCountDraw   = 12;
		double dLengthArrow = dLengthModel / dCountDraw / 2;

		CDPoint vDir(0, 1), vX(1,0);
		CDPoint xyDraw(0,0);
		for(long i=0; i<=(long)dCountDraw; i++)
		{
			xyDraw = xyDn + i*(dLengthArrow*2)*vDir;
			pDom->GiSiArrowExt(xyDraw.x, xyDraw.y, MAX(1000, dLengthArrow), MAX(1000, dLengthArrow), 4 ,_T(""));
		}
		pDom->LineTo(xyUp - MAX(1000, dLengthArrow)*vX, xyDn - MAX(1000, dLengthArrow)*vX); 
	}

	double dApplyWidth = m_pStd->GetSlabWidth(-1);
	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 2;
	XLText->AddFormatText("$h상부슬래브 : %.3f %s$n",GetValueUnitChange(m_InertialForce_Slab[eGoalEq][iUPPER], UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h하부슬래브 : %.3f %s$n",GetValueUnitChange(m_InertialForce_Slab[eGoalEq][iLOWER], UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
	*XLText += 1;
	XLText->AddFormatText("$h시점벽체 : %.3f %s$n",GetValueUnitChange(m_InertialForce_Wall[eGoalEq][iLEFT], UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
	XLText->AddFormatText("$h종점벽체 : %.3f %s$n",GetValueUnitChange(m_InertialForce_Wall[eGoalEq][iRIGHT], UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}


void CARcBridgeCalcEarthQuake::DrawDiagramSoilPressure(CXLFormatText *XLText, BOOL bLeftWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxWall = bLeftWall ? 0 : 1;
	long nJijum   = bLeftWall ? 0 : pBri->m_nQtyJigan;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	double HeightToap = m_HeightToap[nIdxWall];
	double ToapValue  = GetValueUnitChange(m_ToapEQ[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);

	CElement *pEle = m_pStd->m_pFrame->GetElementWallFix(pFEM,nJijum);
	CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJEnd);
	CDPoint xyNode = bLsd? CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z) : frM(CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z));

	CDPoint vY(0,1),vX(1,0);
	CDPoint xyToap = xyNode + frM(HeightToap)*vY;
	if(bLeftWall)
	{
		if(pBri->GetTypeSoilPressureEQ()==0)
		{
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap-2500*vX);
			pDom->TextOut(xyToap-2500*vX+pDom->GetTextHeight()*0.1," Pae ");
		}
		else
		{
			xyToap = xyNode + frM(m_HeightWall[nIdxWall]/3)*vY;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap-2500*vX);
			pDom->TextOut(xyToap-2500*vX+pDom->GetTextHeight()*0.1," Pa ");
			//
			xyToap = xyNode + frM(m_HeightWall[nIdxWall]*0.6)*vY;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap-2500*vX);
			pDom->TextOut(xyToap-2500*vX+pDom->GetTextHeight()*0.1," ΔPae ");
		}
	}
	else
	{
		if(pBri->GetTypeSoilPressureEQ()==0)
		{
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap+2500*vX);
			pDom->SetTextAlignHorz(TA_RIGHT);
			pDom->TextOut(xyToap+2500*vX+pDom->GetTextHeight()*0.1," Pae ");
		}
		else
		{
			xyToap = xyNode + frM(m_HeightWall[nIdxWall]/3)*vY;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap+2500*vX);
			pDom->SetTextAlignHorz(TA_RIGHT);
			pDom->TextOut(xyToap+2500*vX+pDom->GetTextHeight()*0.1," Pa ");
			//
			xyToap = xyNode + frM(m_HeightWall[nIdxWall]*0.6)*vY;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyToap, pDom->GetScaleDim(), 0, TRUE);
			pDom->LineTo(xyToap,xyToap+2500*vX);
			pDom->SetTextAlignHorz(TA_RIGHT);
			pDom->TextOut(xyToap+2500*vX+pDom->GetTextHeight()*0.1," ΔPae ");
		}
	}

	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	if(pBri->GetTypeSoilPressureEQ()==0)
	{
		XLText->AddFormatText("$hPae = %.3f %s$n",ToapValue,pData->m_szTonf_M);
		XLText->AddFormatText("$hHe  = %.3f m$n",HeightToap);
	}
	else
	{
		double ToapValue1 = GetValueUnitChange(m_ToapNormal[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		double ToapValue2 = GetValueUnitChange(m_ToapEQ[nIdxWall]-m_ToapNormal[nIdxWall],UNIT_CGS_TONF,pData->m_szTonf);
		XLText->AddFormatText("$h  Pa  = %.3f %s (작용높이 = %.3fm)$n",ToapValue1,pData->m_szTonf_M, m_HeightWall[nIdxWall]/3.0);
		XLText->AddFormatText("$hΔPae = %.3f %s  (작용높이 = %.3fm)$n",ToapValue2,pData->m_szTonf_M, m_HeightWall[nIdxWall]*0.6);
	}
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcEarthQuake::DrawDiagramModeling(CDomyun *pDomP, BOOL bUpperOffset)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CDomyun Dom(pDomP);

	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	double dOffsetBasic = 500;
	double dOffsetSide = 5000;
	double dOffsetUpper = bUpperOffset ? 3000 : dOffsetBasic;

	CDRect rect =m_pStd->DrawDiagramModeling(&Dom, pFEM, FALSE);
	Dom.Point(rect.left - dOffsetSide ,rect.bottom + dOffsetUpper);
	Dom.Point(rect.right + dOffsetSide, rect.top - dOffsetBasic);

	*pDomP << Dom;
}

void CARcBridgeCalcEarthQuake::WriteLoadDiagram_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	CFEMManage *pFem = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFem == NULL) return;

	CForceDiagram FDiagram(pFem);
	
	m_dDimScaleDiagram = (pBri->m_nQtyJigan==1) ? toM(pBri->m_dLengthBridge)*12 : toM(pBri->m_dLengthBridge)*6;

	FDiagram.SetCWnd(m_pStd->m_pView);		
	FDiagram.SetScaleDim(m_dDimScaleDiagram);
	FDiagram.SetLTScale(m_dDimScaleDiagram*5);	

	long nIndexSmall = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$a%d.%d 지 진 시$n$n",nIndexTitle,nIndexMiddle);

	CString strComment = _T("");

	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix == 1)? TRUE : FALSE;

		if(pBri->m_plsdManager->IsSelectedLoadDesign(bDW? eLSD_LOADSELECT_DW : eLSD_LOADSELECT_DC))
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bDW? eLSD_LOADCASE_DW1 : eLSD_LOADCASE_DC1, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bDW? eLSD_LOADCASE_DW1 : eLSD_LOADCASE_DC1, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramDeadLoad(XLText, TRUE, bDW);
			*XLText += 1;
		}
	}

	// 연직토압
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV) && m_pStd->m_pLoad->m_dSoilPressureVert>0 || (pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting))
	{
		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !pBri->m_bApplyGroundWaterLevel) continue;

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramSoilPressureVertical(XLText, bWater, TRUE);
			*XLText += 1;
		}
	}

	// 지진시 활하중
	// 활하중 (지중라멘교 : 상재하중)
	// 지중라멘일때 상재 활하중만 태워줬었는데 이동하중도 태워 주도록 변경
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LL1))
	{
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, FALSE) + " (" + pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, TRUE) + ")"; 
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

			m_pStd->m_pLoad->DrawDiagramLiveLoadUnderGroundRahmen(XLText, TRUE);
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, FALSE) + " (" + pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

			m_pStd->m_pLoad->DrawDiagramLiveLoad_Lsd(XLText, FALSE, TRUE);
		}
		*XLText += 1;
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LS))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bLeft = (ix == 0);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramLosdLS_ES_Lsd(XLText, bLeft, TRUE);

			*XLText += 1;
		}
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME,eLSD_LOADCASE_ES))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bLeft = (ix == 0);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramLosdLS_ES_Lsd(XLText, bLeft, TRUE, FALSE);

			*XLText += 1;
		}
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_PL))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, TRUE) + ")";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

		m_pStd->m_pLoad->DrawDiagramLiveLoadFootWay(XLText, FALSE, nIndexSmall, TRUE);

		*XLText += 1;
	}

	if(pBri->IsBoxType())
	{
		// 지진시 관성력
		for(long nCase = 0; nCase < eGoalEarthQuakeCount; nCase++)
		{
			if(pBri->m_bGoalEarthQuake[nCase] == FALSE) continue;
			EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(nCase);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment.Format(_T("%s (%s)"), _T("지진시 관성력"), pBri->GetStringGoalEarthQuake(eGoalEq, TRUE));
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramLoadInertialForce(XLText, eGoalEq);
			*XLText += 1;		
		}
	}
	else
	{
		// 종방향 지진하중
		strComment = "종방향 지진하중";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramLoadEQ(XLText);
		*XLText += 1;
	}

	// 지진시 토압
	if(pBri->IsBoxType())
	{
		for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
		{
			if(pBri->m_bGoalEarthQuake[ix] == FALSE) continue;
			EGoalOfEarthQuake eGoalEq = static_cast<EGoalOfEarthQuake>(ix);
			strComment.Format(_T("지진시 토압 : 좌측 (%s)"), pBri->GetStringGoalEarthQuake(eGoalEq));
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramHoriPressure(XLText, TRUE, FALSE, FALSE, TRUE, eGoalEq);
			*XLText += 1;

			strComment.Format(_T("지진시 토압 : 우측 (%s)"), pBri->GetStringGoalEarthQuake(eGoalEq));
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			m_pStd->m_pLoad->DrawDiagramHoriPressure(XLText, TRUE, TRUE, FALSE, TRUE, eGoalEq);
			*XLText += 1;
		}
	}
	else
	{
		if(pBri->IsOutsideWall())
		{
			if(pBri->m_nSelectModelingCountEQ==2)
			{
				// 지진시 토압
				strComment = "지진시 토압 : 시점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,TRUE);
				*XLText += 1;

				// 지진시 토압
				strComment = "지진시 토압 : 종점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,FALSE);
				*XLText += 1;
			}
			else
			{
				BOOL bSttWall = (pBri->m_nSelectModelingCountEQ==0) ? TRUE : FALSE;
				// 지진시 토압
				strComment = bSttWall ? "지진시 토압 : 시점측벽체" : "지진시 토압 : 종점측벽체";
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramSoilPressure(XLText,bSttWall);
				*XLText += 1;
			}
		}
	}

	for(long nType = 1; nType <= eGoalEarthQuakeCount; ++nType)
	{
		HGINT32 icLoadSet = pBri->GetCountUserLoadSetLsd(nType);
		if(pBri->IsBoxType())
		{
			if(pBri->m_bGoalEarthQuake[nType-1] == FALSE) continue;
		}
		else
		{
			if(nType > 1) break;
		}

		if(icLoadSet > 0)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			
			if(nType >= 1)
			{
				strComment.Format(_T("사용자 하중(%s)"), pBri->GetStringGoalEarthQuake(static_cast<EGoalOfEarthQuake>(nType-1)));
			}
			else
			{
				strComment = "사용자 하중";
			}
				
			XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

			for(HGINT32 ixLoadSet = 0; ixLoadSet < icLoadSet; ++ixLoadSet)
			{
				m_pStd->m_pLoad->DrawDiagramUserLoad(XLText, nType, ixLoadSet);
			}
		}
	}
	
}

void CARcBridgeCalcEarthQuake::WriteResponseFactor_KDS17( CXLFormatText *pXLText, CRcBridgeRebar *pBri, BOOL bBriDir )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsApplyEarthQuakeDesign() == FALSE) return;

	double dTime = m_T[bBriDir ? CALC_DIR_X : CALC_DIR_Y];

	pXLText->GoPosition(NO_MOVE, 1);
	HGINT32 nTypeGround = pBri->GetTypeCoefficientGround();		// 지반종류
	if(nTypeGround > 0)
	{
		// Fa Fv 테이블
		WriteTableGroundAmplificationFactor(pXLText);
		pXLText->GoPosition(NO_MOVE, 1);
		*pXLText += 1;
	}

	CString sGround = pData->GetStringTypeCoefficientGround(nTypeGround);
	CString str = _T(""), str2 = _T("");
	str.Format(_T("%s지반의 가속도표준설계응답스펙트럼"), sGround);
	pXLText->AddFormatText(_T("$h$r%s$n$h"), str);

	pXLText->AddText(_T("$h$r가속도표준설계스펙트럼 전이주기$n$h"));

	double dS  = pBri->GetCoefficientAccelerateEQ();
	double g  = pBri->GetGravity();;

	CString s1, s2, s3, s4;
	double dTo(0), dTs(0), dTl(0);
	if(nTypeGround == 0)
	{
		dTo = 0.06;
		dTs = 0.30;
		dTl = 3.0;

		pXLText->AddFormatText(_T("$h$rT`0 = %.2f, T`S = %.2f, T`L = %.2f$n$h"), dTo, dTs, dTl);

		if(dTime < dTo)
		{
			s2 = _T(" Sa = (1 + 30T) x S");
			s3.Format(_T("    = (1 + 30 x %.3f) x %.3f"), dTime, dS);
		}
		else if(dTime < dTs)
		{
			s2 = _T(" Sa = 2.8 x S");
			s3.Format(_T("    = 2.8 x %.3f"), dS);
		}
		else if(dTime < dTl)
		{
			s2 = _T(" Sa = 0.84/T x S");
			s3.Format(_T("    = 0.84 / %.3f x %.3f"), dTime, dS);
		}
		else
		{
			s2 = _T(" Sa = 2.52/T^2 x S");
			s3.Format(_T("    = 2.52 / %.3f^2 x %.3f"), dTime, dS);
		}
	}
	else
	{
		double dFa = GetGroundAmplificationFactor(nTypeGround, dS, FALSE);
		double dFv = GetGroundAmplificationFactor(nTypeGround, dS, TRUE);
		dTl = 3.0;
		dTs = dFv / (2.5 * dFa);
		dTo = 0.2 * dTs;

		pXLText->AddFormatText(_T("$h$rT`0 = 0.2T`S = 0.2 x %.3f = %.3f, T`S = Fv/(2.5Fa) = %.3f / (2.5 x %.3f) = %.3f , T`L = %.2f$n$h"), dTs, dTo, dFv, dFa, dTs, dTl);
		pXLText->AddFormatText(_T("$h$r여기서 Fa = %.3f, Fv = %.3f$n$h"), dFa, dFv);

		if(dTime < dTo)
		{
			s2 = _T(" Sa = Fa x S (1 + 1.5T/T`0)");
			s3.Format(_T("    = %.3f x %.3f x (1 + 1.5 x %.3f / %.3f)"), dFa, dS, dTime, dTo);
		}
		else if(dTime < dTs)
		{
			s2 = _T(" Sa = 2.5 x Fa x S");
			s3.Format(_T("    = 2.5 x %.3f x %.3f"), dFa, dS);
		}
		else if(dTime < dTl)
		{
			s2 = _T(" Sa = Fv x S / T");
			s3.Format(_T("    = %.3f x %.3f / %.3f"), dFv, dS, dTime);
		}
		else
		{
			s2 = _T(" Sa = Fv x S x T`L / T^2");
			s3.Format(_T("    = %.3f x %.3f x %.3f / %.3f^2"), dFv, dS, dTl, dTime);
		}
	}

	if(dTime < dTo)
		s1 = _T("O ≤ T < T`0 이므로");
	else if(dTime < dTs)
		s1 = _T("T`0 ≤ T < T`S 이므로");
	else if(dTime < dTl)
		s1 = _T("T`S ≤ T < T`L 이므로");
	else
		s1 = _T("T`L ≤ T 이므로");

	s4.Format(_T("    = %.3f"), GetSa(pBri, dTime)/g);

	// CD
	if(pData->m_nTypeEarthQuakeApply ==1 && Compare(pBri->GetRateDAMP_EQ_CD(), 5.0, _T("="))==FALSE)
	{
		double dValue1 = pow(6.42 / (1.42 + pBri->GetRateDAMP_EQ_CD()), 0.48);

 		pXLText->AddFormatText(_T("$n$h$r감쇠보정계수(CD), 감쇠비(ξ) = %.0f%%$n$h"), pBri->GetRateDAMP_EQ_CD());
		if(0 <= dTime && dTime <= dTo)
		{
			double dValue2 = 1.0 + (dValue1 - 1.0) * dTime/dTo;
			pXLText->AddText(_T("$h$r$c-$rO < T ≤ T`0$n$h"));
			pXLText->AddFormatText(_T("$h$r$rCD = 1.0 + [{6.42 / (1.42 + ξ)}^(0.48) - 1.00] x T / T`0$n"));
			pXLText->AddFormatText(_T("$h$r$r   = 1.0 + (%.3f - 1.00) x %.3f / %.3f$n"), dValue1, dTime, dTo);
			pXLText->AddFormatText(_T("$h$r$r   = %.3f$n"), dValue2);

			dValue1 = dValue2;
		}
		else
		{
			pXLText->AddText(_T("$h$r$c-$rT`0 ≤ T$n$h"));
			pXLText->AddFormatText(_T("$h$r$rCD = [6.42 / (1.42 + ξ)]^(0.48) = %.3f$n$n"), dValue1);
		}

		CString strValue;
		strValue.Format(_T(" x %.3f"), dValue1);
		s2 += _T(" x CD");
		s3 += strValue;
	}

	pXLText->AddFormatText(_T("$h$r%s$n$h"), s1);
	pXLText->AddFormatText(_T("$h$r%s$n$h"), s2);
	pXLText->AddFormatText(_T("$h$r%s$n$h"), s3);
	pXLText->AddFormatText(_T("$h$r%s$n$n$h"), s4);
}

void CARcBridgeCalcEarthQuake::WriteTableGroundAmplificationFactor( CXLFormatText *pXLText )
{
	pXLText->GoPosition(NO_MOVE, 2);
	pXLText->AddText(_T("$h지반종류별 지반증폭계수(Fa, Fv)$n$h"));

	pXLText->AddTable(5, 20, 2, 3);
	pXLText->AddFormatText(_T("$h$md1$me2$c지반종류$m+03$me8$c단주기지반증폭계수, Fa$m+09$me8$c장주기지반증폭계수, Fv$m+09$n"));
	pXLText->AddFormatText(_T("$h$me2$c지반종류$m+03$me2$cS ≤ 0.1$m+03$me2$cS = 0.2$m+03$me2$cS = 0.3$m+03$me2$cS ≤ 0.1$m+03$me2$cS = 0.2$m+03$me2$cS = 0.3$m+03$n$h"));

	double dArrS[3] = { 0.1, 0.2, 0.3 };
	for(long ix = 1; ix < 5; ++ix)
	{
		pXLText->AddFormatText(_T("$me2$cS%d$m+03"), ix+1);

		for(long jx=0; jx<2; ++jx)
		{
			for(long nx=0; nx<3; ++nx)
			{
				double dF = GetGroundAmplificationFactor(ix, dArrS[nx], jx==1);
				pXLText->AddFormatText(_T("$me2$c$f1%.1f$m+03"), dF);
			}
		}

		pXLText->AddText(_T("$n$h"));
	}

	pXLText->AddText(_T("$h※ Fa와 Fv는 유효수평지반가속도(S)에 따라 적용하며 유효수평지반가속도(S)의 값이 중간 값에 해당$n$h"));
	pXLText->AddText(_T("$h   할 경우 직선보간하여 적용한다.$n$h"));
}

double CARcBridgeCalcEarthQuake::GetGroundAmplificationFactor( long nTypeGround, double dS, BOOL bLong/*=FALSE*/ )
{
	double dF(0);

	if(nTypeGround < 1 || nTypeGround > 5)
		return dF;

	double dFArr[4][6] = { { 1.4, 1.4, 1.3, 1.5, 1.4, 1.3 },
	{ 1.7, 1.5, 1.3, 1.7, 1.6, 1.5 },
	{ 1.6, 1.4, 1.2, 2.2, 2.0, 1.8 },
	{ 1.8, 1.3, 1.3, 3.0, 2.7, 2.4 }};

	long idx1 = nTypeGround - 1;
	long idx2(0);
	double dValueRate(0);

	if(dS <= 0.1)
	{
		idx2 = bLong ? 3 : 0;
		dValueRate = 0;
	}else if(dS <= 0.2)
	{
		double dDeltaS = (0.2 - dS) / 0.1;
		idx2 = bLong ? 4 : 1;
		dValueRate = (dFArr[idx1][idx2-1] - dFArr[idx1][idx2]) * dDeltaS;
	}
	else if(dS <= 0.3)
	{
		double dDeltaS = (0.3 - dS) / 0.1;
		idx2 = bLong ? 5 : 2;
		dValueRate = (dFArr[idx1][idx2-1] - dFArr[idx1][idx2]) * dDeltaS;
	}
	else
	{
		idx2 = bLong ? 5 : 2;
		dValueRate = 0;
	}

	return dFArr[idx1][idx2] + dValueRate;
}

double CARcBridgeCalcEarthQuake::GetSa( CRcBridgeRebar *pBri, double dTime )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double Sa = 0;
	double g   = pBri->GetGravity(); 
	double A   = pBri->GetCoefficientAccelerateEQ(); 
	double S   = pBri->GetCoefficientGround();

	HGINT32 nTypeGround = pBri->GetTypeCoefficientGround();		// 지반종류

	double dTs(0), dTo(0), dTl(0);

	if(pData->m_nTypeEarthQuakeApply == 0)
	{
		double Cs = 0;
		double dMax = 2.5*A;
		if(dTime<=4.0)
		{
			Cs = min(1.2*A*S/pow(dTime,2./3.),dMax);
			Sa = Cs*g;
		}
		else
		{
			Cs = min(3.0*A*S/pow(dTime,4./3.),dMax);
			Sa = Cs*g;
		}
	}
	else if(nTypeGround == 0)
	{
		dTo = 0.06;
		dTs = 0.3;
		dTl = 3.0;

		// Sag 값을 구한다.
		double dS = A;

		if(dTime < dTo)
		{
			Sa = (1.0 + 30 * dTime) * dS;
		}
		else if(dTime < dTs)
		{
			Sa = 2.8 * dS;
		}
		else if(dTime < dTl)
		{
			Sa = 0.84 / dTime * dS;
		}
		else
		{
			Sa = 2.52 / pow(dTime, 2) * dS;
		}

		Sa *= g;
	}
	else
	{
		// Sag 값을 구한다.
		double dS = A;
		double dFa = GetGroundAmplificationFactor(nTypeGround, dS, FALSE);
		double dFv = GetGroundAmplificationFactor(nTypeGround, dS, TRUE);
		dTl = 3.0;
		dTs = dFv / (2.5 * dFa);
		dTo = 0.2 * dTs;

		if(dTime < dTo)
		{
			Sa = dFa * dS * (1 + 1.5 * dTime / dTo);
		}
		else if(dTime < dTs)
		{
			Sa = 2.5 * dFa * dS;
		}
		else if(dTime < dTl)
		{
			Sa = dFv * dS / dTime;
		}
		else
		{
			Sa = dFv * dS * dTl / pow(dTime, 2);
		}

		Sa *= g;
	}

	// 주기영역별 감쇠보정계수(CD)
	double dCD = 1.0;
	if(pData->m_nTypeEarthQuakeApply ==1 && Compare(pBri->GetRateDAMP_EQ_CD(), 5.0, _T("="))==FALSE)
	{
		if(Compare(dTime, 0.0, _T("=")))
		{
			dCD = 1.0;
		}
		else if(dTime >= dTo)
		{
			dCD = pow(6.42/(1.42+pBri->GetRateDAMP_EQ_CD()), 0.48);
		}
		else
		{
			dCD = 1.0 + (pow(6.42/(1.42+pBri->GetRateDAMP_EQ_CD()), 0.48) - 1.0) * dTime / dTo;
		}
	}
	Sa *= dCD;

	return Sa;
}
