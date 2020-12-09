// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// MeshFace.h: interface for the CMeshFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHFACE_H__039BFB19_E25F_40F3_9EE5_5C386C342494__INCLUDED_)
#define AFX_MESHFACE_H__039BFB19_E25F_40F3_9EE5_5C386C342494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMeshFace : public CObject
{
public:
	CMeshFace();
	virtual ~CMeshFace();

public:
	long	m_nIndexFace;		// ������� Index
	CVector m_vPoint[4];		// ������� ��ǥ(����,���,�Ͽ�,����), �ð��������
	long	m_nIndexJoint[4];	// ������� ��ǥ

	long	m_nTypeHDan;		// Ⱦ�ܱ��� ����  : ����������
	long	m_nIndexHDan;		// Ⱦ�ܱ��� Index : ����������
	long	m_nIndexPos;		// Ⱦ�ܱ��� Index : �������κ����� ZeroBase, Face�� ���� ����
	long	m_nIndexPosStd;		// Ⱦ�ܱ��� Index : �������κ����� ZeroBase, Face�� ���� ���� / �����θ� ������ ����
	//
	long	m_nSpanSlab;		// ���������� ��쿡 ������ȣ
	BOOL	m_bSpanCheckStt;	// ���������� �ش����� ���������ġ (�θ��Ʈ����)
	BOOL	m_bSpanCheckEnd;	// ���������� �ش����� ����������ġ (�θ��Ʈ����)
};

#endif // !defined(AFX_MESHFACE_H__039BFB19_E25F_40F3_9EE5_5C386C342494__INCLUDED_)
