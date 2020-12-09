// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GuardWall.h: interface for the CGuardWallRC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDWALLRC_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
#define AFX_GUARDWALLRC_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GW_REBAR_VERT_1		0	// 방호벽 수직철근
#define GW_REBAR_VERT_2		1
#define GW_REBAR_VERT_3		2
#define GW_REBAR_VERT_4		3
#define GW_REBAR_VERT_5		4
#define GW_REBAR_HORI_1 	5	// 방호벽 수평철근
#define GW_REBAR_SHEAR_1	6	// 방호벽 전단철근
#define GW_REBAR_SHEAR_2	7	
#define GW_REBAR_SHEAR_3	8
#define GW_REBAR_EXT_1		9	
#define GW_REBAR_EXT_2	   10	
#define GW_REBAR_EXT_3	   11	
#define GW_REBAR_EXT_4	   12	
#define GW_REBAR_COUNT	   13

#define GW_REBAR_GUGAN	 	5

class AFX_EXT_CLASS CGuardWallRC : public CObject  
{
public:
	CGuardWallRC();
	virtual ~CGuardWallRC();

public:
	BOOL IsForm();
	BOOL IsTypeHDanNonRebarConcrete();
	CString GetStringDia(double dDia);
	int     m_nTypeGuardWall;	// 방호벽 타입, 도로교, 철도교, 도로교 지중라멘, 철도교 지중라멘 일 경우 각각 다름.
	long	m_nNumberHDan;		// 방호벽이 속해있는 횡단 번호
	BOOL	m_bRailRoad;		// TRUE=철도교 방호벽, FALSE=도로교 방호벽
	BOOL	m_bLeftGuardFence;	// 현재 방호벽 좌측 기준하여 슬래브 중앙에서 좌측에 있는지의 여부
	double	m_dWidhtHDan;		// 횡단에서 입력받은 너비
	CDPoint m_ZzxyGW;			// 사용하지 말것. 삭제할 것. 방호벽 기준점(좌우측:직벽선 하단, 중:가운데 하단)의 좌표
	BOOL	m_bSoundProof;		// 방호벽,중분대,중분대1/2,난간기초 타입중 방음벽기초와 같은 경우인지?

	double  m_dDistFrLeft;		// 입력받은 횡단 너비 좌측으로부터 방호벽까지 이격거리
	double	m_dW1;
	double	m_dW2;
	double	m_dW3;
	double	m_dW4;
	double	m_dW5;
	double	m_dH1;
	double	m_dH2;
	double	m_dH3;			// 라운드 반지름, m_dH1 상면 위치
	double	m_dR1;			// 라운드 반지름, m_dH2 상면 위치
	double	m_dR2;			// 라운드 반지름, m_dH3 상면 위치
	double	m_dR3;			// 라운드 반지름, m_dH3 상면 위치
	double	m_dC;			// 상단 모따기, m_dH3 상면 위치
	double	m_dW6;			// 방호벽에 홀이 있을 경우, 횡단좌측부터 이격거리
	double	m_dW7;			// 방호벽에 홀이 있을 경우, 홀 너비
	double	m_dH4;			// 방호벽에 홀이 있을 경우, 횡단하면부터 높이
	double	m_dH5;			// 방호벽에 홀이 있을 경우, 홀 높이
	//Rebar
	double	m_dHR;			// 수평철근 직경
	double	m_dVR;			// 수직철근 직경
	double	m_dSR;			// 전단철근 직경
	double	m_dEC;			// 전단철근의 주철근 묶음수
	double	m_ddc1;			// 윗쪽 피복
	double	m_ddc2;			// 왼쪽 피복
	double	m_ddc3;			// 오른쪽 피복
	double	m_dF1;			// Slab에 삽입된 철근 길이 1	,	철도교 일때 수직방향 위쪽 배력 철근 구간
	double	m_dF2;			// 철근 길이 2					,	철도교 일때 수직방향 아래쪽 배력 철근 구간
	double	m_dF3;			// 철근 길이 3					,	철도교 일때 수평방향 배력 철근 구간
	double	m_dE;			// V 전단철근 구간
	double	m_dEH;			// H 전단철근 구간
	double	m_dF;			// V 전단철근 구간 길이			,	철도교 일때 수평방향 오른쪽 배력 철근 구간
	double	m_dFH;			// H 전단철근 구간 길이
	double	m_dL1;			// Slab에 삽입된 발
	double	m_dCTC;			// 주철근 간격
	double	m_dSC;			// 전단철근 간격
	double	m_dCrossF2;
	double	m_dCrossF3;
	double	m_dForce;
	long	m_nUGugan;
	long	m_nSGugan;
	long	m_nU[GW_REBAR_GUGAN];
	long	m_nS[GW_REBAR_GUGAN];
	double	m_dU[GW_REBAR_GUGAN];
	double	m_dS[GW_REBAR_GUGAN];

	long	m_nTypePave;				// 포장 재질
	long	m_nTypeInnerFill;			// 속채움 재질
	double	m_dUnitWeightPave;			// 포장 단위중량
	double	m_dUnitWeightInnerFill;		// 속채움 단위중량
	double	m_dThickPaveFootway;		// 보도 포장 두께
	double	m_dSlopeUpper;				// 보도 경사

	double	m_dFck;
	double	m_dFy;
	double  m_dAggregate;				// 골재 최대 치수

	BOOL	m_bGWhalf;					// 중분대 일체형 배근
	long	m_nTypeRebar;				// 배근 타입 (기본 : 0, 방호벽에 대해서만 다른 타입 배근이 존재함 : 1)
	BOOL	m_bDWload;					// DW 하중으로 재하
	
	long GetNumInnerFill(CString strName);
	long GetNumPave(CString strName);
	CString GetStringInnerFill(long nIndex);
	CString GetStringPave(long nIndex);

	CStringArray m_sArrTypePave;	// 보도 포장 재질 (투수콘, 콘크리트)
	CStringArray m_sArrInnerFill;	// 보도 채움재 재질 (콘크리트, 몰탈, 모래)

	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// 콘크리트 영역
	
	void Serialize(CArchive& ar);
	CGuardWallRC& operator=(const CGuardWallRC& Obj);
	BOOL operator==(const CGuardWallRC& rhs);

	double	GetWidth() { return m_dW1+m_dW2+m_dW3+m_dW4+m_dW5; }	//좌측 거리(m_dDistFrLeft)를 포함하지 않은 방호벽만의 너비
	double	GetHeight() { return m_dH1+m_dH2+m_dH3; }
//	void	GetXyGuardFenceBase(CDPoint A[9]);

	BOOL	IsTypeHDanOnlyNone();				// 도로 차도, 철도 없음(도로, 철도)
	BOOL	IsTypeHDanRoadGuardFence();			// 방호벽	
	BOOL	IsTypeHDanRoadCentralWallFull();	// 중분대
	BOOL	IsTypeHDanRoadCentralWallHalf();	// 중분대1/2
	BOOL	IsTypeHDanRoadFootway();			// 보도
	BOOL	IsTypeHDanRoadFootwayBlock();		// 보차도 경계석
	BOOL	IsTypeHDanRoadSoundProof();			// 방음벽 기초
	BOOL	IsTypeHDanRoadRetainingWall();		// 흙막이벽	
	BOOL	IsTypeHDanRoadHandRail();			// 난간기초	
	BOOL	IsTypeHDanRoadDike();				// 다이크

	BOOL	IsTypeHDanRailGuardFence();			// 연석	
	BOOL	IsTypeHDanRailDetectGravel();		// 자갈막이(철근)
	BOOL	IsTypeHDanRailDetectGravel2();		// 자갈막이(무근)
	BOOL	IsTypeHDanRailFootway();			// 보도
	BOOL	IsTypeHDanRailDosangVer();			// 직결식도상
	BOOL	IsTypeHDanRailDosangGravel();		// 자갈도상
	BOOL	IsTypeHDanRailFootwayBlock();		// 연석+보도
	BOOL	IsTypeHDanRailHole();				// 통신구
	BOOL	IsTypeHDanRailHoleFootway();		// 통신구+보도
	BOOL	IsTypeHDanRoadRailDike();			// 도로, 철도 다이크
	
	BOOL	IsTypeHDanRoadTotalGuardWall();						// 도로교 차도 를 제외한 모든 것, 철도교일 경우 모두 TRUE
	BOOL	IsTypeHDanRoadTotalGuardFence();					// 도로교 보도, 차도 를 제외한 모든 것, 철도교일 경우 모두 TRUE
	BOOL	IsTypeHDanRoadTotalGuardFenceAndCentralWall();		// 도로교 방호벽, 중분대, 중분대1/2, 철도교일 경우 모두 TRUE
	BOOL	IsTypeHDanRoadTotalCentralWall();					// 도로교 중분대, 중분대1/2, 철도교일 경우 모두 TRUE

	BOOL	IsTypeHDanRailTotalHole();			// 철도교 통신구, 통신구+보도
	BOOL	IsTypeHDanRailTotalDosang();		// 철도교 직결식도상, 자갈도상

	BOOL	IsLeftGuardFence();
	BOOL	IsRound();			// m_dR3>0 일경우에는 TRUE
	BOOL	IsPaveOnHDan();		// 횡단에서 포장, 방수몰탈 할 것인지 여부

	void	SetDataDefaultGuardWall(double thickPave, BOOL bUnderGroundRahmen = FALSE);
	void	SetDataDefaultGuardWallDoro(double thickPave, BOOL bUnderGroundRahmen);
	void	SetDataDefaultGuardWallRail(double thickPave, BOOL bUnderGroundRahmen);

	void	GetTvGuardWallBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer=TRUE, BOOL bForBM=FALSE, double dSlopeSlab=0.0);
	void	GetTvFootwayBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer=TRUE, BOOL bInclPave=TRUE, double dSlopeSlab=0.0);
	void	DimGuardWallBase(CDomyun* pDomP, BOOL bInput, BOOL bWriteLeft, BOOL bWriteRigh);	//	방호벽의 치수 그림
	void	ArrowDirectionForSlope(CDomyun *pDomP, CDPoint xy, CString str, CDPoint vDir);	// 보도, 방호벽의 상면 경사를 표현(화살표 및 텍스트)
};

#endif // !defined(AFX_GUARDWALL_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
