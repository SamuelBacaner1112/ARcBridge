// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawExFooting.h: interface for the CDrawExFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWEXFOOTING_H__1AB9693E_E564_47B0_ABEA_2012E1F346C0__INCLUDED_)
#define AFX_DRAWEXFOOTING_H__1AB9693E_E564_47B0_ABEA_2012E1F346C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;

static CTwinVectorArray staticTvArrReturnExFooting;
class AFX_EXT_CLASS CDrawExFooting  
{
public:
	CDrawExFooting();
	virtual ~CDrawExFooting();

	CARcBridgeDBStandard* m_pStd;

public:
	void DrawPlan(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bDrawCenter=FALSE, BOOL bHidden=FALSE, BOOL bDrawPile=TRUE, BOOL bDistCheck=FALSE);
	void DrawPlanEarthAnchor(CDomyun *pDomP);
	void DrawLeftSide(CDomyun *pDomP, CExFootingApp *pExFooting, long nDrawLeft=0, BOOL bDrawPile=TRUE, BOOL bDimTextPile=TRUE, BOOL bApplyAnglePile=TRUE, BOOL bLeft=TRUE);
	void DrawFront(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bBack=FALSE, BOOL bCuttingPile=FALSE, CTwinVectorArray *tvArrReturn=&staticTvArrReturnExFooting, BOOL bDrawPile=TRUE, BOOL bDimPile=TRUE, BOOL bApplyAngle=TRUE);
	void DrawLeftSideLowerBox(CDomyun *pDomP, CRcBridgeApp *pBri, double dStation, CDPoint vAng, long nDrawLeft=0, BOOL bDrawPile=TRUE, BOOL bDimTextPile=TRUE, BOOL bApplyAnglePile=TRUE, BOOL bLeft=TRUE, BOOL bDrawExFooting=TRUE, BOOL bDrawHunch=TRUE, BOOL bDrawRubble = FALSE);
	void DrawPileBoxType(CDomyun *pDomP, long nJijum, long nIdxOrg, BOOL bCutting);
	void DimTextPileBoxType(CDomyun *pDomP, long nJijum, long nIdxOrg);
	void DrawEarthAnchor(CDomyun *pDomP, long nJijum, long nIdxOrg, BOOL bCutting);
	void DimTextEarthAnchor(CDomyun *pDomP, long nJijum, long nIdxOrg);
	//
	void MarkFootPlaneCenterOfRoad(CDomyun* pDomP, int nJijum);
	//
	void DimTablePilePlane(CDomyun *pDomP, CExFootingApp *pExFooting, CDPoint xyPos);
	void DimFootPlane(CDomyun* pDomP, int nJijum, BOOL bDimTablePile);
	void DimFootPlaneBoxType(CDomyun* pDomP);	//하부슬래브 평면도 치수표기
	void DimTextFront_MassSlope(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bSide=FALSE);
	void DimTextFrontEL(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bLeft);
	void DimFrontSide(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bLeft, long nDimDan, BOOL bInput, BOOL bSide=FALSE);
	void DimFrontLower(CDomyun *pDomP, CExFootingApp *pExFooting, long nDimDan=0, BOOL bInput=FALSE, BOOL bSide=FALSE);
	void DimFrontLowerBox(CDomyun *pDomP, CRcBridgeApp *pBri, long nDimDan=0, BOOL bInput=FALSE, BOOL bSide=FALSE);
};

#endif // !defined(AFX_DRAWEXFOOTING_H__1AB9693E_E564_47B0_ABEA_2012E1F346C0__INCLUDED_)
