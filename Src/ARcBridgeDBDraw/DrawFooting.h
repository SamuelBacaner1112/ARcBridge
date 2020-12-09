// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawFooting.h: interface for the CDrawFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWFOOTING_H__725E49A5_848B_4CE1_B47A_EFD76EC28294__INCLUDED_)
#define AFX_DRAWFOOTING_H__725E49A5_848B_4CE1_B47A_EFD76EC28294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
class CLineInfoApp;

static CTwinVectorArray staticTvArrReturnFooting;
class AFX_EXT_CLASS CDrawFooting  
{
public:
	void MakrPlaneConcFck(CDomyun *pDom, CFootingApp *pFooting);
//	void DrawRebarSupport_Plane_Slope(CDomyun *pDomP, CFootingApp *pFooting, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight);
//	void DrawRebarMain_Plane_Slope(CDomyun *pDomP, CFootingApp *pFooting, BOOL bMarkRebar, long nDrawUpper, BOOL bDimForTopIsRight);
	void DrawRebarSupport_FrontSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarMain_FrontSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	CDRect DrawRebar_Plane_ObtuseStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight, long nRotate, BOOL bInput=TRUE);
	void DimRebarRange_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront);
	void DrawRebarSupport_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight);
	void DrawRebarMain_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight);
	void DrawRebarSupport_FrontObtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, long nVertDir, BOOL bInput);
	void DrawRebarMain_Front_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarShear_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nDrawLeft);
	void ApplyCuttingArea_Plane(CDomyun *pDomP, CFootingApp *pFooting, CTwinVectorArray &tvArr, BOOL bUpper, BOOL bAtLeft, long nDan, BOOL bDrawCuttingArea, BOOL bExt, BOOL bSlope=FALSE);
	CDRect DrawRebar_SideStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarSupport_Side(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarSideOut_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bLeft, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDimDan=0);
	void DrawRebarSupport_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight=TRUE, long nExt=-1, BOOL bObtuse=FALSE, BOOL bFront=FALSE, BOOL bSlope=FALSE);
	void DrawRebarMain_Side(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bDimTotal, BOOL bMarkRebar, BOOL bInput);
	CDRect DrawRebar_PlaneStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpperAtUpper, BOOL bUpperAtLower, BOOL bMarkRebar, BOOL bDimRebar, long nRotate=-1, BOOL bDrawPathPlane=FALSE, BOOL bInput=FALSE);
	void DrawRebarMain_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight, BOOL bExt, BOOL bObtuse=FALSE, BOOL bFront=FALSE, BOOL bSlope=FALSE);
	CDRect DrawRebar_FrontSectionStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDimSide=FALSE, BOOL bDimRebar=TRUE, BOOL bMarkRebar=TRUE, CString sMark=_T(""), BOOL bInput=TRUE, long nVertDir=0, BOOL bDrawOnlyMainRebar=FALSE);
	void DimRebarMainFront_Cycle2_Slope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainFront_Cycle2_Basic(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainFront_Cycle1_Basic(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainFront_Cycle1_Slope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainFront_Cycle1_BasicSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainAssm_Front(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, long nCycle, long nDrawLeft=0);
	void DrawRebarShear_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, long nDimDan, BOOL bInput, long nVertDir=0);
	void DrawRebarSupport_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, BOOL bInput=FALSE, long nVertDir=0);
	void DrawLeftSide(CDomyun *pDomP, CFootingApp *pFooting, BOOL bInput, long nDrawLeft=0, BOOL bLeft=TRUE, BOOL bForBM=FALSE, double dOffsetPath=0);
	void DimPlaneAngle(CDomyun *pDomP, long nJ, BOOL bInput=TRUE);
	void DimPlaneInput(CDomyun *pDomP, long nJ, BOOL bInput=TRUE);
	void DimRebarMainFront_Cycle2(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DimRebarMainFront_Cycle1(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper);
	void DrawRebarMain_Front(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft=0, BOOL bInput=TRUE);
	CDRect DrawPlan(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDrawCenterLine=TRUE, BOOL bObtuse=FALSE, BOOL bFront=FALSE, BOOL bHidden=FALSE);
	void DimTextFrontEL(CDomyun *pDomP, CFootingApp *pFooting, BOOL bLeft, BOOL bInput);
	void DimFrontSide(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bDimExFooting, BOOL bInput, BOOL bSide=FALSE);
	void DimFrontUpper(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan=0, BOOL bInput=FALSE, BOOL bLeftIsFront=FALSE);
	void DrawFront(CDomyun *pDomP, CFootingApp *pFooting, CTwinVectorArray *tvArrReturn=&staticTvArrReturnFooting, BOOL bDrawConstJoint=FALSE);
	CDrawFooting();
	virtual ~CDrawFooting();

	CARcBridgeDBStandard* m_pStd;
};

#endif // !defined(AFX_DRAWFOOTING_H__725E49A5_848B_4CE1_B47A_EFD76EC28294__INCLUDED_)
