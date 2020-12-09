// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingRebar.h: interface for the CDrawingRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGREBAR_H__A117C80A_1704_4085_A916_F76583D55E4F__INCLUDED_)
#define AFX_DRAWINGREBAR_H__A117C80A_1704_4085_A916_F76583D55E4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingRebar : public CDrawPage  
{
public:
	void AddPageCoupler(CDomyun *pDom, CDRect rcDogak, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	void AddPageArchRibRebar(CDomyun *pDomP, CDRect rcDogak);
	void DrawDetail_ChairBlock(CDomyun *pDomP, CDPoint xyLeftTop);
	CDrawingRebar();
	virtual ~CDrawingRebar();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	void MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nType, BOOL bStt, BOOL bLeft, long nJ, long nRebarMain);
	void GetInfoMainRebarAssm(long nIdxMainRebarAssmPage, long &nLeftCycle, long &nRightCycle);
	CRcBridgeRebar* GetRcBridgeRebar();

	// ������ �߰�.
	void AddPageStandardSection(CDomyun *pDomP, CDRect rcDogak);											// ǥ�شܸ鵵 ������ �߰�
	void AddPageMainRebarAssm(CDomyun *pDomP, long nIdxMainRebarAssmPage, CDRect rcDogak);	// ��ö�� ������ ������ �߰�
	void AddPageRebarUpperSlab(CDomyun *pDomP, CDRect rcDogak);								// ��ν����� ������.
	void AddPageRebarLowerSlab(CDomyun *pDomP, CDRect rcDogak);								// ��ν����� ������.
	void AddPageWingWallRebar(CDomyun *pDomP, long nIdxWingWallRebarPage, CDRect rcDogak);	// ������ ������
	void AddPageOutWallRebar(CDomyun *pDomP, BOOL bStt, CDRect rcDogak);
	void AddPageInWallRebar(CDomyun *pDomP, long nIdxInWallRebarPage, CDRect rcDogak, BOOL bFrontOfRebarBMMain);
	void AddPageRebarBM(CDomyun *pDomP, CDRect rcDogak, long nType, BOOL bStt, BOOL bLeft, long nIdxInWall, BOOL bRebarBMDetail=TRUE, BOOL bRebarBMTable=TRUE);

	// ��Ÿ �׸���
	void AddRebarWingWallFront(CDomyun *pDomP, BOOL bStt, BOOL bLeft, int nOut, CString str);
	void AddRebarWingWallSectionA(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void AddRebarWingWallSectionB(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void AddRebarWingWallSectionC(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	void AddRebarWingWallReinforcDetail(CDomyun *pDomP, BOOL bStt, BOOL bLeft);
	
	void DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, double dMaxYIn=525, double dMinYIn=60);	// �󼼵� �׷��ֱ�
	CDPoint DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase, CDRect rcDogak);		// ö�����ǥ �׷��ֱ�
	void DrawRebarDetailTable_RawRebar(CDomyun *pDom, CDPoint &xyBase, long nType, BOOL bStt=TRUE, BOOL bLeft=TRUE, long nJ=0);

	long m_nBridge;
	CStringArray m_sNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);
};

#endif // !defined(AFX_DRAWINGREBAR_H__A117C80A_1704_4085_A916_F76583D55E4F__INCLUDED_)
