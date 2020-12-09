// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PageInfo.h: interface for the CPageInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEINFO_H__D0D10ADD_9DD0_4537_92DC_9DE58A58FB1F__INCLUDED_)
#define AFX_PAGEINFO_H__D0D10ADD_9DD0_4537_92DC_9DE58A58FB1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 구조도
#define PAGE_REBAR_1			1
#define PAGE_REBAR_2			2
#define PAGE_REBAR_3			3
#define PAGE_REBAR_4			4
#define PAGE_REBAR_5			5
#define PAGE_REBAR_6			6
#define PAGE_REBAR_7			7
#define PAGE_REBAR_8			8
#define PAGE_REBAR_9			9
#define PAGE_REBAR_10			10
#define PAGE_REBAR_11			11
#define PAGE_REBAR_SU			12
// 날개벽 구조도
#define PAGE_REBAR_WING_1		13
#define PAGE_REBAR_WING_2		14
#define PAGE_REBAR_WING_3		15
// 철근재료표(구조도, 날개벽구조도)
#define PAGE_REBAR_BM_MAIN		16

// 접속 슬래브
#define PAGE_APS_1				1
#define PAGE_APS_2				2
#define PAGE_APS_3				3
#define PAGE_APS_4				4

// 완충 슬래브
#define PAGE_CNS_1				1
#define PAGE_CNS_2				2
#define PAGE_CNS_3				3
#define PAGE_CNS_4				4
#define PAGE_CNS_5				5
#define PAGE_CNS_6				6
#define PAGE_CNS_7				7
#define PAGE_CNS_8				8


// 기타 도면
#define PAGE_OTHERS_1			1
#define PAGE_OTHERS_2			2

// 교량 받침 배치도
#define PAGE_SHOE_ARRANGE_1		1
#define PAGE_SHOE_ARRANGE_2		2

// 지질 주상도
#define PAGE_BORING_1			1
#define PAGE_BORING_2			2
#define PAGE_BORING_3			3
#define PAGE_BORING_4			4
#define PAGE_BORING_5			5
#define PAGE_BORING_6			6
#define PAGE_BORING_7			7
#define PAGE_BORING_8			8
#define PAGE_BORING_9			9
#define PAGE_BORING_10			10

class CPageInfo : public CObject  
{
public:
	CPageInfo();
	virtual ~CPageInfo();

	long m_nBri;
	long m_nCategory;
	long m_nPageNumber;
	CString m_strTitle;
	HTREEITEM m_hItem;
};

#endif // !defined(AFX_PAGEINFO_H__D0D10ADD_9DD0_4537_92DC_9DE58A58FB1F__INCLUDED_)
