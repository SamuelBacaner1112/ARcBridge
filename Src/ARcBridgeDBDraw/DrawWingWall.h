// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawWingWall.h: interface for the CDrawWingWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWWINGWALL_H__134057D8_EC81_40A3_8C15_F56670F98E37__INCLUDED_)
#define AFX_DRAWWINGWALL_H__134057D8_EC81_40A3_8C15_F56670F98E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;

class AFX_EXT_CLASS CDrawWingWall  
{
public:
	BOOL IsNeedTopRebarShearSection(CWingWall *pWing);
	void DimForCoverInput(CDomyun *pDomP, CString sArea, BOOL bStt, BOOL bLeft);
	CDrawWingWall();
	virtual ~CDrawWingWall();

//	void DimGenFrontForBM(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void DimRebarPlaneEnd(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper=TRUE);
	void DimRebarPlaneLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper=TRUE);
	void DimRebarSectTop(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DimRebarSectOut(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DimRebarSectIn(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DimRebarFrontUpper(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput=TRUE);
	void DimRebarFrontLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput=TRUE);
	void DimRebarFrontSide(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput=TRUE);
	void DimGenAngle(CDomyun* pDomP, BOOL bStt, BOOL bInput=TRUE);
	void DimGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void DimGenFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void DimGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void DimGenSodan(CDomyun* pDomP, BOOL bStt, BOOL bLeft);

	void MarkRebarPlaneLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkRebarPlaneUpper(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkRebarSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkRebarHunchWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void MarkRebarHunchFootingFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void MarkRebarBlockAFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkRebarBlockBCFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkRebarBlockDFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void MarkGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void MarkGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void MarkGenFrontSlope(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);
	void MarkGenFrontSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bGenDom=TRUE, BOOL bInput=TRUE, CString str=_T(""), BOOL bSecVer=TRUE, BOOL bSecHor=TRUE);
	void MarkGenFrontEl(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput=TRUE);

	void DrawRebarPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper=TRUE);
	void DrawRebarSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DrawRebarHunchWallSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper=TRUE);
	void DrawRebarHunchFootingSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DrawRebarHunchWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawRebarHunchFootingFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawRebarBlockAShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawRebarBlockBCShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawRebarBlockDShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawRebarBlockAMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawRebarBlockASupportFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawRebarBlockBMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawRebarBlockCMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawRebarBlockDMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawRebarBlockDSupportFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan);
	void DrawBlockBoundary(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut);
	void DrawGenCutPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, double dSpare=1500.0, BOOL bUpper=FALSE);
	void DrawGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bDrawWallHidden=FALSE);
	void DrawGenCutBriSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut=1, BOOL bRebar=FALSE);
	void DrawGenFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut=1, BOOL bConcSym=FALSE, BOOL bHunch=TRUE, BOOL bForBM=FALSE);
	void DrawGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, double dSpare=1000.0, BOOL bRebar=FALSE);
	void DrawGenSodan(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void DrawDetailReinforceRebar(CDomyun* pDomP, BOOL bStt, BOOL bLeft);

	void GetTvArrShearRebar(CTwinVectorArray &tvArr, BOOL bDownShape, BOOL bOneLine, BOOL bBack, long nShearType, long nShearTypeC=-1);

	CARcBridgeDBStandard* m_pStd;
};

#endif // !defined(AFX_DRAWWINGWALL_H__134057D8_EC81_40A3_8C15_F56670F98E37__INCLUDED_)
