// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawRcBridge.h: interface for the CDrawVertSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWRCBRIDGE_H__743EAEAF_3905_4A60_9516_7A446F9A9AD4__INCLUDED_)
#define AFX_DRAWRCBRIDGE_H__743EAEAF_3905_4A60_9516_7A446F9A9AD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
class CLineInfoApp;

static CTwinVectorArray staticTvArrReturnBridge;

class AFX_EXT_CLASS CDrawVertSection
{
public:
	void DrawExFootingBoxType(CDomyun *pDomP, BOOL bCutting);

	CDRect GetRectCuttingForGirderDetail(long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder, BOOL bUpperSlabGirder = TRUE);
	void DimThickVertSection(CDomyun *pDom);
	void DimTextVertSectionUpperSlabFck(CDomyun *pDomP, long nStt);
	void DimTextVertSectionLowerSlab(CDomyun *pDomP);
	void DrawVertSectionFlexibleJoint(CDomyun *pDomP, BOOL bStt);
	void DrawVertSectionArchHunchHidden(CDomyun *pDomP);
	void DimVertSectionArchRibThick(CDomyun *pDomP);
	void DrawVertSectionArchRibHiddenLine(CDomyun *pDomP);
	CDRect DrawRebarAssmArchRibStd(CDomyun *pDomP, long nCycleLeft, long nCycleRight, long nVertDir=0);
	void DimRebarMainAssm_VertSection_ArchRib(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft);
	void DrawRebarStirrup_VertSection_ConnectSection(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DimVertSectionArchRiblAndf(CDomyun *pDomP, long nUpper, BOOL bInput);
	CDRect DrawRebarDetailFooting_Front_ArchJijum(CDomyun *pDomP, BOOL bStt, double dScaleDetail);
	void DimTextSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, CString sSection);
	CDRect DrawSupportRebarArchRibStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMarkMainRebar, long nVertDir);
	void DrawRebarShear_VertSection_ArchRib(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit, long nVertDir);
	void DimRebarMain_VertSection_ArchRib(CDomyun *pDomP, long nCycle, BOOL bUpper, long nDan, long nDimDan, BOOL bInput, long nVertDir);
	void DimTextRibInfo(CDomyun *pDomP);
	void DrawRebarSupport_VertSection_OutWallAcuteAngle(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nVertDir);
	void DrawRebarMain_VertSection_OutWallAcute(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarVertSectionObtuseStd(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bLowerSlab = FALSE);
	void DrawRebarMain_VertSection_SlabObtuse(CDomyun *pDomP, BOOL bUpperSlab, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DimJongDanBridge(CDomyun* pDomP, double staBridgeStt, double staBridgeEnd, double scaleVertical, BOOL bDimSttEnd, BOOL bDimSlope) const;
	void DimVertSectionJongSlope(CDomyun *pDomP);
	void DrawRebarSupport_VertSection_OutWallHunch(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit, long nVertDir);
	void DrawRebarMain_VertSection_OutWall_Hunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE, BOOL bForStandardSection=FALSE, long nVertDir=0);
	void DrawVertSectionCenterLine(CDomyun *pDomP, BOOL bInput, BOOL bApplyAngle, BOOL bDimAngle=TRUE, long nJForInput=0);
//	void DimTextVertSectionSlope(CDomyun *pDomP);
	void DimVertSectionUpperOnlyJijum(CDomyun *pDomP, long nDimDan, long nLeft=0);
	void DimForCoverInput(CDomyun *pDomP, CString sArea);
	void DrawJijumSign(CDomyun *pDomP, long nJ);
	void DimRebarMainAssm_VertSection_UpperSlabSide(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft);
	void DimRebarMainAssm_VertSection_FootingLower(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft);
	void DimVertSectionUpperSlabThick(CDomyun *pDomP, BOOL bDimPave, long nLeft);
	void DimVertSectionLowerSlab(CDomyun *pDomP, long nLeft, BOOL bDimThick, BOOL bMarkEl, BOOL bOnlyJijumEL=FALSE);
	CDRect DrawDiagramBMD_SFD(CDomyun *pDomP, BOOL bBMD, BOOL bSFD, BOOL bAXIAL, BOOL bWhenDiagramIsNoneDrawRect=FALSE, BOOL bInput=TRUE);
	void DrawSimpleShoe(CDomyun *pDomP, CDPoint xy);
	void DrawVertSectionPier(CDomyun *pDomP);
	void DimScaffoldForBM(CDomyun *pDomP);
	void DimPostForBM(CDomyun *pDomP);
	void DrawSignHunchByJijum(CDomyun *pDomP, long nJijum, BOOL bLeft=TRUE, BOOL bUpper=TRUE);
	void DrawRebarShear_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarSupport_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarMain_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDrawLeft, CDRect rectLimit=CDRect(0, 0, 0, 0), long nCycle=0);
	void DrawRebarMain_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput = TRUE, CDRect rectLimit=CDRect(0, 0, 0, 0), long nCycle=0);
	void DrawRebarSupport_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarShear_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nCycle, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawVertSectionAbut(CDomyun *pDomP);
	void DrawVirtualRebarMain_VertSection_UpperSlab(CDomyun *pDomP, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawVirtualRebarMain_VertSection_LowerSlab(CDomyun *pDomP, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarMain_VertSection_Danbu(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nCycle, long nDrawLeft=0);
	CDRect DrawRebar_OutWall_VertStd(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput=TRUE, long nVertDir=0, BOOL bDrawOnlyMainRebar=FALSE);
	void DimVertSectionAll(CDomyun *pDomP, BOOL bForOutBM=FALSE);
	// 도면 출력용 
	CDRect DrawRebar_InWall_VertStd(CDomyun *pDomP, CWallApp *pWall, BOOL bInput=TRUE, long nVertDir=0);
	CDRect DrawStandardSectionStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMarkMainRebar, long nVertDir=0);
	CDRect DrawMainRebarAssmStd(CDomyun *pDomP, long nCycleLeft, long nCycleRight, BOOL bVertDir, BOOL bInput=TRUE);
	
	// 주철근 조립도 치수
	void DimRebarMainAssm_VertSection_OutWall(CDomyun *pDomP, long nDimDan, long nCycle, BOOL bStt, long nDrawLeft=0, long nDan=0);
	void DimRebarMainAssm_VertSection_Upper(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft=0, long nDan=0);
	void DimRebarMainAssm_VertSection_Lower(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft=0, long nDan=0, BOOL bUpperSlab=TRUE);
	void DimRebarMainAssm_VertSection_BoxOutSide(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft=0, long nDan=0);

	void DrawDetail_Column_VertSection(CDomyun *pDomP, long nIdxWall);
	CDRect DrawDetail_Girder(CDomyun *pDomP, long nJ, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, BOOL bUpperSlabGirder = TRUE);
	void DrawRebarAll_SlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDanDim, BOOL bInput, BOOL bVirtual, BOOL bStt=TRUE, BOOL bLeft=TRUE, BOOL bGagak=TRUE, BOOL bJongGirder=FALSE, long nIdxInputStirrup=-1, BOOL bApplyVert=FALSE, BOOL bUpperSlabGirder = TRUE, BOOL bDrawDim = FALSE);
	void DrawRebarSupport_VertSection_UpperSlabHunch(CDomyun *pDomP, long nIdxHunch, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarSupport_VertSection_LowerSlabHunch(CDomyun *pDomP, long nIdxHunch, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0));
	void DrawRebarSupport_VertSection_ArchRib(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit, long nVertDir);
	void InsertJijumInfo(CDPointArray &xyArrDim, CStringArray &sArrText, long nType, long nCycle=0, BOOL bUpper=TRUE, BOOL bVertDir=FALSE);

	// 배근
	void DrawRebarMain_VertSectionAll(CDomyun *pDomP, long nCycle, BOOL bDrawJoint, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft=0, BOOL bInput=TRUE, BOOL bForStandardSection=FALSE);
	void DrawRebarMain_VertSection_OutWall(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint=TRUE, BOOL bDimRebar=TRUE, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE, BOOL bForStandardSection=FALSE, long nVertDir=0);
	void DrawRebarMain_VertSection_UpperSlab(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint=TRUE, BOOL bDimRebar=TRUE, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE, long nVertDir=0);
	void DrawRebarMain_VertSection_LowerSlab(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint=TRUE, BOOL bDimRebar=TRUE, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE, long nVertDir=0);
	void DrawRebarMain_VertSection_ArchRib(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint=TRUE, BOOL bDimRebar=TRUE, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE, long nVertDir=0);
	void DrawRebarMain_VertSection_UpperSlabHunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE);
	void DrawRebarMain_VertSection_LowerSlabHunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar=TRUE, long nDrawLeft=0, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bInput=TRUE);
	void DrawRebarSupport_VertSection_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0), long nVertDir=0, BOOL bCalSupRebar=TRUE);
	void DrawRebarSupport_VertSection_LowerSlab(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0), long nVertDir=0, BOOL bCalSupRebar=TRUE);
	void DrawRebarSupport_VertSection_UpperSlabDanBu(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput);
	void DrawRebarSupport_VertSection_LowerSlabDanBu(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput);
	void DrawRebarSupport_VertSection_OutWall(CDomyun *pDomP, BOOL bStt, BOOL bInner, long nDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0), long nVertDir=0);
	void DrawRebarShear_VertSection_UpperSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput);
	void DrawRebarShear_VertSection_LowerSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput);
	void DrawRebarShear_VertSection_OutWall(CDomyun *pDomP, BOOL bStt, long nDimDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, BOOL bInput, long nVertDir=0);
	void DrawRebarShear_VertSection_UpperSlab(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0), long nVertDir=0);
	void DrawRebarShear_VertSection_LowerSlab(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit=CDRect(0, 0, 0, 0), long nVertDir=0);
	//(부상방지저판)
	void DrawBFootingSection(CDomyun *pDomP, BOOL bStt);
	CDRect DrawBFootingRebar(CDomyun *pDom, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMainRebar, BOOL bSupportRebar, BOOL bShearRebar, BOOL bInput = TRUE);
	// 배근치수
	void DimRebarMain_VertSection_UpperSlab_Upper_Cycle(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir=0, long nDan=0);
	void DimRebarMain_VertSection_UpperSlab_Lower_Cycle1(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir=0, long nDan=0);
	void DimRebarMain_VertSection_OutWall_Cycle(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bStt, long nCycle, long nVertDir=0, long nDan=0);
	void DimRebarMain_VertSection_LowerSlab_Lower_Cycle(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir=0, long nDan=0);
	void DimRebarMain_VertSection_LowerSlab_Upper_Cycle1(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir=0, long nDan=0);

	// 일반
	void DimVerSectionInHunch(CDomyun *pDomP, long nDimDan, long nIdxHunch, BOOL bInput, BOOL bUpper=TRUE);
	void DimVerSectionBracketHunch(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput);
	void DimVerSectionHunch(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bUpper=TRUE);
//	void DimVerSectionFootingUpper(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput);
	void DimVerSectionSideEnd(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM=FALSE);
	void DimVerSectionSideStt(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM=FALSE);
	void DimVertSectionUpper(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bOnlyJijum=FALSE, BOOL bSttEndMark=FALSE);
	void DimVertSectionUpperForBMSlabLengthTyp1(CDomyun *pDom);
	void DimTextVerSectionEL(CDomyun *pDomP, BOOL bOnlyJijum=FALSE);
	void DimVertSectionPRF(CDomyun *pDomp, BOOL bInput);
	void DimVerSectionBoxSideEnd(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM=FALSE);
	void DimVerSectionBoxSideStt(CDomyun *pDomP, long nDimDan, double bInput, BOOL bForOutBM=FALSE);
	void DimVerSectionLower(CDomyun *pDomP, long nDimDan);

	void DrawElEarthLine(CDomyun *pDomP, double dStationStt, double dStationEnd,BOOL bDrawBoring, BOOL bBridgeZone, BOOL bMarkGround = FALSE, double dStaMarkG = 0, HGINT32 ixJijum=-1);
	CDRect DrawVertSectionAll(CDomyun *pDomP, BOOL bDrawExFooting=FALSE, BOOL bDrawPile=FALSE, BOOL bDrawPave=TRUE, BOOL bDrawConstJoint=FALSE, BOOL bDrawCuttingPile=FALSE, BOOL bPatternRoundRectWall=TRUE, BOOL bApplyAngle=TRUE, BOOL bFlexibleJoint=TRUE);
	void DrawVertSection(CDomyun *pDomP, BOOL bDrawPave=TRUE, CTwinVectorArray *tvArrReturn=&staticTvArrReturnBridge, CDRect rectLimit=CDRect(0, 0, 0, 0), BOOL bDrawConstJoint=FALSE);

	void DrawTogongLine(CDomyun *pDomP, CRcBridgeRebar *pBri, long iIndex, BOOL bEL=TRUE, BOOL bJusang=TRUE, BOOL bBackFill=TRUE, BOOL bInput=TRUE, BOOL bTyopagi=TRUE, BOOL bForUseBM=FALSE);
	void DrawPartByJijum(CDomyun *pDomP, long iIndex, BOOL bOnlyFooting=FALSE);
	void DrawElByStation(CDomyun *pDomP, double dStd1, double dStd2);
	void DimTogongLine(CDomyun *pDomP, CRcBridgeRebar *pBri, long iIndex, BOOL bDimTypagi, BOOL bDimBackFill, BOOL bInput, BOOL bForUseBM=FALSE);
	CARcBridgeDBStandard* m_pStd;
	CDrawVertSection();
	virtual ~CDrawVertSection();

private:
	void DrawPileBoxType(CDomyun *pDomP, BOOL bCutting);
	void DrawEarthAnchorBoxType(CDomyun *pDomP, BOOL bCutting);
	void DrawStoneBoxType(CDomyun *pDomP, BOOL bCutting);
	void DrawMainRebarAssm(CDomyun *pDomP, long nCycle, long nDrawLeft, BOOL bDrawVertSection, BOOL bVertDir, BOOL bInput=TRUE);
//	BOOL IsUpperMarkRebar(CRebarInfoCycle *pRC);
};

#endif // !defined(AFX_DRAWRCBRIDGE_H__743EAEAF_3905_4A60_9516_7A446F9A9AD4__INCLUDED_)
