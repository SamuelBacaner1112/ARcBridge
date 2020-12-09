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
	long	m_nIndexFace;		// 부재면의 Index
	CVector m_vPoint[4];		// 부재면의 좌표(상좌,상우,하우,하좌), 시계방향으로
	long	m_nIndexJoint[4];	// 부재면의 좌표

	long	m_nTypeHDan;		// 횡단구성 종류  : 슬래브제원
	long	m_nIndexHDan;		// 횡단구성 Index : 슬래브제원
	long	m_nIndexPos;		// 횡단구성 Index : 좌측으로부터의 ZeroBase, Face에 대한 순서
	long	m_nIndexPosStd;		// 횡단구성 Index : 좌측으로부터의 ZeroBase, Face에 대한 순서 / 가각부를 제외한 순서
	//
	long	m_nSpanSlab;		// 슬래브판인 경우에 지간번호
	BOOL	m_bSpanCheckStt;	// 슬래브판의 해당지간 검토시작위치 (부모멘트검토)
	BOOL	m_bSpanCheckEnd;	// 슬래브판의 해당지간 검토종료위치 (부모멘트검토)
};

#endif // !defined(AFX_MESHFACE_H__039BFB19_E25F_40F3_9EE5_5C386C342494__INCLUDED_)
