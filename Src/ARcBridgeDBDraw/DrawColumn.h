// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawColumn.h: interface for the CDrawColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCOLUMN_H__27C4820F_8C14_4468_A7E4_87550F91B51E__INCLUDED_)
#define AFX_DRAWCOLUMN_H__27C4820F_8C14_4468_A7E4_87550F91B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;

static CTwinVectorArray staticTvArrReturnColumn;
class AFX_EXT_CLASS CDrawColumn  
{
private:
	void DrawRebarMainMark_Plan(CDomyun *pDomP, CWallApp *pWall, long nDan, BOOL bLeft, BOOL bInput, BOOL b2Cycle=FALSE);

public:
	void ApplyCuttingArea_Front(CDomyun *pDomP, CColumnApp *pColumn, CTwinVectorArray &tvArr, long nDan, BOOL bDrawCuttingArea, BOOL bSide);
	void DimRebarMain_Plane_Track(CDomyun *pDomP, CColumnApp *pColumn, long nDimDan, BOOL bExist2Cycle=FALSE);
	void DimRebarMain_Plane_Track_InputMode(CDomyun *pDom, CColumnApp *pColumn, long nDan, long nDimDan);
	CDRect DrawRebar_PlaneStd(CDomyun *pDomP, CWallApp *pWall, BOOL bDimMainRebar, BOOL bDimCrossRebar, BOOL bMarkRebar, BOOL bInput, BOOL bDrawCrossTie=TRUE, BOOL bRotateVertDim=FALSE);
	void DrawRebarWallShear_Plane(CDomyun *pDomP, CWallApp *pWall, BOOL bMarkRebar, BOOL bRotateVertDim=FALSE);
	void DrawRebarCrossTie_Plane(CDomyun *pDomP, CColumnApp *pColumn, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDimDan=1, BOOL bRotateVertDim=FALSE);
	void DimRebarCrossTie_Front(CDomyun *pDomP, CColumnApp *pColumn, long nJijim, long nDimDan, BOOL bSide, BOOL bInput, BOOL bIsTotalDim);
	void DrawRebarRound_Front(CDomyun *pDomP, CColumnApp *pColumn, long nJijum, BOOL bSide, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL bIsTotalDim, BOOL bApplyCuttingCircle=TRUE);
	void DrawRebarMain_Front(CDomyun *pDomP, CColumnApp *pColumn, BOOL bSide, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL bOnlyOutSideRebar=FALSE, BOOL bApplyCuttingCircle=TRUE);
	// 그리기
	void DrawRebarRound_Plan(CDomyun *pDomP, CColumnApp *pColumn, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput);
	void DrawRebarMain_Plan(CDomyun *pDomP, CWallApp *pWall, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL b2Cycle=FALSE);
	void DrawFront(CDomyun *pDomP, CColumnApp *pColumn, BOOL bSide, BOOL bOnlyPattern, BOOL bOnlyOutLine);
	void DrawPlan(CDomyun *pDomP, CColumnApp *pColumn, CTwinVectorArray *tvArrReturn=&staticTvArrReturnColumn, BOOL bDrawCenter=TRUE, BOOL bApplyAnglePlane=TRUE, BOOL bHidden=FALSE, long nFront=0, CTwinVectorArray &tvArrCen=staticTvArr);

	// 치수
	void DimPlan(CDomyun *pDomP, CColumnApp *pColumn, long nDimDan, BOOL bInput, BOOL bApplyAnglePlane=TRUE);
	CDrawColumn();
	virtual ~CDrawColumn();

	CARcBridgeDBStandard* m_pStd;
};

#endif // !defined(AFX_DRAWCOLUMN_H__27C4820F_8C14_4468_A7E4_87550F91B51E__INCLUDED_)
