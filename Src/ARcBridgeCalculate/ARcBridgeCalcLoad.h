// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcLoad.h: interface for the CARcBridgeCalcLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCLOAD_H__E7A0BB5A_EDB2_40D8_89C5_9F41DD25BFBD__INCLUDED_)
#define AFX_ARCBRIDGECALCLOAD_H__E7A0BB5A_EDB2_40D8_89C5_9F41DD25BFBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuardWallRC;
class CXLFormatText;
class AFX_EXT_CLASS CARcBridgeCalcLoad  
{
public:
	void CalculateAll(); 
	CARcBridgeCalcLoad();
	virtual ~CARcBridgeCalcLoad();

	CARcBridgeCalcStd *m_pStd;

public:
	void WriteLoadDiagram(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcDeadLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bDW, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLiveLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLiveLoadUnderGround(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLiveLoadTank(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcRailLoadLive(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcRailLoadLR(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcRailLoadSB(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcRailLoadSB_2017(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcSoilPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake, EGoalOfEarthQuake eGoalEq);
	void WriteCalcWaterPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcSubsidence(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcTemperature(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcDryAndShrinkage(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteDesignSpanLengthSkew(CXLFormatText *XLText, CRcBridgeRebar *pBri);

	void WriteLoadDiagram_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLiveLoadLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bFatigue=FALSE);
	void WriteCalcLiveLoadFootwayLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLiveLoadFootway(CXLFormatText * XLText, CRcBridgeRebar * pBri);
	void WriteCalcVerticalSoilPressureForLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcBouyancyLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcAlwaysLiveLoadForLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcPaveAndUpperSoil(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexTiny, BOOL bPave);
	void WriteCalcInteriorWaterPress(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexSmall);
	void WriteLoadWaysFactorLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nColStt, long &nIndexTiny);
	void WriteCalcTemperatureGapLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcAlwaysLoadLsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcUserLoad_Lsd(CXLFormatText *pXLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bEarthquake, long nTypeEQ=0);

	void WriteSectionDeadLoad(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void MakeTableOutValueEta_Lsd(CXLFormatText *pXLText, long iIdxEta, long nIdxCircle, BOOL bForCantilever=FALSE);

	BOOL IsExistEV(CRcBridgeRebar *pBri);
	BOOL IsExistWA(CRcBridgeRebar *pBri);
public:
	BOOL m_bReCalcSpanLength;
	BOOL m_bRailDesign;
	BOOL m_bSingleLane;	// ö������ ��,����
	BOOL m_bExistLane;

	double m_dReCalcRateSpan;

	void DrawLiveLoadLaneCount(CDomyun *pDomP, long nLaneCount);
	void DrawDeadLoadRail(CDomyun *pDomP, BOOL bForCalc=TRUE, double dStation=0);
	double DrawDeadLoadSectionEtc(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC *pGW, CStringArray &strArr);
	void DrawDeadLoadBodo(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC *pGW, long nHDan);
	void DrawDeadLoadHDan(CDomyun *pDomP);
	double GetLoadSoilPressure(double dHeightFromSlab, BOOL bSttWall = TRUE, BOOL bExistWater = FALSE, BOOL bEarthQuake = FALSE, EGoalOfEarthQuake eGoalEq=eGoalEQ_None, BOOL bVertLoad=FALSE);
	double GetLoadWaterPressure(double dElevation);
	double GetLengthSpanLiveLoad();
	double GetLoadLiveAlwaysHori();

	BOOL	m_bGWhalf;
	double m_dWeightAscon;
	double m_dWeightMortar;
	double m_dWeightBangHo[2];
	double m_dWeightFootway[2];
	double m_dWeightCentro[2];
	double m_dWeightBoCha[2];
	double m_dWeightNangan[2];
	double m_dWeightSound[2];
	double m_dWeightNanganBase[2];	// ��������
	double m_dWeightSoundBase[2];		// ����������
	double m_dWeightRetainingWall[2];	// �븷�̺�
	double m_dWeightRailLane;
	double m_dWeightRailDosang;
	double m_dUpperPBangHo[2];			// ��ȣ�� ��� P
	double m_dUpperPCentro[2];
	double m_dUpperPBoCha[2];
	double m_dUpperPRetainingWall[2];
	
	double m_DeadLoadDistribution[2];	// ����������
	double m_dSoilPressureVert;		// �������
	double m_dPfApply[2];
	double m_dPmApplyTruck[2];			// ǥ��Ʈ������ Pm
	double m_dPrApply[2];
	double m_dPmApply[2];
	double m_dPsApply[2];
	double m_dLaneLoad;				// ǥ����������
	double m_dLiveLoadFootway;

	// ��ũ �� ����������
	double m_dTank_E;				// 
	double m_dTank_i;				// 0.15
	double m_dTank_Ptank;
	double m_dTank_Ptrailer1;
	double m_dTank_Ptrailer2;
	double m_dTank_Ptrailer3;
	//
	double m_dTank_WeightTank;		// 58.0 ton
	double m_dTank_WeightTrailer;	// 71.0 ton
	double m_dTank_TankLength;		// 4.5
	double m_dTank_TankWidth;		// 2.6
	double m_dTank_TrailerL1;		// 3.6
	double m_dTank_TrailerL2;		// 1.5
	double m_dTank_TrailerL3;		// 4.5
	double m_dTank_TrailerL4;		// 1.2
	double m_dTank_TrailerWidth;	// 2.225

	double m_dSpanLengthDesign;
	double m_dWidthBetweenCurb;
	double m_dDistributionApply;
	double m_dEffectiveWidth;
	double m_dEffectiveWidthLane2;
	double m_dDesignRoadWidth;	// ����������
	double m_dDesignTruckWidth;	// ǥ��Ʈ���� -> 1.800m
	double m_dCoefficientImpact[2];
	long m_nCaseDesignLane[2];

	// ����� ��ݰ�� ����
	double m_dSpanLengthImpactMin;
	double m_dSpanLengthImpactMax;
	double m_dSpanLengthImpactAvg;
	double m_dSpanLengthImpactApply;
	double m_dCoefficientImpactSingle;

// 	CDoubleArray m_dArrSoilPressure[2];	// ��������� �迭...,SAP DATA ������ ����....
// 	CDoubleArray m_dArrSoilPressureW[2];// ��������� �迭...,SAP DATA ������ ����. ���ϼ��� �ִ� ���
// 	CDoubleArray m_dArrSoilHeight[2];	// ��������� �迭...

// 	CDoubleArray m_dArrWaterPressure[2];// �������
// 	CDoubleArray m_dArrWaterHeight[2];	// 
	double m_dLoadBuoyancy;	// �η�
	double m_dLoadWaterVert;	// �λ�����ǿ� �ۿ��ϴ� ��������

	double m_dLoadTempertureAxial;
	double m_dLoadTempertureUpDown;
	double m_dLoadDryAndShrinkage;
	double m_dLoadSubsidence;
	
	///////////////////////////////////////////////////////////////////////////
	// ö�����߰��� ����
	double m_dRail_WidthDistribute;			// �˵����߿� ���� ������
	double m_dRail_WidthDistributeJong;		// �˵����߿� ���� ������ - ������
	double m_dHeightRail;				// ���ϳ���
	double m_dRail_WeightLane;				// �˵��߷� (1�˵�)
	double m_dHeightDosangUnderTie;	// ħ��Ʒ� �������
	double m_dRail_HeightTie;				// ħ�����
	double m_dRail_LengthTie;				// ħ�����
	double m_dRail_DistBetweenRail;			// �˵��߽ɰ� �Ÿ� (������ ���)
	double m_dRail_AngleDosangDistribute;	// ������ �������� (��ȿ������)
	double m_dRail_AngleEffective;			// ���ǵ� �������� (��ȿ������)
	double m_dRail_DosangWidthU;		// ���� ���� �ʺ�
	double m_dRail_DosangWidthL;		// ���� �Ʒ��� �ʺ�
	double m_dRail_LengthLv;

 	// ��뷹�� ������
	double m_dRail_UnitForceLR;	// ���� ����(2004:1.0, 2017:0.6);
	double m_dRail_LoadLR;		// �����
	double m_dRail_MomentLR;	// ���Ʈ
	double m_dRail_HeightLR;	// �𵨸� �༱�� ���� �ۿ����

	double m_dRail_LoadSB;
	double m_dRail_MomentSB;
	double m_dRail_HeightSB;
	// �õ�����
	double m_dRail_UnitForceSidong;
	double m_dRail_TsbSidong;
	double m_dRail_LoadSidong;
	double m_dRail_MomentSidong;
	double m_dRail_HeightSidong;
	// ��������
	long   m_nSttWheelZedong;
	double m_dRail_UnitForceZedong;
	double m_dRail_TsbZedong;
	double m_dRail_LoadZedong;
	double m_dRail_MomentZedong;
	double m_dRail_HeightZedong;

	// Ȱ���� : ���߶�౳
	double m_dLiveLoaUR_W1;
	double m_dLiveLoaUR_W;
	double m_dLiveLoaUR_D;
	double m_dLiveLoaUR_i;
	double m_dLiveLoaUR_P;
	double m_dLiveLoaUR_Pvl1;
	double m_dLiveLoaUR_Pvl2;
	double m_dLiveLoaUR_Pvl; // ��ν����� ��������
	double m_dLiveLoaUR_Phl; // ���� ��������	
	double m_dLiveLoaUR_Ko;
	double m_dLiveLoaUR_Bo;
	double m_dLiveLoaUR_B;

	double m_dLoadLsApply[2];

	double m_dLoadTgValueMg[2];						// [��οµ��� �������][��� �µ��� ���� ���]
	double m_dLoadTgValueT[2];						// [��οµ��� �������][��� �µ��� ���� ���]

public:
	void CalculateSpanLength();
	double GetWeightHDan(long nHDan, BOOL bDW=FALSE);
	// ���� ���߿� ���� ���
	void CalculateDeadLoad();
	void DrawDiagramDeadLoad(CXLFormatText *XLText, BOOL bEarthquake=FALSE, BOOL bDW=FALSE);
	void DrawDiagramHoriPressure(CXLFormatText *XLText, BOOL bSoilPressure, BOOL bBothSide, BOOL bWaterCase, BOOL bEarthquake, EGoalOfEarthQuake eGoalEq);
	void DrawDiagramWaterPressureVertical(CXLFormatText *XLText, BOOL bLoadAll=TRUE, BOOL bBuoyancy=TRUE);
	void DrawDiagramWaterPressureInterior(CXLFormatText *XLText);
	void DrawDiagramEarthAnchor(CXLFormatText *XLText, BOOL bEarthQuake);
	void DrawDiagramLiveLoadUnderGroundRahmen(CXLFormatText *XLText, BOOL bEarthQuake=FALSE);
	void DrawDiagramSoilPressureVertical(CXLFormatText *XLText, BOOL bWater=FALSE, BOOL bEarthQuake=FALSE);
	void DrawDiagramLiveLoad_Lsd(CXLFormatText *XLText, BOOL bFatigue=FALSE, BOOL bEarthQuake=FALSE);
	void DrawDiagramLosdLS_ES_Lsd(CXLFormatText *XLText, BOOL bLeft, BOOL bEarthQuake=FALSE, BOOL bLS=TRUE);
	void DrawDiagramLiveLoadFootWay(CXLFormatText *XLText, BOOL bTitle, long &nIndexSmall, BOOL bEarthQuake=FALSE);
	void DrawDiagramUserLoad(CXLFormatText *XLText, long nTypeEQ, long nIndex);

	double GetEquivalentHeightWall(double dHeightWall);
	double GetCalculateRoadWayFactor();

	double GetSoilPressureForProtectionRiseFooting(BOOL bStt, BOOL bWater=FALSE, BOOL bUnderWater=FALSE, BOOL bIncludeAscon=FALSE);
	double GetHeightBackFillForProtectionRiseFooting(BOOL bStt, BOOL bWater=FALSE, BOOL bUnderWater=FALSE);

	double GetRForEffectiveWidth(CRcBridgeRebar *pBri, HGBOOL bApplyMin=TRUE);
	double GetEForEffectiveWidth(CRcBridgeRebar *pBri, HGINT32 iLane);

private:
	// ���� ���߿� ���� ���
	void CalculateLiveLoad();
	void CalculateLiveLoadUnderGround();
	void CalculateLiveLoadTank();
	void CalculateRailLoadLive();
	void CalculateRailLoadLR();
	void CalculateRailLoadSB();
	void CalculateSoilPressure();
	void CalculateWaterLoad();
	void CalculateTemperature();
	void CalculateDryAndShrinkage();
	void CalculateSubsidence();
	void CalculateLoadLs_Lsd();
	void CalculateLoadTG();

	CString GetStringEquivalentHeightCondition(double dHeightWall);
	
	double GetAngleForEffectiveWidth(CRcBridgeRebar *pBri);
	double GetLaneFactor(CRcBridgeRebar *pBri, HGINT32 iLane);

	double GetTemperatureSlopeH(long nPosition);
	double GetTemperatureSlopeTz(long nPosition, HGDOUBLE dZi, HGBOOL bLowerTemperature=FALSE);
	double GetTemperatureSlopeS(long nPosition, HGDOUBLE dZi, HGDOUBLE dTz, HGBOOL bS2);
	double GetCoefficientImpactFactor(double dLenSpan, double &dFactor1, double &dFactor2, double &dLenGap);

private:
	void MakeTableImpactCoeffience(CXLFormatText *XLText, BOOL bYoRyung);
	void DrawDiagramModeling(CDomyun *pDomP, BOOL bUpperOffset, BOOL bDrawNLLink);
	double m_dDimScaleDiagram;
	void DrawDiagramLiveLoad(CXLFormatText *XLText);
	void DrawDiagramLiveLoadRail(CXLFormatText *XLText);
	void DrawDiagramLiveLoadTank(CXLFormatText *XLText, BOOL bTitle, long &nIndexSmall);
	void DrawDiagramSubsidence(CXLFormatText *XLText);
	void DrawDiagramRailLoadLR(CXLFormatText *XLText);
	void DrawDiagramRailLoadSB(CXLFormatText *XLText);
	void DrawDiagramShrink(CXLFormatText *XLText);
	void DrawDiagramTemperature(CXLFormatText *XLText, BOOL bPlus, BOOL bTU=TRUE);

	void MakeTableIEquivalentHeight(CXLFormatText *XLText, long nColStt);
	void MakeTableIRoadWaysFactor(CXLFormatText *XLText, long nColStt);

	double m_dUnderGroundDesignHeight;

	CString GetStringAreaArray(CDoubleArray &dArrArea);
	void InitializeVariables();

	long GetCountUserLoadType(long nLoadType, long ixLoadSet, BOOL bEarthquake, long nTypeEQ);

	CDoubleArray m_dArrUpperWeight[2];
	CDoubleArray m_dArrAreaBangHo[2];
	CDoubleArray m_dArrAreaCentro[2];
	CDoubleArray m_dArrAreaBoCha[2];
	CDoubleArray m_dArrAreaNanganBase[2];		// ��������
	CDoubleArray m_dArrAreaSoundBase[2];		// ����������
	CDoubleArray m_dArrAreaRetainingWall[2];	// �븷�̺�

	double m_UW_Ascon;
};

#endif // !defined(AFX_ARCBRIDGECALCLOAD_H__E7A0BB5A_EDB2_40D8_89C5_9F41DD25BFBD__INCLUDED_)
