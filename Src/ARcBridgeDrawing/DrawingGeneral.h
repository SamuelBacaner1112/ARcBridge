// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingGeneral.h: interface for the CDrawingGeneral class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGGENERAL_H__903EA240_6F06_46A8_B95B_1B1D551D8530__INCLUDED_)
#define AFX_DRAWINGGENERAL_H__903EA240_6F06_46A8_B95B_1B1D551D8530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingGeneral : public CDrawPage  
{
public:
	void AddDetailAntiCrack(CDomyun *pDom);
	CString GetStrSectionNumberFlexibleJoint(BOOL bStt);
	void AddDetailFlexibleJoint(CDomyun *pDomP, BOOL bStt, CString sSectionNum);
	CDrawingGeneral();
	virtual ~CDrawingGeneral();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	void AddPlane(CDomyun* pDomP);
	void AddCross(CDomyun* pDomP);
	void AddLongi(CDomyun* pDomP);
	void AddDetailA(CDomyun* pDomP, CDPoint &xyCen, double &dRadius, BOOL bOnlyCalcCen=FALSE);
	void AddDetailB(CDomyun* pDomP, CDPoint &xyCen, double &dRadius, BOOL bOnlyCalcCen=FALSE);
	void AddNotch(CDomyun* pDomP);
	void AddOrder(CDomyun* pDomP);
	void AddFront(CDomyun* pDomP, int nJijum);
	void AddFootPlane(CDomyun* pDomP, int nJijum);
	void AddWingWallPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void AddWingWallSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft);
	void AddWingWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, CString str);
	void AddNote(CDomyun* pDomP);
	void AddDetailConcJoint(CDomyun* pDomP);
	void AddLowerSlabPlane(CDomyun* pDomP);

	void AddPageGeneral1(CDomyun *pDomP, CDRect rcDogak);	// 일반도(1)
	void AddPageGeneral2(CDomyun *pDomP, CDRect rcDogak, int nCategoryPage);	// 일반도(2)
	void AddPageGeneral3(CDomyun *pDomP, CDRect rcDogak);	// 일반도(3)
	CRcBridgeRebar* GetRcBridgeRebar();

	long m_nBridge;
	CStringArray m_sNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
};

#endif // !defined(AFX_DRAWINGGENERAL_H__903EA240_6F06_46A8_B95B_1B1D551D8530__INCLUDED_)
