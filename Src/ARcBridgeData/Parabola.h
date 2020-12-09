// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Parabola.h: interface for the CParabola class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARABOLA_H__F4E2D769_1A5A_4870_9CB8_AADF1C3D79F3__INCLUDED_)
#define AFX_PARABOLA_H__F4E2D769_1A5A_4870_9CB8_AADF1C3D79F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CParabola : public CObject  
{
public:
	long m_nFlag;
	CDPoint m_xyCrown;
	double m_dDistTotal;
	double m_dHeightTotal;
	double m_dDistStt;
	double m_dDistEnd;
	double m_dFactorM;
	double m_dLenArchLine;
	double m_dOffsetFromUpper;	// 현재 포물선의 기준상단에서의 떨어진 거리

	double m_dRatefl;		// 라이즈비

	CParabola& operator=(const CParabola& Obj);
	void Serialize(CArchive& ar);

public:
	void SetCalcRate();
	double GetDistByXPos(double dX);
	double GetDistByDistToLen(double dDist, double dLen);
	double GetLenByDistToDist(double dDistStt, double dDistEnd);
	CDPoint GetXyDirTanByDist(double dDist);
	CDPoint GetXyDirPerByDist(double dDist, BOOL bRev=FALSE);
	CDPoint GetXyPerByDistAndOffset(double dDist, double dOffset);
	
	CDPoint GetXyMatchXyAndAngle(CDPoint xy, CDPoint vAng, BOOL bLeft=TRUE);
	CDPoint GetXyByDist(double dDist);
//	CDPoint GetXyCen();
	void Init();
	void GetOffsetParabola(CParabola &pb, double dOffset);
	CParabola();
	virtual ~CParabola();

};

#endif // !defined(AFX_PARABOLA_H__F4E2D769_1A5A_4870_9CB8_AADF1C3D79F3__INCLUDED_)
