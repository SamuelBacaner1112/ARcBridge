// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeClacEarthQuake.h: interface for the CARcBridgeCalcEarthQuake class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCEARTHQUAKE_H__FE2D8658_7CD8_428F_9585_85EB27A63885__INCLUDED_)
#define AFX_ARCBRIDGECALCEARTHQUAKE_H__FE2D8658_7CD8_428F_9585_85EB27A63885__INCLUDED_

#include "RigidityFooting.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRigidityFooting;

class AFX_EXT_CLASS CARcBridgeCalcEarthQuake  
{
public:
	CARcBridgeCalcEarthQuake();
	virtual ~CARcBridgeCalcEarthQuake();

	CARcBridgeCalcStd *m_pStd;
	void CalculateAll();

public:
	void WriteLoadDiagram(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteLoadDiagram_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	CRigidityFooting* GetRigidityFootingByJijumNumber(long nJijum);
	void DrawFootingPileBase(CDomyun *pDomP, long nJijum);
	void WriteCalcFootingSpring(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle);
	//
	void WriteCalcLoadDirX(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLoadDirY(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLoadSoilPressure(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteCalcLoadInertialForce(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);

	double GetDisplacement(long nJoint, BOOL bDirBridge);
	double GetMaxDisplacementUpper(BOOL bDirBridge, BOOL bAvg=FALSE);
	CRigidityFooting* GetRigidityFooting(long nJijum);
	CTypedPtrArray <CObArray, CRigidityFooting*> m_pListRighty;	// 기초의 강성에 대한 배열

	double m_UW_Conc;
	double m_dWidthSlab;
	double m_DeadLoadDistribution;	// 분포사하중
	double m_DeadLoadDistributionLsd[2];
	double m_dWeightSlab;
	double m_dWeightWall;
	double m_dWeightHunch;
	CDoubleArray m_dArrWeightWall;
	CDoubleArray m_dArrWeightHunch;
	CStringArray m_sArrWeightHunch;
	CStringArray m_sArrWeightWall;
	double m_dWeightUpper;
	double m_DeadLoadSoil;

	double m_alpha[CALC_DIR];		// 종방향,횡방향
	double m_beta[CALC_DIR];
	double m_gamma[CALC_DIR];
	double m_T[CALC_DIR];
	double m_Cs[CALC_DIR];
	double m_Pe[CALC_DIR];
	CDoubleArray m_ArrPe;

	double m_ToapEQ[2];				// 시종점벽체 - 경사말뚝이 아닌 경우는 동일
	double m_ToapNormal[2];			// 시종점벽체
	double m_HeightToap[2];
	double m_HeightWall[2];

	CDoubleArray m_dArrSoilPressure[eGoalEarthQuakeCount][2];	// 붕괴방지, 기능수행 / 좌ㅡ우
	CDoubleArray m_dArrSoilHeight[eGoalEarthQuakeCount];		// 적용토압의 배열...
	double m_InertialForce_Slab[eGoalEarthQuakeCount][2]; 
	double m_InertialForce_Wall[eGoalEarthQuakeCount][2];

	CMatrix m_MatDispUnit;			// 단위하중재하시의 변위, Row-절점, Col-방향
	void DrawDiagramModeling(CDomyun *pDomP, BOOL bUpperOffset);

private:
	double m_dDimScaleDiagram;
	void WriteCalcFootingSpring_Combine(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteCalcFootingSpring_Wall(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteCalcFootingSpring_Direct(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteCalcFootingSpring_Pile(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum);
	void WriteCalcLoadUpperWeight(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTiny);
	double GetScaleDiagram(long nRow);
	void DrawDiagramUnitLoad(CDomyun *pDomP, long nSizeRow, BOOL bDirBri);
	void DrawDiagramDeadLoad(CXLFormatText *XLText);
	void DrawDiagramLoadEQ(CXLFormatText *XLText);
	void DrawDiagramSoilPressure(CXLFormatText *XLText, BOOL bLeftWall);
	void DrawDiagramLoadInertialForce(CXLFormatText *XLText, EGoalOfEarthQuake eGoalEq);

	void ReadSapOutUnitDisplacement();
	void CalculateSoilPressure();
	void CalculateCoefficientXDir();
	void CalculateCoefficientYDir();
	void CalculateUnitLoad();
	void CalculateRigidityAbut(BOOL bStt);
	void CalculateRigidityPier(long nJijum);
	double GetSa(CRcBridgeRebar *pBri, double dTime);

	void WriteTableGroundAmplificationFactor(CXLFormatText *pXLText);
	void WriteResponseFactor_KDS17(CXLFormatText *pXLText, CRcBridgeRebar *pBri, BOOL bBriDir);
	double GetGroundAmplificationFactor(long nTypeGround, double dS, BOOL bLong=FALSE);
};

#endif // !defined(AFX_ARCBRIDGECALCEARTHQUAKE_H__FE2D8658_7CD8_428F_9585_85EB27A63885__INCLUDED_)
