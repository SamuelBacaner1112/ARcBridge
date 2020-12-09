// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WingData.h: interface for the CWingData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_)
#define AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 배면
#define WW_REBAR_SLOPE_A_IN					0	// A구간 상단 경사 철근
#define WW_REBAR_HOR_A_IN					1	// A구간 수평 철근
#define WW_REBAR_HOR_A_IN_VAR_SEC			2	// A구간 수평 변단면 철근
#define WW_REBAR_HOR_A_IN_VAR_SEC_BLOCK		3	// A구간 수평 변단면 구간 철근
#define WW_REBAR_HOR_B_IN_VAR				4	// B구간 수평 변화 철근 
#define WW_REBAR_HOR_B_IN					5	// B구간 수평 철근
#define WW_REBAR_HOR_B_IN_VAR_SEC			6	// B구간 수평 변단면 철근
#define WW_REBAR_HOR_B_IN_VAR_SEC_VAR		7	// B구간 수평 변단면 변화 철근
#define WW_REBAR_HOR_B_IN_INC_FOOT			8	// B구간 수평 기초안쪽             *************
#define WW_REBAR_SLOPE_D_IN					9	// D구간 상단 경사 철근
#define WW_REBAR_HOR_D_IN					10	// D구간 수평 철근
#define WW_REBAR_HOR_D_IN_VAR				11	// D구간 수평 변화 철근
#define WW_REBAR_HOR_D_IN_VAR_SEC			12	// D구간 수평 변단면 철근
#define WW_REBAR_HOR_D_IN_VAR_SEC_BLOCK		13	// D구간 수평 변단면 구간 철근
#define WW_REBAR_HOR_D_IN_VAR_UNDER			14	// D구간 수평 변화철근 변단면구간 아래 철근
#define WW_REBAR_HOR_D_IN_VAR2					15	// D구간 수평 변화 철근2

#define WW_REBAR_VER_A_IN					18	// A구간 수직 철근
#define WW_REBAR_VER_A_IN_VAR_SEC			19	// A구간 수직 변단면 철근
#define WW_REBAR_VER_C_IN					20	// C구간 수직 철근
#define WW_REBAR_VER_C_IN_EXC_FOOT			21	// C구간 수직 철근 기초 바깥쪽
#define WW_REBAR_VER_D_IN					22	// D구간 수직 철근

#define WW_REBAR_VER_D_IN_VAR_SEC			23	// D구간 수직 변단면 철근		- 세로방향 변단면일경우 변단면 구간 아래
#define WW_REBAR_VER_D_IN_VAR_SEC_BLOCK		24	// D구간 수직 변단면 구간 철근	- 가로방향 변단면일경우 변단면 구간내
#define WW_REBAR_TAIL_END_IN				25	// D구간 꼬리 끝단 철근
#define WW_REBAR_VER_D_IN_VAR_HU			26  // D구간 수직 변단면 헌치 철근	- 변단면헌치만 있는부분
#define WW_REBAR_VER_D_IN_VAR_UP			27	// D구간 수직 변단면 상단 		- 세로방향 변단면일경우 변단면 구간 위
#define WW_REBAR_VER_D_IN_FLAT				28	// D구간 수직 하단 수평 구간 철근
#define WW_REBAR_VER_D_IN_VAR_SEC_FLAT		29	// D구간 수직 변단면 철근		- 세로방향 변단면일경우 변단면 구간 아래
// 전면
#define WW_REBAR_SLOPE_A_OUT				31	// A구간 상단 경사 철근
#define WW_REBAR_HOR_A_OUT					32	// A구간 수평 철근
#define WW_REBAR_HOR_A_OUT_VAR_SEC_BLOCK	33	// A구간 수평 변단면 구간 철근
#define WW_REBAR_HOR_B_OUT_VAR				34	// B구간 수평 변화 철근
#define WW_REBAR_HOR_B_OUT					35	// B구간 수평 철근
#define WW_REBAR_HOR_B_OUT_INC_FOOT			36	// B구간 수평 기초안쪽             *************
#define WW_REBAR_SLOPE_D_OUT				37	// D구간 상단 경사 철근
#define WW_REBAR_HOR_D_OUT					38	// D구간 수평 철근
#define WW_REBAR_HOR_D_OUT_VAR				39	// D구간 수평 변화 철근
#define WW_REBAR_HOR_D_OUT_VAR_SEC_BLOCK	40	// D구간 수평 변단면 구간 철근
#define WW_REBAR_HOR_D_OUT_VAR2				41	// D구간 수평 변화 철근2

#define WW_REBAR_VER_A_OUT					46	// A구간 수직 철근
#define WW_REBAR_VER_C_OUT					47	// C구간 수직 철근
#define WW_REBAR_VER_C_OUT_EXC_FOOT			48	// C구간 수직 철근 기초 바깥쪽
#define WW_REBAR_VER_D_OUT					49	// D구간 수직 철근

#define WW_REBAR_VER_D_OUT_VAR_SEC_BLOCK	50	// D구간 수직 변단면 구간 철근
#define WW_REBAR_TAIL_END_OUT				51	// D구간 꼬리 끝단 철근
#define WW_REBAR_VER_D_OUT_FLAT				52	// D구간 수직 하단 수평 구간 철근
// 기타
#define WW_REBAR_REINFORCE_A				54	// A구간 흉벽 보강철근
#define WW_REBAR_REINFORCE_B				55	// B구간 흉벽 보강철근
#define WW_REBAR_HUNCH_WALL_UP				56	// 벽체 상부 헌치 철근
#define WW_REBAR_HUNCH_WALL_LO				57	// 벽체 하부 헌치 철근
#define WW_REBAR_HUNCH_FOOT					58	// 기초부 헌치 철근
#define WW_REBAR_SHEAR_A					59	// A구간 전단 철근
#define WW_REBAR_SHEAR_B					60	// B구간 전단 철근
#define WW_REBAR_SHEAR_C					61	// C구간 전단 철근
#define WW_REBAR_SHEAR_D					62	// D구간 전단 철근
// 추가

#define WW_REBAR_COUNT						64	// 날개벽 철근 총 개수

class CTwinVectorAreaShape;

class AFX_EXT_CLASS CWingWall : public CObject
{
public:
	CWingWall();
	virtual ~CWingWall();
	void Serialize(CArchive& ar);

public:
	BOOL IsValid();
//	BOOL IsBlockA();
	BOOL IsBlockB();
	BOOL IsBlockC();
//	BOOL IsBlockD();
	void Clear();
	CWingWall& operator=(const CWingWall& Obj);
	long GetCountRebarSlop(int nOut);						// 날개벽 철근마크할때 경사철근갯수를 빼고해야 위치를 맞출수 있어서.. 
	double GetEndEl(BOOL bIncludeDancha=FALSE);

	// 일반 입력
	BOOL m_bExist;	// 날개벽 설치 유무
	BOOL m_bRAngEnd;	// 날개벽 끝단 처리
	long m_nDirHunch;	// 헌치 방향(0:변화 없음, 1:세로, 2:가로)
	double m_dEL;	// 날개벽끝단 EL
	double m_dL1;	// 날개벽끝단에서 연직방향까지의 거리
	double m_dL2;	// 연직방향위치에서 흉벽까지의 거리
	double m_dHL;	// L1이 있을 경우 L1 부분의 높이
	double m_dH2;	// 함형인 경우 기초하단에서의 높이
	double m_dH3;	// 날개벽 상단에서의 단차 높이
	double m_dT1;	// 날개벽 두께
	double m_dT2;	// 날개벽 변단면의 두께
	double m_dLT;	// 변단면 두께까지의 길이
	double m_dHuW1;	// 기초와 이루는 헌치 폭
	double m_dHuH1;	// 기초와 이루는 헌치 높이
	double m_dHuW2;	// 벽체와 이루는 헌치 폭
	double m_dHuH2;	// 벽체와 이루는 헌치 높이
	double m_dSW;	// L1이 있을 경우 L2와 이루는 슬로프
	double m_dSH;	// 변단면일 경우의 변단면 슬로프
	CDPoint m_vAng;	// 날개벽 각도(슬래브 끝단 접선에서의 각도)
	long m_nAttachPos;	// 부착 위치(0: 벽체 배면, 1: 벽체 측면)
	BOOL m_bSWHor;	// TRUE시 sw를 수평으로 실제로 SW값은 999999가 들어가고 Display하지 않는다.

	BOOL m_bUseUserOutput3D;
	double m_dMomentUltPlate[5];	// A, B, C, D, A'
	double m_dMomentUsePlate[5];
	double m_dMomentUseJudge_Plate[5];
	double m_dShearUltPlate[5];	// A, B, C, D, A'
	double m_dShearUsePlate[5];	// A, B, C, D, A'

	// 철근 배치 [2][2] // 배면,전면 - 1단,2단
	long m_nCountLayerA[2];
	long m_nCountLayerB[2];
	long m_nCountLayerC[2];
	long m_nCountLayerD[2];
	CDPointArray m_xyArrUpper[2][2];	// CDPoint(개수, 간격)[In or Out]
	CDPointArray m_xyArrLower[2][2];	// CDPoint(개수, 간격)[In or Out]
	CDPointArray m_xyArrSide[2][2];	// CDPoint(개수, 간격)[In or Out]
	double m_dDiaMainA[2][2];			// A구간 주철근 직경[In or Out]
	double m_dDiaMainB[2][2];			// B구간 주철근 직경[In or Out]
	double m_dDiaMainC[2][2];			// C구간 주철근 직경[In or Out]
	double m_dDiaMainD[2][2];			// D구간 주철근 직경[In or Out]
	double m_dDiaSupportA[2][2];		// A구간 배력철근 직경[In or Out]
	double m_dDiaSupportB[2][2];		// B구간 배력철근 직경[In or Out]
	double m_dDiaSupportC[2][2];		// C구간 배력철근 직경[In or Out]
	double m_dDiaSupportD[2][2];		// D구간 배력철근 직경[In or Out]
	double m_dDiaHuUpper;			// 벽체 상부 헌치철근 직경
	double m_dDiaHuLower;			// 벽체 하부 헌치철근 직경
	double m_dDiaHuFoot;			// 기초쪽 헌치철근 직경
	double m_dDiaShearA;			// A구간 전단철근 직경
	double m_dDiaShearB;			// B구간 전단철근 직경
	double m_dDiaShearC;			// C구간 전단철근 직경
	double m_dDiaShearD;			// D구간 전단철근 직경
	double m_dSpaceShearA;			// A구간 전단철근 간격
	double m_dSpaceShearB;			// B구간 전단철근 간격
	double m_dSpaceShearC;			// C구간 전단철근 간격
	double m_dSpaceShearD;			// D구간 전단철근 간격
	double m_dStdVerCTC;			// 표준 수직 철근 배근 간격
	double m_dMainCTCA;				// A구간 주철근 배치 간격
	double m_dMainCTCB;				// B구간 주철근 배치 간격
	int m_nHunchCTC;				// 헌치 철근 간격(0:배면, 1:전면)
	int m_nCountShearBind;			// 전단철근 배근시 주철근 묶음간격 개수
	int m_nRebarCurve;				// C구간 주철근 절곡부 처리(0:시방서, 1:직선, 2:곡선)
	BOOL m_bUpperRebarSlope;		// 날개벽 상면 주철근을 경사로 배치
	int m_nCountSlopeBlock[2];		// 날개벽 상면으로부터의 경사 철근 구간 개수
	BOOL m_bSelMainCTCD;				// D구간 주철근 간격을 기본 간격의 2배로
	long m_nTypeShearBC;			// B, C 구간 전단철근배근 형식 - 0: 수직, 1: 수평, 2: 양방향
	BOOL m_bAddRebarReinforce;		// 흉벽보강철근 추가

	// 소단 입력
	double m_dDistNoriStart;	// 소단 시작거리(날개벽 끝에서)
	double m_dSlopeNoriFirst;	// 경사부 구배(1단)
	double m_dSlopeNoriNext;	// 경사부 구배(2단이상)
	double m_dHeightMaxSodan;	// 한단 최대 높이
	double m_dSlopeSodan;		// 소단 경사
	double m_dWidthSodan;		// 소단 폭
	double m_dWidthSodanWallFront;	// 벽체 전면에 대한 소단 폭
	double m_dElSodanBottom;		// 소단 바닥 EL.
	BOOL m_bRetainWallFront;	// 옹벽 설치 여부

	double m_dHeightSoil;		// 적용 토피고

	// 날개벽 형상 관련 -  tvArr을 만들면서 판단 되는 변수로 [Serializ는 하지 않는다]..
	BOOL m_bAttachBraket;			// 외측벽체의 브라켓이 날개벽와 붙어있는지.
	BOOL m_bMatchBottom;			// 날개벽 노리가 바닥서 수평으로 나가는 형태( BOX타입 부상방지턱 없을때만 )

	void SetDataDefaultSodan(double dEl);
	// 철근집계
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar_User;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// 콘크리트 영역

	// 전단철근 타입 설정
	CRebarShearDetail m_RebarShearDetailA;
	CRebarShearDetail m_RebarShearDetailB;
	CRebarShearDetail m_RebarShearDetailC;
	CRebarShearDetail m_RebarShearDetailD;
};

#endif // !defined(AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_)
