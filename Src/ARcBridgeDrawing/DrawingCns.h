// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingCns.h: interface for the CDrawingCns class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGCNS_H__113DE4CA_6B8D_44AD_932E_ECFEEF6BA6F0__INCLUDED_)
#define AFX_DRAWINGCNS_H__113DE4CA_6B8D_44AD_932E_ECFEEF6BA6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARcBridgeDrawingOptionData;

class AFX_EXT_CLASS CDrawingCns : public CDrawPage  
{
public:
	void MakeRebarList(CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	CDPoint DrawRebarTable(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase);
	void DrawRebarDetail(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CDPoint xyBase);
	void DrawRebarDetailTable_RawRebar(CDomyun *pDom, CApsApp *pCns, long nIdxTable, CDPoint &xyBase);
	CDrawingCns();
	virtual ~CDrawingCns();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	CRcBridgeRebar* GetRcBridgeRebar();
	void AddPageCns(CDomyun *pDomP, BOOL bStt, BOOL bLeft, long nIdx, CDRect rcDogak);
	long m_nBridge;
	CStringArray m_sNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);	
};

#endif // !defined(AFX_DRAWINGCNS_H__113DE4CA_6B8D_44AD_932E_ECFEEF6BA6F0__INCLUDED_)
