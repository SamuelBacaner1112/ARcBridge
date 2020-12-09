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

	int m_nTypePave;			// ���� ����
	int m_nTypeInnerFill;		// ��ä�� ����
	double m_dUnitWeightPave;	// ���� �����߷�
	double m_dUnitWeightInnerFill;	// ��ä�� �����߷�

	double m_dThickPaveFootway;			// ���� ���� �β�
	double m_dH1;			// ���� �β�
	double m_dSlopeUpper;			// ���� ���
	
	void Serialize(CArchive& ar);
	CFootway& operator=(const CFootway& Obj);
};

#endif // !defined(AFX_FOOTWAY_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
