// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingOthers.h: interface for the CDrawingOthers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGOTHERS_H__8CFFD6F9_8B75_4380_964D_8A3FCD86167E__INCLUDED_)
#define AFX_DRAWINGOTHERS_H__8CFFD6F9_8B75_4380_964D_8A3FCD86167E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingOthers : public CDrawPage  
{
public:
	CDrawingOthers();
	virtual ~CDrawingOthers();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

	void SetArrEhpDXFName(long nBri);
	CStringArray m_arrEhpDxfName;				// 복합말뚝 상세도 도면 이름 리스트

private:
	long m_nBridge;
	CStringArray m_sNoteArr;

	double GetDetailScale(CDRect DogakRect, CDRect DetailRect);
	CRcBridgeRebar* GetRcBridgeRebar();
	long GetNumberBridge(long nCategoryPageNum);
	void AddFrontHyunTa(CDomyun* pDomP, int nJijum);
	void AddSectionHyunTa(CDomyun* pDomP, int nJijum, long nGugan);
	void AddPageHyunTa(CDomyun *pDomP, CDRect rcDogak, int nCategoryPage, int nJijum);	// 현장타설 말뚝 상세
	void AddPageOthers(CDomyun *pDomP, CDRect rcDogak);						// 기타 철근
	void AddPageEHP(CDomyun *pDomP, CDRect rcDogak, long nPageNum);			// EHP
	void AddPageMonument(CDomyun *pDomP, CDRect rcDogak);					// 교명주 받침
	void AddPageShoe(CDomyun *pDomP, CDRect rcDogak);						// 교량받침
	void AddShoePosition(CDomyun *pDom);									// 교량받침 배치도
	void AddTableShoeCoordinate(CDomyun *pDom);								// 교량받침 좌표
	void AddTableShoeList(CDomyun *pDom);									// 교량받침 리스트
	void AddTableShoeLegend(CDomyun *pDom);									// 교량받침 범례

	void MakeRebarList(CGuardWallRC *pGW, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	void MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	long DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString str, CDRect rt, long nCnt, BOOL bOthers=TRUE, BOOL bMonu=FALSE);
	CDPoint DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString str, CString sSubTitle, CDRect rt, long* pGugan, long nTableSu, BOOL bOthers=TRUE, CGuardWallRC *pGW=NULL, BOOL bMonu=FALSE);
	void DrawRebarDetailTable_RawRebar(CDomyun *pDom, CGuardWallRC *pGW, CDPoint &xyBase, long &nIdxTable, BOOL bHyunTa=FALSE, long nJijum=0);
	BOOL IsDuplicate(long n, long& nHDan, long nType);
	void GetDupGugan(long n, long* pGugan, long nType);
	void MakeRebarListHyunTa(CTypedPtrArray <CObArray, CRebar*> *pArrRebar, int nJijum);
	CDPoint DrawRebarTableHyunTa(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase);
	void AddHyunTaRebarHoldDetail(CDomyun* pDomP, int nJijum);
	void AddHyunTaSpacerDetail(CDomyun* pDomP, int nJijum);
	void AddHyunTaNote(CDomyun* pDomP);
};

#endif // !defined(AFX_DRAWINGOTHERS_H__8CFFD6F9_8B75_4380_964D_8A3FCD86167E__INCLUDED_)
