// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RcBridgeData.h: interface for the CRcBridgeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_)
#define AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PILE_SU			100

#define MAX_INWALL_HUNCH	20
#define MAX_JIJUM_COUNT_RC		21	// 변경시 주의!!!  최대지점개수 변경시 반드시 수량에 정의된 숫자도 변경해야 된다!!!!
#define MAX_QTY_HDAN	30
#define MAX_REBAR_DAN	3
#define MAX_STRUP_REBAR		MAX_JIJUM_COUNT_RC-2
#define MAX_CNS		2
#define RCBRIDGE_ANCHOR_MAX 30

const double dRateWidthFootingOut		= 0.65;		// 최상단에서 최하단까지 깊이에 대한 기초의 너비 비율(외측)
const double dRateWidthOutWall			= 0.1;		// 최상단에서 최하단까지에 대한 외측벽의 두께
const double dHeightFootingOut			= 1000;		// 외측벽체의 일반적인 기초 높이
const double dHeightHunch				= 300;		// 헌치의 높이
const double dWidthHunch				= 900;		// 헌치의 너비(슬래브교일때는 높이 * 5), 도설 507-5 슬래브교 헌치는 1/3~1/6배를 보통으로 함
const double dWidthFootingOutWallInner	= 1200;		// 외측벽 기초의 안쪽 너비
const double dDepthFromEL				= 1500;		// 지반에서 기초상단까지의 깊이
const double dDepthToBracket			= 800;		// 시점에서 브라켓 상단까지의 깊이
const double dSizeBracket				= 300;		// 브라켓의 너비/높이(헌치포함)
const double dMaxHeight					= 15000;	// 권고안시 교량의 최대 높이
const double dMinHeight					= 6000;		// 권고안시 교량의 최소 높이
const double dSta_Check_NotUse_Minus	= -1000000;

#define BRIDGE_TYPE_PORTAL			0	// 문형 라멘교
#define BRIDGE_TYPE_PI				1	// ㅠ형 라멘교
#define BRIDGE_TYPE_V				2	// V형 라멘교
#define BRIDGE_TYPE_TRAP			3	// 경사교각라멘교
#define BRIDGE_TYPE_SLAB			4	// 슬래브교
#define BRIDGE_TYPE_ARCH			5	// 아치교
#define BRIDGE_TYPE_BOX				6	// 박스교
#define BRIDGE_TYPE_TRAP2			7	// 경사교각라멘교2
#define BRIDGE_TYPE_SLAB_STANDARD	8 // 슬래브 표준도 지원

#define LOWERSLAB_TYPE_LEAN			0	// 일반 버림 콘크리트(직접기초)
#define LOWERSLAB_TYPE_MASS			1	// MASS
#define LOWERSLAB_TYPE_PILE			2	// 파일
#define LOWERSLAB_TYPE_COBBSTONE	3	// 잡석기초
#define LOWERSLAB_TYPE_ANCHOR		4	// Anchor

#define COMBO_SERVICE			0
#define COMBO_ULTIMATE			1
#define COMBO_ECCENTRIC			2
#define COMBO_EARTHQUAKE		3
#define COMBO_EARTHQUAKE2		4
#define RCBRIDGE_COMBO_COUNT	5
#define MAX_RCBRIDGE_COMBO    200

#define FOOTING_CHECK_AXIAL		0
#define FOOTING_CHECK_MOMENT_MINUS	1
#define FOOTING_CHECK_MOMENT_PLUS	2
#define FOOTING_CHECK_EQ		3
#define FOOTING_CHECK_LSD		4			// 한계상태 추가 데이터 : 명칭을 모르겠음...

#define OUTVALUE_AXIAL		0	// 축력
#define OUTVALUE_MOMENT		1	// M2
#define OUTVALUE_SHEAR		2	// V3
#define OUTVALUE_DISP		3	// 변위(X)
#define OUTVALUE_COUNT		4

#define MIN_JOINT_DIST	5.0

// 2D FRAME : JOINT
#define LONG_0_JOINT_TYPE			0
#define LONG_1_JOINT_JIJUMNO		1
#define LONG_2_JOINT_SECTION_NO		2
#define LONG_3_JOINT_HUNCHPOS		3	// -1 : 일반절점, 0 : 헌치시작점, 1 : 헌치끝점
#define LONG_4_INDEX_SPRING		4	// Spring, NLLink의 위치별 Index

#define BOOL_0_JOINT_SPRING		0 // 지점스프링, 하부슬래브(BOX형)
#define BOOL_1_JOINT_JIJUM_CONNECT	1
#define BOOL_2_JOINT_JIJUM_FIX		2
#define BOOL_3_JOINT_COLUMN_END		3
#define BOOL_4_JOINT_SHOE_POS		4

// 2D FRAME : ELEMENT
#define LONG_0_ELEMENT_TYPE		0
#define LONG_1_ELEMENT_JIJUMNO		1
#define LONG_2_ELEMENT_COLUMN_INDEX	2 

#define BOOL_0_ELEMENT_CHECK_STT	0
#define BOOL_1_ELEMENT_CHECK_END	1

// 3D FRAME
#define LONGSHELL_0_HDAN_NODE	 0
#define LONGSHELL_1_JIJUMNO	 1
#define LONGSHELL_2_SHOE_POS	 2

// 3D 날개벽
#define LONGSHELL_0_AREA_POS	 0

#define WINGSHELL_AREA_A	 0
#define WINGSHELL_AREA_B	 1
#define WINGSHELL_AREA_C	 2
#define WINGSHELL_AREA_D	 3
#define WINGSHELL_AREA_A_	 4

#define BOOLSHELL_0_CHECKPOS_STT 0	// 슬래브의 좌측,벽체의 상단
#define BOOLSHELL_1_CHECKPOS_END 1	// 슬래브의 우측,벽체의 하단
#define BOOLSHELL_2_CHECKPOS_MID 2	// 슬래브의 중앙,벽체의 중앙 (벽체인 경우에 사용... 슬래브는 없어도 가능)

#define SHELL_ELEMENT_UPPERSLAB		0
#define SHELL_ELEMENT_LOWERSLAB		1
#define SHELL_ELEMENT_WALL			2
#define SHELL_ELEMENT_ARCHRIB		3

// 수량관련된 정의
#define TYPE_POST_BRIDGE	0	// 동바리 타입 (육교용)
#define TYPE_POST_NORMAL	1	// 동바리 타입 (일반용)

// 함형라멘교 : 직접기초,MASS기초,말뚝기초,잡석기초,부력방지 Anchor
#define BOXFOOTING_TYPE_LEAN			0
#define BOXFOOTING_TYPE_MASS			1
#define BOXFOOTING_TYPE_PILE			2
#define BOXFOOTING_TYPE_STONE			3
#define BOXFOOTING_TYPE_ANCHOR			4

// STM 검토 기초 데이터 개수
#define STM_MODEL_FOOT_COUNT			4
#define STM_MODEL_FOOT_LSD_COUNT		5

enum eStmModelPosLsd { eLsdStmCornerUpper, eLsdStmCornerLower, eLsdStmBracketStt, eLsdStmBracketEnd, eLsdStmFootStt, eLsdStmFootEnd, eLsdStmFootMid, eLsdStmCount };
enum eStmModelPos{ eStmBracket, eStmCorner, eStmFoot, eStmMidFoot };
enum eStmModelExportPos{ eStmBracketExport, eStmCornerExport, eStmFootSttExport, eStmFootEndExport, eStmMidFootExport };
// 일반해석 모델과 구분 해야 되므로 Key는 10부터 시작하자.
#define STM_MODEL_BRACKET_STT			10
#define STM_MODEL_BRACKET_END			11
#define STM_MODEL_CORNER_STT_UPPER_MINUS	12
#define STM_MODEL_CORNER_END_UPPER_MINUS	13
#define STM_MODEL_CORNER_STT_LOWER_MINUS	14
#define STM_MODEL_CORNER_END_LOWER_MINUS	15
#define STM_MODEL_CORNER_STT_UPPER_PLUS	16
#define STM_MODEL_CORNER_END_UPPER_PLUS	17
#define STM_MODEL_CORNER_STT_LOWER_PLUS	18
#define STM_MODEL_CORNER_END_LOWER_PLUS	19
#define STM_MODEL_FOOT_STT_MAXP			20
#define STM_MODEL_FOOT_STT_MAXM			21
#define STM_MODEL_FOOT_STT_MINM			22
#define STM_MODEL_FOOT_STT_EARTH		23
#define STM_MODEL_FOOT_END_MAXP			24
#define STM_MODEL_FOOT_END_MAXM			25
#define STM_MODEL_FOOT_END_MINM			26
#define STM_MODEL_FOOT_END_EARTH		27
#define STM_MODEL_MIDFOOT_HEAD			28
// LSD - 기초 데이터 : 축력 최대, 부모멘트 최대, 정모멘트 최대, 수평력 최대, 수평력 최소
#define STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1	50		// 우각부 ExtremeI - 1
#define STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1	51
#define STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1	52
#define STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1	53
#define STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1		54
#define STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1		55
#define STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1		56
#define STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1		57
#define STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2	58		// 우각부 ExtremeI - 2
#define STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2	59
#define STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2	60
#define STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2	61
#define STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2		62
#define STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2		63
#define STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2		64
#define STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2		65
#define STM_MODEL_FOOT_STT_MAX_AXIAL				66		// 시점
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT		67		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT		68		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE		69		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE		70		
#define STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1		71		// ExtremeI - 1
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_1	72		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_1	73		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_1	74		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1	75		
#define STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2		76		// ExtremeI - 2
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_2	77		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_2	78		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_2	79		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2	80		
#define STM_MODEL_FOOT_END_MAX_AXIAL				81		// 종점
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT		82		
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT		83		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE		84		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE		85		
#define STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1		86
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_1	87
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_1	88		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_1	89		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1	90		
#define STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2		91
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_2	92		
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_2	93		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_2	94		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2	95
#define STM_MODEL_FOOT_MID_MAX_AXIAL				96		// 중간지점
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT		97		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT		98		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE		99		
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE		100		
#define STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_1		101		
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_1	102		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_1	103		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_1	104		
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_1	105		
#define STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_2		106
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_2	107		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_2	108		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_2	109
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2	110

enum EPartOfBridge { ePartUpperSlab, ePartLowerSlab, ePartOutWallStt, ePartFootStt, ePartWingWallStt, ePartApsStt, ePartOutWallEnd, ePartFootEnd, ePartWingWallEnd, ePartApsEnd, ePartInWall, ePartInFoot, ePartCount };
enum ETypeEnvComboLsd { eEnvLimit, eEnvExtremeI, eEnvExtremeII, eEnvService, eEnvService_Crack, eCountEnvTypeLsd};
enum EFootingCheckLsd { eFootCheckAxial, eFootCheckMomentMinus, eFootCheckMomentPlus, eFootCheckHoriMax, eFootCheckHoriMin, eCountFootCheck};
enum EWallCheckLsd { eWallCheckAxialMax, eWallCheckMomentMax, eWallCheckEccMax, eWallCheckHoriMax, eWallCheckAxialMin, eCountWallCheck};
enum EGoalOfEarthQuake { eGoalEQ_None = -1, eGoalEQ_Collapse, eGoalEQ_Function, eGoalEQ_Immediately, eGoalEQ_LongTerm, eGoalEarthQuakeCount };

#define LSD_DESIGN_WINGWALL		0
#define LSD_DESIGN_APPROACH		1
#define LSD_DESIGN_WINGWALL_SERVICE_JUDGE	0
#define LSD_DESIGN_WINGWALL_SERVICE_CHECK	1
#define LSD_DESIGN_APPROACH_SERVICE_JUDGE	2
#define LSD_DESIGN_APPROACH_SERVICE_CHECK	3

#define SCOFFOLD_WING_CON		0 //날개벽 측면포함(일체식 비계수량)
#define SCOFFOLD_WING_NOTCON	1 //날개벽 측면미포함(일체식 비계수량)

class CFactor;
class CARcBridgeDataStd;
class CParabola;
class CBoringData;
class CJijum;
class CWallData;
class CWallApp;
class CExFootingData;
class CExFootingApp;
class CFootingData;
class CFootingApp;
class CGuardWallRC;
class CFootway;
class CApsData;
class CApsApp;
class CWingWall;
class CSapData;
class CFEMManage;
class CConcBasic;
class CTypeDefault;
class CSaveCrossSection;
class CSaveXySlabUpperByLenFrSttStation;
class CSaveXyMatchLineAndSlabUpper;
class CSaveDistRigidEndSlab;
class CSaveDistRigidEndWall;
class CDesignExpansionJoint;
class CExpansionJointData;
class CLsdManagerBridge;
class CLsdRebarManager;
class CLsdEnviromentRating;
class CCheckCantileverData;

#include "TypeDefault.h"


#define iNORMAL	0
#define iEXP 1
#define COUNT_CONC_LSD		eCountEnvTypeLsd-1

class AFX_EXT_CLASS CRcBridgeData : public CObject  
{
public:
	double	GetCoverFooting(BOOL bStt, BOOL bUpper, long nDan=0);
	CString GetStringBridgeSpan();
//	void	SetHDanRoad(long nQtyHDan, long nIndex);
	long	GetCountCycleMainRebar();
	CFootingApp* GetFootingByJijumNumber(long nJijum);
	/// 구조계산관련 변수 ////////////////////////////////////////////////////
	CConcBasic *GetConcBasicByTitle(CSafetyTypedPtrArray <CObArray, CConcBasic*> *pArrConc, CString sTitle);
	long m_nSelectModelingCountEQ;
	BOOL m_bGoalEarthQuake[eGoalEarthQuakeCount];

	CFEMManage m_FemNormal;	// 평상시 (2D Frame)
	CFEMManage m_FemEQ[4];	// 내진해석 (2D Frame),0-단위하중,1-단일모드(좌측편토압,우측편토압) (ARCBRIDGE-3408) 내진설계기준 KDS 17 추가하면서 4개로 늘림.
	CFEMManage m_FemShell;	// 
	CFEMManage m_FemWingWall[2][2];	// 날개벽 Plate 해석 시.종 + 좌.우

	CFEMManage* GetFemWingWall(long nType);
	CFEMManage* GetFemWingWall(BOOL bStt, BOOL bLeft);
	CFEMManage* GetFemManage(long nTypeDesign);

	// SHELL 해석 결과 사용자 입력....
	BOOL m_bUseShellUserOutput;
	CMatrix m_MatShellSlab;
	CMatrix m_MatShellWallStt;
	CMatrix m_MatShellWallEnd;
	CMatrix m_MatShellWallMid[MAX_JIJUM_COUNT_RC];
	CMatrix m_MatShellPRF;
	//////////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcSlab[COUNT_CONC_LSD];			// 슬래브단면검토
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcGirder;			// 거더단면검토
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWall[COUNT_CONC_LSD];			// 벽체,기둥
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcInWall[COUNT_CONC_LSD];			// 내측벽체
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFoot[COUNT_CONC_LSD];			// 기초
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFootRev[COUNT_CONC_LSD];			// 기초 인장 위치 반대
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFootCheckAdd[COUNT_CONC_LSD];	// 기초 앞굽검토 위치 추가
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcPRF[COUNT_CONC_LSD];	// 

	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingSttLeft;		// 날개벽 시점 좌측
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingSttRight;	// 날개벽 시점 우측
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingEndLeft;		// 날개벽 종점 좌측
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingEndRight;	// 날개벽 종점 우측
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcArchRib;			// 아치리브 슬래브검토

	//////////////////////////////////////////////////////////////////////////

	double	m_dFckMassConc;
	double	m_dFckLeanConc;

	// 철도교관련 변수
	double	m_ZzdRail_HeightLane;
	double	m_dRail_HeightTie;
	double	m_ZzdRail_HeightDosangUnderTie;
	double	m_dRail_LengthTie;
	double	m_dRail_WidthTie;
	double	m_dRail_DistBetweenWheel;
	double	m_dRail_DistBetweenRail;
	CLineInfoApp* m_pLineApp;	// 라멘교의 선형 정보
	BOOL m_bQuickSearchLine;	// 빠른 선형 검색 적용

	BOOL	IsCns(BOOL bStt, BOOL bLeft, long nIdx);
	BOOL	IsAps(BOOL bStt, BOOL bLeft);
	BOOL	IsAnyAps();
	long	GetIdxInWall(CWallApp *pWall);
	long	GetCountFooting();
	BOOL	IsFooting(BOOL bStt);
	BOOL	IsFootingJijum(long nJijum);
	BOOL IsBoxType();
	BOOL IsArchType();
	CTwinVectorArray m_tvArrVertSection;	// 종단면 단면 좌표를 항상 최신으로 유지하고 있는 녀석.
	CTwinVectorArray m_tvArrBriSect;		// 날개벽 도면에서 쓰이는 종단면 좌표
	CTwinVectorArray m_tvArrVertSectionRib[2];	// 리브선 상하.
	CTwinVectorArray m_tvArrVertSectionRibCen;	// 리브축선
	BOOL	IsOutWall(BOOL bStt);
	void	InitWall(BOOL bStt);
	void	InitHunch(long nIdx);

	// 내부벽체
	CWallApp* GetInWall(long nIdx);
	long	GetCountInWall(BOOL bNotColumn=FALSE);
	long	GetCountInWallReal(BOOL bCheckTypeWall=FALSE);	// 실제 내측벽체의 개수를 구한다.
//	long	GetCountInWallEach(BOOL bColumn); // 내측벽체갯수중 벽체식과 기둥식을 각각 구함.
	BOOL	RemoveInWall(long nIdx);
	BOOL	MakeInWall();
	
	// 슈 지점 데이터
	void	MakeJijum(long nQtyMake);
	CJijum* GetJijum(int nJijum);
	int		GetCountJijum();

	// 날개벽
	CWingWall* GetWingWall(BOOL bStt, BOOL bLeft);
//	int GetCountWingWall();
	CTwinVectorArray m_tvArrPlaneWingWall;
	CTwinVectorArray m_tvArrFrontWingWall;

	// 기초
	CFootingApp* GetFooting(int nFooting);
	BOOL m_bAutoGroundInfo[MAX_JIJUM_COUNT_RC];			// 기초지반 - 자동적용
	BOOL m_bAutoCalcPileProperty[MAX_JIJUM_COUNT_RC];	// 말뚝특성치 자동계산
	long m_nSelect_Eo_VertType[MAX_JIJUM_COUNT_RC];		// 지반변형계수(Eo) 입력 방법
	long m_nSelect_Eo_HoriType[MAX_JIJUM_COUNT_RC];		// 지반변형계수(Eo) 입력 방법

	BOOL IsPiRahmen();
	BOOL IsRcSlab();

	CRcBridgeData();
	virtual ~CRcBridgeData();

	// common 함수
	CARcBridgeDataStd* m_pARcBridgeDataStd;
	void Serialize(CArchive &ar);
//	BOOL CheckDataValidity();
	CRcBridgeData& operator=(const CRcBridgeData& Obj);

	// 여기 아래는 저장되는 변수들
	// 시리얼 플래그
	long	m_nFlag;

	// 교량 형식 및 제원
	CString m_strBridgeName;		// 교량명
	long    m_nTypeBridge;			// 문형, ㅠ형, 슬래브, V형, 박스형, 경사 교각, 복합형 등..
	double  m_dStationBridgeStt;	// 시작 스테이션
	double  m_dLengthBridge;		// 총길이
	long    m_nQtyJigan;			// 지간수
	long	m_nSeparBri;			// 상하행 분리 형태(0:상하행 일체, 1:상행선, 2:하행선)

	CSafetyTypedPtrArray <CObArray, CJijum*> m_pArrJijum;
	BOOL	m_bUsePadShoe;			// 교량받침이 있는 경우 탄성받침을 사용할 것인지?
	
	long m_nIdxBridge;

	// 횡단 위치 입력
	long	m_nLineBase;			// 기준 선형
	long	m_nLineLeft;			// 좌측 선형
	long	m_nLineRight;			// 우측 선형
	double	m_dWidthSlabLeft;		// 슬래브 좌측단 거리
	double	m_dWidthSlabRight;		// 슬래브 우측단 거리
	double	m_dDisExtLineSttLeft;	//확폭일 경우에만 사용
	double	m_dDisExtLineSttRight;	//확폭일 경우에만 사용
	double	m_dDisExtLineEndLeft;	//확폭일 경우에만 사용
	double	m_dDisExtLineEndRight;	//확폭일 경우에만 사용

	double m_dDistRoadCenterLevelLeft;		// 도로중심 레벨거리 : 좌측
	double m_dDistRoadCenterLevelRight;		// 도로중심 레벨거리 : 우측


	// 지간 구성 입력
	BOOL	m_bExpJoint[MAX_JIJUM_COUNT_RC];	// 신축 이음 설치 유무
	long	m_nExpJointType[2];	// 신축 이음 형태 (교대, 교각)
	double	m_dLengthJigan[MAX_JIJUM_COUNT_RC-1];			// 지간 길이
	// 입력 기호와 지점간 길이를 저장(시리얼라이즈 안함). 치수선에 사용.
	CStringArray m_arrStrJijumLen;
	CDoubleArray m_arrJijumLen;
	BOOL	m_bEWDlgUsed;
	// 나머지는 CJijum 클래스에서 받음.
	long	m_nTypeShoeAngle;
	// 신축이음
	CDesignExpansionJoint	*m_pDesignExpanJoint;

	// 확폭 제원
	BOOL	m_bIsExp;			// 가각(학폭) 지원할지?
	double	m_dExpSttW[2];	// 시점부, [0]:좌, [1]:우
	double	m_dExpSttH[2];
	double	m_dExpSttR[2];
	double	m_dExpEndW[2];	// 종점부, [0]:좌, [1]:우
	double	m_dExpEndH[2];
	double	m_dExpEndR[2];
	BOOL	m_bExpCutStt[2];	// [0] : 좌
	BOOL	m_bExpCutEnd[2];	// [1] : 우
	BOOL	m_bExpSttRoundIsTan[2];	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
	BOOL	m_bExpEndRoundIsTan[2];


	// NOTCH 및 시공이음 규격
	long	m_nNotchType;
	double	m_dNotchHeight;
	double	m_dNotchWidth1;
	double	m_dNotchWidth2;
	double	m_dConsJointHeight;
	double	m_dConsJointWidth1;
	double	m_dConsJointWidth2;
	
	// 교량 받침 위치 입력
	// CJijum 클래스와 CShoePos 클래스에서 받음.

	CSafetyTypedPtrArray <CObArray, CGuardWallRC*> m_pArrGuardWall;		// 방호벽 입력
	CSafetyTypedPtrArray <CObArray, CGuardWallRC*> m_pArrGuardWallRoad;	// 방호벽 입력(지중라멘의 상부)
	CSafetyTypedPtrArray <CObArray, CFootway*> m_pArrFootway;			// 보도 입력
	BOOL	m_bRoundGW;
	BOOL	m_bRoundFW;
	BOOL	m_bGuardWallDlgUsed;		//슬래브 상부 구조물 입력
	BOOL	m_bGuardWallUnderDlgUsed;	//지중 라멘 - 도로 부속 구조물 입력(도로 상면)
	BOOL	m_bSSDlgUsed;

	// 헌치 두께는 종단 제원의 m_dHHS를 씀.
	double	m_dDH[2];			// 슬래브 끝단에서 헌치까지의 거리
	BOOL	m_bCantilSlab[2];	// 켄틸레버 
	int		m_nSlabChemfer[2];	// 슬래브 끝단 모따기 형태(0:없음, 1:직선, 2:곡선)
	double	m_dCantilH1[2];	// 켄틸레버 형태일때 높이1
	double	m_dCantilH2[2];	// 켄틸레버 형태일때 높이2
	double	m_dCantilB[2];	// 켄틸레버 형태일때 끝단에서의 거리
	double	m_dRE1[2];	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
	double	m_dRE2[2];	// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
	double	m_dTCW1[2];	// 슬래브 하부 끝단 모따기 처리시 헌치 폭
	double	m_dTCH1[2];	// 슬래브 하부 끝단 모따기 처리시 헌치 높이
	double	m_dTCW2[2];	// 슬래브 상부 끝단 모따기 처리시 헌치 폭
	double	m_dTCH2[2];	// 슬래브 상부 끝단 모따기 처리시 헌치 높이

	// 설계조건 -------------------------------------------------------
	double	m_dUnitWeigthBackFill;			// 뒷채움의 단위중량
	double	m_dUnitWeigthBackFillWater;	// 뒷채움의 수중단위중량
	double	m_dAngleFrictionBackFill;		// 뒷채움의 내부 마찰각
	double	m_dLiveLoadAlways;			// 상재 활하중
	double	m_dDeadLoadAlways;			// 상재 고정하중
	long	m_nSelectLiveLoadHori;				// 상재활하중에 대한 측벽하중 적용
	double	m_dLiveLoadFootway;			// 보도활하중
	double	m_dUnitWeightHeapSoil;		// 성토재료의 단위중량
	double	m_dUnitWeightValidDepth;	// 근입지반의 단위중량
	double	m_dValidDepthBaseGround;	// 기초의 유효근입 깊이
	double  m_dEo_CalcWallSpringEQ;		// 지진시 벽체부 SPRING계산을 위한 Eo
	double	m_dNValue_Eo_Hori_XXX;			// 수평 지반변형계수
	double	m_dNValue_Eo_Vert_XXX;			// 수직 지반변형계수
	long	m_nTypeToapSafeCheck;		// 안정검토시의 토압검토
	double	m_SafeFactorOverturning[2];	// 전도에 대한 안정률(평상시, 지진시)
	double	m_SafeFactorSliding[2];		// 활동에 대한 안정률(평상시, 지진시)
	long	m_nSelectFootForce;			// 지진시 기초작용력 가져오는 산식선택
	long	m_nSelect2DFrameTool;		// 2D Frame 해석 툴
	long	m_nSelect3DShellTool;		// 3D Shell 해석 툴
	long	m_nSelectMidasVer;			// MIDAS input 버전선택
	long	m_nSelect3DOutputValue;	// 3D 해석결과값 적용 방법
	long	m_nApplyWingWallCalcValueCount;		// 3D 해석결과값 적용 부재 수.
	long m_nSelectWingCalc;		// 날개벽 해석방법
	long	m_bApplyUserDispMax;		// 단위하중 최대변위 사용자 입력
	double	m_dUnitLoadDispMaxUser;
	long	m_nSelectWingLoad_LS_ES;
	long	m_nLiveLoadDistToBackWall;	// 날개벽 상재활하중 산정시 배면에서 떨어진 거리
	BOOL	m_bCorrectionFactor_LiveLoadAlways;	// 상재활하중 산정시 보정계수
	double	m_dCorrectionFactor_LiveLoadAlways;
	long	m_nApplyWingWallHeightBasePos;		// 관용법해석시 기준위치 

	// 한계상태
	double m_dEo_Input_Pile_XXX;			// 수평 지반변형계수 (Eo) : 0 = N치로 계산
	// ------------------------------------------------------ 설계조건

	//////////////////////////////////////////////////////////////////////////
	// 하중조합
	CMatrix	m_MatComboUlt;	// 계수하중 조합
	CMatrix m_MatComboEcc;	// 계수하중 조합 (최대편심)
	CMatrix m_MatComboEQ;	// 지진하중 조합
	CMatrix m_MatComboUse;	// 사용하중 조합

	CLongArray m_lArrComboUlt; // 해석데이타에 적용된 하중조합의 입력상(원래 하중조합 Combo)의 Index
	CLongArray m_lArrComboEcc;
	CLongArray m_lArrComboUse;

	DWORD   m_dWLoadNormal;	// 하중선택 (평상시)
	DWORD   m_dWLoadEQ;		// 하중선택 (지진시)
	BOOL	m_bApplyLoad[BRIDGE_LOAD_BASIC];
	BOOL	m_bApplyLiveLoadEQ;	// 지진시 활하중재하
	BOOL	m_bApplyAnchorForce;	// 앵커력 재하
	BOOL	m_bApplyCombo3D[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];	
	BOOL	m_bApplyGroundWater[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];
	double	m_dFactorComboVLoad;
	double m_dFactorAnchor[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];

	// 설계선택사항 -------------------------------------------------------
	BOOL	m_bCheckMainSlabAndFootTemper;	// 슬래브 및 기초판 수축온도철근비 검토
	BOOL	m_bSelectSupportBarWall;		// 수평철근량검토시 노출면 배치
	
	BOOL	m_bCheckSuppoerBarSlabMain;		// 수평철근비 슬래브 주철근비 [도.설 4.7.5.7(2)]
	BOOL	m_bCheckSuppoerBarSlabMainOver20;	// 수평철근비 슬래브 주철근비 20% 이상
	BOOL	m_bCheckSuppoerBarSlab300;		// 수평철근비 슬래브 300mm²이상
	BOOL	m_bCheckSuppoerBarSlabArea;		// 수평철근비 슬래브 단면적비

	BOOL	m_bCheckSuppoerBarWall15hs;		// 수평철근비 벽체 0.0015hs
	BOOL	m_bCheckSuppoerBarWallMainOver20;	// 수평철근비 슬래브 주철근비 20% 이상
	BOOL	m_bCheckSuppoerBarWall300;		// 수평철근비 벽체 300mm²이상
	BOOL	m_bCheckSuppoerBarWallArea;		// 수평철근비 벽체 단면적비

	BOOL	m_bCheckSuppoerBarFoot15hs;		// 수평철근비 기초 0.0015hs
	BOOL	m_bCheckSuppoerBarFootMainOver20;	// 수평철근비 슬래브 주철근비 20% 이상
	BOOL	m_bCheckSuppoerBarFoot300;		// 수평철근비 기초 300mm²이상
	BOOL	m_bCheckSuppoerBarFootArea;		// 수평철근비 기초 단면적비

	BOOL	m_bCheckSuppoerBarWing15hs;		// 수평철근비 날개벽 0.0015hs
	BOOL	m_bCheckSuppoerBarWing300;		// 수평철근비 날개벽 300mm²이상
	BOOL	m_bCheckSuppoerBarWingArea;		// 수평철근비 날개벽 단면적비
	double m_dCheckSuppoerBarWallArea;		// 수평철근비 벽체 단면적비
	double m_dCheckSuppoerBarWallAreaForOver400;		// 수평철근비 벽체 단면적비 Fy가 400이상이고 직경이 16이하인경우
	double m_dCheckSuppoerBarFootArea;		// 수평철근비 기초 단면적비
	double m_dCheckSuppoerBarSlabArea;		// 수평철근비 슬래브 단면적비
	double m_dCheckSuppoerBarWingArea;		// 수평철근비 날개벽 단면적비

	BOOL	m_bCheckSupportWallDryingShrinkage;	//건조수축 및 온도철근(벽체)
	BOOL	m_bCheckSupportWallMainRate; //수직 철근량의 25%
	BOOL	m_bCheckSupportFootDryingShrinkage;	//건조수축 및 온도철근(기초)
	BOOL	m_bCheckSupportSlabDryingShrinkage;	//건조수축 및 온도철근(슬래브)
	BOOL	m_bCheckSupportSlabByReqRebar; //필요철근량/사용철근량
	BOOL	m_bCheckSupportSlabMainRate; //주철근의 20%
	BOOL	m_bCheckSupportSlabMainRateSpan; //차량방향에 따른 주철근비
	BOOL	m_bSkipAsMinCheckByHoriRebar;		// 건조수축 및 온도철근으로 최소인장철근 만족 (도설 5.12.2.1(1)(다)) 

	long m_nSelectPileCalcSpringEQ;			// 말뚝기초 기초스프링강성계산시 고정적용 0 : FIX, 1:HINGE
	long m_nSelectApsShearCheck;			// 접속슬래브 전단검토시 하중산정 위치		

	BOOL	m_bCheckSuppoerBarArchRibMinDia1;		// 아치리브 배력철근 최소직경
	BOOL	m_bCheckSuppoerBarArchRibMinDia2;		// 아치리브 배력철근 주철근대비직경
	BOOL	m_bCheckFrontFootTrap2;				// 경사교2, 아치형라멘교의 앞굽검토(외측벽체연단)

	BOOL	m_bApplyMinAvSlab;				// 전단검토시 최소전단검토
	BOOL	m_bApplyMinAvWall;				// 전단검토시 최소전단검토
	BOOL	m_bApplyMinAvFoot;				// 전단검토시 최소전단검토
	BOOL	m_bApplyMinAvWing;				// 전단검토시 최소전단검토
	BOOL	m_bApplyMinAvAps;				// 전단검토시 최소전단검토
	BOOL	m_bApplyMinAvHyunTa;				// 전단검토시 최소전단검토
	BOOL	m_bCheckMinCtcShearWing;		// 날개벽 검토시 최소전단철근 간격 검토
	BOOL	m_bCheckMinCtcShearAps;		// 접속슬래브 검토시 최소전단철근 간격 검토
	BOOL	m_bCheckCornerInside;			// 우각부 검토시 내측인장 보강철근량 검토
	BOOL	m_bApplyFootingGForce;			// 지진시 기초 관성력 적용
	BOOL	m_bApplyMassCalcSafe;			// 안정검토시 MASS 작용력 적용
	BOOL	m_bApplyFootingMinThick;		// 기초두께 상한값 적용 (도.설.하 p85)
	BOOL m_bCheckRebarRateFooting;		//기초주철근의 온도철근비를 최소철근비로 적용(도로공사)
	long		m_nSelectApplyFs;
	long		m_nApplyCalcShearDistFront;		// 앞굽 전단검토시 위험단면 위치
	long		m_nApplyCalcShearDistBack;		// 뒷굽 전단검토시 위험단면 위치
	long		m_nTypeCrackDesignSlab;			// 강재의 부식에 대한 환경조건(상부슬래브)
	long		m_nTypeCrackDesignWall;			// 강재의 부식에 대한 환경조건(벽체)
	long		m_nTypeCrackDesignFoot;			// 강재의 부식에 대한 환경조건(기초(하부슬래브))
	long		m_nTypeCrackDesignWing;		// 강재의 부식에 대한 환경조건(날개벽)
	long		m_nTypeCrackDesignAps;			// 강재의 부식에 대한 환경조건(접속슬래브)
	long		m_nSelectJointUpperRange;		// 상부이음, 정착장 적용 범위
	long		m_nIdxCountCycleMainRebar;		// 주철근 배치 옵션(CYCLE)
	double	m_dFactorToap;						// 날개벽 계산시 토압에대한 하중계수
	double	m_dFactorLive;							// 날개벽 계산시 상재하중에대한 하중계수
	long		m_nSelFactorKhStt;					// 시점벽체 토압계수 산정시 수평토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
	long		m_nSelFactorKhEnd;					// 종점벽체 토압계수 산정시 수푱토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
	long		m_nSelCalcCornerFt;					// 절점부 보강철근 판정시 인장응력 ft 선택
	long		m_nSelectForceCheckPos;			// 단면검토 절점 위치
	long		m_nSelectRigidModeling;			// 모델링해석시 강역 설정...
	BOOL	m_bSelectRigidCalcHunch;		// 강역 설정시 헌치 두께 포함 (중간벽체)
	BOOL	m_bSelectTempLoadAddWall;	// 벽체에 온도하중 재하
	BOOL	m_bSelectShrinkLoadAddWall;	// 벽체에 건조수축 하중 재하
	BOOL m_bSelectEnvToapOne;			// 편토압 Env 적용
	BOOL m_bSelectEnvSubsid;			// 지점침하 Env 적용
	BOOL m_bSelectHunchModeling;
	BOOL m_bSelectHunchDesignCheck;
	long		m_nSelectTempLoadSlab;			// 슬래브에 온도하중 재하(온도하중 +/-, 상하면 온도차)
	long		m_nSelSubsideType;					// 한기초에 벽체가 두개 있을때 지점침하 선택
	long		m_nSelectDesignRoadWidth;		// 설계차로폭 선택
	long		m_nSelectLiveLoadSpanLength;
	long		m_nSelectDesignSpanLength;	// 설계지간의 선택
	long		m_nSelectDesignSpanApply;		// 상시 모델링시 경사지간 고려
	BOOL	m_bSelectSpanLengthApplyShoe;	// 설계지간 산정시 받침위치 고려
	long		m_nSelectWallSectionDesign;		// 벽체 단면 검토
	long		m_nSelectInWallSectionDesign;	// 내측벽체 단면 검토(0 : 슬래브, 1 : 벽체(기본값))
	long		m_nSelectFootingReactionForce;	// 벽체 단면 검토
	long		m_nArchRibSectionMomentPos;	// 아치리브 검토시 단면2차 모멘트 산정
	long		m_nSelectModelingHeight;		// 모델링 구성시 벽체 높이
	long		m_nSelectAlphaKhKv_XXX;				// 말뚝 스프링 정수 계산시 α값 적용
	BOOL	m_bCalcShearCenter;				// 부재중앙부 전단 검토
	long		m_nSelectStartLane;					// 차선하중 재하기준 위치
	BOOL	m_bCheckUseri;						// 활하중 계산시 충격계수 사용자입력 여부
	double	m_dCheckUseriValue;				// 활하중 계산시 사용자입력 충격계수
	long		m_nSelectApsAxleWeight;		// 접속슬래브 활하중산정시 축중 적용
	long		m_nSelectApsDeadLoad;		// 접속슬래브 활하중산정시 축중 적용
	BOOL	m_bApsDowelSeparateRebar;			// 다웰바 철근 분리 집계 
	BOOL	m_bOutWallUsebilityCheck;			// 시,종점부 벽체 휨압축부재 사용성검토
	// ------------------------------------------------------ 설계선택사항

	// 수량선택사항 -------------------------------------------------------
	long		m_nFormType_ConcNormal;		// 거푸집 : 일반콘크리트
	long		m_nFormType_Footing;				// 거푸집 : 기초
	long		m_nFormType_WallFront;			// 거푸집 : 벽체 전면
	long		m_nFormType_WallBack;			// 거푸집 : 벽체 후면
	long		m_nFormType_WingFront[2][2];	// 거푸집 : 날개벽 전면
	long		m_nFormType_WingBack[2][2];	// 거푸집 : 날개벽 배면
	long		m_nFormType_Aps;					// 거푸집 : 접속슬래브
	long		m_nFormType_MassConc;			// 거푸집 : mass 콘크리트
	long		m_nFormType_LeanConc;			// 거푸집 : 버림 콘크리트
	BOOL	m_bSelectBM_ConcCare_Main;		// 콘크리트양생 : 교대본체, 날개벽
	BOOL	m_bSelectBM_ConcCare_Aps;			// 콘크리트양생 : 접속슬래브, 완충슬래브
	long	m_nSelectBM_ConcCare_Cal;		// 콘크리트 양생 산출 방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
	long		m_nTypeRcBridgeBlock;			// 교대보호블럭 설치(0 : 없음, 1 : 하천용, 2 : 육교용)
	double	m_dAngleRcBridgeBlock;			// 교대보호블럭 외측설치각도(도)
	BOOL	m_bSelectBM_ApsCnsInclude;			// 수량집계에 접속슬래브, 완충슬래브 포함
	double	m_dPavementThick;					// 뒷채움시 콘크리트 포장두께
	double	m_dNosangThick;					// 뒷채움시 노상두께
	long		m_nTypeHeightWall;				// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이, 3 : 지점별 평균높이((최대+최소)/2)
	long		m_nTypeSlabLength;				// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
	long		m_nTypeSurfaceProof;				// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
	long		m_nTypePost;						// 본체 동바리 타입(육교용, 일반용)
	long		m_nTypePostBaseLine;				// 동바리 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	double	m_dPostLineEL;						// 사용자 입력 동바리 기준선 EL
	long		m_nTypeScaffoldBaseLine;			// 비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	long		m_nTypeScaffoldSideLocation;			// 날개벽 측면 기준선 (SCOFFOLD_WING_CON(0): 측면포함, SCOFFOLD_WING_NOTCON(1):측면 미포함)
	double	m_dScaffoldLineEL;					// 사용자 입력 비계 기준선 EL
	double	m_dThickPostUpper;				// 육교용 동바리 상부 두께(자동으로 구할 예정)
	double	m_dThickPostSide;					// 육교용 동바리 좌측 두께(우측은 8을 빼고난 나머지 값으로 자동 설정)
	double	m_dThickScaffold;					// 비계 두께
	double	m_dTimeConcForm;					// 콘크리트 양생, 거푸집 해체 시간
	BOOL m_bApplyYongSu;							//용수터파기

	long m_nDeductNearFormBM;				// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
	long m_nDeductNearTupagiBM;				// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)

	double	m_dAggregate_Footing;			// 콘크리트 골재최대치수(기초)
	double	m_dAggregate_Main;				// 콘크리트 골재최대치수(본체)
	double	m_dAggregate_Aps;				// 콘크리트 골재최대치수(접속슬래브)
	double	m_dAggregate_Lean;				// 콘크리트 골재최대치수(버림)
	double	m_dAggregate_Mass;				// 콘크리트 골재최대치수(MASS)
	double  m_dAggregate_Hyunta;				// 콘크리트 골재최대치수(현장타설말뚝)
	double	m_dAggregate_Etc;					// 콘크리트 골재최대치수(방호벽등)

	double	m_dSlump_Footing;					// 콘크리트 Slump(기초)
	double	m_dSlump_Main;					// 콘크리트 Slump(본체)
	double	m_dSlump_Aps;						// 콘크리트 Slump(접속슬래브)
	double	m_dSlump_Lean;						// 콘크리트 Slump(버림)
	double	m_dSlump_Mass;					// 콘크리트 Slump(MASS)
	double  m_dSlump_Hyunta;					// 콘크리트 Slump(현장타설말뚝)
	double	m_dSlump_Etc;						// 콘크리트 Slump(방호벽등)

	BOOL	m_bMonumenu;							// 교명주 받침 설치(OLD)
	long		m_nMonumenu;						// 교명주 받침 설치
	long		m_nDesPanel;						// 설명판 설치개수
	long		m_nNamePanel;						// 교명판 설치개수
	double	m_dAddLenghForGuardWallBM;	// 방호벽 수량 산출시 추가 길이(OLD)
	double	m_dAddLenghForGuardWall[2][2];	// 방호벽 수량 산출시 추가 길이 날개벽[시종점][좌우측] 
	long		m_nHeightForSlabBM;				// 슬래브 거푸집 수량 산출시 높이 기준	
	double	m_dTackCotting;						// 택 코팅 개수
	long		m_nTypePileMilkBM;				// 내부굴착말뚝의 밀크량 산출 옵션(0 : 직경+0.2 전체 채움, 1 : 직경+0.2 - 직경 부분만 채움)
	BOOL		m_bExcludeApsDowelRebar;		//다웰바 철근 철근가공조립 호함 여부(FALSE: 포함안함 기본값)
	double m_dSurplusSoil_HyunTaPile;  //현장타설말뚝 잔토처리
	double m_dConcreteSoil_HyunTaPile; //현장타설말뚝 콘크리트 타설(토사)
	double m_dConcreteRock_HyunTaPile; //현장타설말뚝 콘크리트 타설(암반)

	// ------------------------------------------------------ 수량선택사항
	// 피복설정 -------------------------------------------------------
	double	m_dCoverUpper_UpperSlab[MAX_REBAR_DAN];	// 상부슬래브 상면
	double	m_dCoverLower_UpperSlab[MAX_REBAR_DAN];	// 상부슬래브 하면
	double	m_dCoverSide_UpperSlab[MAX_REBAR_DAN];	// 상부슬래브 측면
	double	m_dCoverUpper_LowerSlab[MAX_REBAR_DAN];	// 하부슬래브 상면
	double	m_dCoverLower_LowerSlab[MAX_REBAR_DAN];	// 하부슬래브 하면
	double	m_dCoverWallOut[MAX_REBAR_DAN];		// 벽체외면
	double	m_dCoverWallIn[MAX_REBAR_DAN];		// 벽체 내면
	double	m_dCoverWallMid[MAX_REBAR_DAN];		// 중간벽체
	double	m_dCoverColumn[MAX_REBAR_DAN];		// 기둥
	double	m_dCoverWingOut[MAX_REBAR_DAN];		// 날개벽 전면
	double	m_dCoverWingIn[MAX_REBAR_DAN];		// 날개벽 배면
	double	m_dCoverWingUpper;					// 날개벽 상면
	double	m_dCoverWingEnd;						// 날개벽 끝단
	double	m_dCoverUpper_Footing[MAX_REBAR_DAN];	// 기초 상면
	double	m_dCoverLower_Footing_Lean[MAX_REBAR_DAN];	// 기초 하면(직접기초시)
	double	m_dCoverLower_Footing_Pile[MAX_REBAR_DAN];	// 기초 하면(말뚝기초시)
	double  m_dCoverRib[2][MAX_REBAR_DAN];	// 리브
	double	m_dCoverSide_Footing[MAX_REBAR_DAN];	// 기초 측면
	// ------------------------------------------------------ 피복설정

	// 기타조건 -------------------------------------------------------
	BOOL	m_bPileNvalueAuto;
	// ------------------------------------------------------ 기타조건

	// 종단면형상 -------------------------------------------------------
	long	m_nTypeJongdanForRebar;		// 종단면 타입 : 0 : 수평, 1 : 경사(배근시만 적용됨), 2 : 일반도를 위해서 종단선형으로 그림
	long	m_nTypeWallForRebarGen;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
	long	m_nTypeOutWall;				// 외측벽체 유무(좌우)
	long	m_bApplyAngleJongdan;

	// 시점측 벽체
	CFlexibleJoint m_fxJointStt;
	double	m_dFHS;
	double	m_dFLS;
	double	m_dFRS;
	double	m_dWS;
	double  m_dWSHW;	// 시점벽체 헌치너비(경사교2에서만 지원)
	double  m_dWSHH;	// 시점벽체 헌치너비(경사교2에서만 지원)
	double	m_dBHWS;
	double	m_dBHHS;
	double	m_dBHS;
	double	m_dBTHS;
	double	m_dBWS;
	double	m_dHWS;
	double	m_dHRS;		// 시점헌치 반경
	double	m_dELSB;
	CFootingApp m_footingStt;
	BOOL	m_bDivisionApsStt;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	long	m_nCountCnsStt[2];	// 완충슬래브 수
	CApsApp m_apsStt[2];
	CApsApp m_cnsStt[2][MAX_CNS];
	//CExFootingApp m_ExFootingStt;

	// 종점측 벽체
	CFlexibleJoint m_fxJointEnd;
	double	m_dFHE;
	double	m_dFLE;
	double	m_dFRE;
	double	m_dWE;
	double  m_dWEHW;	// 종점벽체 헌치너비(경사교2에서만 지원)
	double  m_dWEHH;	// 종점벽체 헌치너비(경사교2에서만 지원)
	double	m_dBHWE;
	double	m_dBHHE;
	double	m_dBHE;
	double	m_dBTHE;
	double	m_dBWE;
	double	m_dHWE;	// (상부, 하부) Box타입 추가
	double	m_dHHE;	// (상부, 하부) Box타입 추가
	double	m_dHRE;			// 종점헌치 반경
	double	m_dELEB;
	CFootingApp m_footingEnd;
	BOOL	m_bDivisionApsEnd;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	long	m_nCountCnsEnd[2];	// 완충슬래브 수
	CApsApp m_apsEnd[2];
	CApsApp m_cnsEnd[2][MAX_CNS];
	//CExFootingApp m_ExFootingEnd;

	// 내측벽체헌치 제원
	long	m_nCountInWallHunch;		// 내측벽체 헌치 개수
	double	m_dHHL[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 높이(좌측높이)	상부, 하부
	double	m_dHHR[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 높이(우측높이)
	double	m_dHBH[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치아래 돌출 높이
	double	m_dHLW[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 너비(왼쪽)
	double	m_dHRW[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 너비(오른쪽)
	double	m_dWL[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 시작위치(왼쪽)
	double	m_dWR[MAX_INWALL_HUNCH][2];	// 각 벽체 헌치 시작위치(오른쪽)
	double	m_dHLR[MAX_INWALL_HUNCH][2];	// 내측헌치 왼쪽 반경
	double	m_dHRR[MAX_INWALL_HUNCH][2];	// 내측헌치 오른쪽 반경

	// 헌치부 라운드 타입
	long m_nTypeRoundOfHunchStt;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
	long m_nTypeRoundOfHunchEnd;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
	long m_nTypeRoundOfHunchInWall[MAX_INWALL_HUNCH][2];	// 내측헌치 좌우측

	

	// 포물선(리브정의) 2개 (상판 하판)
	BOOL m_bConnectRibAndUpperSlab;	// 아치리브와 상부슬래브를 연결할지?
	CParabola m_pbRib[2];
	/*
	CDPoint m_xyRibCrown[2];
	double m_dRibLenTotal[2];
	double m_dRibHeightTotal[2];
	double m_dRibLenStt[2];
	double m_dRibLenEnd[2];
	double m_dRibFactorM[2];
	double m_dRibLenArchLine[2];
	double m_dRibOffsetFromUpper[2];	// 현재 포물선의 기준상단에서의 떨어진 거리
	*/

	// 포물선(리브정의) 중심선
	CParabola m_pbRibCen;
	/*
	CDPoint m_xyRibCrownCen;
	double m_dRibLenTotalCen;
	double m_dRibHeightTotalCen;
	double m_dRibLenSttCen;
	double m_dRibLenEndCen;
	double m_dRibFactorMCen;
	double m_dRibLenArchLineCen;
	double m_dRibOffsetFromUpperCen;	// 현재 포물선의 기준상단에서의 떨어진 거리
	*/

	double m_dRibBlockW[MAX_STRUP_REBAR];	// 리브위 사각 블럭 폭
	double m_dRibBlockH[MAX_STRUP_REBAR];	// 리브위 사각 블럭 높이
	// ------------------------------------------------------ 종단면형상

	// 날개벽 제원 입력
	CWingWall* m_pWingWallStt[2];		// 시점측 날개벽 [0]:좌, [1]:우
	CWingWall* m_pWingWallEnd[2];		// 종점측 날개벽 [0]:좌, [1]:우
	BOOL	m_bAutoWingEl;					// 날개벽 끝단 EL 자동 계산
	BOOL	m_bAutoWingHeightSoil;

	BOOL	m_bBracketLengthStt;			// 시점 브래킷 길이 설정 여부
	BOOL	m_bBracketLengthEnd;			// 종점 브래킷 길이 설정 여부
	double	m_dBrkLenStt[2];				// 시점 브래킷 이격 거리 [0]:좌, [1]:우
	double	m_dBrkLenEnd[2];				// 종점 브래킷 이격 거리 [0]:좌, [1]:우

	// 교명주 받침 제원
	double	m_dMonuH1;
	double	m_dMonuH2;
	double	m_dMonuH3;
	double	m_dMonuD;
	double	m_dMonuW;
	double	m_dMonuL;
	double	m_dMonuFck;
	double	m_dMonuR_Fy;
	double	m_dMonuR_DiaHor;
	double	m_dMonuR_DiaVer;
	double	m_dMonuR_H;
	double	m_dMonuR_L;
	int		m_nMonuR_nA;
	double	m_dMonuR_A;
	double	m_dMonuR_A1;
	int		m_nMonuR_nB;
	double	m_dMonuR_B;
	int		m_nMonuR_nC;
	double	m_dMonuR_C;
	double	m_dMonuR_C1;
	double	m_dMonuR_C2;
	double	m_dMonuR_C3;
	int		m_nMonuR_nD;
	double	m_dMonuR_D;
	double	m_dMonuR_D1;
	double	m_dMonuR_D2;
	double	m_dMonuR_D3;
	double	m_dMonuAggregate;

	// 기타 : 쓰지 않음 -> 각 방호벽 별로 Fck, Fy 값을 저장함
	double	m_dOther_Fck;
	double	m_dOther_Fy;

	CTypedPtrArray <CObArray, CTypeDefault*> m_pArrTypeDefault;
	// 기타
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;			// 콘크리트 영역
	
	// 수량 셀 참조용 구조체
	typedef struct STRUCT_REF_CELL
	{

	}structRefCell;

	double	m_dLengthRail[MAX_QTY_HDAN]	;		// 레일 간격(좌측), 철도교에서 사용
	long	m_nLineNumber[MAX_QTY_HDAN]	;		//해당 레일의 기준선형, 철도교에서 사용
	//횡단 슬래브 제원
	int		m_nRoadMaterial				;		// 도로 포장 재질(0:아스콘, 1:LMC)
	double	m_dTP						;		// 포장 두께
	double	m_dTS						;		// 슬래브 두께
	double	m_dTSOld;	// 경사가 고려되지 않은 org두께(경사식에서 도면그릴 때 m_dTS를 가공하는 경우가 있음)
	double	m_dHHS						;		// 시점헌치 두께 (상부, 하부) Box타입 추가
	BOOL	m_bLevelFrRoadCenterToEdge	;		// --도로중심간격 레벨 적용//횡단 슬래브 제원입력 추가사항 20050610
	BOOL	m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030

	// 철도교 슬래브 제원입력
	long	m_nQtyRail					;		// [슬래브 제원 입력], 레일 개소수, 철도교에서 사용
	double  m_dDisBaseElevation			;		// F.L 기준위치(철도교 도상높이 기준 위치)
	double  m_dDisBaseElevationRL		;		// --R.L 기준위치(철도교 도상높이 기준 위치)//횡단 슬래브 제원입력 추가사항 20050610
	double  m_dHeightRail				;		// --철도교 레일높이//횡단 슬래브 제원입력 추가사항 20050610
	double  m_dHeightDosangUnderTie		;		// 철도교 도상높이
	double  m_dSlopeDosang				;		// 철도교 도상 기울기
	double	m_dUnitWeightRail			;		// 궤도 단위중량

	// 지중라멘교 슬래브 제원입력
	BOOL	m_bUnderGroundRahmen		;		// 지중 라멘교
	BOOL	m_bCalcOpen;						// 지중라멘교를 노출라멘으로 계산
	BOOL	m_bPrevApplyCalcOpen		;		// 지중라멘교를 노출라멘으로 계산 옵션 적용전인지..
	BOOL	m_bUnderGroundPaveDetail	;		// 세부 포장 적용
	BOOL	m_bApplyLineSlope			;		// --횡경사 선형 편경사 적용//횡단 슬래브 제원입력 추가사항 20050610

	double	m_dWidthRoadLeft			;		// 좌측 노면 거리
	double	m_dWidthRoadRight			;		// 우측 노면 거리
	double	m_dUnderGroundCenterELStt	;		// 슬래브상단EL (종단시점의 EL)
	double	m_dUnderGroundCenterELEnd	;		// 슬래브상단EL (종단종점의 EL)
	long	m_nUnderGroundDesignType	;		// 노면활하중 종류
	long	m_nUnderGroundDesignSelect	;		// 노면활하중 설계기준
	long	m_nUnderGroundDistribute	;		// 철도 상재하중 분포폭 산정

	double	m_dUnderGroundSlopeSlabLeft;		// 삭제할 것	
	double	m_dUnderGroundSlopeSlabRight;		// 삭제할 것
	//
	double	m_dUnderGroundNoriSlopeFirst;
	double	m_dUnderGroundNoriSlopeNext;
	double	m_dUnderGroundNoriMaxHeight;
	double	m_dUnderGroundNoriWidth;
	double	m_dUnderGroundNoriSolpeSodan;
	//
	//
	long	m_nTypePavement;
	double  m_dUnderGroundHeightPavement;		// 포장두께
	BOOL	m_bUnderGroundDesignHeight;			// 계산토피 자동계산
	double  m_dUnderGroundDesignHeight;			// 계산토피
	double  m_dUnderGroundDesignLiveAlways;		// 노면활하중(상재하중)
	double  m_dUnderGroundHeightMortar;			// 방수 몰탈 두께
	double  m_dUnderGroundWeightMortar_xx;		// 방수 몰탈 단위중량(kgf/m3), // 기초자료의 m_UWeightCement를 사용
	double  m_dUnderGroundPaveHeightPyo;		// 표층
	double  m_dUnderGroundPaveHeightGee;		// 기층
	double  m_dUnderGroundPaveHeightMiddle;		// 중간층
	double  m_dUnderGroundPaveHeightBozo;		// 보조기층
	double  m_dUnderGroundPaveHeightDong;		// 동상방지층
	double  m_dUnderGroundPaveHeightEps;		// EPS
	double  m_dUnderGroundPaveHeightTosa_XXX;	// 토사				// (ARCBRIDGE-3184) 토사 높이를 계산해서 저장하지 않고 매번 계산해서 쓰도록 변경
	double  m_dUnderGroundPaveWeightPyo;		// 표층
	double  m_dUnderGroundPaveWeightGee;		// 기층
	double  m_dUnderGroundPaveWeightMiddle;		// 중간층
	double  m_dUnderGroundPaveWeightBozo;		// 보조기층
	double  m_dUnderGroundPaveWeightDong;		// 보조기층
	double  m_dUnderGroundPaveWeightEps;		// EPS
	double  m_dUnderGroundPaveWeightTosa;		// 토사
	double  m_dUnderGroundPaveWidthPyo;			// 표층
	double  m_dUnderGroundPaveWidthGee;			// 기층
	double  m_dUnderGroundPaveWidthMiddle;		// 중간층
	double  m_dUnderGroundPaveWidthBozo;		// 보조기층
	double  m_dUnderGroundPaveWidthDong;		// 보조기층
	double  m_dUnderGroundPaveWidthEps;			// EPS
	double  m_dUnderGroundPaveWidthTosa;		// 토사

	// 자동 최적 설계 관련 
	double  m_dOptimizeDiaMinimum;	// 주철근 최소직경
	double  m_dOptimizeDiaMaximum;	// 주철근 최대직경
	double  m_dOptimizeSafeRate;	// 최적화시 안전율
	BOOL    m_bOptimizeRebar2Dan;	// 2단철근을 적용할 것인지
	BOOL    m_bOptimizeRebarSplit;	// 1,2Cycle의 직경을 다르게도 적용할 것인지.
	BOOL    m_bOptimizeRebarMatch;	// 대칭위치의 철근이 같게 배치되도록 

	// 철근 할증률 설정 옵션
	long	m_nTypeAddRate;	// 0 : 직경별, 1 : 주철근,기타철근
	double	m_dAddRateLower;	// 하위 할증률
	double	m_dAddRateUpper;	// 상위 할증률
	double	m_dAddRateDiaOrg;	// 직경별일경우 기준 직경

	// BOX 타입 추가
	CFootingApp m_footingBox;

	BOOL	m_bApplyGroundWaterLevel;		// 지하수위 적용
	double	m_dGroundWaterLevel;			// 지하수위 EL
	double	m_dCoffKh_Sei;					// 내진해석시 설계수평지진계수 적용률;
	BOOL	m_bExistProtectionRiseFooting;	// [부상방지저판] 설치여부
	double	m_dPRF_W;						// [부상방지저판] 옆으로 내민 길이
	double	m_dPRF_H1;						// [부상방지저판] 경사면 높이
	double	m_dPRF_H2;						// [부상방지저판] 수직면 높이
	BOOL	m_bPRF_SameCTC_Main;			// [부상방지저판 철근] TRUE: 주철근CTC, FALSE: 주철근CTC/2
	double	m_dPRF_CTC_Main;				// [부상방지저판 철근] 표준간격
	double	m_dPRF_CTC_Support;				// [부상방지저판 철근] 수평철근 수평표준간격
	double	m_dPRF_CTC_SupportVert;			// [부상방지저판 철근] 수평철근 수직표준간격
	double	m_dPRF_CTC_Shear;				// [부상방지저판 철근] 전단철근 표준간격
	double	m_dPRF_Dia_Main;				// [부상방지저판 철근] 직경
	double	m_dPRF_Dia_Support;				// [부상방지저판 철근] 수평철근 직경
	double	m_dPRF_Dia_Shear;				// [부상방지저판 철근] 전단철근 직경
	long	m_nPRF_Type_Shear;				// [부상방지저판 철근] 전단철근 타입
	long	m_nPRF_Count_Shear;				// [부상방지저판 철근] 전단철근 주철근 감싸는 간격

	// 하부슬래브 입력
	double	m_dTS_Lower;					// 하부슬래브 두께
	long	m_nTypeLower;					// 기초 형식 (직접기초,MASS기초,말뚝기초,잡석기초,부력방지 Anchor)
	double	m_dThickCobbStone;				// 잡석기초 두께
	BOOL	m_bSlopeLowerSlab;				// 하부슬래브 종단적용 (FALSE), TRUE - 선형중심기준

	// 앵커관련 변수
	long	m_nCountAnchor;					// 앵커 종방향 개수
	BOOL	m_bAnchorSameValue;				// 앵커력,길이,직경 동일적용 (횡방향 입력시 1열만 입력)
	CDoubleArray	m_dArrDistJongAnchor;	// 앵커 종방향 거리
	CDoubleArray	m_dArrLoadAnchor;		// 앵커력
	CDoubleArray	m_dArrLengthAnchor;	// 앵커길이
	CDoubleArray	m_dArrDiaAnchor;		// 앵커직경
	CDoubleArray	m_dArrEaAnchor;		// 앵커개수 (횡방향)
	CDoubleArray	m_dArrDistHDanAnchor[RCBRIDGE_ANCHOR_MAX];	// 앵커 횡방향 간격

	// 기초부설정
	long m_nGroundFactorKV;
	double m_dGroundFactorKV;
	long m_nGroundFactorBv;
	long m_nEquivalentApplyKsb;
	double m_dEquivalentApplyKsb;		// 전단지반반력계수 산정시 Ksb = λ × Kv, λ = 1 / 입력값 
	long m_n2DGeneralSpringType;
	long m_n2DEQSpringType;
	long m_nVerticalFactor;
	double m_dSpringSpaceBottom;
	double m_dSpringSpaceWall;
	BOOL m_bSpringSpaceIncludeWallTick;

	HGDOUBLE m_dMeshSizeArcCorner;		// 우각부 Mesh Size
	HGDOUBLE m_dMeshSizeBracket;		// 브리캣 Mesh Size	
	HGDOUBLE m_dMeshSizeBase;			// 기초 Mesh Size
	HGDOUBLE m_dMeshSizeWing[2][2];			// 날개벽 Plate Mesh Size 시종, 좌우

	/////////////////////////////////////////////////////////////////////////

protected:
	CLineInfoApp *m_pLineBaseVert;	// 주철근 배치시 사보강배치를 위해서 임시로 생성하는 baseline 대체용

	// 횡단 구성 입력
	long	m_nQtyHDan;							// 횡단 구성 개수
	double	m_dLengthHDan[MAX_QTY_HDAN];		// 각 횡단 구간의 길이
	BOOL	m_bLevelHDan[MAX_QTY_HDAN];			// 각 횡단 구간의 레벨 처리 유무, m_bApplyLineSlope == TRUE 일 경우에만 유효
	double	m_dSlopeHDan[MAX_QTY_HDAN]	;		// 배수횡경사, 철도교에서 사용
	long	m_nTypeHDan[MAX_QTY_HDAN];			// 각 횡단 구간의 구성 형태(방호벽 등)
	BOOL	m_ZzbVariableLenHDan[MAX_QTY_HDAN];	// 각 횡단 구간의 가변 구간 설정 유무


public:
	double GetWidthHunchByJijum(long nJ, BOOL bUpper=TRUE);
	double GetWidthHunchBottomByJijum(long nJ, BOOL bUpper=TRUE);
	double GetHeightHunchByJijum(long nJ, BOOL bUpper=TRUE);
	BOOL IsRoundHunch(long nJ, BOOL bLeft, BOOL bUpperSlab);
	BOOL IsWall(long nJijum);
	BOOL GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2);
	// 지중라멘교관련사항
	long	m_nQtyHDanRoad;						// 횡단 구성 개수
	double	m_dLengthHDanRoad[MAX_QTY_HDAN];	// 각 구간의 길이
	BOOL	m_bLevelHDanRoad[MAX_QTY_HDAN];		// 각 횡단 구간의 레벨 처리 유무, m_bApplyLineSlope == TRUE 일 경우에만 유효
	double	m_dSlopeHDanRoad[MAX_QTY_HDAN];		// 각 구간의 구배
	long	m_nTypeHDanRoad[MAX_QTY_HDAN];		// 각 구간의 형태 : 차도,노견,중분대,다이크
	BOOL	m_ZzbVariableLenHDanRoad[MAX_QTY_HDAN];	// 각 횡단 구간의 가변 구간 설정 유무
	long	m_ZznVariableLenHDanRoad[MAX_QTY_HDAN];	//삭제할 것. 각 횡단 구간의 가변 구간 설정 유무

	//방호벽
	void	MakeGuardWall(long nQtyMake, BOOL bRoadNotSlab=FALSE);
	long	GetQtyGuardWall(BOOL bRoadNotSlab=FALSE) const;	//200506 현재 GetQtyHDan() 과 동일함.
	CGuardWallRC* GetGuardWallByHDan(long nHDan, BOOL bRoadNotSlab=FALSE);
	CString GetGuardWallName(CGuardWallRC* pGW, BOOL bNameForExcelSheet=FALSE);

	CGuardWallRC* GetGuardWallRoadGuardFenceAndCentralWall(long nGW, BOOL bRoadNotSlab=FALSE);	// 방호벽, 중분대, 중분대1/2
	CGuardWallRC* GetGuardWallRoadFootway(long nGW, BOOL bRoadNotSlab=FALSE);			// 보도
	CGuardWallRC* GetGuardWallRoadFootwayBlock(long nGW, BOOL bRoadNotSlab=FALSE);	// 보차도 경계석
	CGuardWallRC* GetGuardWallRoadSoundProof(long nGW, BOOL bRoadNotSlab=FALSE);		// 방음벽 기초
	CGuardWallRC* GetGuardWallRoadRetainingWall(long nGW, BOOL bRoadNotSlab=FALSE);	// 흙막이벽
	CGuardWallRC* GetGuardWallRoadHandRail(long nGW, BOOL bRoadNotSlab=FALSE);		// 난간기초	

	CGuardWallRC* GetGuardWallRailGuardFence(long nGW, BOOL bRoadNotSlab=FALSE);		// 연석	
	CGuardWallRC* GetGuardWallRailDetectGravel(long nGW, BOOL bRoadNotSlab=FALSE);	// 자갈막이(철근)
	CGuardWallRC* GetGuardWallRailDetectGravel2(long nGW, BOOL bRoadNotSlab=FALSE);	// 자갈막이(무근)
	CGuardWallRC* GetGuardWallRailFootway(long nGW, BOOL bRoadNotSlab=FALSE);			// 보도
	CGuardWallRC* GetGuardWallRailFootwayBlock(long nGW, BOOL bRoadNotSlab=FALSE);	// 연석+보도
	CGuardWallRC* GetGuardWallRailHole(long nGW, BOOL bRoadNotSlab=FALSE);			// 통신구
	CGuardWallRC* GetGuardWallRailHoleFootway(long nGW, BOOL bRoadNotSlab=FALSE);		// 통신구+보도
	CGuardWallRC* GetGuardWallRoadRailDike(long nGW, BOOL bRoadNotSlab=FALSE);		// 통신구+보도

	long	GetQtyHDanTotalGuardFence(BOOL bRoadNotSlab=FALSE);

	long	GetQtyHDanRoadGuardFence(BOOL bRoadNotSlab=FALSE);			// 방호벽	
	long	GetQtyHDanRoadCentralWallFull(BOOL bRoadNotSlab=FALSE);		// 중분대
	long	GetQtyHDanRoadCentralWallHalf(BOOL bRoadNotSlab=FALSE);		// 중분대1/2
	long	GetQtyHDanRoadFootway(BOOL bRoadNotSlab=FALSE);				// 보도
	long	GetQtyHDanRoadFootwayBlock(BOOL bRoadNotSlab=FALSE);		// 보차도 경계석
	long	GetQtyHDanRoadSoundProof(BOOL bRoadNotSlab=FALSE);			// 방음벽 기초
	long	GetQtyHDanRoadRetainingWall(BOOL bRoadNotSlab=FALSE);		// 흙막이벽	
	long	GetQtyHDanRoadHandRail(BOOL bRoadNotSlab=FALSE);			// 난간기초	
	long	GetQtyHDanRoadRailDike(BOOL bRoadNotSlab=FALSE);			// 다이크	

	long	GetQtyHDanRailGuardFence(BOOL bRoadNotSlab=FALSE);			// 연석	
	long	GetQtyHDanRailDetectGravel(BOOL bRoadNotSlab=FALSE);		// 자갈막이(철근)
	long	GetQtyHDanRailDetectGravel2(BOOL bRoadNotSlab=FALSE);		// 자갈막이(무근)
	long	GetQtyHDanRailFootway(BOOL bRoadNotSlab=FALSE);				// 보도
//	long	GetQtyHDanRailDosangVer(BOOL bRoadNotSlab=FALSE);			// 직결식도상
//	long	GetQtyHDanRailDosangGravel(BOOL bRoadNotSlab=FALSE);		// 자갈도상
	long	GetQtyHDanRailFootwayBlock(BOOL bRoadNotSlab=FALSE);		// 연석+보도
	long	GetQtyHDanRailHole(BOOL bRoadNotSlab=FALSE);				// 통신구
	long	GetQtyHDanRailHoleFootway(BOOL bRoadNotSlab=FALSE);			// 통신구+보도

	void	SetQtyHDan(long nQtyHDan) {m_nQtyHDan = nQtyHDan;}
	long	GetQtyHDan(BOOL bRoadNotSlab=FALSE) const;

	// 횡단 구성 입력
	void	SetLengthHDan(long nHDan, double dHDanLen) { m_dLengthHDan[nHDan] = dHDanLen; }
	double	GetLengthHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;				//각 횡단 횡단 입력 길이
	void	SetLevelHDan(long nHDan, BOOL bLevelHDan) {m_bLevelHDan[nHDan] = bLevelHDan;}
	void	SetSlopeHDan(long nHDan, double dSlopeHDan) { m_dSlopeHDan[nHDan] = dSlopeHDan; }
	double	GetSlopeHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;				//각 횡단 기울기(편경사, 입력값)
	void	SetTypeHDan(long nHDan, long nHDanType) { m_nTypeHDan[nHDan] = nHDanType; }
	long	GetTypeHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;					//각 횡단 횡단 방호벽 타입

	void	SetQtyHDanRoad(long nQtyHDan) {m_nQtyHDanRoad = nQtyHDan;}
	void	SetLevelHDanRoad(long nHDan, BOOL bLevelHDan) {m_bLevelHDanRoad[nHDan] = bLevelHDan;}
	void	SetLengthHDanRoad(long nHDan, double dHDanLen) { m_dLengthHDanRoad[nHDan] = dHDanLen; }
	void	SetSlopeHDanRoad(long nHDan, double dSlopeHDanRoad) { m_dSlopeHDanRoad[nHDan] = dSlopeHDanRoad; }
	void	SetTypeHDanRoad(long nHDan, long nHDanType) { m_nTypeHDanRoad[nHDan] = nHDanType; }
	

	BOOL	IsLevelFrRoadCenterToEdge() const { return m_bLevelFrRoadCenterToEdge; }
	void	SetLevelFrRoadCenterToEdge(BOOL bLevelFrRoadCenterToEdge) { m_bLevelFrRoadCenterToEdge = bLevelFrRoadCenterToEdge; }
	void	SetLengthRailByLine(long nRail, double dRailLen) { m_dLengthRail[nRail] = dRailLen; }
	double	GetLengthRailByLine(long nRail) const { return m_dLengthRail[nRail]; }
	void	SetRailLineNumber(long nRail, long nLineNum) { m_nLineNumber[nRail] = nLineNum; }
	long	GetRailLineNumber(long nRail) const { return m_nLineNumber[nRail]; }
	long	GetQtyRailNode() const { return m_nQtyRail; }
	void	SetQtyRailNode(long nQtyRail) { m_nQtyRail = nQtyRail; }
	double	GetDisBaseElevation() const { return m_dDisBaseElevation; }
	void	SetDisBaseElevation(double dDisBaseElevation) { m_dDisBaseElevation = dDisBaseElevation; }
	double	GetSlopeDosang() const { return m_dSlopeDosang; }
	void	SetSlopeDosang(double dSlopeDosang) { m_dSlopeDosang = dSlopeDosang; }
	double	GetHeightDosangInput() const { return m_dHeightDosangUnderTie; }
	void	SetHeightDosang(double dHeightDosang) { m_dHeightDosangUnderTie = dHeightDosang; }

	double	GetDisBaseElevationRL() const { return m_dDisBaseElevationRL; }
	void	SetDisBaseElevationRL(double dDisBaseElevationRL) { m_dDisBaseElevationRL = dDisBaseElevationRL; }
	double	GetHeightRail() const { return m_dHeightRail; }
	void	SetHeightRail(double dHeightRail) { m_dHeightRail = dHeightRail; }
	BOOL	IsApplyLineSlope() const { return m_bApplyLineSlope; }
	void	SetApplyLineSlope(BOOL bApplyLineSlope) { m_bApplyLineSlope = bApplyLineSlope; }

	
	double	GetWidthHunch(long nJijum, BOOL bStt, BOOL bUpper=TRUE);
	double	GetThickHunch(long nJijum, BOOL bStt, BOOL bIncludeVertHunch, BOOL bUpper=TRUE);
	long	GetTypeColumnByJijum(long nJijum);
	double	GetThickPave(BOOL bRoadNotSlab=FALSE) const;

	
	BOOL	IsHunch(long nJ, BOOL bUpper=TRUE);
	void	SetDefaultedState(long nIDDialog, DWORD dwDefaultState);
	BOOL	IsDefaulted(long nIDDialog, DWORD dwDefault);
	BOOL	IsFirstDefault(long nIDDialog);
	double	GetLoadFactor(long nTypeCombo, long nCombo, long nLoad);
	BOOL	IsPileBase(long nJijum);

	CTypeDefault * GetTypeDefault(long nIDDialog);
	BOOL    GetXyMatchSegAndSegTwinVector(CDPoint xyStt,CDPoint xyEnd, CTwinVectorArray& tvArr, long nQtyXy, CDPoint &xyResult=CDPoint(0,0));
//	double	GetHeightDosangUnderTie();

protected:
	// 가각부 횡단 EL 계산용
	CDPoint m_Aslope[30];
	long m_nQtyHDanSlope;
	double m_dDisExpLeft;
	double m_dDisExpRight;
	BOOL m_bSetGagakDist;

private:
	CSafetyTypedPtrArray <CObArray, CWallApp*> m_pArrInWall;

public:
	BOOL IsIncludeEHP();
	CString GetEhpDxfNameByDia(long nKindDia);
	void Sort(CTwinVectorArray &tvArr, double dCorrectLevelOfDoubleLoacl=0.0002);
	BOOL IsApplyTosaHeight();
	long GetQtyHDanNonRebarConcrete(BOOL bRoadNotSlab=FALSE);
	BOOL IsApplyDisBaseElevationFL();
	void InitSaveDistRigidEndWall();
	void InitSaveDistRigidEndSlab();
	void InitSaveXyMatchLineAndSlabUpper();
	void InitSaveXySlabUpperByLenFrSttStation();
	void InitSaveOrgInWall();
	void InitSaveCrossSection();
	void SetDataDefaultBridgeType();
	BOOL IsBlockOut(BOOL bStt);
	void AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, double dDistStt, double dDistEnd, BOOL bPline=TRUE);
	void AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, BOOL bPline=TRUE);
	void AddFromArch(CTwinVectorArray &tvArr, CDPoint xyMove, double dLenTotal, double dHeightTotal, double dMFactor, double dLenStt, double dLenEnd, double dLenArchLine, BOOL bPline=TRUE);
	CString GetStringJigan();
	void SortTvArrByLinePlane(CTwinVectorArray &tvArr);
	long GetCountVariableLenHDan(BOOL bRoadNotSlab);
	void SetVariableLenHDan(long nDan, HGBOOL bRoadNotSlab, BOOL bVariable);
	BOOL IsVariableLenHDan(long nDan, HGBOOL bRoadNotSlab);
	BOOL IsOutWallHunch(BOOL bStt);
	void SetDataInitForChangeType();
	CBoringDataEx *GetBoringDataEx(long nJijum);

	double GetThickOutWallByLine(BOOL bStt) { return m_dWOutWallByLine[bStt ? iSTT : iEND]; }
	double m_dWOutWallByLine[2];

	// 속도개선을 위해서 종단면 입력이 끝나면 직방향과 사방향 좌우중심선형기준으로한 종단면을 모두 구해 놓는다.
	// 그리고 종단 그리는 방법에 따라서도 모두 구해 놓는다.
	BOOL m_bSettingVertSection;	// 종단제원이 셋팅되어 있는지
	BOOL m_bNeedSettingVertSection;	// 종단제원이 셋팅이 필요한지?
	CTwinVectorArray m_tvArrVertSection_Vert_Left[3];
	CTwinVectorArray m_tvArrVertSection_Vert_Center[3];
	CTwinVectorArray m_tvArrVertSection_Vert_Right[3];

	// 사방향일 경우 날개벽 각도 고려한 종단면도 고려해야 된다. 0 : 고려안함, 1 : 고려함
	CTwinVectorArray m_tvArrVertSection_Ang_Left[3][2];
	CTwinVectorArray m_tvArrVertSection_Ang_Center[3][2];
	CTwinVectorArray m_tvArrVertSection_Ang_Right[3][2];

	// 가공되지 않은 종단좌표 
	// 아치교 전용임
	// 아치교는 m_tvArrVertSection을 리브/리브블럭관련해서 가공을 하므로 원본 좌표가 항상 필요함
	CTwinVectorArray m_tvArrVertSection_NonModify_ForArch;

	// 슬래브 평면 좌표(가각때문에 도저히 안 되겟으 ㅡㅡ)
	BOOL m_bSettingPlane;
	BOOL m_bNeedSettingPlane;
	CTwinVectorArray m_tvArrSttEndPlane[2][2];	// [2] : 시종점, [2] : 가각
	CTwinVectorArray m_tvArrLeftRightPlane[2][2];	// [2] : 좌우측, [2] : 가각

	// 횡단 좌표 보관
	CSafetyTypedPtrArray <CObArray, CSaveCrossSection*> m_pArrSaveCrossSection;
	// 배측벽체 기준점 보관
	CSafetyTypedPtrArray <CObArray, CSaveOrgInWall*> m_pArrSaveOrgInWall;
	//
	CSafetyTypedPtrArray <CObArray, CSaveXySlabUpperByLenFrSttStation*> m_pArrSaveXySlabUpperByLenFrSttStation;
	//
	CSafetyTypedPtrArray <CObArray, CSaveXyMatchLineAndSlabUpper*> m_pArrSaveXyMatchLineAndSlabUpper;
	//
	CSafetyTypedPtrArray <CObArray, CSaveDistRigidEndSlab*> m_pArrSaveDistRigidEndSlab;
	//
	CSafetyTypedPtrArray <CObArray, CSaveDistRigidEndWall*> m_pArrSaveDistRigidEndWall;

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	long m_nTypeJongdanForRebarOld;
	CTwinVectorArray m_tvArrVertSectionOld;
	BOOL m_bIsIncludeOnlyLine;	// 선형이 직선만 포함하는지.

	long m_nLeftCurVertSection;	// 현재 GetTvVertSection을 통해서 가져온 단면이 좌측인지 우측인지 센터인지 보관 
	long m_nLeftCurVertSectionOld;	// 현재 GetTvVertSection을 통해서 가져온 단면이 좌측인지 우측인지 센터인지 보관 

	// 중복호출 방지용
	long m_nLeftParaOld;
	BOOL m_bApplyAngleParaOld;
	BOOL m_bWingFrontParaOld;
	long m_nTypeJongdanParaOld;
	BOOL m_bForBMOfFootingParaOld;
	BOOL m_bFirstCalledGetTvVertSection_Std;
	BOOL m_bSecondCalledGetTvVertSection_Std;

	long m_nTypeJongdanForRebarOldOfOld;
	CTwinVectorArray m_tvArrVertSectionOldOfOld;
	long m_nLeftCurVertSectionOldOfOld;	// 현재 GetTvVertSection을 통해서 가져온 단면이 좌측인지 우측인지 센터인지 보관 
	CDPointArray m_xyArrOrdOldOfOld;
	CDoubleArray m_dArrElOldOfOld;

	// 수량산출을 위해 만들어진 교량인지 - 저장안함.
	BOOL m_bBridgeForBM;

	//////////////////////////////////////////////////////////////////////////
	// STM
	// 브라켓(시종점), 우각부(시종점, 상하단), 시종점기초, 중간기초
	BOOL m_bApplyStmModelingBracket[2];	// 모델링 타입 [시점, 종점]
	BOOL m_bStmTrussModelingBracket[2];	// Truss 모델링 적용 (??, 윤영묵교수) [시점, 종점]
	BOOL m_bApplyStmModelingCorner[2][3][2][2];	// [시점, 종점][극한한계, 극단상황1, 극단상황2][상단, 하단][-, +]
	BOOL m_bStmTrussModelingCorner[2][3][2][2];	// [시점, 종점][극한한계, 극단상황1, 극단상황2][상단, 하단][-, +]
	BOOL m_bApplyStmModelingFooting[2][3][5];	// [시점, 종점][극한한계, 극단상황1, 극단상황2][축력최대, 정모멘트최대, 부모멘트최대, 지진시] - 극한한계, 5번데이터는 한계상태에서만 사용
	BOOL m_bStmTrussModelingFooting[2][3][5];	// [시점, 종점][극한한계, 극단상황1, 극단상황2][축력최대, 정모멘트최대, 부모멘트최대, 지진시] - 극한한계, 5번데이터는 한계상태에서만 사용
	BOOL m_bApplyStmModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];	// [내측지점 갯수][극한한계, 극한한계1, 극한한계2][축력최대, 정모멘트최대, 부모멘트최대, 지진시]
	BOOL m_bStmTrussModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];	// [내측지점 갯수][축력최대, 정모멘트최대, 부모멘트최대, 지진시]
	// Stm - 하중조합
	CString m_strStmModelingCorner[2][3][2][2];						// 우각부 - [시점, 종점][극한한계, 극단상황1, 극단상황2][상단, 하단][-, +]
	CString m_strStmModelingBracket[2];								// 브라켓
	CString m_strStmModelingFooting[2][3][5];						// 기초 - 시종점 [시점, 종점][극한한계, 극단상황1, 극단상황2][축력최대, 정모멘트최대, 부모멘트최대, 지진시]
	CString m_strStmModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];		// 기초 - 중간지점 [내측지점 갯수][극한한계, 극한한계1, 극한한계2][축력최대, 정모멘트최대, 부모멘트최대, 지진시]

	HGBOOL IsApplyStmBracket();
	HGBOOL IsApplyStmCorner();
	HGBOOL IsApplyStmFooting();
	HGBOOL IsApplyStmMidFooting();

	HGINT32 m_ixSolverStmTool;

	CStmModelForArcbridge *m_pStmSolverInput;
	std::map<int, CStmModelForArcbridge*> m_mStmSolver;
	std::map<int, CStmModelForArcbridge*> m_mStmSolverOut;

	void InitStmData();
	void DeleteStmData();
	void SerializeStm(CArchive &ar);
	//////////////////////////////////////////////////////////////////////////

	// (LSD:0000)
public:
	CLsdManagerBridge *m_plsdManager; //하중조합 매니저
	CLsdRebarManager *m_plsdRebar; //정착,이음 매니저
	CLsdEnviromentRating *m_plsdEnvRating; // 노출등급, 설계등급

	CLsdManagerBridge *m_plsdMngForCantilever; //하중조합 매니저

	HGBOOL m_bEtaCombinEach;					// 하중수정계수 위치별로 각각 적용

	double m_Fck[ePartCount];	
	double m_Fy[ePartCount];
	double m_Fvy[ePartCount];
	double m_Ec[ePartCount];
	double m_Fctk[eLsdStmCount];				// 한계상태 - 콘크리트 인장강도
	HGBOOL m_bCheckCorner;						// 한계상태 - STM 우각부 최대 우력

	// 한계상태 하중 추가
	double m_dDepthApplySubSid;					// 지점침하 적용값
	long m_nSelectKindCoefficientKo;			// 정지토압계수 선택
	long m_nSelectInteriorWaterPress;			// 내부수압 선택( EV, BP, WA )
	double m_dHeightInteriorWaterPress;			// 내부수압 적용높이
	double m_dTemperatureChangeLoadGap;			// 온도하중 온도변화차
	double m_dTemperatureSlopeT[3];				// 온도경사 [T1,T2,T3]
	long m_nApplyPavement;						// 온도경사 - 하부의 온도가 높을때 선택 : 0 : 아스팔트포장, 1 : 콘크리트 포장

	long m_nLoadEffeciveW;						// 활하중 - 유효폭
	long m_nLoadEquiStrip;						// 활하중 - 등가스트립 폭에 사각고려
	HGBOOL m_bLoadMultiRoadSurConfficient;		// 활하중 - 유효폭 계산시 다차로 재하계수
	long m_nLoadStandardLane;					// 활하중 - 표준차로하중

	HGBOOL m_bCheckSupportSlabArea;			// 수평철근량 검토 - 슬래브
	HGBOOL m_bCheckSupportWallArea;			// 수평철근량 검토 - 벽체
	HGBOOL m_bCheckSupportFootArea;			// 수평철근량 검토 - 기초
	HGBOOL m_bCheckSupportWingWallArea;		// 수평철근량 검토 - 날개벽
	HGBOOL m_bCheckSupportApsArea;			// 수평철근량 검토 - 접속슬래브

	HGBOOL m_bAggregate_SlumpDetail;	// 재료 물성치 상세 입력 - 수량산출 옵션
	HGDOUBLE m_dAggregate[ePartCount];	// 콘크리트골재최대치수
	HGDOUBLE m_dAggregate_LeanLSD;		// 콘크리트골재최대치수(버림)
	HGDOUBLE m_dAggregate_MassLSD;		// 콘크리트골재최대치수(Mass)
	HGDOUBLE m_dAggregate_HyuntaLSD;	// 콘크리트골재최대치수(현장타설말뚝)
	HGDOUBLE m_dAggregate_EtcLSD;		// 콘크리트골재최대치수(방호벽등)

	HGDOUBLE m_dSlump[ePartCount];		// 콘크리트Slump
	HGDOUBLE m_dSlump_LeanLSD;			// 콘크리트Slump(버림)
	HGDOUBLE m_dSlump_MassLSD;			// 콘크리트Slump(Mass)
	HGDOUBLE m_dSlump_HyuntaLSD;		// 콘크리트Slump(현장타설말뚝)
	HGDOUBLE m_dSlump_EtcLSD;			// 콘크리트Slump(방호벽등)

	HGDOUBLE m_dThetaShearDesign;				// 전단검토시 Θ : 0 = 자동계산
	HGBOOL	m_bCheckDeltaTR;		// 전단력에 의한 종방향 추가인장력 검토
	HGDOUBLE m_dElasticModularRatioUseCheck;	// 사용성 검토시 유효탄성계수비 : 0 = 탄성계수비
	long m_nSelectUsebilityCheck;				// 사용성 검토시 균열검토
	long m_nTypeCrackWidthCondition;			// 균열폭계산 
	long m_nSelectExposureCoverCheck;			// 설계피복 산정
	long m_nSelectPminColumn;					// 기둥 최소 철근비

	long m_nApplyCalcShearDistSlab;				// 슬래브 전단검토시 위험단면 위치
	long m_nApplyCalcShearDistWall;				// 벽체 전단검토시 위험단면 위치
	long m_nApplyCalcShearDistFrontPile;		// 앞굽 검토시 위험단면 위치(말뚝기초)
	long m_nApplyCalcShearDistBackPile;			// 뒷굽 검토시 위험단면 위치(말뚝기초)
	BOOL m_bApplyT1CalcWingWall;				// 날개벽 D구간 철근량 산정시 적용 두께

	std::vector<CBridgeUserLoadSet*> m_vpUserLoadSet[5];	// (ARCBRIDGE-3409) BOX교 지진시 Case 추가. 3->5
	CCheckCantileverData *m_pCheckCantilever;			// 캔틸레버 검토 - 작용하중
	
//	void InitFemManage();
	void InitLsdData();
	void DeleteLsdData();
	void SerializeLsd(CArchive &ar);
	BOOL IsUsePart(EPartOfBridge ePart, long nType);
	CString GetStringPart(EPartOfBridge ePart);
	CString GetStringPart(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	CString GetStringPartMerge(EPartOfBridge ePart);
	long GetLsdMatrialIndex(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	long GetLsdMatrialTypePos(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	double GetCrackLimitWidth_LSD(EPartOfBridge ePart, HGBOOL bUpperOrFront, CString &sKind, CString &sDesingLevel);
	BOOL IsValidBridgePart(EPartOfBridge ePart);
	BOOL IsCheckPosEtaUseLoad(EPartOfBridge ePart);
	BOOL IsUseSelectLoadCaseWingOrApsMatchDesignRating(CString sGrade, EPartOfBridge ePart);
	BOOL IsUseLsdLoadFactor(long nIdexCombin);
	BOOL IsDesignRatingSelectAbutLoadCaseKind(ELSDCombKind eKind, EPartOfBridge ePart);
	BOOL IsValidComboEnvType(ETypeEnvComboLsd eTypeEnvLsd);
	CString GetStringComboEnv_Lsd(ELSDCombKind eKind, EPartOfBridge ePart, BOOL bMark=TRUE, BOOL bServiceCrack=FALSE);
	CString GetStringComboEta_Lsd(ELSDCombKind eKind, EPartOfBridge ePart, long nIndexCombo=-1, BOOL bOrigne=FALSE, long nIdxModel=0);
	CString GetStringComboEnv(long nTypeDesign, ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos);
	CString GetStringComboEnvType_Lsd(ETypeEnvComboLsd eTypeEnvLsd, BOOL bSubClass=TRUE, BOOL bTwoLine=FALSE);
	CString GetStringFootingCheckCase(EFootingCheckLsd eCheck);
	CString GetStringWallCheckCase(EWallCheckLsd eCheck);
	ELSDLoadGroupKind GetGroupKindByEnvType(ETypeEnvComboLsd eTypeEnvLsd);
	ELSDCombKind GetComboKindByEnvType(ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos);
	ELSDCombKind GetComboKindServiceByDesignGrade(EPartOfBridge ePart, long nPos);
	void SetDataDefaultSelectComboForWingWallAndAps(ELSDLoadGroupKind eType);
	long GetSizeComboByType(ETypeEnvComboLsd eEnvTypeLsd, EPartOfBridge ePart, CString &szComboFirst);
	long GetCountApplyEnvType(ETypeEnvComboLsd eCountEnvType = eCountEnvTypeLsd);

	HGINT32 GetCountUserLoadSetLsd(long nKind);
//	void ResizeUserLoadSetLsd(BOOL bNormal, HGINT32 icSize);
	void InsertUserLoadSetLsd(long nKind, HGINT32 Idx);
	void EraseUserLoadSetLsd(long nKind, HGINT32 Idx);
	CString GetStringUserLoadType(HGINT32 nType);

	CString GetStringCombo(ELSDCombKind eComboKind, long nIndexCombo, HGBOOL bForCantilever);
	CString GetLoadCaseName(ELSDLoadCaseKind eLoadCaseKind);

	HGBOOL &GetMaterialDetail();
	void SetMaterialDetail(HGBOOL bMaterialDetail);
	HGDOUBLE &GetRateDryAndShrinkage();
	void SetRateDryAndShrinkage(HGDOUBLE dRateDryAndShrinkage);
	HGDOUBLE &GetCoefficientExpansion();
	void SetCoefficientExpansion(HGDOUBLE dCoefficientExpansion);
	HGBOOL &GetDBar_ForDowelBar();
	void SetDBar_ForDowelBar(HGBOOL bIsDBar_ForDowelBar);
	long &GetTypeDesingEQ();
	void SetTypeDesingEQ(long nTypeDesingEQ);
	long &GetDegreeEarthQuake();
	void SetDegreeEarthQuake(long nDegreeEarthQuake);
	long &GetRegionEarthQuake();
	void SetRegionEarthQuake(long nRegionEarthQuake);
	HGDOUBLE &GetCoefficientGround();
	void SetCoefficientGround(double dCoefficientGround);
	long &GetTypeCoefficientGround();
	void SetTypeCoefficientGround(long dTypeGround);
	double &GetRateDAMP_EQ_CD();
	void SetRateDAMP_EQ_CD(double dDampRate);
	HGDOUBLE &GetGravity();
	void SetGravity(HGDOUBLE gValue);
	long &GetTypeRigidFootingEQ();
	void SetTypeRigidFootingEQ(long nTypeRigidFootingEQ);
	HGBOOL &GetApplyRigidWallEQRotate();
	void SetApplyRigidWallEQRotate(HGBOOL bApplyRigidWallEQRotate);
	long &GetTypeJointPosFootingEQ();
	void SetTypeJointPosFootingEQ(long nTypeJointPosFootingEQ);
	long &GetTypePropertyPosFootingEQ();
	void SetTypePropertyPosFootingEQ(long nTypePropertyPosFootingEQ);
	long &GetTypeSoilPressureEQ();
	void SetTypeSoilPressureEQ(long nTypeSoilPressureEQ);
	long &GetTypeUpperWeightEQ();
	void SetTypeUpperWeightEQ(long nTypeUpperWeightEQ);
	CString GetStringDegreeEarthQuake();

	// Stm
	HGDOUBLE &GetValueStmFck(long nStmType);
	HGDOUBLE &GetValueStmFctk(long nStmType);
	HGDOUBLE &GetValueStmEc(long nStmType);
	HGDOUBLE &GetValueStmFy(long nStmType);
	HGDOUBLE &GetValueStmFvy(long nStmType);
	
	CString GetStrLsdStmTitle(long nStmType);

	void InitAggregateAndSlump();

	HGDOUBLE GetValueAggregate(EPartOfBridge ePos, HGBOOL bNotLsd=FALSE) const;
	HGDOUBLE GetValueAggregateLean() const;
	HGDOUBLE GetValueAggregateMass() const;
	HGDOUBLE GetValueAggregateHyunta() const;
	HGDOUBLE GetValueAggregateEtc() const;
	
	HGDOUBLE GetValueSlump(EPartOfBridge ePos, HGBOOL bNotLsd=FALSE) const;
	HGDOUBLE GetValueSlumpLean() const;
	HGDOUBLE GetValueSlumpMass() const;
	HGDOUBLE GetValueSlumpHyunta() const;
	HGDOUBLE GetValueSlumpEtc() const;

	CString GetStringLoadTemperatureSlopeApply();

	long GetLsdEnvType(HGINT32 iKey);

	BOOL IsBoredPileASD(long nJijum);
	BOOL IsPhcPileASD(long nJijum);
	BOOL IsApplyEarthQuakeDesign();

	long GetEarthQuakeModelIndex(EGoalOfEarthQuake eGoalEq);
	long GetCountEarthQuakeModel(BOOL bResult=TRUE);
	EGoalOfEarthQuake GetEarthQuakeGoalByIndex(long nIndexEq);
	CString GetStringGoalEarthQuake(EGoalOfEarthQuake eGoalEq, BOOL bLong=TRUE);
	CString GetNameStringEarthQuakeModel(long nDesignType);

protected:
	void InitConcSlab();
	void InitConcWall();
	void InitConcInWall();
	void InitConcPRF();
	void InitConcFoot();
	void InitConcFootRev();
	void InitConcFootCheckAdd();

protected:
	HGBOOL m_bMaterialDetail;			// 재료 물성치 상세 입력
	double  m_dRateDryAndShrinkage;		// 건조수축율
	double  m_dCoefficientExpansion;	// 가동받침의 이동량 산정시 선팽창계수
	BOOL m_bIsDBar_ForDowelBar;	// 다웰바 강종

	long m_nTypeDesingEQ;			// 내진해석 방법
	long m_nDegreeEarthQuake;		// 내진등급
	long m_nRegionEarthQuake;		// 지진구역
	double m_dCoefficientGround;	// 지반계수
	double m_gValue;				// 중력가속도
	long m_nTypeRigidFootingEQ;		// 내진해석시 기초부 모델링 방법
	BOOL m_bApplyRigidWallEQRotate;		// 벽체강성 적용시 회전강성 적용
	long m_nTypeJointPosFootingEQ;		// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
	long m_nTypePropertyPosFootingEQ;	// 내진해석시 기초 중앙으로 모델링하는 경우의 강성
	long m_nTypeSoilPressureEQ;		// 내진해석시 수평토압의 재하
	long m_nTypeUpperWeightEQ;		// 내진해석시 상부중량 설정	
	long m_nTypeCoefficientGround;		// 지반종류
	double m_dRateDAMP_EQ_CD;		// 감쇠비(ξ)
};

#endif // !defined(AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_)
