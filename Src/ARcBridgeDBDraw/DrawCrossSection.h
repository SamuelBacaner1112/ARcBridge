// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawCrossSection.h: interface for the CDrawCrossSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCROSSSECTION_H__9E4EAFD4_01AA_4C89_8816_2168B82740AB__INCLUDED_)
#define AFX_DRAWCROSSSECTION_H__9E4EAFD4_01AA_4C89_8816_2168B82740AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
class CLineInfoApp;

class AFX_EXT_CLASS CDrawCrossSection  
{
public:
	void DimCrossSide(CDomyun *pDom, double dSta, CDPoint vAng, BOOL bLeftDim=TRUE, BOOL bRightDim=TRUE);
	void DimThickCrossSection(CDomyun *pDom, double dSta, CDPoint vAng, BOOL bLeftDir=TRUE);
	void DrawCrossGuardWall(CDomyun *pDomP, long nHDan);
	CDRect DrawRebar_CrossSectionStd(CDomyun *pDomP, double dSta, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror, BOOL bExcludeDoupleMainRebar);
	CDRect DrawRebar_CrossSectionStd_LowerSlab(CDomyun *pDomP, double dSta, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror, BOOL bExcludeDoupleMainRebar);
	void DrawCrossSection_Section_ArchRib(CDomyun *pDomP, BOOL bStt);
	void DrawCrossSection_ArchRibStd(CDomyun *pDomP, BOOL bUpper, long nDrawLeft=0, BOOL bDrawOnlyArchRib=FALSE, BOOL bHidden=FALSE, BOOL bDrawPattern=FALSE);
	double GetDistForCrossSection_ArchRib(long nType, BOOL &bOK);
	void DrawRebarShear_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarSupport_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarMain_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, long nCycle, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawCrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, long nDrawLeft, BOOL bVerticalSection=FALSE, double dHeightBlock=0);
	CDRect DrawRebarCrossSection_ArchRib_Std(CDomyun *pDomP, double dXDisOfLeft, double dXDisOfRight, BOOL bDimRebar, BOOL bMarkRebar);
	CDRect DrawRebarCrossSection_AcuteStd(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawVirtualRebarJongGirder(CDomyun *pDomP, double dSta, BOOL bLeft, BOOL bMirror);
	void DimMarkCenterOfRoadCrossSection(CDomyun *pDomP, long nJ, BOOL bCenterOfRoad, BOOL bApplyAngle, double dHeight);
//	CDRect DrawRebar_CrossSection_GirderJongStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bInput);
	void DrawRebarMain_CrossSection_OutWallHunch(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarSuppor_CrossSection_OutWallHunch(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, long nDrawLeft);
	CDRect DrawRebar_Column_SideStd(CDomyun *pDomP, long nJ, BOOL bMarkRebar, BOOL bDimRebar);
	CDRect DrawRebar_InWall_SideStd(CDomyun *pDomP, long nJ, BOOL bLeftAtLeft, BOOL bLeftAtRight, BOOL bMarkRebar, BOOL bDimRebar, CString sLeft, CString sRight, BOOL bDrawCrossSection, BOOL bDrawMainRebar=TRUE, BOOL bDrawSupportRebar=TRUE, BOOL bDrawShearRebar=TRUE, BOOL bInput=FALSE);
	void DrawRebarMain_CrossSection_OutWallExp(CDomyun *pDomP, BOOL bStt, long nLeftWall, BOOL bInnerAtLeft, BOOL bInnerAtRight, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bLeft, CTwinVectorArray &tvArrRebarMainSide);
	CDRect DrawCrossSection_OutWall_Exp(CDomyun *pDomP, BOOL bStt, BOOL bInnerAtLeft, BOOL bInnerAtRight, BOOL bLeft=TRUE);
	CDRect DrawRebar_OutWall_CrossSectionStd(CDomyun *pDomP, BOOL bStt, BOOL bInnerAtLeft, BOOL bInnerAtRight, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDrawMainRebar=TRUE, BOOL bDrawSupportRebar=TRUE, BOOL bDrawShearRebar=TRUE, BOOL bInput=FALSE);
	void DrawCrossSectionUnderHeapSoilSymbol(CDomyun *pDomP, double sta, CDPoint vAng = CDPoint(0, 1));
	void DimForCoverInput(CDomyun *pDomP, CString sArea, long nJ);
	// 일반도에서 이동한 함수...... START !
	void MarkFrontConcFck(CDomyun* pDomP, int nJijum, int nPosDraw);
	void MarkFrontEl(CDomyun* pDomP, int nJijum, BOOL bLeft, BOOL bRight, BOOL bAngleApply);
	void DimFrontSide(CDomyun* pDomP, int nJijum, BOOL bLeft, BOOL bRight, BOOL bCenterDim, BOOL bCenterMark, BOOL bAllBridge = TRUE);
	void DimFrontBottom(CDomyun* pDomP, int nJijum, long nDimDan=0, BOOL bInput = FALSE);
//	void DimGuardWallSide(CDomyun* pDomP);
	void DimCrossSide(CDomyun* pDomP);
	void DimLongi(CDomyun* pDomP);
	void DimLongiInWallSide(CDomyun* pDomP, CWallApp* pWall);
	////////////////////////////////////////////////////////////////////////// END !
	void DrawRebarSupport_CrossSection_UpperSlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarSupport_CrossSection_LowerSlabGirder(CDomyun *pDomP, long nJ, double dStation, CDPoint vAng, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft=TRUE);
	void DrawRebarShear_CrossSection_SlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarShear_CrossSection_LowerSlabGirder(CDomyun *pDomP, long nJ, double dSation, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DimCrossBottom(CDomyun *pDomP, long nJ, BOOL bInput);
	void DimTogongLine(CDomyun *pDomP, long iIndex, BOOL bInput, BOOL bForUseBM, long nDeductLeft=0);
	void DrawTogongLine(CDomyun *pDomP, long iIndex, BOOL bEL, BOOL bJusang, BOOL bInput=TRUE, long nDeductLeft=0);
	void MarkGenCrossSlope(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bStt=FALSE, BOOL bRotate90=FALSE, BOOL bAlignCenter=FALSE, BOOL bMoveForMirro=FALSE, BOOL bAllBridge=FALSE, BOOL bOnlyFootway=FALSE);
	void MarkGenCrossArrowDirection(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bStt=FALSE, BOOL bExp=FALSE);
//	void DrawRebarShear_CrossSection_OutWall(CDomyun *pDomP, BOOL bStt);
	void DrawRebarSupport_CrossSection_SlabHunch(CDomyun *pDomP, BOOL bUpperSlab, double sta, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp=FALSE, BOOL bUseMatchSta=FALSE);
	CDRect DrawRebar_CrossSectionStd(CDomyun *pDomP, double dStaLeft, double dStaRight, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror=TRUE, BOOL bExcludeDoupleMainRebar=FALSE);
	CDRect DrawRebar_CrossSectionStd_LowerSlab(CDomyun *pDomP, double dStaLeft, double dStaRight, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror=TRUE, BOOL bExcludeDoupleMainRebar=FALSE);
	void DrawRebarShear_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bMatchUseSta=FALSE);
	void DrawRebarSupport_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp=FALSE, BOOL bUseMatchSta=FALSE);
	void DrawRebarMain_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, long nCycle, long nDrawLeft, BOOL bExp=FALSE, BOOL bExcludeDoupleMainRebar=FALSE, BOOL bUseMatchSta=FALSE);
	void DrawRebarShear_CrossSection_LowerSlab(CDomyun *pDomP, double sta, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bMatchUseSta=FALSE);
	void DrawRebarSupport_CrossSection_LowerSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp=FALSE, BOOL bUseMatchSta=FALSE);
	void DrawRebarMain_CrossSection_LowerSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, long nCycle, long nDrawLeft, BOOL bExp=FALSE, BOOL bExcludeDoupleMainRebar=FALSE, BOOL bUseMatchSta=FALSE);
	void DimGuardWall(CDomyun* pDomP, CGuardWallRC* pGW, double dSta, BOOL bInput, BOOL bWriteLeft, BOOL bWriteRight, BOOL bRoadNotSlab=FALSE);
	void DimCrossSection(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bInput = TRUE, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE);
	void DimCrossSectionRailCenter(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bInput = TRUE);
	void DimCrossSectionFullLeftRight(CDomyun* pDomP, CRcBridgeRebar* pBri1, CRcBridgeRebar* pBri2, long nDan = 2);
	void DimCrossSectionSlabEnd(CDomyun* pDomP, BOOL bLeft, double sta, CDPoint vAng = CDPoint(0,1), BOOL bInput = TRUE, BOOL bDimLowerNotch=FALSE, BOOL bDimSlabThick=TRUE);
	void DimCrossSectionStationAndElevation(CDomyun* pDomP, HGDOUBLE dSta, HGBOOL bLeft, HGBOOL bRight, HGBOOL bCenter, HGBOOL bSta, CDPoint vAng = CDPoint(0,1));

	void DrawCrossSlabHunch(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bDrawSlopPattern=TRUE, long nDrawLeft=0, BOOL bMirror=FALSE, BOOL bExp=FALSE, CDPoint vAngWall=CDPoint(0, 1), BOOL bCalHeightHunch=TRUE, double dHeightHunch=0);
	void DrawCrossLowerSlabHunch(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bDrawSlopPattern=TRUE, long nDrawLeft=0, BOOL bMirror=FALSE, BOOL bExp=FALSE, CDPoint vAngWall=CDPoint(0, 1), BOOL bCalHeightHunch=TRUE, double dHeightHunch=0);
	void DrawCrossSection(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bInclSubStruct=FALSE, BOOL bInclFootway=TRUE, BOOL bInclSlabLower=TRUE, BOOL bInclGuardWall=TRUE, BOOL bInclPave=TRUE, BOOL bDrawPile=TRUE, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, BOOL bInner=TRUE, long nDrawLeftOfSubStruct=0, long nJSubStruct=-1, BOOL bHatch=TRUE, BOOL bRubble = FALSE);
	void DrawCrossSectionBoxType(CDomyun* pDomP, double sta, CDPoint vAng = CDPoint(0,1), BOOL bInclSubStruct=FALSE, BOOL bInclFootway=TRUE, BOOL bInclSlabLower=TRUE, BOOL bInclGuardWall=TRUE, BOOL bInclPave=TRUE, BOOL bDrawPile=TRUE, BOOL bRoadNotSlab=FALSE, BOOL bExp=FALSE, BOOL bInner=TRUE, long nDrawLeftOfSubStruct=0, long nJSubStruct=-1, BOOL bHatch=TRUE, BOOL bRubble = FALSE);
	void DrawSlabDosang(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bRoadNotSlab=FALSE);
	void DrawSlabDosangByHDan(CDomyun *pDomP, double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab=FALSE);
	void DrawSlabDosangRailCenter(CDomyun *pDomP, double sta, CDPoint vAng, long nRail);
	void DrawRailSymbol(CDomyun *pDomP, double sta, CDPoint vAng, long nRail);
	void DrawRailSymbolUnit(CDomyun *pDomP, BOOL bLeft);

	void DimCrossDistShoe(CDomyun* pDomP, CString strJiJum, BOOL bInput = TRUE);
	void DimCrossDistSlab(CDomyun* pDomP, BOOL bInput , BOOL bDimAllSection, BOOL bOtherBridge, double dSta);
	void DrawSimpleShoe(CDomyun* pDomP, double dEL, double dDist);
	void DrawSimpleCross(CDomyun* pDomP, double sta, BOOL bMarkCenter, BOOL bMarkLeft, BOOL bMarkRight);

	// 방호벽
	void DrawRebarGuardFence(CDomyun* pDomP, long nHDan, BOOL bInclPave = TRUE, BOOL bInclFootway = TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkGuardFence(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarGuardFence(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	// 중분대
	void DrawRebarCenterGuard(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkCenterGuard(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarCenterGuard(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	void DrawRebarMarkCenterGuard2(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarCenterGuard2(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, CTwinVectorArray& tvArrRebar, BOOL bInput);
	//중분대1/2
	void DrawRebarCenterGuardHalf(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkCenterGuardHalf(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarCenterGuardHalf(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	// 난간기초
	void DrawRebarHandRail(CDomyun* pDomP, long nHDan, BOOL bInclPave = TRUE, BOOL bInclFootway = TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkHandRail(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarHandRail(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	// 방음벽 기초
	void DrawRebarSoundProof(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkSoundProof(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarSoundProof(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	// 흙막이벽
	void DrawRebarRetainingWall(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkRetainingWall(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	void DimRebarRetainingWall(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput);
	// 연석
	void DrawRebarRailGuardFence(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE);
	void DrawRebarMarkRailGuardFence(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck);
	
	// 연석+보도
	void DrawRebarFootwayBlock(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE, BOOL bCover = FALSE);
	void DrawRebarMarkFootwayBlock(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck, BOOL bCover = FALSE);
	void DimRebarFootwayBlock(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput, BOOL bCover = FALSE);
	// 통신구, 통신구+보도
	void DrawRebarTotalHole(CDomyun* pDomP, long nHDan, BOOL bInclPave=TRUE, BOOL bInclFootway=TRUE, BOOL bInput = TRUE, BOOL bCover = FALSE);
	void DrawRebarMarkTotalHole(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck, BOOL bCover = FALSE);
	void DimRebarTotalHole(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput, BOOL bCover = FALSE);
			
	void MakeRebarList(CTypedPtrArray <CObArray, CRebar*>  &pArrRebar, CGuardWallRC *pGW);
	
	void DrawCrossAllAtJijum(CDomyun* pDomP, long nJ, BOOL bApplyAngle=FALSE, BOOL bDim=TRUE, BOOL bInput=TRUE, BOOL bDrawPile=TRUE, BOOL bDrawRubble = FALSE);

	// 교명주 받침
	void DrawMonumentFront(CDomyun* pDomP);
	void DrawMonumentPlane(CDomyun* pDomP);
	void DimMonumentFront(CDomyun* pDomP, BOOL bInput=TRUE);
	void DimMonumentPlane(CDomyun* pDomP, BOOL bInput=TRUE);
	void DrawRebarMonumentFront(CDomyun* pDomP);
	void DrawRebarMonumentPlane(CDomyun* pDomP);
	void DimRebarMonumentFront(CDomyun* pDomP, BOOL bInput=TRUE);
	void DimRebarMonumentPlane(CDomyun* pDomP, BOOL bInput=TRUE);
	void MarkRebarMonuFront(CDomyun* pDomP, BOOL bInput=TRUE);
	void MarkRebarMonuPlane(CDomyun* pDomP, BOOL bInput=TRUE);

	CDrawCrossSection();
	virtual ~CDrawCrossSection();

	CARcBridgeDBStandard* m_pStd;
};

#endif // !defined(AFX_DRAWCROSSSECTION_H__9E4EAFD4_01AA_4C89_8816_2168B82740AB__INCLUDED_)
