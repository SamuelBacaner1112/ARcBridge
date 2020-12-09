// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Footway.h: interface for the CFootway class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOOTWAY_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
#define AFX_FOOTWAY_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CFootway : public CObject  
{
public:
	CFootway();
	virtual ~CFootway();
public:
	long	m_nNumberHDan;
	BOOL	m_bRailRoad;
	BOOL	m_bLeftGuardFence;


public:
	CStringArray m_sArrTypePave;
	CStringArray m_sArrInnerFill;

	int m_nTypePave;			// 포장 재질
	int m_nTypeInnerFill;		// 속채움 재질
	double m_dUnitWeightPave;	// 포장 단위중량
	double m_dUnitWeightInnerFill;	// 속채움 단위중량

	double m_dThickPaveFootway;			// 보도 포장 두께
	double m_dH1;			// 보도 두께
	double m_dSlopeUpper;			// 보도 경사
	
	void Serialize(CArchive& ar);
	CFootway& operator=(const CFootway& Obj);
};

#endif // !defined(AFX_FOOTWAY_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
