// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingUser.h: interface for the CDrawingUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGUSER_H__872692A7_E652_4718_8870_166FD004A8B0__INCLUDED_)
#define AFX_DRAWINGUSER_H__872692A7_E652_4718_8870_166FD004A8B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CDrawingUser : public CDrawPage  
{
public:
	CDrawingUser();
	virtual ~CDrawingUser();

	long GetPageSu();
	CString GetTitle();
	double GetScale();
	void SetNumber(long nSubPage);
	void DrawOnePage();

};

#endif // !defined(AFX_DRAWINGUSER_H__872692A7_E652_4718_8870_166FD004A8B0__INCLUDED_)
