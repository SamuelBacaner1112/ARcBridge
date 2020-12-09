// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawWall.h: interface for the CDrawWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWWALL_H__4A8C11BB_26D2_49F7_ACF0_5F86A78279A9__INCLUDED_)
#define AFX_DRAWWALL_H__4A8C11BB_26D2_49F7_ACF0_5F86A78279A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
static CTwinVectorArray staticTvArrReturnInWall;
class AFX_EXT_CLASS CDrawWall  
{
public:
	void ApplyCuttingArea_Side_ForGagak(CDomyun *pDom, CWallApp *pWall, CTwinVectorArray &tvArr, long nLeft, BOOL bDrawCuttingArea);
	void DimFootingUpperReferForSupAndShearRebar(CDomyun *pDomP, CWallApp *pWall);
	void DimRebarMain_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bInput, long nCycle, long nDan=0);
	CDRect DrawRebar_Column_SideStd(CDomyun *pDomP, CWallApp *pWall, BOOL bMarkRebar, BOOL bDimRebar);
	void DrawRebarShear_Side(CDomyun *pDomP, CWallApp *pWall, long nDrawLeft, BOOL bLeftWall);
	void ApplyCuttingArea_Side(CDomyun *pDomP, CWallApp *pWall, CTwinVectorArray &tvArr, long nLeft, long nDan, long nUpdn, BOOL bDrawCuttingArea, BOOL bLeftRebar);
	CDRect DrawRebar_FrontSectionStd(CDomyun *pDomP, CWallApp *pWall, BOOL bInput=TRUE, BOOL bDimRebar=TRUE, BOOL bCut=FALSE, BOOL bVertDir=TRUE);
	CDRect DrawRebar_SideStd(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftAtLeft, BOOL bLeftAtRight, BOOL bMarkRebar, BOOL bDimRebar, CString sLeft=_T("ÁÂÃø"), CString sRight=_T("¿ìÃø"), BOOL bDrawCrossSection=TRUE, BOOL bLeft=TRUE, BOOL bDrawMainRebar=TRUE, BOOL bDrawSupportRebar=TRUE, BOOL bDrawShearRebar=TRUE, BOOL bInput=FALSE, double dDistFromCenter=0);
	void DrawSectionMark_Side(CDomyun *pDomP, CWallApp *pWall, double dDistFromCenter=0);
	void DrawRebarMain_Side(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftRebar, long nCycle, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, CString sDescription=_T(""), BOOL bLeft=TRUE);
	void DrawRebarSupport_Side(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftRebar, long nCycle, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bLeft, BOOL bInput=FALSE);
	void DimMidFront(CDomyun *pDomP, CWallApp *pWall, BOOL bInput);
	void DimRebarMainAssm_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, long nCycle, BOOL bLeft, long nDrawLeft=0, long nDan=0);
	void DimLeftFront(CDomyun *pDomP, CWallApp *pWall);
	void DimLeftPlane(CDomyun *pDomP, CWallApp *pWall, BOOL bInput);
	void DrawLeftSide(CDomyun *pDomP, CWallApp *pWall, BOOL bIncludeFooting=FALSE, BOOL bIncludeUpperSlab=FALSE, BOOL bLeft=TRUE, long nDrawLeft=0, BOOL bDrawCutting=FALSE, BOOL bHiddenSide=TRUE, BOOL bDrawPattern=TRUE, BOOL bOnlyViewFromLeft=FALSE, BOOL bCorrectionEL=FALSE);
	void DrawRebarShear_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, BOOL bInput, BOOL bVertDir=TRUE);
	void DrawRebarSupport_Front(CDomyun *pDomP, CWallApp *pWall, BOOL bLeft, long nDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, BOOL bVertDir=TRUE);
	void DrawRebarMain_Front(CDomyun *pDomP, CWallApp *pWall, long nCycle, long nDan, BOOL bLeft, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft=0, BOOL bDrawJoint=TRUE, BOOL bInput=TRUE, BOOL bForStandardSection=FALSE);
	void DimRightPlane(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bInput, BOOL bDimForWallPlane=FALSE);
	void DrawPlan(CDomyun *pDomP, CWallApp *pWall, BOOL bCenterLine = TRUE, BOOL bHidden=TRUE, long nUpper=0, long nFront=0, CTwinVectorArray &tvArrCen=staticTvArr);
	void DrawFront(CDomyun *pDomP, CWallApp *pWall, BOOL bDrawPattern,  CTwinVectorArray *tvArrReturn=&staticTvArrReturnInWall, BOOL bDrawConstJoint=FALSE);
	

	CARcBridgeDBStandard* m_pStd;
	CHgBaseDrawStd m_baseDraw;
	CDrawWall();
	virtual ~CDrawWall();

};

#endif // !defined(AFX_DRAWWALL_H__4A8C11BB_26D2_49F7_ACF0_5F86A78279A9__INCLUDED_)
