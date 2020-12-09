// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawLine.h: interface for the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLINE_H__4BEF3E85_4733_497D_96D3_06DBBB6FF69C__INCLUDED_)
#define AFX_DRAWLINE_H__4BEF3E85_4733_497D_96D3_06DBBB6FF69C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDBStandard;
class CLineInfoApp;

class AFX_EXT_CLASS CDrawLine  
{
public:
	CDrawLine();
	virtual ~CDrawLine();

	CARcBridgeDBStandard* m_pStd;

public:
	void DimSpanLengthPlan(CDomyun *pDomP, BOOL bSttMark, BOOL bEndMark);
	void DrawJongDan(CDomyun *pDomP);
	void DrawJongDanTable(CDomyun *pDomP, CDPoint ptBase, double dStaStt, double dStaEnd);
	// 지중라멘교의 토피
//	void DimRoadCrossSection(CDomyun *pDomP, double sta, BOOL bInput);
	//////////////////////////////////////////////////////////////////////////
	// 종평면도
	// 편구배도
	void DrawCant(CDomyun *pDomP);
	// 보링정보 범례
	void DrawBoringLegend(CDomyun *pDomP, double dStaStt, double dStaEnd); 
	void DrawBroing(CDomyun *pDomP, CBoringDataEx *pBoringEx, BOOL bLeft, BOOL bMove = TRUE, BOOL bFixWidth = FALSE);

	double GetStationJongDanStt();
	double GetStationJongDanEnd();
private:
//	double GetElevationJongDanTop();
	double GetElevationJongDanBottom();
//	void GetTvArrayDike(double sta, long nHDanUnder, CTwinVectorArray &tvArr);
//	void GetPointDikeFront(double sta, long nHDanUnder, CDPoint &xy1, CDPoint &xy2);
//	void GetPointDikeBack(double sta, long nHDanUnder, CDPoint &xy1, CDPoint &xy2);
//	void DrawCentGuardWall(CDomyun *pDomP, double sta);
//	void DrawLineMarking(CDomyun *pDomP, double sta);
};

#endif // !defined(AFX_DRAWLINE_H__4BEF3E85_4733_497D_96D3_06DBBB6FF69C__INCLUDED_)
