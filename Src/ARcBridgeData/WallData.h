// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WallData.h: interface for the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_)
#define AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WALL_TYPE_VER		0
#define WALL_TYPE_SLOPE		1
#define WALL_TYPE_V			2

#define MAX_COLUMN			30	// 한 벽체에 들어가는 기둥의 개수 (20060209 10개에서 30개로 늘림)
#define MAX_REBAR_DAN	3
#define GAP_FOOTING_LOWER	0			// 기초하면에 들어오는 철근의 갭 

// 철근마크 리턴을 위한 부위 정의 ///////////////////////////////
#define MARK_LEFT	0	// 좌측 철근 마크
#define MARK_RIGHT	1	// 우측 철근 마크
/////////////////////////////////////////////////////////////////

// 기타철근 ////////////////////
// 배력철근
#define W_REBAR_S_L1		0	// 배력철근 좌측 1단.
#define W_REBAR_S_L2		1	// 배력철근 좌측 2단.
#define W_REBAR_S_L3		2	// 배력철근 좌측 3단.
#define W_REBAR_S_R1		3	// 배력철근 우측 1단.
#define W_REBAR_S_R2		4	// 배력철근 우측 2단.
#define W_REBAR_S_R3		5	// 배력철근 우측 3단.
#define W_REBAR_S_SU		6	// 배력철근 수

// 전단철근
#define W_REBAR_V			0	// 전단철근
#define W_REBAR_V_SU		1	// 전단철근 개수

// 기타철근  
#define W_REBAR_ETC_SU		0	// 기타철근 개수
////////////////////////////////


const double dRateWidthInWall			= 0.1;		// 최상단에서 최하단까지에 대한 내측벽의 두께
const double dRateWidthFootingIn		= 0.6;		// 최상단에서 최하단까지 깊이에 대한 기초의 너비 비율(내측)
const double dHeightFootingIn			= 800;		// 내측벽체의 일반적인 기초 높이
class CRebarShearDetail;
class CFootingApp;
class CColumnApp;
class CFactor;

class AFX_EXT_CLASS CWallData : public CObject  
{
public:
	void SaveCountJointRebar();
	double GetAngleByVert(double dAngle, double dAngleApply);
	
	double GetAngleByAngle(double dAngle, double dAngleApply);
	BOOL GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2);
	double GetCrossSectionCenterOfRoad(BOOL bLeft);
	void SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr);
	CString GetMarkRebarShear(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	CString GetMarkRebarSupport(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	void CheckSerialize();
	CString GetMarkRebarColumn(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	CString GetCircleText(long nNumber);
	long GetCountCycleMainRebar();
	CString GetMarkRebarMain(long nType, BOOL bInput, long nCycle, BOOL bOnlyInit=FALSE, BOOL bForBM=TRUE, CString sDecription=_T(""), long nDan=0);
	double GetCoverUpper(BOOL bUpper, long nDan, BOOL bApplyVertForSlope=FALSE);
	double GetCoverFront(BOOL bFront, long nDan);
	void CopyPlaceRebar_ShearRebar(CWallData *pWall);
	void CopyPlaceRebar_SupportRebar(CWallData *pWall);
	void InitWall();
	CWallData& operator=(const CWallData& Obj);
	void Serialize(CArchive &ar);
	long m_nFlag;

	CRebar* GetRebarMainByDescription(CString sDescription);
	CRebar* GetRebarSupportByDescription(CString sDescription);
	CRebar* GetRebarShearByDescription(CString sDescription);
	CRebar* GetRebarColumnByDescription(CString sDescription);
	
	CWallData();
	virtual ~CWallData();

	double m_dWidthExp[2];	// 배력근 배근을 위한 것으로 가각부를 지원하기 위함(배력근배근시만 사용함)
	// (ARCBRIDGE-2819) 가각이 있을때 주철근 사보강배치를 했을때 철근 배근 범위를주철근 배근 범위를 가지고 있는다.
	double m_dWidthOverRebarExp[2];

	// 벽체마다 기초는 하나씩 다 있다. 물론 없는경우도 있을 수 있음.
	CFootingApp m_footing;
	CFootingApp m_footingByRotateToVert;	// 수직으로 세웠을때의 기초
	CTwinVectorArray m_tvArrFront;	// 벽체의 좌표를 가지고 있는 변수
	CTwinVectorArray m_tvArrPlan;	// 벽체단면 좌표
	CTwinVectorArray m_tvArrLowerLimit_Front;	// 벽체의 하부 한계(보통은 없지만, 아치교의 경우 벽체하부를 잘라낼 필요가 있을 수 있다.)
	CTwinVectorArray m_tvArrLowerLimit_FrontRebar;	// 벽체 철근의 하부 한계(보통은 없지만, 아치교의 경우 벽체하부를 잘라낼 필요가 있을 수 있다.)
	double m_dAngleUpperSlab_Front;		// 상부슬래브 각도
	double m_dAngleUpperWall_Front;		// 벽체 상단 각도
	// 벽체형상 -------------------------------------------------------
	BOOL m_bIs;
	CDPoint m_xyOrg;		// 기준점 위치
	long m_nType;			// 벽체 종류(0 : 수직, 1 : 각도 있는거, 2 : v자형);
	BOOL m_bRoundRect;		// 라운드식 벽체인지?(현재는 수직인 경우에만 지원)
	BOOL m_bSideIsArc;		// 좌우측면이 아크인지(아치교에서 라운드 형태를 표현하기 위해 지원함, 실제 벽체는 이거 지원 안함)

	// 일반형상
	double m_dELBDiff;		// 기둥하단 EL에서 추가로 더해야 되는 el(저장안함)입력받는 데이타가 아니므로
	double m_dDiffTop[2];		// Box형에서는 상단과 하단의 Diff적용값이 달라지기때문에 따로 만들자.
	// {기준위치, 좌측, 우측} Box형에서는 상단과 하단의 Diff적용값이 달라지기때문에 따로 만들자.

	double m_dELB;			// 기둥하단 EL
	double m_dW;			// 벽체 너비
	double m_dH;			// 벽체 높이(길이)
	double m_dAngle;		// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
	BOOL m_bApplyAngleEnd;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
		
	// V자형
	double m_dELRT;			// V자 우측팔 Top
	double m_dELLT;			// V자 좌측팔 Top
	double m_dWL;
	double m_dWR;
	double m_dLR;
	double m_dAngleV;		// V자형의 사이각
	double m_dR1;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
	double m_dR2;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)

	// 기둥식의 시작위치는 벽체의 시작위치에서 떨어진 거리를 가지고 판단함.
	CColumnApp m_Column;				// 벽체 단면이 기둥식일 경우 벽체대신 이 기둥을 사용함.
	long m_nCountColumn;				// 기둥식일 경우 기둥 개수
	double m_dDistColumn[MAX_COLUMN];	// 각 기둥간의 거리
	BOOL m_bRevDirPlacingColumn;		// 기둥 배치 방향이 패스의 반대방향인지...
	double m_dAnglePath;				// 사거리적용시 적용할 사각(90도기준)

	// 평면형상
	BOOL m_bTopIsColumn;			// 평면이 기둥식인지...?
	CTwinVector m_tvPathPlan;		// 평면상에 벽체가 뻗는 길.
	CTwinVector m_tvPathPlanLower;	// 평면상에서 벽체 하면이 뻗는 길.
	double m_dMarginLeft;			// 벽체가 path에서 끝위치에서 이격되는 거리
	double m_dMarginRight;			// 벽체가 path에서 시작위치에서 이격되는 거리
	BOOL m_bRAngLeft;				// path의 끝부에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	BOOL m_bRAngRight;				// Path의 시작부에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	CDPoint m_vAngLeft;				// 좌측단 각도(선형안쓰면 0도)
	CDPoint m_vAngRight;			// 우측단 각도(선형안쓰면 0도)
	double m_dOffsetStt;			// Path시작부 직각적용시 안쪽으로 들어갈 거리
	double m_dOffsetEnd;			// Path끝부 직각적용시 안쪽으로 들어갈 거리

	// 좌측면도 상부 형상
	CTwinVectorArray m_tvArrLeftUpper;	// 좌측면도 상부 슬래브 형상(벽체 너비만)
	CTwinVectorArray m_tvArrLeftUpperSlab;	// 좌측면도 상부슬래브 실제 형상
	double m_dDiffHLeftByCenter;		// 벽체상면 중심과 좌측과의 차이(시리얼저장안함)
	double m_dDiffHRightByCenter;		// 벽체상면 중심과 우측과의 차이(시리얼저장안함)
	double m_dThickUppeSlab;			// 상부슬래브 두께.
	double m_dThickHunch;				// 상부슬래브 헌치 두께.
	double m_dThickHunchL;				// 상부슬래브 헌치 두께.좌측
	double m_dThickHunchR;				// 상부슬래브 헌치 두께.우측

	double m_dThickLowerSlab;			// 하부슬래브 두께.
	double m_dThichLowerHunch;			// 하부슬래브 헌치 두께.(값없음)
	double m_dBottomElCorrect;			// 하부슬래브가 경사고 헌치가 없는경우 벽체가 짧게 그려지기 때문에 하단EL을 줘서 맞춰주자.
	// ------------------------------------------------------ 벽체형상

	// 피복 -------------------------------------------------------
	double m_dCoverLeft[MAX_REBAR_DAN];	// 벽체 왼쪽 피복
	double m_dCoverRight[MAX_REBAR_DAN];	// 벽체 오른쪽 피복
	double m_dCoverFrontBack[MAX_REBAR_DAN];	// 벽체 전면후면
	//------------------------------------------------------- 피복 


	// 배근 ----------------------------------------------------------
	BOOL m_bDetailInput23DanMainRebar;			// 2,3단 주철근을 2Cycle 1단 입력하듯이 세부적으로 조정해서 입력 할 수 잇게 한다.
	BOOL m_bNotRebar;						// 철근을 아무거도 가지지 않음(단지 외형만 가짐)
	long m_nTypeRebarFront;					// 정면도 그리는 타입(0:좌우측기준, 1:중심기준) - 시리얼 아직 안 햇음
	long m_nIdxCTC_MainRebar;				// 주철근 CTC
	double m_dCTC_MainRebar;				// index에서 double로 변경(2005.04.25)
	BOOL m_bSameOffsetPlaceTAndCBlock;		// 인장압축구간을 동일 간격으로 배치		
	BOOL m_bPlacing23DanByStandardOffset;	// 2,3단 배력근 표준 간격으로 배치할지?
	BOOL m_bPlacingInWallRebarOfCycle24;	// 2,4사이클에 대해서 철근을 생성할지...??
	long m_nTypeCurveOfVertRebar;			// 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
	BOOL m_bApplyAngleToMainRebarCTC;		// 주철근 CTC에 사각이 적용 되어야 하는지?(상부슬래브가 선형방향 배치라면 적용이 되어야 함)
	double m_dRangeSupportRebarForSameUpperSlab;	// 상부슬래브 처럼 꺽을 배력근 범위
	CPlaceRebarByBlock m_placeByBlock_SupportRebarAddSameUpperSlab;	// 상부슬래브 처럼 꺽을 배력근이 추가될 범위
	double m_dDistAddSupportRebarForSameUpperSlab[2];	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	BOOL m_bRebarLowerSettle[2];				// 벽체하단 철근 정착장유무


	CTwinVectorArray m_tvArrCuttingArea_Side_For1Dan;	// 1단 철근을 파는 용도의 파기 영역(없으면 안 팜)

	// 이음 개수
	long m_nCountJointMainRebar_Cycle1[2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_Cycle2[2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수

	// 이음 개수(예전꺼 보관용) - 이음 개수 변경시 변경된 부분만 권고안을 해야 하기 때문에(Seralize는 하지 않음) //////
	long m_nCountJointMainRebar_Cycle1_Old[2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	long m_nCountJointMainRebar_Cycle2_Old[2][MAX_REBAR_DAN];	// 외측벽체 시종점,좌우측 이음 개수
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMain[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 좌우측면 rebar info cycle
	
	// 이음간격
	CDoubleArray m_dArrDistRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 좌우측 벽체 철근 이음 간격

	// 철근지름
	CDoubleArray m_dArrDiaRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 좌우측 벽체 철근 지름

	// 철근존재여부
	CDoubleArray m_dArrExistRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// 좌우측 벽체 철근 존재 여부

	// 배치단수 설정/철근지름 설정
	CSetRebarDan m_setRebarDan[2];	// 벽체 좌우측 단수 설정

	double m_dCoverUpper_UpperSlab;				// 상부슬래브 상면의 피복
	double m_dCoverLower_LowerSlab;				// 하부슬래브 하면의 피복
	CTwinVectorArray m_tvArrUpperLimit_Front;	// 정면도 상에서 상부슬래브 상면
	CTwinVector m_tvUpperLimitOfMainRebar;		// 축방향 주철근의 상부 한계(RcBridge에서는 상부슬래브 상면 주철근 1단의 좌표를 의미)
	CTwinVector m_tvLowerLimitOfMainRebar;		// 축방향 주철근의 gk부 한계(RcBridge에서는 하부슬래브 상면 주철근 1단의 좌표를 의미)
	CTwinVector m_tvUpperLimitOfSupportRebar;	// 배력근/전단철근의 상부 한계(RcBridge에서는 상부슬래브 하면 좌표)
	CTwinVector m_tvLowerLimitOfSupportRebar;	// 배력근/전단철근의 상부 한계(RcBridge에서는 상부슬래브 하면 좌표)
	long m_nIdxCountCycleMainRebar;				// 주철근 사이클 수.
	long m_nCountWrapMainRebar;					// 전단철근이 철근을 감싸는 구간수
	long m_nTypeDefaultCycle3;					// 3cycle 설정타입
	
	// 표준단면 철근 배치 관련 변수들 (배력철근, 전단철근)
	CPlaceRebarByBlock m_placeByBlock_SupportRebar[2][MAX_REBAR_DAN]; // 좌우측
	CPlaceRebarByBlock m_placeByBlock_ShearRebar;
	long m_nIdxCTC_SupportAndShearRebar;			// 배력,전단철근 표준배치간격
	double m_dCTC_SupportRebar;				// index에서 double로 변경
	double m_dCTC_ShearRebar;

	CRebarShearDetail m_rebarShearDetail;	// 전단철근

	// 도면출력관련 옵션
	long m_nTypeOutputMainRebar_Side;	// 측면 주철근 출력 옵션(0 : 기본, 1 : 파기)
	//---------------------------------------------------------- 배근 

	// 기타
	double m_Fck;
	double m_Fy;
	double m_FyShear;
	double m_dLengthRebarMax;
	CString m_sNameRight;	// 벽체 좌측에 해당하는 이름
	CString m_sNameLeft;	// 벽체 우측에 해당하는 이름
	CBasicConcInfo *m_pBasicConcInfo;

	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// 콘크리트 영역
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain;			// 주철근
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarSupport;		// 배력철근
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarShear;			// 전단철근
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarColumn;			// 기둥부 철근

	double m_dCenterOfWall;				// 벽체중심 (저장하지 않음)

	// 치수 기입용
	CTwinVectorArray m_tvArrRebarMainSideForDim;	// 가각인 경우 가각에서 주철근의 위치를 보관 해야 된다.(치수기입용)

	// 속도 개선용
	CDPoint m_xyOrgOld;

	// 철근 할증률 설정 옵션
	CDockingLong m_nTypeAddRate;		// 0 : 직경별, 1 : 주철근,기타철근
	CDockingDouble m_dAddRateLower;		// 하위 할증률
	CDockingDouble m_dAddRateUpper;		// 상위 할증률
	CDockingDouble m_dAddRateDiaOrg;	// 직경별일경우 기준 직경
};

#endif // !defined(AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_)
