// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawPlane.h: interface for the CDrawPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWPLANE_H__D46DAAEB_62B7_4A1B_BEFD_B806CEC2FC97__INCLUDED_)
#define AFX_DRAWPLANE_H__D46DAAEB_62B7_4A1B_BEFD_B806CEC2FC97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
class CLineInfoApp;

class AFX_EXT_CLASS CDrawPlane  
{
public:
//	void DrawRebar_Plane_UpperSlabStdForTest(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark);
	void DimPlaneWingWall(CDomyun *pDom);
	void StretchCurringArea(CTwinVectorArray &tvArr, double dLenStretch);
	void DimPlaneArchRibWidth(CDomyun *pDomP);
	void DrawPlane_Section_Arch(CDomyun *pDomP, BOOL bStt);
	void DrawRebarShear_Plane_ArchRib(CDomyun *pDomP, long nDrawLeft);
	void ApplyCuttingArea_Plane_ArchRib_ForDan(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDrawCuttingArea);
	void ApplyCuttingArea_Plane_ArchRib_Normal(CDomyun *pDomP, CTwinVectorArray &tvArr, long nDrawLeft, BOOL bDrawCuttingArea);
	void DrawRebarSupport_Plane_ArchRib(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	CDRect DrawRebar_Plane_ArchRibStd(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar);
	void DrawRebarMain_Plane_ArchRib(CDomyun *pDomP, long nCycle, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar);
	void DrawPlanePosSign(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void DrawRebarPlane_Slab_Obtuse(CDomyun *pDomP, BOOL bUpperSlab, BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawPlaneGenObtuse(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bLowerSlab = FALSE);
	void DrawRebarStirrup_Plane_UpperSlabGirderJong(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarSupport_Plane_UpperSlabGirderJong(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawMainRebarOrgLineOfExp(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void MakrPlaneConcFck(CDomyun *pDomp);
	void DrawRebarMain_Plane_Slab_Exp(CDomyun *pDomP, BOOL bUpperSlab, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bInput=FALSE);
	void DrawRebarMain_Plane_SlabGirder(CDomyun *pDomP, BOOL bUpperSlab, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawPlaneGenWallByJijum(CDomyun *pDomP, long nJ, BOOL bHidden);
	void DrawPlaneGenOutWall(CDomyun *pDomP, BOOL bStt, BOOL bHidden);
	void DrawPlaneGenInWallAndHunch(CDomyun *pDomP, BOOL bUpperSlab=TRUE);
	void MarkRebarMain_Plane_UpperSlab_ForVert(CDomyun *pDomP, CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrMainLeft, CTwinVectorArray &tvArrMainRight, CTwinVectorArray &tvArrSup, long nIdxRC, BOOL bUpper, long nDan, long nCycle, BOOL bHunch, BOOL bSttBracket, BOOL bEndBracket, long nDrawLeft);
	void MarkRebarMain_Plane_LowerSlab_ForVert(CDomyun *pDomP, CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrMainLeft, CTwinVectorArray &tvArrMainRight, CTwinVectorArray &tvArrSup, long nIdxRC, BOOL bUpper, long nDan, long nCycle, BOOL bHunch, BOOL bSttPRF, BOOL bEndPRF, long nDrawLeft);
	void DrawPlaneScaffold(CDomyun *pDomP);
	void DrawPlaneGenBracket(CDomyun *pDomP, BOOL bStt, BOOL bHidden=FALSE);
	void DrawRebarSupport_Plane_UpperSlabBracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarMain_Plane_UpperSlabBracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawPlaneGenLowerSlabPRF(CDomyun *pDomP, BOOL bStt, BOOL bCuttingCircle, BOOL bHidden=FALSE);
	void DrawRebarSupport_Plane_LowerSlabPRF(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarMain_Plane_LowerSlabPRF(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft);
	void DrawRebarShear_Plane_UpperSlab(CDomyun *pDomP, long nDrawLeft);
	void DrawRebarShear_Plane_LowerSlab(CDomyun *pDomP, long nDrawLeft);
	void GetXyCountMarkSupport_Plane_Slab(CDPointArray &xyArr, CTwinVectorArray &tvArrSup, BOOL bUpper, double &dDistMark, CDoubleArray &dArrDistMain, BOOL bOnlyMatchLine=TRUE);
	void DrawRebarMain_Plane_UpperSlabHunch(CDomyun *pDomP, long nCycle, long nJ, BOOL bUpper, long nLeftRebar, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, long nDirMark=2, BOOL bApplyCutArea=TRUE);
	void DrawRebarSupport_Plane_UpperSlabGirderAndHunch(CDomyun *pDomP, long nJ, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bApplyCutArea=TRUE);
	void DrawRebarMain_Plane_LowerSlabHunch(CDomyun *pDomP, long nCycle, long nJ, BOOL bUpper, long nLeftRebar, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, long nDirMark=2, BOOL bApplyCutArea=TRUE);
	void DrawRebarSupport_Plane_LowerSlabGirderAndHunch(CDomyun *pDomP, long nJ, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bApplyCutArea=TRUE);
	void ApplyCuttingArea_Plane(CDomyun *pDomP, CDPointArray &xyArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt=FALSE, BOOL bEndBracket=FALSE, BOOL bHunch=FALSE, long nJ=0, BOOL bJongGirder=FALSE, BOOL bGagak=FALSE, CLineInfo *pLine=NULL, BOOL bUpperSlab=TRUE);
	void ApplyCuttingArea_Plane(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt=FALSE, BOOL bEndBracket=FALSE, BOOL bHunch=FALSE, long nJ=0, BOOL bJongGirder=FALSE, BOOL bGagak=FALSE, CLineInfo *pLine=NULL, BOOL bUpperSlab=TRUE);
	void ApplyCuttingArea_Plane(CDomyun *pDomP, CTwinVectorArrayArray *pTvArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt=FALSE, BOOL bEndBracket=FALSE, BOOL bHunch=FALSE, long nJ=0, BOOL bJongGirder=FALSE, BOOL bGagak=FALSE, CLineInfo *pLine=NULL, BOOL bUpperSlab=TRUE);
	CDRect DrawRebar_Plane_UpperSlabStd(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark=2, BOOL bForSlabStandard=FALSE, BOOL bDrawHunchRebar=TRUE);
	CDRect DrawRebar_Plane_LowerSlabStd(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark=2, BOOL bForSlabStandard=FALSE, BOOL bDrawHunchRebar=TRUE);
	void DimRebarInfoCycle_Plane_UpperSlab(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpper, long nDan, long nCycle, BOOL bGirder, long nDrawLeft, BOOL bSttBracket=FALSE, BOOL bEndBracket=FALSE, BOOL bHunch=FALSE, long nDirMark=2, BOOL bApplyCutArea=TRUE);
	void DimRebarInfoCycle_Plane_LowerSlab(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpper, long nDan, long nCycle, BOOL bGirder, long nDrawLeft, BOOL bSttPRF=FALSE, BOOL bEndPRF=FALSE, BOOL bHunch=FALSE, long nDirMark=2, BOOL bApplyCutArea=TRUE);
	void DrawRebarSupport_Plane_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar);
	void DrawRebarSupport_Plane_LowerSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar);
	void DrawRebarMain_Plane_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nCycle, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDirMark=2);
	void DrawRebarMain_Plane_LowerSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nCycle, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDirMark=2);
	void DimPlaneJijumLength(CDomyun *pDomP, BOOL bInput = TRUE, BOOL bGenDom = FALSE, BOOL bVerticalPlan = FALSE);
	void DimPlaneJijumGagak(CDomyun *pDomP, BOOL bInput = TRUE, BOOL bGenDom = FALSE);
	void DimPlaneJijumGagakByEach(CDomyun *pDomP, BOOL bInput, BOOL bGenDom, BOOL bLeft, BOOL bStt);
	void DimPlaneJijumAngle(CDomyun *pDomP, BOOL bInput = TRUE);
	CDRect DrawRebar_Plane_SlabHunch(CDomyun *pDomP, BOOL bUpperSlab, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark=2);

	void MarkPlaneBridgeSttEnd(CDomyun *pDomP);
	void MarkPlaneArrowDirection(CDomyun *pDomP, double dLenSpare=3000.0);
	void MarkPlaneCoordinates(CDomyun *pDomP, long nJ, BOOL bMarkJijum, BOOL bMarkSlabSttEnd);
	void MarkPlaneJijum(CDomyun *pDomP, long nJ);
	void MarkGenCrossSlope(CDomyun *pDomP);

	void DrawPlaneGen(CDomyun* pDomP, double dLenSpare=5000.0, BOOL bInput=TRUE, BOOL bGenDom=TRUE, BOOL bIncludeWall=FALSE, BOOL bMarkJijumXY=TRUE, BOOL bMarkSlabXY=TRUE, BOOL bMarkShoe=TRUE);
	void DrawPlaneCenterLineEachSlab(CDomyun* pDomP, double dLenSpare=5000.0, BOOL bInput=TRUE, BOOL bGenDom=TRUE);
	void DrawPlaneSttEndLine(CDomyun* pDomP);
	void DrawPlaneSlabSttEnd(CDomyun* pDomP);
	void DrawPlaneShoeSttEnd(CDomyun* pDomP);
	void DrawPlaneShoeDirLine(CDomyun* pDomP);
	void DrawPlaneShoeSymbol(CDomyun* pDomP, long nType, double BX, double BY, CDPoint vAng, double ScaleFactor=1.0);
	void DrawPlaneSlabOnlyOutLine(CDomyun* pDomP, BOOL bForBM = FALSE, CString str = _T(""));
	void DrawPlaneSlabCrossBlock(CDomyun* pDomP);
	void DrawPlanePierLine(CDomyun *pDomP, long nJ);
	void DrawPlaneShoeLineOnPier(CDomyun *pDomP, long nJ);
	void DrawPlaneUganLineOnPier(CDomyun *pDomP, long nJ);
	void DrawPlaneSlabHunch(CDomyun* pDomP);

	void DrawPlaneLane(CDomyun* pDomP, HGBOOL bRoadNotSlab);
	void DrawPlaneNori(CDomyun* pDomP);
	void DrawPlaneNoriRuler(CDomyun *pDom, CTwinVectorArray &tvArrLane, CTwinVectorArray &tvArrNori, HGBOOL bCCW);

	void DrawMainRebarUpperSlabByTvArr(CDomyun* pDomP, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, CTwinVectorArray &tvArrAngLeftHalf, CTwinVectorArray &tvArrAngRightHalf, CTwinVectorArray &tvArrJoint, CTwinVectorArray &tvArrHidden);

	CDrawPlane();
	virtual ~CDrawPlane();

	CARcBridgeDBStandard* m_pStd;

	BOOL m_bApplyCutAreaHunch;
	long m_nPosBaseDimSupportRebar;
};

#endif // !defined(AFX_DRAWPLANE_H__D46DAAEB_62B7_4A1B_BEFD_B806CEC2FC97__INCLUDED_)
