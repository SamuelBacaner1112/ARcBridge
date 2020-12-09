// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcStd.h: interface for the CARcBridgeCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCSTD_H__2886A2F3_8181_423D_865C_29E017071FEC__INCLUDED_)
#define AFX_ARCBRIDGECALCSTD_H__2886A2F3_8181_423D_865C_29E017071FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CALC_DIR_X 0
#define CALC_DIR_Y 1
#define CALC_DIR   2

#define COL_POS_1	1
#define COL_POS_2	2
#define COL_POS_3	3
#define COL_POS_4	4

#define POS_L_UP 0 // 좌측 상단
#define POS_R_UP 1 // 우측 상단
#define POS_L_LO 2 // 좌측 하단
#define POS_R_LO 3// 우측 하단	

#define NUM_CIRCLE	0
#define NUM_GANA	3

#define ROUND_POS_DISP 10

#define ALPHA_EARTHQUAKE		2
#define ALPHA_NORMALTIMES		1

// #define EQ_BOX_COLLAPSE	0
// #define EQ_BOX_FUNCTION	1
// enum EGoalOfEarthQuake { eGoalEQ_None = -1, eGoalEQ_Collapse, eGoalEQ_Function, eGoalEQ_Immediately, eGoalEQ_LongTerm, eGoalEarthQuakeCount };

//
#define UNLIMITED_RIGIDITY	100

//////////////////////////////////////////////////////////////////////////
class CRcBridgeRebar;
class CARcBridgeCalcLoad;
class CARcBridgeDesignFrame;
class CARcBridgeDesignShell;
class CARcBridgeCalcEarthQuake;
class CARcBridgeCalcAps;
class CARcBridgeMeshCalc;
class CARcBridgeCalcCornerAndSupport;
class CDesignPile;
class CARcBridgeCalcFooting;
class CARcBridgeCalcWing;
class CARcBridgeCalcColAndGir;
class CARcBridgeCalcFatigue;
class CARcBridgeCalcSummary;
class CARcBridgeCalcArchRib;
class CModulusFoundation;
class CARcBridgeCalcStm;
class CCalcCantilever;

class AFX_EXT_CLASS CARcBridgeCalcStd  
{
public:
	CARcBridgeCalcStd(CRcBridgeRebar *pRcBridge);
	virtual ~CARcBridgeCalcStd();

	CDocument* m_pDoc;
	CView* m_pView;
	CARoadOptionStd* m_pARoadOptionStd;
	
	CRcBridgeRebar		*m_pBri;

	CARcBridgeCalcLoad			*m_pLoad;
	CARcBridgeCalcEarthQuake		*m_pLoadEQ;
	CARcBridgeDesignFrame			*m_pFrame;
	CARcBridgeDesignShell			*m_pShell;
	CARcBridgeCalcAps				*m_pApsStt[2];
	CARcBridgeCalcAps				*m_pApsEnd[2];
	CARcBridgeMeshCalc			*m_pMeshCalc;
	CARcBridgeCalcCornerAndSupport  *m_pCorner[2];
	CARcBridgeCalcWing			*m_pWing[4]; // 시점좌측,시점우측,종점좌측,종점우측
	CARcBridgeCalcColAndGir		*m_pColGir;  // 보기둥
	CModulusFoundation			*m_pModulusFoundation; // 함형라멘교 지반계수
	CARcBridgeCalcStm			*m_pStm;
	CCalcCantilever			*m_pCantilever;

	// ENGINE
	CSafetyTypedPtrArray <CObArray, CDesignPile*> m_pArrPileCalc;
	CARcBridgeCalcFooting		*m_pFooting;
	CARcBridgeCalcFatigue		*m_pFatigue;
	CARcBridgeCalcSummary		*m_pSummary;
	CARcBridgeCalcArchRib		*m_pArchRib;

public:
	void DrawDiagramSlabDistribution(CDomyun *pDomP, CFEMManage *pFEM, double dLoadValue, double dOffset=0, double dLen=0, BOOL bUpperSlab = TRUE, BOOL bHeightHalf=FALSE, BOOL bAll=TRUE);
	void DrawDiagramJointLoad(CDomyun *pDomP, CFEMManage *pFEM, long nJoint, CVector vLoad, BOOL bMoment);
	void DrawDiagramElementLoad(CDomyun *pDomP, CFEMManage *pFEM, long nElement, long nDir, long nTypeLoad, double dDist1, double dDist2, double dLoad1, double dLoad2);
	void DrawDiagramProtectionRiseFooting(CDomyun *pDomP, CFEMManage *pFEM, double dLoadValueLeft, double dLoadValueRight, double dLen=0);
	double GetRebarAreaSuportBar(CConcBasic *pConc, BOOL bTension, BOOL bCompression, BOOL bUnitCm2=TRUE);
	void DrawModelingSpringAndFix(CDomyun *pDomP, CFEMManage *pFEM);
	void DrawModelingSpringAndNLLink(CDomyun *pDomP, CFEMManage *pFEM);
	void DimModelingSlab(CDomyun *pDomP, CFEMManage *pFEM, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge=FALSE, BOOL bUpperSlab=TRUE);
	void DimModelingArchRib(CDomyun *pDomP, CFEMManage *pFEM, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge=FALSE);
	void DimModelingWall(CDomyun *pDomP, CFEMManage *pFEM, long nJijum, BOOL bJointMark, BOOL bEleMark, BOOL bDim, BOOL bMerge=FALSE);
	CDRect DrawDiagramModeling(CDomyun *pDomP, CFEMManage *pFEM, BOOL bDrawNLLink = TRUE);
//	BOOL IsDistributionLoadSlab(CString strLoad);
	double GetScaleDiagram(long nRow, long nType);
	CDesignPile* GetDesignPile(long nJijum);
	BOOL IsSkew();
	double GetThickSlabByDist(long nJijum, double DistFromJijum, BOOL bUpperSlab);
	double GetThickWallByDist(long nJijum, double DistFromSlab, BOOL bFrBottom=FALSE);
	double GetThickArchRib(double dSta);
	void CalculateCofficient();
	long GetQtyRailWheel(); 
	double GetSlabWidth(long nJijum=-1);
	CDPoint GetAngleArchRibBySta(double dSta);
	double GetElDiffModelingEnd();
	double m_dCoefficientKo;			// 정지토압계수

	double GetCoefficient_coulomb();
//	double GetCoefficient_Rankine();
	double GetCoefficient_MononobeOkabe(long nIdxPos);
	double GetKhValue(BOOL bFixdMovement, BOOL bStt=TRUE, EGoalOfEarthQuake eGoalEq=eGoalEQ_None);
	double GetWheelLoadRear();
	double GetWheelLoadMiddle();
	double GetWheelLoadFront();
	double GetWheelLoadPm();
	double GetWheelLoadPs();
	double GetWheelLoadDistribution();
	double GetLenSpanMaxOut(BOOL bVert=FALSE);

	double GetDesignRoadWidth();
	long GetCountLane(double dWidth=0);
	void CalculateAll();
	double GetLengthModelSpanMin();
	double GetLengthModelSpanMax();
	double GetLengthModelSpanAvg();
	void SetCalcPileData(CDesignPile *pCalcPile);
	double GetCofficientForAlwaysLiveLoad(long nTypeMethod=-1);

	void WriteCalcCifficientKo(long nTypeMethod, CXLFormatText *XLText, long nIdxTitle, long nColStt, BOOL bTitle=FALSE);

	void SetDomyunScale(CDomyun *pDomP, long nRows);
	void MakeStmFrameSection(CStmModel *pStm);

private:
	double m_dCoefficient_Rankine;		// 시점벽체,종점벽체
	double m_dCoefficient_Coulomb;
	double m_dCoefficient_MononobeOkabe[eGoalEarthQuakeCount];		// 0: 시점, 1: 종점
	void CalculateCofficient_Rankine();
	void CalculateCofficient_coulomb();
	void CalculateCofficient_MononobeOkabe();
	void GetLengthModelSpanMinMax(HGDOUBLE &dMin, HGDOUBLE &dMax, HGDOUBLE &dAvg);

	CExPileData *m_pPileEx;
};

#endif // !defined(AFX_ARCBRIDGECALCSTD_H__2886A2F3_8181_423D_865C_29E017071FEC__INCLUDED_)
