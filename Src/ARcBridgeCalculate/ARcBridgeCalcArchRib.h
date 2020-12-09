// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcArchRib.h: interface for the CARcBridgeCalcArchRib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCARCHRIB_H__8A71FF04_805C_45C2_BBD8_A3F5B7589AD3__INCLUDED_)
#define AFX_ARCBRIDGECALCARCHRIB_H__8A71FF04_805C_45C2_BBD8_A3F5B7589AD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGirderAndColumn;

class AFX_EXT_CLASS CARcBridgeCalcArchRib  
{
public:
	CARcBridgeCalcArchRib();
	virtual ~CARcBridgeCalcArchRib();

	CARcBridgeCalcStd *m_pStd;

	CTypedPtrArray <CObArray, CGirderAndColumn*> m_pList;
	CDoubleArray m_dArrDelta;
	CDoubleArray m_dArrRamda;
	CDoubleArray m_dArrAngTan;
	CDoubleArray m_dArrHcr;
	
	double m_dWidth;
	double m_dRibJigan;
	double m_dRibRaise;

	void CalculateAll();
	double GetOutValueArchRibBySlab(long nSpan, long nPos, long nTypeDesign, long nTypeForce, BOOL bCrown=FALSE);
	double GetOutValueArchRibBySlabEQ(long nSpan, long nPos, long nTypeForce, BOOL bCrown=FALSE);
	void WriteCalcArchRibDesign(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);

	double GetThickArchRib(long nSpan, long nPos, BOOL bCrown=FALSE);
	void GetInxSpanAndCheckPosByIndex(long nIndex, long &nSpan, long &nPosCheck, BOOL &bCompare, BOOL &bCrown);
private:
	void CalculateArchRib();
	void MakeDesignList();
	void SetConcDataByCol();
	void SetConcDataBySlab();
	void SetConcData_BaseInfo(CConcBasic *pConc, BOOL bSectWall);
	void GetMomentAndAxialOnArchRib(long nSpan, long nPos, long nTypeCheck, double &dAxialReturn, double &dMomentReturn, double &dShearReturn, long &nTypeDesignReturn, long &nComboReturn, BOOL bCrown=FALSE);
	
	double GetAngleRibTan(long nSpan, long nPos);
	double GetLenArchRib();

	double GetValueDeltaByRaiseRatio(double dRiseRatio);
	double GetValueAlphaByRaiseRatio(double dRiseRatio);
	void MakeTableColumnLoad(CXLFormatText *XLText, CGirderAndColumn *pGirCol);
	void MakeTableColumnLoadExc(CXLFormatText *XLText, CGirderAndColumn *pGirCol, long nIndex);
	void MakeTableDeltaByRaiseRatio(CXLFormatText *XLText);
	void MakeTableAlphaByRaiseRatio(CXLFormatText *XLText);
	void MakeTableArchRibJewon(CXLFormatText *XLText);

	void DrawGraphXY(CXLFormatText *XLText);
};

#endif // !defined(AFX_ARCBRIDGECALCARCHRIB_H__8A71FF04_805C_45C2_BBD8_A3F5B7589AD3__INCLUDED_)
