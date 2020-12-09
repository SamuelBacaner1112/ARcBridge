// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingShoeArrange.h: interface for the CDrawingShoeArrange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSHOEARRANGE_H__4CEE76A6_59C3_4F78_BC70_226513DCE6FC__INCLUDED_)
#define AFX_DRAWINGSHOEARRANGE_H__4CEE76A6_59C3_4F78_BC70_226513DCE6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingShoeArrange : public CDrawPage  
{
public:
	CDrawingShoeArrange();
	virtual ~CDrawingShoeArrange();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

private:
	long m_nBridge;
	CStringArray m_sNoteArr;

	long GetNumberBridge(long nCategoryPageNum);
	void AddPageOne(CDomyun* pDom, long nSubPage);
};

#endif // !defined(AFX_DRAWINGSHOEARRANGE_H__4CEE76A6_59C3_4F78_BC70_226513DCE6FC__INCLUDED_)
