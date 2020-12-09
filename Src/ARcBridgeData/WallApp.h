// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WallApp.h: interface for the CWallApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALLAPP_H__9E894664_816B_4E62_AA0C_1C71A9EFED54__INCLUDED_)
#define AFX_WALLAPP_H__9E894664_816B_4E62_AA0C_1C71A9EFED54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WallData.h"
typedef CSimpleTypedArray <CRebarJoint> CJointArray;

class AFX_EXT_CLASS CWallApp : public CWallData  
{
public:
	double GetLength();
	void SetDataDefaultRangeSupportRebarForSameUpperSlab();
	void SyncWall_DesignData();
	void CheckOverLapLineBetweenSubObject();
	double GetWidthFrontLower();
	CFootingApp* GetFootingByRotateToVert(BOOL bLeft);
	double GetAngleWallByHor(BOOL bApplyAngle=FALSE);
	double GetAngleUpperSlab(BOOL bApplyAngle=FALSE);
	BOOL IsVertWall();
	void SetDataDefaultRebarShearFront_Footing(BOOL bFront);
	void SetDataDefaultRebarSupportFront_Footing(BOOL bUpper);
	CDPoint GetXyDirRotate_Front();
	CDPoint GetXyOrgTopForFront(BOOL bLeft);
	CDPoint GetXySttRebarSupport_Front(long nDan, BOOL bLeft, BOOL bVertDir);
//	CTwinVector GetTvCenterOfRoad_Side(BOOL bLeft, double dExtLen);
	double GetLengthExt23DanRebar(BOOL bLeft);
	void ApplyExtLength23DanMainRebar(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, double dLengthExt23DanRebar, CTwinVector tvRebarUpperSlab);
	BOOL IsExistMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx);
	CDPoint GetXyMatchXAndTvArr(double dX, CTwinVectorArray &tvArr);
	double GetDiffHeightByCenterOfRoad(CTwinVectorArray &tvArr, double dX, double dOrg);
	double GetELBottomOfWall();
	BOOL SetDiaMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx, double dDia, BOOL bMax=FALSE);
	double GetDistFromFootingToColumn(BOOL bSttColumn);
	BOOL IsRoundRectWall();
	BOOL IsEnableRoundRectWall();
	void GetOffsetInfoRebarMain_Plane_LineSection(CDPointArray &xyArrOffsetInfo, BOOL bLeft, long nDan);
	void SetColumnForRoundRectWall();
	void GetInfoCuttingCircle_Side(BOOL bLeft, CDPoint &xyCen, double &dR, double &dAngStt, double &dAngEnd);
	double GetValueJoint(CRebarInfoCycle *pRCCur, double dDiaNext);
	double GetDistMoveJointByTypeDefaultCycle3(long nTypeDefaultCycle3, double dLenJoint);
	double GetDistMoveJointCycle13(BOOL bCycle3);
	BOOL GetJointAtRebarInfoCycle(CRebarInfoCycle *pRC1, CRebarInfoCycle *pRC2, CRebarJoint &joint);
	void GetJointAtRebarInfoCycleArray(CJointArray &arrJoint, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC);
	void MakeJointMainRebar(CJointArray &arrJoint, long nCycle, BOOL bLeft, long nDan);
	double GetDistMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx, BOOL bAccumulation);
	void SetDataDefaultRebarJewonColumn(long nBaseNum, long nJijum);
	CColumnApp* GetColumn(long nIdx);
	void GetTvCuttingArea_Side_Shear(CTwinVectorArray &tvArr, long nDrawLeft);
	double GetCTCRebarShear();
	void SetDataDefaultRebarJewonShear(long nBaseNum);
	double GetLenShear();
	void GetTvRebarShear_Side(CTwinVectorArray &tvArr, BOOL bLeftWall);
	void GetTvRebarShear_Plane(CTwinVectorArray &tvArr);
	void ClearRebarList(long nType);
	double GetCTCRebarSupport(BOOL bLeft, long nDan, BOOL bStd=FALSE);
	void SetDataDefaultRebarJewonSupport(long nBaseNum);
	void SetDataDefaultRebarInfoCycle(long nCycle, BOOL bOnlyCountArray=FALSE, long nDan=0, BOOL bOnlyDiffJoint=FALSE);
	void DivideTvArrayByRound(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRound, double dLenRound, BOOL bRoundAtStt);
	void GetTvCuttingArea_Side(CTwinVectorArray &tvArr, BOOL bLeft, long nDan, BOOL bUpper);
	double GetDistArm(BOOL bLeft);
	double GetHeightArm(BOOL bLeft);
	void SetDataDefaultRebarJewonMain();

	CDPoint GetXyCenterColumn_Left(long nColumn);
	long GetCountRebarInfoCycle(long nCycle, long nDan, BOOL bLeft);
//	CTwinVector GetTvRoadCenter_Side(BOOL bLeft, double dExtLen=0);
	CTwinVector GetTvCenter_Side(BOOL bLeft, double dExtLen=0);
	void GetTvOffset_Side(CTwinVectorArray &tvArr, BOOL bLeft, long nDan);
	void GetTvRebarMain_Side(CTwinVectorArray &tvArr, BOOL bLeftRebar, long nCycle, long nDan, long nIdxRC, CTwinVectorArray &tvArrJoint, BOOL bIncludeRound=TRUE, BOOL bLeft=TRUE, BOOL bForDim=FALSE);
	void GetDistRebarSupport_Front(CDoubleArray &dArr, BOOL bLeft, long nDan, BOOL bCoverLocation = FALSE);
	void GetTvRebarSupport_Side(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngle, CTwinVectorArray &tvArrAngleAdd, BOOL bLeftWall, long nDan, BOOL bLeft=TRUE, BOOL bCutAddRebar=TRUE, BOOL bCoverLocation = FALSE);
	BOOL IsApplyAngle();
	void InvertRebarInfoCycle(CRebarInfoCycle *pRC);
	void AddRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrDest, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrSource, BOOL bInvert=FALSE);
	CDPoint GetXyMatchUpperSlab(double dXInLeft, BOOL bBottomOfHunch=FALSE);
	void GetTvLeftSide(CTwinVectorArray &tvArr, BOOL bGetWall=FALSE, BOOL bIncludeFooting=FALSE, BOOL bIncludeUpperSlab=FALSE, BOOL bLeft=TRUE, long nDrawLeft=0, BOOL bForRebar=FALSE, BOOL bVertDir=TRUE, BOOL bOnlyViewFromLeft=FALSE, BOOL bPattern=FALSE, BOOL bCorrectionEL=FALSE);
	void SetDataDefaultRebarShear_VertSection();
	void SetDataDefaultRebarSupport_VertSection_InWall(BOOL bLeft);
	void GetTvRebarShear_Front(CTwinVectorArray &tvArr, BOOL bVertDir=TRUE);
	CRebarInfoCycle *GetRebarInfoCycle(long nCycle, long nDan, BOOL bLeft, long nIdx);
	void GetXyRebarSupport_Front(CDPointArray &xyArr, BOOL bLeft, long nDan, BOOL bApplyRebarRadius=TRUE, BOOL bVertDir=TRUE);
	double GetLenRebarAtEdge(double dDia, BOOL bInFooting);
	long GetDanMainRebar(long nCycle, BOOL bLeft);

	// MakeRebarInfoCycle후 bExist가 FALSE인 것을 완전히 제거하는 함수
	void RemoveNotExistRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC);

	void SetStringDescriptionRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bLeft, long nDan, long nCycle);
	// 배근 관련 함수 /////////////////////////////////////////////////////////////
	double GetDiaMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx);
	void MakeRebarInfoCycle(long nCycle, BOOL bLeft, long nDan, BOOL bInput, BOOL bMarkInitAtFirst=TRUE, BOOL bVertDir=TRUE);
	CTwinVector GetTvOffset_Front(long nCycle, long nDan, BOOL bArm, BOOL bLeftArm, BOOL bLeftRebar, BOOL bVertDir);
	///////////////////////////////////////////////////////////////////////////////

	void SyncWall(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle=FALSE, double dAngle=90);
	BOOL SetDataDefaultShapeFooting();
	CTwinVector GetTvWallTopCen(BOOL bUpper=TRUE);
	void SetDataDefaultDistColumn(BOOL bDefaultCountColumn=FALSE);
	CDPoint GetXyDirTop();
	CTwinVector GetTvWallTopCenOfTypeV(BOOL bLeft);
	BOOL GetTvFront(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle=FALSE); 
	BOOL GetTvPlan();
	double GetWidthWall();
	BOOL IsLeftSlope();
	CDPoint GetXyWallTopCen(BOOL bLeft);
	double GetColumnMainRebarDia(long nCycle, long nDan, BOOL bLeft);

	CWallApp();
	virtual ~CWallApp();

	CDPoint GetXyOrgFootingLowerForSide(BOOL bLeft);
	void GetXyRebarMain_Plan_For_Round_Wall(CDPointArray &xyArr, CDPointArray &xyArrDirApplyScale, long nDan, BOOL b2Cycle);

	void SetRebarLsdOption(SRebarEnvOption *pSLsdOption, BOOL bLeft, BOOL bMain, long nCycle, long nDan);
};

#endif // !defined(AFX_WALLAPP_H__9E894664_816B_4E62_AA0C_1C71A9EFED54__INCLUDED_)
