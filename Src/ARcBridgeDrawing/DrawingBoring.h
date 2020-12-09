// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingBoring.h: interface for the CDrawingBoring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGBORING_H__D17CE0F7_2E22_4230_8805_A7D0AAB827B5__INCLUDED_)
#define AFX_DRAWINGBORING_H__D17CE0F7_2E22_4230_8805_A7D0AAB827B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CARcBridgeCalcStd;
class CDrawingBoring : public CDrawPage  
{
public:
	CDrawingBoring();
	virtual ~CDrawingBoring();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	long m_nBridge;
	CStringArray m_sNoteArr;

	CARcBridgeCalcStd *m_pCalc;

	CDRect AddVerSect(CDomyun* pDomP);
	CDRect AddForceTableDirect(CDomyun* pDomP);
	CDRect AddForceTableDirect_Usd(CDomyun* pDomP, long nCntItem);
	CDRect AddForceTableDirect_Lsd(CDomyun* pDomP, long nCntItem);
	CDRect AddForceTableDirect_BoxType_Usd(CDomyun* pDomP);
	CDRect AddForceTableDirect_BoxType_Lsd(CDomyun* pDomP);
	CDRect AddForceTablePile(CDomyun* pDomP);
	CDRect AddBoringLegend(CDomyun* pDomP);
	CRcBridgeRebar* GetRcBridgeRebar();
	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);
	CDRect AddNote(CDomyun* pDomP);		// NOTE
};

#endif // !defined(AFX_DRAWINGBORING_H__D17CE0F7_2E22_4230_8805_A7D0AAB827B5__INCLUDED_)
