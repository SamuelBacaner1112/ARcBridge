// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDrawingStd.h: interface for the CARcBridgeDrawingStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDRAWINGSTD_H__8E258764_8D26_45D1_94B0_1451A32BFA34__INCLUDED_)
#define AFX_ARCBRIDGEDRAWINGSTD_H__8E258764_8D26_45D1_94B0_1451A32BFA34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARoadOptionStd;
class CARcBridgeDataStd;
class CARcBridgeDrawingOptionStd;
class CPageInfo;

#define GAP_TITLE	Dom.Always(12)	// 도면과 제목과의 사이 간격
#define GAP_TOP		Dom.Always(75)	// 도각 상단과 객체와의 간격
#define GAP_LEFT	Dom.Always(40)	// 도각 좌측과 객체와의 간격

class AFX_EXT_CLASS CARcBridgeDrawingStd : public CDrawStd  
{
public:
	CARcBridgeDrawingStd();
	virtual ~CARcBridgeDrawingStd();

public:
	void MakePage();
	double GetRebarListByFy(CTypedPtrArray <CObArray, CRebar*> *pArrRebarOrg, CTypedPtrArray <CObArray, CRebar*> *pArrRebarGet, double dHigherThanFy);
	CDRect DrawCouplerDetail(CDomyun *pDom, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	void DrawTableCouplerDetail(CDomyun *pDom, CTypedPtrArray <CObArray, CRebar*> *pArrRebar);
	CString GetStringTitle(CRcBridgeApp *pBri, CString sTitle);
	void DrawNoteBottomOfDomyun(CDomyun *pDomP, CString sNote);
	long GetCountPageOther(long nBri, long nCountEHP=0);
	// 재정의
	virtual long GetCnvPage(long nPage);
	virtual long GetCnvSubPage(long nPage);

	long GetCountPageApsRebar(long nBri);
	long GetCountPageInWallRebar(long nBri, BOOL bFrontOfRebarBMMain);
	long GetCountPageWingWallRebar(long nBri);
	
	CARoadOptionStd* m_pARoadOptionStd;
	CARcBridgeDrawingOptionStd* m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* m_pARcBridgeDataStd;

	CTypedPtrArray <CObArray, CPageInfo*> m_pPageInfo;

	CDrawPageMng* GetPageManage() { return m_pDrawPageMng; }

	void AddTitleOnDom(CDomyun *pDom, long nBri, CString strTitle, CDPoint xy, long nScaleDir, double dScale, BOOL bSmallTitle=FALSE);
	void ChangeSeperateLayer(CDomyun *pDom);
	void MakePageInfo();
	void MakeCategoryVerticalPlane(long nBri, long nCategory);
	void MakeCategoryGeneral(long nBri, long nCategory);
	void MakeCategoryRebar(long nBri, long nCategory);
	void MakeCategoryAps(long nBri, long nCategory);
	void MakeCategoryCns(long nBri, long nCategory);
	void MakeCategoryOthers(long nBri, long nCategory);
	void MakeCategoryShoeArrange(long nBri, long nCategory);
	void MakeCategoryBoring(long nBri, long nCategory);
	void MakeCategorySlabStandardOutput(long nBri, long nCategory);

	CPageInfo* AddPageInfo(long nBri, long nCategory, long nCategoryPageNum, CString strTitle);
	CPageInfo* GetPageInfo(long nSubPage);				// nSubPage : 페이지 번호
	CPageInfo* GetPageInfo(long nBri, long nSubPage);	// nBri : 교량 번호, nPageNum : 페이지 번호
	CPageInfo* GetPageInfo(HTREEITEM hItem);
	CPageInfo* GetPageInfoOrg(CString strCategory, long nCategoryPageNum);	// nCategory : 도면 카테고리, nCategoryPageNum : 카테고리 내에서의 페이지 번호

	long GetCountTotalPage();
	long GetPageNum(CString strCategory, HTREEITEM hItem);
	long GetPageNum(CPageInfo* pInfo);
	long GetPageNumCategory(CPageInfo* pInfo);
	long GetPageFirstCategory(long nSubPage);
	long GetPageLastCategory(long nSubPage);
	long GetBridgeNumber(long nSubPage);
	long GetCategoryNumber(long nSubPage);
	long GetCategorySttPage(long nCategory);
	long GetOrgPage(long nSubPage);
	long GetSubPage(long nOrgPage);
	long GetSubPageSttCategory(long nBri, CString strCategory);
	long GetCountCategoryPage(CString strCategory);
	long GetCountCategory();
	long GetCountBriCategoryPage(long nBri, CString strCategory);
//	long GetCountBridgePage(long nBri);
	long GetCountPageMainRebarAssm(long nBri);

	CString GetTitleCategory(int nCategory);	
};

#endif // !defined(AFX_ARCBRIDGEDRAWINGSTD_H__8E258764_8D26_45D1_94B0_1451A32BFA34__INCLUDED_)
