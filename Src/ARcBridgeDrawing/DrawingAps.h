// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingAps.h: interface for the CDrawingAps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGAPS_H__5CE0F1E5_112A_4DB9_8B80_94358AC3DAF3__INCLUDED_)
#define AFX_DRAWINGAPS_H__5CE0F1E5_112A_4DB9_8B80_94358AC3DAF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDrawingOptionData;

class AFX_EXT_CLASS CDrawingAps : public CDrawPage  
{
public:
	void MakeRebarList(CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	CDPoint DrawRebarTable(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase, long nTableIdx=0);
	void DrawRebarDetail(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CDPoint xyBase);
	void DrawRebarDetailTable_RawRebar(CDomyun *pDom, CApsApp *pAps, long nLeft, CDPoint &xyBase);
	CDrawingAps();
	virtual ~CDrawingAps();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	CRcBridgeRebar* GetRcBridgeRebar();
	void AddPageAps(CDomyun *pDomP, BOOL bStt, BOOL bLeft, CDRect rcDogak);
	long m_nBridge;
	CStringArray m_sNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);
};

#endif // !defined(AFX_DRAWINGAPS_H__5CE0F1E5_112A_4DB9_8B80_94358AC3DAF3__INCLUDED_)
