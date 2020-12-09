// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Jijum.h: interface for the CJijum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_)
#define AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007.02.27 슈 최대 개수를 30개 까지 입력 받게 해 놓고 여기선 20개 까지만 되어 있어서
// 최대 개수 30개로 조정함. modified by jaeho
// 시리얼 체크 잘 해 줘야 함.
#define MAX_SHOE_COUNT_RC_OLD 20
#define MAX_SHOE_COUNT_RC_20090716 30
#define MAX_SHOE_COUNT_RC 40

class CShoePos;

class AFX_EXT_CLASS CJijum : public CObject  
{
public:
	void SerializeNew(CArchive &ar);
	CJijum();
	virtual ~CJijum();

	void SetAngJijum(CDPoint vAng);
	BOOL IsFixJijum();

	void Serialize(CArchive& ar);
	CJijum& operator=(const CJijum& Obj);

	long m_nFlag;						// added by jaeho 2007.02.27
	double m_dLengthUgan[2];			// 유간 거리
	double m_dLengthSlabToShoe[2];		// 종방향 교량 받침 거리
	CDPoint m_vAngle;					// 지점 각도
	double m_dStaJijum;					// 지점 스테이션

	// 교량 받침
	int m_nQtyShoe[2];						// 교량 받침 개수
	int m_nDirShoe[2][MAX_SHOE_COUNT_RC];		// 교량 받침 방향
	CString m_strJijumShoe[2];				// 교량 받침 열의 이름
	double m_dDistShoe[2][MAX_SHOE_COUNT_RC];	// 중심 선형으로 부터의 교량 받침 법선 거리
	double m_dKh[2][MAX_SHOE_COUNT_RC];		// 탄성받침일 경우의 전단 스프링계수
	double m_dKv[2][MAX_SHOE_COUNT_RC];		// 탄성받침일 경우의 압축 스프링계수

	// 블럭아웃
	double m_dWidBlockOut[2];	// nV=0 전열, nV=1 후열
	double m_dHeightBlockOut[2];	// nV=0 전열, nV=1 후열
};

#endif // !defined(AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_)
