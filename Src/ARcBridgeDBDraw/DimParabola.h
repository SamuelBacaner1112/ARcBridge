// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DimParabola.h: interface for the CDimParabola class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMPARABOLA_H__89C63081_4A2C_47AE_9E99_42BA424CC603__INCLUDED_)
#define AFX_DIMPARABOLA_H__89C63081_4A2C_47AE_9E99_42BA424CC603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CParabola;
class CARcBridgeDBStandard;

// 포물선 치수 기입
class AFX_EXT_CLASS CDimParabola : public CDomyun  
{
public:
	CDomyun *m_pMainDom;
	CParabola *m_pPb;
	CParabola m_pPbDan;
	double m_dLastXDis;	// 마지막 치수의 위치(xDis값)
	BOOL m_bBlock;
	BOOL m_bFirst;
	

public:
//	void SetParabola(CDomyun *pDomP, CParabola *pPb);
	CARcBridgeDBStandard* m_pStd;
	CDPoint m_xyLastDotPos;
	BOOL IsUpperDim();
	void BlockDot(CDPoint xy, double dAng=0, BOOL bStart=TRUE);

	void DimLineToParabola(double dist, long nDan, CString sText1, CString sText2=_T(""), BOOL bOver=FALSE);
	void DimMoveToParabola(double xDis, long nDan, CString sText1=_T(""), CString sText2=_T(""));
	CDimParabola(CDomyun *pDom, CParabola *pPb);
	CDimParabola();
	virtual ~CDimParabola();

};

#endif // !defined(AFX_DIMPARABOLA_H__89C63081_4A2C_47AE_9E99_42BA424CC603__INCLUDED_)
