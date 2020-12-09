// ARcBridgeCalcLoad.cpp: implementation of the CARcBridgeCalcLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

#define DIAGRAM_ROW_SIZE	13
#define RAIL_DOSANG_ADDRATE	1.30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcLoad::CARcBridgeCalcLoad()
{
	InitializeVariables();
	m_dDimScaleDiagram = 0;

	m_bReCalcSpanLength	= FALSE;
	m_dReCalcRateSpan	= 1;
	m_dUnderGroundDesignHeight = 0;

	m_bExistLane = TRUE;

	m_dSpanLengthImpactMin = 1000;
	m_dSpanLengthImpactMax = 0;
	m_dSpanLengthImpactAvg = 0;
	m_dSpanLengthImpactApply = 0;;
}

CARcBridgeCalcLoad::~CARcBridgeCalcLoad()
{

}

void CARcBridgeCalcLoad::InitializeVariables()
{
	m_bGWhalf				= FALSE;
	m_UW_Ascon				= 0;
	m_dWeightAscon			= 0;
	m_dWeightMortar			= 0;
	m_dWeightRailLane		= 0;
	m_dWeightRailDosang		= 0;
	m_dSoilPressureVert		= 0;
	for(long ix=0; ix<2; ++ix)
	{
		m_dPfApply[ix]				= 0;
		m_dPmApplyTruck[ix]			= 0;
		m_dPrApply[ix]				= 0;
		m_dPmApply[ix]				= 0;
		m_dPsApply[ix]				= 0;
	}
	m_dLiveLoadFootway		= 0;

	for(long ix=0; ix<2; ++ix)
	{
		m_dWeightBangHo[ix]			= 0;
		m_dWeightFootway[ix]		= 0;
		m_dWeightCentro[ix]			= 0;
		m_dWeightBoCha[ix]			= 0;
		m_dWeightNangan[ix]			= 0;
		m_dWeightSound[ix]			= 0;
		m_dUpperPBangHo[ix]			= 0;			// ��ȣ�� ��� P
		m_dUpperPCentro[ix]			= 0;
		m_dUpperPBoCha[ix]			= 0;
		m_dUpperPRetainingWall[ix]	= 0;
		m_DeadLoadDistribution[ix]	= 0;
	}
	
	m_dDistributionApply	= 0;
	m_dLaneLoad				= 0;
	m_dSpanLengthDesign		= 0;
	m_dWidthBetweenCurb		= 0;
	m_dEffectiveWidth		= 0;
	m_dEffectiveWidthLane2	= 0;
	m_dDesignRoadWidth		= 3.0;		// ����������
	m_dDesignTruckWidth		= 1.800;	// ǥ��Ʈ���� -> 1.800m
	m_dCoefficientImpact[0]	= 0;
	m_dCoefficientImpact[1]	= 0;
	m_nCaseDesignLane[0]	= 0;
	m_nCaseDesignLane[1]	= 0;
	m_dLoadTempertureAxial	= 0;
	m_dLoadTempertureUpDown	= 0;
	m_dLoadDryAndShrinkage	= 0;
	m_dLoadSubsidence		= 0;

	m_bRailDesign = FALSE;
	m_bSingleLane = TRUE;
	m_dRail_WidthDistribute	= 0;
	m_dRail_WidthDistributeJong = 0;
	m_dHeightRail		= 0;
	m_dRail_WeightLane		= 0;
	m_dHeightDosangUnderTie= 0;
	m_dRail_HeightTie		= 0;
	m_dRail_LengthTie		= 0;
	m_dRail_AngleDosangDistribute= 0;
	m_dRail_LengthLv		= 0;
	m_dRail_UnitForceLR		= 0;
	m_dRail_LoadLR			= 0;
	m_dRail_MomentLR		= 0;
	m_dRail_HeightLR		= 0;
	m_dRail_LoadSB			= 0;
	m_dRail_MomentSB		= 0;
	m_dRail_HeightSB		= 0;
	m_dRail_UnitForceSidong	= 0;
	m_dRail_TsbSidong		= 0;
	m_dRail_LoadSidong		= 0;
	m_dRail_MomentSidong	= 0;
	m_dRail_HeightSidong	= 0;
	m_dRail_LoadZedong		= 0;
	m_dRail_UnitForceZedong	= 0;
	m_dRail_TsbZedong		= 0;
	m_nSttWheelZedong		= 0;
	m_dRail_MomentZedong	= 0;
	m_dRail_HeightZedong	= 0;
	m_dRail_DosangWidthL	= 0;
	m_dRail_DosangWidthU	= 0;

	m_dLiveLoaUR_W1	  = 0;
	m_dLiveLoaUR_W	  = 0;
	m_dLiveLoaUR_D	  = 0;
	m_dLiveLoaUR_i	  = 0;
	m_dLiveLoaUR_P	  = 0;
	m_dLiveLoaUR_Pvl  = 0;
	m_dLiveLoaUR_Pvl1 = 0;
	m_dLiveLoaUR_Pvl2 = 0;
	m_dLiveLoaUR_Phl  = 0;
	m_dLiveLoaUR_Ko	  = 0;
	m_dLiveLoaUR_Bo	  = 0;
	m_dLiveLoaUR_B	  = 0;

	// ��ũ �� ������ ����
	m_dTank_E			= 0;
	m_dTank_i			= 0;
	m_dTank_Ptank		= 0;
	m_dTank_Ptrailer1	= 0;
	m_dTank_Ptrailer2	= 0;
	m_dTank_Ptrailer3	= 0;
	m_dTank_WeightTank	= 0;
	m_dTank_WeightTrailer	= 0;
	m_dTank_TankLength	= 0;
	m_dTank_TankWidth	= 0;
	m_dTank_TrailerL1	= 0;
	m_dTank_TrailerL2	= 0;
	m_dTank_TrailerL3	= 0;
	m_dTank_TrailerL4	= 0;
	m_dTank_TrailerWidth= 0;

	m_dLoadLsApply[0]	=	0;
	m_dLoadLsApply[1]	=	0;

	for (HGINT32 ix=0; ix<2; ++ix)
	{
		m_dLoadTgValueMg[0] = 0;
		m_dLoadTgValueT[0] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	m_dLoadBuoyancy = 0;
	m_dLoadWaterVert = 0;
}

void CARcBridgeCalcLoad::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	InitializeVariables();
	m_bRailDesign = pBri->m_pARcBridgeDataStd->IsRailDesign();
	m_bSingleLane = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? TRUE : FALSE;
	m_dUnderGroundDesignHeight = pBri->m_bUnderGroundRahmen ? pBri->m_dUnderGroundDesignHeight : 0;

	pBri->SetDataInitPaveDetailWidth();
	// ���� ���߿� ���� ���
	CalculateDeadLoad();
	CalculateLiveLoad();
	CalculateRailLoadLive();
	CalculateLiveLoadUnderGround();
	CalculateLiveLoadTank();
	CalculateRailLoadLR();
	CalculateRailLoadSB();
	CalculateSoilPressure();
	CalculateWaterLoad();
	CalculateTemperature();
	CalculateDryAndShrinkage();
	CalculateSubsidence();
	CalculateLoadLs_Lsd();
	CalculateLoadTG();
}

// ��������
void CARcBridgeCalcLoad::CalculateDeadLoad()
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

	if(m_bRailDesign)
	{
		// ��ȿ������ : ���α� ������� 4.7.5.1,(2)
		m_dRail_AngleDosangDistribute = 53.0;
		m_dRail_AngleEffective		  = 45.0;
		m_dHeightRail			  = pBri->m_dHeightRail;
		m_dRail_WeightLane			  = pBri->m_dUnitWeightRail * pBri->m_nQtyRail;
		m_dRail_HeightTie			  = pBri->m_dRail_HeightTie;
		m_dHeightDosangUnderTie  = pBri->m_dHeightDosangUnderTie;
		m_dRail_LengthTie			  = pBri->m_dRail_LengthTie;
		m_dRail_DistBetweenRail		  = pBri->m_dRail_DistBetweenRail;

		if(pData->IsRailDesign2017() && pBri->m_bCalcOpen == FALSE && pBri->m_nUnderGroundDistribute == 1) //2017 �������� ���� �� ����ü�
			m_dRail_AngleDosangDistribute = 45.0;

/*		double H1 = toM(m_dHeightDosangUnderTie);
		double A1 = ToRadian(m_dRail_AngleDosangDistribute);
		double H2 = toM(m_dUnderGroundDesignHeight+pBri->m_dUnderGroundHeightMortar+pBri->m_dTS/2);
		double A2 = ToRadian(m_dRail_AngleEffective);

		if(pData->m_nTypeCalcSlabEffWidth==0)
			m_dRail_WidthDistribute = toM(m_dRail_LengthTie) + 2*(H1/tan(A1)+H2/tan(A2));
		else
		{
			m_dRail_AngleDosangDistribute = 45.0;
			A1 = ToRadian(m_dRail_AngleDosangDistribute);
			m_dRail_WidthDistribute = toM(m_dRail_LengthTie) + 2*(H1/tan(A1));
		}
*/
		// (34934) ���߶���̰� ���ǰ� �ִٸ� ���� �����ش�.
		double dHe = 0;
		if(pBri->m_bUnderGroundRahmen)
		{
			dHe = toM(pBri->m_dUnderGroundDesignHeight);
		}
		// 10497
		double dPave = toM(pBri->m_dUnderGroundHeightMortar);
		double dH = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - m_dRail_HeightTie) - dPave;
		if(pBri->m_dDisBaseElevationRL == 0)
			dH = toM(pBri->GetHeightDosangInput());

//		double dH = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - m_dRail_HeightTie);
//		if(pBri->m_dDisBaseElevationRL == 0) dH = toM(pBri->GetHeightDosangInput() - pBri->m_dHeightRail);

		// (ARCBRIDGE-3599) ������ 53��, ����, ������� 45 ��   ���ǰ� x 1.5�� 53���� ����ǰ� �־ ����.
		m_dRail_WidthDistribute = toM(m_dRail_LengthTie) + 1.5 * (dH + dPave) + 2 * dHe;
		m_dRail_WidthDistributeJong = MIN(toM(pBri->m_dRail_DistBetweenWheel),toM(pBri->m_dRail_WidthTie) + 1.5 * (dH + dPave) + 2 * dHe);
		if(pData->m_nTypeCalcSlabEffWidth == 0)
		{
			m_dRail_WidthDistribute += toM(pBri->m_dTS);
			m_dRail_WidthDistributeJong += toM(pBri->m_dTS);
		}
	}

	// ����,����,��ȣ��,�ߺд�,�������� ������ ����.....
	// ��ü�� SAP���� �ڵ����� ����
	m_UW_Ascon = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt),3);
	if(pBri->m_bUnderGroundRahmen)
	{
		if(pBri->m_nTypePavement==1)
			m_UW_Ascon = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete),3);
	}
	else
	{
		if(pBri->m_nRoadMaterial==1)
			m_UW_Ascon = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement),3);
	}

	double UW_Ascon  = m_UW_Ascon;
	double UW_Mortar = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement),3);
	double UW_Conc   = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete),3);
	double UW_ConcOnly   = Round(toTon(pData->m_pBasicConcInfo->m_UWeightConcrete),3);
	double UW_Dosang = Round(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightGravel),3);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge    = pBri->GetStationOnJijum(nSpan,nLine);
	double dWidthBetweenCurb = toM(pBri->GetWidthTotalRoadWay(dStationBridge));
//	double dWidthRoad		 = toM(pBri->m_dWidthRoadRight - pBri->m_dWidthRoadLeft);
	double dWidthSlab        = toM(pBri->GetWidthSlabAct(dStationBridge,vAng));
	double dWidthMortar = dWidthSlab - toM(pBri->m_dCantilB[iLEFT] + pBri->m_dCantilB[iRIGHT]);
	double dThickAsocn		 = toM(pBri->m_dTP);
	double dThickMortar		 = 0;
	if(pBri->m_bUnderGroundRahmen)
	{
		dThickAsocn	 = pBri->m_bUnderGroundPaveDetail? 0 : toM(pBri->m_dUnderGroundHeightPavement);
		dThickMortar = toM(pBri->m_dUnderGroundHeightMortar);
	}
	if(pData->IsRailDesign())
	{
		dThickAsocn	 = 0;
		dThickMortar = toM(pBri->m_dUnderGroundHeightMortar);
	}

	m_dWeightAscon   = dThickAsocn*UW_Ascon*dWidthBetweenCurb;
	m_dWeightMortar  = dThickMortar*UW_Mortar*dWidthMortar;
	m_dWeightRailLane	= 0;
	m_dWeightRailDosang	= 0;

	if(pData->IsRailDesign())
	{
		if(m_bSingleLane)
			m_dWeightRailLane	= m_dRail_WeightLane/m_dRail_WidthDistribute;
		else
			m_dWeightRailLane	= m_dRail_WeightLane*2/(m_dRail_WidthDistribute+toM(m_dRail_DistBetweenRail));

		m_dWeightRailDosang	= UW_Dosang*toM(m_dRail_HeightTie+m_dHeightDosangUnderTie)*RAIL_DOSANG_ADDRATE;

		CDoubleArray dArrWidthL, dArrWidthU;
		m_dRail_DosangWidthL = 0;
		m_dRail_DosangWidthU = 0;
		CDPointArray	xyArr;
		long nQtyHDan = pBri->GetQtyHDan();
		for(long nHDan = 0; nHDan < nQtyHDan; nHDan++)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);

			if(!pGW)	continue;
			if(pGW->IsTypeHDanRailTotalDosang())	//����ĵ���, �ڰ����� �̸�
			{
				// ������ ������ �ϳ��� ���ؼ� ���̸� ������Ų��.
				CDPoint A[6]; 
				xyArr.RemoveAll();
				pBri->GetXyDosang(dStationBridge, CDPoint(0, 1), nHDan, A);	// ������ ����Ʈ
				long n = 1;
				for(n = 1; n <= 4; n++)
					xyArr.Add(A[n]);
				
				CTwinVectorArray	tvArr;
				
				for(n = 0; n < xyArr.GetSize(); n++)
				{
					CTwinVector tv;
					if(n == xyArr.GetSize() - 1)
					{
						tv.m_v1 = xyArr.GetAt(n);
						tv.m_v2 = xyArr.GetAt(0);
					}
					else
					{
						tv.m_v1 = xyArr.GetAt(n);
						tv.m_v2 = xyArr.GetAt(n + 1);
					}
					tvArr.Add(tv);
				}
				
				if(xyArr.GetSize() > 3)
				{
					m_dRail_DosangWidthL += toM(fabs(tvArr.GetRect().Width()));
					m_dRail_DosangWidthU += toM(~(xyArr.GetAt(1) - xyArr.GetAt(2)));
				}
			}
		}

		double dHeight = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail);
		if(pBri->m_dDisBaseElevationRL == 0) 
			dHeight = toM(pBri->GetHeightDosangInput());

		if(dHeight > 0 && m_dRail_DosangWidthL > 0 && m_dRail_DosangWidthU > 0)
			m_dWeightRailDosang = UW_Dosang * RAIL_DOSANG_ADDRATE * (m_dRail_DosangWidthL + m_dRail_DosangWidthU) / 2 * dHeight;
	}

	// 2006.10.18 ������ǿ� ������� ����Է��� �� �߰� �Ѵ�.
	// double dHgt = toM(m_dUnderGroundDesignHeight);
	if(pBri->m_bUnderGroundRahmen && (bLsd || pBri->m_bApplyLoad[BRIDGE_LOAD_SOIL]))
	{
		m_dSoilPressureVert = 0;
		if(pBri->m_bUnderGroundPaveDetail)
		{
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightPyo) * toTon(pBri->m_dUnderGroundPaveWeightPyo) * pBri->m_dUnderGroundPaveWidthPyo;
			
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightGee) * toTon(pBri->m_dUnderGroundPaveWeightGee) * pBri->m_dUnderGroundPaveWidthGee;
			
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightMiddle) * toTon(pBri->m_dUnderGroundPaveWeightMiddle) * pBri->m_dUnderGroundPaveWidthMiddle;
			
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightBozo) * toTon(pBri->m_dUnderGroundPaveWeightBozo) * pBri->m_dUnderGroundPaveWidthBozo;
			
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightDong) * toTon(pBri->m_dUnderGroundPaveWeightDong) * pBri->m_dUnderGroundPaveWidthDong;
			
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightEps) * toTon(pBri->m_dUnderGroundPaveWeightEps) * pBri->m_dUnderGroundPaveWidthEps;
			
			m_dSoilPressureVert += toM(pBri->GetUnderGroundPaveHeightTosa()) * toTon(pBri->m_dUnderGroundPaveWeightTosa) * pBri->m_dUnderGroundPaveWidthTosa;
			
			m_dSoilPressureVert /= dWidthSlab;
			m_dSoilPressureVert  = Round(m_dSoilPressureVert, bLsd? 4 : 3);
		}
		else
		{
			// (ARCBRIDGE-2939) ���������� �ƴҶ��� �����ǿ��� ����β��� ������Ѵ�.
			double H  = toM(m_dUnderGroundDesignHeight - (m_bRailDesign? 0 : pBri->m_dUnderGroundHeightPavement));
			double UW = pBri->m_dUnitWeightHeapSoil;
			m_dSoilPressureVert = H*UW;
		}
	}
	else
	{
		// ���߶���� �ƴϸ� �ش� ���� ������ �ʾƾ� �Ѵ�.
/*		m_dSoilPressureVert = 0;
		if(toM(pBri->m_dUnderGroundPaveHeightTosa_XXX) > 0)
		{
			m_dSoilPressureVert += toM(pBri->m_dUnderGroundPaveHeightTosa_XXX) * toTon(pBri->m_dUnderGroundPaveWeightTosa);
			m_dSoilPressureVert  = Round(m_dSoilPressureVert,3);
		}*/
	}

	for(long ix=0; ix<2; ++ix)
	{
		m_dWeightFootway[ix]	= 0;
		m_dWeightBangHo[ix]		= 0;
		m_dWeightCentro[ix]		= 0;
		m_dWeightBoCha[ix]		= 0;
		m_dWeightNangan[ix]		= 0;
		m_dWeightSound[ix]		= 0;
		m_dWeightNanganBase[ix]	= 0;
		m_dWeightSoundBase[ix]	= 0;
		m_dWeightRetainingWall[ix]	= 0;
		m_dUpperPBangHo[ix]			= 0;			// ��ȣ�� ��� P
		m_dUpperPCentro[ix]			= 0;
		m_dUpperPBoCha[ix]			= 0;
		m_dUpperPRetainingWall[ix]	= 0;

		m_dArrUpperWeight[ix].RemoveAll();
		m_dArrAreaBangHo[ix].RemoveAll();
		m_dArrAreaCentro[ix].RemoveAll();
		m_dArrAreaBoCha[ix].RemoveAll();
		m_dArrAreaNanganBase[ix].RemoveAll();
		m_dArrAreaSoundBase[ix].RemoveAll();
		m_dArrAreaRetainingWall[ix].RemoveAll();
	}

	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix==0)? FALSE : TRUE;
		if(bDW && !bLsd)
			continue;

		CTwinVectorArray tvArr;
		CTwinVector tv, tvTemp;
		CStringArray strArr;

		// LSD���迡�� ������� ������ ��� DW �����̴�.
		long nLoadCaseUpperP = bLsd? 1 : 0;

		long nQtyHDan = pBri->GetQtyHDan();
		for(int nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			CGuardWallRC *pGW = pBri->GetGuardWallByHDan(nHDan); // �߸��� �Լ��� ����.... ��ȣ���� Ⱦ�ܼ���ŭ ����.

			if(!pGW) continue;
			if(bLsd)
			{
				if((pGW->m_bDWload || pGW->IsPaveOnHDan()) != bDW)
				{
					m_dArrUpperWeight[ix].Add(0);
					continue;
				}
			}

			double dArea1  = 0.;
			//		double dArea2  = 0.;
			double dLength = 0.;
			if(pGW)
			{
				CDomyun pDom;
				dArea1  = DrawDeadLoadSectionEtc(&pDom, pBri, pGW, strArr);
			}

			if(pGW->IsTypeHDanRoadGuardFence()/* || pGW->IsTypeHDanRailDosangGravel()*/)		// �ڰ������� ������ �̹� ������.
			{
				m_dWeightBangHo[ix] += dArea1*UW_Conc;

				m_dUpperPBangHo[nLoadCaseUpperP]	+= pGW->m_dForce;
				m_dArrAreaBangHo[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRailDetectGravel2() || pGW->IsTypeHDanRailDetectGravel())	// ö��(�ڰ�����(����))
			{
				double dUW = pGW->IsTypeHDanRailDetectGravel2()? UW_ConcOnly : UW_Conc;
				m_dWeightBangHo[ix] += dArea1*dUW;
				m_dUpperPBangHo[nLoadCaseUpperP]	+= pGW->m_dForce;
				m_dArrAreaBangHo[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*dUW);
			}
			else if(pGW->IsTypeHDanRoadSoundProof())
			{
				m_dWeightSound[nLoadCaseUpperP]     += pGW->m_dForce;
				m_dWeightSoundBase[ix] += dArea1*UW_Conc;
				m_dArrAreaSoundBase[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRoadRetainingWall())
			{
				m_dWeightRetainingWall[ix] += dArea1*UW_Conc;
				m_dUpperPRetainingWall[nLoadCaseUpperP] += pGW->m_dForce;
				m_dArrAreaRetainingWall[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRoadHandRail())
			{
				m_dWeightNangan[nLoadCaseUpperP]     += pGW->m_dForce;
				m_dWeightNanganBase[ix] += dArea1*UW_Conc;
				m_dArrAreaNanganBase[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRoadTotalCentralWall())
			{
				m_bGWhalf = pGW->m_bGWhalf;
				if(m_bGWhalf) m_dWeightCentro[ix] += dArea1*UW_ConcOnly;
				else m_dWeightCentro[ix] += dArea1*UW_Conc;
				m_dUpperPCentro[nLoadCaseUpperP]	+= pGW->m_dForce;
				m_dArrAreaCentro[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRoadFootwayBlock() || pGW->IsTypeHDanRailFootwayBlock())
			{
				dArea1 = (pGW->m_dH3 * pGW->m_dW3) + (pGW->m_dH1 * (pGW->m_dW3 + pGW->m_dW1 + pGW->m_dW5)) - (pGW->m_dH5 * pGW->m_dW7);
				dArea1 = toM2(dArea1);
				m_dWeightBoCha[ix] += dArea1*UW_Conc;
				m_dUpperPBoCha[nLoadCaseUpperP] += pGW->m_dForce;
				m_dArrAreaBoCha[ix].Add(dArea1);
				m_dArrUpperWeight[ix].Add(dArea1*UW_Conc);
			}
			else if(pGW->IsTypeHDanRoadFootway() || pGW->IsTypeHDanRailFootway())
			{
				//
				CTwinVectorArray tvArr;
				double dSlopeSlab = pBri->GetSlopeHDan(nHDan);
				if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope && pBri->IsLevelHDanApplyLineSlope(pGW->m_nNumberHDan))		// 31695
					dSlopeSlab = 0;
				pGW->GetTvGuardWallBase(tvArr, FALSE, FALSE, dSlopeSlab);

				dLength = toM(pBri->GetLengthHDan(nHDan)); 
				double UW_AsconFootway  = Round(toTon(pGW->m_dUnitWeightPave),3);
				double UW_ConcFootway   = Round(toTon(pGW->m_dUnitWeightInnerFill),3);

				CTwinVector tvL = tvArr.GetAt(5);
				CTwinVector tvR = tvArr.GetAt(9);
				double dL1 = toM(tvL.GetVerLength());
				double dL2 = toM(tvR.GetVerLength());
				dArea1 = (dL1+dL2)/2 * dLength * UW_ConcFootway;
				double dArea2 = toM(pGW->m_dThickPaveFootway) * dLength * UW_AsconFootway;
				m_dArrUpperWeight[ix].Add(dArea1+dArea2); // �߷�
				m_dWeightFootway[ix] += dArea1+dArea2;
			}
			else if(pGW->IsTypeHDanOnlyNone())
			{
				dLength = toM(pBri->GetLengthHDan(nHDan));
				dArea1 = dLength*dThickAsocn;
				m_dArrUpperWeight[ix].Add(dArea1*UW_Ascon);
			}
		}
	}
	
//////////////////////////////////////////////////////////////////////////

	long nRound = pData->IsSI() ? 4 : 3;
	// ��������
	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix==0)? FALSE : TRUE;
		m_DeadLoadDistribution[ix]	= 0;

		if(bDW && !bLsd)
			continue;
		
		// ������ ������ DW�� �����Ѵ�.
		if(bLsd == bDW)
		{
			m_DeadLoadDistribution[ix] += Round(m_dWeightAscon/dWidthSlab,nRound);
			m_DeadLoadDistribution[ix] += Round(m_dWeightMortar/dWidthSlab,nRound);
		
			m_DeadLoadDistribution[ix] += Round(m_dWeightRailLane,nRound);
			m_DeadLoadDistribution[ix] += Round(m_dWeightRailDosang/dWidthSlab,nRound);
		}
		
		m_DeadLoadDistribution[ix] += Round((m_dWeightBangHo[ix]+m_dUpperPBangHo[ix])/dWidthSlab,nRound);
		m_DeadLoadDistribution[ix] += Round(m_dWeightFootway[ix]/dWidthSlab,nRound);
		m_DeadLoadDistribution[ix] += Round((m_dWeightCentro[ix]+m_dUpperPCentro[ix])/dWidthSlab,nRound);
		m_DeadLoadDistribution[ix] += Round(m_dWeightNanganBase[ix]/dWidthSlab,nRound);	//
		m_DeadLoadDistribution[ix] += Round((m_dWeightRetainingWall[ix]+m_dUpperPRetainingWall[ix])/dWidthSlab,nRound);	//
		m_DeadLoadDistribution[ix] += Round(m_dWeightSoundBase[ix]/dWidthSlab,nRound);		//
		m_DeadLoadDistribution[ix] += Round((m_dWeightBoCha[ix]+m_dUpperPBoCha[ix])/dWidthSlab,nRound);
		m_DeadLoadDistribution[ix] += Round(m_dWeightNangan[ix]/dWidthSlab,nRound);
		m_DeadLoadDistribution[ix] += Round(m_dWeightSound[ix]/dWidthSlab,nRound);
		
	}
}

// ���߶�౳�� ���� Ȱ���� ���
void CARcBridgeCalcLoad::CalculateLiveLoadUnderGround()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen) return;

	long nDesignSelect = pBri->m_nUnderGroundDesignSelect; // ���Ȱ���� �������
	long nDesignType   = pBri->m_nUnderGroundDesignType;   // ���Ȱ���� ����

	m_dLiveLoaUR_Ko = m_pStd->m_dCoefficientKo;
	m_dLiveLoaUR_D	= toM(m_dUnderGroundDesignHeight);
	switch(nDesignType)
	{
		// ���λ�������
		case 0 :
			// ��������ö����
			if(nDesignSelect==0)
			{
				m_dLiveLoaUR_Pvl = pBri->m_dUnderGroundDesignLiveAlways;
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * pBri->m_dUnderGroundDesignLiveAlways;
			}
			// ö�����������(�����)
			else if(nDesignSelect==1)
			{
				m_dLiveLoaUR_Pvl = pBri->m_dUnderGroundDesignLiveAlways;
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * pBri->m_dUnderGroundDesignLiveAlways;
			} 
			// ���μ�����(��) : 2��, 3.3.2 606������
			else if(nDesignSelect==2)
			{
				double Pr = m_pStd->GetWheelLoadRear();
				// (��) ���ǰ� 4m �̸��� ���
				if(m_dLiveLoaUR_D<4.0)
				{
					m_dLiveLoaUR_W1	= 2 * m_dLiveLoaUR_D + 0.2;
					m_dLiveLoaUR_B  = toM(pBri->GetLengthModelSpan(-1));
					m_dLiveLoaUR_Bo = toM(GetLengthSpanLiveLoad());
					m_dLiveLoaUR_i	= 0; // <ǥ 3.3>
					if(m_dLiveLoaUR_D<=3.00 && m_dLiveLoaUR_D>2.00) m_dLiveLoaUR_i = 0.1;
					if(m_dLiveLoaUR_D<=2.00 && m_dLiveLoaUR_D>1.00) m_dLiveLoaUR_i = 0.2;
	//				if(m_dLiveLoaUR_D<=1.00 && m_dLiveLoaUR_D>0.15) m_dLiveLoaUR_i = 0.3;
					if(m_dLiveLoaUR_D<=1.00) m_dLiveLoaUR_i = 0.3;
					m_dLiveLoaUR_P = 2.0 * Pr / 3.0;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W1)
					{
						double P  = m_dLiveLoaUR_P;
						double i  = m_dLiveLoaUR_i;
						double Bo = m_dLiveLoaUR_Bo;
						double D  = m_dLiveLoaUR_D;
						m_dLiveLoaUR_Pvl = 2*P*(1+i)*(Bo-D-0.1)/pow(Bo,2);
					}
					else
					{
						m_dLiveLoaUR_Pvl = m_dLiveLoaUR_P*(1+m_dLiveLoaUR_i)/m_dLiveLoaUR_W1;
					}
					m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * m_dLiveLoaUR_Pvl;
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					m_dLiveLoaUR_Pvl = 1.0;
					m_dLiveLoaUR_Phl = 0.6;
				}
			}
			// ���μ������(��) : 2�� 408-30
			else if(nDesignSelect==3)
			{
				double Pr = m_pStd->GetWheelLoadRear();
				// (��) ���ǰ� 4m �̸��� ���
				if(m_dLiveLoaUR_D<4.0)
				{
					m_dLiveLoaUR_W1	= 2 * m_dLiveLoaUR_D + 0.2;
					m_dLiveLoaUR_B  = 3.0;
					m_dLiveLoaUR_Bo = toM(GetLengthSpanLiveLoad());
					m_dLiveLoaUR_i	= 0; // <ǥ 408.9>
					if(m_dLiveLoaUR_D<=3.00 && m_dLiveLoaUR_D>2.00) m_dLiveLoaUR_i = 0.1;
					if(m_dLiveLoaUR_D<=2.00 && m_dLiveLoaUR_D>1.00) m_dLiveLoaUR_i = 0.2;
	//				if(m_dLiveLoaUR_D<=1.00 && m_dLiveLoaUR_D>0.15) m_dLiveLoaUR_i = 0.3;
					if(m_dLiveLoaUR_D<=1.00) m_dLiveLoaUR_i = 0.3;
					m_dLiveLoaUR_P = 2.0 * Pr / 3.0;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W1)
					{
						double P  = m_dLiveLoaUR_P;
						double i  = m_dLiveLoaUR_i;
						double Bo = m_dLiveLoaUR_Bo;
						double D  = m_dLiveLoaUR_D;
						m_dLiveLoaUR_Pvl = 2*P*(1+i)*(Bo-D-0.1)/pow(Bo,2);
					}
					else
					{
						m_dLiveLoaUR_Pvl = 2*Pr*(1+m_dLiveLoaUR_i)/(m_dLiveLoaUR_B*m_dLiveLoaUR_W1);
					}
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					m_dLiveLoaUR_Pvl = 1.0;
				}
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * m_dLiveLoaUR_Pvl;
			}
			// �����������(��) : 2001�� 6.06
			else if(nDesignSelect==4)
			{
				double Pr = m_pStd->GetWheelLoadRear();
				double Pf = m_pStd->GetWheelLoadFront();
				m_dLiveLoaUR_i	= 0; // <ǥ 408.9>
				if(m_dLiveLoaUR_D<=3.00 && m_dLiveLoaUR_D>2.00) m_dLiveLoaUR_i = 0.1;
				if(m_dLiveLoaUR_D<=2.00 && m_dLiveLoaUR_D>1.00) m_dLiveLoaUR_i = 0.2;
				if(m_dLiveLoaUR_D<=1.00) m_dLiveLoaUR_i = 0.3;

				m_dLiveLoaUR_W1	= 2 * m_dLiveLoaUR_D + 0.2;
				m_dLiveLoaUR_W	= 4.2 * 2 + m_dLiveLoaUR_W1;
				m_dLiveLoaUR_Bo = toM(GetLengthSpanLiveLoad());
				m_dLiveLoaUR_B  = 3.0;
				m_dLiveLoaUR_P  = 2.0 * Pr / m_dLiveLoaUR_B;
				// (��) �κе���� ���� �ۿ�� : 0.60��D��2.0
				if(m_dLiveLoaUR_D<=2.0 && m_dLiveLoaUR_D>=0.6)
				{
					double P  = m_dLiveLoaUR_P;
					double i  = m_dLiveLoaUR_i;
					double Bo = m_dLiveLoaUR_Bo;
					double D  = m_dLiveLoaUR_D;

					m_dLiveLoaUR_Pvl1 = P*(1+i)/(2*D+0.2);
					double Pvl1 = m_dLiveLoaUR_Pvl1;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W)
					{
						m_dLiveLoaUR_Pvl  = 8*P*(1+i)*(0.75*Bo-0.25*D-4.225)/pow(Bo,2);
					}
					else
					{
						m_dLiveLoaUR_Pvl  = 8*Pvl1*(0.125*Bo*Bo-2*Bo+8.4+(Bo-4.2)*D)/pow(Bo,2);
					}
				}
				// (��) ����� ���� �ۿ�� : 0.60��D��2.0
				else if(m_dLiveLoaUR_D<4.0 && m_dLiveLoaUR_D>=2.0)
				{
					double P  = m_dLiveLoaUR_P;
					double P2  = 2.0 * Pf / m_dLiveLoaUR_B;
					double i  = m_dLiveLoaUR_i;
					double Bo = m_dLiveLoaUR_Bo;
					double D  = m_dLiveLoaUR_D;

					m_dLiveLoaUR_Pvl1 = P*(1+i)/(2*D+0.2);
					// (ARCBRIDGE-3295 ) Pvl2 = 2 x P2 x (1 + i) / (2D + 0.2) -> Pvl2 = 2 x P1 x (1 + i) / (2D + 0.2)���� ����
					m_dLiveLoaUR_Pvl2 = 2*P*(1+i)/(2*D+0.2);
					double Pvl1 = m_dLiveLoaUR_Pvl1;
					double Pvl2 = m_dLiveLoaUR_Pvl2;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W)
					{
						m_dLiveLoaUR_Pvl  = 8*Pvl1*(4.15*Bo-17.645-(0.5*Bo+0.1)*D-0.5*D*D) / pow(Bo,2) + 8*Pvl2*(8.4-2*Bo+(Bo-4.2)*D) / pow(Bo,2);
					}
					else
					{
						m_dLiveLoaUR_Pvl  = 8*Pvl1*(0.125*Bo*Bo+2*Bo-8.4-(Bo-4.2)*D) / pow(Bo,2) + 8*Pvl2*(8.4-2*Bo+(Bo-4.2)*D) / pow(Bo,2);
					}
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					m_dLiveLoaUR_Pvl = 1.0;
				}
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * m_dLiveLoaUR_Pvl;
			}
			break;
		// ö���������� : ö���������(�����) 2.2.3
		case 1 :
			{
				double P = pData->m_RailLoadWheel;
				double W = pData->m_DistributionLoadFront;
				if(pData->IsRailDesign2017() && pBri->m_nUnderGroundDistribute == 0) //���ϱ�����
				{
					m_dLiveLoaUR_Pvl = P / (m_dRail_WidthDistributeJong * m_dRail_WidthDistribute) + W / m_dRail_WidthDistribute;
				}
				else //���� �� ����ü�
				{
					m_dLiveLoaUR_Ko = 0.40; // ������ ���� �ִ� -> ����x
					double He = m_dLiveLoaUR_D;
					double a  = toM(pBri->m_dRail_DistBetweenWheel);
					double b  = toM(pBri->m_dRail_LengthTie);
					double c  = toM(pBri->m_dRail_DistBetweenRail);
					double d  = toM(pBri->m_dHeightDosangUnderTie);
					if(pData->IsRailDesign2017())
						P += W * 3; //����

					if(He<0.5)
					{
						m_dLiveLoaUR_Pvl = P / (a*(b+2*d));
					}
					else
					{
						if(m_bSingleLane)
							m_dLiveLoaUR_Pvl = P / (a*(b+2*d));
						else
							m_dLiveLoaUR_Pvl = 2 * P / (a*(c+b+2*(d+He)));
					}
				}

				if(pData->m_nTypeFootingSafetyApplyCoefficientImpact == 0)
				{
					m_dLiveLoaUR_Pvl = m_dLiveLoaUR_Pvl * (1+m_dCoefficientImpact[0]);
				}
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * m_dLiveLoaUR_Pvl;
				break;
			}
		// ������Է�
		case 2 :
			{
				m_dLiveLoaUR_Pvl = pBri->m_dUnderGroundDesignLiveAlways;
				m_dLiveLoaUR_Phl = (pBri->m_nSelectLiveLoadHori==0) ? 0 : m_dLiveLoaUR_Ko * pBri->m_dUnderGroundDesignLiveAlways;
			}
			break;
		default :
			break;
	}	
}

void CARcBridgeCalcLoad::CalculateLiveLoad()
{
	if(m_bRailDesign) return;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge    = pBri->GetStationOnJijum(nSpan,nLine);
//	m_dSpanLengthDesign = toM(m_pStd->GetLengthModelSpanMin());
//	if(m_pStd->IsSkew())
//	{
//		CDPoint vAngle = pBri->GetAngleJijum(0);
//		double dAngDegree = MIN(vAngle.GetAngleDegree(),180-vAngle.GetAngleDegree());
//		double B  = m_pStd->GetSlabWidth();
//		double ls = m_dSpanLengthDesign;
//		double ln = ls*vAngle.y;
//		if(dAngDegree<75 && ls/B<1.50)
//		{
//			m_bReCalcSpanLength = TRUE; 
//			m_dSpanLengthDesign = (ls+ln)/2;
//			m_dReCalcRateSpan   = m_dSpanLengthDesign/ls;
//		}
//	}
//	else
//	{
//		m_bReCalcSpanLength = FALSE;
//	}
	CalculateSpanLength();
	m_dWidthBetweenCurb = Round(toM(pBri->GetWidthTotalRoadWay(dStationBridge)), 3);

	// ��ȿ������ : ���α� ������� 4.7.5.1,(2)
	m_dEffectiveWidth = Round(MIN(1.2 + 0.06*m_dSpanLengthDesign,2.1),3);
	if (pData->IsLsdDesign())
	{
		m_dEffectiveWidthLane2 = m_dEffectiveWidth;
		if (pBri->m_nLoadEffeciveW == 0)
		{
			HGDOUBLE dR = GetRForEffectiveWidth(pBri);

			HGDOUBLE dSpanLengthDesign = min(18000., GetValueUnitChange(m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM));
			HGDOUBLE dWidthBetweenCurb = min(9000., GetValueUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM));
			m_dEffectiveWidth = 1.2 * (250 + 0.42 * sqrt(dSpanLengthDesign * dWidthBetweenCurb)) * dR;
			m_dEffectiveWidth = toM(m_dEffectiveWidth);

			dWidthBetweenCurb = min(18000., GetValueUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM));
			m_dEffectiveWidthLane2 = 1.1 * (2100 + 0.12 * sqrt(dSpanLengthDesign * dWidthBetweenCurb)) * dR;
			m_dEffectiveWidthLane2 = toM(m_dEffectiveWidthLane2);
		}
	}
	
	// ��ݰ�� ����
	if (pData->IsLsdDesign())
	{
		m_dCoefficientImpact[0] = 0.250; // [���α� �������] 3.7.1
		m_dCoefficientImpact[1] = 0.150;
	}
	else if(pBri->m_bCheckUseri)
		m_dCoefficientImpact[0] = pBri->m_dCheckUseriValue;
	else
		m_dCoefficientImpact[0] = Round(MIN(15.0/(40.0+m_dSpanLengthDesign),0.30),3);
	// �������� ����, ������ ���
	long nCountLane = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));
	if(nCountLane>0)
	{
		m_bExistLane = TRUE;
		m_dDesignRoadWidth = toM(m_pStd->GetDesignRoadWidth());

		// DB���߻���
		double dPfDesign = m_pStd->GetWheelLoadFront();
		double dPmDesign = m_pStd->GetWheelLoadMiddle();
		double dPrDesign = m_pStd->GetWheelLoadRear();

		for(long nTypeLive=0; nTypeLive<2; ++nTypeLive)
		{
			m_nCaseDesignLane[nTypeLive] = 0;
			m_dPfApply[nTypeLive] = 0;
			m_dPmApplyTruck[nTypeLive] = 0;
			m_dPrApply[nTypeLive] = 0;

			if(pData->IsLsdDesign() == FALSE && nTypeLive > 0)
				continue;

			for(long nLane=1; nLane<=nCountLane; nLane++)
			{
				double ApplyFactor = GetLaneFactor(pBri, nLane);
				HGINT32 iLane = (pData->IsLsdDesign() && pBri->m_nLoadEffeciveW == 0) ? 2 : 2*nLane;

				double E  = GetEForEffectiveWidth(pBri, nLane);;
				double Pr = (iLane*dPrDesign*(1+m_dCoefficientImpact[nTypeLive]))/E;
				double Pm = (iLane*dPmDesign*(1+m_dCoefficientImpact[nTypeLive]))/E;
				double Pf = (iLane*dPfDesign*(1+m_dCoefficientImpact[nTypeLive]))/E;

				Pr *= ApplyFactor;
				Pm *= ApplyFactor;
				Pf *= ApplyFactor;

				if(nTypeLive == 1)
				{
					// (ARCBRIDGE-2982) �Ƿ��Ѱ�����϶� �󵵿����� ��� ���� ���ϵ��� ����
					// (ARCBRIDGE-2496) �Ƿ��Ѱ���� Ȱ����.
					// double dArrFactor[3] = { 1.0, 0.85, 0.8 };
					// double dFactor = (nLane < 3)? dArrFactor[nLane-1] : dArrFactor[2];
					double dFactor = 1.0;

					Pr *= dFactor * 0.8;
					Pm *= dFactor * 0.8;
					Pf *= dFactor * 0.8;
				}

				if(m_dPfApply[nTypeLive]<Pf)
				{
					m_dPfApply[nTypeLive] = Pf;
					m_dPmApplyTruck[nTypeLive] = Pm;
					m_dPrApply[nTypeLive] = Pr;
					m_nCaseDesignLane[nTypeLive] = nLane;
				}
			}

			if (pData->IsLsdDesign())
			{
				if(nTypeLive == 0)
				{
					// ǥ����������
					if (m_dSpanLengthDesign <= 60)
					{
						m_dLaneLoad = m_pStd->GetWheelLoadDistribution() / 3.0;
					}
					else
					{
						m_dLaneLoad = m_pStd->GetWheelLoadDistribution() * pow((60/m_dSpanLengthDesign), 0.1) / 3.0;
					}
				}
			}
			else
			{
				// DL���߻���
				m_dPmApply[nTypeLive] = m_pStd->GetWheelLoadPm() * (1+m_dCoefficientImpact[nTypeLive]) / 3;
				m_dPsApply[nTypeLive] = m_pStd->GetWheelLoadPs() * (1+m_dCoefficientImpact[nTypeLive]) / 3;
				m_dDistributionApply  = m_pStd->GetWheelLoadDistribution() * (1+m_dCoefficientImpact[nTypeLive]) / 3;
			}
		}
	}
	else
	{
		m_bExistLane		= FALSE;
		m_dDesignRoadWidth	= 0;
		m_nCaseDesignLane[0]	= 0;
		m_dPfApply[0]			= 0;
		m_dPrApply[0]			= 0;
		m_dDistributionApply= 0;
	}

	// ����Ȱ����
	double dWidthSlab = toM(pBri->GetWidthSlabAct(dStationBridge,vAng));
	double dWidthFootway = 0;
	long nQtyHDan  = pBri->GetQtyHDan();
	for(int nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(!pGW || !pGW->IsTypeHDanRoadFootway()) continue;
		dWidthFootway += toM(pBri->GetLengthHDan(nHDan));
	}
	m_dLiveLoadFootway = pBri->m_dLiveLoadFootway*dWidthFootway/dWidthSlab;
}

// ��ũ �� ����������
void CARcBridgeCalcLoad::CalculateLiveLoadTank()
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsApplyTankLoad() == FALSE) return;

	m_dTank_E = MIN(1.20+0.06*m_dSpanLengthDesign, 2.1);	// m_dEffectiveWidth;
	m_dTank_i = pData->m_dTank_i;
	//
	m_dTank_WeightTank	 = pData->m_dTank_WeightTank;
	m_dTank_WeightTrailer= (pData->m_dTank_Ptrailer1 + pData->m_dTank_Ptrailer2*2 + pData->m_dTank_Ptrailer3*2)*2 ;
	m_dTank_TankLength	 = pData->m_dTank_TankLength;
	m_dTank_TankWidth	 = pData->m_dTank_TankWidth_Side + pData->m_dTank_TankWidth_Inner;
	m_dTank_TrailerL1	 = pData->m_dTank_TrailerL1;
	m_dTank_TrailerL2	 = pData->m_dTank_TrailerL2;
	m_dTank_TrailerL3	 = pData->m_dTank_TrailerL3;
	m_dTank_TrailerL4	 = pData->m_dTank_TrailerL4;
	m_dTank_TrailerWidth = (pData->m_dTank_TrailerWidth_Side + pData->m_dTank_TrailerWidth_Inner);

	// ��ũ����
	double P  = m_dTank_WeightTank/2/m_dTank_TankLength;
	double Ea = m_dTank_TankWidth + m_dTank_E;
	m_dTank_Ptank = P*(1+m_dTank_i)*2/Ea;

	// Ʈ���Ϸ�����
	double P1 = pData->m_dTank_Ptrailer1;
	double P2 = pData->m_dTank_Ptrailer2;
	double P3 = pData->m_dTank_Ptrailer3;
	Ea = m_dTank_TrailerWidth + m_dTank_E;
	m_dTank_Ptrailer1 = P1*(1+m_dTank_i)*2/Ea;
	m_dTank_Ptrailer2 = P2*(1+m_dTank_i)*2/Ea;
	m_dTank_Ptrailer3 = P3*(1+m_dTank_i)*2/Ea;
}

// ��ݰ���� ����
// L��18 i = 45 - L��/45;
// L��18 i = 24 + 240/(L-0.6);
// ---
// ������ �����ϴ� ������ ���
// 1) L<50		�α˵��� �����
// 2) 50��L��70	�ѱ˵��� ����ݰ� �ٸ� �˵� ������� (350-5L)%
// 3) L>70		�ѱ˵����� �����
// 3���̻� ���ؼ��� 2�˵��� ����ݸ��� ����Ѵ�.
// ---
// ��������鿡 ���� 1m�̻��� ��� i = io x (1 - 0.1*(Hc - 1.0)), Hc : ��������鿡�� ħ���ܱ����� �������
void CARcBridgeCalcLoad::CalculateRailLoadLive()
{
	if(!m_bRailDesign) return;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
/*	
	m_dSpanLengthDesign = toM(m_pStd->GetLengthModelSpanMin());
	if(pBri->m_nSelectDesignSpanLength==1) m_dSpanLengthDesign = toM(pBri->m_dLengthBridge);
	if(pBri->m_nSelectDesignSpanLength==2) m_dSpanLengthDesign = toM(m_pStd->GetLengthModelSpanMax());

	// �簢�� �ִ� ����
	if(m_pStd->IsSkew())
	{
		CDPoint vAngle = pBri->GetAngleJijum(0);
		double dAngDegree = MIN(vAngle.GetAngleDegree(),180-vAngle.GetAngleDegree());
		double B  = m_pStd->GetSlabWidth();
		double ls = m_dSpanLengthDesign;
		double ln = ls*vAngle.y;

		if(Compare(dAngDegree,75.0,">="))
		{
			// ������ �簢�� 75���̻� �̹Ƿ� ��������� �����Ѵ�.
		}
		else if(ls/B>=1.50)
		{
			// ������ �簢�� 75������ �̰�, ls/B �� 1.5�Ƿ� ��������� �����Ѵ�.
		}
		else
		{
			// (ls + ln) / 2
			m_dSpanLengthDesign = (ls + ln) / 2.0;
		}
	}*/
	CalculateSpanLength();

	double L  = m_dSpanLengthDesign;

	// ��ݰ�� ����
	if(pBri->m_pARcBridgeDataStd->IsRailDesign2017())
	{
		double dI0 = 1.44 / (sqrt(m_dSpanLengthDesign) - 0.2) - 0.18;
		m_dCoefficientImpact[0] = MIN(0.67, dI0);
	}
	else
	{
		m_dCoefficientImpact[0] = Round((45.0 - pow(L,2)/45.0)*0.01,3);
		if(L>18.0)
		{
			m_dCoefficientImpact[0] = Round((24.0 + 240.0/(L-0.6))*0.01,3);
		}
	}
	
	// ���߶���̰� ������ǰ� 1.0���� �̻��� ���
	if(pBri->m_bUnderGroundRahmen && toM(m_dUnderGroundDesignHeight)>1.0)
	{
		double Hc = toM(m_dUnderGroundDesignHeight);
		m_dCoefficientImpact[0] = m_dCoefficientImpact[0] * (1 - 0.1*(Hc - 1.0));
	}

	//////////////////////////////////////////////////////////////////////////
	// ���߶������ ����ϴ� ��쿡 ����� ���� ����Ȱ���߿� ���� ��ݰ���� ����
	// ö��������� (�����) 166 ������
	// 0 - 0.600, 5 - 0.480, 10 - 0.430, 20 - 0.370, 30 - 0.340, 40 - 0.320
	m_dSpanLengthImpactMin = 1000;
	m_dSpanLengthImpactMax = 0;
	m_dSpanLengthImpactAvg = 0;
	m_dSpanLengthImpactApply = 0;

	if(pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen==FALSE)
	{
// 		double dSpanSum = 0;
// 
// 		for(long nSpan=0; nSpan<pBri->m_nQtyJigan; nSpan++)
// 		{
// 			double dSpanLength = toM(pBri->GetLengthJigan(nSpan));
// 			dSpanSum += dSpanLength;
// 			m_dSpanLengthImpactMax  = MAX(m_dSpanLengthImpactMax,dSpanLength);
// 			m_dSpanLengthImpactMin  = MIN(m_dSpanLengthImpactMin,dSpanLength);
// 		}
// 		m_dSpanLengthImpactAvg  = Round(dSpanSum/(double)(pBri->m_nQtyJigan),3);

		m_dSpanLengthImpactMax  = Round(toM(m_pStd->GetLengthModelSpanMax()),3);
		m_dSpanLengthImpactMin  = Round(toM(m_pStd->GetLengthModelSpanMin()),3);
		m_dSpanLengthImpactAvg	= Round(toM(m_pStd->GetLengthModelSpanAvg()),3);

		// �ּ������� �ִ������� 70%�̻��� ��� �����������
		// �ּ������� �ִ������� 70%�̸��� ��� �ּ���������
		m_dSpanLengthImpactApply = (m_dSpanLengthImpactMin>=0.7*m_dSpanLengthImpactMax) ? m_dSpanLengthImpactAvg : m_dSpanLengthImpactMin;
		// ǥ 2.2.4
		double L = m_dSpanLengthImpactApply;
		double dFactor1(0) ,dFactor2(0), dLenGap(0);
		double dLenBase = GetCoefficientImpactFactor(L, dFactor1, dFactor2, dLenGap);
		m_dCoefficientImpactSingle = dFactor1 + (dFactor2 - dFactor1) * (L - dLenBase) / dLenGap;

		m_dCoefficientImpact[0] = m_bSingleLane ? 1 - m_dSpanLengthDesign/200.0 : m_dCoefficientImpactSingle;

		// (ARCBRIDGE-3311) ���ǰ� + ħ����� => ���ǰ� + ������� �� ����
		// double H  = toM(m_dUnderGroundDesignHeight + m_dRail_HeightTie);
		double H  = toM(m_dUnderGroundDesignHeight + pBri->m_dHeightDosangUnderTie);
		if(H>0)
		{
			m_dCoefficientImpact[0] = m_dCoefficientImpactSingle * (2.5 - H) / 1.5;
			if(H>2.50) m_dCoefficientImpact[0] = 0;
			if(H<1.00) m_dCoefficientImpact[0] = m_dCoefficientImpactSingle;
		}
	}
}

void CARcBridgeCalcLoad::CalculateRailLoadLR()
{
	if(!m_bRailDesign) return;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	// ���ǰ� 1.0�����̻��� ���� ������� �ʴ´�.
	if(pBri->m_bUnderGroundRahmen && toM(m_dUnderGroundDesignHeight)>1.0) return;

	double dQtyRail  = m_bSingleLane ? 1.0 : 2.0;
	double dWidthEff = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);

	// ��뷹�� �������� ���μ��� �����ϰ� ���μ���ŭ �����Ѵ�.
	// �׷��� ����M�� ���Ǵ� FRAME�ؼ������� ���μ��� �ǹ̰� ���� (���������� ����)
	m_dRail_UnitForceLR = pBri->m_pARcBridgeDataStd->IsRailDesign2017() ? 0.6 : 1.0;
	m_dRail_LoadLR	 = m_dRail_UnitForceLR * dQtyRail / dWidthEff;		// �����
	m_dRail_HeightLR = toM(m_dHeightRail+m_dRail_HeightTie+m_dHeightDosangUnderTie+m_dUnderGroundDesignHeight+pBri->m_dUnderGroundHeightMortar+pBri->m_dTS/2);
	m_dRail_MomentLR = m_dRail_LoadLR * m_dRail_HeightLR;
}

// �õ����� : ������ ���������� 25%, ���ϸ�� 2.0m
void CARcBridgeCalcLoad::CalculateRailLoadSB()
{
	if(!m_bRailDesign) return;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bDesign2017 = pData->IsRailDesign2017();
	double dLengthModel  = toM(pBri->GetLengthModelSpan(-1));
	double dWidthEff = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);

	if(bDesign2017)
	{
		if(pData->IsKRL_2012())		// KRL-2012, KRL-2012(��������) - 0.75���
		{
			m_dRail_HeightSB = 1.8;

			double dFactor = (pData->m_nTypeRailLoad == RAIL_LOAD_KRL2012_P)? 0.75 : 1.0;
			m_dRail_UnitForceSidong = 3.3;
			m_dRail_UnitForceZedong = 2.0;
			m_dRail_TsbSidong = MIN(100, m_dRail_UnitForceSidong * dLengthModel * dFactor);
			m_dRail_TsbZedong = MIN(600, m_dRail_UnitForceZedong * dLengthModel * dFactor);
		}
		else if(pData->m_nTypeRailLoad == RAIL_LOAD_EL18_2017)	// EL ǥ�ؿ�������
		{
			double WheelLoad =  pBri->m_pARcBridgeDataStd->m_RailLoadWheel;

			m_dRail_HeightSB = 1.5;
			m_dRail_LengthLv = 18.0;

			m_dRail_TsbSidong = (0.27 + 0.95 * dLengthModel / m_dRail_LengthLv) * WheelLoad;
			m_dRail_TsbZedong = (0.27 + 1.00 * dLengthModel / m_dRail_LengthLv) * WheelLoad;
		}
	}
	else
	{
		m_dRail_HeightSB = 2.0;

		// �õ�����
		long nQtyWheel   = m_pStd->GetQtyRailWheel();
		double WheelLoad =  pBri->m_pARcBridgeDataStd->m_RailLoadWheel;
		m_dRail_TsbSidong    = nQtyWheel * WheelLoad * 0.25;

		// ��������
		m_nSttWheelZedong = 0;
		long nRailLoadType = pBri->m_pARcBridgeDataStd->m_nTypeRailLoad;
		if(nRailLoadType==RAIL_LOAD_LS25 || nRailLoadType==RAIL_LOAD_LS22 || nRailLoadType==RAIL_LOAD_LS18)
			m_nSttWheelZedong = 1; // ù��° �������� �����ϴ� ���� �Ҹ�
		double dWheelWeight = pBri->m_pARcBridgeDataStd->GetRailLoadWeightWheel(m_nSttWheelZedong,dLengthModel);
		double dWheelLength = pBri->m_pARcBridgeDataStd->GetRailLoadLengthWheel(m_nSttWheelZedong);	// �������� ���ϵǴ� ����
		
		double dDistribute  = pBri->m_pARcBridgeDataStd->m_DistributionLoadBack;			// ��������
		long nQtyDist = pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetSize();
		long nIdxDist = (dDistribute==0) ? nQtyDist-1 : nQtyDist-2;
		double dDistDistribute = pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetAt(nIdxDist); // ������ �����߿��� �������߱����� �Ÿ�

		// �Ϲ����� ��� ���������� ���� (�� ������ �� ��� ����)
		if(dLengthModel<=(dWheelLength+dDistDistribute))
			m_dRail_TsbZedong = dWheelWeight * 0.150;
		else
			m_dRail_TsbZedong = (dWheelWeight + dDistribute*(dLengthModel-dWheelLength-dDistDistribute)) * 0.150;
	}

	// �õ�����
	m_dRail_LoadSidong	 = m_dRail_TsbSidong / dWidthEff / dLengthModel;
	m_dRail_HeightSidong = toM(m_dHeightRail+m_dRail_HeightTie+m_dHeightDosangUnderTie+m_dUnderGroundDesignHeight+pBri->m_dUnderGroundHeightMortar+pBri->m_dTS/2) + m_dRail_HeightSB;
	m_dRail_MomentSidong = m_dRail_LoadSidong * m_dRail_HeightSidong;

	// ��������
	m_dRail_LoadZedong  = m_dRail_TsbZedong / dWidthEff / dLengthModel;
	m_dRail_HeightZedong = m_dRail_HeightSidong;
	m_dRail_MomentSidong = m_dRail_LoadZedong * m_dRail_HeightZedong;

	m_dRail_LoadSB	 = m_bSingleLane ? MAX(m_dRail_LoadZedong,m_dRail_LoadSidong) : (m_dRail_LoadZedong+m_dRail_LoadSidong);
	m_dRail_MomentSB = m_dRail_LoadSB * m_dRail_HeightSidong;
}

double CARcBridgeCalcLoad::GetLoadWaterPressure(double dElevation)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;

	double dEle_Water = toM(pBri->m_dGroundWaterLevel);
	
	if(dElevation > dEle_Water) return 0.0;


	double dUW_Water = toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightWater);

	return Round((dEle_Water - dElevation) * dUW_Water, 3);
}


void CARcBridgeCalcLoad::CalculateWaterLoad()
{
	m_dLoadBuoyancy = 0;
	m_dLoadWaterVert = 0;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	if(pBri->m_bApplyGroundWaterLevel)
	{
		double dEle_Water = toM(pBri->m_dGroundWaterLevel);
		double dEle_Bottom = toM(pBri->m_dELSB);

		m_dLoadWaterVert = dEle_Water - dEle_Bottom;
		m_dLoadBuoyancy  = dEle_Water - dEle_Bottom;

		// ��������� �Լ��� ó�� : GetLoadWaterPressure()
	}
}


void CARcBridgeCalcLoad::CalculateSoilPressure()
{
	// ��������� �Լ��� ó�� : GetLoadSoilPressure()
}

void CARcBridgeCalcLoad::CalculateTemperature()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if (pData->IsLsdDesign() == FALSE)
	{
		// ����������� �µ����� : ���α� ������� 2.1.12
		m_dLoadTempertureAxial  = (pBri->m_dTS>=700) ? 10 : 15;
	}
	else
	{
		m_dLoadTempertureAxial = pBri->m_dTemperatureChangeLoadGap;
	}
	
	// ���ϸ��� �µ���
	m_dLoadTempertureUpDown = 5;
}

void CARcBridgeCalcLoad::CalculateDryAndShrinkage()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	m_dLoadDryAndShrinkage = pBri->GetRateDryAndShrinkage()/pBri->GetCoefficientExpansion();
}

void CARcBridgeCalcLoad::CalculateSubsidence()
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if (pData->IsLsdDesign() == FALSE)
	{
		m_dLoadSubsidence = 0.01; //m
	}
	else
	{
		m_dLoadSubsidence = toM(pBri->m_dDepthApplySubSid); //m
	}
}

double CARcBridgeCalcLoad::GetLoadSoilPressure(double dHeightFromSlab, BOOL bSttWall, BOOL bExistWater, BOOL bEarthQuake, EGoalOfEarthQuake eGoalEq, BOOL bVertLoad)
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(bEarthQuake && pBri->IsBoxType() == FALSE) return 0.0;

	// ��꼭 ��� ���� ���߱� ���ؼ� �Ҽ��� 3�ڸ� ������ ������.
	dHeightFromSlab = Round(dHeightFromSlab, 3);

	double Ko = bEarthQuake ? m_pStd->GetCoefficient_MononobeOkabe(eGoalEq) : m_pStd->m_dCoefficientKo;
	if(bVertLoad)
	{
		Ko = 1;
	}

	double UW_BackFill  = pBri->m_dUnitWeigthBackFill;
	double UW_Ascon	  = m_UW_Ascon;
	double dLiveAlways  = pData->IsRailDesign() ? pBri->m_dDeadLoadAlways+pBri->m_dLiveLoadAlways : pBri->m_dLiveLoadAlways; // ö���� �˵��߷�,��������
	double dThickAsocn  = pData->IsRailDesign() ? 0 : toM(pBri->m_dTP);
	double dThickSoil   = toM(m_dUnderGroundDesignHeight);
	double dThickSlab = toM(pBri->m_dTS);

	double dWeightTotal			= m_dSoilPressureVert;

	if(pBri->m_nSelectLiveLoadHori==1)
	{
		dLiveAlways = 0;
		// (ARCBRIDGE-3598) ��а��� �������� ������ �ƴҶ� ���ܿ� �������� �߰� - ö�� �����϶���, ö���� �̴Ҷ��� ���������� ���� ����.
		if(pData->IsRailDesign())
			dWeightTotal += m_DeadLoadDistribution[0] + m_DeadLoadDistribution[1];
	}

	if (bEarthQuake == FALSE && pData->IsLsdDesign())
		dLiveAlways = 0;

	double dLoadToap  = 0;
// 	double dEle_Top = 0;
// 	double dEle_Bot = 0;
// 	double dEle_Water = 0;
	double UW_BackFill_W = pBri->m_dUnitWeigthBackFillWater;

	if(pBri->m_bUnderGroundRahmen)
	{
		// (34189) �ۿ������ �������߿� ���忡���� ���ߵ� ����.
		double dStationBridge = pBri->GetStationOnJijum(0,0);
		double dWidthSlab = toM(pBri->GetWidthSlabAct(dStationBridge, CDPoint(0,1)));
		double dPressAscon = m_dWeightAscon / dWidthSlab;

		dLoadToap  = Ko*(dWeightTotal + dPressAscon + dLiveAlways + (dThickSlab/2+dHeightFromSlab)*UW_BackFill);
	}
	else
	{
		if(bExistWater && pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
		{
			BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE; // ���� EL�� ����
			long nIdxSlab = bSttWall ? 0 : m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetSize()-1;
			double dEle_Top = toM(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(nIdxSlab).z);
			if(bUpperSlope == FALSE)
			{
				CLineInfo *pLine = pBri->GetLineBase();
				CDPoint vAngStt = pBri->GetAngleJijum(0);
				double dWS = pBri->m_dWS/vAngStt.y;
				double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
				double dSta = pBri->GetStationOnJijum(0,0) - dWS / 2.;
				dEle_Top = toM(pLine->GetElevationPlanEl(dSta) - dUT);
			}
			double dEle_Bot = dEle_Top - dHeightFromSlab;
			double dEle_Water = toM(pBri->m_dGroundWaterLevel);

			double dUW = dEle_Top <= dEle_Water ? UW_BackFill_W : UW_BackFill;

			// �𵨸� ���
			dLoadToap = Ko * (dLiveAlways + dThickAsocn*UW_Ascon + dWeightTotal + dThickSoil*dUW + dThickSlab/2*dUW);
			// ���ϼ��� ��
			dLoadToap += Ko * (MIN(dHeightFromSlab, MAX(0, dEle_Top - dEle_Water)) * UW_BackFill);
			// ���ϼ��� �Ʒ�
			dLoadToap += Ko * (MIN(dHeightFromSlab, MAX(0, dEle_Water - dEle_Bot)) * UW_BackFill_W);
		}
		else
		{
			dLoadToap = Ko*(dLiveAlways + dThickAsocn*UW_Ascon + dWeightTotal + dThickSoil*UW_BackFill + (dThickSlab/2+dHeightFromSlab)*UW_BackFill);
		}
	}

	return dLoadToap;
}

void CARcBridgeCalcLoad::WriteCalcPaveAndUpperSoil( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexTiny, BOOL bPave )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

// 	double UW_Ascon  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_Ascon	 = GetValueUnitChange(m_UW_Ascon,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_Mortar = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightCement),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge    = pBri->GetStationOnJijum(nSpan,nLine);
	double dWidthBetweenCurb = toM(pBri->GetWidthTotalRoadWay(dStationBridge));
	double dWidthSlab        = toM(pBri->GetWidthSlabAct(dStationBridge,vAng));
	double dWidthMortar = dWidthSlab - toM(pBri->m_dCantilB[iLEFT] + pBri->m_dCantilB[iRIGHT]);
	double dThickAsocn		 = toM(pBri->m_dTP);
	double dThickMortar		 = 0;
	if(pBri->m_bUnderGroundRahmen)
	{
		dThickAsocn	 = toM(pBri->m_dUnderGroundHeightPavement);
		dThickMortar = toM(pBri->m_dUnderGroundHeightMortar);
	}
	if(pData->IsRailDesign())
	{
		dThickAsocn	 = 0;
		dThickMortar = toM(pBri->m_dUnderGroundHeightMortar);
	}

	if(bLsd)
	{
		dWidthBetweenCurb	= frM(dWidthBetweenCurb);
		dWidthSlab			= frM(dWidthSlab);
		dWidthMortar		= frM(dWidthMortar);
		dThickAsocn			= frM(dThickAsocn);
		dThickMortar		= frM(dThickMortar);
	}

	double dWeightAscon			= GetValueUnitChange(m_dWeightAscon			,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightMortar		= GetValueUnitChange(m_dWeightMortar		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightSoilVert		= GetValueUnitChange(m_dSoilPressureVert	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double dWeightTotal			= dWeightSoilVert;
	double dUnderGroundPaveWeightPyo		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightPyo),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightGee		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightGee),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightMiddle	= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightMiddle),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightBozo		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightBozo),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightDong		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightDong),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightEps		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightEps),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dUnderGroundPaveWeightTosa		= GetValueUnitChange(toTon(pBri->m_dUnderGroundPaveWeightTosa),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);

	long nColFormula = bLsd? 12 : 10;
	long nColResult	= bLsd? 3 : 2;
	long nDecResult = bLsd? 6 : 3;

	// �ѰԻ����̰� ���߶���ΰ�� ����������� �з��ȴ�.
	BOOL bVerticalSoilPressure = (bLsd && pBri->m_bUnderGroundRahmen && bPave == FALSE)? TRUE : FALSE;

	XLText->GoPosition(NO_MOVE,nColStt);

	if(Compare(dWeightAscon,0.0,">") || Compare(dWeightSoilVert,0.0,">"))
	{
		if((pBri->m_bUnderGroundPaveDetail && pBri->m_bUnderGroundRahmen) || (bPave && Compare(dWeightAscon,0.0,">")))
			XLText->AddFormatText("$n$h%s$r�� ��$m+03$c:$r",GetNumberString(nIndexTiny++,NUM_CIRCLE));


		if(Compare(dWeightAscon,0.0,">") && (!pBri->m_bUnderGroundPaveDetail || !pBri->m_bUnderGroundRahmen) && (bLsd == FALSE || bPave == TRUE))
		{
			if(bLsd)
			{
				XLText->AddFormatText("$h$m+05(%s x %s) x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s", 
					pData->GetStrDouble(dThickAsocn, UNIT_CGS_M), pData->GetStrDouble(UW_Ascon, UNIT_CGS_TONF_M3, TRUE), 
					pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M), pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), 
					nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightAscon/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult,pData->m_szTonf_M2);
			}
			else
			{
				XLText->AddFormatText("$h$m+05(%.3f x %.3f$m+04", dThickAsocn, UW_Ascon);
				XLText->AddFormatText(") x %.3f / %.3f$m+06$c=$r$me1$f3$i%.3f$r$r%s", dWidthBetweenCurb, dWidthSlab,dWeightAscon/dWidthSlab,pData->m_szTonf_M2);
			}
		}

		if(pBri->m_bUnderGroundPaveDetail && pBri->m_bUnderGroundRahmen && (bLsd == FALSE || bPave == FALSE))
		{
			double dWidth(0);
			double dTickPave = 0;
			double dWpre = 0;
			if(pBri->m_dUnderGroundPaveHeightPyo > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthPyo) : pBri->m_dUnderGroundPaveWidthPyo;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightPyo : toM(pBri->m_dUnderGroundPaveHeightPyo);

				dWpre = dTickPave * dUnderGroundPaveWeightPyo * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$rǥ ��$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightPyo, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->m_dUnderGroundPaveHeightGee > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthGee) : pBri->m_dUnderGroundPaveWidthGee;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightGee : toM(pBri->m_dUnderGroundPaveHeightGee);

				dWpre = dTickPave * dUnderGroundPaveWeightGee * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$r�� ��$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightGee, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->m_dUnderGroundPaveHeightMiddle > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthMiddle) : pBri->m_dUnderGroundPaveWidthMiddle;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightMiddle : toM(pBri->m_dUnderGroundPaveHeightMiddle);

				dWpre = dTickPave * dUnderGroundPaveWeightMiddle * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$r�߰���$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightMiddle, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->m_dUnderGroundPaveHeightBozo > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthBozo) : pBri->m_dUnderGroundPaveWidthBozo;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightBozo : toM(pBri->m_dUnderGroundPaveHeightBozo);

				dWpre = dTickPave * dUnderGroundPaveWeightBozo * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$r��������$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightBozo, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->m_dUnderGroundPaveHeightDong > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthDong) : pBri->m_dUnderGroundPaveWidthDong;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightDong : toM(pBri->m_dUnderGroundPaveHeightDong);

				dWpre = dTickPave * dUnderGroundPaveWeightDong * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$r���������$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightDong, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->m_dUnderGroundPaveHeightEps > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthEps) : pBri->m_dUnderGroundPaveWidthEps;
				dTickPave = bLsd? pBri->m_dUnderGroundPaveHeightEps : toM(pBri->m_dUnderGroundPaveHeightEps);

				dWpre = dTickPave * dUnderGroundPaveWeightEps * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$rEps���$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightEps, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			if(pBri->GetUnderGroundPaveHeightTosa() > 0)
			{
				dWidth = bLsd? frM(pBri->m_dUnderGroundPaveWidthTosa) : pBri->m_dUnderGroundPaveWidthTosa;
				dTickPave = bLsd? pBri->GetUnderGroundPaveHeightTosa() : toM(pBri->GetUnderGroundPaveHeightTosa());

				dWpre = dTickPave * dUnderGroundPaveWeightTosa * dWidth / dWidthSlab;
				XLText->AddFormatText("$n$h$r�� ��$m+03$c=$r%s x %s x %s/%s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					pData->GetStrDouble(dTickPave, UNIT_CGS_M), pData->GetStrDouble(dUnderGroundPaveWeightTosa, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidth, UNIT_CGS_M), 
					pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWpre, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
		}
	}

	if(bPave == FALSE || bLsd == FALSE)
	if(Compare(dWeightSoilVert,0.0,">") && (m_dUnderGroundDesignHeight > 0 && pBri->m_bUnderGroundRahmen) && !pBri->m_bUnderGroundPaveDetail)
	{
		double H  = (bLsd ? m_dUnderGroundDesignHeight : toM(m_dUnderGroundDesignHeight)) - dThickAsocn;
		double UW = GetValueUnitChange(pBri->m_dUnitWeightHeapSoil,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
		XLText->AddFormatText("$n$h%s$r�� ��$m+03$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(UW, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult, 
			pData->GetStrDouble(dWeightSoilVert, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}
	if(Compare(dWeightSoilVert,0.0,">") && !pBri->m_bUnderGroundRahmen)
	{
		double H  = bLsd? pBri->GetUnderGroundPaveHeightTosa() : toM(pBri->GetUnderGroundPaveHeightTosa());
		double UW = dUnderGroundPaveWeightTosa;
		XLText->AddFormatText("$n$h%s$r�� ��$m+03$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(H, UNIT_CGS_M), pData->GetStrDouble(UW, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult, 
			pData->GetStrDouble(dWeightSoilVert, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightMortar,0.0,">") && (bPave || bLsd == FALSE))
	{
		XLText->AddFormatText("$n$h%s$r�� Ż$m+03$c:$r%s x %s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dThickMortar, UNIT_CGS_M), pData->GetStrDouble(UW_Mortar, UNIT_CGS_TONF_M3), pData->GetStrDouble(dWidthMortar, UNIT_CGS_M),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightMortar/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	// ��������� ��츸 �հ踦 ����Ѵ�.
	if(bVerticalSoilPressure && Compare(dWeightTotal,0.0,">"))
	{
		XLText->AddFormatText("$h$-%d", bLsd? 23 : 19);
		XLText->AddFormatText("$n$h$r�� ��$m+03$c:$r$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
			nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightTotal, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		XLText->AddText(_T("$n"));
	}

	XLText->GoPosition(NO_MOVE,nColStt);
}

void CARcBridgeCalcLoad::WriteCalcDeadLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bDW, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bDW && m_DeadLoadDistribution[1] == 0)
		return;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d �� �� �� ��%s$n",nIndexTitle,nIndexMiddle, bLsd? (bDW? _T(" (DW)") : _T(" (DC)")) : _T(""));
	if(bDW == FALSE)
	{
		XLText->AddFormatText("$n$h %d)$r��ü����$m+03$c:$r���α׷������� �ڵ����  ( w = A �� ��c )$n",nIndexSmall++);

		if(m_DeadLoadDistribution[0] == 0 && bLsd == TRUE)
			return;
	}

	XLText->AddFormatText("$n$h %d)$r��Ÿ ��������$n",nIndexSmall++);
	if(pBri->IsSkew()) XLText->AddFormatText("$h$m+16()�� ��Ÿ� ġ����$n");

	// ö�����϶��� �Ѱ���� ����ȵ�. ����� ���� ���ص� ��.
	if(m_bRailDesign)
	{
		//double H1 = toM(m_dHeightDosangUnderTie);
		double H2 = toM(pBri->m_dTS);
		//double H2 = toM(m_dUnderGroundDesignHeight+pBri->m_dTP+pBri->m_dTS/2);

		CDomyun *pDom = new CDomyun;
		DrawDeadLoadRail(pDom);
		long nSizeRow = 8;
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow, CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		double dHe = (pBri->m_bUnderGroundRahmen)? toM(pBri->m_dUnderGroundDesignHeight) : 0;
		double dPave = toM(pBri->m_dUnderGroundHeightMortar);
		double dH = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - m_dRail_HeightTie) - dPave;
		if(pBri->m_dDisBaseElevationRL == 0) 
			dH = toM(pBri->GetHeightDosangInput());
		
		if(!pData->IsRailDesign2017() || pBri->m_nUnderGroundDistribute != 1)
		{
			CString szHe(_T(""));
			if(dHe > 0)
			{
				szHe.Format(_T(" + 2 x %.3f"), dHe);
			}

			if(pData->m_nTypeCalcSlabEffWidth == 0)	// 10497
				XLText->AddFormatText("$hB$r$c=$r %.3f + 1.5 x (%.3f + %.3f)%s + %.3f$m+14$c=$r$me1$i$f3%.3f$r$rm$n",toM(m_dRail_LengthTie), dH, dPave, szHe, H2, m_dRail_WidthDistribute);
			else
				XLText->AddFormatText("$hB$r$c=$r %.3f + 1.5 x (%.3f + %.3f)%s$m+14$c=$r$me1$i$f3%.3f$r$rm$n",toM(m_dRail_LengthTie), dH, dPave, szHe, m_dRail_WidthDistribute);
			*XLText += 1;
		}		
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		CDomyun *pDom = new CDomyun;
		DrawDeadLoadHDan(pDom);
		long nSizeRow = 8;
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
	}

	double UW_Conc   = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_ConcOnly   = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightConcrete),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_Dosang = Round(GetValueUnitChange(toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightGravel),UNIT_CGS_TONF_M3,pData->m_szTonf_M3),3);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge    = pBri->GetStationOnJijum(nSpan,nLine);
//	double dWidthRoad		 = toM(pBri->m_dWidthRoadRight - pBri->m_dWidthRoadLeft);
	double dWidthSlab        = toM(pBri->GetWidthSlabAct(dStationBridge,vAng));
	
	BOOL bIncludeSoilPressureVert = TRUE;
	if(bLsd)
	{
		dWidthSlab			= frM(dWidthSlab);
		// �Ѱ���¿����� ������ �������(EV)���� ����. 
		bIncludeSoilPressureVert	= FALSE;
	}
	
	double dWeightBangHo		= GetValueUnitChange(m_dWeightBangHo[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightFootway		= GetValueUnitChange(m_dWeightFootway[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightCentro		= GetValueUnitChange(m_dWeightCentro[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightNanganBase	= GetValueUnitChange(m_dWeightNanganBase[bDW? 1:0]	,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightSoundBase		= GetValueUnitChange(m_dWeightSoundBase[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightRetainingWall	= GetValueUnitChange(m_dWeightRetainingWall[bDW? 1:0]	,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightBoCha			= GetValueUnitChange(m_dWeightBoCha[bDW? 1:0]			,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dUpperPBangHo		= GetValueUnitChange(m_dUpperPBangHo[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dUpperPCentro		= GetValueUnitChange(m_dUpperPCentro[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dUpperPBoCha			= GetValueUnitChange(m_dUpperPBoCha[bDW? 1:0]			,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dUpperPRetainingWall	= GetValueUnitChange(m_dUpperPRetainingWall[bDW? 1:0]	,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightNangan		= GetValueUnitChange(m_dWeightNangan[bDW? 1:0]		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightSound			= GetValueUnitChange(m_dWeightSound[bDW? 1:0]			,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dRail_WeightLane		= GetValueUnitChange(m_dRail_WeightLane		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightRailLane		= bDW? 0 : GetValueUnitChange(m_dWeightRailLane		,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightRailDosang	= bDW? 0 : GetValueUnitChange(m_dWeightRailDosang	,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightTotal			= GetValueUnitChange(m_DeadLoadDistribution[bDW? 1:0] + (bIncludeSoilPressureVert? m_dSoilPressureVert : 0),UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	
	long nColFormula = bLsd? 12 : 10;
	long nColResult	= bLsd? 3 : 2;
	long nDecResult = bLsd? 6 : 3;
	CString strArea = _T("");
	// 2006.10.18 �������߿��� ������ �Է°�����
	// double dHgt = toM(m_dUnderGroundDesignHeight);
	XLText->GoPosition(NO_MOVE,COL_POS_2);

	if(bLsd == FALSE || bDW == TRUE)
	{
		// �Ѱ�����̰� ���߶���� ���� �������߸� �����ϰ� ���� �����������.
		WriteCalcPaveAndUpperSoil(XLText, pBri, COL_POS_2, nIndexTiny, TRUE);
	}
	
	if(Compare(dWeightBangHo,0.0,">"))
	{
		// ���ٰ� ö���� �ִµ� �������... 
		double dWtot = 0;
		long n=0; for(n=0; n<m_dArrAreaBangHo[bDW? 1 : 0].GetSize(); n++)
		{
			dWtot += m_dArrAreaBangHo[bDW? 1 : 0].GetAt(n);
		}
		double dUW = GetValueUnitChange(m_dWeightBangHo[bDW? 1 : 0]/dWtot, UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
		strArea = GetStringAreaArray(m_dArrAreaBangHo[bDW? 1 : 0]);
		if(Compare(dUpperPBangHo,0.0,">"))
		{
			XLText->AddFormatText("$n$h%s$r��ȣ��$m+03$c:$r(%s x %s + %s) / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(dUW, UNIT_CGS_TONF_M3),  pData->GetStrDouble(dUpperPBangHo, UNIT_CGS_TONF),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble((dWeightBangHo+dUpperPBangHo)/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$n$h%s$r��ȣ��$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(dUW, UNIT_CGS_TONF_M3),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightBangHo/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
	}
	else if(bDW && Compare(dUpperPBangHo,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r��ȣ��$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dUpperPBangHo, UNIT_CGS_TONF),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dUpperPBangHo/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightCentro,0.0,">"))
	{
		strArea = GetStringAreaArray(m_dArrAreaCentro[bDW? 1 : 0]);
		if(Compare(dUpperPCentro,0.0,">")) 
		{
			XLText->AddFormatText("$n$h%s$r�ߺд�$m+03$c:$r(%s x %s + %s) / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dUpperPCentro, UNIT_CGS_TONF),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble((dWeightCentro+dUpperPCentro)/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$n$h%s$r�ߺд�$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(m_bGWhalf? UW_ConcOnly:UW_Conc, UNIT_CGS_TONF_M3),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightCentro/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
	}
	else if(bDW && Compare(dUpperPCentro,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r�ߺд�$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dUpperPCentro, UNIT_CGS_TONF),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dUpperPCentro/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightNanganBase,0.0,">"))
	{
		strArea = GetStringAreaArray(m_dArrAreaNanganBase[bDW? 1 : 0]);
		XLText->AddFormatText("$n$h%s$r��������$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightNanganBase/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}
	if(Compare(dWeightSoundBase,0.0,">"))
	{
		strArea = GetStringAreaArray(m_dArrAreaSoundBase[bDW? 1 : 0]);
		XLText->AddFormatText("$n$h%s$r����������$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3), 
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightSoundBase/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightRetainingWall,0.0,">"))
	{
		strArea = GetStringAreaArray(m_dArrAreaRetainingWall[bDW? 1 : 0]);
		if(Compare(dUpperPRetainingWall,0.0,">")) 
		{
			XLText->AddFormatText("$n$h%s$r�븷�̺�$m+03$c:$r(%s x %s + %s) / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dUpperPRetainingWall, UNIT_CGS_TONF),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble((dWeightRetainingWall+dUpperPRetainingWall)/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$n$h%s$r�븷�̺�$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3), 
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightRetainingWall/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
	}
	else if(bDW && Compare(dUpperPRetainingWall,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r�븷�̺�$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dUpperPRetainingWall, UNIT_CGS_TONF),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dUpperPRetainingWall/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightFootway,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r�� ��$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dWeightFootway, UNIT_CGS_TONF_M3), 
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightFootway/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightBoCha,0.0,">"))
	{
		strArea = GetStringAreaArray(m_dArrAreaBoCha[bDW? 1 : 0]);
		if(Compare(dUpperPBoCha,0.0,">")) 
		{
			XLText->AddFormatText("$n$h%s$r��輮$m+03$c:$r(%s x %s + %s) / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dUpperPBoCha, UNIT_CGS_TONF), 
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble((dWeightBoCha+dUpperPBoCha)/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
		else
		{
			XLText->AddFormatText("$n$h%s$r��輮$m+03$c:$r%s x %s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
				GetNumberString(nIndexTiny++,NUM_CIRCLE), strArea, pData->GetStrDouble(UW_Conc, UNIT_CGS_TONF_M3),
				pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightBoCha/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
		}
	}
	else if(bDW && Compare(dUpperPBoCha,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r��輮$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dUpperPBoCha, UNIT_CGS_TONF),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dUpperPBoCha/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if(Compare(dWeightNangan,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r����$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dWeightNangan, UNIT_CGS_TONF_M), 
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightNangan/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}
	if(Compare(dWeightSound,0.0,">"))
	{
		XLText->AddFormatText("$n$h%s$r������$m+03$c:$r%s / %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
			GetNumberString(nIndexTiny++,NUM_CIRCLE),  pData->GetStrDouble(dWeightSound, UNIT_CGS_TONF_M3),
			pData->GetStrDouble(dWidthSlab, UNIT_CGS_M), nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightSound/dWidthSlab, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}
	if(Compare(dWeightRailLane,0.0,">"))
	{
		if(m_bSingleLane)
			XLText->AddFormatText("$n$h%s$r�˵�$m+03$c:$r%.3f / %.3f$m+10$c=$r$me1$f3$i%.3f$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),dRail_WeightLane,m_dRail_WidthDistribute,dWeightRailLane,pData->m_szTonf_M2);
		else
			XLText->AddFormatText("$n$h%s$r�˵�$m+03$c:$r%.3f x 2 / (%.3f + %.3f)$m+10$c=$r$me1$f3$i%.3f$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),dRail_WeightLane,m_dRail_WidthDistribute,toM(m_dRail_DistBetweenRail),dWeightRailLane,pData->m_szTonf_M2);
	}
	if(Compare(dWeightRailDosang,0.0,">"))
	{
		double dHeight = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail);
		if(pBri->m_dDisBaseElevationRL == 0) dHeight = toM(pBri->GetHeightDosangInput());
		
		if(m_dRail_DosangWidthL == m_dRail_DosangWidthU)
		{
			if(m_dRail_DosangWidthL == 0)
				XLText->AddFormatText("$n$h%s$r����$m+03$c:$r%.3f x (%.3f + %.3f) x %.2f(����) / %.3f$m+10$c=$r$me1$f3$i%.3f$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),UW_Dosang,toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),RAIL_DOSANG_ADDRATE,dWidthSlab,dWeightRailDosang/dWidthSlab,pData->m_szTonf_M2);
			else
				XLText->AddFormatText("$n$h%s$r����$m+03$c:$r%.3f x (%.3f x %.3f) x %.2f(����) / %.3f$m+10$c=$r$me1$f3$i%.3f$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),UW_Dosang,m_dRail_DosangWidthU,dHeight,RAIL_DOSANG_ADDRATE,dWidthSlab,dWeightRailDosang/dWidthSlab,pData->m_szTonf_M2);
		}
		else
			XLText->AddFormatText("$n$h%s$r����$m+03$c:$r%.3f x (%.3f + %.3f) / 2 x %.3f x %.2f(����) / %.3f$m+10$c=$r$me1$f3$i%.3f$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),UW_Dosang,m_dRail_DosangWidthL,m_dRail_DosangWidthU,dHeight,RAIL_DOSANG_ADDRATE,dWidthSlab,dWeightRailDosang/dWidthSlab,pData->m_szTonf_M2);
	}

	XLText->AddFormatText("$h$-%d", bLsd? 23 : 19);
	XLText->AddFormatText("$n$h$r�� ��$m+03$c:$r$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
		nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightTotal, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
}

void CARcBridgeCalcLoad::WriteCalcLiveLoadUnderGround(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	if(!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen) return;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	long nDesignSelect = pBri->m_nUnderGroundDesignSelect; // ���Ȱ���� �������
	long nDesignType   = pBri->m_nUnderGroundDesignType;   // ���Ȱ���� ����

	double Pr  = GetValueUnitChange(m_pStd->GetWheelLoadRear(),UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double Pf  = GetValueUnitChange(m_pStd->GetWheelLoadFront(),UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double Pvl = GetValueUnitChange(m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double Phl = GetValueUnitChange(m_dLiveLoaUR_Phl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	
 	const HGBOOL bLsd = pData->IsLsdDesign();

	long nIndexSmall = 1;
//	long nIndexTiny  = 1;
	CString str = _T("");

	CString szDesignStanard(_T("�����α� ������ء�4.8.3.2"));
	if(nDesignType == 1)
	{
		if(pData->IsRailDesignKDS2018())
			szDesignStanard = _T("��KDS 24 14 20��4.9.3");
		else if( pData->IsRailDesign2017())
			szDesignStanard = _T("��ö��������ء�10.11.3");
	}

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+12$n$h$a%d.%d Ȱ �� ��$n",nIndexTitle,nIndexMiddle);
/////////////////////////////////////
	XLText->AddFormatText("$n$h %d)$r�� �� �� ��$m+04$c:$r$me1%s%s$r$r%s , %s$n",
		nIndexSmall++, bLsd ? "" : "$f3", pData->GetStrDoubleUnitChange(m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM), pData->m_szM, szDesignStanard);
	if(m_pStd->IsSkew())
	{
		WriteDesignSpanLengthSkew(XLText, pBri);
	}
/////////////////////////////////////
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	switch(nDesignType)
	{
		// ���λ�������
		case 0 :
			// ��������ö����
			if(nDesignSelect==0)
			{
				char  *TH[12] = {"1.0","1.5","2.0","2.5","3.0","4.0","5.0","6.0","7.0","8.0","9.0","10.0"};
				double LV[12] = {5.1,3.9,2.1,1.7,1.5,1.5,1.5,1.5,1.5,1.2,1.1,0.9};

				XLText->AddFormatText("$n$h %d)$r�������� ���� : ���� %s %s$m+19$i����$me1$m+02$c:$r$e%s$n",
					nIndexSmall++, pData->GetStrDoubleUnitChange(m_dLiveLoaUR_D, UNIT_CGS_M, pData->m_szM), pData->m_szM, pData->m_szTonf_M2);
				XLText->GoPosition(NO_MOVE, 0);
				XLText->AddTable(2, 25, 1, 2);
				long i=0; for(i=0; i<2; i++)
				{
					long j=0; for(j=0; j<13; j++)
					{
						if(i==0 && j==0) str = _T("����(m)");
						else if(i==1 && j ==0) str.Format("�� ��");
						else if(i==0) str = TH[j-1];
						else if(i==1) str.Format("%s%s", bLsd ? "" : "$f1",pData->GetStrDoubleUnitChange(LV[j-1], UNIT_CGS_TONF_M2, pData->m_szTonf_M2, FALSE, CELL_DEC_1));
						else str = _T("");

						XLText->AddMergeFormatText(i,2-1,"$c%s",str);
						XLText->AddFormatText("$m+0%d",2);
					}
					*XLText += i+1;
					XLText->AddFormatText("$h");
				}

				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$n$h %d)$r���� ���� �� ���� ����$n",nIndexSmall++);
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$h$cPvl$me1$m+02$c=$r$e%s %s$n", pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
				if(pBri->m_nSelectLiveLoadHori==1)
					XLText->AddFormatText("$h$cPhl$me1$m+02$c=$r$e%.3f x %s$me3$m+04$c=$r$e%s %s$n", 
					m_dLiveLoaUR_Ko, pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);
			}
			// ö�����������(�����)
			else if(nDesignSelect==1)
			{
				char  *TH[12] = {"1.0","1.5","2.0","2.5","3.0","4.0","5.0","6.0","7.0","8.0","9.0","10.0"};
				double LV[12] = {5.1,3.9,2.1,1.7,1.5,1.5,1.5,1.5,1.5,1.2,1.1,1.0};

				XLText->AddFormatText("$n$h %d)$r�������� ���� : ���� %s %s$n",nIndexSmall++, pData->GetStrDoubleUnitChange(m_dLiveLoaUR_D, UNIT_CGS_M, pData->m_szM), pData->m_szM);
				XLText->GoPosition(NO_MOVE, 0);
				XLText->AddTable(2, 25, 1, 2);
				long i=0; for(i=0; i<2; i++)
				{
					long j=0; for(j=0; j<13; j++)
					{
						if(i==0 && j==0) str = _T("����(m)");
						else if(i==1 && j ==0) str.Format("$tx08����\n(%s)", pData->m_szTonf_M2);
						else if(i==0) str = TH[j-1];
						else if(i==1) str.Format("%s%s", bLsd ? "" : "$f1",pData->GetStrDoubleUnitChange(LV[j-1], UNIT_CGS_TONF_M2, pData->m_szTonf_M2, FALSE, CELL_DEC_1));
						else str = _T("");
						
						XLText->AddMergeFormatText(i,2-1,"$c%s",str);
						XLText->AddFormatText("$m+0%d",2);
					}
					*XLText += i+1;
					XLText->AddFormatText("$h");
				}
		
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$n$h %d)$r���� ���� �� ���� ����$n",nIndexSmall++);
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$h$cPvl$me1$m+02$c=$r$e%s %s$n", pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
				if(pBri->m_nSelectLiveLoadHori==1)
					XLText->AddFormatText("$h$cPhl$me1$m+02$c=$r$e%.3f x %s$me3$m+04$c=$r$e%s %s$n",
					m_dLiveLoaUR_Ko, pData->GetStrDoubleUnitChange(pBri->m_dUnderGroundDesignLiveAlways, UNIT_CGS_TONF_M2, pData->m_szTonf_M2),
					pData->GetStrDoubleUnitChange(m_dLiveLoaUR_Phl, UNIT_CGS_TONF_M2, pData->m_szTonf_M2, TRUE), pData->m_szTonf_M2);
			} 
			// ���μ�����(��) : 2��, 3.3.2 606������
			else if(nDesignSelect==2)
			{
				double P   = GetValueUnitChange(m_dLiveLoaUR_P,UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double i   = m_dLiveLoaUR_i;
				double Bo  = m_dLiveLoaUR_Bo;
				double D   = m_dLiveLoaUR_D;
				// (��) ���ǰ� 4m �̸��� ���
				if(m_dLiveLoaUR_D<4.0)
				{
					XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���ǵβ��� %s%s �̸��� ��� , �����μ����ɡ�3.2.2,(2),(��)$n",
						pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$r���� ��鿡 �ۿ��ϴ� Ȱ���߿� ���� ���������� ���� ������ ����Ѵ�.$n");
					XLText->AddFormatText("$h$m+04$p[PVL_LIVE_1]$n$n");
					XLText->AddFormatText("$h$r���⼭, Pvl : ������鿡 �ۿ��ϴ� Ȱ���߿� ���� �������� (%s)$n",pData->m_szTonf_M2);
					XLText->AddFormatText("$h$r        W1  : Ȱ������ ������ (W1 = 2D + %s) (%s)$n", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					XLText->AddFormatText("$h$r        D   : ���ǵβ� (%s)$n", pData->m_szM);
					XLText->AddFormatText("$h$r        i   : ��ݰ��$n");
					XLText->AddFormatText("$h$r        P   : ������ �������̴� ���� (P1 = 2 x T1/3 = %.3f %s)$n",P,pData->m_szTonf_M);
					XLText->AddFormatText("$h$r        T1  : DB���� ���Ͻ��� �޹��� ���� (T1 = %.3f %s)$n",Pr,pData->m_szTonf_M);
					XLText->AddFormatText("$h$r        B   : ������ (��ü �β� ����) , (B = %s %s)$n", pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					XLText->AddFormatText("$s+04");
					MakeTableImpactCoeffience(XLText,TRUE);
					*XLText += 1;
					XLText->AddFormatText("$s+10");
					XLText->AddFormatText("$h$m+04$p[PVL_LIVE_1_PIC]");
					*XLText += 9;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W1)
					{
						XLText->AddFormatText("$h$rB��W1�� ��쿡�� �Ʒ� �׸��� Pvl�� ����(W1)�� �ۿ����Ѵٰ� �� ���¿�$n");
						XLText->AddFormatText("$h$r%s + 2D�� ���� ���ϵǴ� ����� ���Ʈ�� ���ٰ� ���� Pvl�� ���Ͽ�,$n", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$r�� ���� ����� �������� �� �ܸ���� ���Ѵ�.$n");
						XLText->AddFormatText("$h$r        $p[PVL_LIVE_2]$n$n");
						XLText->AddFormatText("$h$rPvl$r$c=$r2 x %.3f x (1 + %.3f) x  (%s - %s - %s) / %s��= %s %s$n",
							P,i,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM), 
							pData->GetStrDoubleUnitChange(0.1, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), 
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					else
					{
						XLText->AddFormatText("$h$rPvl$r$c=$r%.3f x (1 + %.3f) / (%s + %s) = %s %s$n",
							P,i, pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), 
							pData->GetStrDoubleUnitChange(2*D, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					if(pBri->m_nSelectLiveLoadHori==1)
						XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
						m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���ǵβ��� %s%s �̻��� ��� , �����μ����ɡ�3.2.2,(2),(��)$n", 
						pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$r���ǵβ� %s%s �̻��� ����� ���� ���� �� ���� ���������� �Ʒ� �׸���$n", pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$rǥ���ϴ� ������ ������ �����ϸ� �ȴ�.$n");
					XLText->AddFormatText("$h$r        $p[PVL_LIVE_2_PIC]");
					*XLText += 9;
					XLText->AddFormatText("$h$rPvl$r$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
					if(pBri->m_nSelectLiveLoadHori==1)
						XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
						m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
				}
			}
			// ���μ������(��) : 2�� 408-30
			else if(nDesignSelect==3)
			{
				double P   = GetValueUnitChange(m_dLiveLoaUR_P,UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double i   = m_dLiveLoaUR_i;
				double B   = m_dLiveLoaUR_B;
				double Bo  = m_dLiveLoaUR_Bo;
				double H   = m_dLiveLoaUR_D;
				// (��) ���ǰ� 4m �̸��� ���
				if(m_dLiveLoaUR_D<4.0)
				{
					XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���ǰ� %s �� H �� %s%s �� ��� , �����μ��������408-29$n",
						pData->GetStrDoubleUnitChange(1, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
					XLText->AddFormatText("$h$r���� ��鿡 �ۿ��ϴ� Ȱ���߿� ���� ���������� ���� ������ ����Ѵ�.$n");
					XLText->AddFormatText("$h$m+04$p[PVL_LIVE_3]$n$n");
					XLText->AddFormatText("$h$r���⼭, Pvl : ������鿡 �ۿ��ϴ� Ȱ���߿� ���� �������� (%s)$n",pData->m_szTonf_M2);
					XLText->AddFormatText("$h$r        W1  : Ȱ������ ������ ������ (W1 = 2H + %s) (%s)$n", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
					XLText->AddFormatText("$h$r        B   : ������ ���� ��, (B = %s %s)$n", pData->GetStrDoubleUnitChange(B, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					XLText->AddFormatText("$h$r        H   : ���ǵβ� (%s)$n", pData->m_szM);
					XLText->AddFormatText("$h$r        i   : ��ݰ��$n");
					XLText->AddFormatText("$h$r        Pr  : DB���� ���Ͻ��� �޹��� ���� (Pr = %.3f %s)$n",Pr,pData->m_szTonf_M);
					XLText->AddFormatText("$h$r        Bo  : ������ (��ü �β� ����) , (Bo = %s %s)$n",pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					XLText->AddFormatText("$s+04");
					MakeTableImpactCoeffience(XLText,TRUE);
					*XLText += 1;
					XLText->AddFormatText("$s+10");
					XLText->AddFormatText("$h$m+04$p[PVL_LIVE_1_PIC]");
					*XLText += 9;
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W1)
					{
						XLText->AddFormatText("$h$rBo��W1�� ��쿡�� �Ʒ� �׸��� Pvl�� ����(W1)�� �ۿ����Ѵٰ� �� ���¿�$n");
						XLText->AddFormatText("$h$r%s + 2H�� ���� ���ϵǴ� ����� ���Ʈ�� ���ٰ� ���� Pvl�� ���Ͽ�,$n", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$r�� ���� ����� �������� �� �ܸ���� ���Ѵ�.$n");
						XLText->AddFormatText("$h$r        $p[PVL_LIVE_4]$n$n");
						XLText->AddFormatText("$h$rPvl$r$c=$r2 x %.3f x (1 + %.3f) x  (%s - %s - %s) / %s��= %s %s$n",
							P,i,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(H, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(0.1, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1),
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					else
					{
						XLText->AddFormatText("$h$rPvl$r$c=$r%.3f x (1 + %.3f) / 1.5 x (%s + %s) = %s %s$n",
							Pr,i, pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->GetStrDoubleUnitChange(2*H, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					if(pBri->m_nSelectLiveLoadHori==1)
						XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
						m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2, TRUE),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���ǵβ��� %s%s �̻��� ��� , �����μ����ɡ�3.2.2,(2)$n", pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$r���ǵβ� %s%s �̻��� ����� ���� ���� �� ���� ���������� �Ʒ� �׸���$n", pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$rǥ���ϴ� ������ ������ �����ϸ� �ȴ�.$n");
					XLText->AddFormatText("$h$r        $p[PVL_LIVE_2_PIC]");
					*XLText += 9;
					XLText->AddFormatText("$h$rPvl$r$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
					if(pBri->m_nSelectLiveLoadHori==1)
						XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
						m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
				}
			}
			// �����������(��)
			else if(nDesignSelect==4)
			{
				CString strLive = pData->GetStringLoadDB();
				
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$n$h %d)$r�������� ���� ,������������ء��� 53-12$n",nIndexSmall++);
				long nIndexTiny  = 1;
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$n$h%s$r��ݰ��$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				MakeTableImpactCoeffience(XLText,TRUE);
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$n$h%s$r���Ȱ���� (Pvl)",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				XLText->AddFormatText("$n$h$r���Ȱ������ ����� �ܼ����� �����Ͽ� Ȱ������ ��������� �ۿ���");
				XLText->AddFormatText("$n$h$r���� 2D+%s%s�� �������� ���ϵǴ� ���� ���Ʈ�� ���ٰ� ����", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
				XLText->AddFormatText("$n$h$r������ Ȱ������ ������ ����.");
				XLText->AddFormatText("$n$h%s$rȰ���� ���� ����",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				XLText->AddFormatText("$n$h$rȰ���� �������� ��� ������ �ʰ��ϴ� �κ��� Ȱ���� ������ ����");
				XLText->AddFormatText("$n$h%s$r�������� ���� ����",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				XLText->AddFormatText("$n$h$r���������� ������ �����ϰ� �ķ����߿� ���ؼ��� ���");

				double P1 = GetValueUnitChange(m_dLiveLoaUR_P,UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double P2 = GetValueUnitChange(2.0*m_pStd->GetWheelLoadFront()/m_dLiveLoaUR_B,UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double i  = m_dLiveLoaUR_i;
				double Bo = m_dLiveLoaUR_Bo;
				double D  = m_dLiveLoaUR_D;
				double Pvl1 = GetValueUnitChange(m_dLiveLoaUR_Pvl1, UNIT_CGS_TONF_M, pData->m_szTonf_M);
				double Pvl2 = GetValueUnitChange(m_dLiveLoaUR_Pvl2, UNIT_CGS_TONF_M, pData->m_szTonf_M);
				if(m_dLiveLoaUR_D<=2.0 && m_dLiveLoaUR_D>=0.6)
				{
					XLText->GoPosition(NO_MOVE,COL_POS_1);
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W)
					{
						XLText->AddFormatText("$s+20$n$h %d)$r���� ���� �� ���� ���� : �ε���� ���� �ۿ�� (Bo��W) , ������������ء��� 53-13$n",nIndexSmall++);
						XLText->AddFormatText("$h$m+02$p[PVL_LIVE_21]");
						*XLText += 19;
						nIndexTiny  = 1;
						XLText->GoPosition(NO_MOVE,COL_POS_2);
						XLText->AddFormatText("$n$h%s$r������ �������̴� ���� (%s)",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive);
						XLText->AddFormatText("$n$h$rP1 = 2 x %.3f / %.1f = %.3f %s",Pr,m_dLiveLoaUR_B,P1,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h$rP2 = 2 x %.3f / %.1f = %.3f %s",Pf,m_dLiveLoaUR_B,P2,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h%s$r�������� �ۿ��ϴ� ���� (������ %s%s, ������ 45��)",
							GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
						XLText->AddFormatText("$n$h$rPvl1 = P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl2 = P2 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h%s$r����(Bo)�� �ۿ��ϴ� ���Ȱ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
						XLText->AddFormatText("$h$r$p[UR_EQA_1]");
						*XLText += 2;
						XLText->AddFormatText("$h$r$iPvl$r$c=$r8 x %.3f x (1+%.3f) x (0.75 x %s - 0.25 x %s - %s) / %s��$n",
							Pvl1,i,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(4.225, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
						if(pBri->m_nSelectLiveLoadHori==1)
							XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
							m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					else
					{
						XLText->AddFormatText("$s+20$n$h %d)$r���� ���� �� ���� ���� : �ε���� ���� �ۿ�� (Bo��W) , ������������ء��� 53-14$n",nIndexSmall++);
						XLText->AddFormatText("$h$m+02$p[PVL_LIVE_22]");
						*XLText += 19;
						nIndexTiny  = 1;
						XLText->GoPosition(NO_MOVE,COL_POS_2);
						XLText->AddFormatText("$n$h%s$r������ �������̴� ���� (%s)",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive);
						XLText->AddFormatText("$n$h$rP1 = 2 x %.3f / %.1f = %.3f %s",Pr,m_dLiveLoaUR_B,P1,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h$rP2 = 2 x %.3f / %.1f = %.3f %s",Pf,m_dLiveLoaUR_B,P2,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h%s$r�������� �ۿ��ϴ� ���� (������ %s%s, ������ 45��)",
							GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
						XLText->AddFormatText("$n$h$rPvl1 = P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl2 = P2 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h%s$r����(Bo)�� �ۿ��ϴ� ���Ȱ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
						XLText->AddFormatText("$h$r$p[UR_EQA_2]");
						*XLText += 2;
						XLText->AddFormatText("$h$r$iPvl$r$c=$r8 x %.3f x (0.125 x %s��- 2 x %s + %s + (%s-%s) x %s) / %s��$n",
							Pvl1,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(8.4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1),
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(4.2, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
						if(pBri->m_nSelectLiveLoadHori==1)
							XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
							m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
				}
				// (��) ����� ���� �ۿ�� : 0.60��D��2.0
				else if(m_dLiveLoaUR_D<4.0 && m_dLiveLoaUR_D>=2.0)
				{
					XLText->GoPosition(NO_MOVE,COL_POS_1);
					if(m_dLiveLoaUR_Bo>m_dLiveLoaUR_W)
					{
						XLText->AddFormatText("$s+20$n$h %d)$r���� ���� �� ���� ���� : ����� ���� �ۿ�� (Bo��W) , ������������ء��� 53-15$n",nIndexSmall++);
						XLText->AddFormatText("$h$m+02$p[PVL_LIVE_23]");
						*XLText += 19;
						nIndexTiny  = 1;
						XLText->GoPosition(NO_MOVE,COL_POS_2);
						XLText->AddFormatText("$n$h%s$r������ �������̴� ���� (%s)",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive);
						XLText->AddFormatText("$n$h$rP1 = 2 x %.3f / %.1f = %.3f %s",Pr,m_dLiveLoaUR_B,P1,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h$rP2 = 2 x %.3f / %.1f = %.3f %s",Pf,m_dLiveLoaUR_B,P2,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h%s$r�������� �ۿ��ϴ� ���� (������ %s%s, ������ 45��)",
							GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
						XLText->AddFormatText("$n$h$rPvl1 = P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl2 = 2 x P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl3 = (P1 + P2) x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl4 = P2 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h%s$r����(Bo)�� �ۿ��ϴ� ���Ȱ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
						XLText->AddFormatText("$h$r$p[UR_EQA_3]");
						*XLText += 2;
						XLText->AddFormatText("$h$r$iPvl$r$c=$r8 x %.3f x (4.15 x %s - %s - (0.5 x %s + %s) x %s - 0.5 x %s��) / %s��$n",
							Pvl1,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(17.645, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(0.1, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+03$e + 8 x %.3f x (%s - 2 x %s + (%s - %s) x %s) / %s��$n",
							Pvl2,pData->GetStrDoubleUnitChange(8.4, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(4.2, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
						if(pBri->m_nSelectLiveLoadHori==1)
							XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
							m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
					else
					{
						XLText->AddFormatText("$s+24$n$h %d)$r���� ���� �� ���� ���� : ����� ���� �ۿ�� (Bo��W) , ������������ء��� 53-16$n",nIndexSmall++);
						XLText->AddFormatText("$h$m+02$p[PVL_LIVE_24]");
						*XLText += 23;
						nIndexTiny  = 1;
						XLText->GoPosition(NO_MOVE,COL_POS_2);
						XLText->AddFormatText("$n$h%s$r������ �������̴� ���� (%s)",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive);
						XLText->AddFormatText("$n$h$rP1 = 2 x %.3f / %.1f = %.3f %s",Pr,m_dLiveLoaUR_B,P1,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h$rP2 = 2 x %.3f / %.1f = %.3f %s",Pf,m_dLiveLoaUR_B,P2,pData->m_szTonf_M);
						XLText->AddFormatText("$n$h%s$r�������� �ۿ��ϴ� ���� (������ 0.2m, ������ 45��)",
							GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1), pData->m_szM);
						XLText->AddFormatText("$n$h$rPvl1 = P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl2 = 2 x P1 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl3 = (P1 + P2) x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h$rPvl4 = P2 x (1 + i) / (2D + %s)", pData->GetStrDoubleUnitChange(0.2, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_1));
						XLText->AddFormatText("$n$h%s$r����(Bo)�� �ۿ��ϴ� ���Ȱ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
						XLText->AddFormatText("$h$r$p[UR_EQA_4]");
						*XLText += 2;
						XLText->AddFormatText("$h$r$iPvl$r$c=$r8 x %.3f x (0.125 x %s��+ 2 x %s - %s - (%s - %s) x %s) / %s��$n",
							Pvl1,pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(8.4, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(4.2, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+03$e + 8 x %.3f x (%s - 2 x %s + (%s - %s) x %s) / %s��$n",
							Pvl2,pData->GetStrDoubleUnitChange(8.4, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(4.2, UNIT_CGS_M, pData->m_szM),
							pData->GetStrDoubleUnitChange(D, UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(Bo, UNIT_CGS_M, pData->m_szM));
						XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
						if(pBri->m_nSelectLiveLoadHori==1)
							XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
							m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
					}
				}
				// (��) ���ǰ� 4m �̻��� ���
				else
				{
					XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���ǵβ��� %s%s �̻��� ��� , �����μ����ɡ�3.2.2,(2)$n", 
						pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$r���ǵβ� %s%s �̻��� ����� ���� ���� �� ���� ���������� �Ʒ� �׸���$n", 
						pData->GetStrDoubleUnitChange(4, UNIT_CGS_M, pData->m_szM, FALSE, CELL_DEC_0), pData->m_szM);
					XLText->AddFormatText("$h$rǥ���ϴ� ������ ������ �����ϸ� �ȴ�.$n");
					XLText->AddFormatText("$h$r        $p[PVL_LIVE_2_PIC]");
					*XLText += 9;
					XLText->AddFormatText("$h$rPvl$r$c=$r%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
					if(pBri->m_nSelectLiveLoadHori==1)
						XLText->AddFormatText("$h$rPhl$r$c=$r%.3f x %s = %s %s$n",
						m_dLiveLoaUR_Ko,pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
				}
			}
			break;
		// ö���������� : ö���������(�����) 2.2.3
		case 1 :
			{
				XLText->AddFormatText("$n$h %d)$r�������� ���� : ���� %s %s$m+19$i����$me1$m+02$c:$r$e%s$n",
					nIndexSmall++, pData->GetStrDoubleUnitChange(m_dLiveLoaUR_D, UNIT_CGS_M, pData->m_szM), pData->m_szM, pData->m_szTonf_M2);

				// if(pData->IsRailDesign2017())
				{
					// ��ݰ�� �����ٰ� �߰�.
					// 0 - 0.600, 5 - 0.480, 10 - 0.430, 20 - 0.370, 30 - 0.340, 40 - 0.320
					XLText->AddFormatText(_T("$h$r"));
					if(pData->IsRailDesign2017())
					{
						XLText->AddTable(1, 13, 1, 1);
						XLText->AddFormatText(_T("$h$r$c���� L(m)$me3$m+04$c0$me1$m+02$c5$me1$m+02$c10$me1$m+02$c20$me1$m+02$c30$me1$n"));
						XLText->AddFormatText(_T("$h$r$c��ݰ��(i)$me3$m+04$c0.60$me1$m+02$c0.48$me1$m+02$c0.43$me1$m+02$c0.37$me1$m+02$c0.34$me1$n"));
					}
					else
					{
						XLText->AddTable(1, 21, 1, 1);
						XLText->AddFormatText(_T("$h$r$c���� L(m)$me3$m+04$c0$me1$m+02$c5$me1$m+02$c10$me1$m+02$c20$me1$m+02$c30$me1$m+02$c40$me1$m+02$c50$me1$m+02$c70$me1$m+02$c100$me1$n"));
						XLText->AddFormatText(_T("$h$r$c��ݰ��(i)$me3$m+04$c0.60$me1$m+02$c0.48$me1$m+02$c0.43$me1$m+02$c0.37$me1$m+02$c0.34$me1$m+02$c0.32$me1$m+02$c0.30$me1$m+02$c0.27$me1$m+02$c0.24$me1$n"));
					}
					*XLText += 1;

					double L = m_dSpanLengthImpactApply;
					// (ARCBRIDGE-3311) He = ���ǰ� + �������
					double H = GetValueUnitChange(m_dLiveLoaUR_D + toM(pBri->m_dHeightDosangUnderTie), UNIT_CGS_M, pData->m_szM);
					double dI1(0), dI2(0), dLenGap(0);
					double dLenBase = GetCoefficientImpactFactor(L, dI1, dI2, dLenGap);
					
					if(L == dLenBase)
					{
						XLText->AddFormatText(_T("$h$ri0 = %.2f$n"), m_dCoefficientImpactSingle);
					}
					else
					{
						XLText->AddFormatText(_T("$h$ri0 = %.2f + (%.2f - %.2f) x %.3f / %.3f = %.2f$n"), dI2, dI1, dI2, L - dLenBase, dLenGap, m_dCoefficientImpactSingle);
					}

					if(H < 1.0)
					{
						XLText->AddFormatText(_T("$h$ri = %.2f$n"), m_dCoefficientImpact[0]);
					}
					else if(H > 2.5)
					{
						XLText->AddFormatText(_T("$h$ri = 0$m+04(H' = %.3f > 2.5 m)$n"), H);
					}
					else
					{
						XLText->AddFormatText(_T("$h$ri = i0 x (2.5 - H') / 1.5$m+09(H' = %.3f < 2.5 m)$n"), H);
						XLText->AddFormatText(_T("$h$ri = %.2f x (2.5 - %.3f) / 1.5$n"), m_dCoefficientImpactSingle, H);
						XLText->AddFormatText(_T("$h$ri = %.2f$n"), m_dCoefficientImpact[0]);
					}
				}

				XLText->AddFormatText("$n$h$rȰ���� ����� Ȱ������ ������������ �ϸ� �������߿� ���� �ڽ�������$n");
				XLText->AddFormatText("$h$r���е� �����Ѵ�.$n");
				*XLText += 1;

				if(pData->IsRailDesign2017() && pBri->m_nUnderGroundDistribute == 0)//�ɼ�1:���ϱ�����
				{
					double H2 = toM(pBri->m_dTS);
 					double dHe = (pBri->m_bUnderGroundRahmen)? toM(pBri->m_dUnderGroundDesignHeight) : 0;
 					double dPave = toM(pBri->m_dUnderGroundHeightMortar);
 					double dH = toM(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - m_dRail_HeightTie) - dPave;
 					if(pBri->m_dDisBaseElevationRL == 0) 
						dH = toM(pBri->GetHeightDosangInput());

					CString szHe(_T(""));
					if(dHe > 0)
					{
						szHe.Format(_T(" + 2 x %.3f"), dHe);
					}

					XLText->AddFormatText("$h$r- Ⱦ���� ���ߺ�����$n");
 					if(pData->m_nTypeCalcSlabEffWidth == 0)	// 10497
 						XLText->AddFormatText("$h$r%.3f + 1.5 x (%.3f + %.3f)%s + %.3f$m+14$c=$r$me1$i$f3%.3f$r$rm$n",toM(m_dRail_LengthTie), dH, dPave, szHe, H2, m_dRail_WidthDistribute);
 					else
 						XLText->AddFormatText("$h$r%.3f + 1.5 x (%.3f + %.3f)%s$m+14$c=$r$me1$i$f3%.3f$r$rm$n",toM(m_dRail_LengthTie), dH, dPave, szHe, m_dRail_WidthDistribute);

					XLText->AddFormatText("$h$r- ������ ���ߺ�����$n");
					double dWidthJong = toM(pBri->m_dRail_WidthTie) + 1.5 * (dH + dPave) + 2 *dHe + (pData->m_nTypeCalcSlabEffWidth == 0? H2 : 0);
					if(pData->m_nTypeCalcSlabEffWidth == 0)	// 10497
						XLText->AddFormatText("$h$r%.3f + 1.5 x (%.3f + %.3f)%s + %.3f$m+14$c=$r$me1$i$f3%.3f$r$rm �� %.3f(�ణ�Ÿ�)$n",toM(pBri->m_dRail_WidthTie), dH, dPave, szHe, H2, dWidthJong, toM(pBri->m_dRail_DistBetweenWheel));
					else
						XLText->AddFormatText("$h$r%.3f + 1.5 x (%.3f + %.3f)%s$m+14$c=$r$me1$i$f3%.3f$r$rm �� %.3f(�ణ�Ÿ�)$n",toM(pBri->m_dRail_WidthTie), dH, dPave, szHe, dWidthJong, toM(pBri->m_dRail_DistBetweenWheel));
					*XLText += 1;

					double W	= GetValueUnitChange(pData->m_DistributionLoadFront, UNIT_CGS_TONF_M, pData->m_szTonf_M);
					double P	= GetValueUnitChange(pData->m_RailLoadWheel, UNIT_CGS_TONF, pData->m_szTonf);

					double Pvl = GetValueUnitChange(m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
					double Phl = GetValueUnitChange(m_dLiveLoaUR_Phl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
					double i(0);
					if(pData->m_nTypeFootingSafetyApplyCoefficientImpact == 0)
						i = m_dCoefficientImpact[0];

					if(pData->IsKRL_2012())
					{
						XLText->AddFormatText("$h$rWl$r$c=$r%.3f / (%.3f x %.3f) + %.3f / %.3f$n", P, m_dRail_WidthDistributeJong, m_dRail_WidthDistribute, W, m_dRail_WidthDistribute);
					}
					else
					{
						XLText->AddFormatText("$h$rWl$r$c=$r%.3f / (%.3f x %.3f)$n", P, m_dRail_WidthDistributeJong, m_dRail_WidthDistribute);
					}
					XLText->AddFormatText("$h$r$r$c=$r'%s %s$n",pData->GetStrDouble(Pvl/(1+i), UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
				}
				else //���� �� ����ü�
				{
					double P  = GetValueUnitChange(pBri->m_pARcBridgeDataStd->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf);
					double He = m_dLiveLoaUR_D;
					double a  = toM(pBri->m_dRail_DistBetweenWheel);
					double b  = toM(pBri->m_dRail_LengthTie);
					double c  = toM(pBri->m_dRail_DistBetweenRail);
					double d  = toM(pBri->m_dHeightDosangUnderTie);

					CString szP = CommaC(P, 3);
					if(pData->IsRailDesign2017())
					{
						double W = GetValueUnitChange(pBri->m_pARcBridgeDataStd->m_DistributionLoadFront,UNIT_CGS_TONF,pData->m_szTonf);
						szP.Format(_T("(%s + %.0f x 3)"), CommaC(P, 3), W);
						P += W*3;
					}

					BOOL bWriteW = (pData->IsRailDesignKDS2018() && pBri->m_nUnderGroundDistribute == 1) ? TRUE : FALSE;
					if(He<0.5)
					{
						XLText->AddFormatText("$h$rWl = %s / [a x (b + 2d)] , He < %s%s �� ��$n", bWriteW ? _T("(P + aW)") : _T("P"), pData->GetStrDoubleUnitChange(0.5, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					}
					else
					{
						if(m_bSingleLane)
							XLText->AddFormatText("$h$rWl = %s / [a x (b + 2 x d)] , He �� %s%s �� ��$n", bWriteW ? _T("(P+aW)") : _T("P"),pData->GetStrDoubleUnitChange(0.5, UNIT_CGS_M, pData->m_szM), pData->m_szM);
						else
							XLText->AddFormatText("$h$rWl = 2%s / [a x (c + b + 2 x (d + He))] , He �� %s%s �� ��$n", bWriteW ? _T("(P+aW)") : _T("P"),pData->GetStrDoubleUnitChange(0.5, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					}
					XLText->AddFormatText("$h$r���⼭, He : F.L������ ���ǰ� (%s)$n", pData->m_szM);
					XLText->AddFormatText("$h$r        Wl : ������ο� �ۿ��ϴ� Ȱ���� (%s)$n",pData->m_szTonf_M2);
					XLText->AddFormatText("$h$r        P  : ����$n");
					if(bWriteW)
					{
						XLText->AddFormatText("$h$r        W  : ���ϴ��� ��������� ( = %.0f %s)$n", 80.0, pData->m_szTonf_M2);
					}
					XLText->AddFormatText("$h$r        a  : ������ �ణ�Ÿ� (%s)$n", pData->m_szM);
					XLText->AddFormatText("$h$r        b  : ħ����� (%s)$n", pData->m_szM);
					if(He>=0.5 && c>0)
					{
						XLText->AddFormatText("$h$r        c  : �������� �߽ɰ��� (%s)$n", pData->m_szM);
					}
					XLText->AddFormatText("$h$r        d  : ħ���ϸ��� ����β� (%s)$n", pData->m_szM);
					if(He<0.5)
					{
						XLText->AddFormatText("$h$rWl$r$c=$r%s / %.3f x (%.3f + 2 x %.3f)$n",szP,a,b,d);
						XLText->AddFormatText("$h$r$r$c=$r'%s %s$n",pData->GetStrDouble(Pvl/(1+m_dCoefficientImpact[0]), UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
					}
					else
					{
						if(m_bSingleLane)
							XLText->AddFormatText("$h$rWl$r$c=$r%s / [%.3f x (%.3f + 2 x %.3f)]$n",szP,a,b,d);
						else
							XLText->AddFormatText("$h$rWl$r$c=$r2 x %s / [%.3f x (%.3f + %.3f + 2 x (%.3f + %.3f))]$n",szP,a,c,b,d,He);
						XLText->AddFormatText("$h$r$r$c=$r'%s %s$n",pData->GetStrDouble(Pvl/(1+m_dCoefficientImpact[0]), UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
					}
				}
				
				if(pData->m_nTypeFootingSafetyApplyCoefficientImpact == 0)
				{
					XLText->AddFormatText("$h$rWli$r$c=$r%s x (1 + %.3f) : ��ݰ�� = %.3f$n",pData->GetStrDouble(Pvl/(1+m_dCoefficientImpact[0]), UNIT_CGS_TONF_M2),m_dCoefficientImpact[0],m_dCoefficientImpact[0]);
					XLText->AddFormatText("$h$r$r$c=$r'%s %s$n",pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
				}
				
				if(pBri->m_nSelectLiveLoadHori==1)
					XLText->AddFormatText("$h$rWh$r$c=$r%.3f x %s = %s %s$n",
					m_dLiveLoaUR_Ko, pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M);
				break;
			}
		// ������Է�
		case 2 :
			{
				XLText->AddFormatText("$n$h$r���� ���� �� ���� ���� : ���� %s %s$n",  pData->GetStrDoubleUnitChange(m_dLiveLoaUR_D, UNIT_CGS_M, pData->m_szM), pData->m_szM);
				*XLText += 1;
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$h$cPvl$me1$m+02$c=$r$e%s %s$n", pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
				if(pBri->m_nSelectLiveLoadHori==1)
					XLText->AddFormatText("$h$cPhl$me1$m+02$c=$r$e%.3f x %s$me3$m+04$c=$r$e%s %s$n",
					m_dLiveLoaUR_Ko, pData->GetStrDouble(Pvl, UNIT_CGS_TONF_M2), pData->GetStrDouble(Phl, UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);
			}
			break;
		default :
			break;
	}
	*XLText += 1;
}

void CARcBridgeCalcLoad::WriteCalcLiveLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen) return;

	long nIndexSmall = 1;

	CString str = _T("");
	CString strLive = "-24";
	if(pData->m_nDegreeBridge==1) strLive = "-18";
	if(pData->m_nDegreeBridge==2) strLive = "-13.5";
	long nCountLane  = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));
	double dPfDesign  = GetValueUnitChange(m_pStd->GetWheelLoadFront(),UNIT_CGS_TONF,pData->m_szTonf);
	double dPrDesign  = GetValueUnitChange(m_pStd->GetWheelLoadRear() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPm		  = GetValueUnitChange(m_pStd->GetWheelLoadPm() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPs		  = GetValueUnitChange(m_pStd->GetWheelLoadPs() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dDistribute= GetValueUnitChange(m_pStd->GetWheelLoadDistribution() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPmApply	  = GetValueUnitChange(m_dPmApply[0] ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPsApply	  = GetValueUnitChange(m_dPsApply[0] ,UNIT_CGS_TONF,pData->m_szTonf);
	double dDistApply = GetValueUnitChange(m_dDistributionApply ,UNIT_CGS_TONF,pData->m_szTonf);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d Ȱ �� ��$n",nIndexTitle,nIndexMiddle);
	//
	XLText->AddFormatText("$n$h %d)$r�� �� �� ��$m+04$c:$r$me1$f3%.3f$r$rm , �����α� ������ء�4.8.3.2$n",nIndexSmall++,m_dSpanLengthDesign);
	if(m_pStd->IsSkew())
	{
		WriteDesignSpanLengthSkew(XLText, pBri);
	}
	if(m_bExistLane)
	{
		long nIndexTiny  = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$h %d)$r�������� ����$m+04$c:$r$me1$f3%.3f$r$rm$n",nIndexSmall++,m_dWidthBetweenCurb);
		XLText->AddFormatText("$n$h %d)$r��ȿ�� ���� : �����α� ������ء�4.7.5.1,(2)$n",nIndexSmall++);
		XLText->AddFormatText("$h$rE$r$c=$r 1.2 + 0.06 x L = %.3f m (�� 2.100 m) , �� E = %.3f m$n",1.20+0.06*m_dSpanLengthDesign,m_dEffectiveWidth);
		//
		XLText->AddFormatText("$n$h %d)$r��ݰ�� ����$n",nIndexSmall++);
		if(pBri->m_bCheckUseri)
			XLText->AddFormatText("$h$ri$r$c=$r$me1%.3f$n$n",m_dCoefficientImpact[0]);
		else
			XLText->AddFormatText("$h$ri$r$c=$r 15 / (40 + L) = %.3f �� 0.300 , �� i = %.3f$n$n",15.0/(40+m_dSpanLengthDesign),m_dCoefficientImpact[0]);

		//////////////////////////////////////////////////////////////////////////
		XLText->AddFormatText("$s+08$h %d)$r�������� ����$n",nIndexSmall++);
		XLText->AddFormatText("$h$r�������� ����(Wc)�� %.3fm �̹Ƿ� %d������ �����Ѵ�.$n",m_dWidthBetweenCurb,nCountLane);

		char *Title1[] = { "Wc�� ����(m)","���μ�","Wc�� ����(m)","���μ�"};
		int nWidthJoint1[] = {8, 4, 8, 4};
		long nSizeRow = 6;
		long nSizeCol = 4;

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddTable(nSizeRow-1,24-1,1,0);
		long j=0; for(j=0; j<nSizeCol; j++)
		{
			XLText->AddMergeFormatText(0,nWidthJoint1[j]-1,"$c%s",Title1[j]);
			XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
		}
		long i=0; for(i=1; i<nSizeRow; i++)
		{
			for(j=0; j<nSizeCol; j++)
			{
				if(j==0)
				{
					if(i==1) str = " 6.0 �� Wc ��  9.1";
					if(i==2) str = " 9.1 �� Wc �� 12.8";
					if(i==3) str = "12.8 �� Wc �� 16.4";
					if(i==4) str = "16.4 �� Wc �� 20.1";
					if(i==5) str = "20.1 �� Wc �� 23.8";
				}
				if(j==1)
				{
					if(i==1) str = "2";
					if(i==2) str = "3";
					if(i==3) str = "4";
					if(i==4) str = "5";
					if(i==5) str = "6";
				}
				if(j==2)
				{
					if(i==1) str = "23.8 �� Wc �� 27.4";
					if(i==2) str = "27.4 �� Wc �� 31.1";
					if(i==3) str = "31.1 �� Wc �� 34.7";
					if(i==4) str = "34.7 �� Wc �� 38.4";
					if(i==5) str = "";
				}
				if(j==3)
				{
					if(i==1) str = "7";
					if(i==2) str = "8";
					if(i==3) str = "9";
					if(i==4) str = "10";
					if(i==5) str = "";
				}

				if(j==0)	XLText->AddText("$n$h");
				XLText->AddMergeFormatText(0,nWidthJoint1[j]-1,"$c%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
			}
		}
		if(pBri->m_nSelectDesignRoadWidth==0)
			XLText->AddFormatText("$n$h$e���������� W�� ǥ�� �������� 3 m���� �����Ѵ�.$n");
		else
		{
			XLText->AddFormatText("$n$h$e���������� W�� %.3f m���� �����Ѵ�.$n",m_dDesignRoadWidth);
			XLText->AddFormatText("$h�� W = Wc / N = %.3f / %d = %.3f �� 3.6 m$n",m_dWidthBetweenCurb,nCountLane,m_dWidthBetweenCurb/(double)(nCountLane));
		}

		//////////////////////////////////////////////////////////////////////////
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		double dRowFactor = 2.50;
		CDomyun *pDom = new CDomyun;
		DrawLiveLoadLaneCount(pDom,nCountLane);

		CDRect rt = pDom->GetExtRect();
		nSizeRow = long(RoundUp(MAX(3,m_pStd->GetCountLane(frM(m_dWidthBetweenCurb)))*dRowFactor,0)); // �ּ�3��������...

		if(nSizeRow * (rt.Width()/rt.Height()) > 30) nSizeRow = long(RoundDown(30*(rt.Height()/rt.Width()),0));

		str.Format("$s+%02d$n$h %d)$rȰ����(DB%s,DL%s) ����$n",nSizeRow+1,nIndexSmall++,strLive,strLive);
		XLText->AddFormatText("%s",str);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		//////////////////////////////////////////////////////////////////////////

		// DB����
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+20$h%s$rDB%s : %d�������Ͻ� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive,m_nCaseDesignLane[0]);
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		for(long nLane=1; nLane<=nCountLane; nLane++)
		{
			double ApplyFactor = 1.00;
			if(nLane==3) ApplyFactor = 0.90;
			if(nLane>=4) ApplyFactor = 0.75;
			double E  = m_dDesignTruckWidth*nLane + (m_dDesignRoadWidth-m_dDesignTruckWidth)*(nLane-1) + m_dEffectiveWidth;
			double Pr = (2*nLane*dPrDesign*(1+m_dCoefficientImpact[0]))/E;
			double Pf = (2*nLane*dPfDesign*(1+m_dCoefficientImpact[0]))/E;
			Pr *= ApplyFactor;
			Pf *= ApplyFactor;
			if(ApplyFactor<1.00)
			{
				XLText->AddFormatText("$h%s$r%d���� ���Ͻ� : %.0f%% ����$n",GetNumberString(nLane,NUM_GANA),nLane,ApplyFactor*100);
			}
			else
			{
				XLText->AddFormatText("$h%s$r%d���� ���Ͻ�$n",GetNumberString(nLane,NUM_GANA),nLane);
			}
			XLText->AddFormatText("$h$r$cEa$r$c=$r%.3f x %d + %.3f x %d + %.3f$m+11$c=$r$me1$f3$i%.3f$r$rm$n",m_dDesignTruckWidth,nLane,(m_dDesignRoadWidth-m_dDesignTruckWidth),nLane-1,m_dEffectiveWidth,E);
			if(ApplyFactor<1.00)
			{
				XLText->AddFormatText("$h$r$cPf$r$c=$r%d x %.3f x (1 + %.3f) / %.3f x %.2f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",2*nLane,dPfDesign,m_dCoefficientImpact[0],E,ApplyFactor,Pf,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPr$r$c=$r%d x %.3f x (1 + %.3f) / %.3f x %.2f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",2*nLane,dPrDesign,m_dCoefficientImpact[0],E,ApplyFactor,Pr,pData->m_szTonf);
			}
			else
			{
				XLText->AddFormatText("$h$r$cPf$r$c=$r%d x %.3f x (1 + %.3f) / %.3f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",2*nLane,dPfDesign,m_dCoefficientImpact[0],E,Pf,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPr$r$c=$r%d x %.3f x (1 + %.3f) / %.3f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",2*nLane,dPrDesign,m_dCoefficientImpact[0],E,Pr,pData->m_szTonf);
			}
		}

		// DL����
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+07$h%s$rDL%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),strLive);
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		long nIndexTiny2 = 1;
		XLText->AddFormatText("$h%s$r���Ʈ ����$n",GetNumberString(nIndexTiny2++,NUM_GANA));
		XLText->AddFormatText("$h$r$cPm$r$c=$r%.3f x (1 + %.3f) / 3$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",dPm,m_dCoefficientImpact[0],dPmApply,pData->m_szTonf);
		XLText->AddFormatText("$h%s$r���ܷ� ����$n",GetNumberString(nIndexTiny2++,NUM_GANA));
		XLText->AddFormatText("$h$r$cPs$r$c=$r%.3f x (1 + %.3f) / 3$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",dPs,m_dCoefficientImpact[0],dPsApply,pData->m_szTonf);
		XLText->AddFormatText("$h%s$r���������$n",GetNumberString(nIndexTiny2++,NUM_GANA));
		XLText->AddFormatText("$h$r$cw$r$c=$r %.3f x (1 + %.3f) / 3$m+11$c=$r$me1$f3$i%.3f$r$r%s$n",dDistribute,m_dCoefficientImpact[0],dDistApply,pData->m_szTonf_M);

		// �������� Ȱ����
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+%02d$h%s$r�������� Ȱ����$n", nCountLane+2, GetNumberString(nIndexTiny++,NUM_CIRCLE));

		char *Title[] = { "����", "E(m)", "Pr(tonf)", "Pf(tonf)", "w(tonf/m)", "Pm(tonf)", "Ps(tonf)", "���"};
		if(pData->IsSI())
		{
			Title[2] = "Pr(kN)";
			Title[3] = "Pf(kN)";
			Title[4] = "w(kN/m)";
			Title[5] = "Pm(kN)";
			Title[6] = "Ps(kN)";
		}
		int nWidthJoint[] = { 3, 3, 3, 3, 3, 3, 3, 2 };

		nSizeRow = nCountLane + 2;
		nSizeCol = 8;
		XLText->AddTable(nSizeRow-1,23-1,2,1);

		for(i=0; i<2; i++)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_3);
			long j=0; for(j=0; j<nSizeCol; j++)
			{
				str = (i==0) ? _T("") : Title[j];
				if(j==0 && i>0)	XLText->AddText("$n$h");
				XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
				if((i==1 && j==0) || (i==1 && j==1) || (i==1 && j==7))
				{
					XLText->AddFormatText("$u$md1$n");
				}
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
			if(i==0)
			{
				long nWidthCell = nWidthJoint[2]+nWidthJoint[3];
				XLText->GoPosition(NO_MOVE,COL_POS_3);
				XLText->AddFormatText("$m+0%d",nWidthJoint[0]+nWidthJoint[1]);
				XLText->AddMergeFormatText(0,nWidthCell-1,"$c%s","DB"+strLive);
				XLText->AddFormatText("$m+0%d",nWidthCell);
				nWidthCell = nWidthJoint[4]+nWidthJoint[5]+nWidthJoint[6];
				XLText->AddMergeFormatText(0,nWidthCell-1,"$c%s","DL"+strLive);
			}
		}
		for(i=2; i<nSizeRow; i++)
		{
			long nLane = i - 1;
			double ApplyFactor = 1.00;
			if(nLane==3) ApplyFactor = 0.90;
			if(nLane>=4) ApplyFactor = 0.75;
			double E  = m_dDesignTruckWidth*nLane + (m_dDesignRoadWidth-m_dDesignTruckWidth)*(nLane-1) + m_dEffectiveWidth;
			double Pr = (2*nLane*dPrDesign*(1+m_dCoefficientImpact[0]))/E;
			double Pf = (2*nLane*dPfDesign*(1+m_dCoefficientImpact[0]))/E;
			Pr *= ApplyFactor;
			Pf *= ApplyFactor;
			long j=0; for(j=0; j<nSizeCol; j++)
			{
				if(j==0) str.Format("$n$h$me%d$c%d����",nWidthJoint[j]-1,i-1);
				if(j==1) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,E);
				if(j==2) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,Pr);
				if(j==3) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,Pf);
				if(j==4) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,dDistApply);
				if(j==5) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,dPmApply);
				if(j==6) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,dPsApply);
				if(j==7) str.Format("$me%d$c%s",nWidthJoint[j]-1,nLane==m_nCaseDesignLane[0]?"����":_T(""));
				XLText->AddFormatText("%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
		}
		XLText->AddFormatText("$n$n");
	}

	if(m_dLiveLoadFootway>0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);

		str.Format("$s+05$h %d)$r���� Ȱ���� ����$n",nIndexSmall++);
		XLText->AddFormatText("%s",str);

		WriteCalcLiveLoadFootway(XLText, pBri);
	}
}

void CARcBridgeCalcLoad::WriteCalcLiveLoadTank(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGBOOL bLSD = pData->IsLsdDesign();

	if(pBri->IsApplyTankLoad() == FALSE)
		return;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	double P  = GetValueUnitChange(m_dTank_WeightTank/2/m_dTank_TankLength,UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf);
	double P1 = GetValueUnitChange(pData->m_dTank_Ptrailer1,UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf);
	double P2 = GetValueUnitChange(pData->m_dTank_Ptrailer2,UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf);
	double P3 = GetValueUnitChange(pData->m_dTank_Ptrailer3,UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf);

	// 2015-07-13 ������ : "���߻���"��ȹ���� ���� �۾� �Ϸ�. ��� �ɼ��� �۾��� ���� �ʾ� ���� ��Ȯ��, ��������������� �̻����.
	// todo : ��ũ ���� ���� �ɼ��� �Ǹ� �׽�Ʈ�� �غ��� ��.
	CString strWidth = bLSD==TRUE ? "$n$h %d)$r��ȿ�� ���� : [���α� ������� 4.6.2.4]$n" : "$n$h %d)$r��ȿ�� ���� : �����α� ������ء�4.7.5.1,(2)$n";
	
	double dE1 = GetValueUnitChange(1.20, UNIT_CGS_M, pData->m_szM);	
	double dE = GetValueUnitChange(1.20+0.06*m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM);
	double dTank_E = bLSD? frM(m_dTank_E) : m_dTank_E;
	double dTankLength = bLSD? frM(m_dTank_TankLength) : m_dTank_TankLength;
	double dTankWidth_Side = bLSD? frM(pData->m_dTank_TankWidth_Side) : pData->m_dTank_TankWidth_Side;
	double dTankWidth_Inner = bLSD? frM(pData->m_dTank_TankWidth_Inner) : pData->m_dTank_TankWidth_Inner;
	double dTankWidth = bLSD? frM(m_dTank_TankWidth) : m_dTank_TankWidth;
	double dTrailerWidth = bLSD? frM(m_dTank_TrailerWidth) : m_dTank_TrailerWidth;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+25$n$h$e$a%d.%d ��ũ �� ������ ����$n",nIndexTitle,nIndexMiddle);
	//
	XLText->AddFormatText(strWidth, nIndexSmall++);
	XLText->AddFormatText("$h$rE$r$c=$r %s + 0.06 x L = %s %s (�� %s %s) , �� E = %s %s$n", pData->GetStrDouble(dE1, UNIT_CGS_M), 
		pData->GetStrDouble(dE, UNIT_CGS_M), pData->m_szM, pData->GetStrDouble(bLSD? 2100 : 2.1, UNIT_CGS_M), pData->m_szM, pData->GetStrDouble(dTank_E, UNIT_CGS_M), pData->m_szM);
	//
	XLText->AddFormatText("$n$h %d)$r��ݰ�� i�� �Ϸ������� %.3f�� �����Ѵ�. (�� ���籸���� P12)$n",nIndexSmall++,m_dTank_i);
	//
	XLText->AddFormatText("$n$h %d)$r��ũ���߰� ��ũ Ʈ���Ϸ� ������ 1���� ���ϸ� ��Ģ���� �Ѵ�.$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$c-$r�� ������ �������� �޼��� ���Ͽ� �������� ����� �� �� �ֵ��� �����ؾ� �Ѵ�.$n");
	XLText->AddFormatText("$h$r���⼭ �������� ����� �������� 30m�� �����ϰ�, �ü� 40km�� �������� ���Ѵ�.$n");
	*XLText += 1;
	XLText->AddFormatText("$h$c-$r����޼� (60�޼�)$n");
	XLText->AddFormatText("$h$r�޼� ǥ�� ����� ���ձ����� ���������� ����Ͽ� NATO �޼� ǥ�� ����� ����ϴ°���$n");
	XLText->AddFormatText("$h$r���� �������� ��Ģ�̴�. ���� �̱��� ����ϴ� short ton�� ������ �츮�� ����ϴ� ������$n");
	XLText->AddFormatText("$h$rmetric ton���� ȯ���Ͽ� �����Ѵ�. (��ȣ���� P42)$n");
	//
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r��ũ ���� ����$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	// (35533) W -> P, P -> Wtk
	XLText->AddFormatText("$h%s$r�������� : P = %s %s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),
		pData->GetStrDoubleUnitChange(m_dTank_WeightTank,UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	XLText->GoPosition(NO_MOVE,0);
	XLText->AddFormatText("$h$p[TANK_60]$n");
	XLText->AddFormatText("$h$u$u$u");
	XLText->AddFormatText("$h$m+21L$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDouble(dTankLength, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDouble(dTankWidth_Side, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDouble(dTankWidth_Inner, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDouble(dTankWidth_Side, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDouble(dTankWidth, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
	*XLText += 6;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r���߻���$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
	XLText->AddFormatText("$h$r$cWtk$r$c=$r(P / 2) / %s$m+04$c=$r%s %s$n", pData->GetStrDouble(dTankLength, UNIT_CGS_M), pData->GetStrDouble(P, UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$r$cPtk$r$c=$rWtk x (1 + i) x 2EA / (W + E)$n");
	XLText->AddFormatText("$h$m+02$c=$r%s x (1 + %.3f) x 2EA / (%s + %s)$n",pData->GetStrDouble(P, UNIT_CGS_TONF), m_dTank_i, pData->GetStrDouble(dTankWidth, UNIT_CGS_M), pData->GetStrDouble(dTank_E, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s %s$n",pData->GetStrDoubleUnitChange(m_dTank_Ptank,UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	*XLText += 1;
	
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	nIndexTiny = 1;
	XLText->AddFormatText("$s+25$h %d)$r��ũ Ʈ���Ϸ� ���� ����$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r�������� : W = %s %s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDoubleUnitChange(m_dTank_WeightTrailer, UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	XLText->GoPosition(NO_MOVE,0);
	XLText->AddFormatText("$h$r$r$p[TRAILOR_60]$n");
	*XLText += 1;
	XLText->AddFormatText("$h$m+21L��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerL1, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21L��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerL2, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21L��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerL3, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21L��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerL4, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerWidth_Side, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerWidth_Inner, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W��$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(pData->m_dTank_TrailerWidth_Side, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);
	XLText->AddFormatText("$h$m+21W$r$c=$r$i%s$me1$m+02%s$n", pData->GetStrDoubleUnitChange(m_dTank_TrailerWidth, UNIT_CGS_M, pData->m_szM, FALSE, -1, TRUE), pData->m_szM);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r���߻���$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	XLText->AddFormatText("$h$r$cP1$r$c=$r%s x 1$m+04$c=$r%s %s$n", pData->GetStrDouble(P1, UNIT_CGS_TONF), pData->GetStrDouble(P1, UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$r$cP2$r$c=$r%s x 1$m+04$c=$r%s %s$n", pData->GetStrDouble(P2, UNIT_CGS_TONF), pData->GetStrDouble(P2, UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$r$cP3$r$c=$r%s x 1$m+04$c=$r%s %s$n", pData->GetStrDouble(P3, UNIT_CGS_TONF), pData->GetStrDouble(P3, UNIT_CGS_TONF), pData->m_szTonf);
	XLText->AddFormatText("$h$r$iPtr1$r$c=$rP1 x (1 + i) x 2EA / (%s + E)$n", pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s x (1 + %.3f) x 2EA / (%s + %s)$n",
		pData->GetStrDouble(P1, UNIT_CGS_TONF), m_dTank_i, pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M), pData->GetStrDouble(dTank_E, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s %s$n", pData->GetStrDoubleUnitChange(m_dTank_Ptrailer1, UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	XLText->AddFormatText("$h$r$iPtr2$r$c=$rP2 x (1 + i) x 2EA / (%s + E)$n", pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s x (1 + %.3f) x 2EA / (%s + %s)$n",
		pData->GetStrDouble(P2, UNIT_CGS_TONF), m_dTank_i, pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M), pData->GetStrDouble(dTank_E, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s %s$n", pData->GetStrDoubleUnitChange(m_dTank_Ptrailer2, UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	XLText->AddFormatText("$h$r$iPtr3$r$c=$rP3 x (1 + i) x 2EA / (%s + E)$n", pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s x (1 + %.3f) x 2EA / (%s + %s)$n",
		pData->GetStrDouble(P3, UNIT_CGS_TONF),m_dTank_i, pData->GetStrDouble(dTrailerWidth, UNIT_CGS_M), pData->GetStrDouble(dTank_E, UNIT_CGS_M));
	XLText->AddFormatText("$h$m+02$c=$r%s %s$n", pData->GetStrDoubleUnitChange(m_dTank_Ptrailer3, UNIT_CGS_TONF, pData->m_szTonf), pData->m_szTonf);
	XLText->AddFormatText("$n");
}

void CARcBridgeCalcLoad::WriteCalcRailLoadLive(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	if(!m_bRailDesign) return;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
//	if(pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen) return;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d Ȱ �� ��$n",nIndexTitle,nIndexMiddle);
	//
	//XLText->AddFormatText("$n$h %d)$r�� �� �� ��$m+04$c:$r$me1$f3%.3f$r$rm$n",nIndexSmall++,m_dSpanLengthDesign);
	CString szDesignCode = pData->IsRailDesign2017() ? _T("��ö��������ء�10.11.3") : _T("��ö���� ������ء�P376");
	XLText->AddFormatText("$n$h %d)$r�� �� �� ��$m+04$c:$r$me1$f3%.3f$r$rm , %s$n",nIndexSmall++,m_dSpanLengthDesign, szDesignCode);
	if(m_pStd->IsSkew())
	{
		WriteDesignSpanLengthSkew(XLText, pBri);
	}
	//
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r��ݰ�� ����$n",nIndexSmall++);
	double L  = m_dSpanLengthDesign;
	double dI = 0;
	if(pData->IsRailDesign2017())
	{
		double dI0 = 1.44 / (sqrt(L) - 0.2) - 0.18;
		dI = MIN(0.67, dI0);
		XLText->AddFormatText("$h$rIm$r$c=$r 1.44 / (��Lc - 0.2,) - 0.18 = %.3f �� 0.67, �� Im = %.3f$n",dI0,dI);
	}
	else
	{
		if(m_dSpanLengthDesign<=18.0)
		{
			dI = 45.0 - pow(L,2)/45.0;
			XLText->AddFormatText("$h$ri$r$c=$r 45 - L��/ 45 = %.3f %%, �� i = %.3f$n",dI,dI*0.01);
		}
		else
		{
			dI = 24.0 + 240.0/(L-0.6);
			XLText->AddFormatText("$h$ri$r$c=$r 24 + 240 / (L - 0.6) = %.3f %% , �� i = %.3f$n",dI,dI*0.01);
		}
	}

	if(pBri->m_bUnderGroundRahmen && toM(m_dUnderGroundDesignHeight)>1.0)
	{
		double Hc = toM(m_dUnderGroundDesignHeight);
		XLText->AddFormatText("$h$r'- ��������鿡 ���� 1m�̻� ������ �ִ� ��� : ö���� ������� (ö������), 1-2.4,(3)$n");
		XLText->AddFormatText("$h$r$ri$r$c=$r io x {1 - 0.1 x (Hc - 1.0)} = %.3f x {1 - 0.1 x (%.3f - 1.0)} = %.3f$n",dI,Hc,m_dCoefficientImpact[0]);
		XLText->AddFormatText("$h$m+04���⼭, i  : �������� ��� ���$n");
		XLText->AddFormatText("$h$m+04        io : �������� ���䰡 ���ٰ� ������ ���� ��ݰ��$n");
		XLText->AddFormatText("$h$m+04        Hc : ������ ��鿡�� ħ���ܰ����� ���� ���� (m)$n");
	}
	//
	double Pd = GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf);
	double dW = GetValueUnitChange(pData->m_DistributionLoadFront,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double B  = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);
	double I  = 0;
	double N  = (pData->m_nTypeRailQty==0) ? 1 : 2;
	XLText->AddFormatText("$n$h %d)$r�������� : %s$n",nIndexSmall++,pData->GetStringRailLoad(pData->m_nTypeRailLoad));
	XLText->GoPosition(NO_MOVE,COL_POS_2);

	CString szWidth = _T("");
	if(m_bSingleLane)
		szWidth.Format("%.3f",m_dRail_WidthDistribute);
	else
		szWidth.Format("(%.3f+%.3f)",m_dRail_WidthDistribute,toM(m_dRail_DistBetweenRail));

	CString szImpact(_T(""));
	if(pData->m_nTypeFootingSafetyApplyCoefficientImpact == 0)
	{
		I  = m_dCoefficientImpact[0];
		szImpact.Format(_T(" x %.3f"), 1+I);
	}

	if(pData->IsRailDesign2017())
	{
		double dFactor = (pData->m_nTypeRailLoad == RAIL_LOAD_KRL2012_P)? 0.75 : 1.0;
		XLText->AddFormatText("$h%s$rP $r$c=$r%.1f x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd * dFactor,N,szImpact,szWidth,Pd*dFactor*N*(1+I)/B,pData->m_szTonf);
		if(pData->IsKRL_2012())
		{
			XLText->AddFormatText("$h%s$rw $r$c=$r%.1f x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),dW * dFactor,N,szImpact,szWidth,dW*dFactor*N*(1+I)/B,pData->m_szTonf);
		}
	}
	else
	{
		if(pData->IsRailLoadLS())
		{
			XLText->AddFormatText("$h%s$rPd$r$c=$r%.1f x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd/2,N,szImpact,szWidth,Pd/2*N*(1+I)/B,pData->m_szTonf);
			XLText->AddFormatText("$h%s$rP $r$c=$r%.1f x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd,N,szImpact,szWidth,Pd*N*(1+I)/B,pData->m_szTonf);
			XLText->AddFormatText("$h%s$rP $r$c=$r%.1f/3 x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd*2,N,szImpact,szWidth,Pd*2.0/3.0*N*(1+I)/B,pData->m_szTonf);
			XLText->AddFormatText("$h%s$rw $r$c=$r%.1f/3 x %.0f%s / %s$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd,N,szImpact,szWidth,Pd/3.0*N*(1+I)/B,pData->m_szTonf);
		}
		if(pData->IsRailLoadEL())
		{
			XLText->AddFormatText("$h%s$rPd$r$c=$r%.1f x %.0f%s / %.3f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),Pd,N,szImpact,B,Pd*N*(1+I)/B,pData->m_szTonf);
		}
	}
}

void CARcBridgeCalcLoad::WriteCalcRailLoadLR(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;



	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+05$n$h$a%d.%d ��뷹�� ������$n",nIndexTitle,nIndexMiddle);
	if(pBri->m_bUnderGroundRahmen && toM(m_dUnderGroundDesignHeight)>1.0)
	{
		XLText->AddFormatText("$n$h$r: ���ǰ� 1.0���� �̻��̹Ƿ� ������� �ʴ´�.$n$n");	
	}
	else
	{
		double dQtyRail  = m_bSingleLane ? 1.0 : 2.0;
		double dWidthEff = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);
		double UForce    = GetValueUnitChange(m_dRail_UnitForceLR,UNIT_CGS_TONF_M,pData->m_szTonf_M);
		double HForce    = GetValueUnitChange(m_dRail_LoadLR,UNIT_CGS_TONF_M,pData->m_szTonf_M);
		double Moment    = GetValueUnitChange(m_dRail_MomentLR,UNIT_CGS_TONFM,pData->m_szTonfM);

		// ö���������� m_dTP �����ʴ´�. pBri->m_dUnderGroundHeightMortar ���.
		long nIndexSmall = 1;
		XLText->AddFormatText("$n$h %d)$r�ۿ�����$m+03$c:$r%.3f x %.0f / %.3f$m+12$c=$r$me1$f3$i%.3f$r$r%s$n",nIndexSmall++,UForce,dQtyRail,dWidthEff,HForce,pData->m_szTonf_M);
		if(pBri->m_bUnderGroundRahmen)
			XLText->AddFormatText("$n$h %d)$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f + %.3f/2$m+12$c=$r$me1$f3$i%.3f$r$rm$n",nIndexSmall++,toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(m_dUnderGroundDesignHeight),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS),m_dRail_HeightLR);
		else
			XLText->AddFormatText("$n$h %d)$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f/2$m+12$c=$r$me1$f3$i%.3f$r$rm$n",nIndexSmall++,toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS),m_dRail_HeightLR);
		XLText->AddFormatText("$n$h %d)$r�ۿ���Ʈ$m+03$c:$r%.3f x %.3f$m+12$c=$r$me1$f3$i%.3f$r$r%s$n$n",nIndexSmall++,HForce,m_dRail_HeightLR,Moment,pData->m_szTonfM);
	}
}


void CARcBridgeCalcLoad::WriteCalcRailLoadSB(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pData->IsRailDesign2017())
	{
		WriteCalcRailLoadSB_2017(XLText, pBri, nIndexTitle, nIndexMiddle);
		return;
	}

	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+20$h$a%d.%d �õ� �� ��������$n",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$h: �õ�����(��������)�� ���ϸ�� %.1fm���̿� ����������� �������� �ۿ��ϴ� ������ �Ѵ�.$n", m_dRail_HeightSB);
	XLText->AddFormatText("$h   �������� - LS������ 15%%, �õ����� - ���������� 25%%$n");
	XLText->AddFormatText("$h   ���⿡�� ����� ��� ������� �ʴ´�.$n");

	// �õ�����
	long   nQtyWheel	= m_pStd->GetQtyRailWheel();
	double dWidthEff	= m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);
	double WheelLoad	= GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf);
	double dLengthModel = toM(pBri->GetLengthModelSpan(-1));
	double LoadUnitSi   = GetValueUnitChange(m_dRail_LoadSidong,UNIT_CGS_TONF,pData->m_szTonf);
	double HForce       = GetValueUnitChange(m_dRail_TsbSidong,UNIT_CGS_TONF,pData->m_szTonf);

	XLText->AddFormatText("$n$h %d)$r�õ�����$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r�𵨸����̰� %.3fm �̹Ƿ� %d�� ���������� �����Ѵ�.$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),dLengthModel,nQtyWheel);
	XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	XLText->AddFormatText("$h$rS$r$c=$r%dEA x %.3f x 0.25$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",nQtyWheel,WheelLoad,HForce,pData->m_szTonf);
	XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
	XLText->AddFormatText("$h$rS$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",HForce,dWidthEff,dLengthModel,LoadUnitSi,pData->m_szTonf_M);
	//
	// ��������
	double dWheelWeight = pBri->m_pARcBridgeDataStd->GetRailLoadWeightWheel(m_nSttWheelZedong,dLengthModel);
	double dWheelLength = pBri->m_pARcBridgeDataStd->GetRailLoadLengthWheel(m_nSttWheelZedong);	// �������� ���ϵǴ� ����
	double dDistribute  = pBri->m_pARcBridgeDataStd->m_DistributionLoadBack;					// ��������
	long   nQtyDist		= pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetSize();
	long   nIdxDist		= (dDistribute==0) ? nQtyDist-1 : nQtyDist-2;
	double dDistDistribute = pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetAt(nIdxDist);	 // ������ �����߿��� �������߱����� �Ÿ�
	dWheelWeight = GetValueUnitChange(dWheelWeight,UNIT_CGS_TONF,pData->m_szTonf);
	dDistribute  = GetValueUnitChange(dDistribute ,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	HForce       = GetValueUnitChange(m_dRail_TsbZedong,UNIT_CGS_TONF,pData->m_szTonf);
	double LoadUnitZe    = GetValueUnitChange(m_dRail_LoadZedong,UNIT_CGS_TONF,pData->m_szTonf);

	nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r��������$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r��������(L = %.3fm)���� LS����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),dLengthModel);
	// �Ϲ����� ��� ���������� ����
	if(dLengthModel<=(dWheelLength+dDistDistribute))
	{
// 		if(HForce/0.15>=1000)
// 		{
// 			XLText->AddFormatText("$h$rTb$r$c=$r'%.3f$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",dWheelWeight,HForce/0.15,pData->m_szTonf);
// 		}
// 		else
// 		{
			XLText->AddFormatText("$h$rTb$r$c=$r'%.3f$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",dWheelWeight,HForce/0.15,pData->m_szTonf);
// 		}
	}
	else
	{
		XLText->AddFormatText("$h$rTb$r$c=$r%.3f + %.3f x (%.3f - %.3f - %.3f)$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",dWheelWeight,dDistribute,dLengthModel,dWheelLength,dDistDistribute,HForce/0.15,pData->m_szTonf);
	}
	XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	XLText->AddFormatText("$h$rS$r$c=$r%.3f x 0.15$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",HForce/0.15,HForce,pData->m_szTonf);
	XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
	XLText->AddFormatText("$h$rS$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",HForce,dWidthEff,dLengthModel,LoadUnitZe,pData->m_szTonf_M);

	//
	HForce = GetValueUnitChange(m_dRail_LoadSB,UNIT_CGS_TONF,pData->m_szTonf);
	double Moment = GetValueUnitChange(m_dRail_MomentSB,UNIT_CGS_TONFM,pData->m_szTonfM);
	nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r�ۿ����� ����$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(m_bSingleLane)
	{
		XLText->AddFormatText("$h: �ܼ��̹Ƿ� �õ����߰� ���������߿� ū ���� �����Ѵ�.$n");
		XLText->AddFormatText("$h%s$r�ۿ�����$m+03$c:$rMAX(%.3f , %.3f)$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),LoadUnitSi,LoadUnitZe,HForce,pData->m_szTonf);
	}
	else
	{
		XLText->AddFormatText("$h: �����̹Ƿ� �õ����� + ���������� �����Ѵ�.$n");
		XLText->AddFormatText("$h%s$r�ۿ�����$m+03$c:$r%.3f + %.3f$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),LoadUnitSi,LoadUnitZe,HForce,pData->m_szTonf);
	}
	if(pBri->m_bUnderGroundRahmen)
		XLText->AddFormatText("$h%s$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f + %.3f + %.2f$m+13$c=$r$me1$f3$i%.3f$r$rm$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(m_dUnderGroundDesignHeight),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS/2),m_dRail_HeightSB,m_dRail_HeightSidong);
	else
		XLText->AddFormatText("$h%s$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f + %.2f$m+13$c=$r$me1$f3$i%.3f$r$rm$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS/2),m_dRail_HeightSB,m_dRail_HeightSidong);
	XLText->AddFormatText("$h%s$r�ۿ���Ʈ$m+03$c:$r%.3f x %.3f$m+13$c=$r$me1$f3$i%.3f$r$r%s$n$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),HForce,m_dRail_HeightSidong,Moment,pData->m_szTonfM);
}

void CARcBridgeCalcLoad::WriteCalcRailLoadSB_2017( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle )
{
	if(!m_bRailDesign) return;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+20$h$a%d.%d �õ� �� ��������$n",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$h: �õ�����(��������)�� ���ϸ�� %.1fm���̿� ����������� �������� �ۿ��ϴ� ������ �Ѵ�.$n", m_dRail_HeightSB);

	double dLengthModel  = toM(pBri->GetLengthModelSpan(-1));
	double dWidthEff = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);
	double LoadUnitSi   = GetValueUnitChange(m_dRail_LoadSidong,UNIT_CGS_TONF,pData->m_szTonf);
	double LoadUnitZe    = GetValueUnitChange(m_dRail_LoadZedong,UNIT_CGS_TONF,pData->m_szTonf);

	double HForceSi       = GetValueUnitChange(m_dRail_TsbSidong,UNIT_CGS_TONF,pData->m_szTonf);
	double HForceZe       = GetValueUnitChange(m_dRail_TsbZedong,UNIT_CGS_TONF,pData->m_szTonf);

	if(pData->IsKRL_2012())		// KRL-2012, KRL-2012(��������) - 0.75���
	{
		double dFactor = (pData->m_nTypeRailLoad == RAIL_LOAD_KRL2012_P)? 0.75 : 1.0;
		BOOL bFactor = (pData->m_nTypeRailLoad == RAIL_LOAD_KRL2012_P)? TRUE : FALSE;

		double dUnitForceSi = GetValueUnitChange(m_dRail_UnitForceSidong, UNIT_CGS_TONF_M, pData->m_szTonf_M);
		double dUnitForceZe = GetValueUnitChange(m_dRail_UnitForceZedong, UNIT_CGS_TONF_M, pData->m_szTonf_M);
		double dMaxLoadSi = GetValueUnitChange(100, UNIT_CGS_TONF, pData->m_szTonf);
		double dMaxLoadZe = GetValueUnitChange(600, UNIT_CGS_TONF, pData->m_szTonf);
		if(bFactor)
		{
			XLText->AddFormatText("$h   �õ����� - %.2f x %.1f%s x L �� %.0f%s, �������� - %.2f x %.1f%s x L �� %.0f%s$n",
				dFactor, dUnitForceSi, pData->m_szTonf_M, dMaxLoadSi, pData->m_szTonf,
				dFactor, dUnitForceZe, pData->m_szTonf_M, dMaxLoadZe, pData->m_szTonf);
		}
		else
		{
			XLText->AddFormatText("$h   �õ����� - %.1f%s x L �� %.0f%s, �������� - %.1f%s x L �� %.0f%s$n",
				dUnitForceSi, pData->m_szTonf_M, dMaxLoadSi, pData->m_szTonf,
				dUnitForceZe, pData->m_szTonf_M, dMaxLoadZe, pData->m_szTonf);
		}
		
		XLText->AddFormatText("$h   ���⿡�� ����� ��� ������� �ʴ´�.$n");

		XLText->AddFormatText("$n$h %d)$r�õ�����$n",nIndexSmall++);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		if(bFactor)
		{
			XLText->AddFormatText("$h$rTs$r$c=$r%.2f x %.1f x %.3f �� %.0f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",
				dFactor, dUnitForceSi, dLengthModel, dMaxLoadSi,HForceSi,pData->m_szTonf);
		}
		else
		{
			XLText->AddFormatText("$h$rTs$r$c=$r%.1f x %.3f �� %.0f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",
				dUnitForceSi, dLengthModel, dMaxLoadSi,HForceSi,pData->m_szTonf);
		}
		
		XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTs'$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",HForceSi,dWidthEff,dLengthModel,LoadUnitSi,pData->m_szTonf_M);

		nIndexTiny  = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$h %d)$r��������$n",nIndexSmall++);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		if(bFactor)
		{
			XLText->AddFormatText("$h$rTb$r$c=$r%.2f x %.1f x %.3f �� %.0f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",
				dFactor, dUnitForceZe, dLengthModel, dMaxLoadZe,HForceZe,pData->m_szTonf);
		}
		else
		{
			XLText->AddFormatText("$h$rTb$r$c=$r%.1f x %.3f �� %.0f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",
				dUnitForceZe, dLengthModel, dMaxLoadZe,HForceZe,pData->m_szTonf);
		}

		XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTb'$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",HForceZe,dWidthEff,dLengthModel,LoadUnitZe,pData->m_szTonf_M);

	}
	else if(pData->m_nTypeRailLoad == RAIL_LOAD_EL18_2017)	// EL ǥ�ؿ�������
	{
		XLText->AddFormatText("$h   �õ����� - (0.27 + 0.95 x L / Lv) x T, �������� - (0.27 + 1.00 x L / Lv) x T$n");
		XLText->AddFormatText("$h   ���⿡�� ����� ��� ������� �ʴ´�.$n");

		double WheelLoad	= GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf);

		XLText->AddFormatText("$n$h %d)$r�õ�����$n",nIndexSmall++);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTs$r$c=$r(0.27 + 0.95 x %.3f / %.3f) x %.1f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n", dLengthModel, m_dRail_LengthLv, WheelLoad, HForceSi, pData->m_szTonf);

		XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTs'$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me1$f3$i%.3f$r$r%s$n",HForceSi,dWidthEff,dLengthModel,LoadUnitSi,pData->m_szTonf_M);

		nIndexTiny  = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$h %d)$r��������$n",nIndexSmall++);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h%s$r����� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTb$r$c=$r(0.27 + 1.00 x %.3f / %.3f) x %.1f$m+11$c=$r$me1$f3$i%.3f$r$r%s$n", dLengthModel, m_dRail_LengthLv, WheelLoad, HForceSi, pData->m_szTonf);
		
		XLText->AddFormatText("$h%s$r������ ȯ��$n",GetNumberString(nIndexTiny,NUM_CIRCLE));
		XLText->AddFormatText("$h$rTb'$r$c=$r%.3f / %.3f / %.3f$m+09$c=$r$me2$f3$i%.3f$r$r$r%s$n",HForceZe,dWidthEff,dLengthModel,LoadUnitZe,pData->m_szTonf_M);
	}

	double HForce = GetValueUnitChange(m_dRail_LoadSB,UNIT_CGS_TONF,pData->m_szTonf);
	double Moment = GetValueUnitChange(m_dRail_MomentSB,UNIT_CGS_TONFM,pData->m_szTonfM);
	nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r�ۿ����� ����$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(m_bSingleLane)
	{
		XLText->AddFormatText("$h: �ܼ��̹Ƿ� �õ����߰� ���������߿� ū ���� �����Ѵ�.$n");
		XLText->AddFormatText("$h%s$r�ۿ�����$m+03$c:$rMAX(%.3f , %.3f)$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),LoadUnitSi,LoadUnitZe,HForce,pData->m_szTonf);
	}
	else
	{
		XLText->AddFormatText("$h: �����̹Ƿ� �õ����� + ���������� �����Ѵ�.$n");
		XLText->AddFormatText("$h%s$r�ۿ�����$m+03$c:$r%.3f + %.3f$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),LoadUnitSi,LoadUnitZe,HForce,pData->m_szTonf);
	}
	if(pBri->m_bUnderGroundRahmen)
		XLText->AddFormatText("$h%s$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f + %.3f + %.2f$m+13$c=$r$me1$f3$i%.3f$r$rm$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(m_dUnderGroundDesignHeight),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS/2),m_dRail_HeightSB,m_dRail_HeightSidong);
	else
		XLText->AddFormatText("$h%s$r�ۿ����$m+03$c:$r%.3f + %.3f + %.3f + %.3f + %.3f + %.2f$m+13$c=$r$me1$f3$i%.3f$r$rm$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),toM(m_dHeightRail),toM(m_dRail_HeightTie),toM(m_dHeightDosangUnderTie),toM(pBri->m_dUnderGroundHeightMortar),toM(pBri->m_dTS/2),m_dRail_HeightSB,m_dRail_HeightSidong);
	XLText->AddFormatText("$h%s$r�ۿ���Ʈ$m+03$c:$r%.3f x %.3f$m+13$c=$r$me1$f3$i%.3f$r$r%s$n$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),HForce,m_dRail_HeightSidong,Moment,pData->m_szTonfM);
}

void CARcBridgeCalcLoad::WriteCalcWaterPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	if(pBri->IsBoxType() == FALSE) return;

	long nIndexSmall = 1;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CLineInfo *pLine = pBri->GetLineBase();
	CFEMManage *pFEM = &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dEle_Water = toM(pBri->m_dGroundWaterLevel);
	double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;

	double dEle_Top  = toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(0).x) - dUT);
	if(bLsd)
	{
		dEle_Water	= frM(dEle_Water);
		dEle_Top	= frM(dEle_Top);
	}

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+15$h$a%d.%d �� ��%s$n",nIndexTitle,nIndexMiddle, bLsd? _T(" (WA)") : _T(""));

	if(pBri->m_bApplyGroundWaterLevel)
	{
		if (bLsd)
		{
			XLText->AddFormatText("$s+08$h %d)$r������ �ۿ��ϴ� ���� : ���ϼ��� EL. %.3fm$n",nIndexSmall++, toM(dEle_Water));
		}
		else
		{
			XLText->AddFormatText("$s+08$h %d)$r������ �ۿ��ϴ� ���� : ���ϼ��� EL %.3f$n",nIndexSmall++, dEle_Water);
		}

		long nColFormula = bLsd? 15 : 14;
		long nColResult	= bLsd? 3 : 2;
		long nDecResult = bLsd? 6 : 3;

		long nSizeEle   = pFEM->GetElementSize();
		double dQ_Point = 0;
		double dQ_Height = 0;

		long nIdxToap = 1;
		CJoint *pJntSlab = pFEM->GetJoint((USHORT)m_pStd->m_pFrame->GetNumberJointSlabStt(pFEM));
		CJoint *pJntWall = NULL;

		double dQ_prev = dQ_Point;
		double dQ_Height_prev = 0;
		for(long i=0; i<nSizeEle; i++)
		{
			CElement *pEle = pFEM->GetElement((USHORT)i);
			if(pEle->m_nFlag != ELEMENT_STTWALL) continue;

			pJntWall = pFEM->GetJoint(pEle->m_nJEnd);
			dQ_Height = pJntSlab->m_vPoint.z - pJntWall->m_vPoint.z;

			if(Compare(dEle_Top - dQ_Height, dEle_Water, _T(">"))) continue;

			dQ_Point  = GetValueUnitChange(GetLoadWaterPressure(bLsd? toM(dEle_Top - dQ_Height) : (dEle_Top - dQ_Height)), UNIT_CGS_TONF_M2 ,pData->m_szTonf_M2);
			if(dQ_prev > 0)
			{
				if(bLsd)
				{
					XLText->AddFormatText("$h$r$cqw%d$r$c=$r%s + %s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
						nIdxToap++, pData->GetStrDouble(dQ_prev, UNIT_CGS_TONF_M2, TRUE), pData->GetStrDouble(dQ_Height - dQ_Height_prev, UNIT_CGS_M), pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3, TRUE), 
						nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
				}
				else
				{
					XLText->AddFormatText("$h$r$cqw%d$r$c=$r%6.3f + %.3f x %.3f$m+14$c=$r$me1$f3$i%.3f$r$r%s$n",nIdxToap++, dQ_prev, dQ_Height - dQ_Height_prev, dUW_Water, dQ_Point, pData->m_szTonf_M2);
				}

			}
			else
			{
				XLText->AddFormatText("$h$r$cqw%d$r$c=$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
					nIdxToap++,  pData->GetStrDouble(dEle_Water - (dEle_Top - dQ_Height), UNIT_CGS_M), pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3, TRUE),
					nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
			dQ_prev = dQ_Point;
			dQ_Height_prev = dQ_Height;
		}
		*XLText += 1;
	}

	if(bLsd == FALSE)
	{
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
		{
			double dQ_Point = GetValueUnitChange(m_dLoadBuoyancy, UNIT_CGS_TONF ,pData->m_szTonf);

			XLText->AddFormatText("$s+02$h %d)$r�Ϻν����꿡 �ۿ��ϴ� ����$n",nIndexSmall++);
			XLText->AddFormatText("$h$r$cVw$r$c=$r(%6.3f - %6.3f) x %.3f$m+14$c=$r$me1$f3$i%.3f$r$r%s$n", dEle_Water, toM(pBri->m_dELSB), dUW_Water, dQ_Point, pData->m_szTonf_M2);
			*XLText += 1;
		}
	}

	if(bLsd)
	{
		// �Ѱ���� �϶��� �η��� ������ �������� �и�
		// �ɼǿ� ���� ���μ����� ���� �Ѵ�.
		if(pBri->m_nSelectInteriorWaterPress == 2 && pBri->m_dHeightInteriorWaterPress > 0)
		{
			if(!pBri->m_bApplyGroundWaterLevel)
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$s+15$h$a%d.%d �� ��%s$n",nIndexTitle,nIndexMiddle, bLsd? _T(" (WA)") : _T(""));
			}
			WriteCalcInteriorWaterPress(XLText, pBri, COL_POS_1, nIndexSmall);
		}
	}
	else
	{
		if(pBri->m_dHeightInteriorWaterPress > 0)
		{
			WriteCalcInteriorWaterPress(XLText, pBri, COL_POS_1, nIndexSmall);
		}
	}
}


void CARcBridgeCalcLoad::WriteCalcSoilPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake, EGoalOfEarthQuake eGoalEq)
{
	// ATest �۾� �� ���� ���� - 2015-07-07 : �Լ� ��ü ���� �� ���� ����. ���� �������� # ǥ��.

	if(bEarthquake && pBri->IsBoxType() == FALSE) return;

	long nIndexSmall = 1;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	HGBOOL bLSD = pData->IsLsdDesign();
	
	double Ko = bEarthquake ? m_pStd->GetCoefficient_MononobeOkabe(eGoalEq) : m_pStd->m_dCoefficientKo;		// ���
	double UW_BackFill  = GetValueUnitChange(pBri->m_dUnitWeigthBackFill,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_BackFill_W = GetValueUnitChange(pBri->m_dUnitWeigthBackFillWater,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_Ascon	   = GetValueUnitChange(m_UW_Ascon,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dLiveAlways  = (pBri->m_nSelectLiveLoadHori==1) ? 0 : GetValueUnitChange(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double dThickAsocn  = GetValueUnitChange(toM(pBri->m_dTP), UNIT_CGS_M, pData->m_szM);
	double dThickSlab   = GetValueUnitChange(toM(pBri->m_dTS), UNIT_CGS_M, pData->m_szM);
	double dHeightLeft  = GetValueUnitChange(toM(pBri->GetHeightModelJijum(0)), UNIT_CGS_M, pData->m_szM);
	double dHeightRight = GetValueUnitChange(toM(pBri->GetHeightModelJijum(pBri->m_nQtyJigan)), UNIT_CGS_M, pData->m_szM);
	double dThickSoil   = GetValueUnitChange(toM(m_dUnderGroundDesignHeight), UNIT_CGS_M, pData->m_szM);
	double dWeightTrain = dLiveAlways; // GetValueUnitChange(3.500,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dWeightLane  = GetValueUnitChange(pBri->m_dDeadLoadAlways,UNIT_CGS_TONF_M2,pData->m_szTonf_M2); //GetValueUnitChange(pBri->m_dUnitWeightRail * pBri->m_nQtyRail,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double dEle_Water   = GetValueUnitChange(toM(pBri->m_dGroundWaterLevel), UNIT_CGS_M, pData->m_szM);
	double dApplyWidth	= bEarthquake? GetValueUnitChange(m_pStd->GetSlabWidth(-1), UNIT_CGS_M, pData->m_szM) : GetValueUnitChange(1.0, UNIT_CGS_M, pData->m_szM);
	double dEtcDeadLoad = GetValueUnitChange(m_DeadLoadDistribution[0] + m_DeadLoadDistribution[1], UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if(bEarthquake)
	{
		XLText->AddFormatText("$s+50$h$n$a%d.%d ������ ��� : %s$n",nIndexTitle,nIndexMiddle, pBri->GetStringGoalEarthQuake(eGoalEq));
	}
	else
	{
		if (bLSD)
		{
			XLText->AddFormatText("$s+15$h$n$a%d.%d �� ��(EH)$n",nIndexTitle,nIndexMiddle);
		}
		else
		{
			XLText->AddFormatText("$s+15$h$n$a%d.%d �� ��$n",nIndexTitle,nIndexMiddle);
		}
	}

	if(bEarthquake)
	{
		double Kae   = Ko;
		double Kh	 = m_pStd->GetKhValue(FALSE, TRUE, eGoalEq);
		double Kv	 = 0;
		double theta = atan(Kh / (1 - Kv));

		XLText->AddFormatText("$n$h %d)$r������ ��а��(Mononobe Okabe ���)$n",nIndexSmall++);
		XLText->AddFormatText("$h$r$r$r$p[�������ֵ���а��]$n");
		XLText->AddFormatText("$h$r$p[K_AE]$r$c=$n$n");
		XLText->AddFormatText("$h$r$r$c=$r'%.3f$n",Kae);
		XLText->AddFormatText("$h$m+02���⼭,$r$r$c��$r$c=$r��ä�� ���� ���θ�����( = %.2f ��)$n",pBri->m_dAngleFrictionBackFill);
		XLText->AddFormatText("$h$m+04$c��$r$c=$r��ǥ��� ������� �̷�� ��( = 0 ��)$n");
		XLText->AddFormatText("$h$m+04$c��$r$c=$r������ �������� �̷�� ��( = 0 ��)$n");
		XLText->AddFormatText("$h$m+04$c��$r$c=$r������ ��ä��������� ���鸶����( = 0 ��)$n");
		XLText->AddFormatText("$h$m+04$cKh$r$c=$r����������� = %.3f$n", Kh);
		XLText->AddFormatText("$h$m+04$c��$r$c=$rtan-1[Kh / (1-Kv)] = %.3f, Kv = %.3f$n",ToDegree(theta),Kv);
	}
	else
	{
		XLText->AddFormatText("$n$h %d)$r������� ���$n",nIndexSmall++);
		XLText->AddFormatText("$h$r$cKo$r$c=$r1.0 - sin�� = 1.0 - sin%.1f�� = %.3f$n",pBri->m_dAngleFrictionBackFill,Ko);
	}
	*XLText += 1;

	if(bEarthquake == FALSE)
	{
		double dAlwaysSum   = dThickSoil*UW_BackFill;
		
		if(pBri->m_nSelectLiveLoadHori==0)
		{
			if(pData->IsRailDesign())
				dAlwaysSum += dWeightTrain + dWeightLane;
			else
				dAlwaysSum += dLiveAlways;
		}
		else if(pData->IsRailDesign())
			dAlwaysSum += dEtcDeadLoad;

		if(pData->IsRailDesign() && dAlwaysSum>0)
		{
			XLText->AddFormatText("$h %d)$r��������$n",nIndexSmall++);
			long nIndexTiny = 1;
			if(dThickSoil>0)
			{
				XLText->AddFormatText("$h$r%s$r��������$m+03$c:$r%s x %s$m+10$c=$r$me1$f3$i%s$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),
					pData->GetStrDouble(dThickSoil,UNIT_CGS_M),pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3),
					pData->GetStrDouble(dThickSoil*UW_BackFill,UNIT_CGS_M2),pData->m_szTonf_M);
				XLText->AddFormatText("$n");
			}
			if(pBri->m_nSelectLiveLoadHori==0)
			{
				XLText->AddFormatText("$h$r%s$r��������$m+03$c:$r$m+10$c=$r$me1$f3$i%s$r$r%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),
					pData->GetStrDouble(dWeightTrain, UNIT_CGS_TONF_M),pData->m_szTonf_M);
				XLText->AddFormatText("$h$r%s$r�˵��߷�$m+03$c:$r$m+10$c=$r$me1$f3$i%s$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),
					pData->GetStrDouble(dWeightLane,UNIT_CGS_TONF_M),pData->m_szTonf_M);
			}
			else
			{
				XLText->AddFormatText("$h$r%s$r��������$m+03$c:$r$m+10$c=$r$me1$f3$i%s$r$r%s",GetNumberString(nIndexTiny++,NUM_CIRCLE),
					pData->GetStrDouble(dEtcDeadLoad, UNIT_CGS_TONF_M),pData->m_szTonf_M);
			}
			if(dAlwaysSum>0)
			{
				XLText->AddFormatText("$h$r$-19");
				XLText->AddFormatText("$n$h$r$r��  ��$m+03$c:$r$m+10$c=$r$me1$f3$i%s$r$r%s$n", pData->GetStrDouble(dAlwaysSum, UNIT_CGS_TONF_M), pData->m_szTonf_M);
				*XLText += 1;
			}
		}

		if (bLSD == FALSE)
		{
			if(pData->IsRailDesign()==FALSE && pBri->m_nSelectLiveLoadHori==0)
			{
				XLText->AddFormatText("$h %d)$r�������� : %s %s$n",nIndexSmall++,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
				*XLText += 1;
			}
		}
	}

	const HGINT32 iDecResult = bLSD ? pData->GetDecimal(UNIT_CGS_TONF_M2) : 3;
	const HGINT32 iColResult = bLSD ? 3 : 2;
	BOOL bSameLR = Compare(dHeightLeft,dHeightRight,_T("=")) ? TRUE : FALSE;
	long nCaseW = (pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel) ? 2 : 1;
	for(long nW = 0; nW < nCaseW; nW++)
	{
		BOOL bWaterCase = (nW == 1) ? TRUE : FALSE;
		CString strW = (nCaseW == 1) ? _T("") : (bWaterCase ? _T(" (���ϼ��� �ִ� ���)") : _T(" (���ϼ��� ���� ���)"));
		for(long nSE = iSTT; nSE <= iEND; nSE++)
		{
			BOOL bSttWall = (nSE == iSTT) ? TRUE : FALSE;
			if(nSE == iEND && bSameLR) break;

			CString str = bSameLR ? _T("") : (bSttWall ? _T(" : ������ ��ü") : _T(" : ������ ��ü"));
			XLText->AddFormatText("$s+08$h %d)$r%s �ۿ����%s%s$n",nIndexSmall++, bEarthquake ? _T("������") :  _T("���"), str, strW);

			long nSizeEle   = pFEM->GetElementSize();
			double dQ_Point = GetValueUnitChange(GetLoadSoilPressure(0, bSttWall, bWaterCase, bEarthquake, eGoalEq) * 
				(bLSD==TRUE ? toM(dApplyWidth) : dApplyWidth), bEarthquake ? UNIT_CGS_TONF_M : UNIT_CGS_TONF_M2, bEarthquake ? pData->m_szTonf_M : pData->m_szTonf_M2);

			double dQ_Height = 0;

			BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE; // ���� EL�� ����
			long nIdxSlab = bSttWall ? 0 : m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetSize()-1;
			double dEle_Top  = GetValueUnitChange(toM(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(nIdxSlab).z), UNIT_CGS_M, pData->m_szM);
			if(bUpperSlope == FALSE)
			{
				CLineInfo *pLine = pBri->GetLineBase();
				CDPoint vAngStt = pBri->GetAngleJijum(0);
				double dWS = pBri->m_dWS/vAngStt.y;
				double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
				double dSta = pBri->GetStationOnJijum(0,0) - dWS / 2.;
				dEle_Top = GetValueUnitChange(toM(pLine->GetElevationPlanEl(dSta) - dUT), UNIT_CGS_M, pData->m_szM);
			}

			double dUW = (bWaterCase && (dEle_Top - dQ_Height) < dEle_Water) ? UW_BackFill_W : UW_BackFill;

			long nIdxToap = 1;
			CJoint *pJntSlab = bSttWall ? pFEM->GetJoint((USHORT)m_pStd->m_pFrame->GetNumberJointSlabStt(pFEM)) : pFEM->GetJoint((USHORT)m_pStd->m_pFrame->GetNumberJointSlabEnd(pFEM));
			CJoint *pJntWall = NULL;
			if(pData->IsRailDesign())
			{
				if(dThickSoil>0)
				{
					if(pBri->m_nSelectLiveLoadHori==0)
					{
						if(bEarthquake)
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s + %s x %s) x %s$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko, pData->GetStrDouble(dThickSoil*dUW,UNIT_CGS_M) , pData->GetStrDouble(dWeightTrain, UNIT_CGS_TONF_M),
								pData->GetStrDouble(dWeightLane,UNIT_CGS_TONF_M), pData->GetStrDouble(dThickSlab/2,UNIT_CGS_M),
								pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3), pData->GetStrDouble(dApplyWidth,UNIT_CGS_M), 
								pData->GetStrDouble(dQ_Point,UNIT_CGS_TONF_M), pData->m_szTonf_M);
						else
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickSoil*dUW,UNIT_CGS_TONF_M2), pData->GetStrDouble(dWeightTrain, UNIT_CGS_TONF_M),
								pData->GetStrDouble(dWeightLane, UNIT_CGS_TONF_M), pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
								pData->GetStrDouble(dUW, UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point,UNIT_CGS_TONF_M),pData->m_szTonf_M2);
					}
					else
					{
						if(bEarthquake)
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s) x %s$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickSoil*dUW + dEtcDeadLoad, UNIT_CGS_TONF_M2), pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
								pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3), pData->GetStrDouble(dApplyWidth, UNIT_CGS_M), 
								pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
						else
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickSoil*dUW + dEtcDeadLoad, UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
								pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
					}
				}
				else
				{
					if(bEarthquake)
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s x %s) x %s$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dWeightTrain, UNIT_CGS_TONF_M),pData->GetStrDouble(dWeightLane, UNIT_CGS_TONF_M),
							pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),
							pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
					else
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dWeightTrain, UNIT_CGS_TONF_M),pData->GetStrDouble(dWeightLane, UNIT_CGS_TONF_M),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
							pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
				}
			}
			else
			{
				// (34189) �ۿ������ �������߿� ���忡���� ���ߵ� ����.
				double dSoilPressureVert  = GetValueUnitChange(m_dSoilPressureVert,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				if(pBri->m_bUnderGroundRahmen)
				{
					double dStationBridge    = pBri->GetStationOnJijum(0,0);
					double dWidthSlab        = bLSD? pBri->GetWidthSlabAct(dStationBridge, CDPoint(0,1)) : toM(pBri->GetWidthSlabAct(dStationBridge, CDPoint(0,1)));
					double dPressAscon =  GetValueUnitChange(m_dWeightAscon,UNIT_CGS_TONF_M,pData->m_szTonf_M) / dWidthSlab;
					if(bEarthquake)
					{
						if(dLiveAlways>0)
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s + %s x %s) x %s$m+16$c=$r$me3$f3$i%s$r$r$r%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dPressAscon,UNIT_CGS_TONF_M2),pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2), 
							pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),
							pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
						else
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%.3f + %.3f + %.3f x %.3f) x %.3f$m+16$c=$r$me2$f3$i%.3f$r$r$r%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dPressAscon,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
							pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
					}
					else
					{
						if(dLiveAlways>0 && bLSD == FALSE)
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dPressAscon,UNIT_CGS_TONF_M2),pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),
							pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
						else
							XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s + %s x %s)$m+14$c=$r$me%d$f%d$i%s$m+%02d%s$n",nIdxToap++
							,Ko,pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dPressAscon,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
							pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3), iColResult-1, iDecResult,
							pData->GetStrDouble(dQ_Point, bLSD ? UNIT_CGS_TONF_M2 : UNIT_CGS_TONF_M, bLSD ? TRUE : FALSE), iColResult, pData->m_szTonf_M2);
					}
				}
				else
				{
					if(bEarthquake)
					{
						if(dLiveAlways>0)
						{
							CString strr = pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M);
							if(dSoilPressureVert > 0)
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s + %s + %s x %s) x %s$m+16$c=$r$me2$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3), 
								pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),
								pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
							else
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s + %s x %s) x %s$m+16$c=$r$me2$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3),
								pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),
								pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),	pData->m_szTonf_M);
						}
						else
						{
							if(dSoilPressureVert > 0)
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s x %s + %s + %s x %s) x %s$m+16$c=$r$me2$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3), pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),
								pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),
								pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
							else
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s x %s + %s x %s) x %s$m+16$c=$r$me2$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
								pData->GetStrDouble(dApplyWidth, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M);
						}
					}
					else
					{
						if(dLiveAlways>0 && bLSD == FALSE)
						{
							if(dSoilPressureVert > 0)
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s + %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3), 
								pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),
								pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
							else
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s + %s x %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3),
								pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),
								pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
						}
						else
						{
							if(dSoilPressureVert > 0)
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s x %s + %s + %s x %s)$m+14$c=$r$me1$f3$i%s$r$r$r%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3), pData->GetStrDouble(dSoilPressureVert,UNIT_CGS_TONF_M2),
								pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3),pData->GetStrDouble(dQ_Point, UNIT_CGS_TONF_M),pData->m_szTonf_M2);
							else
								XLText->AddFormatText("$h$r$cq%d$r$c=$r%.3f x (%s x %s + %s x %s)$m+14$c=$r$me%d$f%d$i%s$m+%02d%s$n",nIdxToap++
								,Ko,pData->GetStrDouble(dThickAsocn,UNIT_CGS_M),pData->GetStrDouble(UW_Ascon,UNIT_CGS_TONF_M3),pData->GetStrDouble(dThickSlab/2, UNIT_CGS_M),
								pData->GetStrDouble(dUW,UNIT_CGS_TONF_M3), iColResult-1, iDecResult, 
								pData->GetStrDouble(dQ_Point, bLSD ? UNIT_CGS_TONF_M2 : UNIT_CGS_TONF_M, bLSD ? TRUE : FALSE), iColResult, pData->m_szTonf_M2);
						}
					}
				}
			}
			double dQ_prev = dQ_Point;
			double dQ_Height_prev = 0;
			for(long i=0; i<nSizeEle; i++)
			{
				CElement *pEle = pFEM->GetElement((USHORT)i);
				if( bSttWall && pEle->m_nFlag != ELEMENT_STTWALL) continue;
				if(!bSttWall && pEle->m_nFlag != ELEMENT_ENDWALL) continue;

				pJntWall = pFEM->GetJoint(pEle->m_nJEnd);
				dQ_Height = (pJntSlab->m_vPoint.z - pJntWall->m_vPoint.z);
				double dUW = (bWaterCase && (dEle_Top - dQ_Height) < dEle_Water) ? UW_BackFill_W : UW_BackFill;

				dQ_Point  = GetValueUnitChange(GetLoadSoilPressure((bLSD==TRUE ? toM(dQ_Height) : dQ_Height), bSttWall, bWaterCase, bEarthquake, eGoalEq) * 
					(bLSD==TRUE ? toM(dApplyWidth) : dApplyWidth), bEarthquake ? UNIT_CGS_TONF_M : UNIT_CGS_TONF_M2, bEarthquake ? pData->m_szTonf_M : pData->m_szTonf_M2);

				if(bEarthquake)
				{
					if( bLSD == TRUE )
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%s + %.3f x %s x %s x %s$m+16$c=$r$me2$f3$i%s$r$r$r%s$n",nIdxToap++, 
						pData->GetStrDouble(dQ_prev,UNIT_CGS_TONF_M) , Ko, pData->GetStrDouble(dQ_Height - dQ_Height_prev, UNIT_CGS_M), pData->GetStrDouble(dUW, UNIT_CGS_TONF_M3), pData->GetStrDouble(dApplyWidth,UNIT_CGS_M), 
						pData->GetStrDouble(dQ_Point,UNIT_CGS_TONF_M), pData->m_szTonf_M);
					else
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%6.3f + %.3f x %.3f x %.3f x %.3f$m+16$c=$r$me2$f3$i%.3f$r$r$r%s$n",nIdxToap++, 
						dQ_prev, Ko, dQ_Height - dQ_Height_prev, dUW, dApplyWidth, dQ_Point, pData->m_szTonf_M);
				}
				else
				{
					if( bLSD == TRUE )
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%s + %.3f x %s x %s$m+14$c=$r$me%d$f%d$i%s$m+%02d%s$n",nIdxToap++, pData->GetStrDouble(dQ_prev,UNIT_CGS_TONF_M2, TRUE), Ko, 
						pData->GetStrDouble(dQ_Height - dQ_Height_prev, UNIT_CGS_M), pData->GetStrDouble(dUW, UNIT_CGS_TONF_M3), iColResult - 1, iDecResult,
						pData->GetStrDouble(dQ_Point,UNIT_CGS_TONF_M2, TRUE), iColResult, pData->m_szTonf_M2);
					else
						XLText->AddFormatText("$h$r$cq%d$r$c=$r%6.3f + %.3f x %.3f x %.3f$m+14$c=$r$me1$f3$i%.3f$r$r$r%s$n",nIdxToap++, dQ_prev, Ko, 
						dQ_Height - dQ_Height_prev, dUW, dQ_Point, pData->m_szTonf_M2);
				}
				dQ_prev = dQ_Point;
				dQ_Height_prev = dQ_Height;
			}
			*XLText += 1;
		}
	}
}

void CARcBridgeCalcLoad::WriteCalcSubsidence(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
// 	if(0 && pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID] == FALSE)
// 	{
// 		return;
// 	}

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if (bLsd == FALSE)
	{
		XLText->AddFormatText("$s+07$n$h$a%d.%d �� �� ħ ��$n",nIndexTitle,nIndexMiddle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h����ħ�ϴ� %.0fmm�� �����Ͽ� �����Ѵ�.$n",frM(m_dLoadSubsidence*2));
		XLText->AddFormatText("$h(��ũ��Ʈ�������� ź��������� 50%%�� ����, �����α� ������ء�2.1.15)$n");
		XLText->AddFormatText("$h�� ����ħ�ϴ� %.0fmm�� ź���ؼ��� ����.$n",frM(m_dLoadSubsidence));
	}
	else
	{
		XLText->AddFormatText("$s+07$n$h$a%d.%d �� �� ħ ��(SD)$n",nIndexTitle,nIndexMiddle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h����ħ�ϴ� %.0fmm�� �ؼ��� ����.$n",frM(m_dLoadSubsidence));
	}
	
	*XLText += 1;
}

void CARcBridgeCalcLoad::WriteCalcTemperature(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bIsSlab = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)? TRUE:FALSE;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	BOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		XLText->AddFormatText("$s+07$h$e$a%d.%d �µ���ȭ����(TU)$n",nIndexTitle,nIndexMiddle);
	}
	else
	{
		XLText->AddFormatText("$s+07$h$e$a%d.%d �� �� �� ��$n",nIndexTitle,nIndexMiddle);
	}
	
	if (bLsd == FALSE)
	{
		if(bIsSlab)
		{
			CString str = _T("");
			if(pData->IsSI()) str.Format("%.0fmm", pBri->m_dTS);
			else str.Format("%.0fcm", toCm(pBri->m_dTS));
			XLText->AddFormatText("$h$r�ܸ��� �ּ�ġ���� %s �̹Ƿ� R.C �����걳 �����⿡ ���Ͽ� �µ����� ����� 10��C�� �Ѵ�.$n", str);

			if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==2)
			{
				XLText->AddFormatText("$h$rR.C �����걳���� �����⿡ ���Ͽ� ������ SHOE�� ����ϹǷ� ������ �µ����߿� ����$n");
				XLText->AddFormatText("$h$r������ ���� ������ ��,�ϸ��� �µ��� Tz = �� 5��C�� �����Ѵ�.$n");
			}
			else
			{
				XLText->AddFormatText("$h$rR.C �����걳���� �����⿡ ���Ͽ� ������ SHOE�� ����ϹǷ� ������ �µ����߿� ����$n");
				XLText->AddFormatText("$h$r������ �������� �ʴ´�.$n");
			}
		}
		else
		{
			long nIndexSmall = 1;
			
			XLText->AddFormatText("$n$h %d)$r��ũ��Ʈ���� �µ���ȭ ����$m+07$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n",nIndexSmall++,m_dLoadTempertureAxial,pBri->m_dTS);
			if(pBri->m_nSelectTempLoadSlab==0 || pBri->m_nSelectTempLoadSlab==2)  XLText->AddFormatText("$n$h %d)$r�ٴ��� ���ϸ� �µ���$m+06$c:$r %.0f ��$n$n",nIndexSmall,m_dLoadTempertureUpDown);
		}
	}
	else
	{
		XLText->AddFormatText("$n$h   $r��ũ��Ʈ���� �µ���ȭ ����$m+07$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n", m_dLoadTempertureAxial, pBri->m_dTS);
	}

	*XLText += 1;
}

void CARcBridgeCalcLoad::WriteCalcDryAndShrinkage(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	long nIndexSmall = 1;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if (pData->IsLsdDesign())
	{
		XLText->AddFormatText("$s+12$h$a%d.%d �� �� �� ��(SH)$n",nIndexTitle,nIndexMiddle);
	}
	else
	{
		XLText->AddFormatText("$s+12$h$a%d.%d �� �� �� ��$n",nIndexTitle,nIndexMiddle);
	}
	
	XLText->AddFormatText("$n$h %d)$r����â���(��)$m+04$c:$r%.3f x$r$r$p[Object_10_5]$n",nIndexSmall++,pBri->GetCoefficientExpansion()*pow(10.0,5.0));
	XLText->AddFormatText("$n$h %d)$r���������(��)$m+04$c:$r%.2f x$r$r$p[Object_10_5]$n",nIndexSmall++,pBri->GetRateDryAndShrinkage()*pow(10.0,5.0));
	XLText->AddFormatText("$n$h %d)$r%.0f���� �µ��������� ��ȯ�Ͽ� �����Ѵ�.$n",nIndexSmall++,m_dLoadDryAndShrinkage);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h��T = -��/�� = -%.6f/%.6f = -%.0f��$n",pBri->GetRateDryAndShrinkage(),pBri->GetCoefficientExpansion(),m_dLoadDryAndShrinkage);
	*XLText += 1;
}

void CARcBridgeCalcLoad::DrawLiveLoadLaneCount(CDomyun *pDomP, long nLaneCount)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CDomyun Dom(pDomP);
	double dScaleDim = 120;
	Dom.SetCWnd(m_pStd->m_pView);
	Dom.SetScaleDim(dScaleDim);
	Dom.SetLTScale(dScaleDim*5);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDimExo(2);
	Dom.SetDimVLen(5);
	Dom.SetDimExe(1);

	double dEffectiveWidth = frM(m_dEffectiveWidth);
	double dWidthTruck = frM(m_dDesignTruckWidth);
	double dDistTruck  = frM(m_dDesignRoadWidth-m_dDesignTruckWidth);
	double dLengthLane = nLaneCount*dWidthTruck + (nLaneCount-1)*dDistTruck + dEffectiveWidth;
	double dHeightDraw = dEffectiveWidth/2;
	double dHeightDim  = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimExe());
	double dWidthArrow = 300;

	Dom.Solid(0,0,dLengthLane,0,dLengthLane,pBri->m_dTP,0,pBri->m_dTP);
	Dom.LineTo(0,-dHeightDraw,dLengthLane,-dHeightDraw);

//	sX = 0;
	long i=0; for(i=0; i<nLaneCount; i++)
	{
		double sX = dEffectiveWidth/2 + i*(dWidthTruck + dDistTruck);
		Dom.LineTo(sX,pBri->m_dTP,sX,dHeightDim);
		Dom.Solid(sX,pBri->m_dTP,sX-dWidthArrow/4,dWidthArrow,sX+dWidthArrow/4,dWidthArrow,sX,pBri->m_dTP);
		if(i==0)
		{
			Dom.LineTo(sX,0,sX-dEffectiveWidth/2,-dHeightDraw);
		}
		else
		{
			Dom.LineTo(sX,0,sX-dHeightDraw,-dHeightDraw);
		}
		Dom.LineTo(sX,0,sX+dHeightDraw,-dHeightDraw);
		Dom.TextOut(sX,dHeightDim+Dom.Always(1),"P");
		sX += dWidthTruck;
		Dom.LineTo(sX,pBri->m_dTP,sX,dHeightDim);
		Dom.Solid(sX,pBri->m_dTP,sX-dWidthArrow/4,dWidthArrow,sX+dWidthArrow/4,dWidthArrow,sX,pBri->m_dTP);
		Dom.LineTo(sX,0,sX-dHeightDraw,-dHeightDraw);
		if(i==nLaneCount-1)
		{
			Dom.LineTo(sX,0,sX+dEffectiveWidth/2,-dHeightDraw);
		}
		else
		{
			Dom.LineTo(sX,0,sX+dHeightDraw,-dHeightDraw);
		}
		Dom.TextOut(sX,dHeightDim+Dom.Always(1),"P");
	}

	long nDimDan = 0;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dEffectiveWidth/2,0,nDimDan);
	for(i=0; i<nLaneCount; i++)
	{
		Dom.DimLineTo(dWidthTruck,nDimDan,COMMA(dWidthTruck));
		if(i==nLaneCount-1) continue;
		Dom.DimLineTo(dDistTruck,nDimDan,COMMA(dDistTruck));
	}

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(0,-dHeightDraw,nDimDan);
	Dom.DimLineTo(dEffectiveWidth/2,nDimDan,"E/2");
	Dom.DimLineTo(dLengthLane-dEffectiveWidth,nDimDan,COMMA(dLengthLane-dEffectiveWidth));
	Dom.DimLineTo(dEffectiveWidth/2,nDimDan,"E/2");

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_HIDD);
	Dom.Point(rect.left,rect.bottom+Dom.Always(1));

	*pDomP << Dom;
}

void CARcBridgeCalcLoad::DrawDeadLoadRail(CDomyun *pDomP, BOOL bForCalc, double dStation)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nSize = 1;//pBri->m_nQtyRail;

	for(long nRail=0; nRail<nSize; nRail++)
	{
		double H1  = pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - pBri->m_dRail_HeightTie - pBri->m_dUnderGroundHeightMortar;
		if(pBri->m_dDisBaseElevationRL == 0) H1 = pBri->GetHeightDosangInput();

		double A1  = ToRadian(m_dRail_AngleDosangDistribute);
		
		double H2  = (pData->m_nTypeCalcSlabEffWidth==0) ? m_dUnderGroundDesignHeight+pBri->m_dUnderGroundHeightMortar+pBri->m_dTS/2 : 0;
		double B   = frM(m_dRail_WidthDistribute);
		double Off = 300;
		double dDistRail = pBri->GetLengthRailByLine(nRail);
		
		CDomyun Dom(pDomP);
		double dScaleDim = 50;
		Dom.SetCWnd(m_pStd->m_pView);
		Dom.SetScaleDim(dScaleDim);
		Dom.SetLTScale(dScaleDim*5);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
		Dom.SetDimExo(2);
		Dom.SetDimVLen(5);
		Dom.SetDimExe(1);
		
		// ħ������ �߽��� (0,0);
		Dom.Rectangle(-m_dRail_LengthTie/2,0,m_dRail_LengthTie/2,-m_dRail_HeightTie);
		
		if(bForCalc)
		{
			Dom.Rectangle(-B/2-Off,-m_dRail_HeightTie-H1,
				B/2+Off,-m_dRail_HeightTie-H1-H2);
			Dom.Solid(-B/2-Off,-m_dRail_HeightTie-H1,
				B/2+Off,-m_dRail_HeightTie-H1,
				B/2+Off,-m_dRail_HeightTie-H1-pBri->m_dUnderGroundHeightMortar,
				-B/2-Off,-m_dRail_HeightTie-H1-pBri->m_dUnderGroundHeightMortar);
			
			double DRail = 1500; // ������ ��ġ...
			double WRail =  m_dHeightRail/2;
			double HRail =  m_dHeightRail/6.0;
			double TRail =  m_dHeightRail/6.0;
			Dom.Rectangle(-DRail/2-WRail/2,HRail,-DRail/2+WRail/2,0);
			Dom.Rectangle(-DRail/2-WRail/2,m_dHeightRail,-DRail/2+WRail/2,m_dHeightRail-HRail);
			Dom.Rectangle(-DRail/2-TRail/2,m_dHeightRail-HRail,-DRail/2+TRail/2,HRail);
			Dom.TextOut(0,m_dHeightRail,"+");
			Dom.Rectangle(DRail/2+WRail/2,HRail,DRail/2-WRail/2,0);
			Dom.Rectangle(DRail/2+WRail/2,m_dHeightRail,DRail/2-WRail/2,m_dHeightRail-HRail);
			Dom.Rectangle(DRail/2+TRail/2,m_dHeightRail-HRail,DRail/2-TRail/2,HRail);
			
			Dom.LineTo(-m_dRail_LengthTie/2,0,-m_dRail_LengthTie/2-Off,0);
			Dom.LineTo(-m_dRail_LengthTie/2-Off,0,-m_dRail_LengthTie/2-Off-H1/tan(A1),-m_dRail_HeightTie-H1);
			Dom.LineTo(m_dRail_LengthTie/2,0,m_dRail_LengthTie/2+Off,0);
			Dom.LineTo(m_dRail_LengthTie/2+Off,0,m_dRail_LengthTie/2+Off+H1/tan(A1),-m_dRail_HeightTie-H1);

			// ������ ǥ��
			Dom.Rectangle(-B/2-Off,-m_dRail_HeightTie-H1-H2+pBri->m_dTS/2,
				B/2+Off,-m_dRail_HeightTie-H1-H2-pBri->m_dTS/2);
		}
		else
		{
			Dom.Rectangle(-B/2,-m_dRail_HeightTie-H1-H2,B/2,-m_dRail_HeightTie-H1-H2);
		}
		
		// 
		Dom.LineTo(-m_dRail_LengthTie/2,-m_dRail_HeightTie,-m_dRail_LengthTie/2-H1/tan(A1),-m_dRail_HeightTie-H1);
		Dom.LineTo(-B/2,-m_dRail_HeightTie-H1-H2,-m_dRail_LengthTie/2-H1/tan(A1),-m_dRail_HeightTie-H1);
		//
		Dom.TextOut(-m_dRail_LengthTie/2-H1/tan(A1)+Dom.Always(5),-m_dRail_HeightTie-H1+Dom.Always(1),COMMA(m_dRail_AngleDosangDistribute)+"��");
		if(H2>0)
			Dom.TextOut(-B/2+Dom.Always(5),-m_dRail_HeightTie-H1-H2+Dom.Always(1),COMMA(m_dRail_AngleEffective)+"��");
		//
		Dom.LineTo(m_dRail_LengthTie/2,-m_dRail_HeightTie,m_dRail_LengthTie/2+H1/tan(A1),-m_dRail_HeightTie-H1);
		Dom.LineTo(B/2,-m_dRail_HeightTie-H1-H2,m_dRail_LengthTie/2+H1/tan(A1),-m_dRail_HeightTie-H1);
		
		CString str = _T("");
		long nDimDan = 0;
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(-m_dRail_LengthTie/2,0,nDimDan);
		str = bForCalc ? "" : "b";
		Dom.DimLineToExtend(m_dRail_LengthTie,nDimDan,COMMA(m_dRail_LengthTie), str);
		
		if(bForCalc)
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(B/2+Off,-m_dRail_HeightTie-H1-H2,nDimDan);
			Dom.DimLineTo(pBri->m_dTS/2,nDimDan,COMMA(pBri->m_dTS/2));
			Dom.DimLineToExtend(pBri->m_dUnderGroundHeightMortar,nDimDan,_T(""),COMMA(pBri->m_dUnderGroundHeightMortar));
			if(m_dUnderGroundDesignHeight>0)
				Dom.DimLineTo(m_dUnderGroundDesignHeight,nDimDan,COMMA(m_dUnderGroundDesignHeight));
			Dom.DimLineTo(H1,nDimDan,COMMA(H1));
			Dom.DimLineToExtend(m_dRail_HeightTie,nDimDan,_T(""),COMMA(m_dRail_HeightTie));
			Dom.DimLineTo(m_dHeightRail,nDimDan,COMMA(m_dHeightRail));
			nDimDan++;
			Dom.DimMoveTo(B/2+Off,-m_dRail_HeightTie-H1-H2,nDimDan);
			Dom.DimLineTo(m_dRail_HeightTie+H1+H2,nDimDan,COMMA(m_dRail_HeightTie+H1+H2));
		}
		else
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(B/2, -m_dRail_HeightTie-H1-H2-pBri->m_dTS/2,nDimDan);
			Dom.DimLineToExtend(pBri->m_dTS,nDimDan, "h",COMMA(pBri->m_dTS));
			Dom.DimLineToExtend(pBri->m_dUnderGroundHeightMortar,nDimDan, "g",COMMA(pBri->m_dUnderGroundHeightMortar));
			Dom.DimLineToExtend(H1,nDimDan, "d",COMMA(H1));
			Dom.DimLineToExtend(pBri->m_dRail_HeightTie,nDimDan, "H1",COMMA(pBri->m_dRail_HeightTie));
			
			Dom.SetDirection("BOTTOM");
			Dom.DimMoveTo(-B/2,-m_dRail_HeightTie-H1-H2-pBri->m_dTS/2, nDimDan);
			str = _T("b' = b + 1.5 x (d + g)");
			if(pData->m_nTypeCalcSlabEffWidth == 0)
				str += _T(" + h");
			Dom.DimLineToExtend(B, nDimDan, str, COMMA(B));
		}
		
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_HIDD);
		Dom.Point(rect.left,rect.bottom+Dom.Always(1));
		
		if(!bForCalc)
		{
			CDPoint xyResult(0, 0);
			if(pBri->IsMatchDosangByDis(dStation, CDPoint(0, 1), dDistRail, xyResult))
				Dom.Move(xyResult);
			else
			{
				CDomyun DomText(pDomP);
				m_pStd->m_pARoadOptionStd->SetEnvType(&DomText,HCAD_TXTG);

				double eleMax = pBri->GetElMaxSlabUpper(dStation, CDPoint(0,1), pBri->m_bUnderGroundRahmen) + DomText.Always(10);

				DomText.TextOut(0, eleMax, _T("���� ������ �����ϴ�. [������ Ⱦ��]�� Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�."));

				*pDomP << DomText;
				continue;
			}
		}

		*pDomP << Dom;
	}
}

void CARcBridgeCalcLoad::WriteSectionDeadLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	long nIndexSmall = 1;
	long nSizeRow = 8;

	CString str = _T("");
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	
	CStringArray strArr;

	BOOL bFirstFrint = TRUE;
	for(int nHDan=0; nHDan<pBri->GetQtyHDan(); nHDan++)
	{  
		CGuardWallRC *pGW = pBri->GetGuardWallByHDan(nHDan);

		if(!pGW || pGW->IsTypeHDanOnlyNone()) continue;
		if(pGW->GetHeight()==0 || pGW->GetWidth()==0) continue;

		if(bFirstFrint)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+99$h$a%d.%d �� Ÿ �� ��$n",nIndexTitle,nIndexMiddle);
			XLText->AddFormatText("$n$h");
			bFirstFrint = FALSE;
		}
		
		str = pBri->GetStringTypeHDan(nHDan, TRUE);
		
		CDomyun *pDom = new CDomyun;

		XLText->AddFormatText("$s+10$h %d) %s$n$r",nIndexSmall++, str);

		if(str.Left(4) == "����")		// ������ Ư���� ���� ����ش�.
		{
			CString strTemp = _T("");
			DrawDeadLoadBodo(pDom, pBri, pGW, nHDan);

			nSizeRow = 2;		//pDom->GetExtHeight()/300;
			XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

			CTwinVectorArray tvArr;
			double dSlopeSlab = pBri->GetSlopeHDan(nHDan);
			if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope && pBri->IsLevelHDanApplyLineSlope(pGW->m_nNumberHDan))		// 31695
				dSlopeSlab = 0;
			pGW->GetTvGuardWallBase(tvArr, FALSE, FALSE, dSlopeSlab);
			
			double dWidth = toM(pBri->GetLengthHDan(nHDan)); 
			double UW_AsconFootway  = Round(toTon(pGW->m_dUnitWeightPave), 3);
			double UW_ConcFootway   = Round(toTon(pGW->m_dUnitWeightInnerFill), 3);
			
			CTwinVector tvL = tvArr.GetAt(5);
			CTwinVector tvR = tvArr.GetAt(9);
			double dL1 = toM(tvL.GetVerLength());
			double dL2 = toM(tvR.GetVerLength());
			
			double dWeightFill = (dL1+dL2)/2 * dWidth * UW_ConcFootway;
			double dWeightPave = toM(pGW->m_dThickPaveFootway) * dWidth * UW_AsconFootway;

			int j=0; for(j=0; j<nSizeRow; j++)
			{
				XLText->AddFormatText("$u");
			}
			if(dL1>0 && dL2>0)
			{
				strTemp.Format("(%s + %s)/2 x %s x %s$m+09=$r%s %s", 
					pData->GetStrDoubleUnitChange(dL1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dL2, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dWidth, UNIT_CGS_M, pData->m_szM), 
					pData->GetStrDoubleUnitChange(UW_ConcFootway, UNIT_CGS_TONF_M3, pData->m_szTonf_M3), pData->GetStrDoubleUnitChange(dWeightFill, UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
				XLText->AddFormatText("$h$t$t%s$r%s$n", GetNumberString(1,NUM_CIRCLE), strTemp);
			}
			strTemp.Format("%s x %s x %s$m+09=$r%s %s",
				pData->GetStrDoubleUnitChange(toM(pGW->m_dThickPaveFootway), UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dWidth, UNIT_CGS_M, pData->m_szM),
				pData->GetStrDoubleUnitChange(UW_AsconFootway, UNIT_CGS_TONF_M3, pData->m_szTonf_M3), pData->GetStrDoubleUnitChange(dWeightPave, UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);
			XLText->AddFormatText("$h$t$t%s$r%s$n", GetNumberString((dL1>0 && dL2>0)? 2:1,NUM_CIRCLE), strTemp);
			XLText->AddFormatText("$u$h$t$t$-13$n");
			XLText->AddFormatText("$h$t$t$r��$m+08$r=$r%s %s$n", pData->GetStrDoubleUnitChange(dWeightFill+dWeightPave, UNIT_CGS_TONF_M, pData->m_szTonf_M), pData->m_szTonf_M);

			for(j=0; j<nSizeRow-(2+3); j++)
			{
				XLText->AddFormatText("$n$h");
			}
		}
		else
		{				
			double dArea = DrawDeadLoadSectionEtc(pDom, pBri, pGW , strArr);
			
			nSizeRow = (long)(pDom->GetExtHeight()/150);
			XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
			long n = strArr.GetSize();
			int j=0; for(j=0; j<nSizeRow-2; j++)
			{
				XLText->AddFormatText("$u");
			}
			
			for(j=0; j<n; j++)
			{
				XLText->AddFormatText("$h$t$t%s$r%s$n", GetNumberString(j+1,NUM_CIRCLE), strArr.GetAt(j));
			}
			XLText->AddFormatText("$u$h$t$t$-11$n");
			XLText->AddFormatText("$h$t$t$r��$m+06$r=$r%s %s$n", pData->GetStrDoubleUnitChange(dArea, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
			
			for(j=0; j<nSizeRow-(n+3); j++)
			{
				XLText->AddFormatText("$n$h");
			}
		}
		*XLText += 1;
	}
}

void CARcBridgeCalcLoad::DrawDeadLoadHDan(CDomyun *pDomP)
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	CDomyun Dom(pDomP);
	double dScaleDim = 80;
	Dom.SetCWnd(m_pStd->m_pView);
	Dom.SetScaleDim(dScaleDim);
	Dom.SetLTScale(dScaleDim*5);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDimExo(2);
	Dom.SetDimVLen(5);
	Dom.SetDimExe(1);

	double dStation = pBri->GetStationOnJijum(0,0);
	CDPoint vAng = CDPoint(0,1);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dStation);
//	DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dStation);	// ������ ��ġ
	if(pBri->m_bUnderGroundRahmen)
	{
		DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dStation,vAng,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,pBri->m_bUnderGroundRahmen);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dStation,CDPoint(0,1),FALSE);

	*pDomP << Dom;
}

void CARcBridgeCalcLoad::DrawDeadLoadBodo(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC *pGW, long nHDan)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	CTwinVectorArray tvArr;
	CDomyun Dom(pDomP);

	double dSta = pBri->GetStationBridgeMid();
	pBri->GetTvGuardWall(pGW, tvArr, dSta, CDPoint(0, 1), FALSE);

	CTwinVector tvL = tvArr.GetAt(5);
	CTwinVector tvR = tvArr.GetAt(9);

	DBStd.DrawTvArray(&Dom, tvArr);
	Dom.LineTo(tvL.m_v1,tvR.m_v2);

	Dom.SetScaleDim(12);
	Dom.SetTextHeight(6);
	Dom.SetDimExo(7);
	Dom.SetDimVLen(10);

	Dom.SetDirection("LEFT");
	CDPoint xy = tvL.GetXyBottom();
	Dom.DimMoveTo(xy.x, xy.y, 0);
	Dom.DimLineTo(tvL.GetVerLength(), 0, COMMA(tvL.GetVerLength()));
	Dom.DimLineToOver(pGW->m_dThickPaveFootway, 0, COMMA(pGW->m_dThickPaveFootway));
	
	Dom.SetDirection("RIGHT");
	xy = tvR.GetXyBottom();
	Dom.DimMoveTo(xy.x, xy.y, 0);
	Dom.DimLineTo(tvR.GetVerLength(), 0, COMMA(tvR.GetVerLength()));
	
	double dLength = tvR.m_v2.x - tvL.m_v1.x;
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(tvL.m_v1.x, MIN(tvL.m_v1.y,tvR.m_v2.y), 0);
	Dom.DimLineTo(dLength, 0, COMMA(dLength));

	*pDomP << Dom;
}

double CARcBridgeCalcLoad::DrawDeadLoadSectionEtc(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC *pGW, CStringArray &strArr)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dArea = 0;
	CDomyun Dom(pDomP);
	CString strRet;

	strArr.RemoveAll();

	Dom.SetScaleDim(12);
	Dom.SetTextHeight(6);
	Dom.SetDimExo(10);
	Dom.SetDimVLen(15);
	
	CRebarPlacing rb;
	CTwinVectorArray tvArr, tvArrResult;
	CTwinVector tvTemp, tv;
	CDoubleArray dArrAngleSu;
	CDoubleArray dDimVer;
	CDoubleArray dDimHor;


	pGW->GetTvGuardWallBase(tvArr, FALSE);
	long i=0; for(i=0; i<tvArr.GetSize(); i++)
	{
		tvTemp = tvArr.GetAt(i);
		if(tvTemp.GetLength() == 0)
		{
			tvArr.RemoveAt(i);
			i--;
		}
	}
	if(tvArr.GetSize() <= 0) return 0;

	tvTemp = tvArr.GetAt(0);
	tv.m_v1 = tvTemp.m_v1;
	tvArr.GetLast(tvTemp);
	tv.m_v2 = tvTemp.m_v2;
	tvArr.Add(tv);

	for(i=0; i<tvArr.GetSize()-1; i++)
	{
		tv = tvArr.GetAt(i);
		double dHor = tv.GetHorLength();
		dDimHor.Add(dHor);
	}
	rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
	
	// �������� ���� /

	long nNum	= 1;
	long nIdx	= 0;
	AhTPADelete(&pBri->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	for(i = 0; i < dArrAngleSu.GetSize(); i++)
	{
		CTwinVectorAreaShape *pArea = new CTwinVectorAreaShape;
		for(long j = 0; j < dArrAngleSu.GetAt(i); j++)
		{
			pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
		}
		pArea->Calculate();
		pArea->m_nNum	= nNum++;
		pBri->m_pArrAreaConc.Add(pArea);
		
		double dH = toM(pArea->m_dHeight);
		double dW1 = toM(pArea->m_dWidthLower);
		double dW2 = toM(pArea->m_dWidthUpper);
		
		HGBOOL bRound = pData->IsLsdDesign() == FALSE;
		if(pArea->m_nTypeShape == SHAPE_TV_RECTANGLE)
		{
			dArea += bRound ? Round(dH*dW1,3) : dH*dW1;
			strRet.Format("%s x %s$m+06$r=$r%s %s",
				pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM),
				pData->GetStrDoubleUnitChange(dH*dW1, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else if(pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID)
		{
			dArea += bRound ? Round((dW1+dW2)/2*dH,3) : (dW1+dW2)/2*dH;
			strRet.Format("(%s + %s)/2 x %s$m+06$r=$r%s %s",
				pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dW2, UNIT_CGS_M, pData->m_szM),
				pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange((dW1+dW2)/2*dH, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else if(pArea->m_nTypeShape == SHAPE_TV_TRIANGLE)
		{
			dArea += bRound ? Round(dH*dW1/2,3) : dH*dW1/2;
			strRet.Format("(%s x %s)/2$m+06$r=$r%s %s", pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), 
				pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dH*dW1/2, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else continue;

		dDimVer.Add(pArea->m_dHeight);
		strArr.Add(strRet);
	}

	// ��ȣ �ٿ��� ��� modified by jaeho 2004.04.09
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawAreaSectionNumber(&Dom, &pBri->m_pArrAreaConc, 1);

	double dDist = 0;
	CDRect rc = Dom.GetExtRect();
	CDPoint xyStt = rc.TopRight();

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	for(i=0; i<dDimVer.GetSize(); i++)
	{
		dDist = dDimVer.GetAt(i);
		Dom.DimLineTo( dDist, 0, COMMA(dDist));
	}

	Dom.SetDirection("BOTTOM");
	dDist = rc.Width();
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	Dom.DimLineTo( -dDist, 0, COMMA(dDist));

	Dom.SetDirection("TOP");
	xyStt = rc.BottomLeft();
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	
	dDist = pGW->m_dW1;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW2;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW3;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW4;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW5;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW6;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));

	dDist = pGW->m_dW7;
	if(dDist > 100) Dom.DimLineTo( dDist, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist, 0, "", COMMA(dDist));
	
	*pDomP << Dom;
	return dArea;
}

CString CARcBridgeCalcLoad::GetStringAreaArray(CDoubleArray &dArrArea)
{
	CARcBridgeDataStd* pData = m_pStd->m_pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();
//	CString strArea = _T("");
	CString strEach = _T("");

	CString strArea = pData->GetStrDoubleUnitChange(dArrArea.GetAt(0), UNIT_CGS_M2, pData->m_szM2);
	long nSizeArea = dArrArea.GetSize();
	
	if(nSizeArea>1)
	{
		strArea = _T("(");
		long i=0; for(i=0; i<nSizeArea; i++)
		{
			double dArea = dArrArea.GetAt(i);
			if(dArea == 0)
				continue;

			if(bLsd)
			{
				CString strTemp(_T(""));
				strTemp = pData->GetStrDoubleUnitChange(dArea, UNIT_CGS_M2, pData->m_szM2);
				if(i<nSizeArea-1)
					strEach.Format("%s + ",strTemp);
				else
					strEach = strTemp;
			}
			else
			{
				if(i<nSizeArea-1)
					strEach.Format("%.3f + ",dArea);
				else
					strEach.Format("%.3f",dArea);
			}

			strArea += strEach;
		}
		strArea += _T(")");
	}

	return strArea;
}

void CARcBridgeCalcLoad::WriteLoadDiagram(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	m_dDimScaleDiagram = (pBri->m_nQtyJigan==1) ? toM(pBri->m_dLengthBridge)*12 : toM(pBri->m_dLengthBridge)*6;

	long nIndexSmall = 1;
	CString strComment = _T("");

	BOOL bRcSlab    = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$a%d.%d �� �� ��$n$n",nIndexTitle,nIndexMiddle);
	// ��������
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_DEAD])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "��������";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramDeadLoad(XLText);
		*XLText += 1;
	}

	// �������
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SOIL] && m_dSoilPressureVert>0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "�������";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramSoilPressureVertical(XLText);
		*XLText += 1;
	}

	// Ȱ����
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "Ȱ����";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			DrawDiagramLiveLoadUnderGroundRahmen(XLText, FALSE);
		}
		else if(pData->IsRailDesign())
			DrawDiagramLiveLoadRail(XLText);
		else
			DrawDiagramLiveLoad(XLText);
		*XLText += 1;
	}

	// �����
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH])
	{
		for(long nCaseW = 0; nCaseW < 2; nCaseW++)
		{
			if(nCaseW == 1 && (pBri->m_bApplyGroundWaterLevel == FALSE || pBri->IsBoxType()==FALSE)) break;
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = "�����";
			if(pBri->m_bApplyGroundWaterLevel && pBri->IsBoxType())
			{
				strComment = nCaseW == 0 ? _T("����� : ���ϼ��� ���� ���") : _T("����� : ���ϼ��� �ִ� ���");
			}
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramHoriPressure(XLText,TRUE, TRUE, nCaseW == 1, FALSE, eGoalEQ_None);
			*XLText += 1;
		}
	}

	// �����
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])
	{
		for(long nCaseW = 0; nCaseW < 2; nCaseW++)
		{
			if(nCaseW == 1 && (pBri->m_bApplyGroundWaterLevel == FALSE || pBri->IsBoxType()==FALSE)) break;
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = "�����";
			if(pBri->m_bApplyGroundWaterLevel && pBri->IsBoxType())
			{
				strComment = nCaseW == 0 ? _T("����� : ���ϼ��� ���� ���") : _T("����� : ���ϼ��� �ִ� ���");
			}
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramHoriPressure(XLText, TRUE, FALSE, nCaseW == 1, FALSE, eGoalEQ_None);
			*XLText += 1;
		}
	}

	if(pBri->m_bApplyGroundWaterLevel && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "���� ����";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramHoriPressure(XLText, FALSE, TRUE, TRUE, FALSE, eGoalEQ_None);
		*XLText += 1;
	}

	if(pBri->m_bApplyGroundWaterLevel && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "���� ����";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramWaterPressureVertical(XLText);
		*XLText += 1;
	}

	// ���μ���
	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = _T("���μ��� (IW)");
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramWaterPressureInterior(XLText);
		*XLText += 1;
	}

	// �µ�����
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TEMP1])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "�µ����� (+)";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
		
		DrawDiagramTemperature(XLText, TRUE);

		*XLText += 1;
	}

	// �µ�����
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TEMP2])
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "�µ����� (-)";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
		
		DrawDiagramTemperature(XLText, FALSE);

		*XLText += 1;
	}

	// ��������
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SHRINK] && !bRcSlab)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "��������";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

		DrawDiagramShrink(XLText);

		*XLText += 1;
	}
	// ����ħ��
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID] && !bRcSlab)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "����ħ��";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

		DrawDiagramSubsidence(XLText);

		*XLText += 1;
	}
	// ��Ŀ
	if(pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "��Ŀ��";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

		DrawDiagramEarthAnchor(XLText, FALSE);

		*XLText += 1;
	}
	// �õ�����
	// ��������
	// �õ� �� ��������(SB)
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SB] && m_bRailDesign)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "�õ� �� ��������(SB)";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
		
		DrawDiagramRailLoadSB(XLText);
		
		*XLText += 1;
	}
	// ��뷹�� ������(LR)
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_LR] && m_bRailDesign)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "��뷹�� ������(LR)";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
		
		DrawDiagramRailLoadLR(XLText);
		
		*XLText += 1;
	}

	long nType = 0;
	HGINT32 icLoadSet = pBri->GetCountUserLoadSetLsd(nType);

	if(icLoadSet > 0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "����� ����";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

		for(HGINT32 ixLoadSet = 0; ixLoadSet < icLoadSet; ++ixLoadSet)
		{
			DrawDiagramUserLoad(XLText, nType, ixLoadSet);
		}
	}
}

void CARcBridgeCalcLoad::DrawDiagramDeadLoad(CXLFormatText *XLText, BOOL bEarthquake, BOOL bDW)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, TRUE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	CString szUnitCGS = bEarthquake? UNIT_CGS_TONF_M : UNIT_CGS_TONF_M2;
	CString szUnitApply = bEarthquake? pData->m_szTonf_M : pData->m_szTonf_M2;

	double dApplyWidth = bEarthquake? m_pStd->GetSlabWidth(-1) : 1.0;
	double dLoadValue = GetValueUnitChange(m_DeadLoadDistribution[bDW? 1 : 0], szUnitCGS, szUnitApply) * dApplyWidth;
	m_pStd->DrawDiagramSlabDistribution(pDom,pFEM,dLoadValue);

	long nT_Text = (pBri->GetCountJijum()==2)? 9:6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	XLText->AddFormatText("$hWd = %s %s$n", pData->GetStrDouble(dLoadValue, szUnitCGS, TRUE), szUnitApply);
	if(bEarthquake)
		XLText->AddFormatText("$h�����ô� ����(%s)�� �ش�Ǵ� �������� ����$n", pData->GetStrDouble(bLsd? frM(dApplyWidth) : dApplyWidth, UNIT_CGS_M, TRUE));
	if(bDW == FALSE)
		XLText->AddFormatText("$h��ü ������ PROGRAM�� �ڵ����$n");
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramLiveLoad(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJointStt = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabStt(pFEM))->m_nJStt;
	long nIdxJointEnd = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabEnd(pFEM))->m_nJEnd;
	long nIndexSmall  = 1;
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;

	CJoint *pJntStt  = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd  = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyStt = frM(CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z));
	CDPoint xyEnd = frM(CDPoint(pJntEnd->m_vPoint.x,pJntEnd->m_vPoint.z));
	CDPoint vX(1,0),vY(0,1);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	if(m_bExistLane)
	{
		// DB ����... //////////////////////////////////////////////////////////////////////////
		CString strLive = "-24";
		if(pData->m_nDegreeBridge==1) strLive = "-18";
		if(pData->m_nDegreeBridge==2) strLive = "-13.5";
		XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),"DB"+strLive);

		CDomyun *pDomDB = new CDomyun;
		pDomDB->SetCWnd(m_pStd->m_pView);
		pDomDB->SetScaleDim(m_dDimScaleDiagram);
		pDomDB->SetLTScale(m_dDimScaleDiagram*5);

		DrawDiagramModeling(pDomDB, TRUE, FALSE);

		CDomyun DomVc(pDomDB);
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomVc,HCAD_DIML);
		DomVc.SetTextHeight(pDomDB->GetTextHeight(FALSE)*4.0);

		double Scale  = DomVc.GetScaleDim();

		double Height = 1500;
		double Dist1  = 2000;
		double Dist2  = 4200;

		CDPoint xy = xyStt + Dist1*vX;
		CHgBaseDrawStd::InsertHCadBlock(&DomVc, "HGMODTRUCK", xy, 100.0, 0, TRUE);

		xy = xyStt + (Dist1+Dist2*3)*vX + Height*0.6*vY;
		CHgBaseDrawStd::InsertHCadBlock(&DomVc, "CGAROW3", xy, Scale/2, 0, TRUE);

		if(pBri->m_nQtyJigan>1)
		{
			xy = xyStt + (Dist1+Dist2*4)*vX + Height*0.6*vY;
			CHgBaseDrawStd::InsertHCadBlock(&DomVc, "CGAROW3", xy, Scale/2, 0, TRUE);
		}

		DomVc.Rotate(xyStt, vDir);

		*pDomDB << DomVc;

		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		double Pr = GetValueUnitChange(m_dPrApply[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Pf = GetValueUnitChange(m_dPfApply[0],UNIT_CGS_TONF,pData->m_szTonf);
		XLText->AddFormatText("$hPr = %6.3f %s$n",Pr,pData->m_szTonf);
		XLText->AddFormatText("$hPf = %6.3f %s$n",Pf,pData->m_szTonf);
		XLText->PopRow();
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->InsertDomyun(pDomDB,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
		
		// DL����... //////////////////////////////////////////////////////////////////////////
		XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),"DL"+strLive);

		CDomyun *pDomDL = new CDomyun;
		pDomDL->SetCWnd(m_pStd->m_pView);
		pDomDL->SetScaleDim(m_dDimScaleDiagram);
		pDomDL->SetLTScale(m_dDimScaleDiagram*5);

		DrawDiagramModeling(pDomDL, TRUE, FALSE);

		m_pStd->m_pARoadOptionStd->SetEnvType(pDomDL,HCAD_DIML);
		pDomDL->SetTextHeight(pDomDL->GetTextHeight(FALSE)*4.0);

		double w  = GetValueUnitChange(m_dDistributionApply,UNIT_CGS_TONF_M,pData->m_szTonf_M);
		double Pm = GetValueUnitChange(m_dPmApply[0],UNIT_CGS_TONF,pData->m_szTonf);
		double Ps = GetValueUnitChange(m_dPsApply[0],UNIT_CGS_TONF,pData->m_szTonf);
		m_pStd->DrawDiagramSlabDistribution(pDomDL,pFEM,w);

		xy = xyStt - 1200*vX;
		CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(pDomDL, pIns1, "RB_NO", "Ps");

		xy = xyStt - 2400*vX;
		CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(pDomDL, pIns2, "RB_NO", "Pm");

		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		XLText->AddFormatText("$h w = %6.3f %s$n",w,pData->m_szTonf_M);
		XLText->AddFormatText("$hPm = %6.3f %s$n",Pm,pData->m_szTonf);
		XLText->AddFormatText("$hPs = %6.3f %s$n",Ps,pData->m_szTonf);
		XLText->PopRow();
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->InsertDomyun(pDomDL,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

	}

	if(bLsd == FALSE && !Compare(m_dLiveLoadFootway,0.0,"="))
	{
		DrawDiagramLiveLoadFootWay(XLText, TRUE, nIndexSmall);
	}

	// ��ũ �� ������ ���� ////////////////////////////////////////////////////////////////////////////////
	DrawDiagramLiveLoadTank(XLText, TRUE, nIndexSmall);
}


void CARcBridgeCalcLoad::DrawDiagramLiveLoadRail(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	if(pData->IsRailDesign2017() == FALSE)
		return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	long nIdxJointStt = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabStt(pFEM))->m_nJStt;
	long nIdxJointEnd = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabEnd(pFEM))->m_nJEnd;
	long nIndexSmall  = 1;
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;
	double Scale  = pDom->GetScaleDim();

	CDPoint vX(1,0),vY(0,1);
	CJoint *pJntStt  = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd  = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyStt = frM(CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z));
	CDPoint xyEnd = frM(CDPoint(pJntEnd->m_vPoint.x,pJntEnd->m_vPoint.z));

	double B = m_bSingleLane ? m_dRail_WidthDistribute : m_dRail_WidthDistribute + toM(m_dRail_DistBetweenRail);
	double I = m_dCoefficientImpact[0];
	double N = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? 1 : 2;

	if(pData->IsKRL_2012())
	{
		double Pd = Round(GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf)*N*(1+I)/B,3);
		double dW = Round(GetValueUnitChange(pData->m_DistributionLoadFront,UNIT_CGS_TONF_M,pData->m_szTonf_M)*N*(1+I)/B,3);
		double dDist = frM(pData->m_RailDistWheel);

		m_pStd->DrawDiagramSlabDistribution(pDom,pFEM,dW, 0,0,TRUE, TRUE);

		CDPoint xy = xyEnd - dDist*vX;
		CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARD", xy, Scale, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(pDom, pIns1, "RB_NO", "Pm");

		xy = xyEnd - dDist*2*vX;
		CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARD", xy, Scale, 0, TRUE);
		CHgBaseDrawStd::SetAttrib(pDom, pIns2, "RB_NO", "Pf");

		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		XLText->AddFormatText("$h w = %6.3f %s$n",dW,pData->m_szTonf_M);
		XLText->AddFormatText("$hPm = %6.3f %s$n",Pd,pData->m_szTonf);
		XLText->AddFormatText("$hPf = %6.3f %s$n",Pd,pData->m_szTonf);
		XLText->PopRow();
	}
	else
	{
		double dLenSlab = xyEnd.x - xyStt.x;
		CDPoint xy = xyStt + dLenSlab/3*vX;

		double Pd = Round(GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf)*N*(1+I)/B, 3);

		long nCountLoad = pData->m_dArrLoadTrainWheel.GetSize();
		for(long ix=0; ix<nCountLoad; ++ix)
		{
			CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARD", xy, Scale, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDom, pIns1, "RB_NO", "Pd");
			xy += frM(pData->m_dArrDistTrainWheel.GetAt(ix)) * vX;

			if(xy.x > xyEnd.x)
				break;
		}
		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		XLText->AddFormatText("$h Pd = %6.3f %s$n",Pd,pData->m_szTonf);
		XLText->PopRow();
	}
	
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramEarthAnchor(CXLFormatText *XLText, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	long nSizeEle = pFEM->GetElementSize();
	CString str;
	CDoubleArray dArrDistAnchor, dArrFroceAnchor;
	for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;

		m_pStd->m_pFrame->GetArrayAnchorForceAndDist(pFEM, pE, dArrFroceAnchor, dArrDistAnchor, bEarthQuake);

		CJoint *pJnt = pFEM->GetJoint(pE->m_nJStt);

		for(long ix = 0; ix < dArrFroceAnchor.GetSize(); ix++)
		{
			CDPoint xy = frM(CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z));
			xy.x += frM(dArrDistAnchor.GetAt(ix));

			str.Format(_T("%.3f"), dArrFroceAnchor.GetAt(ix));
			CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARD", xy, pDom->GetScaleDim()*1.1, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDom, pIns1, "RB_NO", str);
		}
	}


	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}


void CARcBridgeCalcLoad::DrawDiagramSoilPressureVertical(CXLFormatText *XLText, BOOL bWater, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = bEarthQuake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	if(bEarthQuake)
		m_pStd->m_pLoadEQ->DrawDiagramModeling(pDom, TRUE);
	else
		DrawDiagramModeling(pDom, TRUE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
	double dLoadValue = GetValueUnitChange(m_dSoilPressureVert,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	if(dLoadValue > 0)
		m_pStd->DrawDiagramSlabDistribution(pDom,pFEM,dLoadValue);

	double dVertSoilLeft(0), dVertSoilRight(0);
	if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
	{
		dVertSoilLeft = GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(TRUE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * dWidth;
		dVertSoilRight = GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(FALSE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * dWidth;
		if(bWater)
		{
			dVertSoilLeft += GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(TRUE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * dWidth;
			dVertSoilRight += GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(FALSE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * dWidth;
		}
		m_pStd->DrawDiagramProtectionRiseFooting(pDom, pFEM, dVertSoilLeft, dVertSoilRight, 1500);
	}

	long nT_Text = (pBri->GetCountJijum()==2)? 9:6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	if(dLoadValue > 0) XLText->AddFormatText("$hWd = %.3f %s$n",dLoadValue,pData->m_szTonf_M2);
	if(dVertSoilLeft > 0) XLText->AddFormatText("$hWdL = %.3f %s$n",dVertSoilLeft,pData->m_szTonf_M2);
	if(dVertSoilRight > 0) XLText->AddFormatText("$hWdR = %.3f %s$n",dVertSoilRight,pData->m_szTonf_M2);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramWaterPressureVertical(CXLFormatText *XLText, BOOL bLoadAll, BOOL bBuoyancy)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	BOOL bLsd = pData->IsLsdDesign();
	double dLoadValueBuoyancy(0);
	if(bLoadAll || bBuoyancy)
	{
		dLoadValueBuoyancy = GetValueUnitChange(m_dLoadBuoyancy, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		m_pStd->DrawDiagramSlabDistribution(pDom, pFEM, dLoadValueBuoyancy, 0.0, 0.0, FALSE);
	}

	if((bLoadAll || !bBuoyancy) && pBri->m_bExistProtectionRiseFooting)
	{
		double dValue = GetValueUnitChange(m_dLoadWaterVert, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		m_pStd->DrawDiagramProtectionRiseFooting(pDom, pFEM, dValue, dValue, 1500);
	}

	long nT_Text = (pBri->GetCountJijum()==2) ? 9 : 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	if((bLoadAll || !bBuoyancy) && pBri->m_bExistProtectionRiseFooting)
	{
		double dLoadVert = GetValueUnitChange(m_dLoadWaterVert, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		XLText->AddFormatText("$hWu = %s %s$n", pData->GetStrDouble(dLoadVert, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	}

	if(bLoadAll || bBuoyancy)
	{
		XLText->AddFormatText("$hWd = %s %s$n", pData->GetStrDouble(dLoadValueBuoyancy, UNIT_CGS_TONF_M2), pData->m_szTonf_M2, pBri->m_bExistProtectionRiseFooting ? _T("(�λ��������)") : _T("()") );
	}

	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramWaterPressureInterior( CXLFormatText *XLText )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	BOOL bLsd = pData->IsLsdDesign();

	double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dHeightWater = bLsd ? pBri->m_dHeightInteriorWaterPress : toM(pBri->m_dHeightInteriorWaterPress);

	double dLoadValue = dUW_Water * dHeightWater;
	m_pStd->DrawDiagramSlabDistribution(pDom, pFEM, -dLoadValue, 0.0, 0.0, FALSE, FALSE, FALSE);
	
	long nT_Text = (pBri->GetCountJijum()==2) ? 9 : 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	
	XLText->AddFormatText("$hWu = %s %s$n", pData->GetStrDouble(dLoadValue, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);
	
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramHoriPressure(CXLFormatText *XLText, BOOL bSoilPressure, BOOL bBothSide, BOOL bWaterCase, BOOL bEarthquake, EGoalOfEarthQuake eGoalEq)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(bEarthquake && pBri->IsBoxType()==FALSE) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CLineInfo *pLine = pBri->GetLineBase();
	CFEMManage *pFEM = bEarthquake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;

	BOOL bLsd	= pData->IsLsdDesign();
	BOOL bLeft  = (bLsd && bSoilPressure)? !bBothSide : TRUE;
	BOOL bRight = bBothSide;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);
	
	long nSizeEle   = pFEM->GetElementSize();
	double dHStt = pFEM->GetJoint(0)->m_vPointOrigin.z;
	double dHEnd = dHStt + (bLsd? frM(m_pStd->GetElDiffModelingEnd()) : m_pStd->GetElDiffModelingEnd());

	double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
	double dEle_Left  = toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(0).x) - dUT);
	double dEle_Right  = pBri->m_bSlopeLowerSlab ? dEle_Left : toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetSize()-1).x) - dUT);
	double dApplyWidth = bEarthquake? (bLsd? frM(m_pStd->GetSlabWidth(-1)) : m_pStd->GetSlabWidth(-1)) : 1.0;

	// ��������ü
	double Scale = pDom->GetScaleDim();
	double dValueUp = 0;
	double dValueDn = 0;
	double dDepthUp(0);
	double dDepthDn(0);
	CString szHead = bSoilPressure ? _T("Q") : _T("qw");
	CDPoint vX(1,0),xyUp(0,0),xyDn(0,0);
	long nCountLeft = 0, nCountRight = 0;
	if(bLeft) 
	{
		BOOL bFirst = TRUE;
		for(long i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_STTWALL) continue;

			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

			if(bSoilPressure)
			{
				dDepthUp = bLsd? toM(dHStt - pJntI->m_vPoint.z) : (dHStt - pJntI->m_vPoint.z);
				dDepthDn = bLsd? toM(dHStt - pJntJ->m_vPoint.z) : (dHStt - pJntJ->m_vPoint.z);
			}
			else
			{
				dDepthUp = dEle_Left - (bLsd? toM(dHStt - pJntI->m_vPoint.z) : (dHStt - pJntI->m_vPoint.z));
				dDepthDn = dEle_Left - (bLsd? toM(dHStt - pJntJ->m_vPoint.z) : (dHStt - pJntJ->m_vPoint.z));
			}

			dValueUp = bSoilPressure ? GetLoadSoilPressure(dDepthUp, TRUE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthUp);
			dValueDn = bSoilPressure ? GetLoadSoilPressure(dDepthDn, TRUE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthDn);
			if(dValueDn <= 0) continue;

			if(bFirst)
			{
				CDPoint xyNodeI = bLsd? CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z) : frM(CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z));
				xyUp = xyNodeI - frM(dValueUp)*0.4*vX;
				CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyNodeI, Scale, 0, TRUE);
				pDom->LineTo(xyNodeI,xyUp);
				nCountLeft++;
				pDom->TextOut(xyUp - pDom->GetTextHeight()*vX, szHead + COMMA(nCountLeft));
				bFirst = FALSE;
			}

			CDPoint xyNodeJ = bLsd? CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z) : frM(CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z));
			xyDn = xyNodeJ - frM(dValueDn)*0.4*vX;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyNodeJ, Scale, 0, TRUE);
			pDom->LineTo(xyNodeJ,xyDn);
			nCountLeft++;
			pDom->TextOut(xyDn - pDom->GetTextHeight()*vX, szHead + COMMA(nCountLeft));
		}
		pDom->LineTo(xyUp,xyDn);
	}
	if(bRight) 
	{
		BOOL bFirst = TRUE;
		for(long i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_ENDWALL) continue;
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

			if(bSoilPressure)
			{
				dDepthUp = bLsd? toM(dHEnd - pJntI->m_vPoint.z) : (dHEnd - pJntI->m_vPoint.z);
				dDepthDn = bLsd? toM(dHEnd - pJntJ->m_vPoint.z) : (dHEnd - pJntJ->m_vPoint.z);
			}
			else
			{
				dDepthUp = dEle_Right - (bLsd? toM(dHEnd - pJntI->m_vPoint.z) : (dHEnd - pJntI->m_vPoint.z));
				dDepthDn = dEle_Right - (bLsd? toM(dHEnd - pJntJ->m_vPoint.z) : (dHEnd - pJntJ->m_vPoint.z));
			}

			dValueUp = bSoilPressure ? GetLoadSoilPressure(dDepthUp, FALSE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthUp);
			dValueDn = bSoilPressure ? GetLoadSoilPressure(dDepthDn, FALSE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthDn);
			if(dValueDn <= 0) continue;

			if(bFirst)
			{
				CDPoint xyNodeI = bLsd? CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z) : frM(CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z));
				xyUp = xyNodeI + frM(dValueUp)*0.4*vX;
				CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyNodeI, Scale, 0, TRUE);
				pDom->LineTo(xyNodeI,xyUp);
				nCountRight++;
				pDom->TextOut(xyUp + pDom->GetTextHeight()*vX, szHead + COMMA(nCountRight));
				bFirst = FALSE;
			}

			CDPoint xyNodeJ = bLsd? CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z) : frM(CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z));
			xyDn = xyNodeJ + frM(dValueDn)*0.4*vX;
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyNodeJ, Scale, 0, TRUE);
			pDom->LineTo(xyNodeJ,xyDn);
			nCountRight++;
			pDom->TextOut(xyDn + pDom->GetTextHeight()*vX, szHead + COMMA(nCountRight));
		}
		pDom->LineTo(xyUp,xyDn);
	}

	XLText->PushRow();
	long nT_Text = (pBri->GetCountJijum()==2) ? 9 : 6;
	XLText->GoPosition(NO_MOVE, nT_Text);
	*XLText += 1;
	//////////////////////////////////////////////////////////////////////////

	CString szUnitCGS = bEarthquake? UNIT_CGS_TONF_M : UNIT_CGS_TONF_M2;
	CString szUnitApply = bEarthquake? pData->m_szTonf_M : pData->m_szTonf_M2;

	if(bLeft)
	{
		long nPrint = 1;
		long nIndex = 1;
		BOOL bFirst = TRUE;
		for(long ix=0; ix<nSizeEle; ix++)
		{
			CElement *pE = pFEM->GetElement((USHORT)ix);			
			if(pE->m_nFlag!=ELEMENT_STTWALL) continue;
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

			if(bSoilPressure)
			{
				dDepthUp = bLsd? toM(dHStt - pJntI->m_vPoint.z) : (dHStt - pJntI->m_vPoint.z);
				dDepthDn = bLsd? toM(dHStt - pJntJ->m_vPoint.z) : (dHStt - pJntJ->m_vPoint.z);
			}
			else
			{
				dDepthUp = dEle_Left - (bLsd? toM(dHStt - pJntI->m_vPoint.z) : (dHStt - pJntI->m_vPoint.z));
				dDepthDn = dEle_Left - (bLsd? toM(dHStt - pJntJ->m_vPoint.z) : (dHStt - pJntJ->m_vPoint.z));
			}

			dValueUp = GetValueUnitChange(bSoilPressure ? GetLoadSoilPressure(dDepthUp, TRUE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthUp), szUnitCGS, szUnitApply);
			dValueDn = GetValueUnitChange(bSoilPressure ? GetLoadSoilPressure(dDepthDn, TRUE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthDn), szUnitCGS, szUnitApply);
			if(dValueDn <= 0) continue;

			if(bEarthquake)
			{
				dValueUp *= bLsd ? toM(dApplyWidth) : dApplyWidth;
				dValueDn *= bLsd ? toM(dApplyWidth) : dApplyWidth;
			}

			if(bFirst)
			{
				XLText->AddFormatText("$hQ%d = %s %s$n",nIndex++, pData->GetStrDouble(dValueUp, szUnitCGS, TRUE, -1, FALSE, 6), szUnitApply);
				bFirst = FALSE;
			}

			if(nCountLeft <= 7 || (nIndex <= 4 || nIndex > nCountLeft - 3))
			{
				if(nCountLeft > 7 && nIndex == 4)
				{
					XLText->AddFormatText("$h~$n");
				}
				else
				{
					XLText->AddFormatText("$hQ%d = %s %s$n",nIndex, pData->GetStrDouble(dValueDn, szUnitCGS, TRUE, -1, FALSE, 6), szUnitApply);
				}
				nPrint++;
			}
			nIndex++;
		}
		if(bRight)
		{
			for(long ix=0; ix<nPrint; ix++)
			{
				XLText->AddFormatText("$u");
			}
		}
	}

	if(bRight)
	{
		if(bLeft) XLText->GoPosition(NO_MOVE,nT_Text + 10);
		long nIndex = 1;
		BOOL bFirst = TRUE;
		for(long ix=0; ix<nSizeEle; ix++)
		{
			CElement *pE = pFEM->GetElement((USHORT)ix);			
			if(pE->m_nFlag!=ELEMENT_ENDWALL) continue;
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

			if(bSoilPressure)
			{
				dDepthUp = bLsd? toM(dHEnd - pJntI->m_vPoint.z) : (dHEnd - pJntI->m_vPoint.z);
				dDepthDn = bLsd? toM(dHEnd - pJntJ->m_vPoint.z) : (dHEnd - pJntJ->m_vPoint.z);
			}
			else
			{
				dDepthUp = dEle_Right - (bLsd? toM(dHEnd - pJntI->m_vPoint.z) : (dHEnd - pJntI->m_vPoint.z));
				dDepthDn = dEle_Right - (bLsd? toM(dHEnd - pJntJ->m_vPoint.z) : (dHEnd - pJntJ->m_vPoint.z));
			}

			dValueUp = GetValueUnitChange(bSoilPressure ? GetLoadSoilPressure(dDepthUp, FALSE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthUp), szUnitCGS, szUnitApply);
			dValueDn = GetValueUnitChange(bSoilPressure ? GetLoadSoilPressure(dDepthDn, FALSE, bWaterCase, bEarthquake, eGoalEq) : GetLoadWaterPressure(dDepthDn), szUnitCGS, szUnitApply);
			if(dValueDn <= 0) continue;

			if(bEarthquake)
			{
				dValueUp *= bLsd ? toM(dApplyWidth) : dApplyWidth;
				dValueDn *= bLsd ? toM(dApplyWidth) : dApplyWidth;
			}

			if(bFirst)
			{
				XLText->AddFormatText("$hQ%d = %s %s$n",nIndex++, pData->GetStrDouble(dValueUp, szUnitCGS, TRUE, -1, FALSE, 6), szUnitApply);
				bFirst = FALSE;
			}

			if(nCountRight <= 7 || (nIndex <= 4 || nIndex > nCountRight - 3))
			{
				if(nCountRight > 7 && nIndex == 4)
				{
					XLText->AddFormatText("$h~$n");
				}
				else
				{
					XLText->AddFormatText("$hQ%d = %s %s$n",nIndex, pData->GetStrDouble(dValueDn, szUnitCGS, TRUE, -1, FALSE, 6), szUnitApply);
				}
			}
			nIndex++;
		}
	}

	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

	if(bBothSide)
	{
		XLText->AddText("$h$r����� ��� ���Ͻ� ����,���� ����� �������ϵ˴ϴ�.$n");
	}
}

void CARcBridgeCalcLoad::DrawDiagramSubsidence(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;
	
	// scale�� �� ũ�⿡ ���� ������ �����ϱ� �������� 30m�϶��� 1�� ����.
	double dScaleFactor = (pData->IsLsdDesign()? 1000 : 1) * (pBri->GetLengthBridge(FALSE) / 30000);
	BOOL bPiRahmen = pBri->IsPiRahmen();
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram * dScaleFactor);
	pDom->SetLTScale(m_dDimScaleDiagram*5 * dScaleFactor);
	pDom->SetTextHeight(5);
	
	long nCntSub = 0;
	long nJijum = pBri->GetCountJijum();
	long nSizeEle = pFEM->GetElementSize();
	CDPoint xyI(0,0),xyJ(0,0);
	CString str = _T("");
	long i=0; for(i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		CJoint  *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint  *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);
		xyI = frM(CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z));
		xyJ = frM(CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z));
		
		pDom->LineTo(xyI,xyJ);

		if(bPiRahmen && pJntJ->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] == TRUE)
		{
			str.Format("<%d>", nCntSub+1);
			pDom->TextOut(xyJ.x-pDom->Always(5), xyJ.y-pDom->Always(7), str);
			nCntSub++;
		}
		if((pJntJ->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] == TRUE || pJntJ->m_BoolAttrib[BOOL_0_JOINT_SPRING] == TRUE)
			|| (pJntI->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] == TRUE || pJntI->m_BoolAttrib[BOOL_0_JOINT_SPRING] == TRUE))
		{
			if(pJntI->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] == TRUE || pJntI->m_BoolAttrib[BOOL_0_JOINT_SPRING] == TRUE)
			{
				pDom->MoveTo(xyI.x-pDom->Always(3), xyI.y);
				pDom->LineTo(xyI.x+pDom->Always(3), xyI.y);
				str.Format("<%d>", nCntSub+1);
				pDom->TextOut(xyI.x-pDom->Always(5), xyI.y-pDom->Always(7), str);
			}
			else
			{
				pDom->MoveTo(xyJ.x-pDom->Always(3), xyJ.y);
				pDom->LineTo(xyJ.x+pDom->Always(3), xyJ.y);
				str.Format("<%d>", nCntSub+1);
				pDom->TextOut(xyJ.x-pDom->Always(5), xyJ.y-pDom->Always(7), str);
			}

			if(pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO] == (bPiRahmen? nJijum-2:nJijum-1))
			{
				str.Format(_T("%.1f mm"), pBri->m_dDepthApplySubSid);
				pDom->MoveTo(xyJ.x-pDom->Always(3), xyJ.y-pDom->Always(2));
				pDom->LineTo(xyJ.x+pDom->Always(3), xyJ.y-pDom->Always(2));
				pDom->SetDirection("RIGHT");
				pDom->SetDimExo(10);
				pDom->SetDimExe(2);
				pDom->DimMoveTo(xyJ.x-pDom->Always(3), xyJ.y-pDom->Always(2), 0);
				pDom->DimLineToExtend(pDom->Always(2), 0, "", str);
			}
			
			nCntSub++;
		}
	}
	
	pDom->SetCalcExtRect();
	CDRect rc = pDom->GetExtRect();

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);

	CDPoint xyStt(0, 0), xyTemp(0, 0);
	xyStt = rc.BottomLeft();

	pDom->Point(rc.left-5000 ,rc.bottom+3000);
	pDom->Point(rc.right+5000,rc.top-500);
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$r$r�� �������� %.0fmm����ħ�ϰ� �ִ������� ȿ���� ��Ÿ���� �����Ѵ�.$n", frM(m_dLoadSubsidence));
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

	if(nCntSub < 3) return;

	XLText->AddFormatText("$h$u$u$u$u$h");

	BOOL bApplyType = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	if((pBri->m_nSelSubsideType==0 || !bApplyType))
	{
		if(nCntSub == 3)
		{
			XLText->AddFormatText("$h$m+05CASE 1$m+03<1>$m+05CASE 4$m+03<1> + <2>$n");
			XLText->AddFormatText("$h$m+05CASE 2$m+03<2>$m+05CASE 5$m+03<1> + <3>$n");
			XLText->AddFormatText("$h$m+05CASE 3$m+03<3>$m+05CASE 6$m+03<2> + <3>$n");
		}
		else if(nCntSub == 4)
		{
			XLText->AddFormatText("$h$rCASE 1$m+03<1>$m+04CASE 5$m+03<1> + <2>$m+04CASE 11$m+03<1> + <2> + <3>$n");
			XLText->AddFormatText("$h$rCASE 2$m+03<2>$m+04CASE 6$m+03<1> + <3>$m+04CASE 12$m+03<1> + <2> + <4>$n");
			XLText->AddFormatText("$h$rCASE 3$m+03<3>$m+04CASE 7$m+03<1> + <4>$m+04CASE 13$m+03<1> + <3> + <4>$n");
			XLText->AddFormatText("$h$rCASE 4$m+03<4>$m+04CASE 8$m+03<2> + <3>$m+04CASE 14$m+03<2> + <3> + <4>$n");
			XLText->AddFormatText("$h$m+08CASE 9$m+03<2> + <4>$n");
			XLText->AddFormatText("$h$m+08CASE 10$m+03<3> + <4>$n");
		}
		else if(nCntSub == 5)
		{
			XLText->AddFormatText("$h$rCASE 1$m+03<1>$m+04CASE 11$m+03<2> + <4>$m+05CASE 21$m+03<1> + <4> + <5>$n");
			XLText->AddFormatText("$h$rCASE 2$m+03<2>$m+04CASE 12$m+03<2> + <5>$m+05CASE 22$m+03<2> + <3> + <4>$n");
			XLText->AddFormatText("$h$rCASE 3$m+03<3>$m+04CASE 13$m+03<3> + <4>$m+05CASE 23$m+03<2> + <3> + <5>$n");
			XLText->AddFormatText("$h$rCASE 4$m+03<4>$m+04CASE 14$m+03<3> + <5>$m+05CASE 24$m+03<2> + <4> + <5>$n");
			XLText->AddFormatText("$h$rCASE 5$m+03<5>$m+04CASE 15$m+03<4> + <5>$m+05CASE 25$m+03<3> + <4> + <5>$n");
			XLText->AddFormatText("$h$rCASE 6$m+03<1> + <2>$m+04CASE 16$m+03<1> + <2> + <3>$m+05CASE 26$m+03<1> + <2> + <3> + <4>$n");
			XLText->AddFormatText("$h$rCASE 7$m+03<1> + <3>$m+04CASE 17$m+03<1> + <2> + <4>$m+05CASE 27$m+03<1> + <2> + <3> + <5>$n");
			XLText->AddFormatText("$h$rCASE 8$m+03<1> + <4>$m+04CASE 18$m+03<1> + <2> + <5>$m+05CASE 28$m+03<1> + <2> + <4> + <5>$n");
			XLText->AddFormatText("$h$rCASE 9$m+03<1> + <5>$m+04CASE 19$m+03<1> + <3> + <4>$m+05CASE 29$m+03<1> + <3> + <4> + <5>$n");
			XLText->AddFormatText("$h$rCASE 10$m+03<2> + <3>$m+04CASE 20$m+03<1> + <3> + <5>$m+05CASE 30$m+03<2> + <3> + <4> + <5>$n");
		}
		else if(nCntSub == 6)
		{
			XLText->AddFormatText("$hCASE 1$m+03<1>$m+04CASE 22$m+03<1> + <2> + <3>$m+05CASE 43$m+03<1> + <2> + <3> + <5>$n");
			XLText->AddFormatText("$hCASE 2$m+03<2>$m+04CASE 23$m+03<1> + <2> + <4>$m+05CASE 44$m+03<1> + <2> + <3> + <6>$n");
			XLText->AddFormatText("$hCASE 3$m+03<3>$m+04CASE 24$m+03<1> + <2> + <5>$m+05CASE 45$m+03<1> + <2> + <4> + <5>$n");
			XLText->AddFormatText("$hCASE 4$m+03<4>$m+04CASE 25$m+03<1> + <2> + <6>$m+05CASE 46$m+03<1> + <2> + <4> + <6>$n");
			XLText->AddFormatText("$hCASE 5$m+03<5>$m+04CASE 26$m+03<1> + <3> + <4>$m+05CASE 47$m+03<1> + <2> + <5> + <5>$n");
			XLText->AddFormatText("$hCASE 6$m+03<6>$m+04CASE 27$m+03<1> + <3> + <5>$m+05CASE 48$m+03<1> + <3> + <4> + <5>$n");
			XLText->AddFormatText("$hCASE 7$m+03<1> + <2>$m+04CASE 28$m+03<1> + <3> + <6>$m+05CASE 49$m+03<1> + <3> + <4> + <6>$n");
			XLText->AddFormatText("$hCASE 8$m+03<1> + <3>$m+04CASE 29$m+03<1> + <4> + <5>$m+05CASE 50$m+03<1> + <3> + <5> + <6>$n");
			XLText->AddFormatText("$hCASE 9$m+03<1> + <4>$m+04CASE 30$m+03<1> + <4> + <6>$m+05CASE 51$m+03<1> + <4> + <5> + <6>$n");
			XLText->AddFormatText("$hCASE 10$m+03<1> + <5>$m+04CASE 31$m+03<1> + <5> + <6>$m+05CASE 52$m+03<2> + <3> + <4> + <5>$n");
			XLText->AddFormatText("$hCASE 11$m+03<1> + <6>$m+04CASE 32$m+03<2> + <3> + <4>$m+05CASE 53$m+03<2> + <3> + <4> + <6>$n");
			XLText->AddFormatText("$hCASE 12$m+03<2> + <3>$m+04CASE 33$m+03<2> + <3> + <5>$m+05CASE 54$m+03<2> + <3> + <5> + <6>$n");
			XLText->AddFormatText("$hCASE 13$m+03<2> + <4>$m+04CASE 34$m+03<2> + <3> + <6>$m+05CASE 55$m+03<2> + <4> + <5> + <6>$n");
			XLText->AddFormatText("$hCASE 14$m+03<2> + <5>$m+04CASE 35$m+03<2> + <4> + <5>$m+05CASE 56$m+03<3> + <4> + <5> + <6>$n");
			XLText->AddFormatText("$hCASE 15$m+03<2> + <6>$m+04CASE 36$m+03<2> + <4> + <6>$m+05CASE 57$m+03<1>+<2>+<3>+<4>+<5>$n");
			XLText->AddFormatText("$hCASE 16$m+03<3> + <4>$m+04CASE 37$m+03<2> + <5> + <6>$m+05CASE 58$m+03<1>+<2>+<3>+<4>+<6>$n");
			XLText->AddFormatText("$hCASE 17$m+03<3> + <5>$m+04CASE 38$m+03<3> + <4> + <5>$m+05CASE 59$m+03<1>+<2>+<3>+<5>+<6>$n");
			XLText->AddFormatText("$hCASE 18$m+03<3> + <6>$m+04CASE 39$m+03<3> + <4> + <6>$m+05CASE 60$m+03<1>+<2>+<4>+<5>+<6>$n");
			XLText->AddFormatText("$hCASE 19$m+03<4> + <5>$m+04CASE 40$m+03<3> + <5> + <6>$m+05CASE 61$m+03<2>+<3>+<4>+<5>+<6>$n");
			XLText->AddFormatText("$hCASE 20$m+03<4> + <6>$m+04CASE 41$m+03<4> + <5> + <6>$m+05CASE 62$m+03<2>+<3>+<4>+<5>+<6>$n");
			XLText->AddFormatText("$hCASE 21$m+03<5> + <6>$m+04CASE 42$m+03<1> + <2> + <3> + <4>$n");
		}
	}
	else
	{
		XLText->AddFormatText("$h$m+05CASE 1$m+03<1> + <3>$m+05CASE 2$m+03<2> + <4>$n");
	}
}

void CARcBridgeCalcLoad::DrawDiagramRailLoadLR(CXLFormatText *XLText)
{
	
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;
	
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);
	
	DrawDiagramModeling(pDom,TRUE,FALSE);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);
	
	double dLoadLR  = GetValueUnitChange(m_dRail_LoadLR,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	
	long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(pFEM);
	long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(pFEM);
	CJoint *pJntStt = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd = pFEM->GetJoint((USHORT)nIdxJointEnd);
	double dLengthModel = frM(pJntEnd->m_vPoint.x - pJntStt->m_vPoint.x);
	double dCountDraw   = 12;
	double dLengthArrow = dLengthModel/dCountDraw/2;
	
	CDPoint vX(1,0),vY(0,1);
	CDPoint xyStt = CDPoint(frM(pJntStt->m_vPoint.x),frM(pJntStt->m_vPoint.z)) + dLengthArrow*vX;
	CDPoint xyDraw(0,0);
	long i=0; for(i=0; i<(long)dCountDraw; i++)
	{
		xyDraw = xyStt + i*(dLengthArrow*2)*vX + dLengthArrow/2*vY;
		pDom->GiSiArrowExt(xyDraw.x,xyDraw.y,dLengthArrow,dLengthArrow,4,_T(""));
	}
	
	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	XLText->AddFormatText("$hH = %.3f %s$n",dLoadLR,pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramRailLoadSB(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM =	&pBri->m_FemNormal;	// &pBri->m_FemEQ[1];
	
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);
	
	DrawDiagramModeling(pDom, TRUE,FALSE);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);
	
	double dLoadSB  = GetValueUnitChange(m_dRail_LoadSB,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	
	long nIdxJointStt = m_pStd->m_pFrame->GetNumberJointSlabStt(pFEM);
	long nIdxJointEnd = m_pStd->m_pFrame->GetNumberJointSlabEnd(pFEM);
	CJoint *pJntStt = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd = pFEM->GetJoint((USHORT)nIdxJointEnd);
	double dLengthModel = frM(pJntEnd->m_vPoint.x - pJntStt->m_vPoint.x);
	double dCountDraw   = 12;
	double dLengthArrow = dLengthModel/dCountDraw/2;
	
	CDPoint vX(1,0),vY(0,1);
	CDPoint xyStt = CDPoint(frM(pJntStt->m_vPoint.x),frM(pJntStt->m_vPoint.z)) + dLengthArrow*vX;
	CDPoint xyDraw(0,0);
	long i=0; for(i=0; i<(long)dCountDraw; i++)
	{
		xyDraw = xyStt + i*(dLengthArrow*2)*vX + dLengthArrow/2*vY;
		pDom->GiSiArrowExt(xyDraw.x,xyDraw.y,dLengthArrow,dLengthArrow,4,_T(""));
	}
	
	long nT_Text = 6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	XLText->AddFormatText("$hH = %.3f %s$n",dLoadSB,pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramLiveLoadUnderGroundRahmen(CXLFormatText *XLText, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = bEarthQuake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	long nT_Text = (pBri->GetCountJijum()==2)? 9:6;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);
	
	if(bEarthQuake)
		m_pStd->m_pLoadEQ->DrawDiagramModeling(pDom, TRUE);
	else
		DrawDiagramModeling(pDom, TRUE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*4.0);
	
	long nIndexSmall  = 1;

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
	double Wv  = GetValueUnitChange(m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	double Wh  = GetValueUnitChange(m_dLiveLoaUR_Phl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;

	if(bLsd == FALSE)
		XLText->AddFormatText("$s+%02d$h$r$c%s$r���� Ȱ����$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE));

	m_pStd->DrawDiagramSlabDistribution(pDom, pFEM, Wv);

	long nSizeEle   = pFEM->GetElementSize();
	CJoint *pJntStt = pFEM->GetJoint(0);
	
	double Scale = pDom->GetScaleDim();
	CDPoint vX(1,0),xyUp(0,0),xyDn(0,0);
	
	BOOL bFirst = TRUE;
	long i=0; for(i=0; i<nSizeEle; i++)
	{
		if(Wh<=0.0) break;
		CElement *pE = pFEM->GetElement((USHORT)i);			
		if(pE->m_nFlag!=ELEMENT_STTWALL) continue;
		CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);
		CDPoint xyNodeJ = bLsd?CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z) :  frM(CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z));
		xyDn = xyNodeJ - CDPoint(700, 0);
		CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyNodeJ, Scale, 0, TRUE);
		pDom->LineTo(xyNodeJ,xyDn);
		if(bFirst)
		{
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CDPoint xyNodeI = bLsd? CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z) : frM(CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z));
			xyUp = xyNodeI - CDPoint(700, 0);
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYR", xyNodeI, Scale, 0, TRUE);
			pDom->LineTo(xyNodeI,xyUp);
			bFirst = FALSE;
		}
	}
	pDom->LineTo(xyUp,xyDn);
	
	bFirst = TRUE;
	for(i=0; i<nSizeEle; i++)
	{
		if(Wh<=0.0) break;
		CElement *pE = pFEM->GetElement((USHORT)i);			
		if(pE->m_nFlag!=ELEMENT_ENDWALL) continue;
		CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);
		CDPoint xyNodeJ = bLsd? CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z) : frM(CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z));
		xyDn = xyNodeJ + CDPoint(700, 0);
		CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyNodeJ, Scale, 0, TRUE);
		pDom->LineTo(xyNodeJ,xyDn);
		if(bFirst)
		{
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CDPoint xyNodeI = bLsd? CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z) : frM(CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z));
			xyUp = xyNodeI + CDPoint(700, 0);
			CHgBaseDrawStd::InsertHCadBlock(pDom, "HGMODARRONLYL", xyNodeI, Scale, 0, TRUE);
			pDom->LineTo(xyNodeI,xyUp);
			bFirst = FALSE;
		}
	}
	pDom->LineTo(xyUp,xyDn);
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 5;
	XLText->AddFormatText("$h Pvl = %s %s$n", pData->GetStrDouble(Wv, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 6), pData->m_szTonf_M);
	if(Wh>0)
		XLText->AddFormatText("$h Phl = %s %s$n", pData->GetStrDouble(Wh, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 6), pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

	// ����Ȱ����... //////////////////////////////////////////////////////////////////////////
	if(bLsd == FALSE && !Compare(m_dLiveLoadFootway,0.0,"="))
	{
		DrawDiagramLiveLoadFootWay(XLText, TRUE, nIndexSmall);
	}

}

double CARcBridgeCalcLoad::GetWeightHDan(long nHDan, BOOL bDW)
{
	if(nHDan<0 || nHDan>=m_dArrUpperWeight[bDW? 1:0].GetSize()) return 0;

	return m_dArrUpperWeight[bDW? 1:0].GetAt(nHDan);
}

// bUpperOffset �𵨸� ��ܿ� ������ Ÿ�� ���
void CARcBridgeCalcLoad::DrawDiagramModeling(CDomyun *pDomP, BOOL bUpperOffset, BOOL bDrawNLLink)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CDomyun Dom(pDomP);

	double dOffsetBasic = 500;
	double dOffsetSide = 5000;
	double dOffsetUpper = bUpperOffset ? 3000 : dOffsetBasic;

	CDRect rect =m_pStd->DrawDiagramModeling(&Dom,&pBri->m_FemNormal,bDrawNLLink);
	Dom.Point(rect.left - dOffsetSide ,rect.bottom + dOffsetUpper);
	Dom.Point(rect.right + dOffsetSide, rect.top - dOffsetBasic);

	*pDomP << Dom;
}

void CARcBridgeCalcLoad::MakeTableImpactCoeffience(CXLFormatText *XLText, BOOL bYoRyung)
{
	char *Title1[] = { "���� D(m)", "0.15��D��1.0","1.0��D��2.0", "2.0��D��3.0","3.0��D"};
	char *Title2[] = { "��ݰ��", "0.30","0.20", "0.10","0.00"};

	if(!bYoRyung)
	{
		Title1[0] = "���� H(m)";
		Title1[1] = "0.15��H��1.0";
		Title1[2] = "1.0��H��2.0";
		Title1[3] = "2.0��H��3.0";
		Title1[4] = "3.0��H";
	}

	int nWidthJoint[] = {4, 4, 4, 4, 4};
	long nSizeRow = 2;
	long nSizeCol = 5;
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	XLText->AddTable(nSizeRow-1,20-1,1,0);
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title1[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
	}
	long i=0; for(i=1; i<nSizeRow; i++)
	{
		CString str = _T("");
		for(j=0; j<nSizeCol; j++)
		{
			if(j==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title2[j]);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcLoad::CalculateSpanLength()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	m_dReCalcRateSpan = 1;
	m_dSpanLengthDesign = Round(toM(m_pStd->GetLengthModelSpanMin()),3);
	if(pBri->m_nSelectDesignSpanLength==1) m_dSpanLengthDesign = toM(pBri->m_dLengthBridge);
	if(pBri->m_nSelectDesignSpanLength==2) m_dSpanLengthDesign = toM(m_pStd->GetLengthModelSpanMax());


	if(m_pStd->IsSkew())
	{
		CDPoint vAngle = pBri->GetAngleJijum(0);
		double dAngDegree = MIN(vAngle.GetAngleDegree(),180-vAngle.GetAngleDegree());
		double B  = m_pStd->GetSlabWidth();
		double ls = m_dSpanLengthDesign;
		double ln = ls*vAngle.y;
		if((pBri->m_nSelectDesignSpanApply==0 && Compare(dAngDegree,65.0,"<") && ls/B<1.50) || (pBri->m_nSelectDesignSpanApply==3))
		{
			m_bReCalcSpanLength = TRUE;
			m_dSpanLengthDesign = (ls+ln)/2;
			m_dReCalcRateSpan   = m_dSpanLengthDesign/ls;
		}
		else if(pBri->m_nSelectDesignSpanApply==1)
		{
			m_bReCalcSpanLength = FALSE;
			m_dSpanLengthDesign = ls;
		}
		else if(pBri->m_nSelectDesignSpanApply==2)
		{
			m_bReCalcSpanLength = TRUE;
			m_dSpanLengthDesign = ln;
			m_dReCalcRateSpan   = m_dSpanLengthDesign/ls;
		}
	}
	else
	{
		m_bReCalcSpanLength = FALSE;
	}
}

void CARcBridgeCalcLoad::WriteDesignSpanLengthSkew(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	if(m_pStd->IsSkew() == FALSE) return;
	
	CDPoint vAngle = pBri->GetAngleJijum(0);
	double dAngDegree = MIN(vAngle.GetAngleDegree(),180-vAngle.GetAngleDegree());
	double B  = m_pStd->GetSlabWidth();
	double ls = (pBri->m_nSelectDesignSpanLength==0)? Round(toM(m_pStd->GetLengthModelSpanMin()),3) : toM(pBri->m_dLengthBridge);
	if(pBri->m_nSelectDesignSpanLength==2) 
		ls = Round(toM(m_pStd->GetLengthModelSpanMax()),3);
	
	double ln = ls * sin(ToRadian(dAngDegree)); // vAngle.y;
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	
	if(pBri->m_nSelectDesignSpanApply == 0)
	{
		XLText->AddOutLine(3,20,XFL_TABLE_COLOR,1);
		XLText->AddFormatText("$h ���������� �簢 45���̻��� ���� �Ʒ��Ŀ� ���� �����ϴ� ������ �Ѵ�.$n");
		XLText->AddFormatText("$h ��, �簢�� 65���̻��� ���� �簢�� ������ �����Ƿ� ��������� �����Ѵ�.$n");
		XLText->AddFormatText("$h  l = ls                (ls/B �� 1.5),  ls = %.3f m : �������)$n",ls);
		XLText->AddFormatText("$h  l = (ls + ln) / 2     (ls/B �� 1.5),  ln = %.3f m : �����Ÿ�)$n",ln);
		if(Compare(dAngDegree,65.0,">="))
		{
			XLText->AddFormatText("$h �� ������ �簢�� %.0f���̹Ƿ� ��������� �����Ѵ�.$n",dAngDegree);
		}
		else if(ls/B>=1.50)
		{
			XLText->AddFormatText("$h �� ������ �簢�� %.0f���̰�, ls/B(=%.3f) �� 1.5�Ƿ� ��������� �����Ѵ�.$n",dAngDegree,ls/B);
		}
		else
		{
			XLText->AddFormatText("$h �� ������ �簢�� %.1f���̰�, ls/B(=%.3f) �� 1.5�̹Ƿ� $n",dAngDegree,ls/B);
			XLText->AddFormatText("$h    ���������� (%.3f + %.3f) / 2 = %.3f m�� �����Ѵ�.$n",ls,ln,m_dSpanLengthDesign);
			XLText->AddFormatText("$h �� �𵨸� ������ ��ǥ�� �������뿡 ���� �����Ѵ�.$n");
		}
	}
	else if(pBri->m_nSelectDesignSpanApply == 3)
	{
		XLText->AddFormatText("$h    ���������� (ls+ln)/2 = (%.3f + %.3f) / 2 = %.3f m�� �����Ѵ�.$n",ls,ln,m_dSpanLengthDesign);
		XLText->AddFormatText("$h �� �𵨸� ������ ��ǥ�� �������뿡 ���� �����Ѵ�.$n");
		XLText->AddFormatText("$h �� ls = %.3fm : �������, ln = %.3fm : ��������$n", ls, ln);
	}
}

double CARcBridgeCalcLoad::GetLengthSpanLiveLoad()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	// Ȱ���� ���� �������̿� �ɼ� ����
	double dSapn = 0;
	
	if(pBri->m_nSelectLiveLoadSpanLength == 0)
	{
		// ���������� ���� �����ϸ� ������ ������ ���濡�� ������.
		double dSapnMax = m_pStd->GetLenSpanMaxOut();		// ������ ����� �ִ�����
		dSapn = dSapnMax * m_dReCalcRateSpan;
	}
	else
	{
		// ������ �Ǵ� �������� ���
		long bVert = (pBri->m_nSelectLiveLoadSpanLength == 2)? TRUE : FALSE;
		dSapn = m_pStd->GetLenSpanMaxOut(bVert);
	}
	
	return dSapn;
}

void CARcBridgeCalcLoad::WriteCalcLiveLoadLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bFatigue)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen) return;

	long nIndexSmall = 1;
	long nTypeLive = bFatigue? 1 : 0;
	
	CString str = _T("");
	long nCountLane  = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));
	double dPfDesign  = GetValueUnitChange(m_pStd->GetWheelLoadFront(),UNIT_CGS_TONF,pData->m_szTonf);
	double dPmDesign  = GetValueUnitChange(m_pStd->GetWheelLoadMiddle() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dPrDesign  = GetValueUnitChange(m_pStd->GetWheelLoadRear() ,UNIT_CGS_TONF,pData->m_szTonf);
	double dDistribute= GetValueUnitChange(m_pStd->GetWheelLoadDistribution() ,UNIT_CGS_TONF,pData->m_szTonf);
	HGDOUBLE dSpanLengthDesign = min(18000., GetValueUnitChange(m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM));
	HGDOUBLE dWidthBetweenCurb = min(9000., GetValueUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM));
	HGDOUBLE dWidthBetweenCurbLane2 = min(18000., GetValueUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM));
	double dLaneLoad  = GetValueUnitChange(m_dLaneLoad,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	XLText->GoPosition(NO_MOVE,COL_POS_1);

	if(bFatigue)
		XLText->AddFormatText("$n$h$a%d.%d �Ƿ��Ѱ���� Ȱ����(LLFG)$n",nIndexTitle,nIndexMiddle);
	else
		XLText->AddFormatText("$n$h$a%d.%d Ȱ �� ��(LL)$n",nIndexTitle,nIndexMiddle);
	//
	XLText->AddFormatText("$n$h %d)$r�� �� �� ��$m+04$c:$r$me1%s$r$r%s$n",nIndexSmall++,pData->GetStrDoubleUnitChange(m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM), pData->m_szM);

	if(m_pStd->IsSkew())
	{
		WriteDesignSpanLengthSkew(XLText, pBri);
	}

	if(m_bExistLane)
	{
		long nIndexTiny  = 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);

		XLText->AddFormatText("$n$h %d)$r�������� ����$m+04$c:$r$me1%s$r$r%s$n",nIndexSmall++,pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		const HGBOOL bEnableR = pBri->m_nLoadEffeciveW == 0 && pBri->m_nLoadEquiStrip != 0;
		HGDOUBLE dR = GetRForEffectiveWidth(pBri);
		HGDOUBLE dOldR = GetRForEffectiveWidth(pBri, FALSE);

		if (pBri->m_nLoadEffeciveW == 0)
		{
			XLText->AddFormatText("$n$h %d)$r��ȿ�� ���� : %s$n",nIndexSmall++, pData->IsDesignKDS2019() ? _T("��KDS 24 10 11��4.6.4") : _T("�����α� ������ء�4.6.4"));
			if (bEnableR)
			{
				HGDOUBLE dAngle = GetAngleForEffectiveWidth(pBri);
				XLText->AddFormatText("$h$r�簢�� ���� ���$n");
				XLText->AddFormatText("$h$rr$r=$r1.05 - 0.25tan��� 1.00$n");
				XLText->AddFormatText("$h$r$r=$r1.05 - 0.25 x tan(%.f) = %.3f$n", dAngle, dOldR);
				XLText->AddFormatText("$h$r$r=$r%.3f �� 1.00$m+13$c=$r$f3$i$me1%.3f$n", dOldR, dR);
			}

			HGDOUBLE dValue = GetEForEffectiveWidth(pBri, 1);
			XLText->AddFormatText("$h$r�� ���� ���Ͻ�$n");
			if (bEnableR)
			{
				XLText->AddFormatText("$h$rE$r=$r1.2 ( 250 + 0.42 ��(L1W1)) x r$n");
				XLText->AddFormatText("$h$r$r=$r1.2 x (250 + 0.42 x ��(%s x %s)) x %.3f$m+13$c=$r$i$me1%s$r$r%s$n", 
					pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M),
					dR, pData->GetStrDoubleUnitChange(dValue, UNIT_CGS_M, pData->m_szM), pData->m_szM);
			}
			else
			{
				XLText->AddFormatText("$h$rE$r=$r1.2 ( 250 + 0.42 ��(L1W1))$n");
				XLText->AddFormatText("$h$r$r=$r1.2 x (250 + 0.42 x ��(%s x %s))$m+13$c=$r$i$me1%s$r$r%s$n", 
					pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M),
					pData->GetStrDoubleUnitChange(dValue, UNIT_CGS_M, pData->m_szM), pData->m_szM);
			}

			
			dValue = GetEForEffectiveWidth(pBri, nCountLane);
			XLText->AddFormatText("$h$r�� ���� �̻� ���Ͻ�$n");
			if (bEnableR)
			{
				XLText->AddFormatText("$h$rE$r=$r1.1 ( 2,100 + 0.12 ��(L1W1)) x r �� W/NL$n");
				XLText->AddFormatText("$h$rE$r=$r1.1 x ( 2,100 + 0.12 x ��(%s x %s)) x %.3f$n", 
					pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurbLane2, UNIT_CGS_M), dR);
				XLText->AddFormatText("$h$rE$r=$r%s %s W/NL = %s/%d = %s$m+13$c=$r$i$me1%s$r$r%s$n", 
					pData->GetStrDoubleUnitChange(m_dEffectiveWidthLane2, UNIT_CGS_M, pData->m_szM), (m_dEffectiveWidthLane2 <= m_dWidthBetweenCurb / nCountLane)? _T("��") : _T(">"), pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM), nCountLane,
					pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb / nCountLane, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dValue, UNIT_CGS_M, pData->m_szM), pData->m_szM);
			}
			else
			{
				XLText->AddFormatText("$h$rE$r=$r1.1 ( 2,100 + 0.12 ��(L1W1)) �� W/NL$n");
				XLText->AddFormatText("$h$rE$r=$r1.1 x ( 2,100 + 0.12 x ��(%s x %s))$n", 
					pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurbLane2, UNIT_CGS_M));
				XLText->AddFormatText("$h$rE$r=$r%s %s W/NL = %s/%d = %s$m+13$c=$r$i$me1%s$r$r%s$n", 
					pData->GetStrDoubleUnitChange(m_dEffectiveWidthLane2, UNIT_CGS_M, pData->m_szM), (m_dEffectiveWidthLane2 <= m_dWidthBetweenCurb / nCountLane)? _T("��") : _T(">"), pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM), nCountLane,
					pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb / nCountLane, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dValue, UNIT_CGS_M, pData->m_szM), pData->m_szM);
			}


			XLText->AddFormatText("$h$r���⼭$n");
			XLText->AddFormatText("$h$rE = ���(mm)$n");
			XLText->AddFormatText("$h$rL1 = ���� ������� 18,000 mm �� ���� ��(mm)$n");
			XLText->AddFormatText("$h$rW1 = 1 ���� ������ ��� : ���� ������ 9,000 mm �� ���� ��$n");
			XLText->AddFormatText("$h$r     2 ���� �̻� ������ ��� : ���� ������ 18,000 mm �� ���� ��$n");
			XLText->AddFormatText("$h$rW = ���� ����(mm)$n");
			XLText->AddFormatText("$h$rNL = 3.6.1.1���� ������ �������� ��$n");
			XLText->AddFormatText("$h$r�� = �簢(��)$n");
		}
		else
		{
			HGDOUBLE dValue = GetValueUnitChange(1.2, UNIT_CGS_M, pData->m_szM)+0.06*GetValueUnitChange(m_dSpanLengthDesign, UNIT_CGS_M, pData->m_szM);
			XLText->AddFormatText("$n$h %d)$r��ȿ�� ���� : %s$n",nIndexSmall++, pData->IsDesignKDS2019() ? _T("��KDS 24 10 11��4.6.2.4") : _T("�����α� ������ء�4.6.2.4"));
			XLText->AddFormatText("$h$rE$r$c=$r 1,200 + 0.06 x L = %s %s (�� 2,100 %s) , �� E = %s %s$n", pData->GetStrDouble(dValue, UNIT_CGS_M),
				pData->m_szM, pData->m_szM, pData->GetStrDoubleUnitChange(m_dEffectiveWidth, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		}

		XLText->AddFormatText("$n$h %d)$r��ݰ��$n",nIndexSmall++);
		XLText->AddFormatText("$h$ri$r$c=$r%.3f : %s$n$n",m_dCoefficientImpact[nTypeLive], pData->IsDesignKDS2019() ? _T("��KDS 24 12 21��4.4.1 ǥ4.4-1") : _T("�����α� ������ء�3.7.1"));

		XLText->AddFormatText("$s+08$h %d)$r�������� ����$n",nIndexSmall++);
		XLText->AddFormatText("$h$r����������(Wp) : %s %s$n$n", pData->GetStrDoubleUnitChange(pData->m_dDesignRoadWidth, "mm", pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$rN = Wc/Wp = %s / %s = %.1f$n", pData->GetStrDoubleUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM),
			pData->GetStrDoubleUnitChange(pData->m_dDesignRoadWidth, "mm", pData->m_szM), RoundDown(GetValueUnitChange(m_dWidthBetweenCurb, UNIT_CGS_M, pData->m_szM) / pData->m_dDesignRoadWidth, 0));
		XLText->AddFormatText("$h$r�� %d������ �����Ѵ�.$n", nCountLane);
		if(pBri->m_nSelectDesignRoadWidth==0)
			XLText->AddFormatText("$h$rǥ�� �������� 3m���� �����Ѵ�.  %s$n", pData->IsDesignKDS2019() ? _T("��KDS 24 12 21��4.3.1.5") : _T("�����α� ������ء�3.6.1.5"));
		else
		{
			XLText->AddFormatText("$n$h$e���������� W�� %.3f m���� �����Ѵ�.$n",m_dDesignRoadWidth);
			XLText->AddFormatText("$h�� W = Wc / N = %.3f / %d = %.3f �� 3.6 m$n",m_dWidthBetweenCurb,nCountLane,m_dWidthBetweenCurb/(double)(nCountLane));
		}

		// (ARCBRIDGE-2982) �Ƿ��Ѱ�����϶� �󵵿����� ��� ���� ���ϵ��� ����
		if(FALSE && bFatigue)
		{
			XLText->AddFormatText("$n$h %d)$rũ��� ����$n",nIndexSmall++);
			XLText->AddText("$h$r�� ǥ��Ʈ�� ������ 80% ����$n$n");

			XLText->AddFormatText("$n$h %d)$r��$n",nIndexSmall++);
			XLText->AddText("$h$r1���� -> P : 1.00$n");
			XLText->AddText("$h$r2���� -> P : 0.85$n");
			XLText->AddText("$h$r3���� -> P : 0.80$n$n");
		}

		//////////////////////////////////////////////////////////////////////////
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		double dRowFactor = 2.50;
		CDomyun *pDom = new CDomyun;
		DrawLiveLoadLaneCount(pDom,nCountLane);

		CDRect rt = pDom->GetExtRect();
		long nSizeRow = long(RoundUp(MAX(3,m_pStd->GetCountLane(frM(m_dWidthBetweenCurb)))*dRowFactor,0)); // �ּ�3��������...

		if(nSizeRow * (rt.Width()/rt.Height()) > 30) nSizeRow = long(RoundDown(30*(rt.Height()/rt.Width()),0));

		str.Format("$s+%02d$n$h %d)$rȰ���� ����$n",nSizeRow+1,nIndexSmall++);
		XLText->AddFormatText("%s",str);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
		//////////////////////////////////////////////////////////////////////////

		if (pBri->m_bLoadMultiRoadSurConfficient)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$h%s$r���������ϰ��(m) ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
			if (pData->m_nLSDRoadType == 0)
			{
				XLText->AddFormatText("$h$r��ӵ��ΰ� �ƴϹǷ� ���������ϰ���� �������� ����.$n");
			}
			else
			{
				XLText->AddFormatText("$h$r�������μ��� ���� ���������ϰ���� ����.  %s$n", pData->IsDesignKDS2019() ? _T("��KDS 24 12 21��4.3.1.2") : _T("�����α� ������ء�3.6.1.2"));
				if(bFatigue == FALSE)
					MakeTableIRoadWaysFactor(XLText, COL_POS_3);
			}
		}


		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+20$h%s$rǥ��Ʈ������ : %d�������Ͻ� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE), m_nCaseDesignLane[nTypeLive]);
		XLText->GoPosition(NO_MOVE,COL_POS_3);
		for(long nLane=1; nLane<=nCountLane; nLane++)
		{
			double ApplyFactor = GetLaneFactor(pBri, nLane);
			HGINT32 iLane = pBri->m_nLoadEffeciveW == 0 ? 2 : 2*nLane;

			double E  = GetEForEffectiveWidth(pBri, nLane);
			double Pr = (iLane*dPrDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);
			double Pm = (iLane*dPmDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);
			double Pf = (iLane*dPfDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);
			Pr *= ApplyFactor;
			Pm *= ApplyFactor;
			Pf *= ApplyFactor;

			CString szAddFactor = _T("");
			if(bFatigue)
			{
				// (ARCBRIDGE-2982) �Ƿ��Ѱ�����϶� �󵵿����� ��� ���� ���ϵ��� ����
				// (ARCBRIDGE-2496) �Ƿ��Ѱ���� Ȱ����.
				// double dArrFactor[3] = { 1.0, 0.85, 0.8 };
				// double dFactor = (nLane < 3)? dArrFactor[nLane-1] : dArrFactor[2];

				double dFactor = 1.0;
				Pr *= dFactor * 0.8;
				Pm *= dFactor * 0.8;
				Pf *= dFactor * 0.8;

				// szAddFactor.Format(_T(" x %.2f x 0.8"), dFactor);
				szAddFactor = _T(" x 0.8");
			}

			if(ApplyFactor<1.00)
			{
				XLText->AddFormatText("$h%s$r%d���� ���Ͻ� : %.0f%% ����$n",GetNumberString(nLane,NUM_GANA),nLane,ApplyFactor*100);
			}
			else
			{
				XLText->AddFormatText("$h%s$r%d���� ���Ͻ�$n",GetNumberString(nLane,NUM_GANA),nLane);
			}

			if (pBri->m_nLoadEffeciveW == 0)
			{
				if (bEnableR)
				{
					// (ARCBRIDGE-2738) ���⼭�� ��ȿ�� ��� ���� �ʴ´�.
// 					if (nLane == 1)
// 					{
// 						XLText->AddFormatText("$h$r$cE$r$c=$r1.2 x (250 + 0.42 x ��(%s x %s)) x %.3f$m+13$c=$r$me1$i%s$r$r%s$n",
// 							pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M),
// 							dR, pData->GetStrDoubleUnitChange(m_dEffectiveWidth, UNIT_CGS_M, pData->m_szM), pData->m_szM);
// 					}
// 					else
// 					{
// 						XLText->AddFormatText("$h$r$cE$r$c=$r1.1 x ( 2,100 + 0.12 x ��(%s x %s)) x %.3f$m+13$c=$r$me1$i%s$r$r%s$n",
// 							pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurbLane2, UNIT_CGS_M),
// 							dR, pData->GetStrDoubleUnitChange(m_dEffectiveWidthLane2, UNIT_CGS_M, pData->m_szM), pData->m_szM);
// 					}
				}
				else
				{
					if (nLane == 1)
					{
						XLText->AddFormatText("$h$r$cE$r$c=$r1.2 x (250 + 0.42 x ��(%s x %s))$m+13$c=$r$me1$i%s$r$r%s$n",
							pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurb, UNIT_CGS_M),
							pData->GetStrDoubleUnitChange(m_dEffectiveWidth, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					}
					else
					{
						XLText->AddFormatText("$h$r$cE$r$c=$r1.1 x ( 2,100 + 0.12 x ��(%s x %s))$m+13$c=$r$me1$i%s$r$r%s$n",
							pData->GetStrDouble(dSpanLengthDesign, UNIT_CGS_M), pData->GetStrDouble(dWidthBetweenCurbLane2, UNIT_CGS_M),
							pData->GetStrDoubleUnitChange(m_dEffectiveWidthLane2, UNIT_CGS_M, pData->m_szM), pData->m_szM);
					}
				}
			}
			else
			{
				XLText->AddFormatText("$h$r$cEa$r$c=$r%s x %d + %s x %d + %s$m+13$c=$r$me1$i%s$r$r%s$n", pData->GetStrDoubleUnitChange(m_dDesignTruckWidth, UNIT_CGS_M, pData->m_szM),
					nLane, pData->GetStrDoubleUnitChange((m_dDesignRoadWidth-m_dDesignTruckWidth), UNIT_CGS_M, pData->m_szM),nLane-1,pData->GetStrDoubleUnitChange(m_dEffectiveWidth, UNIT_CGS_M, pData->m_szM),
					pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM), pData->m_szM);
			}

			if(ApplyFactor<1.00)
			{
				XLText->AddFormatText("$h$r$cPf$r$c=$r%d x %s x (1 + %.3f) / %s x %.2f%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPfDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM),ApplyFactor, szAddFactor,Pf,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPm$r$c=$r%d x %s x (1 + %.3f) / %s x %.2f%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPmDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM),ApplyFactor, szAddFactor,Pm,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPr$r$c=$r%d x %s x (1 + %.3f) / %s x %.2f%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPrDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM),ApplyFactor, szAddFactor,Pr,pData->m_szTonf);
			}
			else
			{
				XLText->AddFormatText("$h$r$cPf$r$c=$r%d x %s x (1 + %.3f) / %s%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPfDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM), szAddFactor,Pf,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPm$r$c=$r%d x %s x (1 + %.3f) / %s%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPmDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM), szAddFactor,Pm,pData->m_szTonf);
				XLText->AddFormatText("$h$r$cPr$r$c=$r%d x %s x (1 + %.3f) / %s%s$m+13$c=$r$me1$f3$i%.3f$r$r%s$n",
					iLane,pData->GetStrDouble(dPrDesign, UNIT_CGS_M),m_dCoefficientImpact[nTypeLive],pData->GetStrDoubleUnitChange(E, UNIT_CGS_M, pData->m_szM), szAddFactor,Pr,pData->m_szTonf);
			}
		}

		if(bFatigue == FALSE)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$h%s$rǥ����������$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
			XLText->GoPosition(NO_MOVE,COL_POS_3);
			XLText->AddFormatText("$hL = %.3f m �̹Ƿ�$n", m_dSpanLengthDesign);
			if (m_dSpanLengthDesign <= 60)
			{
				XLText->AddFormatText("$hL �� 60m, w = %.3f kN/m$n", toM(dDistribute));
				XLText->AddFormatText("$h$rw = %.3f / %s$m+13$c=$r$me2$f6$i%.6f$r$r$r%s$n", toM(dDistribute), pData->GetStrDoubleUnitChange(3, UNIT_CGS_M, pData->m_szM), dLaneLoad, pData->m_szTonf_M);
			}
			else
			{
				XLText->AddFormatText("$hL > 60m, w = %.3f x (60/L)^0.10 kN/m$n", toM(dDistribute));
				XLText->AddFormatText("$h$rw = %.3f x (60 / %.3f) ^ 0.10 / %s$m+13$c=$r$me2$f6$i%.6f$r$r$r%s$n", 
					toM(dDistribute), m_dSpanLengthDesign, pData->GetStrDoubleUnitChange(3, UNIT_CGS_M, pData->m_szM), dLaneLoad, pData->m_szTonf_M);
			}
		}			

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+%02d$h%s$r�������� Ȱ����$n", nCountLane+2, GetNumberString(nIndexTiny++,NUM_CIRCLE));

		char *Title[] = { "����", "E(m)", "Pr(N)", "Pm(N)", "Pf(N)", "w(N/mm)", "���"};
		int nWidthJoint[] = { 3, 3, 3, 3, 3, 3, 2 };

		nSizeRow = nCountLane + 2;
		long nSizeCol = 7;
		XLText->AddTable(nSizeRow-1,(bFatigue? 17: 20)-1,2,1);

		for(HGINT32 i=0; i<2; i++)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_3);
			long j=0; for(j=0; j<nSizeCol; j++)
			{
				// �Ƿ��Ѱ���¿� Ȱ���߿��� ǥ������ ������ ����.
				if(bFatigue && j == 5)
					continue;

				str = (i==0) ? _T("") : Title[j];
				if(j==0 && i>0)	XLText->AddText("$n$h");
				XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
				if((i==1 && j==0) || (i==1 && j==1) || (i==1 && j==6))
				{
					XLText->AddFormatText("$u$md1$n");
				}
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
			if(i==0)
			{
				long nWidthCell = nWidthJoint[2]+nWidthJoint[3]+nWidthJoint[4];
				XLText->GoPosition(NO_MOVE,COL_POS_3);
				XLText->AddFormatText("$m+0%d",nWidthJoint[0]+nWidthJoint[1]);
				XLText->AddMergeFormatText(0,nWidthCell-1,"$c%s","ǥ��Ʈ������");
				XLText->AddFormatText("$m+0%d",nWidthCell);
				if(bFatigue == FALSE)
				{
					nWidthCell = nWidthJoint[5];
					XLText->AddMergeFormatText(0,nWidthCell-1,"$c%s","ǥ����������");
				}
			}
		}

		for(HGINT32 i=2; i<nSizeRow; i++)
		{
			long nLane = i - 1;
			double ApplyFactor = GetLaneFactor(pBri, nLane);
			HGINT32 iLane = pBri->m_nLoadEffeciveW == 0 ? 2 : 2*nLane;

			double E  = GetEForEffectiveWidth(pBri, nLane);
			double Pr = (iLane*dPrDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);
			double Pm = (iLane*dPmDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);
			double Pf = (iLane*dPfDesign*(1+m_dCoefficientImpact[nTypeLive]))/GetValueUnitChange(E, UNIT_CGS_M, pData->m_szM);

			if(bFatigue)
			{
				// (ARCBRIDGE-2982) �Ƿ��Ѱ�����϶� �󵵿����� ��� ���� ���ϵ��� ����
				// �� ���
				// double dArrFactor[3] = { 1.0, 0.85, 0.8 };
				// double dFactor = (nLane < 3)? dArrFactor[nLane-1] : dArrFactor[2];
				double dFactor = 1.0;
				ApplyFactor *= (dFactor * 0.8);
			}

			Pr *= ApplyFactor;
			Pm *= ApplyFactor;
			Pf *= ApplyFactor;
			
			long j=0; for(j=0; j<nSizeCol; j++)
			{
				if(bFatigue && j == 5)
					continue;

				if(j==0) str.Format("$n$h$me%d$c%d����",nWidthJoint[j]-1,i-1);
				if(j==1) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,E);
				if(j==2) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,Pr);
				if(j==3) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,Pm);
				if(j==4) str.Format("$me%d$c$f3%.3f",nWidthJoint[j]-1,Pf);
				if(j==5) str.Format("$me%d$c$f6%.6f",nWidthJoint[j]-1,dLaneLoad);
				if(j==6) str.Format("$me%d$c%s",nWidthJoint[j]-1,nLane==m_nCaseDesignLane[nTypeLive]? "����":_T(""));
				XLText->AddFormatText("%s",str);
				XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
			}
		}

		if(bFatigue)
		{
			XLText->AddFormatText("$n$h$e�� �Ƿμ���� �ϳ��� �Ƿμ���Ʈ���� ���������� ���������ؼ��� �����ϱ� ������$n");
			XLText->AddFormatText("$h$r$e�ִ�� �߻��Ǵ� ������ �����Ͽ� Ȱ������ �����մϴ�.$n");
		}
		else
		{
			XLText->AddFormatText("$n$h$e�� ǥ��Ʈ�����߰� ǥ��Ʈ������x75%%+ǥ���������� �� �ִ� ���߿����� ����Ѵ�.$n");
			XLText->AddFormatText("$h$r$e%s$n", pData->IsDesignKDS2019() ? _T("��KDS 24 12 21��4.3.1.5") : _T("�����α� ������ء�3.6.1.5"));
		}
	}
}

void CARcBridgeCalcLoad::WriteCalcLiveLoadFootwayLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	if (m_dLiveLoadFootway <= 0)
		return;

// 	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d ��������(PL)$n",nIndexTitle,nIndexMiddle);

	WriteCalcLiveLoadFootway(XLText, pBri);
}

void CARcBridgeCalcLoad::WriteCalcVerticalSoilPressureForLsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd == FALSE)
		return;
	if(pBri->m_bUnderGroundRahmen == FALSE && pBri->IsBoxType() == FALSE)
		return;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d ������� (EV)$n",nIndexTitle,nIndexMiddle);

	if(pBri->m_bUnderGroundRahmen)
	{
		XLText->AddFormatText("$n$h %d)$r�������$n",nIndexSmall++);
		WriteCalcPaveAndUpperSoil(XLText, pBri, COL_POS_2, nIndexTiny, FALSE);
	}
	else if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
	{
		// �λ�����ǿ� �ۿ��ϴ� ����.

		long nColFormula = bLsd? 12 : 10;
		long nColResult	= bLsd? 3 : 2;
		long nDecResult = bLsd? 6 : 3;

		double UW_Ascon  = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt),UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
		double dUwBackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dUwBackFill_W = GetValueUnitChange(pBri->m_dUnitWeigthBackFillWater, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !pBri->m_bApplyGroundWaterLevel) continue;

			for(long istt=0; istt<2; ++istt)
			{
				BOOL bStt = (istt == 0);

				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$n$h %d)$r�������%s(%s)$n", nIndexSmall++, bWater? _T(" ���ϼ��ۿ�") : _T(" "), bStt? _T("����") : _T("����"));

				XLText->GoPosition(NO_MOVE,COL_POS_2);

				double dHeightBackFill = GetHeightBackFillForProtectionRiseFooting(bStt, bWater, FALSE);
				double dWeightBackFill = GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(bStt, bWater, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
				double dHeightBackFill2(0), dWeightBackFill2(0);
				if(bWater)
				{
					dHeightBackFill2 = GetHeightBackFillForProtectionRiseFooting(bStt, TRUE, TRUE);
					dWeightBackFill2 = GetValueUnitChange(GetSoilPressureForProtectionRiseFooting(bStt, TRUE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
				}
				double dWeightTotal		= dWeightBackFill + dWeightBackFill2 + (pBri->m_dTP * UW_Ascon);

				XLText->AddFormatText("$h%s$r�� ��$m+03$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(pBri->m_dTP, UNIT_CGS_M), pData->GetStrDouble(UW_Ascon, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult, 
					pData->GetStrDouble(pBri->m_dTP * UW_Ascon, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);

				XLText->AddFormatText("$n$h%s$r������$m+03$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
					GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dHeightBackFill, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult, 
					pData->GetStrDouble(dWeightBackFill, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);

				if(bWater)
				{
					XLText->AddFormatText("$n$h%s$r������(W)$m+03$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s",
						GetNumberString(nIndexTiny++,NUM_CIRCLE), pData->GetStrDouble(dHeightBackFill2, UNIT_CGS_M), pData->GetStrDouble(dUwBackFill_W, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult, 
						pData->GetStrDouble(dWeightBackFill2, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
				}

				XLText->AddFormatText("$h$-%d", 23);
				XLText->AddFormatText("$n$h$r�� ��$m+03$c:$r$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
					nColFormula, nColResult-1, nDecResult, pData->GetStrDouble(dWeightTotal, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
		}

		// ��������
		if(pBri->m_bApplyGroundWaterLevel)
		{
			for(long istt=0; istt<2; ++istt)
			{
				BOOL bStt = (istt == 0);

				double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
				double dEl_Bot = pBri->GetElevationBoxBottom(bStt? 0 : pBri->m_nQtyJigan) + pBri->m_dPRF_H1; 
				double dEle_Water = pBri->m_dGroundWaterLevel;
				double dHeightWater = MAX(0, dEle_Water - dEl_Bot);

				XLText->GoPosition(NO_MOVE,COL_POS_1);
				XLText->AddFormatText("$n$h %d)$r��������(%s)$m+04$c:$r(%s - %s) x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
					nIndexSmall++, bStt? _T("����") : _T("����"), pData->GetStrDouble(dEle_Water, UNIT_CGS_M), pData->GetStrDouble(dEl_Bot, UNIT_CGS_M), 
					pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult,
					pData->GetStrDouble(dUW_Water * dHeightWater, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
			}
		}
	}

	if(pBri->IsBoxType() && pBri->m_nSelectInteriorWaterPress == 0 && pBri->m_dHeightInteriorWaterPress > 0)
	{
		// ���μ���
		WriteCalcInteriorWaterPress(XLText, pBri, COL_POS_1, nIndexSmall);
	}
}

void CARcBridgeCalcLoad::WriteCalcBouyancyLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	// ���� ������ �ڽ�, ���ϼ��� ����, �������� �η� BP üũ
	if (pBri->IsBoxType() == FALSE)
		return;

	long nIndexSmall = 1;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	if(pBri->m_bApplyGroundWaterLevel || (pBri->m_nSelectInteriorWaterPress == 1 && pBri->m_dHeightInteriorWaterPress > 0))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$h$a%d.%d �� ��(BP)$n",nIndexTitle,nIndexMiddle);
	}
	
	if(pBri->m_bApplyGroundWaterLevel)
	{
		double dEle_Water = pBri->m_dGroundWaterLevel;
		double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
		double dEl_Bot = pBri->GetElevationBoxBottom(0);
		double dHeightWater = MAX(0, dEle_Water - dEl_Bot);

		BOOL bLsd = pData->IsLsdDesign();
		long nColFormula = bLsd? 12 : 10;
		long nColResult	= bLsd? 3 : 2;
		long nDecResult = bLsd? 6 : 3;	

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$h %d)$r�η�$m+04$c:$r(%s - %s) x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n",
			nIndexSmall++, pData->GetStrDouble(dEle_Water, UNIT_CGS_M), pData->GetStrDouble(dEl_Bot, UNIT_CGS_M), 
			pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult,
			pData->GetStrDouble(dUW_Water * dHeightWater, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}

	if (pBri->m_nSelectInteriorWaterPress == 1)
		WriteCalcInteriorWaterPress(XLText, pBri, COL_POS_1, nIndexSmall);
}


double CARcBridgeCalcLoad::GetSoilPressureForProtectionRiseFooting( BOOL bStt, BOOL bWater, BOOL bUnderWater, BOOL bIncludeAscon )
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dHeight = toM(GetHeightBackFillForProtectionRiseFooting(bStt, bWater, bUnderWater));
	double dPressure = ((bWater && bUnderWater)? pBri->m_dUnitWeigthBackFillWater : pBri->m_dUnitWeigthBackFill) * dHeight;

	if(bIncludeAscon)
	{
		dPressure += toM(pBri->m_dTP) * toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt);
	}

	return dPressure;
}

double CARcBridgeCalcLoad::GetHeightBackFillForProtectionRiseFooting( BOOL bStt, BOOL bWater, BOOL bUnderWater )
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;

	double dSta = bStt? pBri->m_dStationBridgeStt : (pBri->m_dStationBridgeStt + pBri->m_dLengthBridge);
	double dHeightBox = pBri->GetHeightBridgeBox(dSta);
	double dHF = pBri->m_dPRF_H1;
	if(bWater)
	{
		double dEle_Water = pBri->m_dGroundWaterLevel;
		double dEle_Bottom = pBri->GetElevationBoxBottom(bStt? 0 : pBri->m_nQtyJigan);
		if(bUnderWater)
		{
			dHeightBox = dEle_Water - dEle_Bottom;
		}
		else
		{
			dHeightBox -= (dEle_Water - dEle_Bottom);
			dHF = 0;
		}
	}

	return dHeightBox - dHF;
}

void CARcBridgeCalcLoad::WriteCalcInteriorWaterPress( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexSmall )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();

	long nColFormula = bLsd ? 12 : 10;
	long nColResult	= bLsd ? 3 : 2;
	long nDecResult = bLsd ? 6 : 3;

	// ���μ���
	double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dHeightWater = bLsd ? pBri->m_dHeightInteriorWaterPress : toM(pBri->m_dHeightInteriorWaterPress);

	XLText->GoPosition(NO_MOVE,nColStt);
	XLText->AddFormatText("$n$h %d)$r���μ���$m+04$c:$r%s x %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n$n",
		nIndexSmall++, pData->GetStrDouble(dHeightWater, UNIT_CGS_M), pData->GetStrDouble(dUW_Water, UNIT_CGS_TONF_M3), nColFormula, nColResult-1, nDecResult,
		pData->GetStrDouble(dUW_Water * dHeightWater, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
}

void CARcBridgeCalcLoad::WriteCalcAlwaysLiveLoadForLsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd == FALSE)
		return;

	double dUwBackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill, UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
	double dKo = m_pStd->GetCofficientForAlwaysLiveLoad();
	double dHeightWallStt = pBri->GetHeightBridgeWall(TRUE);
	double dHeightWallEnd = pBri->GetHeightBridgeWall(FALSE);
	double dEquivalentHeightStt = GetEquivalentHeightWall(dHeightWallStt);
	double dEquivalentHeightEnd = GetEquivalentHeightWall(dHeightWallEnd);
	// CString szConitionStt = GetStringEquivalentHeightCondition(dHeightWallStt);
	// CString szConitionEnd = GetStringEquivalentHeightCondition(dHeightWallEnd);
	double dRoadWaysFactor = GetCalculateRoadWayFactor();
	
	long nColFormula = bLsd? 16 : 14;
	long nColResult	= bLsd? 3 : 2;
	long nDecResult = bLsd? 6 : 3;

	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d ����Ȱ���� (LS)$n",nIndexTitle,nIndexMiddle);

	m_pStd->WriteCalcCifficientKo(pBri->m_nSelectKindCoefficientKo, XLText, nIndexSmall++, COL_POS_1, TRUE);

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h %d)$r����Ȱ����$n",nIndexSmall++);

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h%s$r�������߿� ���� ���� � ����$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	
	XLText->AddFormatText("$h$r- ������ü ����  :  H = %s mm$n", 
		pData->GetStrDouble(dHeightWallStt, UNIT_CGS_M));
	XLText->AddFormatText("$h$r- ���� � ���� :  Heq = %s mm$n", 
		pData->GetStrDouble(dEquivalentHeightStt, UNIT_CGS_M));
	XLText->AddFormatText("$h$r- ������ü ����  :  H = %s mm$n", 
		pData->GetStrDouble(dHeightWallEnd, UNIT_CGS_M));
	XLText->AddFormatText("$h$r- ���� � ���� :  Heq = %s mm$n", 
		pData->GetStrDouble(dEquivalentHeightEnd, UNIT_CGS_M));

	MakeTableIEquivalentHeight(XLText, COL_POS_3);

	WriteLoadWaysFactorLsd(XLText, pBri, COL_POS_2, nIndexTiny);

	double dCorrFactor = pBri->m_bCorrectionFactor_LiveLoadAlways ? pBri->m_dCorrectionFactor_LiveLoadAlways : 1.00;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	for(HGINT32 ixStt=0; ixStt<2; ++ixStt)
	{
		BOOL bStt = (ixStt == 0)? TRUE : FALSE;

		double dLoad = GetValueUnitChange(m_dLoadLsApply[ixStt], UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
		CString szRoadWaysFactor = (pData->m_nLSDRoadType == 0) ? _T("") : _T("x ") + CommaC(dRoadWaysFactor, 2, FALSE);

		XLText->AddFormatText("$n$h %d)$r�ۿ���� : %s$n",nIndexSmall++, bStt? _T("��������ü") : _T("��������ü"));
		XLText->AddFormatText("$h$r$cq$r$c=$rKo x �� x Heq%s$n", (pData->m_nLSDRoadType == 0) ? _T("") : _T(" x m'"));
		XLText->AddFormatText("$h$r$cq$r$c=$r%.3f x %s x %s x %.2f %s$m+%02d$c=$r$me%d$f%d$i%s$m+%02d%s$n", 
			dKo, pData->GetStrDouble(dUwBackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(bStt? dEquivalentHeightStt : dEquivalentHeightEnd, UNIT_CGS_M), dCorrFactor, szRoadWaysFactor, nColFormula, nColResult-1, nDecResult,
			pData->GetStrDouble(dLoad, UNIT_CGS_TONF_M2, TRUE), nColResult, pData->m_szTonf_M2);
	}
	XLText->AddText(_T("$n"));
}

double CARcBridgeCalcLoad::GetEquivalentHeightWall( double dHeightWall )
{
	double dEquivalentHeight(0);

	// (ARCBRIDGE-2955) �� ���̺��� �������� �������������� ����
	if(dHeightWall >= 6000)
		dEquivalentHeight = 600;
	else if(dHeightWall < 1500)
		dEquivalentHeight = 1200;
	else if(dHeightWall >= 3000)
	{
		// 0.9 - 0.1 x (h - 3)
		dEquivalentHeight = 900 - 0.1 * (dHeightWall - 3000);
	}
	else
	{
		// 0.9 + 0.2 x (3 - h)
		dEquivalentHeight = 900 + 0.2 * (3000 - dHeightWall);
	}

	/*
	if(dHeightWall >= 6000)
		dEquivalentHeight = 600;
	else if(dHeightWall > 3000)
		dEquivalentHeight = 900;
	else
		dEquivalentHeight = 1200;*/

	return dEquivalentHeight;
}

CString CARcBridgeCalcLoad::GetStringEquivalentHeightCondition( double dHeightWall )
{
	CString szCondition(_T(""));

	if(dHeightWall >= 6000)
		szCondition = _T("��ü ���� 6000 mm �̻��� ���");
	else if(dHeightWall >= 3000)
		szCondition = _T("��ü ���� 3000 mm �̻�, 6000 mm �̸��� ���");
	else
		szCondition = _T("��ü ���� 1500 mm �̻�, 3000 mm �̸��� ���");

	return szCondition;
}

void CARcBridgeCalcLoad::MakeTableIEquivalentHeight( CXLFormatText *XLText, long nColStt )
{
	XLText->GoPosition(NO_MOVE, nColStt);

	XLText->AddText(_T("$r  ������: ASSHTO Table 3.11.6.4-1$n$h"));

	//ASSHTO Table ����
	XLText->AddTable(3, 13, 1, 0);
	XLText->AddText(hggettext("$h$me6$c��ü����(m) $m+07$me6$cheq(m) $n"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("1.5"), _T("1.2"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("3.0"), _T("0.9"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("��6.0"), _T("0.6"));
}

void CARcBridgeCalcLoad::MakeTableIRoadWaysFactor( CXLFormatText *XLText, long nColStt )
{
	XLText->GoPosition(NO_MOVE, nColStt);

	XLText->AddText(_T("$r  ǥ 3.6.1 ���������ϰ��$n$h"));

	//ASSHTO Table ����
	XLText->AddTable(5, 13, 1, 0);
	XLText->AddText(hggettext("$h$me6$c���������� �� $m+07$me6$c���������ϰ�� 'm' $n"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("1"), _T("1.0"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("2"), _T("0.9"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("3"), _T("0.8"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("4"), _T("0.7"));
	XLText->AddFormatText(_T("$h$me6$c%s $m+07$me6$c%s$n$h"), _T("5�̻�"), _T("0.65"));
}

void CARcBridgeCalcLoad::WriteLoadWaysFactorLsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexTiny )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	//2 ������ ���ϰ�� (m) ����
	CString str(_T("")), sEtc(_T(""));
	double dRoadFactor	= GetCalculateRoadWayFactor();
	long nCountLane = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));

	sEtc = _T("");
	if(dRoadFactor == 0.9 || dRoadFactor == 0.65)	sEtc = _T("��");
	else											sEtc = _T("��");

	if(pData->m_nLSDRoadType == 1) //��ӵ���
		str.Format(_T("�������ΰ� %d �̹Ƿ� ���������ϰ���� %.2f%s ����."), nCountLane , dRoadFactor, sEtc);
	else //�Ϲݵ���
		str.Format(_T("��ӵ��ΰ� �ƴϹǷ� ���������ϰ���� �������� ����."));


	XLText->GoPosition(NO_MOVE, nColStt);
	XLText->AddFormatText("$n$h%s$r���������ϰ��(m) ����$n$h",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	XLText->AddFormatText(_T("$r%s$n$n$h"), str);
	
	if(pData->m_nLSDRoadType == 1)
	{
		MakeTableIRoadWaysFactor(XLText, nColStt+1);
	}
}

void CARcBridgeCalcLoad::WriteCalcTemperatureGapLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	XLText->AddFormatText("$s+07$h$e$a%d.%d �µ��� ����(TG)$n",nIndexTitle,nIndexMiddle);

	XLText->AddFormatText("$h    $r������ؿ��� ���� ���̿� ���� �������� �µ���縦 ���̳�,$n�ؼ��� ���Ǵ� �����ؼ� ���α׷��� �������� �µ���翡 ���ؼ��� �ؼ��� �� �ִ�.$n���� �������� �µ� ��縦 ��� �������οµ����� ġȯ�Ͽ� �����Ѵ�.");

	HGINT32 nIndexSmall = 1, nIndexSubSmall=1;
	// 1) ����
	XLText->AddFormatText("$n$n$h %d)$r����$n",nIndexSmall++);
	XLText->AddFormatText("$h$r%s$r��� �ܸ��� ���� �Ŀ��� ����� �����Ѵ�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE)); 
	XLText->AddFormatText("$n$h$r%s$r�µ��� ���̿� ���� �ٸ����� �־��� ���� z�� ���ؼ��� �����ϴ�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE)); 
	XLText->AddFormatText("$n$h$r%s$r��� Ư���� �µ��� �������̴�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
	XLText->AddFormatText("$n$h$r%s$rT(z)�� �µ��� ���� ��ȭ�� �ǹ��Ѵ�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
	XLText->AddFormatText("$n$h$r%s$rb(z)�� ���� z������ �ܸ��� ���� �ǹ��Ѵ�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
	XLText->AddFormatText("$n$h$r%s$rc�� �ܸ� ���ɿ������� �������� �Ÿ��̴�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
	XLText->AddFormatText("$n$h$r%s$rcp�� �ܸ� �������� ���� �ϸ������ �Ÿ��̴�.",GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
	XLText->AddFormatText("$n$h$r%s$r H = c + cp",GetNumberString(nIndexSubSmall,NUM_CIRCLE));
	// 2) ��� �µ� ��� ġȯ ���
	XLText->AddFormatText("$n$n$h %d)$r��� �µ� ��� ġȯ ���$n",nIndexSmall++);
	XLText->AddFormatText("$h$r- ���ɿ����� ���Ʈ$n");
	XLText->AddFormatText("$h$r$r$p[EQ_TG_MOMENT]$n");
	XLText->AddFormatText("$n$h$r- ���$n");
	XLText->AddFormatText("$h$r$r$p[EQ_TG_AXIAL_FORCE]$n");

	*XLText += 4;
	XLText->AddFormatText("$h$r$s+02���� �µ� ��簡 ���������� ���Ѵٸ�$n");
	XLText->AddFormatText("$h$r$r$p[EQ_TG_T]$n");
	XLText->AddFormatText("$h$m+08$e�̰�, ���⼭ T�� ��� �µ� ��簪�̴�.$n");
	XLText->AddFormatText("$h$r$r$s+02T(z)�� S1�� S2�� �����Ͽ� ����ϸ�,$n");
	XLText->AddFormatText("$h$r$r$p[EQ_TG_S]$n");
	XLText->AddFormatText("$n$h$r$r$s+04����,$n");
	XLText->AddFormatText("$h$r$r$p[EQ_TG_M_T]$n");

	// 3) �µ����
	XLText->AddFormatText("$n$n$n$h %d)$r�µ� ���$n",nIndexSmall++);
	XLText->AddFormatText("$h$r$p[EQ_TG_SLOPE]$n");

	*XLText += 9;
	XLText->AddFormatText("$h$r%s ��ũ��Ʈ�� �����α������� �߻��ϴ� �µ��� ������ȭ�$n", pData->IsDesignKDS2019() ? _T("[KDS 24 12 21 4.11.2 �׸� 4.11-1]") : _T("���α���������ؼ�2015, �׸� 3.14.1"));

	HGDOUBLE dPave = pBri->m_nApplyPavement==0 ? -0.2 : -0.3;
	HGDOUBLE dTLower[3];
	dTLower[0] = pBri->m_dTemperatureSlopeT[0]*dPave;
	dTLower[1] = pBri->m_dTemperatureSlopeT[1]*dPave;
	dTLower[2] = pBri->m_dTemperatureSlopeT[2];

	nIndexSubSmall = 1;
	XLText->AddFormatText("$n$n$h$r%s$r�µ���� ����$n", GetNumberString(nIndexSubSmall++,NUM_CIRCLE));
 	XLText->AddFormatText("$h$r$r - ����� �µ��� ���� ��� T1:%.1f��C, T2:%.1f��C, T3:%.1f��C ����$n", pBri->m_dTemperatureSlopeT[0], pBri->m_dTemperatureSlopeT[1], pBri->m_dTemperatureSlopeT[2]);
	XLText->AddFormatText("$h$r$r - ����� �µ��� ���� ��� %s�� �����Ͽ� T1:%.1f��C, T2:%.1f��C, T3:%.1f��C ����$n", 
		pBri->GetStringLoadTemperatureSlopeApply(), dTLower[0], dTLower[1], dTLower[2] );

	// Table
	long nCntRow  = 16;
	long nCntCol  = 22;

	CStringArray 	sArr;
	sArr.Add("T1 ~ T2"); sArr.Add("T2 ~ 0"); sArr.Add("0 ~ T3");

	HGINT32 index(0);
	HGINT32 nMergeCol(3);

	for(HGINT32 iUpper=0; iUpper<2; ++iUpper)
	{
		index = 0;
		HGDOUBLE dDivisionH (0);
		HGDOUBLE zi = pBri->m_dTS / 2;
		HGDOUBLE dSumS1(0), dSumS2(0);
		XLText->AddFormatText("$n$n$h$r%s$r����� �µ��� %s ���$n", GetNumberString(nIndexSubSmall++,NUM_CIRCLE), (iUpper==0)? _T("����") : _T("����"));
		XLText->AddTable(nCntRow, nCntCol, 1, 3);

		for (HGINT32 ix=0; ix<nCntRow; ++ix)
		{
			if( ix==0 )		// Title
			{
				CStringArray 	sArrTitle;
				sArrTitle.Add("����");
				sArrTitle.Add("zi");
				sArrTitle.Add("zi+1");
				sArrTitle.Add("z");
				sArrTitle.Add("T+(z)");
				sArrTitle.Add("S1");
				sArrTitle.Add("S2");

				for (HGINT32 iy=0; iy<7; ++iy)
				{
					XLText->AddMergeFormatText(0, iy < 5 ? nMergeCol-1 : nMergeCol,_T("$c%s"), sArrTitle.GetAt(iy));
					XLText->AddFormatText(_T("$m+0%d"), iy < 5 ? nMergeCol : nMergeCol+1);
				}
			}
			else
			{
				if( ix == 1 || ix == 6 || ix == 11 )
				{
					dDivisionH = GetTemperatureSlopeH(index);
					if( ix == 11 ) zi = -(pBri->m_dTS / 2) + dDivisionH;
				}

				HGDOUBLE zi_1 = (dDivisionH <= 0) ? zi : zi - (dDivisionH / 5);
				HGDOUBLE z = (dDivisionH <= 0) ? zi : (zi + zi_1) / 2;
				HGDOUBLE Tz = GetTemperatureSlopeTz(index, zi, iUpper==1 ? TRUE : FALSE);
				HGDOUBLE S1 = GetTemperatureSlopeS(index, zi, Tz, FALSE);
				HGDOUBLE S2 = GetTemperatureSlopeS(index, zi, Tz, TRUE);

				XLText->AddMergeFormatText(0, 2,_T("$c%s"), sArr.GetAt(index));
				XLText->AddFormatText(_T("$m+0%d"), 3);

				XLText->AddMergeFormatText(0, 2,_T("%.f"), zi);
				XLText->AddFormatText(_T("$m+0%d"), 3);

				XLText->AddMergeFormatText(0, 2,_T("%.f"), zi_1);
				XLText->AddFormatText(_T("$m+0%d"), 3);

				XLText->AddMergeFormatText(0, 2,_T("%.f"), z);
				XLText->AddFormatText(_T("$m+0%d"), 3);

				XLText->AddMergeFormatText(0, 2,_T("%.2f"), Tz);
				XLText->AddFormatText(_T("$m+0%d"), 3);

				XLText->AddMergeFormatText(0, 3,_T("%s"), pData->GetStrDouble(S1,UNIT_CGS_M) );
				XLText->AddFormatText(_T("$m+0%d"), 4);

				XLText->AddMergeFormatText(0, 3,_T("%s"), pData->GetStrDouble(S2, UNIT_CGS_M) );
				XLText->AddFormatText(_T("$m+0%d"), 4);

				zi -= (dDivisionH / 5);
				dSumS1 += S1;
				dSumS2 += S2;

				if( ix == 5 || ix == 10 )
				{
					++index;
				}
			}

			XLText->AddFormatText("$n$h$r$r");

			if( ix == nCntRow-1 )
			{
				XLText->AddMergeFormatText(0, 14,_T("$c�� ��"));
				XLText->AddFormatText(_T("$m+%d"), 15);

				XLText->AddMergeFormatText(0, 3,_T("%s"), pData->GetStrDouble(dSumS1, UNIT_CGS_M));
				XLText->AddFormatText(_T("$m+0%d"), 4);

				XLText->AddMergeFormatText(0, 3,_T("%s"), pData->GetStrDouble(dSumS2, UNIT_CGS_M));
			}
		}
		XLText->AddFormatText("$n$n$h$r$r");

		// Mg
		XLText->AddFormatText("$h$r$p[EQ_TG_M]");
		XLText->AddFormatText(_T("$m+0%d"), 7);
		XLText->AddMergeFormatText(0, 5,_T("%s N.mm"), pData->GetStrDouble(m_dLoadTgValueMg[iUpper], UNIT_CGS_TONF));
		// T
		XLText->AddFormatText("$n$n$h$r$p[EQ_TG_T]");
		XLText->AddFormatText(_T("$m+0%d"), 6);
		XLText->AddMergeText(1, 0,_T("$c=$r"));
		XLText->AddMergeFormatText(1, 5,_T("%.3f��C"), m_dLoadTgValueT[iUpper]);

		*XLText += 3;
	}
}

void CARcBridgeCalcLoad::WriteCalcAlwaysLoadLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d �� �� �� ��(ES)$n",nIndexTitle,nIndexMiddle);

	HGINT32 nIndexSmall = 1;
	XLText->AddFormatText("$n$h %d)$r������� ���$n",nIndexSmall);
	m_pStd->WriteCalcCifficientKo(0, XLText, nIndexSmall++, COL_POS_1, FALSE);
	
	double dLiveAlways  = GetValueUnitChange(pBri->m_dLiveLoadAlways,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	XLText->AddFormatText("$n$h %d)$r�������� : %s %s$n",nIndexSmall++,pData->GetStrDouble(dLiveAlways,UNIT_CGS_TONF_M2),pData->m_szTonf_M2);

	double dKo = m_pStd->GetCofficientForAlwaysLiveLoad(0);
	double dLoad = dKo * dLiveAlways;
	HGINT32 iDecResult = pData->GetDecimal(UNIT_CGS_TONF_M2);
	XLText->AddFormatText("$n$h %d)$r��� �ۿ����$n",nIndexSmall++);
	XLText->AddFormatText("$h$r$cq$r$c=$r%.3f x %.3f$m+14$c=$r$me2$f%d$i%s$r$r$r%s$n",
		dKo, dLiveAlways, iDecResult, pData->GetStrDouble(dLoad, UNIT_CGS_TONF_M2, TRUE), pData->m_szTonf_M2);

	*XLText += 1;
}

double CARcBridgeCalcLoad::GetCalculateRoadWayFactor()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pData->IsLsdDesign() == FALSE)	return 0;

	//�Ϲ� ������ ��� ������ ����� �������� ���� ���� �� 1
	if(pData->m_nLSDRoadType == 0) 
		return 1;

	double dValue = 0;
	double dRoadFactor[5] = {1.0, 0.9, 0.8, 0.7, 0.65};

	long nCntRoad = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));
	if(nCntRoad == 0)
		return 1;

	switch(nCntRoad)
	{
	case 1:	dValue = dRoadFactor[0]; break;
	case 2: dValue = dRoadFactor[1]; break;
	case 3: dValue = dRoadFactor[2]; break;
	case 4: dValue = dRoadFactor[3]; break;
	default:dValue = dRoadFactor[4]; break;
	}

	return dValue;
}

double CARcBridgeCalcLoad::GetAngleForEffectiveWidth(CRcBridgeRebar *pBri)
{
	if (pBri->m_nLoadEquiStrip == 0)
	{
		return 0;
	}
	else if (pBri->m_nLoadEquiStrip == 2 || pBri->m_nLoadEquiStrip == 3)
	{
		HGINT32 ixJijum = pBri->m_nLoadEquiStrip == 2 ? 0 : pBri->GetCountJijum() - 1;
		CJijum *pJ = pBri->GetJijum(ixJijum);
		if (pJ == NULL)
		{
			return 0;
		}

		return fabs(90 - pJ->m_vAngle.GetAngleDegree());
	}
	
	HGINT32 icJijum = pBri->GetCountJijum();
	if (icJijum == 0)
		return 0;

	HGDOUBLE dAngle = DBL_MAX;
	for (HGINT32 ix = 0; ix < icJijum; ++ix)
	{
		CJijum *pJ =  pBri->GetJijum(ix);
		if (pJ == NULL)
		{
			continue;
		}

		dAngle = min(dAngle,  fabs(90 - pJ->m_vAngle.GetAngleDegree()));
	}

	return dAngle;
}

double CARcBridgeCalcLoad::GetRForEffectiveWidth(CRcBridgeRebar *pBri, HGBOOL bApplyMin)
{
	HGDOUBLE dR = 1;
	if (pBri->m_nLoadEquiStrip != 0)
	{
		HGDOUBLE dAngle = GetAngleForEffectiveWidth(pBri);
		dR = 1.05-0.25*tan(ToRadian(dAngle));

		if (bApplyMin)
		{
			dR = min(1.0, dR);
		}
	}

	return dR;
}

double CARcBridgeCalcLoad::GetEForEffectiveWidth(CRcBridgeRebar *pBri, HGINT32 iLane)
{
	HGDOUBLE dE = 0;
	if (pBri->m_pARcBridgeDataStd->IsLsdDesign() && pBri->m_nLoadEffeciveW == 0)
	{
		if (iLane == 1)
		{
			dE = m_dEffectiveWidth;
		}
		else
		{
			long nCountLane  = m_pStd->GetCountLane(frM(m_dWidthBetweenCurb));
			dE = min(m_dEffectiveWidthLane2, m_dWidthBetweenCurb / nCountLane);
		}
	}
	else
	{
		dE = m_dDesignTruckWidth*iLane + (m_dDesignRoadWidth-m_dDesignTruckWidth)*(iLane-1) + m_dEffectiveWidth;
	}

	return dE;
}

double CARcBridgeCalcLoad::GetLaneFactor(CRcBridgeRebar *pBri, HGINT32 iLane)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dApplyFactor = 1.00;
	if (pData->IsLsdDesign())
	{
		if (pBri->m_bLoadMultiRoadSurConfficient && pData->m_nLSDRoadType == 1)
		{
			if(iLane==1)		dApplyFactor = 1.00;
			else if(iLane==2)	dApplyFactor = 0.90;
			else if(iLane==3)	dApplyFactor = 0.80;
			else if(iLane==4)	dApplyFactor = 0.70;
			else				dApplyFactor = 0.65;
		}
	}
	else
	{
		if(iLane==3) dApplyFactor = 0.90;
		if(iLane>=4) dApplyFactor = 0.75;
	}

	return dApplyFactor;
}

void CARcBridgeCalcLoad::WriteCalcLiveLoadFootway(CXLFormatText * XLText, CRcBridgeRebar * pBri)
{
	CARcBridgeDataStd * pData = pBri->m_pARcBridgeDataStd;

	// ����Ȱ����
	double dWidthSlab = m_pStd->GetSlabWidth();
	double dWidthFootway = 0;
	long nQtyHDan  = pBri->GetQtyHDan();
	CDoubleArray dArrWidth;
	for(int nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(!pGW || !pGW->IsTypeHDanRoadFootway()) continue;
		dWidthFootway += toM(pBri->GetLengthHDan(nHDan));
		dArrWidth.Add(toM(pBri->GetLengthHDan(nHDan)));
	}

	double dDistFoot = GetValueUnitChange(pBri->m_dLiveLoadFootway,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	double dFootLoad = GetValueUnitChange(m_dLiveLoadFootway,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

	if (pData->IsLsdDesign())
	{
		XLText->AddFormatText("$h$r%s %s�� ����������� �����Ѵ�.$n",pData->GetStrDouble(dDistFoot, UNIT_CGS_TONF_M2),pData->m_szTonf_M2);
	}
	else
	{
		XLText->AddFormatText("$h$r: %.3f %s�� ����������� �����Ѵ�.$n",dDistFoot,pData->m_szTonf_M2);
	}
	
	if(dArrWidth.GetSize()==2)
		XLText->AddFormatText("$h$r$cWf$r$c=$r%s x (%s + %s) / %s$m+09$c=$r$me1$i%s$r$r%s$n",
		pData->GetStrDouble(dDistFoot, UNIT_CGS_TONF_M2),pData->GetStrDoubleUnitChange(dArrWidth.GetAt(0), UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(dArrWidth.GetAt(1), UNIT_CGS_M, pData->m_szM),
		pData->GetStrDoubleUnitChange(dWidthSlab, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(dFootLoad, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
	else if(dArrWidth.GetSize()==3)
		XLText->AddFormatText("$h$r$cWf$r$c=$r%s x (%s + %s + %s) / %s$m+11$c=$r$me1$i%s$r$r%s$n",
		pData->GetStrDouble(dDistFoot, UNIT_CGS_TONF_M2),pData->GetStrDoubleUnitChange(dArrWidth.GetAt(0), UNIT_CGS_M, pData->m_szM),
		pData->GetStrDoubleUnitChange(dArrWidth.GetAt(1), UNIT_CGS_M, pData->m_szM),pData->GetStrDoubleUnitChange(dArrWidth.GetAt(2), UNIT_CGS_M, pData->m_szM),
		pData->GetStrDoubleUnitChange(dWidthSlab, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(dFootLoad, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
	else
		XLText->AddFormatText("$h$r$cWf$r$c=$r%s x %s / %s$m+07$c=$r$me1$i%s$r$r%s$n",
		pData->GetStrDouble(dDistFoot, UNIT_CGS_TONF_M2),pData->GetStrDoubleUnitChange(dWidthFootway, UNIT_CGS_M, pData->m_szM),
		pData->GetStrDoubleUnitChange(dWidthSlab, UNIT_CGS_M, pData->m_szM),pData->GetStrDouble(dFootLoad, UNIT_CGS_TONF_M2, TRUE),pData->m_szTonf_M2);
	XLText->AddFormatText("$n");
}

double CARcBridgeCalcLoad::GetLoadLiveAlwaysHori()
{
	CRcBridgeRebar    *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dLiveAlways  = pBri->m_dLiveLoadAlways;
	double dKo = m_pStd->GetCofficientForAlwaysLiveLoad(0);
	double dLoad = dKo * dLiveAlways;

	return dLoad;
}

void CARcBridgeCalcLoad::WriteLoadDiagram_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle )
{
	m_dDimScaleDiagram = (pBri->m_nQtyJigan==1) ? toM(pBri->m_dLengthBridge)*12 : toM(pBri->m_dLengthBridge)*6;

	long nIndexSmall = 1;
	CString strComment = _T("");

	BOOL bRcSlab    = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bUnderGroundWater = pBri->m_bApplyGroundWaterLevel;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$a%d.%d �� �� ��$n$n",nIndexTitle,nIndexMiddle);
	
	// ��������
	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix == 1)? TRUE : FALSE;

		if(pBri->m_plsdManager->IsSelectedLoadDesign(bDW? eLSD_LOADSELECT_DW : eLSD_LOADSELECT_DC))
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bDW? eLSD_LOADCASE_DW1 : eLSD_LOADCASE_DC1, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bDW? eLSD_LOADCASE_DW1 : eLSD_LOADCASE_DC1, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramDeadLoad(XLText, FALSE, bDW);
			*XLText += 1;
		}
	}

	// �������
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV) && m_dSoilPressureVert>0 || (pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting))
	{
		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !bUnderGroundWater) continue;

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
			DrawDiagramSoilPressureVertical(XLText, bWater);
			*XLText += 1;
		}
	}

	// Ȱ����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1))
	{
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, FALSE) + " (" + pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, TRUE) + ")"; 
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

			DrawDiagramLiveLoadUnderGroundRahmen(XLText, FALSE);

			*XLText += 1;
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, FALSE) + " (" + pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_LL1, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

			DrawDiagramLiveLoad_Lsd(XLText, FALSE);

			// (ARCBRIDGE-3232) �Ƿ� ���� ���� ���� -> �Ƿ�Ȱ������ �״�� ����. ������ �����Ƿ� ������ ���
			// if(pBri->m_plsdManager->IsUsedComboKind(eLSD_COMBINE_STRESS))
			{
				XLText->GoPosition(NO_MOVE,COL_POS_1);
				strComment = _T("�Ƿ��Ѱ���� Ȱ����(LLFG)");
				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

				DrawDiagramLiveLoad_Lsd(XLText, TRUE);
			}

			*XLText += 1;
		}
	}

	// ���� Ȱ����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_PL))
	{
		if(!Compare(m_dLiveLoadFootway,0.0,"="))
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);

			DrawDiagramLiveLoadFootWay(XLText, FALSE, nIndexSmall);

			*XLText += 1;
		}
	}

	// ����Ȱ����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LS))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bLeft = (ix == 0);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);
			DrawDiagramLosdLS_ES_Lsd(XLText, bLeft, FALSE, TRUE);

			*XLText += 1;
		}
	}

	// �����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EH))
	{
		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !bUnderGroundWater) continue;

			for(long ixDir=0; ixDir<2; ++ixDir)
			{
				BOOL bLeft = (ixDir == iLEFT);
				ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_EH);

				if(bWater)
					eLoadCase = bLeft? eLSD_LOADCASE_EHW1 : eLSD_LOADCASE_EHW2;
				else
					eLoadCase = bLeft? eLSD_LOADCASE_EH : eLSD_LOADCASE_EH2;

				XLText->GoPosition(NO_MOVE,COL_POS_1);
				strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE) + ")";

				XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
				DrawDiagramHoriPressure(XLText,TRUE, !bLeft, bWater, FALSE, eGoalEQ_None);
				*XLText += 1;
			}
		}
	}

	// ���� ��������
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_ES))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bLeft = (ix == 0);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, FALSE) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, TRUE) + ")";
			XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE + 2,nIndexSmall++,strComment);
			DrawDiagramLosdLS_ES_Lsd(XLText, bLeft, FALSE, FALSE);

			*XLText += 1;
		}
	}

	// ������ - ����
	if(pBri->m_bApplyGroundWaterLevel && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_WA))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1, TRUE) + ")";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramHoriPressure(XLText, FALSE, TRUE, TRUE, FALSE, eGoalEQ_None);
		*XLText += 1;
	}

	// ��������
	if(pBri->m_bApplyGroundWaterLevel && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW, TRUE) + ")";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramWaterPressureVertical(XLText, FALSE, FALSE);
		*XLText += 1;
	}
	
	// �η�
	if(pBri->m_bApplyGroundWaterLevel && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_BP))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_BP) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_BP, TRUE) + ")";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramWaterPressureVertical(XLText, FALSE, TRUE);
		*XLText += 1;
	}

	// ���μ���
	ELSDLoadSelectKind eKindBpi = eLSD_LOADSELECT_EV;
	ELSDLoadCaseKind eLoadCase = eLSD_LOADCASE_EVI; 
	if(pBri->m_nSelectInteriorWaterPress == 1)
	{
		eKindBpi = eLSD_LOADSELECT_BP;
		eLoadCase = eLSD_LOADCASE_BPI;
	}
	else if(pBri->m_nSelectInteriorWaterPress == 2)
	{
		eKindBpi = eLSD_LOADSELECT_WA;
		eLoadCase = eLSD_LOADCASE_WA21;
	}

	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0 && pBri->m_plsdManager->IsSelectedLoadDesign(eKindBpi))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE) + ")";
		XLText->AddFormatText("$s+%02d$h$c%d)$r%s$n",DIAGRAM_ROW_SIZE,nIndexSmall++,strComment);
		DrawDiagramWaterPressureInterior(XLText);
		*XLText += 1;
	}

	// �µ�����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TU))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bPlus = (ix == 0)? TRUE : FALSE;
	
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment =  pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TU21 : eLSD_LOADCASE_TU22) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TU21 : eLSD_LOADCASE_TU22, TRUE) + ")";
			XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
	
			DrawDiagramTemperature(XLText, bPlus, TRUE);
	
			*XLText += 1;
		}
	}
	
	// �µ��� ����
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TG))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bPlus = (ix == 0)? TRUE : FALSE;
	
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			strComment =  pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM) + " (" + pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM, TRUE) + ")";
			XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
	
			DrawDiagramTemperature(XLText, bPlus, FALSE);
	
			*XLText += 1;
		}
	}
	
	// ��������
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SH) && !bRcSlab)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "��������";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
	
		DrawDiagramShrink(XLText);
	
		*XLText += 1;
	}
	
	// ����ħ��
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD) && !bRcSlab)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_SD, FALSE) + pBri->m_plsdManager->GetStringSelectLoadLsd(eLSD_LOADSELECT_SD, TRUE);
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);
	
		DrawDiagramSubsidence(XLText);
	
		*XLText += 1;
	}

	long nType = 0;
	HGINT32 icLoadSet = pBri->GetCountUserLoadSetLsd(nType);

	if(icLoadSet > 0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		strComment = "����� ����";
		XLText->AddFormatText("$s+05h$c%d)$r%s$n",nIndexSmall++,strComment);

		for(HGINT32 ixLoadSet = 0; ixLoadSet < icLoadSet; ++ixLoadSet)
		{
			DrawDiagramUserLoad(XLText, nType, ixLoadSet);
		}
	}
}

void CARcBridgeCalcLoad::DrawDiagramShrink( CXLFormatText *XLText )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	long nIndexTiny  = 1;
	XLText->AddFormatText("$h$r$c%s$r����â���(��)$m+04$c:$r%.3f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),pBri->GetCoefficientExpansion()*pow(10.0,5.0));
	XLText->AddFormatText("$h$r$c%s$r���������(��)$m+04$c:$r%.2f x$r$r$p[Object_10_5]$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),pBri->GetRateDryAndShrinkage()*pow(10.0,5.0));
	XLText->AddFormatText("$h$r$c%s$r%.0f���� �µ��������� ��ȯ�Ͽ� �����Ѵ�.$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),m_dLoadDryAndShrinkage);
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$r��T = -��/�� = -%.6f/%.6f = -%.0f��$n",pBri->GetRateDryAndShrinkage(),pBri->GetCoefficientExpansion(),m_dLoadDryAndShrinkage);
}

void CARcBridgeCalcLoad::DrawDiagramTemperature( CXLFormatText *XLText, BOOL bPlus, BOOL bTU )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	BOOL bLsd		= pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bRcSlab    = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	double dSign = bPlus ? 1 : -1;

	if(bLsd)
	{
		if(bTU)
		{
			XLText->AddFormatText("$h$r��ũ��Ʈ���� �µ���ȭ ����$m+07$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n",m_dLoadTempertureAxial * dSign, pBri->m_dTS);
		}
		else
		{
			XLText->AddFormatText("$h$r�ٴ��� ���ϸ� �µ���$m+06$c:$r %.3f ��$n",m_dLoadTgValueT[bPlus? 0 : 1]);
		}
	}
	else
	{
		if(bRcSlab)
		{
			XLText->AddFormatText("$h$r�ٴ��� ���ϸ� �µ���$m+06$c:$r %.0f ��$n",m_dLoadTempertureUpDown);
		}
		else
		{
			long nIndexTiny  = 1;
			XLText->AddFormatText("$h$r$c%s$r��ũ��Ʈ���� �µ���ȭ ����$m+07$c:$r %.0f �� (�ܸ��ּ�ġ�� : %.0f mm)$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),(m_dLoadTempertureAxial * dSign), pBri->m_dTS);
			XLText->AddFormatText("$h$r$c%s$r�ٴ��� ���ϸ� �µ���$m+06$c:$r %.0f ��$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),(m_dLoadTempertureUpDown * dSign));
		}
	}
}

void CARcBridgeCalcLoad::DrawDiagramLiveLoadFootWay( CXLFormatText *XLText, BOOL bTitle, long &nIndexSmall, BOOL bEarthQuake )
{

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = bEarthQuake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;
	
	// ����Ȱ����... //////////////////////////////////////////////////////////////////////////
	if(!Compare(m_dLiveLoadFootway,0.0,"="))
	{
		if(bTitle)
			XLText->AddFormatText("$s+%02d$h$r$c%s$r���� Ȱ����$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE));

		CDomyun *pDomFW = new CDomyun;
		pDomFW->SetCWnd(m_pStd->m_pView);
		pDomFW->SetScaleDim(m_dDimScaleDiagram);
		pDomFW->SetLTScale(m_dDimScaleDiagram*5);

		if(bEarthQuake)
			m_pStd->m_pLoadEQ->DrawDiagramModeling(pDomFW, TRUE);
		else
			DrawDiagramModeling(pDomFW, TRUE, FALSE);

		m_pStd->m_pARoadOptionStd->SetEnvType(pDomFW,HCAD_DIML);
		pDomFW->SetTextHeight(pDomFW->GetTextHeight(FALSE)*4.0);

		double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
		double w  = GetValueUnitChange(m_dLiveLoadFootway,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
		m_pStd->DrawDiagramSlabDistribution(pDomFW,pFEM,w);

		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		XLText->AddFormatText("$h w = %s %s$n", pData->GetStrDouble(w, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 6),pData->m_szTonf_M2);
		XLText->PopRow();
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->InsertDomyun(pDomFW,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
	}
}

void CARcBridgeCalcLoad::DrawDiagramLiveLoad_Lsd( CXLFormatText *XLText, BOOL bFatigue, BOOL bEarthQuake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = bEarthQuake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;
	
	CElement *eEleStt = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabStt(pFEM));
	CElement *eEleEnd = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabEnd(pFEM));
	if(eEleStt == NULL || eEleEnd == NULL)
		return;

	long nTypeLive = bFatigue? 1 : 0;

	long nIdxJointStt = eEleStt->m_nJStt; //pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabStt(pFEM))->m_nJStt;
	long nIdxJointEnd = eEleEnd->m_nJEnd; //pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabEnd(pFEM))->m_nJEnd;
	long nIndexSmall  = 1;
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;

	CJoint *pJntStt  = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd  = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyStt = CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z);
	CDPoint xyEnd = CDPoint(pJntEnd->m_vPoint.x,pJntEnd->m_vPoint.z);
	CDPoint vX(1,0),vY(0,1);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	if(m_bExistLane)
	{
		// DB ����... //////////////////////////////////////////////////////////////////////////
		CString strLive = bFatigue? _T("KLTFG (ǥ��Ʈ������ - �Ƿ��Ѱ���� Ȱ����)") : _T("KLTRG (ǥ��Ʈ������)"); // pData->GetStringLoadDB();
		XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),strLive);

		CDomyun *pDomDB = new CDomyun;
		pDomDB->SetCWnd(m_pStd->m_pView);
		pDomDB->SetScaleDim(m_dDimScaleDiagram);
		pDomDB->SetLTScale(m_dDimScaleDiagram*5);

		if(bEarthQuake)
			m_pStd->m_pLoadEQ->DrawDiagramModeling(pDomDB, TRUE);
		else
			DrawDiagramModeling(pDomDB, TRUE, FALSE);

		CDomyun DomVc(pDomDB);
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomVc,HCAD_DIML);
		DomVc.SetTextHeight(pDomDB->GetTextHeight(FALSE)*4.0);

		double Scale  = DomVc.GetScaleDim();

		double Height = 1500;
		double Dist1  = 2000;

		CDPoint xy = xyStt + Dist1*vX;
		CHgBaseDrawStd::InsertHCadBlock(&DomVc, "HGMODTRUCK", xy, 100.0, 0, TRUE);

		xy = xyStt + (Dist1+3600)*vX + Height*0.6*vY;
		CHgBaseDrawStd::InsertHCadBlock(&DomVc, "CGAROW3", xy, Scale/2, 0, TRUE);

//		if(pBri->m_nQtyJigan>1)
		{
			xy = xyStt + (Dist1+1200)*vX + Height*0.6*vY;
			CHgBaseDrawStd::InsertHCadBlock(&DomVc, "CGAROW3", xy, Scale/2, 0, TRUE);
		}

		DomVc.Rotate(xyStt, vDir);

		*pDomDB << DomVc;

		XLText->PushRow();
		XLText->GoPosition(NO_MOVE,nT_Text);
		*XLText += 4;
		double Pr = GetValueUnitChange(m_dPrApply[nTypeLive],UNIT_CGS_TONF_M,pData->m_szTonf_M);
		double Pm = GetValueUnitChange(m_dPmApplyTruck[nTypeLive],UNIT_CGS_TONF_M,pData->m_szTonf_M);
		double Pf = GetValueUnitChange(m_dPfApply[nTypeLive],UNIT_CGS_TONF_M,pData->m_szTonf_M);
		XLText->AddFormatText("$hPr = %6.3f %s$n",Pr,pData->m_szTonf_M);
		XLText->AddFormatText("$hPm = %6.3f %s$n",Pm,pData->m_szTonf_M);
		XLText->AddFormatText("$hPm = %6.3f %s$n",Pm,pData->m_szTonf_M);
		XLText->AddFormatText("$hPf = %6.3f %s$n",Pf,pData->m_szTonf_M);
		XLText->PopRow();
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->InsertDomyun(pDomDB,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

		if(bFatigue == FALSE)
		{
			// DL����... //////////////////////////////////////////////////////////////////////////
			strLive = _T("KLLAN (ǥ��Ʈ������x75%+ǥ����������)"); //pData->GetStringLoadDB();
			XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),strLive);

			CDomyun *pDomDL = new CDomyun;
			pDomDL->SetCWnd(m_pStd->m_pView);
			pDomDL->SetScaleDim(m_dDimScaleDiagram);
			pDomDL->SetLTScale(m_dDimScaleDiagram*5);

			DrawDiagramModeling(pDomDL, TRUE, FALSE);

			m_pStd->m_pARoadOptionStd->SetEnvType(pDomDL,HCAD_DIML);
			pDomDL->SetTextHeight(pDomDL->GetTextHeight(FALSE)*4.0);

			double w  = GetValueUnitChange(m_dLaneLoad,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

			m_pStd->DrawDiagramSlabDistribution(pDomDL,pFEM,w);

			xy = xyStt + 1500*vX;
			CObInsert *pIns1 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDomDL, pIns1, "RB_NO", "Pf");

			xy = xyStt + (1500 + 3600)*vX;
			CObInsert *pIns2 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDomDL, pIns2, "RB_NO", "Pm");

			xy = xyStt + (1500 + 3600 + 1200)*vX;
			CObInsert *pIns3 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDomDL, pIns3, "RB_NO", "Pm");

			xy = xyStt + (1500 + 3600 + 1200 + 7200)*vX;
			CObInsert *pIns4 = CHgBaseDrawStd::InsertHCadBlock(pDomDL, "HGMODARD", xy, Scale*1.2, 0, TRUE);
			CHgBaseDrawStd::SetAttrib(pDomDL, pIns4, "RB_NO", "Pr");

			XLText->PushRow();
			XLText->GoPosition(NO_MOVE,nT_Text);
			*XLText += 4;
			XLText->AddFormatText("$h w = %8.6f %s$n",w,pData->m_szTonf_M2);
			XLText->AddFormatText("$hPr = %6.3f %s$n",Pr * 0.75,pData->m_szTonf_M);
			XLText->AddFormatText("$hPm = %6.3f %s$n",Pm * 0.75,pData->m_szTonf_M);
			XLText->AddFormatText("$hPm = %6.3f %s$n",Pm * 0.75,pData->m_szTonf_M);
			XLText->AddFormatText("$hPf = %6.3f %s$n",Pf * 0.75,pData->m_szTonf_M);
			XLText->PopRow();
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->InsertDomyun(pDomDL,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
		}
	}

	if(bFatigue)
	{
		// ��ũ �� ������ ���� ////////////////////////////////////////////////////////////////////////////////
		DrawDiagramLiveLoadTank(XLText, TRUE, nIndexSmall);
	}
}

void CARcBridgeCalcLoad::DrawDiagramLiveLoadTank( CXLFormatText *XLText, BOOL bTitle, long &nIndexSmall )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJointStt = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabStt(pFEM))->m_nJStt;
	long nIdxJointEnd = pFEM->GetElement((USHORT)m_pStd->m_pFrame->GetNumberElementSlabEnd(pFEM))->m_nJEnd;
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;

	CJoint *pJntStt  = pFEM->GetJoint((USHORT)nIdxJointStt);
	CJoint *pJntEnd  = pFEM->GetJoint((USHORT)nIdxJointEnd);
	CDPoint xyStt = bLsd? CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z) : frM(CDPoint(pJntStt->m_vPoint.x,pJntStt->m_vPoint.z));
	CDPoint xyEnd = bLsd? CDPoint(pJntEnd->m_vPoint.x,pJntEnd->m_vPoint.z) : frM(CDPoint(pJntEnd->m_vPoint.x,pJntEnd->m_vPoint.z));
	double Scale  = m_dDimScaleDiagram;
	CDPoint vX(1,0),vY(0,1);
	
	if(pBri->IsApplyTankLoad() == FALSE)
		return;

	if(bTitle)
		XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),"��ũ ����");

	CDomyun *pDomTank = new CDomyun;
	pDomTank->SetCWnd(m_pStd->m_pView);
	pDomTank->SetScaleDim(m_dDimScaleDiagram);
	pDomTank->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDomTank, TRUE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomTank,HCAD_DIML);
	pDomTank->SetTextHeight(pDomTank->GetTextHeight(FALSE)*4.0);

	double dDist = 2000;
	double dLength  = frM(pData->m_dTank_TankLength);

	double P = GetValueUnitChange(m_dTank_WeightTank/2/m_dTank_TankLength,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double Ptk = GetValueUnitChange(m_dTank_Ptank,UNIT_CGS_TONF_M,pData->m_szTonf_M);

	m_pStd->DrawDiagramSlabDistribution(pDomTank,pFEM,P, dDist, dLength);

	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;

	XLText->AddFormatText("$hP   = %6.3f %s$n",P,pData->m_szTonf);
	XLText->AddFormatText("$hPtk = %6.3f %s$n",Ptk,pData->m_szTonf);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDomTank,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);

	// ��ũ Ʈ���Ϸ� ����
	if(bTitle)
		XLText->AddFormatText("$s+%02d$h$r$c%s$r%s$n",DIAGRAM_ROW_SIZE,GetNumberString(nIndexSmall++,NUM_CIRCLE),"��ũ Ʈ���Ϸ� ����");

	CDomyun *pDomTr = new CDomyun;
	pDomTr->SetCWnd(m_pStd->m_pView);
	pDomTr->SetScaleDim(m_dDimScaleDiagram);
	pDomTr->SetLTScale(m_dDimScaleDiagram*5);

	DrawDiagramModeling(pDomTr, TRUE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomTr,HCAD_DIML);
	pDomTr->SetTextHeight(pDomTr->GetTextHeight(FALSE)*4.0);

	dDist = 2000;

	CDPoint xy = xyStt + dDist*vX;
	CObInsert *pInP1 = CHgBaseDrawStd::InsertHCadBlock(pDomTr, "HGMODARD", xy, Scale*1.2, 0, TRUE);
	CHgBaseDrawStd::SetAttrib(pDomTr, pInP1, "RB_NO", "P1");

	xy += frM(m_dTank_TrailerL1) * vX;
	pInP1 = CHgBaseDrawStd::InsertHCadBlock(pDomTr, "HGMODARD", xy, Scale*1.2, 0, TRUE);
	CHgBaseDrawStd::SetAttrib(pDomTr, pInP1, "RB_NO", "P2");

	xy += frM(m_dTank_TrailerL2) * vX;
	pInP1 = CHgBaseDrawStd::InsertHCadBlock(pDomTr, "HGMODARD", xy, Scale*1.2, 0, TRUE);
	CHgBaseDrawStd::SetAttrib(pDomTr, pInP1, "RB_NO", "P2");

	xy += frM(m_dTank_TrailerL3) * vX;
	pInP1 = CHgBaseDrawStd::InsertHCadBlock(pDomTr, "HGMODARD", xy, Scale*1.2, 0, TRUE);
	CHgBaseDrawStd::SetAttrib(pDomTr, pInP1, "RB_NO", "P3");

	xy += frM(m_dTank_TrailerL4) * vX;
	pInP1 = CHgBaseDrawStd::InsertHCadBlock(pDomTr, "HGMODARD", xy, Scale*1.2, 0, TRUE);
	CHgBaseDrawStd::SetAttrib(pDomTr, pInP1, "RB_NO", "P3");

	nT_Text = (pBri->GetCountJijum()==2)? 9:6;
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 4;
	double P1 = GetValueUnitChange(m_dTank_Ptrailer1,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double P2 = GetValueUnitChange(m_dTank_Ptrailer2,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	double P3 = GetValueUnitChange(m_dTank_Ptrailer3,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	XLText->AddFormatText("$hP1 = %6.3f %s$n",P1,pData->m_szTonf);
	XLText->AddFormatText("$hP2 = %6.3f %s$n",P2,pData->m_szTonf);
	XLText->AddFormatText("$hP3 = %6.3f %s$n",P3,pData->m_szTonf);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDomTr,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::DrawDiagramLosdLS_ES_Lsd( CXLFormatText *XLText, BOOL bLeft, BOOL bEarthQuake, BOOL bLS )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = bEarthQuake ? &pBri->m_FemEQ[0] : &pBri->m_FemNormal;

	if(pData->IsLsdDesign() == FALSE) return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);


	if(bEarthQuake)
		m_pStd->m_pLoadEQ->DrawDiagramModeling(pDom, TRUE);
	else
		DrawDiagramModeling(pDom, TRUE, FALSE);

	double dWidth = bEarthQuake ? frM(m_pStd->GetSlabWidth()) : 1.0;
	long nT_Text	  = (pBri->GetCountJijum()==2)? 9 : 6;
	double dValue(0);
	if(bLS)
	{
		dValue = GetValueUnitChange(m_dLoadLsApply[bLeft? iSTT : iEND], UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * dWidth;
	}
	else
	{
		dValue = GetValueUnitChange(GetLoadLiveAlwaysHori()	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	}
	long nSizeEle   = pFEM->GetElementSize();
	
	double Scale = pDom->GetScaleDim();
	CDPoint vX(1,0),xyUp(0,0),xyDn(0,0);

	BOOL bFirst = TRUE;
	long i=0; for(i=0; i<nSizeEle; i++)
	{
		if(dValue<=0.0) break;
		CElement *pE = pFEM->GetElement((USHORT)i);			
		if(pE->m_nFlag!= (bLeft? ELEMENT_STTWALL : ELEMENT_ENDWALL)) continue;
		CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);
		CDPoint xyNodeJ = CDPoint(pJntJ->m_vPoint.x,pJntJ->m_vPoint.z);
		xyDn = xyNodeJ + CDPoint(700, 0) * (bLeft? -1 : 1);
		CHgBaseDrawStd::InsertHCadBlock(pDom, bLeft? _T("HGMODARRONLYR") : _T("HGMODARRONLYL"), xyNodeJ, Scale, 0, TRUE);
		pDom->LineTo(xyNodeJ,xyDn);
		if(bFirst)
		{
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CDPoint xyNodeI = CDPoint(pJntI->m_vPoint.x,pJntI->m_vPoint.z);
			xyUp = xyNodeI + CDPoint(700, 0) * (bLeft? -1 : 1);
			CHgBaseDrawStd::InsertHCadBlock(pDom, bLeft? _T("HGMODARRONLYR") : _T("HGMODARRONLYL"), xyNodeI, Scale, 0, TRUE);
			pDom->LineTo(xyNodeI,xyUp);
			bFirst = FALSE;
		}
	}
	pDom->LineTo(xyUp,xyDn);
	XLText->PushRow();
	XLText->GoPosition(NO_MOVE,nT_Text);
	*XLText += 5;
	XLText->AddFormatText("$h q = %s %s$n", pData->GetStrDouble(dValue,  UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 6), pData->m_szTonf_M);
	XLText->PopRow();
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcLoad::CalculateLoadLs_Lsd()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd == FALSE)
		return;

	double dUwBackFill = pBri->m_dUnitWeigthBackFill;
	double dKo = m_pStd->GetCofficientForAlwaysLiveLoad();
	double dHeightWallStt = pBri->GetHeightBridgeWall(TRUE);
	double dHeightWallEnd = pBri->GetHeightBridgeWall(FALSE);
	double dEquivalentHeightStt = toM(GetEquivalentHeightWall(dHeightWallStt));
	double dEquivalentHeightEnd = toM(GetEquivalentHeightWall(dHeightWallEnd));
	double dRoadWaysFactor = GetCalculateRoadWayFactor();
	// (ARCBRIDGE-2665) '����Ȱ���� ������ �������' ����ɼ� �߰�.
	double dCorrFactor = pBri->m_bCorrectionFactor_LiveLoadAlways ? pBri->m_dCorrectionFactor_LiveLoadAlways : 1.00;

	for(HGINT32 ixStt=0; ixStt<2; ++ixStt)
	{
		BOOL bStt = (ixStt == 0)? TRUE : FALSE;

		m_dLoadLsApply[ixStt] = dKo * dUwBackFill * (bStt? dEquivalentHeightStt : dEquivalentHeightEnd) * dCorrFactor * dRoadWaysFactor;
	}
}

void CARcBridgeCalcLoad::MakeTableOutValueEta_Lsd( CXLFormatText *pXLText, long iIdxEta, long nIdxCircle, BOOL bForCantilever )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CLsdManagerBridge *pLsdMng = bForCantilever? pBri->m_plsdMngForCantilever : pBri->m_plsdManager;
	CString str = _T("");

	long nSizeTot(0);
	for(long ix=0; ix<eLSD_COMBINE_TOTAL_COUNT; ++ix)
	{
		if(pLsdMng->IsUsedComboKind((ELSDCombKind)ix))
			++nSizeTot;
	}
	
	long nCntRow  = 2 + nSizeTot;
	long nCntCol  = 7;


	int nWidthTitle[] = { 6, 3, 3, 3, 9 };
	int nWidthColumn[] = { 6, 3, 3, 3, 3, 3, 3 };
	HGCHAR *Title1[] = { hggettext_noop("��������"), hggettext_noop("��D"), hggettext_noop("��R"), hggettext_noop("��I"), hggettext_noop("��i") };
	HGCHAR *Title2[] = { hggettext_noop("��������"), hggettext_noop("��D"), hggettext_noop("��R"), hggettext_noop("��I"), hggettext_noop("�ִ�"), hggettext_noop("ǥ��"), hggettext_noop("�ּ�") };

	CString strPos = pLsdMng->GetLsdLoadModifiedName(iIdxEta);
	
	pXLText->GoPosition(NO_MOVE, COL_POS_2);
	if(nIdxCircle > 0) pXLText->AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(nIdxCircle, NUM_CIRCLE), strPos);
	
	pXLText->AddTable(nCntRow-1, 23, 2, 1);

	pXLText->OutXL();
	// Title	
	long jx=0; for(jx=0; jx<5; ++jx)
	{
		pXLText->AddMergeFormatText((jx==0 || jx==1 || jx==2 || jx==3)? 1:0, nWidthTitle[jx]-1, _T("$c%s"), hggettext(Title1[jx]));
		pXLText->AddFormatText(_T("$m+0%d"), nWidthTitle[jx]);
	}
	pXLText->AddText(_T("$n$h"));
	for(jx=0; jx<nCntCol; ++jx)
	{
		pXLText->AddMergeFormatText(0, nWidthColumn[jx]-1,_T("$c%s"), hggettext(Title2[jx]));
		pXLText->AddFormatText(_T("$m+0%d"), nWidthColumn[jx]);
	}
	pXLText->AddText(_T("$n$h"));

	long kx=0; for (kx=0; kx<eLSD_COMBINE_TOTAL_COUNT; ++kx)
	{
		// ���õ� ���ո� ��� �Ѵ�.
		if(pLsdMng->IsUsedComboKind((ELSDCombKind)kx) == FALSE)
			continue;

		for(jx=0; jx<nCntCol; ++jx)
		{
			switch(jx) 
			{
			case 0:	str.Format(hggettext("  %s"), pLsdMng->GetCombineNameByIndex((ELSDCombKind)kx));	break;
			case 1: str.Format(hggettext("$c$f2%.2f"), pLsdMng->GetValueLsdLoadModified(iIdxEta, (ELSDCombKind)kx, eLsdDuctility)); break;
			case 2: str.Format(hggettext("$c$f2%.2f"), pLsdMng->GetValueLsdLoadModified(iIdxEta, (ELSDCombKind)kx, eLsdRedundancy)); break;
			case 3: str.Format(hggettext("$c$f2%.2f"), pLsdMng->GetValueLsdLoadModified(iIdxEta, (ELSDCombKind)kx, eLsdImportance)); break;
			case 4: str.Format(hggettext("$c$f3%.3f"), pLsdMng->GetLsdEtaiValueMaxLoadModified(iIdxEta, (ELSDCombKind)kx)); break;
			case 5: str.Format(hggettext("$c$f3%.3f"), pLsdMng->GetLsdEtaiValueStandardLoadModified(iIdxEta, (ELSDCombKind)kx)); break;
			case 6: str.Format(hggettext("$c$f3%.3f"), pLsdMng->GetLsdEtaiValueMinLoadModified(iIdxEta, (ELSDCombKind)kx)); break;
			default: str = _T("");
			}

			pXLText->AddMergeFormatText(0, nWidthColumn[jx]-1, jx==0?_T("$e%s"):_T("$c%s"), str); 
			pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[jx]);
		}
		pXLText->AddFormatText(_T("$n$h"));
	}
	pXLText->AddFormatText(_T("$n$h"));
}

double CARcBridgeCalcLoad::GetTemperatureSlopeH(long nPosition)
{
	if( nPosition > 2) return 0.;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	HGDOUBLE dHeight = pBri->m_dTS;
	HGDOUBLE dT[3] = {0,0,0};

	dT[0] = (dHeight <= 100) ? dHeight : 100;						// T1~T2
	dT[1] = (dHeight >= 400) ? 300 : dHeight - 100;					// T2~0
	dT[2] = (dHeight >= 600) ? 200 : dHeight - dT[0] - dT[1];			// T2~0
	if( dT[2] < 0 )
		dT[2] = 0;

	return dT[nPosition];
}

double CARcBridgeCalcLoad::GetTemperatureSlopeTz(long nPosition, HGDOUBLE dZi, HGBOOL bLowerTemperature)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	HGDOUBLE dZMax(0);
	HGDOUBLE dDivisionH = GetTemperatureSlopeH(nPosition);
	if(dDivisionH <= 0)
		return 0;

	HGDOUBLE dZi_1 = dZi - (dDivisionH / 5);
	HGDOUBLE dZ = (dZi + dZi_1) / 2;
	HGDOUBLE Tz(0);
	HGDOUBLE dPave = bLowerTemperature==TRUE ? (pBri->m_nApplyPavement==0 ? -0.2 : -0.3) : 1;

	switch(nPosition)
	{
	case 0:
		dZMax = pBri->m_dTS / 2;
		Tz = (pBri->m_dTemperatureSlopeT[0] * dPave) - ((pBri->m_dTemperatureSlopeT[0] * dPave) - (pBri->m_dTemperatureSlopeT[1] * dPave)) / (dZMax - (dZMax - dDivisionH)) * (dZMax - dZ);
		break;
	case 1:
		dZMax = pBri->m_dTS / 2 - GetTemperatureSlopeH(0);
		Tz = (pBri->m_dTemperatureSlopeT[1] * dPave) - (pBri->m_dTemperatureSlopeT[1] * dPave) / (dZMax - (dZMax - dDivisionH)) * (dZMax - dZ);
		break;
	case 2:
		dZMax = -(pBri->m_dTS / 2) + GetTemperatureSlopeH(2);
		Tz = pBri->m_dTemperatureSlopeT[2] / (dZMax - (dZMax - dDivisionH)) * (dZMax - dZ);
		break;
	}

	return Tz;
}

double CARcBridgeCalcLoad::GetTemperatureSlopeS(long nPosition, HGDOUBLE dZi, HGDOUBLE dTz, HGBOOL bS2)
{
	HGDOUBLE dResult(0.);
	HGDOUBLE dCoffient(1000);

	HGDOUBLE dDivisionH = GetTemperatureSlopeH(nPosition);
	if( dDivisionH <= 0)
		return 0;

	HGDOUBLE dZi_1 = dZi - (dDivisionH / 5);
	HGDOUBLE dZ = (dZi + dZi_1) / 2;

    dResult = dTz * dCoffient * (dDivisionH / 5);
	if( bS2 )
		dResult *= dZ;

	return dResult;
}

void CARcBridgeCalcLoad::WriteCalcUserLoad_Lsd( CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake, long nTypeEQ )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nType = bEarthquake? (1 + nTypeEQ) : 0;
	HGINT32 icLoadSet = pBri->GetCountUserLoadSetLsd(nType);
	if(icLoadSet<1) return;

	CString strAdd(_T(""));
	if(pBri->IsBoxType())
	{
		strAdd = _T(" ") + pBri->GetStringGoalEarthQuake(static_cast<EGoalOfEarthQuake>(nTypeEQ), TRUE);
	}

	pXLText->GoPosition(NO_MOVE,COL_POS_1);
	pXLText->AddFormatText(_T("$h$a%d.%d %s%s$n"), nIndexTitle, nIndexMiddle, hggettext("���������"), strAdd);

	pXLText->GoPosition(NO_MOVE, COL_POS_2);
	HGINT32 nWidthColumn[] = {3, 3, 3, 3, 3, 3};
	HGCHAR *TitleJoint[] = { _T("��������"), _T("���� No."), _T("X"), _T("Y"), _T("Z")};
	HGCHAR *TitleElemV[] = { _T("��������"), _T("���� No."), _T("d1"), _T("Vx"), _T("Vy"), _T("Vz")};
	HGCHAR *TitleElemW[] = { _T("��������"), _T("���� No."), _T("d1"), _T("d2"), _T("w1"), _T("w2")};

	double dCol1 = 0, dCol2 = 0, dCol3 = 0, dCol4 = 0;


	// 0 : �������� , 1:��������V, 2:��������W
	
	for(HGINT32 ixLoadSet = 0; ixLoadSet < icLoadSet; ++ixLoadSet)
	{
		CBridgeUserLoadSet *pUserLoadSet = pBri->m_vpUserLoadSet[nType][ixLoadSet];
		if(pUserLoadSet == NULL) continue;

		HGINT32 icUserLoadTotal = pUserLoadSet->m_pArrUserLoad.GetSize();
		if(icUserLoadTotal < 1) continue;

		CString strLoadName = pUserLoadSet->m_szTitle;
		CString strLoadCase = pUserLoadSet->m_sTypeLoadCase;
		pXLText->AddFormatText(_T("$h%d) %s : %s$n"),ixLoadSet+1, strLoadName, strLoadCase);

		for(long ixLoadType = 0; ixLoadType < 3; ++ ixLoadType)
		{
			long icUserLoad = GetCountUserLoadType(ixLoadType, ixLoadSet, bEarthquake, nTypeEQ);
			if(icUserLoad < 1) continue;

			long icCol = (ixLoadType == 0) ? 5 : 6;
			pXLText->AddTable(icUserLoad, icCol*3 - 1, 1, 0);

			//Title
			for(HGINT32 iCol = 0; iCol < icCol; ++iCol)
			{
				CString szTitle = (ixLoadType == 0) ? TitleJoint[iCol] : (ixLoadType == 1 ? TitleElemV[iCol] : TitleElemW[iCol]);
				pXLText->AddMergeFormatText(0, nWidthColumn[iCol]-1, _T("$c%s"), szTitle);
				pXLText->AddFormatText(_T("$m+0%d"), nWidthColumn[iCol]);
			}
			pXLText->AddText(_T("$n$h"));

			//Data
			for(HGINT32 iUser = 0; iUser < icUserLoadTotal; ++iUser)
			{
				CBridgeUserLoad *pUserLoad = pUserLoadSet->m_pArrUserLoad[iUser];
				if(pUserLoad == NULL) continue;

				long nUserLoadType = pUserLoad->m_nType;
				if(ixLoadType == 0 && (nUserLoadType != USER_LOAD_JOINT_V && nUserLoadType != USER_LOAD_JOINT_M)) continue;
				else if(ixLoadType == 1 && (nUserLoadType != USER_LOAD_ELEMENT_V)) continue;
				else if(ixLoadType == 2 && (nUserLoadType != USER_LOAD_ELEMENT_WX && nUserLoadType != USER_LOAD_ELEMENT_WY && nUserLoadType != USER_LOAD_ELEMENT_WZ)) continue;

				CString strLoadType = pBri->GetStringUserLoadType(nUserLoadType);

				pXLText->AddMergeFormatText(0, nWidthColumn[0]-1, _T("$c%s"), strLoadType); 
				pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[0]);

				pXLText->AddMergeFormatText(0, nWidthColumn[1]-1, _T("$c%d"), pUserLoad->m_nIdxNo); 
				pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[1]);

				long nDec(3);
				if(nUserLoadType == USER_LOAD_JOINT_V)
				{
					dCol1 = GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF, pData->m_szTonf);
					dCol2 = GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF, pData->m_szTonf);
					dCol3 = GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF, pData->m_szTonf);
				}
				else if(nUserLoadType == USER_LOAD_JOINT_M)
				{
					dCol1 = GetValueUnitChange(pUserLoad->m_Mx, UNIT_CGS_TONF, pData->m_szTonf);
					dCol2 = GetValueUnitChange(pUserLoad->m_My, UNIT_CGS_TONF, pData->m_szTonf);
					dCol3 = GetValueUnitChange(pUserLoad->m_Mz, UNIT_CGS_TONF, pData->m_szTonf);
				}
				else if(nUserLoadType == USER_LOAD_ELEMENT_V)
				{
					dCol1 = pUserLoad->m_dDist[0];
					dCol2 = GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF_M, pData->m_szTonf_M);
					dCol3 = GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF_M, pData->m_szTonf_M);
					dCol4 = GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF_M, pData->m_szTonf_M);
				}
				else
				{
					nDec = 6;
					dCol1 = pUserLoad->m_dDist[0];
					dCol2 = pUserLoad->m_dDist[1];
					dCol3 = GetValueUnitChange(pUserLoad->m_dLoadW2[0], UNIT_CGS_TONF_M, pData->m_szTonf_M);
					dCol4 = GetValueUnitChange(pUserLoad->m_dLoadW2[1], UNIT_CGS_TONF_M, pData->m_szTonf_M);
				}

				pXLText->AddMergeFormatText(0, nWidthColumn[2]-1, _T("$c$f3%s"), GetStrDouble(dCol1, 3, TRUE)); 
				pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[2]);

				pXLText->AddMergeFormatText(0, nWidthColumn[3]-1, _T("$c$f3%s"), GetStrDouble(dCol2, 3, TRUE));  
				pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[3]);

				pXLText->AddMergeFormatText(0, nWidthColumn[4]-1, _T("$c$f%d%s"), nDec, GetStrDouble(dCol3, nDec, TRUE)); 
				pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[4]);

				if(nUserLoadType != USER_LOAD_JOINT_V && nUserLoadType != USER_LOAD_JOINT_M)
				{
					pXLText->AddMergeFormatText(0, nWidthColumn[5]-1, _T("$c$f%d%s"), nDec, GetStrDouble(dCol4, nDec, TRUE)); 
					pXLText->AddFormatText(_T("$m+%02d"), nWidthColumn[5]);
				}

				pXLText->AddText(_T("$n$h"));
			}

			pXLText->AddText(_T("$n$h"));
		}
	}

	*pXLText += 1;
}

// 0 : �������� , 1:��������V, 2:��������W
long CARcBridgeCalcLoad::GetCountUserLoadType( long nLoadType, long ixLosdSet, BOOL bEarthquake, long nTypeEQ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nType = bEarthquake? (1 + nTypeEQ) : 0;
	HGINT32 icLoad = pBri->GetCountUserLoadSetLsd(nType);
	if(icLoad<1) return 0;

	long icTotal = 0;

	CBridgeUserLoadSet *pUserLoadSet = pBri->m_vpUserLoadSet[nType][ixLosdSet];
	if(pUserLoadSet == NULL) return 0;

	HGINT32 icUserLoad = pUserLoadSet->m_pArrUserLoad.GetSize();
	if(icUserLoad < 1) return 0;
	
	//Data
	for(HGINT32 iUser = 0; iUser < icUserLoad; ++iUser)
	{
		long nType = pUserLoadSet->m_pArrUserLoad[iUser]->m_nType;

		if(nLoadType == 0 && (nType == USER_LOAD_JOINT_V || nType == USER_LOAD_JOINT_M))
			icTotal++;
		else if(nLoadType == 1 && (nType == USER_LOAD_ELEMENT_V)) 
			icTotal++;
		else if(nLoadType == 2 && (nType == USER_LOAD_ELEMENT_WX || nType == USER_LOAD_ELEMENT_WY || nType == USER_LOAD_ELEMENT_WZ)) 
			icTotal++;
	}
	

	return icTotal;
}

void CARcBridgeCalcLoad::CalculateLoadTG()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pData->IsLsdDesign() == FALSE) return;

	// Table
	long nCntDivision(5);
	for(HGINT32 iUpper=0; iUpper<2; ++iUpper)
	{
		HGDOUBLE zi = pBri->m_dTS / 2;
		HGDOUBLE dSumS1(0), dSumS2(0);
	
		for (HGINT32 index=0; index<3; ++index)
		{
			HGDOUBLE dDivisionH = GetTemperatureSlopeH(index);

			if(index == 2) zi = -(pBri->m_dTS / 2) + dDivisionH;

			for(long ix=0; ix<nCntDivision; ++ix)
			{
				HGDOUBLE zi_1 = (dDivisionH <= 0) ? zi : zi - (dDivisionH / nCntDivision);
				HGDOUBLE z = (dDivisionH <= 0) ? zi : (zi + zi_1) / 2;
				HGDOUBLE Tz = GetTemperatureSlopeTz(index, zi, iUpper==1 ? TRUE : FALSE);
				HGDOUBLE S1 = GetTemperatureSlopeS(index, zi, Tz, FALSE);
				HGDOUBLE S2 = GetTemperatureSlopeS(index, zi, Tz, TRUE);

				zi -= (dDivisionH / nCntDivision);
				dSumS1 += S1;
				dSumS2 += S2;
			}
		}

		m_dLoadTgValueMg[iUpper] = pBri->GetValueEc(ePartUpperSlab, TRUE) * pBri->GetCoefficientExpansion() * dSumS2;
		m_dLoadTgValueT[iUpper] = Round( m_dLoadTgValueMg[iUpper] * pBri->m_dTS / (pBri->GetCoefficientExpansion() * pBri->GetValueEc(ePartUpperSlab, TRUE) * 1000 * pow(pBri->m_dTS,3) / 12 ), 3 );
	}
}

double CARcBridgeCalcLoad::GetCoefficientImpactFactor( double dLenSpan, double &dFactor1, double &dFactor2, double &dLenGap )
{
	double dL[9] = { 0, 5, 10, 20, 30, 40, 50, 70 , 100 };
	double dFact[9] = { 0.60, 0.48, 0.43, 0.37, 0.34, 0.32, 0.30, 0.27, 0.24 };
	double dLenBase(0);
	for(long ix=0; ix<8; ++ix)
	{
		double dL1 = dL[ix];
		double dL2 = dL[ix+1];
		if(dLenSpan >= dL1 && dLenSpan <= dL2)
		{
			dLenBase = dL1;
			dLenGap = dL2 - dL1;
			dFactor1 = dFact[ix];
			dFactor2 = dFact[ix + 1];
			break;
		}
	}
	
	return dLenBase;
}

void CARcBridgeCalcLoad::DrawDiagramUserLoad( CXLFormatText *XLText, long nTypeEQ, long nIndex )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = nTypeEQ == 0 ? &pBri->m_FemNormal : &pBri->m_FemEQ[0];
	BOOL bLsd	= pData->IsLsdDesign();

	CBridgeUserLoadSet *pUserLoadSet = pBri->m_vpUserLoadSet[nTypeEQ][nIndex];
	if(pUserLoadSet == NULL) return;

	HGINT32 icUserLoadTotal = pUserLoadSet->m_pArrUserLoad.GetSize();
	if(icUserLoadTotal < 1) return;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(m_dDimScaleDiagram);
	pDom->SetLTScale(m_dDimScaleDiagram*5);

	CString szNum;
	szNum.Format(_T("USER%02d"), nIndex+1);
	XLText->AddFormatText("$h$r%s$r%s - %s(%s)$n",GetNumberString(nIndex+1,NUM_CIRCLE), szNum, pUserLoadSet->m_szTitle, pUserLoadSet->m_sTypeLoadCase);

	DrawDiagramModeling(pDom, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE));

	for(HGINT32 iUser = 0; iUser < icUserLoadTotal; ++iUser)
	{
		CBridgeUserLoad *pUserLoad = pUserLoadSet->m_pArrUserLoad[iUser];
		if(pUserLoad == NULL) continue;

		long nUserLoadType = pUserLoad->m_nType;		
		if(nUserLoadType == USER_LOAD_JOINT_V)
		{
			CVector vLoad;
			vLoad.x = GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF, pData->m_szTonf);
			vLoad.y = GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF, pData->m_szTonf);;
			vLoad.z = GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF, pData->m_szTonf);;
			m_pStd->DrawDiagramJointLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, vLoad, FALSE);
		}
		else if(nUserLoadType == USER_LOAD_JOINT_M)
		{
			CVector vLoad;
			vLoad.x = GetValueUnitChange(pUserLoad->m_Mx, UNIT_CGS_TONFM, pData->m_szTonfM);
			vLoad.y = GetValueUnitChange(pUserLoad->m_My, UNIT_CGS_TONFM, pData->m_szTonfM);
			vLoad.z = GetValueUnitChange(pUserLoad->m_Mz, UNIT_CGS_TONFM, pData->m_szTonfM);
			m_pStd->DrawDiagramJointLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, vLoad, TRUE);
		}
		else if(nUserLoadType == USER_LOAD_ELEMENT_V)
		{
			if(Compare(pUserLoad->m_Ux, 0.0, _T("=")) == FALSE)
				m_pStd->DrawDiagramElementLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, 0, 0, pUserLoad->m_dDist[0], 0, GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF, pData->m_szTonf), 0);
			if(Compare(pUserLoad->m_Uy, 0.0, _T("=")) == FALSE)
				m_pStd->DrawDiagramElementLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, 1, 0, pUserLoad->m_dDist[0], 0, GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF, pData->m_szTonf), 0);
			if(Compare(pUserLoad->m_Uz, 0.0, _T("=")) == FALSE)
				m_pStd->DrawDiagramElementLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, 2, 0, pUserLoad->m_dDist[0], 0, GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF, pData->m_szTonf), 0);
		}
		else
		{
			long nDir(0);
			if(pUserLoad->m_nType == USER_LOAD_ELEMENT_WX)
			{
				nDir = 0;		// 0-X,1-Y,2-Z
			}
			else if(pUserLoad->m_nType == USER_LOAD_ELEMENT_WY)
			{
				nDir = 1;		// 0-X,1-Y,2-Z
			}
			else if(pUserLoad->m_nType == USER_LOAD_ELEMENT_WZ)
			{
				nDir = 2;		// 0-X,1-Y,2-Z
			}

			m_pStd->DrawDiagramElementLoad(pDom, pFEM, pUserLoad->m_nIdxNo-1, nDir, 1, pUserLoad->m_dDist[0], pUserLoad->m_dDist[1], GetValueUnitChange(pUserLoad->m_dLoadW2[0], UNIT_CGS_TONF_M, pData->m_szTonf_M), GetValueUnitChange(pUserLoad->m_dLoadW2[1], UNIT_CGS_TONF_M, pData->m_szTonf_M));
		}
	}

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,DIAGRAM_ROW_SIZE,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

BOOL CARcBridgeCalcLoad::IsExistEV(CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	if(bLsd == FALSE) return FALSE;
	
	if(pBri->m_bUnderGroundRahmen == FALSE && pBri->IsBoxType() == FALSE) return FALSE;

	if(pBri->m_bUnderGroundRahmen) return TRUE;

	if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting) return TRUE;

	if(pBri->IsBoxType() && pBri->m_nSelectInteriorWaterPress == 0 && pBri->m_dHeightInteriorWaterPress > 0)
		return TRUE;

	return FALSE;
}

BOOL CARcBridgeCalcLoad::IsExistWA( CRcBridgeRebar *pBri )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();

	if(pBri->IsBoxType() == FALSE) return FALSE;

	if(pBri->m_bApplyGroundWaterLevel) return TRUE;

	if(bLsd)
	{
		if(pBri->m_nSelectInteriorWaterPress == 2 && pBri->m_dHeightInteriorWaterPress > 0)
			return TRUE;
	}
	else
	{
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT]) 
			return TRUE;
	}

	return FALSE;
}
