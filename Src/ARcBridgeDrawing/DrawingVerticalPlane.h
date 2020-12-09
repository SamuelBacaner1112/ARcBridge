// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingVerticalPlane.h: interface for the CDrawingVerticalPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGVERTICALPLANE_H__B8A45798_7388_4518_A457_AD878228DDDA__INCLUDED_)
#define AFX_DRAWINGVERTICALPLANE_H__B8A45798_7388_4518_A457_AD878228DDDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingVerticalPlane : public CDrawPage  
{
public:
	CDrawingVerticalPlane();
	virtual ~CDrawingVerticalPlane();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	long m_nBridge;
	CStringArray m_strNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);

private:
	void AddJongDan(CDomyun *pDomP);
	void AddPlane(CDomyun* pDomP);
	void AddCrossSection(CDomyun* pDomP);
	void AddCant(CDomyun* pDomP);		// 편경사도
	void AddVLineDom(CDomyun* pDomP);		// 편경사도
	void AddBoringLegend(CDomyun* pDomP);			// 주상도 범례
	void AddNote(CDomyun* pDomP);		// NOTE
};

#endif // !defined(AFX_DRAWINGVERTICALPLANE_H__B8A45798_7388_4518_A457_AD878228DDDA__INCLUDED_)
