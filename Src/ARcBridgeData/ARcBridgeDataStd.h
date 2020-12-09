// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDataStd.h: interface for the CARcBridgeDataStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_)
#define AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ARcBridge Lock Version
#define  ARCBRIDGE_LOCK_CODE_PORTAL		0x0001
#define  ARCBRIDGE_LOCK_CODE_PI			0x0002
#define  ARCBRIDGE_LOCK_CODE_SLAB		0x0004
#define  ARCBRIDGE_LOCK_CODE_TRAP		0x0008
#define  ARCBRIDGE_LOCK_CODE_TRAP2		0x0010
#define  ARCBRIDGE_LOCK_CODE_ARCH		0x0020
#define  ARCBRIDGE_LOCK_CODE_BOX		0x0040
#define  ARCBRIDGE_LOCK_CODE_SLAB_STD	0x0080
#define  ARCBRIDGE_LOCK_CODE_STM		0x0100
#define  ARCBRIDGE_LOCK_CODE_LSD		0x0200

#define DESIGN_METHOD_STRENGTH		0
#define DESIGN_METHOD_LSD			1

#define DESIGN_CONDITION_CONCRETE	0
#define DESIGN_CONDITION_ROAD		1
#define DESIGN_CONDITION_RAIL		2
#define MAX_CONDITION_TYPE			3

#define DESIGN_CODE_KCI_2003		0
#define DESIGN_CODE_KCI_2007		1
#define DESIGN_CODE_KCI_2012		2

#define DESIGN_ROAD_2008			0
#define DESIGN_ROAD_2010			1
#define DESIGN_ROAD_2015			2
#define DESIGN_ROAD_KDS_2019		3

#define DESIGN_RAIL_2004			0
#define DESIGN_RAIL_2017			1
#define DESIGN_RAIL_KDS_2018		2

//절곡 철근 
#define JOINT_BEBD_RADIUS	0
#define JOINT_BEBD_CHAMFER	1

#define MAX_REBAR_DETAIL	45

#define REBAR_RADIUS		22.5

#define MAX_UPPER_TYPE		6
#define MAX_PIER_TYPE		7
#define MAX_CONDITION_TYPE	3
#define MAX_CRACK_ENV_TYPE	9

#define BMTYPE_NORMAL		0	// 일반수량
#define BMTYPE_RESULT		1	// 실적수량
#define MAX_BM_FORM_TYPE		8
#define MAX_BM_FORM_TYPE_NORMAL	7


// 상행선, 하행선
#define UPPER_LINE 0
#define LOWER_LINE 1

// 단위계 종류 
#define UNIT_TYPE_CGS	0
#define UNIT_TYPE_SI	1
#define UNIT_TYPE_MAX	2

#define UNIT_KIND_MAX	20

//지반
#define MAX_JIBAN_COUNT 20

#define JOINT_UPPERSLAB	0
#define JOINT_OUTWALL		1
#define JOINT_MIDWALL		2
#define JOINT_NLLINK		3
#define JOINT_ARCHRIB		4
#define JOINT_LOWERSLAB	5

#define ELEMENT_UPPERSLAB	0
#define ELEMENT_STTWALL	1
#define ELEMENT_ENDWALL	2
#define ELEMENT_MIDWALL	3
#define ELEMENT_RIGID		4
#define ELEMENT_ARCHRIB	5
#define ELEMENT_LOWERSLAB	6

//////////////////////////////////////////////////////////////////////////
#define LOAD_CASE_DEAD			0x00000001 // 고정하중, 연직토압
#define LOAD_CASE_SOIL			0x00000002 // 고정하중, 연직토압
#define LOAD_CASE_LIVE_FULL	0x00000004 // 활하중 (만재)
#define LOAD_CASE_LIVE_ECC		0x00000008 // 활하중 (편재)
#define LOAD_CASE_TANK			0x00000010 // 탱크 및 특수 중차량하중
#define LOAD_CASE_TOAP_BOTH	0x00000020 // 양토압
#define LOAD_CASE_TOAP_ONE		0x00000040 // 편토압
#define LOAD_CASE_TEMP1		0x00000080 // 온도하중(+)
#define LOAD_CASE_TEMP2		0x00000100 // 온도하중(-)
#define LOAD_CASE_SHRINK		0x00000200 // 건조수축
#define LOAD_CASE_SUBSID		0x00000400 // 지점침하
#define LOAD_CASE_SB			0x00000800 // 시동 및 제동하중
#define LOAD_CASE_LR			0x00001000 // 장대레일 종하중
#define LOAD_CASE_EQ			0x00002000 // 지진하중
#define LOAD_CASE_WATERHORI	0x00004000 // 수평수압
#define LOAD_CASE_WATERVERT	0x00008000 // 연직수압

// LOAD_CASE와 같은 순서로 설정.... //////////////////////////////////////
#define BRIDGE_LOAD_DEAD		 0 // 고정하중
#define BRIDGE_LOAD_SOIL		 1 // 연직토압
#define BRIDGE_LOAD_LIVE_FULL	 2 // 활하중 (만재)
#define BRIDGE_LOAD_LIVE_ECC	 3 // 활하중 (편재)
#define BRIDGE_LOAD_TANK		 4 // 탱크 및 특수 중차량하중
#define BRIDGE_LOAD_TOAP_BOTH	 5 // 양토압
#define BRIDGE_LOAD_TOAP_ONE	 6 // 편토압
#define BRIDGE_LOAD_TEMP1		 7 // 온도하중(+)
#define BRIDGE_LOAD_TEMP2		 8 // 온도하중(-)
#define BRIDGE_LOAD_SHRINK		 9 // 건조수축
#define BRIDGE_LOAD_SUBSID		10 // 지점침하
#define BRIDGE_LOAD_SB			11 // 시동 및 제동하중
#define BRIDGE_LOAD_LR			12 // 장대레일 종하중
#define BRIDGE_LOAD_EQ			13 // 지진하중
#define BRIDGE_LOAD_WATERHORI	14 // 수평수압
#define BRIDGE_LOAD_WATERVERT	15 // 연직수압
#define BRIDGE_LOAD_BASIC		16 // 기본하중의 개수
//////////////////////////////////////////////////////////////////////////

#define LOAD_COMBINE_USE	0
#define LOAD_COMBINE_ULT	1
#define LOAD_COMBINE_ECC	2
#define LOAD_COMBINE_EQ	3

// 검토위치
#define CHECK_POS_LEFT		0	
#define CHECK_POS_TOP		0
#define CHECK_POS_CENTER	1
#define CHECK_POS_RIGHT	2
#define CHECK_POS_BOTTOM	2

// 지진시 (단일모드)
#define EARTHQUAKE_LOAD_DEAD		0 // 고정하중
#define EARTHQUAKE_LOAD_TRAP_X		1 // 종방향 TRAP하중
#define EARTHQUAKE_LOAD_TRAP_Y		2 // 횡방향 TRAP하중
#define EARTHQUAKE_LOAD_TOAP		3 // 수평토압,수평수압
#define EARTHQUAKE_LOAD_INERTIAL	4 // 관성력

// 철도하중 //////////////////////////////////////////////////////////////////////////
#define RAIL_LOAD_KRL2012	0
#define RAIL_LOAD_KRL2012_P	1
#define RAIL_LOAD_EL18_2017	2
#define RAIL_LOAD_USER_2017	3
#define MAX_RAIL_LOAD_2017	4

#define RAIL_LOAD_LS25	0
#define RAIL_LOAD_LS22	1
#define RAIL_LOAD_LS18	2
#define RAIL_LOAD_EL25	3
#define RAIL_LOAD_EL22	4
#define RAIL_LOAD_EL18	5
#define RAIL_LOAD_HL25	6
#define RAIL_LOAD_USER	7
#define MAX_RAIL_LOAD	8

#define RAIL_LOADTYPE_PD	0
#define RAIL_LOADTYPE_P		1
#define RAIL_LOADTYPE_W		2

#define UNIT_METER	1.000
//////////////////////////////////////////////////////////////////////////

// 진행 종류.
#define TYPE_PROCESS_NORMAL					0	// 일반
#define TYPE_PROCESS_SLAB_STANDARD_OUTPUT	1	// 슬래브표준도

//주철근구간수
#define CUL_MAX_REBAR	40	

// LSD STM Footing
#define LSD_STM_FOOTING_ULT		0
#define LSD_STM_FOOTING_EXT1	1
#define LSD_STM_FOOTING_EXT2	2
#define LSD_STM_FOOTING_MAX		3

struct SStxOptionItem
{
	CString name;
	HGBOOL check;

	HGINT32 designType;
	HGINT32 iRcBridge;
	HGINT32 ixJijum;

	SStxOptionItem()
	{
		name = NOTRANS("");
		check = TRUE;

		designType = 0;
		iRcBridge = 0;
		ixJijum = 0;
	}

	BOOL operator==(const CString &compare_name)
	{
		if(name == compare_name)
			return TRUE;
		return FALSE;
	}
};

class CLineInfoApp;
class CLineInfo;
class CRcBridgeRebar;
class CRcBridgeApp;
class CRcBridgeData;
class CWallApp;
class CColumnApp;
class CBoringDataEx;
class CExpJointRefArr;
class CLsdEnviromentCoef;
class CLsdRebarManager;
class CUserBmData;

class AFX_EXT_CLASS CARcBridgeDataStd : public CObject
{
public:
	void SerializeBase(CArchive &ar);
	BOOL IsSupportTypeStd(long nType);
	void MakeLinefoBridge(long nBridge);
	BOOL IsEnableSlabStandardOutput();
	void SetLineAppToBridge();
	CString GetFileNameForExcel(long nBri);
	void SetVersionFlag(DWORD dwType);	
	BOOL IsLockMaster();
	BOOL IsLockRcBridgePlus();	
	BOOL IsLockRcBridgeBasic();
	BOOL IsLockRcBridgePortal();
	BOOL IsLockRcBridgeBoxModule();
	BOOL IsLockRcBridgeBox();
	BOOL IsLockRcBridgeStmModule();
	BOOL IsLockRcBridgeLsdModule();
	BOOL m_bOnlyProjectInfoLoad;	// 특별한 옵션.. 프로젝트 정보만 읽어 올지??
	double GetValueBasic(double dDia, long nType);
	double GetValueBasic(long nDia, long nType);
	BOOL IsRailLoadEL();
	BOOL IsRailLoadLS();
	void SetStringUnitType();
	BOOL IsSI();
	double GetUnitWeightReinforcedConcrete();
	double GetValueUnitType(double dValue, CString csOrg, CString csChg);
	CString GetStringUnitType(CString sCGS, long nIdxUnitApply=0);

	CString GetBridgeTotalName(long nBri);
	void CopyToCoupleBridge_RebarStandardSection(long nBri);
	void CopyDoubleArray(CDoubleArray *dArrTarget, CDoubleArray *dArrSource);
	void CopyArrayOfDoubleArray(CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrTarget, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrSource);
	void CopyArrayOfSetRebarJoint(CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrSource);
	void CopyArrayOfSetRebarDan(CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrSource);
	void CopyToCoupleBridge_RebarMain(long nBri);
	void CopyToCoupleBridge_VertSection(long nBri);
	void CopyToCoupleBridge_SpanConst(long nBri);
	long GetIdxMatchBridge(long nBri);
	CRcBridgeApp* GetBridgeMatch(CRcBridgeApp* pBri);
	CRcBridgeRebar* GetBridgeMatchRebar(CRcBridgeRebar* pBri);
	CRcBridgeApp* GetBridge(long nBridge=0);
	CRcBridgeRebar* GetBridgeRebar(long nBridge=0);
	CLineInfoApp* GetLineApp();
	BOOL MakeRcBridgeForBM(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM, BOOL bForTogong=FALSE);
	void SyncBridgeFactor(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM);
	// rcbridge
	long GetBridgeSize(BOOL bBridgeCount=FALSE);

	// common
	void GetPointArrayCountAndValue(CDPointArray& xyArr, CDoubleArray& dArr);
	void SerializeArrayOfDoubleArray(CArchive &ar, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArr);
	void SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr);
	double GetDia(CString str);
	CString GetStringDia(double dDia, double dFy=0);
	BOOL IsValidLine() const;

	double GetValueBendReBar(long nDia, int nTypeValue);
	double GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueJoint (double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
	double GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueSettle(double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);

	CString GetStringBridgeOrder(int nBri);
	CString GetStringCrackDesign(long nType);
	CString GetStringConditionApply(long nType);
	CString GetStringBM_FormType(long nPart);
	CString GetStringCircleNum(long nNum);
	CString GetStringBridgeDir(long nBri);
	CString GetStringTypeCoefficientGround(long nType);
	
//	long GetTypeCrackDesign(CString strType);
//	long GetTypeConditionApply(CString strType);

	BOOL IsExistMainRebarStraightHorz(CColumnApp *pColumn, long nDan);
	BOOL IsExistMainRebarStraightVert(CColumnApp *pColumn, long nDan);
	CString GetStringLoadDB(BOOL bDL=FALSE);

	void SeparateRebarListByDescription(CTypedPtrArray <CObArray, CRebar*> *pArrRebar1, CTypedPtrArray <CObArray, CRebar*> *pArrRebar2, CString sDescription);
	// 도로교 설계기준 2010인지를 판단.

	static CString m_sConditionApply[MAX_CONDITION_TYPE];
	static CString m_sEnvCrack[MAX_CRACK_ENV_TYPE];
	static CString m_sBM_FormType[MAX_BM_FORM_TYPE];
	static CString m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL];
	
//	BOOL CheckDataValidity();

	CARcBridgeDataStd();
	BOOL MakeRcBridge(long nTotal);
	void MakeSoilLayer(int nBoring, int nCountLayer);

	virtual ~CARcBridgeDataStd();
	void Serialize(CArchive &ar);
	
	CDocument* m_pDoc;	// doc
	
	CString m_strTempName;	// 도각 초기화를 위한 임시 변수	

	// 시리얼 플래그
	long m_nFlag;

	// 프로젝트 정보
	CString m_strProjectName;		// 사업명
	CString m_strOwnerName;			// 발주처
	CString m_strConstructorName;	// 시공사
	CString m_strSpotName;			// 현장명
	CString m_strEngName;			// 설계사
	CString m_strLineDirName;		// 선형 방향
	CString m_strLineRevDirName;	// 선형 반대 방향

	// 프로그램 진행 종류
	long m_nTypeProcess;	// 진행종류 ( 0 : 일반, 1 : 슬래브 표준도)

	CBasicConcInfo *m_pBasicConcInfo;
	CExpJointRefArr *m_pExpJointRefArr;		// 신축이음제원

	// 출력관련
	BOOL	m_bExcelDefaultSetting;
	CString m_szFontPrint;
	double m_dFontSizePrint;
	double m_dOffsetLeftPrint;
	double m_dOffsetRightPrint;
	double m_dOffsetTopPrint;
	double m_dOffsetBottomPrint;
	long m_nPageRow;
	// 맴버 클래스
	CLineInfoApp* m_pLineApp;		// 선형 정보
	CTypedPtrArray <CObArray, CRcBridgeRebar*> m_pRcBridgeArray;	// 브리지 객체

	// 지반 정보 입력
	CSafetyTypedPtrArray <CObArray, CBoringDataEx*> m_pArrBoringEx;
	std::map<HGINT32, CBoringDataEx*> m_mpBoring;		// EL보정 지반 정보
	// 설계환경 ------------------------------------------------------- 
	long m_nTypeConditionApply;	// 적용설계기준
	long m_nTypeKciCode;			// KCI CODE
	long m_nTypeRoadCode;			// 도로교 설계기준	(0 : 2008, 1 : 2010)
	long m_nTypeRailCode;			// 철도교 설계기준	(0 : 2004, 1 : 2017)
	long m_nDegreeBridge;			// 설계교량의 등급
	long m_nTypeUnit;				// 단위계 설정
	long m_nApplyBoredPileASD;			// 매입말뚝 검토시 허용응력설계법 적용 
	long m_nApplyPhcPileASD;			// PHC말뚝 검토시 허용응력설계법 적용
	long m_nTypeCheckSteelPile;		// 강관말뚝 검토기준 
	BOOL m_bPrintConditionApply;			//하중조합의 출력방법(TRUE: 기본하중조합, FALSE: 세부하중조합)
	long m_nTypeEarthQuakeApply;	// 내진설계기준

	double m_Fck;
	double m_Fy;
	BOOL m_bIsDBar_ForDowelBar;	// 다웰바 강종
	double m_Ec;
	double m_Es;
	double m_Pi_F;
	double m_Pi_C;
	double m_Pi_V;
	double m_Pi_F_Cobel;				// 코벨의 전단력
	double m_RatePoasong;				// 콘크리트의 포아슨비
	double m_PI_B;						// 콘크리트의 지압
	double m_Es_Steel;					// 강재 탄성계수
	double m_NValue_Steel;				// 강재와 콘크리트의 탄성계수비
	double m_Gs_Steel;					// 강재의 전단탄성계수
	double  m_dDeltaTemperature;		// 가동받침의 이동량 산정시 온도변화량
	double  m_dRateDryAndShrinkage;		// 건조수축율
	double  m_dCoefficientExpansion;	// 가동받침의 이동량 산정시 선팽창계수

	long m_nSelectPmin_Hyunta_Extreme;	// 극단상황한계상태Ⅰ의 현장타설말뚝 철근비, AAB-5626

	double m_dLengthRebarMax;		// 철근 겹이음 적용 길이
	long m_nDegreeEarthQuake;		// 내진등급
	long m_nRegionEarthQuake;		// 지진구역
	double m_dCoefficientGround;	// 지반계수
	long m_nTypeCoefficientGround;	// 지반종류
	double m_dRateDAMP_EQ_CD;		// 감쇠비(ξ)
	double m_gValue;				// 중력가속도
	long m_nTypeDesingEQ;			// 내진해석 방법
	long m_nTypeRigidFootingEQ;		// 내진해석시 기초부 모델링 방법
	BOOL m_bApplyRigidWallEQRotate;		// 벽체강성 적용시 회전강성 적용
	long m_nTypeJointPosFootingEQ;		// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
	long m_nTypePropertyPosFootingEQ;	// 내진해석시 기초 중앙으로 모델링하는 경우의 강성
	long m_nTypeUpperWeightEQ;		// 내진해석시 상부중량 설정
	long m_nTypeComboEQ;			// 내진해석결과에 대한 조합 방법
	long m_nTypeSoilPressureEQ;		// 내진해석시 수평토압의 재하
	double m_dRateDAMP_EQ;			// 내진해석시 감쇠비
	long m_nTypeRebarConnect;		// 겹이음 및 정착장 산출방법
	long m_nTypeBMApply;			// 수량산출 적용기준(0:일반수량산출기준, 1:실적수량산출기준(공통수량분리), 2:실적수량산출기준(공통수량포함))
	BOOL m_bSepFootingForSiljuk;	// 실적수량산출시 기초를 바닥및보에서 분리해서 집계
	BOOL m_bRoundRebarConnect_XX;	// 철근 겹이음, 정착장을 반올림
	BOOL m_bIncludeSumZero;			// 수량집계에 빈수량(zero) 포함
	BOOL m_bApplyAllowInc;			// 말뚝응력 검토시 허용응력 증가계수 적용
	long m_nTypeFileNameForExcel;	// 엑셀 출력시 파일명(0 : 파일명_교량명_방향, 1 : 교량명_방향, 2 : 파일명_방향, 3 : 파일명_교량명, 4 : 교량명)
	long m_nTypeCalcSlabEffWidth;	// 철도교 슬래브 유효폭 산정시 방법 // 0 - 슬래브 두께 적용, 1- 슬래브 두께 무시
	long m_nTypeFootingSafetyApplyCoefficientImpact;		// 철도교 안정검토시 활하중 충격 포함
	// ------------------------------------------------------ 설계환경 

	// 선형 설정
	int m_nHowLineInput;			// 선형 입력 방법 0 : 파일, 1 : 직접 입력
	CString m_strLineFilePath;		// 선형 파일 경로
	BOOL m_bMakeDrawingOptionUsed;	// MakeDrawingOption을 한 번만 해주기 위한 변수

	//////////////////////////////////////////////////////////////////////////
	// 철도 하중
	long	m_nDegreeRail;
	long	m_nTypeRailLoad;
	long	m_nTypeRailQty;
	long	m_nRailQtyCar;	// EL하중 적용시의 객차수
	CString m_sNameRailLoad;			// 하중의 이름,LS-25.....
	double  m_DistributionLoadFront;	// 열차앞쪽 분포하중
	double  m_DistributionLoadBack;		// 열차뒤쪽 분포하중
	double  m_RailLoadWheel;			// 동륜하중
	double  m_RailDistWheel;			// 축거
	double  m_RailDistWheelMid;			// 축거 (중간부분)
	CDoubleArray m_dArrTypeLoadTrainWheel;		// 바퀴하중의 종류
	CDoubleArray m_dArrDistTrainWheel;	// 바퀴하중의 거리, 다음하중과의 거리
	CDoubleArray m_dArrLoadTrainWheel;	// 바퀴하중의 크기, 집중하중만....

	// 탱크 및 중차량 하중
	double m_dTank_i;				
	double m_dTank_WeightTank;
	double m_dTank_TankLength;		
	double m_dTank_TankWidth_Side;
	double m_dTank_TankWidth_Inner;
	
	double m_dTank_TrailerL1;		
	double m_dTank_TrailerL2;		
	double m_dTank_TrailerL3;		
	double m_dTank_TrailerL4;
	double m_dTank_TrailerWidth_Side;
	double m_dTank_TrailerWidth_Inner;
	double m_dTank_Ptrailer1;
	double m_dTank_Ptrailer2;
	double m_dTank_Ptrailer3;	

	// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
	long m_nTypePost;			// 동바리 타입(육교용, 일반용) (삭제예정)
	long m_nTypePosBaseLine;	// 동바리 기준선(0 : 지반선, 2 : 기초) (삭제예정)
	double m_dThickPostUpper;	// 육교용 동바리 상부 두께 (삭제예정)
	double m_dThickPostSide;	// 육교용 동바리 좌우측 두께 (삭제예정)
	double m_dThickScaffold;	// 비계 두께 (삭제예정)	
	
	BOOL m_bFootLenInputFloat;	// 기초길이 입력받을때 소수점 입력.

	static CString m_sRailLoadType[MAX_RAIL_LOAD];
	static CString m_sRailLoadType2017[MAX_RAIL_LOAD_2017];

	BOOL IsRailDesign();
	BOOL IsRailDesign2017();
	BOOL IsRailDesignKDS2018();
	BOOL IsKRL_2012();
	BOOL IsDoro2010();
	BOOL IsDesignKDS2019();
	double GetEs_Steel();
	CString GetStringRailLoad(long nLoadType);
//	long GetTypeRailLoad(CString strLoad);
	long GetTypeConcDesign();

	CString GetStringLoadCase(long nLoadCase, BOOL bEarthQuake=FALSE);
	void SetRailLoad();
	BOOL GetRailLoadWheelInfo(long nIdxWheel, long &nType, double &dWeight, double &dDist);
//	void SetRailLoadWheelInfo(long nIdxWheel, long nType, double dWeight, double dDist);
	double GetRailLoadWeightWheel(long nSttWheel, double dLengthSpan);
//	double GetRailLoadDistWheel(long nWheel);
	double GetRailLoadLengthWheel(long nSttWheel);
	long GetCountRailLoadWheel();
	double GetPmaxColumn();

	CString m_szKgf;
	CString m_szTonf;
	CString m_szKgfM;
	CString m_szTonfM;
	CString m_szCM;
	CString m_szCM2;
	CString m_szCM3;
	CString m_szKgf_CM2;
	CString m_szKgf_CM3;
	CString m_szKgf_M2;
	CString m_szKgf_M3;
	CString m_szTonf_M;
	CString m_szTonf_M2;
	CString m_szTonf_M3;
	CString m_szM;
	CString m_szM2;
	CString m_szM3;
	CString m_szM4;

	// COutBridgeResister에도 같은 함수 있음.
	CString GetStringFootTypeAll(CRcBridgeRebar *pBri);
	CString GetStringExcelFileForm();

	void SyncApsDowelRebarSeperateRebarOption(BOOL bSet, long nBri = -1);

private:
	DWORD m_dwTypeARcBridge;	// ARcBridge타입	

	void SetFooting_Qa_Pa__Calculate();

// (LSD:0000)
public:
	std::vector<SStxOptionItem> m_vStxOption;		// STM 내보내기

	long m_nTypeDesignMethod;	// 강도설계법 or 한계상태설계법
	long m_nLSDRoadType;		//LSD :도로의 종류 (0: 일반도로, 1: 고속도로)
	HGDOUBLE m_dDesignRoadWidth; //LSD : 설계차로의 폭(Wp)

	CLsdEnviromentCoef  *m_plsdEnv; //재료표 매니저
	CLsdRebarManager *m_plsdRebar; //정착,이음 매니저		

	void SerializeLsd(CArchive &ar);
	// 강도설계법(기존)인지 한계상태설계법인지 판단
	BOOL IsLsdDesign();
	void SetChangeDesignMethod();
	void SetDataDefaultMatResistFactor();
	void SetDataDefaultBaseResistFactor();
	void SetDataDefaultBaseResistFactor_SteelPile();

	BOOL IsApplyBoredPileASD(CExPileApp *pPile = NULL);
	BOOL IsApplyPhcPileASD(CExPileApp *pPile = NULL);

	CString GetStrDouble(double dValue, CString sUnitCGS, BOOL bLong=TRUE, long nDec=-1, BOOL bDf=FALSE, long nSpace=-1, BOOL bExponent=FALSE);
	long GetDecimal(CString sUnitCGS, long nDec=-1, BOOL bLong=TRUE);
	CString GetStrDoubleUnitChange(double dValue, CString csUnitOrg, CString csUnitChg, BOOL bLong=TRUE, long nDec=-1, BOOL bDf=FALSE, long nSpace=-1, BOOL bExponent=FALSE);

	CUserBmData *m_pUserBmData;
};

#endif // !defined(AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_)
