// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDesignFrame.h: interface for the CARcBridgeDesignFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDESIGNFRAME_H__A6DB8C0B_B8A9_4546_9F7D_CD6470C40E0B__INCLUDED_)
#define AFX_ARCBRIDGEDESIGNFRAME_H__A6DB8C0B_B8A9_4546_9F7D_CD6470C40E0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJoint;
class CElement;
class CFEMManage;
class CSapOutput;
class CJointFrame;
#include "../ARcBridgeData/ARcBridgeData.h"
class AFX_EXT_CLASS CARcBridgeDesignFrame  
{
public:
	void MakeSapData(long nTypeDesign, BOOL bRemoveAllData);
	void MakeSapDataElement(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataJoint(CFEMManage *pFEM, BOOL bEarthQuake);
	CARcBridgeDesignFrame();
	virtual ~CARcBridgeDesignFrame();

	CARcBridgeCalcStd *m_pStd; 

	CDoubleArray m_dArrThickSlab;		// 슬래브의 두께
	CDoubleArray m_dArrThickSpring;		// 하부슬래브 스프링 위치 두께
	CDoubleArray m_dArrThickWall;		// 벽체의 두께
	CDoubleArray m_dArrThickArchRib;	// 아치교 리브
	//
	CDoubleArray m_dArrColumnArea;	// 기둥의 면적
	CDoubleArray m_dArrColumnI33;	// 기둥의 단면2차모멘트
	CDoubleArray m_dArrColumnI22;	// 기둥의 단면2차모멘트
	CStringArray m_sArrColumnDes;	// 기둥의 단면2차모멘트

public:
	CJoint* GetJointJijumSpring(CFEMManage *pFEM, long nJijum);
	CJoint* GetJointJijumFix(CFEMManage *pFEM, long nJijum);
//	CJoint* GetJointMatchArchRib(CFEMManage *pFEM, long nJijum);
	double GetKsValueJointWall(CFEMManage *pFEM, long nJijum, long nIndexJoint, double &dLenPrev, double &dLenNext);
	double GetKrValueJointWall(CFEMManage *pFEM, long nJijum, long nIndexJoint, double &dLenPrev, double &dLenNext);
	CString GetStringApplyJoint(CFEMManage *pFEM, long nType, long nNum);		// nType - 0: 슬래브, 1: 벽체
	CString GetStringApplyJoint(CLongArray lArr);
	CElement* GetElementOnJointIdx(CFEMManage *pFEM, long nJIdx, BOOL bStt);
	CElement* GetElementWallFix(CFEMManage *pFEM, long nJijum);
	CElement* GetElementWallMatchArchRib(CFEMManage *pFEM, long nJijum);
	double GetLengthBetweenJoint(CFEMManage *pFEM, long nJoint1, long nJoint2);
	long GetNumberJointJijumFix(CFEMManage *pFEM, long nJijum);
	long GetNumberJointJijumSpring(CFEMManage *pFEM, long nJijum);
	long GetNumberElementSlabStt(CFEMManage *pFEM, BOOL bUpperSlab = TRUE);
	long GetNumberElementSlabEnd(CFEMManage *pFEM, BOOL bUpperSlab = TRUE);
	long GetNumberJointSlabStt(CFEMManage *pFEM, BOOL bUpperSlab = TRUE);
	long GetNumberJointSlabEnd(CFEMManage *pFEM, BOOL bUpperSlab = TRUE);

	void MakeSapDataConstraints(CFEMManage *pFEM);
	void MakeSapDataFrameSection(CFEMManage *pFEM, BOOL bEarthQuake);

	void MakeSapDataNLProperty(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataNLLink(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataMode(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataHistoryNormal(CFEMManage *pFEM);
	void MakeSapDataHistoryEarthquake(CFEMManage *pFEM);

	void MakeSapDataMaterial(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataLoadNormal(CFEMManage *pFEM);
	void MakeSapDataLoadWater(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataLoadAnchor(CFEMManage *pFEM, long &nLoadCaseIndex);
	void MakeSapDataLoadLiveAlways(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, BOOL bApplyImpact=TRUE);
	void MakeSapDataLoadLiveFootWay(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataLoadSubsid(CFEMManage *pFEM, long &nLoadCaseIndex);
	void MakeSapDataLoadLiveLoadAll(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataLoadTemperature(CFEMManage *pFEM, long &nLoadCaseIndex);
	void MakeSapDataLoadUserLoad(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, long nTypeDesing=0);
	void MakeSapDataEQLoadHoriSoilPress(CFEMManage *pFEM, BOOL bLeftWallToap, long &nLoadCaseIndex);
	void MakeSapDataEQLoadInertialForce(CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq, long &nLoadCaseIndex);
	void MakeSapDataCombineNormal(CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate);
	void MakeSapDataCombineLiveLoad(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataCombineSubsid(CFEMManage *pFEM, BOOL b3DPlate);
	void MakeSapDataCombineSoilPressure(CFEMManage *pFEM);

	void MakeSapDataSpringEQ(CFEMManage *pFEM, BOOL bLeftWallToap);
	void MakeSapDataUnitLoadEQ(CFEMManage *pFEM);
	void MakeSapDataLoadEQ(CFEMManage *pFEM, BOOL bLeftWallToap);
	void MakeSapDataLoadEQ_BoxType(CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq);
	void MakeSapDataCombineEQ(CFEMManage *pFEM, BOOL bUnitLoad);

	void MakeSapDataLoadDead_Lsd(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataLoadHoriSoilPress_Lsd(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, EGoalOfEarthQuake eGoalEq=eGoalEQ_None);
	void MakeSapDataMode_Lsd(CFEMManage *pFEM, BOOL bEarthQuake);
	void MakeSapDataLoadNormal_Lsd(CFEMManage *pFEM);
	void MakeSapDataHistoryNormal_Lsd(CFEMManage *pFEM);
	void MakeSapDataHistoryEarthquake_Lsd(CFEMManage *pFEM);
	void MakeSapDataCombineNormal_Lsd(CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate);
	void MakeSapDataLoadEQ_Lsd(CFEMManage *pFEM, BOOL bLeftWallToap);
	void MakeSapDataLoadEQ_BoxType_Lsd(CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq, long nTypeDesign);
	void MakeSapDataCombineEQ_Lsd(CFEMManage *pFEM, BOOL bUnitLoad, BOOL bLeftWallToap, long nTypeDesign=0);
	void MakeSapDataLoadLS_Lsd(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataLoadES_Lsd(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake);
	void MakeSapDataCombineEnv_Lsd(CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate);
	void MakeSapDataCombineEtaCase_Lsd(CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate);

	void WriteModelingNormal(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteModelingEQ(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	void WriteModelingCoefficienOfPile(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WriteModelingCoefficienOfSubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bEarthQuake);
	void WriteModeling3DShell(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
	// nType-> 0: 평상시(Normal), 1: 지진시(EQ)
	void WriteRigidEnd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType);
	void DrawRigidEnd(CDomyun *pDomP, CRcBridgeRebar *pBri, long nIndex ,BOOL bUpperSlab);
	void DrawRigidEndSlopeInWall(CDomyun *pDomP, CRcBridgeRebar *pBri, long nIndex);
	void MakeTableXZ(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType);
	void MakeTableSectAffection(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType); 
	void MakeTableSpringValueFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri); 
	void MakeTableSpringValueWall(CXLFormatText *XLText, CRcBridgeRebar *pBri); 
	void MakeXyCoordinates(BOOL bEarthQuake);
	void MakeArrayThick();

	BOOL IsAvailableJijumFix(CFEMManage *pFEM);
	void GetArrayAnchorForceAndDist(CFEMManage *pFEM, CElement *pElement, CDoubleArray &dArrAnchorForce, CDoubleArray &dArrDistAnchor, BOOL bEarthQauake);
	BOOL IsUseMaterialLowerSlab();
	BOOL IsUseMaterialEndWall();
	BOOL IsUseMaterialInnerWall();
	CString GetMaterialNameByValue(CFEMManage *pFEM, double dEc);

//	CTypedPtrArray <CObArray, CJointFrame*> m_pListSlab;
//	CTypedPtrArray <CObArray, CJointFrame*> m_pListWall[MAX_JIJUM_COUNT_RC];
	CVectorArray m_vArrSlab[2]; // 상부슬래브 + BOX교인 경우 하부슬래브
	CVectorArray m_vArrWall[MAX_JIJUM_COUNT_RC];
	CVectorArray m_vArrArchRib;
	CVector m_vMatchArchAndInWall[MAX_JIJUM_COUNT_RC];
	CDPointArray m_xyArrSpringLower;	// 하부슬래브 스프링 위치
	CDPointArray m_xyArrSpringWall[2];  // 벽체 스프링 위치
	CVectorArray m_vArrForceLower[2];	// 하부슬래브 스프링 위치, 상시,지진시
	CVectorArray m_vArrForceWall[2];	// 벽체 스프링 위치  지진시 / 좌,우

private:
	void MakeTablePileSpringFactor(CXLFormatText *pXLText, CRcBridgeRebar *pBri, BOOL bEarthquake);
	void SetSpringForce(BOOL bEarthQuake);
	void MakeXyCoordinates_SlabNode(BOOL bUpperSlab, BOOL bEarthQuake);
	void MakeXyCoordinates_SlabRigid(BOOL bUpperSlab);
	void RemoveAndSortXyCoordinates_Slab(BOOL bUpperSlab);
	void MakeXyCoordinates_ArchRib(BOOL bEarthQuake);

//	void MakeSapDataNLLinkEQ(CFEMManage *pFEM);
	double GetShoeKvValue(long nJijum, BOOL bUnitMeter);
	double GetShoeKhValue(long nJijum, BOOL bUnitMeter);
	void ReCalculateCoordinateBySkew(CFEMManage *pFEM);
	BOOL IsRestraintJoint(CJoint *pJoint);
	long GetCountSpringWall(CRcBridgeRebar *pBri);
	double GetDistRigidElement(CFEMManage *pFEM, CElement *pElement);
	long GetIndexSection(double dThick, BOOL bSlab, BOOL bArch=FALSE);
	void MakeSapDataNormal(BOOL bRemoveAllData);
	void MakeSapDataEQ(long nTypeDesign, BOOL bRemoveAllData);
	CJoint* GetJointJijumOnSlab(CFEMManage *pFEM, long nJijum, BOOL bFirst, BOOL bBottomJoint = FALSE, BOOL bUpperSlab = TRUE);
	CJoint* GetJointWallHunchEnd(CFEMManage *pFEM, long nJijum);
//	CJoint* GetJointWallTop(CFEMManage *pFEM, long nJijum);

	long GetIndexJointLowerSlabMid(CFEMManage *pFEM);
	double  GetThickElementAvg(CFEMManage *pFEM, CElement *pElement);

	long m_nCountSubsidence;
	BOOL m_bHistory[2];
};

#endif // !defined(AFX_ARCBRIDGEDESIGNFRAME_H__A6DB8C0B_B8A9_4546_9F7D_CD6470C40E0B__INCLUDED_)
