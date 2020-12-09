// ARcBridgeDataStd.cpp: implementation of the CARcBridgeDataStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CARcBridgeDataStd::m_sRailLoadType2017[MAX_RAIL_LOAD_2017] = 
{			
	"KRL-2012",
	"KRL-2012(여객전용)",
	"EL-18",
	"사용자정의"
};

CString CARcBridgeDataStd::m_sRailLoadType[MAX_RAIL_LOAD] = 
{			
	"LS-25",
	"LS-22",
	"LS-18",
	"EL-25",
	"EL-22",
	"EL-18",
	"HL-25",
	"사용자정의"
};

CString CARcBridgeDataStd::m_sConditionApply[MAX_CONDITION_TYPE] = 
	{			
		"콘크리트 구조설계기준",
		"도로교 설계기준",
		"철도 설계기준"
		//"고속철도 설계기준"
	};
CString CARcBridgeDataStd::m_sEnvCrack[MAX_CRACK_ENV_TYPE] = 
	{			
		"건조 환경",
		"습윤 환경",
		"부식성 환경",
		"고부식성 환경",
		"건조한 공기 또는 보호막",
		"습기, 습한 공기, 흙",
		"제빙 화학제",
		"해수 및 해수살수 ; 건습",
		"물을 담는 구조물(압력이 없는 피트 포함)"

	};
CString CARcBridgeDataStd::m_sBM_FormType[MAX_BM_FORM_TYPE] = 
	{			
		"거친마감",
		"보통마감",
		"매끈한마감",
		"설명된 특징있는 마감 (무늬거푸집)",
		"설명된 특징있는 마감 (강재거푸집)",
		"설명된 특징있는 마감 (코팅거푸집)",
		"설명된 특징있는 마감 (DECK PLATE)",
		"설명된 특징있는 마감 (1회 사용)"
	};

CString CARcBridgeDataStd::m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL] = 
	{			
		"합판6회",
		"합판4회",
		"합판3회",
		"무늬거푸집",
		"강재거푸집",
		"원형거푸집",
		"유로폼"
	};


CARcBridgeDataStd::CARcBridgeDataStd()
{
	m_bOnlyProjectInfoLoad	= FALSE;	// 특별한 옵션.. 프로젝트 정보만 읽어 올지??
	m_strTempName = _T("기본 교량");	// 도각 초기화를 위한 임시 변수

	m_nFlag		= 0;
	// 메모리동적할당 모아 놓는곳.
	// 종류에 상관없이 소멸자에서 메모리해제를 완전히 해주기 위해 한곳에 모아둠.
	m_pLineApp = new CLineInfoApp;
	m_pBasicConcInfo = new CBasicConcInfo(FALSE);
	m_pExpJointRefArr = new CExpJointRefArr;

	// 테스트
	MakeRcBridge(0);

	// 프로젝트 정보
	m_strProjectName	 = _T("");	// 사업명
	m_strOwnerName		 = _T("");	// 발주처
	m_strConstructorName = _T("");	// 시공사
	m_strSpotName		 = _T("");	// 현장명
	m_strEngName		 = _T("");	// 설계사
	m_strLineDirName	 = _T("");	// 선형 방향
	m_strLineRevDirName	 = _T("");	// 선형 반대 방향
	m_nTypeProcess	= TYPE_PROCESS_NORMAL;	// 진행종류 ( 0 : 일반, 1 : 슬래브 표준도)

	// 설계환경 ------------------------------------------------------- 
	m_nTypeConditionApply		= -1;
	m_nTypeKciCode			= DESIGN_CODE_KCI_2003;
	m_nTypeRoadCode			= DESIGN_ROAD_2008;
	m_nTypeRailCode			= DESIGN_RAIL_2004;
	m_nDegreeBridge			= 0;
	m_nTypeUnit				= UNIT_TYPE_SI;		// 단위계
	m_RatePoasong				= 0.16667;			// 콘크리트의 포아슨비
	m_Fck						= 270;
	m_Fy						= 4000;
	m_bIsDBar_ForDowelBar		= TRUE;	// 다웰바 강종
	m_Ec						= GetValueEcConc(m_Fck); // 도로교설계기준 38페이지
	m_Es						= 2000000;
	m_Es_Steel					= GetEs_Steel();
	m_Pi_F						= 0.85;
	m_Pi_C						= 0.70;
	m_Pi_V						= 0.80;
	m_Pi_F_Cobel				= 0.80;
	m_PI_B						= 0;				// 콘크리트의 지압
	m_dDeltaTemperature			= 20;				// 가동받침의 이동량 산정시 온도변화량
	m_dCoefficientExpansion		= 0.000010;			// 선팽창계수
	m_dRateDryAndShrinkage		= 0.00015;			// 건조수축율
	m_dLengthRebarMax			= 8000;				// 철근 겹이음 적용 길이
	m_nDegreeEarthQuake			= 0;				// 내진등급
	m_nRegionEarthQuake			= 0;				// 지진구역
	m_dCoefficientGround		= 1.0;				// 지반계수
	m_nTypeCoefficientGround	= 1;
	m_dRateDAMP_EQ_CD			= 5;
	m_gValue					= 9.810;			// 중력가속도
	m_nTypeDesingEQ			= 1;				// 내진해석 방법
	m_nTypeRigidFootingEQ		= 0;				// 내진해석시 기초부 모델링 방법
	m_bApplyRigidWallEQRotate	= FALSE;
	m_nTypeJointPosFootingEQ	= 0;				// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
	m_nTypePropertyPosFootingEQ = 0;
	m_nTypeUpperWeightEQ		= 0;				// 내진해석시 상부중량 설정
	m_nTypeComboEQ				= 0;				// 내진해석결과에 대한 조합 방법
	m_nTypeSoilPressureEQ		= 0;
	m_dRateDAMP_EQ				= 0.05;				// 내진해석시 감쇠비
	m_nTypeRebarConnect			= 1;				// 겹이음 및 정착장 산출방법
	m_nTypeBMApply				= 0;				// 수량산출 적용기준
	m_bSepFootingForSiljuk		= FALSE;			// 실적수량산출시 기초를 바닥및보에서 분리해서 집계
	m_bRoundRebarConnect_XX		= TRUE;				// 철근 겹이음, 정착장을 반올림
	m_bIncludeSumZero			= TRUE;				// 수량집계에 빈수량(zero) 포함
	m_bApplyAllowInc			= TRUE;				// 말뚝응력 검토시 허용응력 증가계수 적용
	m_nTypeFileNameForExcel		= 0;	// 엑셀 출력시 파일명(0 : 파일명_교량명_방향, 1 : 교량명_방향, 2 : 파일명_방향, 3 : 파일명_교량명, 4 : 교량명)
	m_nTypeCalcSlabEffWidth		= 0;
	m_nTypeFootingSafetyApplyCoefficientImpact = 0;
	m_nApplyBoredPileASD		= 0;
	m_nApplyPhcPileASD			= 0;
	m_nTypeCheckSteelPile		= 0;
	m_nSelectPmin_Hyunta_Extreme = 0;
	m_bPrintConditionApply = TRUE;	//하중조합의 출력방법(TRUE: 기본하중조합, FALSE: 세부하중조합)
	m_nTypeEarthQuakeApply = 0;
	// ------------------------------------------------------ 설계환경 

	// 선형 설정
	m_nHowLineInput = 1;			// 선형 입력 방법 0 : 파일, 1 : 직접 입력
	m_strLineFilePath = _T("");		// 선형 파일 경로

	m_bMakeDrawingOptionUsed = FALSE;

	// 철도 하중
	m_nDegreeRail	 = 0;
	m_nTypeRailLoad	 = 1;
	m_nTypeRailQty	 = 0;
	m_nRailQtyCar	 = 6;

	m_sNameRailLoad			= _T("");	 // 하중의 이름,LS-25.....
	m_DistributionLoadFront	= 0;		 // 열차앞쪽 분포하중
	m_DistributionLoadBack	= 0;		 // 열차뒤쪽 분포하중
	m_RailLoadWheel			= 0;		 // 동륜하중
	m_RailDistWheel			= 2.10;		 // 축거
	m_RailDistWheelMid		= 0;		 // 축거 (중간부분)
	m_dArrTypeLoadTrainWheel.RemoveAll();// 바퀴하중의 종류
	m_dArrDistTrainWheel.RemoveAll();	 // 바퀴하중의 거리, 다음하중과의 거리
	m_dArrLoadTrainWheel.RemoveAll();	 // 바퀴하중의 크기, 집중하중만....

	// 탱크 및 중차량 하중
	m_dTank_i				= 0.15;				
	m_dTank_WeightTank		= 54.42;
	m_dTank_TankLength		= 4.500;		
	m_dTank_TankWidth_Side	= 0.700;
	m_dTank_TankWidth_Inner	= 1.900;
	
	m_dTank_TrailerL1		= 3.6;		
	m_dTank_TrailerL2		= 1.5;		
	m_dTank_TrailerL3		= 4.5;		
	m_dTank_TrailerL4		= 1.2;
	m_dTank_TrailerWidth_Side	= 0.975;
	m_dTank_TrailerWidth_Inner	= 1.250;
	m_dTank_Ptrailer1			= 4.0815;
	m_dTank_Ptrailer2			= 8.163;
	m_dTank_Ptrailer3			= 5.8955;	

	// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
	m_nTypePost				= 0;	// 동바리 타입(육교용, 일반용)
	m_nTypePosBaseLine		= 0;	// 동바리 기준선(0 : 지반선, 2 : 기초)
	m_dThickPostUpper		= 0;	// 육교용 동바리 상부 두께
	m_dThickPostSide		= 0;	// 육교용 동바리 좌우측 두께
	m_dThickScaffold		= 0;	// 비계 두께
	m_bFootLenInputFloat	= FALSE;

	// 구조계산서 출력 옵션
	m_bExcelDefaultSetting  = TRUE;
	m_szFontPrint		    = "굴림체";
	m_dFontSizePrint	    = 9;
	m_dOffsetLeftPrint		= 1.9;
	m_dOffsetRightPrint		= 1.9;
	m_dOffsetTopPrint		= 2.5;
	m_dOffsetBottomPrint	= 2.5;
	m_nPageRow				= 46;

	//m_dwEnableFunction	= 0x0000;	// 0x0001 : 슬래브 표준도 기능

	// (LSD:0000)
	m_nTypeDesignMethod = DESIGN_METHOD_STRENGTH;
	m_nLSDRoadType		= 0;
	m_dDesignRoadWidth	= 3600;

	m_plsdEnv  = new CLsdEnviromentCoef;
	m_plsdRebar = new CLsdRebarManager;
	m_pBasicConcInfo->SetLsdRebar(m_plsdRebar);

	m_pUserBmData = new CUserBmData;

	m_vStxOption.clear();
}

CARcBridgeDataStd::~CARcBridgeDataStd()
{
	AhTPADelete(&m_pRcBridgeArray, (CRcBridgeRebar*)0);
	AhTPADelete(&m_pArrBoringEx, (CBoringDataEx*)0);

	// (LSD:0000)
	SAFE_DELETE(m_pBasicConcInfo);
	SAFE_DELETE(m_pLineApp);
	SAFE_DELETE(m_pExpJointRefArr);
	SAFE_DELETE(m_plsdEnv);
	SAFE_DELETE(m_plsdRebar);
	SAFE_DELETE(m_pUserBmData);

	ClearMapPtr(m_mpBoring);
}

// 데이타가 유효한지 판단함
// 가끔씩 어이가 없는 값들이 입력되는걸 대비하기 위함.
// BOOL CARcBridgeDataStd::CheckDataValidity()
// {
// 	return TRUE;
// }

// long CARcBridgeDataStd::GetTypeConditionApply(CString strType)
// {
// 	long i=0; for(i=0; i<MAX_CONDITION_TYPE; i++)
// 		if(strType==m_sConditionApply[i])
// 			return i;
// 
// 	return -1;
// }

CString CARcBridgeDataStd::GetStringConditionApply(long nType)
{
	if(nType>=0 && nType<MAX_CONDITION_TYPE)
		return m_sConditionApply[nType];
	return _T("");
}

// long CARcBridgeDataStd::GetTypeCrackDesign(CString strType)
// {
// 	long i=0; for(i=0; i<MAX_CRACK_ENV_TYPE; i++)
// 		if(strType==m_sEnvCrack[i])
// 			return i;
// 
// 	return -1;
// }

CString CARcBridgeDataStd::GetStringCrackDesign(long nType)
{
	if(nType>=0 && nType<MAX_CRACK_ENV_TYPE)
		return m_sEnvCrack[nType];
	return _T("");

}

double CARcBridgeDataStd::GetValueSettle(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	m_pBasicConcInfo->m_nTypeRebarConnect	= m_nTypeRebarConnect;
	return m_pBasicConcInfo->GetValueSettle(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

double CARcBridgeDataStd::GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	return m_pBasicConcInfo->GetValueSettleByCalc(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

double CARcBridgeDataStd::GetValueJoint(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	m_pBasicConcInfo->m_nTypeRebarConnect	= m_nTypeRebarConnect;
	return m_pBasicConcInfo->GetValueJoint(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

// nType : 0 - 일반 - A급, 1 - 일반 B급, 2 - 상부 A급, 3 - 상부 B급, 4 - 일반, 5 - 폐합 띠철근, 6 - 나선철근
double CARcBridgeDataStd::GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	return m_pBasicConcInfo->GetValueJointByCalc(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

//#define JOINT_BEBD_RADIUS		0
//#define JOINT_BEBD_CHAMFER	1
double CARcBridgeDataStd::GetValueBendReBar(long nDia, int nTypeValue)
{
	if(nTypeValue==JOINT_BEBD_RADIUS)
		return m_pBasicConcInfo->GetRebarInfo(nDia, BASIC_RADIUS);
	if(nTypeValue==JOINT_BEBD_CHAMFER)
		return m_pBasicConcInfo->GetRebarInfo(nDia, BASIC_CHAMFER);

	return 0;
}

BOOL CARcBridgeDataStd::IsValidLine() const
{ 
// 	BOOL bValid = FALSE;

	BOOL bValid = m_pLineApp->IsValidLineInuptData();

	return bValid; 
}

CLineInfoApp* CARcBridgeDataStd::GetLineApp()
{
	return m_pLineApp;
}

void CARcBridgeDataStd::Serialize(CArchive &ar)
{
	// 플래그
	// 13 : m_bIsDBar_ForDowelBar;	// 다웰바 강종
	// 14 : m_nTypeProcess;	// 진행종류 ( 0 : 일반, 1 : 슬래브 표준도)
//	m_nFlag = 14;	// 0번은 횡단위치입력창에서 자동 권고안 되는 현상 방지를 위해서 사용함.
	// 4번은 이음 반올림 옵션 인덱스 수정.
	// 15 : m_bApplyRigidWallEQRotate;
	// 16 : m_nPageRow;
	// 17 : m_bSepFootingForSiljuk;		// 실적수량산출시 기초를 바닥및보에서 분리해서 집계
	// 18 : m_Pi_F_Cobel
	// 19 : m_pExpJointRefArr ( BOX_TYPE );
	// 19 :m_nTypeRoadCode, m_nTypeRailCode ( BOX_TYPE );
	// 20 : m_pBasicConcInfo 절곡값 기본값 변경
	// 21 : SerializeLsd() 추가
	// 22 : m_pUserBmData 사용자 수량 저장
	// 23 : m_bApplyBoredPileASD
	// 24 : m_nApplyPhcPileASD
	// 25 : m_nTypeCheckSteelPile
	// 26 : ARCBRIDGE-2826
	// 27 : m_nSelectPmin_Hyunta_Extreme
	// 28 : ARCBRIDGE-2942
	// 29 : ARCBRIDGE-2969
	// 30 : m_bPrintConditionApply
	// 31 : m_nTypeFootingSafetyApplyCoefficientImpact
	// 32 : m_nTypeEarthQuakeApply, m_nTypeCoefficientGround
	// 33 : m_dRateDAMP_EQ_CD

	m_nFlag = 33;

	// 공통으로 사용하는 변수들
	long i(0);
	long nRcBridgeSize = m_pRcBridgeArray.GetSize();

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		// 프로젝트 정보
		ar << m_strProjectName;		// 사업명
		ar << m_strOwnerName;		// 발주처
		ar << m_strConstructorName;	// 시공사
		ar << m_strSpotName;		// 현장명
		ar << m_strEngName;			// 설계사
		ar << m_strLineDirName;		// 선형 방향
		ar << m_strLineRevDirName;	// 선형 반대 방향
		ar << m_nTypeProcess;	// 진행종류 ( 0 : 일반, 1 : 슬래브 표준도)

		m_pBasicConcInfo->Serialize(ar);
		m_pExpJointRefArr->Serialize(ar);

		// 설계환경 ------------------------------------------------------- 
		ar << m_nTypeConditionApply;	// 적용설계기준
		ar << m_nTypeKciCode;
		ar << m_nTypeRoadCode;
		ar << m_nTypeRailCode;
		ar << m_nDegreeBridge;			// 설계교량의 등급
		ar << m_nTypeUnit;				// 단위계
		ar << m_RatePoasong;			// 콘크리트의 포아슨비
		ar << m_Fck;
		ar << m_Fy;
		ar << m_bIsDBar_ForDowelBar;	// 다웰바 강종
		ar << m_Ec;
		ar << m_Es;
		ar << m_Pi_F;
		ar << m_Pi_C;
		ar << m_Pi_V;
		ar << m_PI_B;					// 콘크리트의 지압
		ar << m_Pi_F_Cobel;
		ar << m_Es_Steel;				// 강재 탄성계수
		ar << m_NValue_Steel;			// 강재와 콘크리트의 탄성계수비
		ar << m_Gs_Steel;				// 강재의 전단탄성계수
		ar << m_dDeltaTemperature;
		ar << m_dCoefficientExpansion;
		ar << m_dRateDryAndShrinkage;
		ar << m_dLengthRebarMax;		// 철근 겹이음 적용 길이
		ar << m_nDegreeEarthQuake;		// 내진등급
		ar << m_nRegionEarthQuake;		// 지진구역
		ar << m_dCoefficientGround;		// 지반계수
		ar << m_nTypeCoefficientGround;
		ar << m_dRateDAMP_EQ_CD;
		ar << m_gValue;					// 중력가속도
		ar << m_nTypeDesingEQ;			// 내진해석 방법
		ar << m_nTypeRigidFootingEQ;		// 내진해석시 기초부 모델링 방법
		ar << m_bApplyRigidWallEQRotate;
		ar << m_nTypeJointPosFootingEQ;				// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
		ar << m_nTypePropertyPosFootingEQ;
		ar << m_nTypeUpperWeightEQ;				// 내진해석시 상부중량 설정
		ar << m_nTypeComboEQ;			// 내진해석결과에 대한 조합 방법
		ar << m_nTypeSoilPressureEQ;
		ar << m_dRateDAMP_EQ;			// 내진해석시 감쇠비
		ar << m_nTypeRebarConnect;		// 겹이음 및 정착장 산출방법
		ar << m_nTypeBMApply;			// 수량산출 적용기준
		ar << m_bRoundRebarConnect_XX;	// 철근 겹이음, 정착장을 반올림
		ar << m_bIncludeSumZero;		// 수량집계에 빈수량(zero) 포함
		ar << m_bApplyAllowInc;			// 말뚝응력 검토시 허용응력 증가계수 적용
		ar << m_nTypeFileNameForExcel;	// 엑셀 출력시 파일명(0 : 파일명_교량명_방향, 1 : 교량명_방향, 2 : 파일명_방향, 3 : 파일명_교량명, 4 : 교량명)
		ar << m_nTypeCalcSlabEffWidth;
		ar << m_nTypeFootingSafetyApplyCoefficientImpact;
		ar << m_nApplyBoredPileASD;
		ar << m_nApplyPhcPileASD;
		ar << m_nTypeCheckSteelPile;
		ar << m_nSelectPmin_Hyunta_Extreme;
		ar << m_bPrintConditionApply;
		ar << m_nTypeEarthQuakeApply;
		// ------------------------------------------------------ 설계환경 

		// 철도 하중
		ar << m_nDegreeRail;
		ar << m_nTypeRailLoad;
		ar << m_nTypeRailQty;
		ar << m_nRailQtyCar;
		ar << m_sNameRailLoad;			// 하중의 이름,LS-25.....
		ar << m_DistributionLoadFront;	// 열차앞쪽 분포하중
		ar << m_DistributionLoadBack;		// 열차뒤쪽 분포하중
		ar << m_RailLoadWheel;			// 동륜하중
		ar << m_RailDistWheel;			// 축거
		ar << m_RailDistWheelMid;			// 축거 (중간부분)
		m_dArrTypeLoadTrainWheel.Serialize(ar);		// 바퀴하중의 종류
		m_dArrDistTrainWheel.Serialize(ar);	// 바퀴하중의 거리, 다음하중과의 거리
		m_dArrLoadTrainWheel.Serialize(ar);	// 바퀴하중의 크기, 집중하중만....
		// 탱크 및 중차량 하중
		ar << m_dTank_i;				
		ar << m_dTank_WeightTank;
		ar << m_dTank_TankLength;		
		ar << m_dTank_TankWidth_Side;
		ar << m_dTank_TankWidth_Inner;
		
		ar << m_dTank_TrailerL1;		
		ar << m_dTank_TrailerL2;		
		ar << m_dTank_TrailerL3;		
		ar << m_dTank_TrailerL4;
		ar << m_dTank_TrailerWidth_Side;
		ar << m_dTank_TrailerWidth_Inner;
		ar << m_dTank_Ptrailer1;
		ar << m_dTank_Ptrailer2;
		ar << m_dTank_Ptrailer3;	

		// 수량관련 옵션
		ar << m_nTypePost;			// 동바리 타입(육교용, 일반용)
		ar << m_nTypePosBaseLine;	// 동바리 기준선(0 : 지반선, 2 : 기초)
		ar << m_dThickPostUpper;	// 육교용 동바리 상부 두께
		ar << m_dThickPostSide;		// 육교용 동바리 좌우측 두께
		ar << m_dThickScaffold;		// 비계 두께
		ar << m_bFootLenInputFloat;

		// 선형 설정
		ar << m_nHowLineInput;			// 선형 입력 방법 0 : 파일, 1 : 직접 입력
		ar << m_strLineFilePath;		// 선형 파일 경로
		m_pLineApp->Serialize(ar);

		// 각 교량에 대해 저장
		ar << nRcBridgeSize;
		for(i = 0; i < nRcBridgeSize; i++)
			m_pRcBridgeArray[i]->Serialize(ar);
		AhTPASerial(ar,&m_pArrBoringEx, (CBoringDataEx*)0);

		ar << m_bExcelDefaultSetting;
		ar << m_szFontPrint;
		ar << m_dFontSizePrint;
		ar << m_dOffsetLeftPrint;
		ar << m_dOffsetRightPrint;
		ar << m_dOffsetTopPrint;
		ar << m_dOffsetBottomPrint;
		ar << m_nPageRow;

		ar << m_bSepFootingForSiljuk;		// 실적수량산출시 기초를 바닥및보에서 분리해서 집계
	}
	else
	{
		ar >> m_nFlag;

		// 프로젝트 정보
		ar >> m_strProjectName;		// 사업명
		ar >> m_strOwnerName;		// 발주처
		ar >> m_strConstructorName;	// 시공사
		ar >> m_strSpotName;		// 현장명
		ar >> m_strEngName;			// 설계사
		ar >> m_strLineDirName;		// 선형 방향
		ar >> m_strLineRevDirName;	// 선형 반대 방향
		if(m_nFlag > 13)
			ar >> m_nTypeProcess;	// 진행종류 ( 0 : 일반, 1 : 슬래브 표준도)

		if(m_bOnlyProjectInfoLoad) return;

		m_pBasicConcInfo->Serialize(ar);
		if(m_nFlag < 20)
		{
			for(long nRow = 0; nRow < 12; nRow++)
			{
				m_pBasicConcInfo->m_RebarWeight[nRow][5] = m_pBasicConcInfo->m_RebarWeight[nRow][4];
			}
		}
		if(m_nFlag > 18)
		{
			m_pExpJointRefArr->Serialize(ar);
		}
		if(m_nFlag < 4)
		{
			switch(m_pBasicConcInfo->m_nRoundRebarConnect)
			{
			case 0: m_pBasicConcInfo->m_nRoundRebarConnect	= 2; break;
			case 1: m_pBasicConcInfo->m_nRoundRebarConnect	= 1; break;
			case 2: m_pBasicConcInfo->m_nRoundRebarConnect	= 0; break;
			}
		}

		// 설계환경 ------------------------------------------------------- 
		ar >> m_nTypeConditionApply;	// 적용설계기준
		if(m_nFlag>11)
		{
			ar >> m_nTypeKciCode;
		}
		if(m_nFlag > 18)
		{
			ar >> m_nTypeRoadCode;
			ar >> m_nTypeRailCode;
		}
		ar >> m_nDegreeBridge;			// 설계교량의 등급
		ar >> m_nTypeUnit;				// 단위계
		ar >> m_RatePoasong;			// 콘크리트의 포아슨비
		ar >> m_Fck;
		ar >> m_Fy;
		if(m_nFlag > 12)
			ar >> m_bIsDBar_ForDowelBar;	// 다웰바 강종
		ar >> m_Ec;
		ar >> m_Es;
		ar >> m_Pi_F;
		ar >> m_Pi_C;
		ar >> m_Pi_V;
		ar >> m_PI_B;	
		if(m_nFlag > 17)
			ar >> m_Pi_F_Cobel;
		ar >> m_Es_Steel;				// 강재 탄성계수
		if(m_Es_Steel == 0)
			m_Es_Steel = 2100000;
		ar >> m_NValue_Steel;			// 강재와 콘크리트의 탄성계수비
		ar >> m_Gs_Steel;				// 강재의 전단탄성계수
		ar >> m_dDeltaTemperature;
		ar >> m_dCoefficientExpansion;
		ar >> m_dRateDryAndShrinkage;
		ar >> m_dLengthRebarMax;		// 철근 겹이음 적용 길이
		ar >> m_nDegreeEarthQuake;		// 내진등급
		ar >> m_nRegionEarthQuake;		// 지진구역
		ar >> m_dCoefficientGround;		// 지반계수
		if(m_nFlag > 31)
			ar >> m_nTypeCoefficientGround;
		if(m_nFlag > 32)
			ar >> m_dRateDAMP_EQ_CD;
		ar >> m_gValue;					// 중력가속도
		ar >> m_nTypeDesingEQ;			// 내진해석 방법
		if(m_nFlag < 19)
		{
			m_nTypeDesingEQ = 1;
		}
		ar >> m_nTypeRigidFootingEQ;	// 내진해석시 기초부 모델링 방법
		if(m_nFlag > 14)
			ar >> m_bApplyRigidWallEQRotate;
		if(m_nFlag>1)
		{
			ar >> m_nTypeJointPosFootingEQ;	// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
			if(m_nFlag>8)
				ar >> m_nTypePropertyPosFootingEQ;
			ar >> m_nTypeUpperWeightEQ;		// 내진해석시 상부중량 설정
		}
		ar >> m_nTypeComboEQ;			// 내진해석결과에 대한 조합 방법
		if(m_nFlag>9)
			ar >> m_nTypeSoilPressureEQ;
		ar >> m_dRateDAMP_EQ;			// 내진해석시 감쇠비
		ar >> m_nTypeRebarConnect;		// 겹이음 및 정착장 산출방법
		ar >> m_nTypeBMApply;			// 수량산출 적용기준
		ar >> m_bRoundRebarConnect_XX;	// 철근 겹이음, 정착장을 반올림
		ar >> m_bIncludeSumZero;		// 수량집계에 빈수량(zero) 포함
		if(m_nFlag>5)
		{
			ar >> m_bApplyAllowInc;			// 말뚝응력 검토시 허용응력 증가계수 적용
		}
		if(m_nFlag > 7)
			ar >> m_nTypeFileNameForExcel;	// 엑셀 출력시 파일명(0 : 파일명_교량명_방향, 1 : 교량명_방향, 2 : 파일명_방향, 3 : 파일명_교량명, 4 : 교량명)
		if(m_nFlag>10)
			ar >> m_nTypeCalcSlabEffWidth;
		if(m_nFlag > 30)
			ar >> m_nTypeFootingSafetyApplyCoefficientImpact;
		if(m_nFlag > 22)
			ar >> m_nApplyBoredPileASD;
		if(m_nFlag > 23)
			ar >> m_nApplyPhcPileASD;
		if(m_nFlag > 24)
			ar >> m_nTypeCheckSteelPile;
		if(m_nFlag > 26)
			ar >> m_nSelectPmin_Hyunta_Extreme;
		if(m_nFlag > 29)
			ar >> m_bPrintConditionApply;
		else
			m_bPrintConditionApply = FALSE;

		if(m_nFlag > 31)
			ar >> m_nTypeEarthQuakeApply;

		// ------------------------------------------------------ 설계환경 

		// 철도 하중
		ar >> m_nDegreeRail;
		ar >> m_nTypeRailLoad;
		ar >> m_nTypeRailQty;
		ar >> m_nRailQtyCar;
		ar >> m_sNameRailLoad;				// 하중의 이름,LS-25.....
		ar >> m_DistributionLoadFront;		// 열차앞쪽 분포하중
		ar >> m_DistributionLoadBack;		// 열차뒤쪽 분포하중
		ar >> m_RailLoadWheel;				// 동륜하중
		ar >> m_RailDistWheel;				// 축거
		ar >> m_RailDistWheelMid;			// 축거 (중간부분)
		m_dArrTypeLoadTrainWheel.Serialize(ar);		// 바퀴하중의 종류
		m_dArrDistTrainWheel.Serialize(ar);	// 바퀴하중의 거리, 다음하중과의 거리
		m_dArrLoadTrainWheel.Serialize(ar);	// 바퀴하중의 크기, 집중하중만....

		if(m_nFlag > 4)
		{
			// 탱크 및 중차량 하중
			ar >> m_dTank_i;				
			ar >> m_dTank_WeightTank;
			ar >> m_dTank_TankLength;		
			ar >> m_dTank_TankWidth_Side;
			ar >> m_dTank_TankWidth_Inner;
			
			ar >> m_dTank_TrailerL1;		
			ar >> m_dTank_TrailerL2;		
			ar >> m_dTank_TrailerL3;		
			ar >> m_dTank_TrailerL4;
			ar >> m_dTank_TrailerWidth_Side;
			ar >> m_dTank_TrailerWidth_Inner;
			ar >> m_dTank_Ptrailer1;
			ar >> m_dTank_Ptrailer2;
			ar >> m_dTank_Ptrailer3;	
		}
		else
		{
			// 탱크 및 중차량 하중
			m_dTank_i				= 0.15;				
			m_dTank_WeightTank		= 58.0;
			m_dTank_TankLength		= 4.5;
			m_dTank_TankWidth_Side	= 0.7;
			m_dTank_TankWidth_Inner	= 1.9;
			
			m_dTank_TrailerL1		= 3.6;
			m_dTank_TrailerL2		= 1.5;		
			m_dTank_TrailerL3		= 4.5;		
			m_dTank_TrailerL4		= 1.2;
			m_dTank_TrailerWidth_Side	= 0.975;
			m_dTank_TrailerWidth_Inner	= 1.25;
			m_dTank_Ptrailer1		= 4.5;
			m_dTank_Ptrailer2		= 9.0;
			m_dTank_Ptrailer3		= 6.5;	
		}

		// 수량관련 옵션
		ar >> m_nTypePost;			// 동바리 타입(육교용, 일반용)
		ar >> m_nTypePosBaseLine;	// 동바리 기준선(0 : 지반선, 2 : 기초)
		ar >> m_dThickPostUpper;	// 육교용 동바리 상부 두께
		ar >> m_dThickPostSide;		// 육교용 동바리 좌우측 두께
		ar >> m_dThickScaffold;		// 비계 두께
		if(m_nFlag > 6) ar >> m_bFootLenInputFloat;

		// 선형 설정
		ar >> m_nHowLineInput;			// 선형 입력 방법 0 : 파일, 1 : 직접 입력
		ar >> m_strLineFilePath;		// 선형 파일 경로
		m_pLineApp->Serialize(ar);
		CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
		m_pLineApp->SetDataIp(strErrorMsg);

		// 각 교량 불러오기
		ar >> nRcBridgeSize;
		MakeRcBridge(nRcBridgeSize);
		for(i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeRebar *pBri	= m_pRcBridgeArray[i];
			pBri->Serialize(ar);
		}
		AhTPASerial(ar,&m_pArrBoringEx, (CBoringDataEx*)0);
		ar >> m_bExcelDefaultSetting;
		ar >> m_szFontPrint;
		ar >> m_dFontSizePrint;
		ar >> m_dOffsetLeftPrint;
		ar >> m_dOffsetRightPrint;
		ar >> m_dOffsetTopPrint;
		ar >> m_dOffsetBottomPrint;
		if(m_nFlag > 15)
			ar >> m_nPageRow;
		
		if(m_nFlag > 16)
			ar >> m_bSepFootingForSiljuk;		// 실적수량산출시 기초를 바닥및보에서 분리해서 집계
	}

	if(ar.IsStoring() || m_nFlag > 20)
		SerializeLsd(ar);

	if(ar.IsStoring() || m_nFlag > 21)
		m_pUserBmData->Serialize(ar);

	if(ar.IsLoading())
	{
		SetStringUnitType();

		// 교량을 Sync시켜주는 부분을 Serialize가 모두 끝나는 맨 뒤로 이동했다.
		nRcBridgeSize = m_pRcBridgeArray.GetSize();
		for(i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeRebar *pBri	= m_pRcBridgeArray[i];

			if(pBri->m_nIdxBridge==-1)
			{
				pBri->m_nIdxBridge = i;
			}
			pBri->m_bIsIncludeOnlyLine	= pBri->IsIncludeOnlyLineInBridge();

			BOOL bVertDir	= pBri->IsVertDir();
			long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
			pBri->m_bSettingVertSection		= FALSE;
			pBri->m_nTypeJongdanForRebar	= 2;
			pBri->SyncBridge(0, !bVertDir);						// 교량을 읽어 온 뒤에는 각 구체별 동기화를 한번 맞춰준다.
			pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;

			if(pBri->m_bNeedSettingVertSection || !pBri->m_bSettingVertSection || pBri->m_nFlag < 44)
			{
				pBri->SettingVertSection();							// 속도개선을 위한 작업
				pBri->m_bNeedSettingVertSection	= FALSE;
			}
			if(pBri->m_bNeedSettingPlane || !pBri->m_bSettingPlane || pBri->m_nFlag < 44)
			{
				pBri->SettingPlane();
				pBri->m_bNeedSettingPlane	= FALSE;
			}
			pBri->SyncBridgeRebar(FALSE, 0, TRUE);	// 배근관련된 데이타도 동기화 해줌.

			if(ar.IsLoading() && pBri->m_nFlag < 12)
			{
				pBri->SetDataDefaultRebarShear_VertSection_Bracket();
			}

			if(pBri->m_bAutoWingHeightSoil)
			{
				pBri->SetDataDefaultWingWallHeightSoil();
			}

			// 자동 권고안 방지용.
			if(m_nFlag < 1)
			{
				pBri->SetDefaultedState(7092, 0x00000001);	// 횡단위치 입력 부임.(7092)
			}
		}
	}

	if(m_nFlag < 26 && ar.IsLoading() && IsLsdDesign())
	{
		SetFooting_Qa_Pa__Calculate();
	}

	if(m_nFlag < 28)
	{
		nRcBridgeSize = m_pRcBridgeArray.GetSize();
		for(long i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeApp *pBri = GetBridge(i);

			for(long nJ=0; nJ<pBri->m_nQtyJigan+1; nJ++)
			{
				if(pBri->IsPileBase(nJ))
				{
					CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJ);
					CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
					if(pPile->m_nType == PILE_TYPE_HYUNJANG)
					{
						CBoringDataEx *pBoringEx = pBri->GetBoringDataEx(nJ);
						if(pBoringEx)
						{
							double dElTop = pFooting->m_dEL - pFooting->GetHeight();
							CBoringData *pBoring = pBoringEx->GetBoringByElevation(toM(dElTop));

							pFooting->SetHyunTaPileByBoring(pBoring);
						}
					}
				}
			}
		}
	}

	if(m_nFlag < 29 && ar.IsLoading() && IsLsdDesign())
	{
		long nSizeBorEx = m_pArrBoringEx.GetSize();
		for(long n=0; n<nSizeBorEx; n++)
		{
			CBoringDataEx *pBorEx = m_pArrBoringEx.GetAt(n);
			long nSizeBor = pBorEx->m_pArrBoring.GetSize();
			for(long nB=0; nB<nSizeBor; nB++)
			{
				CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(nB);
				if(pBor->GetBoringType() > TOSA)
				{
					pBor->m_nTypeSoil = 2; // 암반
				}
			}
		}
	}
}

CString CARcBridgeDataStd::GetStringDia(double dDia, double dFy)
{
	if(dFy == 0)
		dFy = m_Fy;

	CString sFy = GetCharRebarMark(dFy);
	if(dDia==0) return _T("");
	
	return sFy+Comma(dDia);
}

// 그리드 데이타에서 지름을 읽어옴.
double CARcBridgeDataStd::GetDia(CString str)
{
	str.Replace("D", "");
	str.Replace("H", "");
	str.Replace("S", "");
	str.Replace("U", "");
	str.Replace("d", "");
	str.Replace("h", "");
	str.Replace("s", "");
	str.Replace("u", "");

	return atof(str);
}

// bBridgeCount = TRUE -> 객체수가 아닌 교량의 개수를 리턴
long CARcBridgeDataStd::GetBridgeSize(BOOL bBridgeCount)
{
	if(!bBridgeCount)
		return m_pRcBridgeArray.GetSize();

	long nQtyBridge = 0;
	long nQtyInput = m_pRcBridgeArray.GetSize();
	long n=0; for(n=0 ;n<nQtyInput; n++)
	{
		CRcBridgeApp *pBri = m_pRcBridgeArray.GetAt(n);
		if(pBri->m_nSeparBri!=2) nQtyBridge++;
	}

	return nQtyBridge;
}

CRcBridgeApp* CARcBridgeDataStd::GetBridge(long nBridge)
{
	if(nBridge < 0 || nBridge >= m_pRcBridgeArray.GetSize()) 
		return NULL;

	return dynamic_cast<CRcBridgeApp *>(m_pRcBridgeArray[nBridge]);
}

CRcBridgeRebar* CARcBridgeDataStd::GetBridgeRebar(long nBridge)
{
	if(nBridge < 0 || nBridge >= m_pRcBridgeArray.GetSize()) 
		return NULL;

	return m_pRcBridgeArray[nBridge];
}

BOOL CARcBridgeDataStd::MakeRcBridge(long nTotal)
{
	long nCount	= m_pRcBridgeArray.GetSize();
	AhTPAMake(nTotal, &m_pRcBridgeArray, (CRcBridgeRebar*)0);
	long i = 0; for(i = 0; i < m_pRcBridgeArray.GetSize(); i++)
	{
		CRcBridgeRebar *pBri = m_pRcBridgeArray.GetAt(i);
		pBri->m_pARcBridgeDataStd = this;
		pBri->m_Util.m_pARcBridgeDataStd	= this;
		if(i >= nCount)
			pBri->SetDataDefaultBridgeType();

		if(!pBri->m_bUnderGroundRahmen)
		{
			m_pLineApp->ReSetSearchIp();
			*pBri->m_pLineApp = *m_pLineApp;
			CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
			pBri->m_pLineApp->SetDataIp(strErrorMsg);
		}
	}

	return TRUE;
}

void CARcBridgeDataStd::MakeSoilLayer(int nBoring, int nCountLayer)
{
	if(nBoring<0) return;

	CBoringDataEx *pBoring = m_pArrBoringEx.GetAt(nBoring);

	AhTPAMake(nCountLayer, &pBoring->m_pArrBoring, (CBoringData*)0);
}

// nPart : 부위 
CString CARcBridgeDataStd::GetStringBM_FormType(long nPart)
{
	if(m_nTypeBMApply == BMTYPE_NORMAL)	// 일반수량
	{
		if(nPart >= 0 && nPart < MAX_BM_FORM_TYPE_NORMAL)
		return m_sBM_FormTypeNormal[nPart];
	}
	else								// 실적수량
	{
		if(nPart >= 0 && nPart < MAX_BM_FORM_TYPE)
		return m_sBM_FormType[nPart];
	}

	return _T("");
}

// 이 함수는 CHgBaseBM에서 가져온 함수다
// CHgBaseBM이 엔진으로 승격되는날 이 함수는 제거되고 CHgBaseBM에서 직접 가져다 쓰게 될 것이다.
CString CARcBridgeDataStd::GetStringCircleNum(long nNum)
{
	const long nCount = 41;
	CString sCircleText[nCount] = 
	{
		"①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩",
		"⑪", "⑫", "⑬", "⑭", "⑮", "ⓐ", "ⓑ", "ⓒ", "ⓓ", "ⓔ",
		"ⓕ", "ⓖ", "ⓗ", "ⓘ", "ⓙ", "ⓚ", "ⓛ", "ⓜ", "ⓝ", "ⓞ",
		"ⓟ", "ⓠ", "ⓡ", "ⓢ", "ⓣ", "ⓤ", "ⓥ", "ⓦ", "ⓧ", "ⓨ", 
		"ⓩ"
	};

	if(nNum <= 0 || nNum >= nCount) return "";
	return sCircleText[nNum-1];
}

void CARcBridgeDataStd::SerializeArrayOfDoubleArray(CArchive &ar, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArr)
{
	if(!pdArr) return;
	long nSizeRoot	= pdArr->GetSize();

	if(ar.IsStoring())
	{
		ar << nSizeRoot;
	}
	else
	{
		ar >> nSizeRoot;
		AhTPAMake(nSizeRoot, pdArr, (CDoubleArray*)0);
	}

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CDoubleArray *dArr;
		dArr	= pdArr->GetAt(i);
		SerializeDoubleArray(ar, dArr);
	}
}

void CARcBridgeDataStd::SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr)
{
	long nSize	= 0;
	
	if(ar.IsStoring())
	{
		nSize = dArr->GetSize();
		ar << nSize;
		long i = 0; for(i = 0; i < nSize; i++)
		{
			ar << dArr->GetAt(i);
		}
	}
	else
	{
		double d	= 0;
		ar >> nSize;
		dArr->RemoveAll();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			ar >> d;
			dArr->Add(d);
		}
	}
}

// 더블어래이에 있는 값을 CDPoint(개수, 값)으로 변환
void CARcBridgeDataStd::GetPointArrayCountAndValue(CDPointArray& xyArr, CDoubleArray& dArr)
{
	xyArr.RemoveAll();
	
	int nSize = dArr.GetSize();
	int nCount = 1;
// 	double d1 = 0, d2 = 0;
	int i=0; for(i=0; i<nSize; i++)
	{
		double d1 = dArr.GetAt(i);
		if(i==nSize-1)
		{
			xyArr.Add(CDPoint(nCount, d1));
			break;
		}
		double d2 = dArr.GetAt(i+1);

		if(d1==d2)
		{
			nCount++;
			continue;
		}
		else
		{
			xyArr.Add(CDPoint(nCount, d1));
			nCount = 1;
		}
	}
}

CString CARcBridgeDataStd::GetStringBridgeOrder(int nBri)
{
	CString str = _T("");

// 	int nSeparBri = 0;
	int nOrder = 0;
	int nSize = GetBridgeSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		int nSeparBri = GetBridge(i)->m_nSeparBri;
		if(nSeparBri<=1) nOrder++;
		if(i==nBri)
		{
			str.Format(_T("교량 %d"), nOrder);

			// 상행 하행으로 표현 할 경우 사용자가 방향 판단시 헷갈릴 수 있어서 단순 인덱스로만 표현함.
			if(nSeparBri==1)
				str += _T("-1");//_T("(상행)");
			if(nSeparBri==2)
				str += _T("-2");//_T("(하행)");
			break;
		}
	}

	return str;
}

// nBri에 짝지되는 bri의 인덱스 리턴
// nBri가 상행선이면 하행선, 하행이면 상행선의 인덱스를 리턴하고, 상하행 분리가 아닐 경우
// -1을 리턴
long CARcBridgeDataStd::GetIdxMatchBridge(long nBri)
{
	long nIdx	= -1;
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(pBri)
	{
		if(pBri->m_nSeparBri == 1)	// 상행이면 다음 교량
			nIdx	= nBri + 1;
		else if(pBri->m_nSeparBri == 2) // 하행이면 이전 교량
			nIdx	= nBri - 1;
	}

	if(nIdx >= GetBridgeSize()) nIdx = -1;

	return nIdx;
}

CRcBridgeApp* CARcBridgeDataStd::GetBridgeMatch(CRcBridgeApp* pBri)
{
	CRcBridgeApp *pBriMatch = NULL;

	if(pBri->m_nSeparBri!=0)
	{
		// 상행이면 다음 교량
		if(pBri->m_nSeparBri == 1)	
			pBriMatch = GetBridge(pBri->m_nIdxBridge+1);
		// 하행이면 이전 교량
		else if(pBri->m_nSeparBri == 2) 
			pBriMatch = GetBridge(pBri->m_nIdxBridge-1);
	}

	return pBriMatch;
}

CRcBridgeRebar* CARcBridgeDataStd::GetBridgeMatchRebar(CRcBridgeRebar* pBri)
{
	CRcBridgeRebar *pBriMatch = NULL;

	if(pBri->m_nSeparBri!=0)
	{
		// 상행이면 다음 교량
		if(pBri->m_nSeparBri == 1)	
			pBriMatch = GetBridgeRebar(pBri->m_nIdxBridge+1);
		// 하행이면 이전 교량
		else if(pBri->m_nSeparBri == 2) 
			pBriMatch = GetBridgeRebar(pBri->m_nIdxBridge-1);
	}

	return pBriMatch;
}

void CARcBridgeDataStd::CopyToCoupleBridge_SpanConst(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri<0) return;

	CRcBridgeApp *pBriSou = GetBridge(nBri);
	CRcBridgeApp *pBriTar = GetBridge(nIdxCoupleBri);
	if(pBriSou && pBriTar) 
	{
		CJijum* pJSou = NULL;
//		CJijum* pJTar = NULL;

		int nCountJijumSou = pBriSou->GetCountJijum();
		int nCountJijumTar = pBriTar->GetCountJijum();
		if(nCountJijumSou==nCountJijumTar)
		{
			int i=0; for(i=0; i<nCountJijumSou; i++)
			{
				pJSou = pBriSou->GetJijum(i);
				CJijum* pJTar = pBriTar->GetJijum(i);

				pBriTar->m_bExpJoint[i] = pBriSou->m_bExpJoint[i];
				pJTar->m_strJijumShoe[0] = pJSou->m_strJijumShoe[0];
				pJTar->m_strJijumShoe[1] = pJSou->m_strJijumShoe[1];
				pJTar->m_vAngle = pJSou->m_vAngle;

				pJTar->m_dLengthUgan[0] = pJSou->m_dLengthUgan[0];
				pJTar->m_dLengthUgan[1] = pJSou->m_dLengthUgan[1];
				pJTar->m_dLengthSlabToShoe[0] = pJSou->m_dLengthSlabToShoe[0];
				pJTar->m_dLengthSlabToShoe[1] = pJSou->m_dLengthSlabToShoe[1];
				pBriTar->m_dLengthJigan[i] = pBriSou->m_dLengthJigan[i];
			}
			
			pBriTar->m_arrStrJijumLen.RemoveAll();
			pBriTar->m_arrJijumLen.RemoveAll();
			int nSize = pBriSou->m_arrStrJijumLen.GetSize();
			for(i=0; i<nSize; i++)
			{
				pBriTar->m_arrStrJijumLen.Add(pBriSou->m_arrStrJijumLen.GetAt(i));
				pBriTar->m_arrJijumLen.Add(pBriSou->m_arrJijumLen.GetAt(i));
			}

			pBriSou->SyncBridge();
			pBriTar->SyncBridge();
		}
	}
}

// 종단면 제원에 대해서 짝지bri에게 데이타를 복사한다.
// 기초를 제외한 모든 데이타를 복사한다.
// 입력받는 데이터에 대해서만 복사를 하면 됨.
void CARcBridgeDataStd::CopyToCoupleBridge_VertSection(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeApp *pBriSource	= GetBridge(nBri);
	CRcBridgeApp *pBriTarget	= GetBridge(nIdxCoupleBri);
	if(pBriSource && pBriTarget) 
	{
		pBriTarget->m_nTypeJongdanForRebar	= pBriSource->m_nTypeJongdanForRebar;		// 종단면 타입 : 0 : 수평, 1 : 경사(배근시만 적용됨), 2 : 일반도를 위해서 종단선형으로 그림
		pBriTarget->m_nTypeOutWall	= pBriSource->m_nTypeOutWall;		// 외측벽체 유무(좌우)

		// 상부 슬래브/헌치
		pBriTarget->m_nCountInWallHunch	= pBriSource->m_nCountInWallHunch;		// 내측벽체 헌치 개수
		for(long upper=0; upper<2; upper++)
		{
			for(long hunch = 0; hunch < MAX_INWALL_HUNCH; hunch++)
			{
				pBriTarget->m_dHHL[hunch][upper]	= pBriSource->m_dHHL[hunch][upper];		// 각 벽체 헌치 높이(좌)
				pBriTarget->m_dHHR[hunch][upper]	= pBriSource->m_dHHR[hunch][upper];		// 각 벽체 헌치 높이(우)
				pBriTarget->m_dHBH[hunch][upper]	= pBriSource->m_dHBH[hunch][upper];	// 각 벽체 헌치아래 돌출 높이
				pBriTarget->m_dHLW[hunch][upper]	= pBriSource->m_dHLW[hunch][upper];	// 각 벽체 헌치 너비(왼쪽)
				pBriTarget->m_dHRW[hunch][upper]	= pBriSource->m_dHRW[hunch][upper];	// 각 벽체 헌치 너비(오른쪽)
				pBriTarget->m_dHLR[hunch][upper]	= pBriSource->m_dHLR[hunch][upper];	// 각 벽체 헌치 왼쪽 R
				pBriTarget->m_dHRR[hunch][upper]	= pBriSource->m_dHRR[hunch][upper];	// 각 벽체 헌치 오른쪽 R
				pBriTarget->m_dWL[hunch][upper]		= pBriSource->m_dWL[hunch][upper];		// 각 벽체 헌치 시작위치(왼쪽)
				pBriTarget->m_dWR[hunch][upper]		= pBriSource->m_dWR[hunch][upper];		// 각 벽체 헌치 시작위치(오른쪽)
			}
		}
		
		// 시점측 벽체
		pBriTarget->m_dFHS	= pBriSource->m_dFHS;
		pBriTarget->m_dFLS	= pBriSource->m_dFLS;
		pBriTarget->m_dFRS	= pBriSource->m_dFRS;
		pBriTarget->m_dWS	= pBriSource->m_dWS;
		pBriTarget->m_dBHWS	= pBriSource->m_dBHWS;
		pBriTarget->m_dBHHS	= pBriSource->m_dBHHS;
		pBriTarget->m_dBHS	= pBriSource->m_dBHS;
		pBriTarget->m_dBTHS	= pBriSource->m_dBTHS;
		pBriTarget->m_dBWS	= pBriSource->m_dBWS;
		pBriTarget->m_dHWS	= pBriSource->m_dHWS;
		pBriTarget->m_dHHS	= pBriSource->m_dHHS;
		pBriTarget->m_dWSHW	= pBriSource->m_dWSHW;
		pBriTarget->m_dWSHH	= pBriSource->m_dWSHH;
		pBriTarget->m_dHRS	= pBriSource->m_dHRS;
		//pBriTarget->m_dELSB	= pBriSource->m_dELSB;

		// 종점측 벽체
		pBriTarget->m_dFHE	= pBriSource->m_dFHE;
		pBriTarget->m_dFLE	= pBriSource->m_dFLE;
		pBriTarget->m_dFRE	= pBriSource->m_dFRE;
		pBriTarget->m_dWE	= pBriSource->m_dWE;
		pBriTarget->m_dBHWE	= pBriSource->m_dBHWE;
		pBriTarget->m_dBHHE	= pBriSource->m_dBHHE;
		pBriTarget->m_dBHE	= pBriSource->m_dBHE;		
		pBriTarget->m_dBTHE	= pBriSource->m_dBTHE;
		pBriTarget->m_dBWE	= pBriSource->m_dBWE;
		pBriTarget->m_dHWE	= pBriSource->m_dHWE;
		pBriTarget->m_dHHE	= pBriSource->m_dHHE;
		pBriTarget->m_dWEHW	= pBriSource->m_dWEHW;
		pBriTarget->m_dWEHH	= pBriSource->m_dWEHH;
		pBriTarget->m_dHRE	= pBriSource->m_dHRE;
		//pBriTarget->m_dELEB	= pBriSource->m_dELEB;

		// 내측벽체
		long nCountInWall1	= pBriSource->GetCountInWall();
		long nCountInWall2	= pBriTarget->GetCountInWall();
		if(nCountInWall1 == nCountInWall2)
		{
			for(long wall = 0; wall < nCountInWall1; wall++)
			{
				CWallApp *pWallSource	= pBriSource->GetInWall(wall);
				CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
				if(pWallSource && pWallTarget)
				{
					pWallTarget->m_bTopIsColumn	= pWallSource->m_bTopIsColumn;
					pWallTarget->m_nType	= pWallSource->m_nType;				// 벽체 종류(0 : 수직, 1 : 각도 있는거, 2 : v자형);
						// 일반형상
					//pWallTarget->m_dELB		= pWallSource->m_dELB;			// 기둥하단 EL
					pWallTarget->m_dW		= pWallSource->m_dW;			// 벽체 너비
					pWallTarget->m_dH		= pWallSource->m_dH;			// 벽체 높이(길이)
					pWallTarget->m_dAngle	= pWallSource->m_dAngle;		// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
					pWallTarget->m_bApplyAngleEnd	= pWallSource->m_bApplyAngleEnd;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
						
						// V자형
					pWallTarget->m_dELRT	= pWallSource->m_dELRT;	// V자 우측팔 Top
					pWallTarget->m_dELLT	= pWallSource->m_dELLT;	// V자 좌측팔 Top
					pWallTarget->m_dWL		= pWallSource->m_dWL;
					pWallTarget->m_dWR		= pWallSource->m_dWR;
					pWallTarget->m_dLR		= pWallSource->m_dLR;
					pWallTarget->m_dAngleV	= pWallSource->m_dAngleV;		// V자형의 사이각
					pWallTarget->m_dR1		= pWallSource->m_dR1;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
					pWallTarget->m_dR2		= pWallSource->m_dR2;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)
				}
			}
		}
	}
}

// 주철근 배치에 대해 짝지bri에게 데이타를 복사한다.
// 기초를 제외한 모든 데이타를 복사한다.
void CARcBridgeDataStd::CopyToCoupleBridge_RebarMain(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeRebar *pBriSource	= GetBridgeRebar(nBri);
	CRcBridgeRebar *pBriTarget	= GetBridgeRebar(nIdxCoupleBri);
	if(pBriSource == NULL || pBriTarget == NULL) return;
	
	if(!pBriSource->m_bApplySameMainRebar) return;

	long upper(0),  stt(0), left(0), inner(0),  dan(0);

	// 주철근 배치 ///////////////////////////////////////////////////////////////////////////////////////////////////
	// 철근 배치 옵션 관련
	pBriTarget->m_bRoundAtEdgeRebar	= pBriSource->m_bRoundAtEdgeRebar;		// 헌치부 철근을 라운드 적용할지?

	// 철근 배치 간격 index
	pBriTarget->m_nIdxCTC_MainRebar	= pBriSource->m_nIdxCTC_MainRebar;

	// 인장, 압축구간을 동일간격으로 배치
	pBriTarget->m_bSameOffsetPlaceTAndCBlock	= pBriSource->m_bSameOffsetPlaceTAndCBlock;	
		
	// 이음 개수
	for(long jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBriTarget->m_nCountJointMainRebar_UpperSlab_Cycle1[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_UpperSlab_Cycle1[jijum][upper][dan];	// 상부슬래브 상,하면 각 지간별 이음 개수
				pBriTarget->m_nCountJointMainRebar_UpperSlab_Cycle2[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_UpperSlab_Cycle2[jijum][upper][dan];	// 상부슬래브 상,하면 각 지간별 이음 개수
				pBriTarget->m_nCountJointMainRebar_LowerSlab_Cycle1[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_LowerSlab_Cycle1[jijum][upper][dan];	// 상부슬래브 상,하면 각 지간별 이음 개수
				pBriTarget->m_nCountJointMainRebar_LowerSlab_Cycle2[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_LowerSlab_Cycle2[jijum][upper][dan];	// 상부슬래브 상,하면 각 지간별 이음 개수
			}
		}
	}
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBriTarget->m_nCountJointMainRebar_OutWall_Cycle1[stt][left][dan]	= pBriSource->m_nCountJointMainRebar_OutWall_Cycle1[stt][left][dan];	// 외측벽체 시종점,좌우측 이음 개수
				pBriTarget->m_nCountJointMainRebar_OutWall_Cycle2[stt][left][dan]	= pBriSource->m_nCountJointMainRebar_OutWall_Cycle2[stt][left][dan];	// 외측벽체 시종점,좌우측 이음 개수
			}
		}
	}

	// 날개벽 배근(함수는 제외)
	// ******************* 여기서부터 아래 표시선 까지는 반드시 SerializeRebar함수에서 파일에 저장함. ***************************
	// CDoubleArray나 CDoubleArray*만 와야 된다.
	// 파일 저장시 복잡한 변수명과 개수로 인한 혼선을 방지하기 위해 하나의 함수에서 일괄적으로 처리하기 때문에..
		
	// CYCLE1~4 ///////////////////////////////////////////////////////////////////
	// rebar info cycle 변수
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		// 이음 간격
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDistUpperSlab[cycle][upper][dan], &pBriSource->m_pArrDistUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDistLowerSlab[cycle][upper][dan], &pBriSource->m_pArrDistLowerSlab[cycle][upper][dan]);
			}
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					CopyDoubleArray(&pBriTarget->m_pArrDistOutWall[cycle][stt][left][dan], &pBriSource->m_pArrDistOutWall[cycle][stt][left][dan]);
				}
			}
		}

		// 철근지름
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDiaUpperSlab[cycle][upper][dan], &pBriSource->m_pArrDiaUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDiaLowerSlab[cycle][upper][dan], &pBriSource->m_pArrDiaLowerSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrExistUpperSlab[cycle][upper][dan], &pBriSource->m_pArrExistUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrExistLowerSlab[cycle][upper][dan], &pBriSource->m_pArrExistLowerSlab[cycle][upper][dan]);
			}
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					CopyDoubleArray(&pBriTarget->m_pArrDiaOutWall[cycle][stt][left][dan], &pBriSource->m_pArrDiaOutWall[cycle][stt][left][dan]);
					CopyDoubleArray(&pBriTarget->m_pArrExistOutWall[cycle][stt][left][dan], &pBriSource->m_pArrExistOutWall[cycle][stt][left][dan]);
				}
			}
		}

		CopyDoubleArray(&pBriTarget->m_dArrDiaUpperSlabHunch[cycle], &pBriSource->m_dArrDiaUpperSlabHunch[cycle]);
	}
	/////////////////////////////////////////////////////////////////// CYCLE1~4 //

	// 철근연결및 이음수 설정
	for(upper = 0; upper < 2; upper++)
	{
		CopyArrayOfSetRebarJoint(&pBriTarget->m_pArrSetRebarJointUpperSlab[upper], &pBriSource->m_pArrSetRebarJointUpperSlab[upper]);
		CopyArrayOfSetRebarJoint(&pBriTarget->m_pArrSetRebarJointLowerSlab[upper], &pBriSource->m_pArrSetRebarJointLowerSlab[upper]);
	}
	for(stt = 0; stt < 2; stt++)
	{
		pBriTarget->m_pArrSetRebarJointOutWall_Outter[stt]	= pBriSource->m_pArrSetRebarJointOutWall_Outter[stt];
		for(upper = 0; upper < 2; upper++)
			pBriTarget->m_pArrSetRebarJointOutWall_Inner[stt][upper]	= pBriSource->m_pArrSetRebarJointOutWall_Inner[stt][upper];	// 시종점, 내측, 위아래 외측벽체  2사이클 철근 연결 및 이음개수
	}

	// 철근배치 옵션
	for(inner = 0; inner < 2; inner++)
	{
		pBriTarget->m_bPlacingOutWallInnerRebar[inner]	= pBriSource->m_bPlacingOutWallInnerRebar[inner];	// 시 종점외측벽체 내측철근 배치
	}

	pBriTarget->m_bPlacingInWallRebarOfCycle24	= pBriSource->m_bPlacingInWallRebarOfCycle24;		// 중간벽체 철근 배치
	pBriTarget->m_bBendingDanBuRebar			= pBriSource->m_bBendingDanBuRebar;					// 단 절점부 철근 구부려서 배치
	pBriTarget->m_bPlacingStirrupAtDanBuRebar	= pBriSource->m_bPlacingStirrupAtDanBuRebar;			// 단 절점부 ROUND부에 스트럽 배치
	pBriTarget->m_bPlacingSupportAtHunchRebar	= pBriSource->m_bPlacingSupportAtHunchRebar;		// 헌치철근위에 배력철근 배치
	pBriTarget->m_bPlacingSupportAtDanBuRebar	= pBriSource->m_bPlacingSupportAtDanBuRebar;		// 단 절점부 ROUND부에 배력철근 배치
	pBriTarget->m_bPlacing23DanByStandardOffset	= pBriSource->m_bPlacing23DanByStandardOffset;		// 2,3단 철근을 표준배치간격으로 배치
	pBriTarget->m_nCountWrapMainRebarUpperSlab			= pBriSource->m_nCountWrapMainRebarUpperSlab;				// 주철근을 감싸는 개수
	pBriTarget->m_nCountWrapMainRebarLowerSlab			= pBriSource->m_nCountWrapMainRebarLowerSlab;
		
	// 철근 단수 설정
	for(upper = 0; upper < 2; upper++)
	{
		CopyArrayOfSetRebarDan(&pBriTarget->m_pArrSetRebarDanUpperSlab[upper], &pBriSource->m_pArrSetRebarDanUpperSlab[upper]);
		CopyArrayOfSetRebarDan(&pBriTarget->m_pArrSetRebarDanLowerSlab[upper], &pBriSource->m_pArrSetRebarDanLowerSlab[upper]);
	}

	for(stt = 0; stt < 2; stt++)
	{
		for(inner = 0; inner < 2; inner++)
		{
			pBriTarget->m_pArrSetRebarDanOutWall[stt][inner]	= pBriSource->m_pArrSetRebarDanOutWall[stt][inner];
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////// 주철근 배치 //

	// 내측벽체에 대해서...
	// 내측벽체
	long nCountInWall1	= pBriSource->GetCountInWall();
	long nCountInWall2	= pBriTarget->GetCountInWall();
	if(nCountInWall1 == nCountInWall2)
	{
		for(long wall = 0; wall < nCountInWall1; wall++)
		{
			CWallApp *pWallSource	= pBriSource->GetInWall(wall);
			CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
			if(pWallSource && pWallTarget)
			{
				// 배근 ----------------------------------------------------------
				pWallTarget->m_nIdxCTC_MainRebar			= pWallSource->m_nIdxCTC_MainRebar;	// 주철근 CTC
				pWallTarget->m_bSameOffsetPlaceTAndCBlock	= pWallSource->m_bSameOffsetPlaceTAndCBlock;	// 인장압축구간을 동일 간격으로 배치		

				// 배치단수 설정/철근지름 설정
				for(left = 0; left < 2; left++)
					pWallTarget->m_setRebarDan[left]		= pWallSource->m_setRebarDan[left];	// 벽체 좌우측 단수 설정
											
				pWallTarget->m_nIdxCountCycleMainRebar		= pWallSource->m_nIdxCountCycleMainRebar;	// 주철근 사이클 수.
				pWallTarget->m_nTypeCurveOfVertRebar		= pWallSource->m_nTypeCurveOfVertRebar;
				//---------------------------------------------------------- 배근 
			}
		}
	}
}

// doublearray간의 복사.
void CARcBridgeDataStd::CopyDoubleArray(CDoubleArray *dArrTarget, CDoubleArray *dArrSource)
{
	if(!dArrTarget || !dArrSource) return;

	dArrTarget->RemoveAll();

	long nSize	= dArrSource->GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
		dArrTarget->Add(dArrSource->GetAt(i));
}

void CARcBridgeDataStd::CopyArrayOfDoubleArray(CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrTarget, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrSource)
{
	if(!pdArrTarget || !pdArrSource) return;
	long nSizeRoot	= pdArrSource->GetSize();

	AhTPAMake(nSizeRoot, pdArrTarget, (CDoubleArray*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CDoubleArray *dArrTarget	= pdArrTarget->GetAt(i);
		CDoubleArray *dArrSource	= pdArrSource->GetAt(i);
		CopyDoubleArray(dArrTarget, dArrSource);
	}
}

void CARcBridgeDataStd::CopyArrayOfSetRebarJoint(CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrSource)
{
	if(!pArrTarget || !pArrSource) return;
	long nSizeRoot	= pArrSource->GetSize();

	AhTPAMake(nSizeRoot, pArrTarget, (CSetRebarJoint*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CSetRebarJoint *pTarget		= pArrTarget->GetAt(i);
		CSetRebarJoint *pSource		= pArrSource->GetAt(i);
		
		*pTarget	= *pSource;
	}
}

void CARcBridgeDataStd::CopyArrayOfSetRebarDan(CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrSource)
{
	if(!pArrTarget || !pArrSource) return;
	long nSizeRoot	= pArrSource->GetSize();

	AhTPAMake(nSizeRoot, pArrTarget, (CSetRebarDan*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CSetRebarDan *pTarget	= pArrTarget->GetAt(i);
		CSetRebarDan *pSource	= pArrSource->GetAt(i);
		
		*pTarget	= *pSource;
	}
}

// 표준단면 배근에 대해 짝지bri에게 데이타를 복사한다.
// 기초를 제외한 모든 데이타를 복사한다.
void CARcBridgeDataStd::CopyToCoupleBridge_RebarStandardSection(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeRebar *pBriSource	= GetBridgeRebar(nBri);
	CRcBridgeRebar *pBriTarget	= GetBridgeRebar(nIdxCoupleBri);
	if(pBriSource == NULL || pBriTarget == NULL) return;

	if(!pBriSource->m_bApplySameSupportRebar) return;

	long stt(0);

	// 표준단면 철근 배치 관련 변수들 (배력철근, 전단철근)
	for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			// 배력철근
			pBriTarget->m_placeByBlock_SupportRebar_UpperSlab[upper][dan]	= pBriSource->m_placeByBlock_SupportRebar_UpperSlab[upper][dan];											// 상부슬래브 상하면
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(long left = 0; left < 2; left++)
			{
				pBriTarget->m_placeByBlock_SupportRebar_OutWall[stt][left][dan]	= pBriSource->m_placeByBlock_SupportRebar_OutWall[stt][left][dan];										// 외측벽체 시종점, 내외측
			}
		}
	}

	// 전단철근
	pBriTarget->m_placeByBlock_ShearRebar_UpperSlab	= pBriSource->m_placeByBlock_ShearRebar_UpperSlab;		// 상부슬래브 전단철근 
	for(stt = 0; stt < 2; stt++)
	{
		pBriTarget->m_placeByBlock_ShearRebar_OutWall[stt]	= pBriSource->m_placeByBlock_ShearRebar_OutWall[stt];	// 측벽체 시종점 전단철근
	}
	pBriTarget->m_nIdxCTC_SupportAndShearRebar	= pBriSource->m_nIdxCTC_SupportAndShearRebar;						// 배력,전단철근 표준 배
	

	// 내측벽체에 대해서...
	// 내측벽체
	long nCountInWall1	= pBriSource->GetCountInWall();
	long nCountInWall2	= pBriTarget->GetCountInWall();
	if(nCountInWall1 == nCountInWall2)
	{
		for(long wall = 0; wall < nCountInWall1; wall++)
		{
			CWallApp *pWallSource	= pBriSource->GetInWall(wall);
			CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
			if(pWallSource && pWallTarget)
			{
				pWallTarget->CopyPlaceRebar_SupportRebar(pWallSource);
				pWallTarget->CopyPlaceRebar_ShearRebar(pWallSource);
				pWallTarget->m_nIdxCTC_SupportAndShearRebar	= pWallSource->m_nIdxCTC_SupportAndShearRebar;		// 배력,전단철근 표준배치간격
			}
		}
	}
}

CString CARcBridgeDataStd::GetBridgeTotalName(long nBri)
{
	CRcBridgeApp* pBri = GetBridge(nBri);
	if(!pBri) return _T("");

// 	CString str	= _T("");
	CString str = GetStringBridgeOrder(nBri) + _T(" : ") + pBri->m_strBridgeName
		+ _T(" [") + pBri->GetStringBridgeType() + _T("]");

	return str;
}


//////////////////////////////////////////////////////////////////////////
// 철도하중 관련 추가함수....
// railload 정보 읽어 옴.
BOOL CARcBridgeDataStd::GetRailLoadWheelInfo(long nIdxWheel, long &nType, double &dWeight, double &dDist)
{
	// 타입
	long nQtyWheel	= m_dArrTypeLoadTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	nType	= (long)m_dArrTypeLoadTrainWheel.GetAt(nIdxWheel);

	// 하중
	nQtyWheel	= m_dArrLoadTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	dWeight	= m_dArrLoadTrainWheel.GetAt(nIdxWheel);

	// 다음하중과의 거리
	nQtyWheel	= m_dArrDistTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	dDist	= m_dArrDistTrainWheel.GetAt(nIdxWheel);

	return TRUE;
}

// railload 정보를 설정함.
// void CARcBridgeDataStd::SetRailLoadWheelInfo(long nIdxWheel, long nType, double dWeight, double dDist)
// {
// 	// 타입
// 	long nQtyWheel	= m_dArrTypeLoadTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrTypeLoadTrainWheel.Add((double)nType);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrTypeLoadTrainWheel.SetAt(nIdxWheel, (double)nType);
// 
// 	// 하중
// 	nQtyWheel	= m_dArrLoadTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrLoadTrainWheel.Add(dWeight);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrLoadTrainWheel.SetAt(nIdxWheel, dWeight);
// 
// 	// 다음하중과의 거리
// 	nQtyWheel	= m_dArrDistTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrDistTrainWheel.Add(dDist);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrDistTrainWheel.SetAt(nIdxWheel, dDist);
// }

double CARcBridgeDataStd::GetRailLoadWeightWheel(long nSttWheel, double dLengthSpan)
{
	long nQtyWheel = m_dArrLoadTrainWheel.GetSize();

	double dLoadSum = 0;
	double dDistSum = 0;
	long i=0; for(i=nSttWheel ;i<nQtyWheel; i++)
	{
		if(dDistSum>dLengthSpan) break;
		if(m_dArrTypeLoadTrainWheel.GetAt(i)==2) continue;
		// 처음 바퀴는 걸림. 거리는 다음 바퀴와의 거리임....
		dDistSum += m_dArrDistTrainWheel.GetAt(i);
		dLoadSum += m_dArrLoadTrainWheel.GetAt(i);
	}

	return dLoadSum;
}

// double CARcBridgeDataStd::GetRailLoadDistWheel(long nWheel)
// {
// 	double dDistSum = 0;
// 	long i=0; for(i=0 ;i<nWheel; i++)
// 	{
// 		dDistSum += m_dArrDistTrainWheel.GetAt(i);
// 	}
// 
// 	return dDistSum;
// }

double CARcBridgeDataStd::GetRailLoadLengthWheel(long nSttWheel)
{
	long nQtyWheel = m_dArrDistTrainWheel.GetSize();

	double dDistSum = 0;
	long i=0; for(i=nSttWheel ;i<nQtyWheel; i++)
	{
		if(i>0 && m_dArrTypeLoadTrainWheel.GetAt(i)==2) 
		{
			dDistSum -= m_dArrDistTrainWheel.GetAt(i-1);
			continue;
		}
		dDistSum += m_dArrDistTrainWheel.GetAt(i);
	}

	return dDistSum;
}

// 철도하중에 적용할 바퀴 하중 개수 리턴
long CARcBridgeDataStd::GetCountRailLoadWheel()
{
	return (long)m_dArrLoadTrainWheel.GetSize();
}

BOOL CARcBridgeDataStd::IsRailDesign()
{
	if(m_nTypeConditionApply<DESIGN_CONDITION_RAIL)
		return FALSE;
	else 
		return TRUE;
}

BOOL CARcBridgeDataStd::IsRailDesign2017()
{
	if(IsRailDesign() == FALSE) return FALSE;

	// (ARCBRIDGE-3484) KDS 2018을 선택해도 2017에 포함된 내용들이 반영되도록
	return m_nTypeRailCode >= DESIGN_RAIL_2017;
}

BOOL CARcBridgeDataStd::IsRailDesignKDS2018()
{
	if(IsRailDesign() == FALSE) return FALSE;

	return m_nTypeRailCode == DESIGN_RAIL_KDS_2018;
}

BOOL CARcBridgeDataStd::IsDoro2010()
{
	if( m_nTypeConditionApply ==DESIGN_CONDITION_ROAD && m_nTypeRoadCode == DESIGN_ROAD_2010 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CARcBridgeDataStd::IsDesignKDS2019()
{
	return m_nTypeRoadCode == DESIGN_ROAD_KDS_2019 ? TRUE : FALSE;
}

CString CARcBridgeDataStd::GetStringRailLoad(long nLoadType)
{
	if(nLoadType < 0) return _T("");

	CString str("");
	if(IsRailDesign2017())
	{
		if(nLoadType>=MAX_RAIL_LOAD_2017) return _T("");

		str = m_sRailLoadType2017[nLoadType];
	}
	else
	{
		if(nLoadType>=MAX_RAIL_LOAD) return _T("");

		str = m_sRailLoadType[nLoadType];
	}

	return str;
}

// long CARcBridgeDataStd::GetTypeRailLoad(CString strLoad)
// {
// 	long i=0; for(i=0; i<MAX_RAIL_LOAD; i++)
// 		if(strLoad==m_sRailLoadType[i])
// 			return i;
// 
// 	return -1;
// }

void CARcBridgeDataStd::SetRailLoad()
{
	m_sNameRailLoad			= _T("");	// 하중의 이름,LS-25.....
	m_DistributionLoadFront	= 0;		// 열차앞쪽 분포하중
	m_DistributionLoadBack	= 0;		// 열차뒤쪽 분포하중
	m_RailLoadWheel			= 0;
	m_dArrTypeLoadTrainWheel.RemoveAll();	// 바퀴하중의 종류
	m_dArrDistTrainWheel.RemoveAll();		// 바퀴하중의 거리, 다음하중과의 거리
	m_dArrLoadTrainWheel.RemoveAll();		// 바퀴하중의 크기, 집중하중만....

	if(!IsRailDesign()) return;
	m_sNameRailLoad = GetStringRailLoad(m_nTypeRailLoad);

	if(IsRailDesign2017())
	{
		if(m_nTypeRailLoad==RAIL_LOAD_KRL2012 || m_nTypeRailLoad==RAIL_LOAD_KRL2012_P)
		{
			BOOL bKRL_P = m_nTypeRailLoad==RAIL_LOAD_KRL2012_P ? TRUE : FALSE;
			m_RailLoadWheel = bKRL_P ? 16.5 : 22.0; // ton
			m_RailDistWheel	= 3.0;
			m_RailDistWheelMid	= 3.0;
			m_DistributionLoadFront = bKRL_P ? 6.0 : 8.0;
			m_DistributionLoadBack = bKRL_P ? 6.0 : 8.0;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(m_RailDistWheel);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);
		}

		// EL18 기존과 동일
		if(m_nTypeRailLoad==RAIL_LOAD_EL18_2017)
		{
			m_RailLoadWheel = 18.0;
			m_RailDistWheel	= 2.10;
			m_RailDistWheelMid	= 1.75*2;

			for(long i=0; i<m_nRailQtyCar; i++)
			{
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(10.30);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	
				if(i==m_nRailQtyCar-1)
					m_dArrDistTrainWheel.Add(0);
				else
					m_dArrDistTrainWheel.Add(3.500);
			}
		}
	}
	else
	{
		if(m_nTypeRailLoad==RAIL_LOAD_LS25 || m_nTypeRailLoad==RAIL_LOAD_LS22 || m_nTypeRailLoad==RAIL_LOAD_LS18)
		{
			if(m_nTypeRailLoad==RAIL_LOAD_LS25) m_RailLoadWheel = 25.0;
			if(m_nTypeRailLoad==RAIL_LOAD_LS22) m_RailLoadWheel = 22.0;
			if(m_nTypeRailLoad==RAIL_LOAD_LS18) m_RailLoadWheel = 18.0;
			//
			m_RailDistWheel		= 1.50;
			m_RailDistWheelMid	= 1.50;

			double dLoad1 = m_RailLoadWheel/2;
			double dLoad2 = m_RailLoadWheel*2.0/3.0;

			m_DistributionLoadBack	= m_RailLoadWheel/3;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad1);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.700);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);	// 50/3
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad1);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.700);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);	// 50/3
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadBack);	m_dArrDistTrainWheel.Add(0);
		}

		if(m_nTypeRailLoad==RAIL_LOAD_EL25 || m_nTypeRailLoad==RAIL_LOAD_EL22 || m_nTypeRailLoad==RAIL_LOAD_EL18)
		{
			if(m_nTypeRailLoad==RAIL_LOAD_EL25) m_RailLoadWheel = 25.0;
			if(m_nTypeRailLoad==RAIL_LOAD_EL22) m_RailLoadWheel = 22.0;
			if(m_nTypeRailLoad==RAIL_LOAD_EL18) m_RailLoadWheel = 18.0;
			m_RailDistWheel		= 2.10;
			m_RailDistWheelMid	= 1.75*2;

			long i=0; for(i=0; i<m_nRailQtyCar; i++)
			{
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(10.30);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	
				if(i==m_nRailQtyCar-1)
					m_dArrDistTrainWheel.Add(0);
				else
					m_dArrDistTrainWheel.Add(3.500);
			}
		}

		if(m_nTypeRailLoad==RAIL_LOAD_HL25)
		{
			m_RailLoadWheel			= 25.0;
			m_RailDistWheel			= 1.60;
			m_RailDistWheelMid		= 1.60;
			m_DistributionLoadFront	= 8.00;
			m_DistributionLoadBack	= 8.00;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadFront);	m_dArrDistTrainWheel.Add(0.8);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(0.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadFront);	m_dArrDistTrainWheel.Add(0.0);
		}
		// 사용자 정의
		if(m_nTypeRailLoad == RAIL_LOAD_USER)
		{
		}
	}	
}

// 단위 변화에 따른 데이터 변화값 구하기
// csOrg : 현재 단위
// csChg : 변환 단위
// 모든 데이터는 CGS 단위로 저장되므로 현재 단위가 CGS 단위이면 처리 안해도 됨.
double CARcBridgeDataStd::GetValueUnitType(double dValue,CString csOrg,CString csChg)
{
	if( m_nTypeUnit == 0 && IsLsdDesign() == FALSE ) return dValue;

	dValue = GetValueUnitChange(dValue,csOrg,csChg);

	return dValue;
}

// CGS단위계에 해당하는 단위를 기준으로 적용된 단위표기를 return;
// nIdxUnitApply : 동일한 단위가 여러개있는 경우의 Index
CString CARcBridgeDataStd::GetStringUnitType(CString sCGS, long nIdxUnitApply)
{
	CString sUnit(_T(""));

	if(IsLsdDesign())
	{
		if(sCGS==UNIT_CGS_TONF)
			sUnit = _T("N");
		else if(sCGS==UNIT_CGS_KGFM || sCGS==UNIT_CGS_KGFCM || sCGS==UNIT_CGS_TONFM)
			sUnit = _T("N.mm");
		else if(sCGS==UNIT_CGS_KGF_M || sCGS==UNIT_CGS_TONF_M)
			sUnit = _T("N/mm");
		else if(sCGS==UNIT_CGS_KGF_M2 || sCGS==UNIT_CGS_TONF_M2 || sCGS == UNIT_CGS_KGF_CM2 )
			sUnit = _T("N/mm²");
		else if(sCGS==UNIT_CGS_KGF_M3 || sCGS==UNIT_CGS_TONF_M3 || sCGS == UNIT_CGS_KGF_CM3 )
			sUnit = _T("N/mm³");
		else if(sCGS == UNIT_CGS_CM || sCGS == UNIT_CGS_M)
			sUnit = _T("mm");
		else if(sCGS == UNIT_CGS_CM2 || sCGS == UNIT_CGS_M2)
			sUnit = _T("mm²");
	}
	else
	{
		sUnit = GetStringUnitByType(sCGS,m_nTypeUnit,nIdxUnitApply);
	}

	return sUnit;
}

double CARcBridgeDataStd::GetUnitWeightReinforcedConcrete()
{
	if( m_nTypeUnit == 0 ) return m_pBasicConcInfo->m_UWeightSteelConcrete;
	CString szKgf_M3_CGS = "kgf/m³";
	CString szKgf_M3_APP = GetStringUnitType(szKgf_M3_CGS);
	return GetValueUnitType(m_pBasicConcInfo->m_UWeightSteelConcrete,szKgf_M3_CGS,szKgf_M3_APP);
}

BOOL CARcBridgeDataStd::IsSI()
{
	return (m_nTypeUnit==1);
}

void CARcBridgeDataStd::SetStringUnitType()
{
	BOOL bLsd = IsLsdDesign();
	m_szKgf	    = GetStringUnitType(UNIT_CGS_KGF);
	m_szTonf    = GetStringUnitType(UNIT_CGS_TONF);
	m_szKgfM    = GetStringUnitType(UNIT_CGS_KGFM);
	m_szTonfM   = GetStringUnitType(UNIT_CGS_TONFM);
	m_szCM		= GetStringUnitType(UNIT_CGS_CM);
	m_szCM2		= GetStringUnitType(UNIT_CGS_CM2);
	m_szCM3		= GetStringUnitType(UNIT_CGS_CM3);
	m_szKgf_CM2 = GetStringUnitType(UNIT_CGS_KGF_CM2, bLsd? 1 : 0);
	m_szKgf_CM3 = GetStringUnitType(UNIT_CGS_KGF_CM3);
	m_szKgf_M2  = GetStringUnitType(UNIT_CGS_KGF_M2);
	m_szKgf_M3  = GetStringUnitType(UNIT_CGS_KGF_M3);
	m_szTonf_M  = GetStringUnitType(UNIT_CGS_TONF_M);
	m_szTonf_M2 = GetStringUnitType(UNIT_CGS_TONF_M2);
	m_szTonf_M3 = GetStringUnitType(UNIT_CGS_TONF_M3);
	m_szM = bLsd ? _T("mm") : _T("m");
	m_szM2 = bLsd ? _T("mm²") : _T("m²");
	m_szM3 = bLsd ? _T("mm³") : _T("m³");
	m_szM4 = bLsd ? _T("mm⁴") : _T("m⁴");
}

BOOL CARcBridgeDataStd::IsRailLoadLS()
{
	if(m_nTypeRailLoad==RAIL_LOAD_LS18) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_LS22) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_LS25) return TRUE;

	return FALSE;
}

BOOL CARcBridgeDataStd::IsRailLoadEL()
{
	if(m_nTypeRailLoad==RAIL_LOAD_EL18) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_EL22) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_EL25) return TRUE;

	return FALSE;
}

double CARcBridgeDataStd::GetValueBasic(double dDia, long nType)
{
	return m_pBasicConcInfo->GetRebarInfo(dDia, nType);
}

double CARcBridgeDataStd::GetValueBasic(long nDia, long nType)
{
	return m_pBasicConcInfo->GetRebarInfo(nDia, nType);
}

/*
#define BRIDGE_LOAD_DEAD		 0 // 고정하중, 연직토압
#define BRIDGE_LOAD_LIVE_FULL	 1 // 활하중 (만재)
#define BRIDGE_LOAD_LIVE_ECC	 2 // 활하중 (편재)
#define BRIDGE_LOAD_TANK		 3 // 탱크 및 특수 중차량하중
#define BRIDGE_LOAD_TOAP_BOTH	 4 // 양토압
#define BRIDGE_LOAD_TOAP_ONE	 5 // 편토압
#define BRIDGE_LOAD_TEMP1		 6 // 온도하중(+)
#define BRIDGE_LOAD_TEMP2		 7 // 온도하중(-)
#define BRIDGE_LOAD_SHRINK		 8 // 건조수축
#define BRIDGE_LOAD_SUBSID		 9 // 지점침하
#define BRIDGE_LOAD_SB			10 // 시동 및 제동하중
#define BRIDGE_LOAD_LR			11 // 장대레일 종하중
#define BRIDGE_LOAD_EQ			12 // 지진하중
#define BRIDGE_LOAD_BASIC		13 // 기본하중의 개수
*/
CString CARcBridgeDataStd::GetStringLoadCase(long nLoadCase, BOOL bEarthQuake)
{
	if(bEarthQuake)
	{
		if(nLoadCase==EARTHQUAKE_LOAD_DEAD)			return "D-EQ";	// 고정하중
		if(nLoadCase==EARTHQUAKE_LOAD_TRAP_X)		return "LOAD-X";	// 종방향 TRAP하중
		if(nLoadCase==EARTHQUAKE_LOAD_TRAP_Y)		return "LOAD-Y";	// 횡방향 TRAP하중
		if(nLoadCase==EARTHQUAKE_LOAD_TOAP)			return "H-EQ";	// 수평토압,수평수압
		if(nLoadCase==EARTHQUAKE_LOAD_INERTIAL)		return "D-IN";	// 관성력
	}
	else
	{
		if(nLoadCase==BRIDGE_LOAD_DEAD)			return "D";		// 고정하중
		if(nLoadCase==BRIDGE_LOAD_SOIL)			return "VH";	// 연직토압
		if(nLoadCase==BRIDGE_LOAD_LIVE_FULL)	return "LF";	// 활하중
		if(nLoadCase==BRIDGE_LOAD_LIVE_ECC)		return "LE";	// 활하중
		if(nLoadCase==BRIDGE_LOAD_TANK)			return "HEAVY";	// 중차량
		if(nLoadCase==BRIDGE_LOAD_TOAP_BOTH)	return "HB";	// 양토압
		if(nLoadCase==BRIDGE_LOAD_TOAP_ONE)		return "HO";	// 편토압
		if(nLoadCase==BRIDGE_LOAD_TEMP1)		return "G1";	// 온도하중(+)
		if(nLoadCase==BRIDGE_LOAD_TEMP2)		return "G2";	// 온도하중(-)
		if(nLoadCase==BRIDGE_LOAD_SHRINK)		return "GS";	// 건조수축
		if(nLoadCase==BRIDGE_LOAD_SUBSID)		return "SU";	// 지점침하
		if(nLoadCase==BRIDGE_LOAD_SB)			return "SB";	// 시동 및 제동하중
		if(nLoadCase==BRIDGE_LOAD_LR)			return "LR";	// 장대레일 종하중
		if(nLoadCase==BRIDGE_LOAD_EQ)			return "EQ";	// 지진하중
		if(nLoadCase==BRIDGE_LOAD_WATERHORI)		return "HW";	// 수평수압
		if(nLoadCase==BRIDGE_LOAD_WATERVERT)		return "VW";	// 연직수압
	}

	return _T("");
}

BOOL CARcBridgeDataStd::IsSupportTypeStd(long nType)
{
	// 문형		0x0001
	// 파이형		0x0002
	// 슬래브형	0x0004
	// 경사1형		0x0008
	// 경사2형		0x0010
	// 아치형		0x0020
	// 박스형		0x0040
	DWORD dwType	= 0x0000;
	switch(nType)
	{
	case BRIDGE_TYPE_PORTAL:		dwType	= ARCBRIDGE_LOCK_CODE_PORTAL; break;
	case BRIDGE_TYPE_PI:			dwType = ARCBRIDGE_LOCK_CODE_PI; break;
	case BRIDGE_TYPE_SLAB:			dwType = ARCBRIDGE_LOCK_CODE_SLAB; break;
	case BRIDGE_TYPE_TRAP:			dwType = ARCBRIDGE_LOCK_CODE_TRAP; break;
	case BRIDGE_TYPE_TRAP2:			dwType = ARCBRIDGE_LOCK_CODE_TRAP2; break;
	case BRIDGE_TYPE_ARCH:			dwType = ARCBRIDGE_LOCK_CODE_ARCH; break;
	case BRIDGE_TYPE_BOX:			dwType = ARCBRIDGE_LOCK_CODE_BOX; break;
	case BRIDGE_TYPE_SLAB_STANDARD:	dwType = ARCBRIDGE_LOCK_CODE_SLAB_STD; break;
	default:
		ASSERT(FALSE);
		return FALSE;
		break;
	}

	if(m_dwTypeARcBridge & dwType) return TRUE;	

	return FALSE;
}

BOOL CARcBridgeDataStd::IsLockMaster()
{
	return m_dwTypeARcBridge == 0xffff;// aGetLockID() == SPLOCK_MASTER_LID;
}

// Plus : 문형~아치,경사
BOOL CARcBridgeDataStd::IsLockRcBridgePlus()
{
	return IsSupportTypeStd(BRIDGE_TYPE_ARCH);	
}

// Basic : 문형,파이,슬래브
BOOL CARcBridgeDataStd::IsLockRcBridgeBasic()
{
	BOOL bSlab = IsSupportTypeStd(BRIDGE_TYPE_SLAB);
	BOOL bArch = IsSupportTypeStd(BRIDGE_TYPE_ARCH);

	return bSlab && !bArch;
}

// 문형 : 문형만 지원
BOOL CARcBridgeDataStd::IsLockRcBridgePortal()
{
	return m_dwTypeARcBridge == ARCBRIDGE_LOCK_CODE_PORTAL;
}

// BOX교 : BOX만 지원
BOOL CARcBridgeDataStd::IsLockRcBridgeBox()
{
	return m_dwTypeARcBridge == ARCBRIDGE_LOCK_CODE_BOX;
}

// BOX교 모듈
BOOL CARcBridgeDataStd::IsLockRcBridgeBoxModule()
{
	return IsSupportTypeStd(BRIDGE_TYPE_BOX);
}

BOOL CARcBridgeDataStd::IsLockRcBridgeStmModule()
{
	// LSD 적용 모듈은 STM도 적용된다.
	return (m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_STM || m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_LSD);
}

BOOL CARcBridgeDataStd::IsLockRcBridgeLsdModule()
{
	return (m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_LSD);
}

// COutBridgeResister에도 같은 함수 있음.
CString CARcBridgeDataStd::GetStringFootTypeAll(CRcBridgeRebar *pBri)
{
	CString szFootType = _T("");

	long nKind		= 0;
	BOOL bDirect	= FALSE;
	BOOL bMass		= FALSE;
	BOOL bPile		= FALSE;

	long nCnt = pBri->GetCountFooting();
	long i=0; for(i=0; i<nCnt; i++)
	{
		CFootingApp *pFoot = pBri->GetFooting(i);
		if(pFoot)
		{
			if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN) bDirect = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) bMass = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) bPile = TRUE;
		}
	}

	if(bDirect)
	{
		if(nKind == 0) szFootType += "직접기초";
//		else if(nKind%2) szFootType += ", 직접기초";
		else szFootType += ", 직접기초";
		nKind++;
	}
	if(bMass)
	{
		if(nKind == 0) szFootType += "MASS기초";
//		else if(nKind%2) szFootType += ", MASS기초";
		else szFootType += ", MASS기초";
		nKind++;
	}
	if(bPile)
	{
		if(nKind == 0) szFootType += "말뚝기초";
//		else if(nKind%2) szFootType += ", 말뚝기초";
		else szFootType += ", 말뚝기초";
		nKind++;
	}
	
	return szFootType;
}

void CARcBridgeDataStd::SetVersionFlag(DWORD dwType)
{	
	m_dwTypeARcBridge	= dwType;
}

// 엑셀 출력시 파일명 만들기
// m_nTypeFileNameForExcel;	
// 엑셀 출력시 파일명(0 : 파일명_교량명_방향, 1 : 교량명_방향, 2 : 파일명_방향, 3 : 파일명_교량명, 4 : 교량명)
CString CARcBridgeDataStd::GetFileNameForExcel(long nBri)
{
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(!pBri) return _T("제목없음");

	CString sFileName	= _T("");

	CString sRunFileName	= m_pDoc->GetPathName();
	long nFind	= sRunFileName.Find(".arb");
	if(nFind > -1)
//  ReleaseBuffer를 하면 CString의 주소를 거져온 변수기 때문에 저장된 Path가 지워진다. 
//		sRunFileName.ReleaseBuffer(nFind);
		sRunFileName  = sRunFileName.Left(sRunFileName.ReverseFind('.'));

	CString strBridgeDir(_T(""));
	strBridgeDir = GetStringBridgeDir(nBri);
		
	switch(m_nTypeFileNameForExcel)
	{
	case 0: sFileName.Format("%s_%s_%s", sRunFileName, pBri->m_strBridgeName, strBridgeDir); break;
	case 1: sFileName.Format("%s_%s", pBri->m_strBridgeName, strBridgeDir); break;
	case 2: sFileName.Format("%s_%s", sRunFileName, strBridgeDir); break;
	case 3: sFileName.Format("%s_%s", sRunFileName, pBri->m_strBridgeName); break;
	case 4: sFileName.Format("%s", pBri->m_strBridgeName); break;
	}

	return sFileName;
}

// 교량생성후 선형이 변경될 경우 DataStd의 선형과 교량의 선형이 서로 매치 되지 않아서 오류가 발생할 수 있음
// 그래서 수시로 DataStd의 선형과 교량의 선형을 매치 시켜 줘야 함.
void CARcBridgeDataStd::SetLineAppToBridge()
{
	int nSizeBri = GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeRebar* pBri = GetBridgeRebar(nBr);
		if(!pBri->m_bUnderGroundRahmen)
		{
			*pBri->m_pLineApp = *m_pLineApp;
			CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
			pBri->m_pLineApp->SetDataIp(strErrorMsg);
		}
		pBri->GetLineBase()->ReSetSearchIp();
	}
}

// 슬래브교만 있을때 슬래브 표준도 작업을 선택할 수 있다.
BOOL CARcBridgeDataStd::IsEnableSlabStandardOutput()
{
	if(!(m_dwTypeARcBridge & BRIDGE_TYPE_SLAB_STANDARD)) return FALSE;
	long nCount	= GetBridgeSize();
	long i = 0; for(i = 0; i < nCount; i++)
	{
		CRcBridgeRebar *pBri	= GetBridgeRebar(i);
		if(!pBri) continue;

		if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
			return FALSE;
	}

	return TRUE;
}

// 지중라멘교인 경우에 선형정보를 생성하고
// 지중라멘교이 아닌 경우는 CARcBridgeDataStd의 선형을 교량선형으로 설정한다...
void CARcBridgeDataStd::MakeLinefoBridge(long nBridge)
{
	CRcBridgeApp	  *pBri = GetBridge(nBridge);

	*pBri->m_pLineApp = *m_pLineApp;
	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
	pBri->m_pLineApp->SetDataIp(strErrorMsg);

	if(pBri->m_bUnderGroundRahmen)
	{
		// 시종점으로 100m씩 연장하여 설정
		CLineInfoApp *pLine = pBri->m_pLineApp;
		double vSlope = frM(pBri->m_dUnderGroundCenterELEnd-pBri->m_dUnderGroundCenterELStt)/pBri->m_dLengthBridge;
		double dAddLength = 100000;
		
		// 1) 종단계획고 설정
		pLine->m_VIPsu = 4;
		pLine->m_VIPData[0][0] = pBri->m_dStationBridgeStt - dAddLength;
		pLine->m_VIPData[0][1] = frM(pBri->m_dUnderGroundCenterELStt) - dAddLength*vSlope;
		pLine->m_VIPData[0][2] = 0;
		pLine->m_VIPData[1][0] = pBri->m_dStationBridgeStt;
		pLine->m_VIPData[1][1] = frM(pBri->m_dUnderGroundCenterELStt);
		pLine->m_VIPData[1][2] = 0;
		pLine->m_VIPData[2][0] = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
		pLine->m_VIPData[2][1] = frM(pBri->m_dUnderGroundCenterELEnd);
		pLine->m_VIPData[2][2] = 0;
		pLine->m_VIPData[3][0] = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
		pLine->m_VIPData[3][1] = frM(pBri->m_dUnderGroundCenterELEnd) + dAddLength*vSlope;
		pLine->m_VIPData[3][2] = 0;

		// 2) 편경사 설정
//		AhTPAMake(2, &pLine->m_pSlopLeft,  (CSlop*)0);
//		AhTPAMake(2, &pLine->m_pSlopRight, (CSlop*)0);
//		pLine->m_pSlopLeft[0]->m_dStationPyungubae = pBri->m_dStationBridgeStt - dAddLength;
//		pLine->m_pSlopLeft[1]->m_dStationPyungubae = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
//
//		pLine->m_pSlopRight[0]->m_dStationPyungubae = pBri->m_dStationBridgeStt - dAddLength;
//		pLine->m_pSlopRight[0]->m_dValuePyungubae   = pBri->m_dUnderGroundSlopeSlabRight/100;
//		pLine->m_pSlopRight[1]->m_dStationPyungubae = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
//		pLine->m_pSlopRight[1]->m_dValuePyungubae   = pBri->m_dUnderGroundSlopeSlabRight/100;

		// 3) 지반고,평면선형은 동일하게 사용...
	}
}

// 기초자료 저장/읽기
void CARcBridgeDataStd::SerializeBase(CArchive &ar)
{
	// 1 : m_bApplyRigidWallEQRotate;
	// 2 : m_Pi_F_Cobel;
	// 3 : m_nTypeRoadCode, m_nTypeRailCode (BOX_TYPE)
	// 4 : m_nLSDRoadType, m_dDesignRoadWidth m_Es_Steel, m_RatePoasong, m_plsdEnv 추가
	// 5 : m_pUserBmData 사용자 수량 저장
	// 6 : m_bApplyBoredPileASD
	// 7 : m_nApplyPhcPileASD
	// 8 : m_nTypeCheckSteelPile
	// 9: m_nSelectPmin_Hyunta_Extreme
	// 10 : m_bPrintConditionApply
	// 11 : m_nTypeFootingSafetyApplyCoefficientImpact
	// 12 : m_nTypeEarthQuakeApply
	// 13 : m_dRateDAMP_EQ_CD

	long nFlagBase = 13;
	const CString sID	= "ARcBridge Base File";

	if(ar.IsStoring())
	{
		ar << nFlagBase;
		ar << sID;

		// SetGridDataFactor
		ar << m_nTypeConditionApply;
		ar << m_nTypeKciCode;
		ar << m_nTypeRoadCode;
		ar << m_nTypeRailCode;
		ar << m_nDegreeBridge;
		ar << m_nDegreeRail;
		ar << m_nTypeRailLoad;
		ar << m_nTypeRailQty;
		ar << m_nTypeUnit;
		ar << m_Fck;
		ar << m_Fy;
		ar << m_bIsDBar_ForDowelBar;
		ar << m_Ec;
		ar << m_Es;
		ar << m_NValue_Steel;
		ar << m_Pi_F; 
		ar << m_Pi_C; 
		ar << m_Pi_V; 
		ar << m_Pi_F_Cobel;
		ar << m_dRateDryAndShrinkage; 
		ar << m_dCoefficientExpansion; 
		ar << m_nDegreeEarthQuake;
		ar << m_nRegionEarthQuake;
		ar << m_dCoefficientGround;
		ar << m_nTypeCoefficientGround;
		ar << m_dRateDAMP_EQ_CD;
		ar << m_gValue;
		ar << m_nTypeRigidFootingEQ;
		ar << m_bApplyRigidWallEQRotate;
		ar << m_nTypeJointPosFootingEQ;
		ar << m_nTypePropertyPosFootingEQ;
		ar << m_nTypeSoilPressureEQ;
		ar << m_nTypeUpperWeightEQ;
		ar << m_dLengthRebarMax;
		ar << m_nTypeRebarConnect;
		ar << m_nTypeBMApply;
		ar << m_pBasicConcInfo->m_nRoundRebarConnect;
		ar << m_bApplyAllowInc;
		ar << m_nTypeFileNameForExcel;
		ar << m_nTypeCalcSlabEffWidth;
		ar << m_nTypeFootingSafetyApplyCoefficientImpact;
		ar << m_nApplyBoredPileASD;
		ar << m_nApplyPhcPileASD;
		ar << m_nTypeCheckSteelPile;
		ar << m_nSelectPmin_Hyunta_Extreme;
		ar << m_bPrintConditionApply;
		ar << m_nTypeEarthQuakeApply;
		ar << m_nTypeProcess;

		// SetGridDataJoint
		long i = 0; for(i = 0; i < REBAR_JOINT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_JOINT_COL; j++)
				ar << m_pBasicConcInfo->m_RebarJoint[i][j];
		}

		// SetGridDataRailLoad
		m_dArrTypeLoadTrainWheel.Serialize(ar);
		m_dArrLoadTrainWheel.Serialize(ar);
		m_dArrDistTrainWheel.Serialize(ar);

		// SetGridDataRebarFactor
		ar << m_pBasicConcInfo->m_dAlpha_UpRebar;
		ar << m_pBasicConcInfo->m_dAlpha_GenBar; 
		ar << m_pBasicConcInfo->m_dBeta_Tomak1; 
		ar << m_pBasicConcInfo->m_dBeta_Tomak2; 
		ar << m_pBasicConcInfo->m_dBeta_GenBar; 
		ar << m_pBasicConcInfo->m_dIpSmallConc; 
		ar << m_pBasicConcInfo->m_dIpGenConc; 

		// SetGridDataSettle
		for(i = 0; i < REBAR_SETTLE_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_SETTLE_COL; j++)
				 ar << m_pBasicConcInfo->m_RebarSettle[i][j];
		}

		// SetGridDataUWeight
		ar << m_pBasicConcInfo->m_UWeightSteel;
		ar << m_pBasicConcInfo->m_UWeightCastIron;
		ar << m_pBasicConcInfo->m_UWeightAluminum;
		ar << m_pBasicConcInfo->m_UWeightSteelConcrete;
		ar << m_pBasicConcInfo->m_UWeightGravel;
		ar << m_pBasicConcInfo->m_UWeightCement;
		ar << m_pBasicConcInfo->m_UWeightLumber;
		ar << m_pBasicConcInfo->m_UWeightInvChungje;
		ar << m_pBasicConcInfo->m_UWeightAsphalt;
		ar << m_pBasicConcInfo->m_UWeightConcrete;


		// SetGridDataWeight
		for(i = 0; i < REBAR_WEIGHT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_WEIGHT_COL; j++)
				ar << m_pBasicConcInfo->m_RebarWeight[i][j];
		}

		// Lsd추가
		ar << m_nLSDRoadType;
		ar << m_dDesignRoadWidth;
		ar << m_Es_Steel;
		ar << m_RatePoasong;
		m_plsdEnv->Serialize(ar);

		m_pUserBmData->Serialize(ar);
	}
	else
	{
		ar >> nFlagBase;
		CString sIDLoad	= _T("");
		ar >> sIDLoad;
		if(sIDLoad != sID)
		{
			AfxMessageBox("ARcBridge 기초자료 파일이 아닙니다.");
			return;
		}


				// SetGridDataFactor
		ar >> m_nTypeConditionApply;
		ar >> m_nTypeKciCode;
		if(nFlagBase > 2)
		{
			ar >> m_nTypeRoadCode;
			ar >> m_nTypeRailCode;
		}
		ar >> m_nDegreeBridge;
		ar >> m_nDegreeRail;
		ar >> m_nTypeRailLoad;
		ar >> m_nTypeRailQty;
		ar >> m_nTypeUnit;
		ar >> m_Fck;
		ar >> m_Fy;
		ar >> m_bIsDBar_ForDowelBar;
		ar >> m_Ec;
		ar >> m_Es;
		ar >> m_NValue_Steel;
		ar >> m_Pi_F; 
		ar >> m_Pi_C; 
		ar >> m_Pi_V; 
		if(nFlagBase > 1)
		{
			ar >> m_Pi_F_Cobel;
		}
		ar >> m_dRateDryAndShrinkage; 
		ar >> m_dCoefficientExpansion; 
		ar >> m_nDegreeEarthQuake;
		ar >> m_nRegionEarthQuake;
		ar >> m_dCoefficientGround;
		if(nFlagBase > 11)
			ar >> m_nTypeCoefficientGround;
		if(nFlagBase > 12)
			ar >> m_dRateDAMP_EQ_CD;
		ar >> m_gValue;
		ar >> m_nTypeRigidFootingEQ;
		if(nFlagBase > 0)
		{
			ar >> m_bApplyRigidWallEQRotate;
		}
		ar >> m_nTypeJointPosFootingEQ;
		ar >> m_nTypePropertyPosFootingEQ;
		ar >> m_nTypeSoilPressureEQ;
		ar >> m_nTypeUpperWeightEQ;
		ar >> m_dLengthRebarMax;
		ar >> m_nTypeRebarConnect;
		ar >> m_nTypeBMApply;
		ar >> m_pBasicConcInfo->m_nRoundRebarConnect;
		ar >> m_bApplyAllowInc;
		ar >> m_nTypeFileNameForExcel;
		ar >> m_nTypeCalcSlabEffWidth;
		if(nFlagBase > 10)
			ar >> m_nTypeFootingSafetyApplyCoefficientImpact;
		if(nFlagBase > 5)
			ar >> m_nApplyBoredPileASD;
		if(nFlagBase > 6)
			ar >> m_nApplyPhcPileASD;
		if(nFlagBase > 7)
			ar >> m_nTypeCheckSteelPile;
		if(nFlagBase > 8)
			ar >> m_nSelectPmin_Hyunta_Extreme;
		if(nFlagBase > 9)
			ar >> m_bPrintConditionApply;
		else
			m_bPrintConditionApply = FALSE;

		if(nFlagBase > 11)
			ar >> m_nTypeEarthQuakeApply;

		ar >> m_nTypeProcess;

		// SetGridDataJoint
		long i = 0; for(i = 0; i < REBAR_JOINT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_JOINT_COL; j++)
				ar >> m_pBasicConcInfo->m_RebarJoint[i][j];
		}

		// SetGridDataRailLoad
		m_dArrTypeLoadTrainWheel.Serialize(ar);
		m_dArrLoadTrainWheel.Serialize(ar);
		m_dArrDistTrainWheel.Serialize(ar);

		// SetGridDataRebarFactor
		ar >> m_pBasicConcInfo->m_dAlpha_UpRebar;
		ar >> m_pBasicConcInfo->m_dAlpha_GenBar; 
		ar >> m_pBasicConcInfo->m_dBeta_Tomak1; 
		ar >> m_pBasicConcInfo->m_dBeta_Tomak2; 
		ar >> m_pBasicConcInfo->m_dBeta_GenBar; 
		ar >> m_pBasicConcInfo->m_dIpSmallConc; 
		ar >> m_pBasicConcInfo->m_dIpGenConc; 

		// SetGridDataSettle
		for(i = 0; i < REBAR_SETTLE_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_SETTLE_COL; j++)
				 ar >> m_pBasicConcInfo->m_RebarSettle[i][j];
		}

		// SetGridDataUWeight
		ar >> m_pBasicConcInfo->m_UWeightSteel;
		ar >> m_pBasicConcInfo->m_UWeightCastIron;
		ar >> m_pBasicConcInfo->m_UWeightAluminum;
		ar >> m_pBasicConcInfo->m_UWeightSteelConcrete;
		ar >> m_pBasicConcInfo->m_UWeightGravel;
		ar >> m_pBasicConcInfo->m_UWeightCement;
		ar >> m_pBasicConcInfo->m_UWeightLumber;
		ar >> m_pBasicConcInfo->m_UWeightInvChungje;
		ar >> m_pBasicConcInfo->m_UWeightAsphalt;
		ar >> m_pBasicConcInfo->m_UWeightConcrete;


		// SetGridDataWeight
		for(i = 0; i < REBAR_WEIGHT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_WEIGHT_COL; j++)
				ar >> m_pBasicConcInfo->m_RebarWeight[i][j];
		}

		if (nFlagBase > 3)
		{
			// Lsd추가
			ar >> m_nLSDRoadType;
			ar >> m_dDesignRoadWidth;
			ar >> m_Es_Steel;
			ar >> m_RatePoasong;
			m_plsdEnv->Serialize(ar);
		}
		if(nFlagBase > 4)
		{
			m_pUserBmData->Serialize(ar);
		}
	}
}

BOOL CARcBridgeDataStd::IsExistMainRebarStraightHorz(CColumnApp *pColumn, long nDan)
{
	BOOL bResult = FALSE;

	CTwinVector tv;
	CTwinVectorArray tvArrRebarPlan;
	pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, nDan);

	if(tvArrRebarPlan.GetSize() > 0)
	{
		switch(pColumn->m_nType)
		{
		case COL_TYPE_CIRCLE:
		case COL_TYPE_RECTANGLE:
		case COL_TYPE_TRACK:
			bResult = tvArrRebarPlan.GetTvByInfo(NOTRANS("기둥단면상면"), tv);
			break;

		case COL_TYPE_POLY:
			tv = tvArrRebarPlan.GetAt(0);
			bResult = Compare(tv.GetLength(), 0.0, _T(">"));
			break;

		default:
			bResult = FALSE;
		}
	}

	return bResult;
}

BOOL CARcBridgeDataStd::IsExistMainRebarStraightVert(CColumnApp *pColumn, long nDan)
{
	BOOL bResult = FALSE;

	CTwinVector tv;
	CTwinVectorArray tvArrRebarPlan;
	pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, nDan);

	if(tvArrRebarPlan.GetSize() > 0)
	{
		switch(pColumn->m_nType)
		{
		case COL_TYPE_CIRCLE:
		case COL_TYPE_RECTANGLE:
		case COL_TYPE_TRACK:
			bResult = tvArrRebarPlan.GetTvByInfo(NOTRANS("기둥단면좌측면"), tv);
			break;

		case COL_TYPE_POLY:
			bResult = FALSE;
			break;

		default:
			bResult = FALSE;
		}
	}

	return bResult;
}

// 기둥 최대철근비 구함
double CARcBridgeDataStd::GetPmaxColumn()
{
	if(IsDoro2010())
		return MaxRebar_Column_Doro2010;
	else
		return MaxRebar_Column;
}

void CARcBridgeDataStd::SyncBridgeFactor(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM)
{
	if(pBriOrg == NULL || pBriForBM == NULL) return;
	//ARCBRIDGE-2492 콘크리트 강도 싱크
	
	//// Fck ////
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fck[ix] = pBriOrg->m_Fck[ix];
	}

	//// Fy ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fy[ix] = pBriOrg->m_Fy[ix];
	}

	//// Fvy ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fvy[ix] = pBriOrg->m_Fvy[ix];
	}

	//// Ec ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Ec[ix] = pBriOrg->m_Ec[ix];
	}
}

BOOL CARcBridgeDataStd::MakeRcBridgeForBM( CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM, BOOL bForTogong )
{
	if(pBriOrg == NULL || pBriForBM == NULL) return FALSE;
	
	// 가각 이라면 무조건 슬래브는 자르지 않고 하나로 구함.
	if(pBriOrg->m_bIsExp && pBriOrg->m_nHeightForSlabBM == 0)
		pBriOrg->m_nHeightForSlabBM	= 1;

	//위치변경이후 이런 파일들
	if(pBriOrg->m_nDesPanel == -1 && pBriForBM->m_nDesPanel > -1)
		pBriOrg->m_nDesPanel = pBriForBM->m_nDesPanel;
	if(pBriOrg->m_nNamePanel == -1 && pBriForBM->m_nNamePanel > -1)
		pBriOrg->m_nNamePanel = pBriForBM->m_nNamePanel;

	*pBriForBM	= *pBriOrg;
	pBriForBM->m_pARcBridgeDataStd	= pBriOrg->m_pARcBridgeDataStd;
	pBriForBM->m_bBridgeForBM = TRUE;

	// 수평으로 그림. //////
	// Box형인 경우는 시종점 벽체길이를 평균으로 놓고 수평으로 만들어준다.
	pBriForBM->m_nTypePlacingMainRebar_UpperSlab	= 0;
	if(pBriForBM->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBriForBM->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ||
		pBriForBM->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		pBriForBM->m_nTypeJongdanForRebar	= 1;
	else
		pBriForBM->m_nTypeJongdanForRebar	= 0;

	// Box탕입일때는 토공수량도 
	if(bForTogong && pBriOrg->IsBoxType() == FALSE)
		pBriForBM->m_nTypeJongdanForRebar	= 1;	// 토공인 경우 무조건 경사로
	/////////////////////////



	// 벽체 기준 높이 옵션에 따라 평구배와 계획고 변경 /////////
	CLineInfo *pLine	= pBriForBM->GetLineBase();
	//*pLine	= *pBri->GetLineBase();
	if(pBriForBM->m_nTypeHeightWall == 0)	// 도로중심 기준 높이
	{
		// 그대로 사용하면 됨.
	}
	else if(pBriForBM->m_nTypeHeightWall == 1 || pBriForBM->m_nTypeHeightWall == 2 || pBriForBM->m_nTypeHeightWall == 3)	// 1 : 평균높이 기준(최대+최소)/2, 2 : 최대 계획고, 3 : 지점별 평균 높이
	{
		double dSta		= pBriOrg->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		CDPoint vAng	= pBriOrg->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		double dELMax	= pBriOrg->GetElMaxSlabUpper(dSta, vAng);
		double dELMin	= pBriOrg->GetElMinSlabUpper(dSta, vAng);
		double dEL		= pLine->GetElevationPlanEl(dSta) - pBriOrg->m_dTP;
		double dELUse	= pBriForBM->m_nTypeHeightWall == 1 || pBriForBM->m_nTypeHeightWall == 3 ? (dELMax+dELMin)/2 : dELMax;
		double dELDiff	= dEL - dELUse;

		long i = 0; for(i = 0; i < pLine->m_VIPsu; i++)
		{
			double dEL		= pLine->m_VIPData[i][1];
			pLine->m_VIPData[i][1]	= dEL - dELDiff;
		}

		// 모든 편구배를 0으로 맞춤
		// 좌
		for(i = 0; i < pLine->m_pSlopLeft.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopLeft.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// 우
		for(i = 0; i < pLine->m_pSlopRight.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopRight.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// 각 지점별 평균 높이인 경우
		// 시점의 평균 높이와 각 지점간의 평균 높이와의 차이를 기초하단 EL에 적용한다.
		if(pBriForBM->m_nTypeHeightWall == 3)
		{
			long nCountJijum	= pBriOrg->GetCountJijum();
			for(i = 1; i < nCountJijum; i++)
			{
				CFootingApp *pFooting	= pBriOrg->GetFootingByJijumNumber(i);
				if(!pFooting) continue;
				if(pFooting->IsVirtualFooting()) continue;
				double dStaJijum	= i == nCountJijum-1 ? pBriOrg->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE) : pBriOrg->GetStationOnJijum(i, 0);
				CDPoint vAngJIjum	= i == nCountJijum-1 ? pBriOrg->GetAngleByTarget(BRIDGE_OFF_END_SLAB,0) : pBriOrg->GetAngleJijum(i);

				double dELMaxJijum	= pBriOrg->GetElMaxSlabUpper(dStaJijum, vAngJIjum);
				double dELMinJijum	= pBriOrg->GetElMinSlabUpper(dStaJijum, vAngJIjum);
				double dELUseJijum	= (dELMaxJijum+dELMinJijum)/2;

				double dELDiff	= dELUseJijum - dELUse;
				
				// 종단을 level로 그리는 경우 종단경사에 의한 차이도 빼 줘야 한다.
				if(pBriForBM->m_nTypeJongdanForRebar == 0)
				{
					double dELDiffByJong	= pBriForBM->GetDiffELSttAndJijumForVertSection(i, FALSE);
					dELDiff -= dELDiffByJong;
				}
				////

				if(i == nCountJijum-1)
				{
					pBriForBM->m_dELEB -= dELDiff;
				}
				else
				{
					CWallApp *pWall	= pBriForBM->GetInWall(i-1);
					if(!pWall) continue;
					pWall->m_dELB -= dELDiff;
				}
				
			}
		}
	}

	if(pBriForBM->IsBoxType())
	{
		double dStaStt		= pBriOrg->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		double dStaEnd		= pBriOrg->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
		double dElStt = pLine->GetElevationPlanEl(dStaStt);
		double dElEnd = pLine->GetElevationPlanEl(dStaEnd);

		double dDiffH = dElEnd - dElStt;
		// 하단에서 가장 낮은 EL을 고정시키고 벽체 평균 높이를 만들어주자.
		if(pBriForBM->m_bSlopeLowerSlab)
		{
			// 시종점 벽체 높이는 같다. 하지만 EL이 시점 상단 기준일것이므로
			// 저점 기준으로 차이만금 내려주자.
			// 시점이 저점이라면 그냥 둬도 된다..
			if(dDiffH < 0)
			{
				long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
				{
					double dEL		= pLine->m_VIPData[ix][1];
					pLine->m_VIPData[ix][1]	= dEL + dDiffH;
				}
				pBriForBM->m_dELSB += dDiffH;
			}
		}
		else
		{
			// 시점 하단 EL은 그대로 두고 상단을 dDiffH/2 만큼 올리거나 내려 주기 위해서 선형을 이동시켜 주자는...
			long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
			{
				double dEL		= pLine->m_VIPData[ix][1];
				pLine->m_VIPData[ix][1]	= dEL + dDiffH/2.;
			}

			pBriForBM->m_bSlopeLowerSlab	= TRUE;
		}

		// 시점벽체 외측 EL을 기준으로 선형을 수평으로 펴준다.
		double dStastt1		= pLine->GetStationByOffsetWidth(pBriForBM->m_dStationBridgeStt, -pBriForBM->m_dWS, pBriForBM->GetAngleJijum(0));
		double dEL		= pLine->GetElevationPlanEl(dStastt1);
 		long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
 		{
 			pLine->m_VIPData[ix][1]	= dEL;
 		}

		pBriForBM->SyncBridge();
	}

	pBriForBM->m_bSettingVertSection	= FALSE;
	pBriForBM->InitSaveCrossSection();
	pBriForBM->InitSaveDistRigidEndSlab();
	pBriForBM->InitSaveDistRigidEndWall();
	pBriForBM->InitSaveOrgInWall();
	pBriForBM->InitSaveXyMatchLineAndSlabUpper();
	pBriForBM->InitSaveXySlabUpperByLenFrSttStation();
	SyncBridgeFactor(pBriOrg, pBriForBM);
	pBriForBM->SyncBridge(0, TRUE);
	pBriForBM->SettingVertSection();
	
	
	return TRUE;
}

double CARcBridgeDataStd::GetEs_Steel()
{
	double dEs_Steel = 2100000;  
	if(IsDoro2010() || IsRailDesign2017() || (m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE && m_nTypeKciCode == 2) || IsLsdDesign())
	{
		dEs_Steel = 2050000;
	}

	return  dEs_Steel;
}

void CARcBridgeDataStd::SerializeLsd( CArchive &ar )
{
	// m_dDesignRoadWidth 추가
	long nFlag(1);

	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_nTypeDesignMethod;
		ar << m_nLSDRoadType;
		ar << m_dDesignRoadWidth;

		m_plsdEnv->Serialize(ar);
		m_plsdRebar->Serialize(ar);
	}
	else
	{
		ar >> nFlag;

		ar >> m_nTypeDesignMethod;
		ar >> m_nLSDRoadType;
		if (nFlag > 0)
		{
			ar >> m_dDesignRoadWidth;
		}
		

		m_plsdEnv->Serialize(ar);
		m_plsdRebar->Serialize(ar);

		if(m_nTypeDesignMethod==DESIGN_METHOD_LSD)
		{
			m_nTypeKciCode = 2;
		}
	}
}

BOOL CARcBridgeDataStd::IsLsdDesign()
{
	if(IsLockRcBridgeLsdModule() && m_nTypeDesignMethod==DESIGN_METHOD_LSD)
		return TRUE;

	return FALSE;
}

void CARcBridgeDataStd::SetChangeDesignMethod()
{
	SetStringUnitType();
	BOOL bLSD = IsLsdDesign();

	m_pBasicConcInfo->SetDesignLsd(bLSD);

	if (bLSD)
	{
		m_nTypeConditionApply = DESIGN_CONDITION_ROAD;
		m_nTypeRoadCode = DESIGN_ROAD_2015;
		SetDataDefaultMatResistFactor();
		SetDataDefaultBaseResistFactor();
	}
	else
	{
		m_nTypeRoadCode = DESIGN_ROAD_2010;
		// 토질분류 - 암반은 한계상태에서만 지원
		long nSizeBorEx = m_pArrBoringEx.GetSize();
		for(long n=0; n<nSizeBorEx; n++)
		{
			CBoringDataEx *pBorEx = m_pArrBoringEx.GetAt(n);
			long nSizeBor = pBorEx->m_pArrBoring.GetSize();
			for(long nB=0; nB<nSizeBor; nB++)
			{
				CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(nB);
				if(pBor->m_nTypeSoil == 2)
				{
					pBor->m_nTypeSoil = 0; // 사질토
				}
			}
		}
	}

	long icBridge = GetBridgeSize();
	for(long ix=0; ix<icBridge; ++ix)
	{
		CRcBridgeRebar *pBri = GetBridgeRebar(ix);
		if(pBri == NULL) continue;

		// 설계환경 하중조합 기본값.
		if(bLSD)
		{
			long icJijum = pBri->GetCountJijum();
			for(long nJ=0; nJ<icJijum; ++nJ)
			{
				CFootingApp* pFoot = pBri->GetFooting(nJ);
				if(pFoot == NULL) continue;

				if(pBri->IsPileBase(nJ))
				{
					pFoot->m_nTypeEquation_Qa = 0;
				}
			}

			pBri->SetDataDefaultCondition();
			
			pBri->SetDataDefaultExposureRatingLsd();
			pBri->SetDataDefaultDesignRatingLsd();

			pBri->SetDataDefaultSelectLoadLsd();
			pBri->SetDataDefaultLoadFactor();
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE);
			pBri->SetDataDefaultModifiedFactor(-1);

			pBri->SetDataDefaultLoadFactor(FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT, FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME, FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE, FALSE);

			if (pBri->m_nUnderGroundDesignSelect == 4) // 교량설계기준(식)을 한계상태설계법에서 사용하지 않기로 함
			{
				pBri->m_nUnderGroundDesignSelect = 3;
			}

			pBri->m_bPileNvalueAuto = FALSE;
		}
		else
		{
			pBri->SetDataDefaultSelectLoad();
			pBri->SetDataDefaultCombineUltLoad();
			pBri->SetDataDefaultCombineUseLoad();
			pBri->SetDataDefaultCombineEccLoad();
			pBri->SetDataDefaultCombineEQLoad();
		}

		// (ARCBRIDGE-2489) 말뚝 수평방향 지반반력계수 인덱스가 서로 다르기 때문에 값을 날려 버린다.
		long nCntFoot = pBri->GetCountFooting();
		for(long ix=0; ix<nCntFoot; ++ix)
		{
			CFootingApp *pFoot = pBri->GetFooting(ix);
			if(pFoot == NULL) continue;

			for(long jx=0; jx<PILE_CASE_COUNT; ++jx)
				pFoot->m_exFooting.m_Pile.m_dKh_Input_Pile[jx] = 0.0;
		}
	}
}

void CARcBridgeDataStd::SetDataDefaultMatResistFactor()
{
	CLsdResistCoefMatCtl *pResistMat = &(m_plsdEnv->GetLsdResistCoefMatCtl());
	if(!pResistMat) return;

	m_plsdEnv->SetDataDefaultResistCoefMat();

	pResistMat->InitResistCoef();

	m_plsdEnv->GetLsdResistCoefMatCtl().SetUsed(eLSD_COMBINE_STRESS, FALSE);
}

void CARcBridgeDataStd::SetDataDefaultBaseResistFactor()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());
	if(!pResistCoefCtl) return;

	m_plsdEnv->SetDataDefaultResistCoefBase();

	pResistCoefCtl->InitResistCoefUsed_NotUsedAll();
	for(long ix = 0 ; ix <= eRESISTCOEF_AxialCompress_Composite ; ++ix)
	{
		pResistCoefCtl->SetUsed(ix, TRUE);
	}

	for(long ix = eRESISTCOEF_IntruStateBad ; ix <= eRESISTCOEF_Activity_Sandy ; ++ix)
	{
		pResistCoefCtl->SetUsed(ix, TRUE);
	}

	pResistCoefCtl->SetUsed(eRESISTCOEF_Activity_Rocky, TRUE);

	if(m_nTypeCheckSteelPile==1)
	{
		SetDataDefaultBaseResistFactor_SteelPile();
	}
}

void CARcBridgeDataStd::SetDataDefaultBaseResistFactor_SteelPile()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());
	if(!pResistCoefCtl) return;

	CLsdResistCoefBase *pStateBad = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_IntruStateBad);
	CLsdResistCoefBase *pStateGood = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_IntruStateGood);
	CLsdResistCoefBase *pStatePipe = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_NonDrivenPile_Pipe);
	CLsdResistCoefBase *pStateFlexure = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_NonDrivenPile_Flexure);

	BOOL b2014 = m_nTypeCheckSteelPile==1 ? TRUE : FALSE;

	pStateBad->SetUltimatelimit(b2014 ? 0.9 : 0.6);
	pStateGood->SetUltimatelimit(b2014 ? 0.9 : 0.7);
	pStatePipe->SetUltimatelimit(b2014 ? 0.9 : 0.8);
	pStateFlexure->SetUltimatelimit(b2014 ? 0.9 : 1.0);

	pStateBad->SetRemark(b2014 ? _T("강.설(2014) 5.1") : _T("도.설(2015) 6.5.4.2"));
	pStateGood->SetRemark(b2014 ? _T("강.설(2014) 5.1") : _T("도.설(2015) 6.5.4.2"));
	pStatePipe->SetRemark(b2014 ? _T("강.설(2014) 7.1.1") : _T("도.설(2015) 6.5.4.2"));
	pStateFlexure->SetRemark(b2014 ? _T("강.설(2014) 7.1.1") : _T("도.설(2015) 6.5.4.2"));

	pStateFlexure->SetCategory2(b2014 ? _T("휨에 대해 (Φb)") : _T("휨에 대해 (Φf)"));
}

// BOOL blong : N/㎟ 은 4자리 N/㎣은 6자리를 표기하고 있지만 값이 작은 경우 값을 다 표현 해주려면 6자리 9자리 까지 표시 해야 한다. 일부 값이 작은경우 사용하자.
CString CARcBridgeDataStd::GetStrDouble( double dValue, CString sUnitCGS, BOOL bLong, long nDec/*=-1*/, BOOL bDf, long nSpace, BOOL bExponent )
{
	BOOL bLsd = IsLsdDesign();

	if(bExponent == FALSE)
	{
		nDec = GetDecimal(sUnitCGS, nDec, bLong);
		if(bLsd && nDec == -1)
		{
			bExponent = TRUE;
		}
	}

	if(bExponent == TRUE)
	{
		nDec = 0;
		bDf = FALSE;
	}

	CString szRet = ::GetStrDouble(dValue, nDec, bLsd, bDf, bExponent);
	
	if(nSpace > 0)
	{
		if(nDec > 0)
			++nSpace;
		CString szCh(_T(""));
		long nLen = szRet.GetLength();
		for(long ix=0; ix< nSpace-nLen; ++ix)
			szCh += _T(" ");
		
		szRet = szCh + szRet;;
	}

	return szRet;
}

CString CARcBridgeDataStd::GetStrDoubleUnitChange( double dValue, CString csUnitOrg, CString csUnitChg, BOOL bLong, long nDec/*=-1*/, BOOL bDf, long nSpace, BOOL bExponent )
{
	double dValUnit = GetValueUnitChange(dValue, csUnitOrg, csUnitChg);

	return GetStrDouble(dValUnit, csUnitOrg, bLong, nDec, bDf, nSpace, bExponent);
}

long CARcBridgeDataStd::GetDecimal(CString sUnitCGS, long nDec, BOOL bLong)
{
	BOOL bLsd = IsLsdDesign();

	if(nDec < 0 || bLsd)
	{
		if(sUnitCGS == UNIT_CGS_TONF || sUnitCGS == UNIT_CGS_M || sUnitCGS == UNIT_CGS_TONFM || sUnitCGS == UNIT_CGS_M2)
			nDec = bLsd? 0 : 3;
		else if (sUnitCGS == UNIT_CGS_TONF_M)
			nDec = 3;//bLsd? 3 : 3;
		else if(sUnitCGS == UNIT_CGS_TONF_M2)
			nDec = bLsd? (bLong? 6 : 4) : 3;
		else if(sUnitCGS == UNIT_CGS_TONF_M3 || sUnitCGS == UNIT_CGS_KGF_CM3)
			nDec = bLsd? (bLong? 7 : 6) : 3;
		else if (sUnitCGS == UNIT_CGS_KGF_CM2 || sUnitCGS == UNIT_CGS_KGF_CM2)
			nDec = bLsd? (bLong? 6 : 4) : 3;
		else if (sUnitCGS == UNIT_CGS_KGF_M3)
			nDec = bLsd? (bLong? 7 : 6) : 3;
		else if (sUnitCGS == UNIT_CGS_M4)
			nDec = bLsd? -1 : 6;
		else
			nDec = 0;
	}	
	
	return nDec;
}

CString CARcBridgeDataStd::GetStringLoadDB( BOOL bDL/*=FALSE*/ )
{
	CString szDB(_T(""));
	BOOL bLsd = IsLsdDesign();
	if(bLsd)
	{
		szDB = _T("KL-510");
		if(m_nDegreeBridge==1)
		{
			szDB = _T("KL-382.5");
		}
		if(m_nDegreeBridge==2)
		{
			szDB = _T("KL-286.87");
		}
	}
	else
	{
		szDB = bDL? _T("DL-24") : _T("DB-24");
		if(m_nDegreeBridge==1)
		{
			szDB = bDL? _T("DL-18") : _T("DB-18");
		}
		if(m_nDegreeBridge==2)
		{
			szDB = bDL? _T("DL-13.5") : _T("DB-13.5");
		}
	}

	return szDB;
}

CString CARcBridgeDataStd::GetStringExcelFileForm()
{
	CXLControl excel;
	double dVersion = excel.GetVersion();

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel files (*.xls)|*.xls||") : _T("Microsoft Excel files (*.xlsx)|*.xlsx||");

	return sFileForm;
}

BOOL CARcBridgeDataStd::IsApplyBoredPileASD(CExPileApp *pPile)
{
	if(m_nApplyBoredPileASD == 0) return FALSE;

	if(pPile)
	{
		return pPile->IsBoredPile();
	}
	else
	{
		// pPile을 지정하지 않았을때는 선택변수만 체크 하기 때문에 TRUE준다.
		return TRUE;
	}
}

BOOL CARcBridgeDataStd::IsApplyPhcPileASD( CExPileApp *pPile /*= NULL*/ )
{
	if(m_nApplyPhcPileASD == 0) return FALSE;

	if(pPile)
	{
		return pPile->IsPHC();
	}
	else
	{
		return TRUE;
	}
}

//nBri == -1(모든 교량), nBri
void CARcBridgeDataStd::SyncApsDowelRebarSeperateRebarOption(BOOL bSet,  long nBri)
{
	if(nBri == -1)
	{
		long nCntBirSize = GetBridgeSize();
		for(long nBri =0; nBri < nCntBirSize; nBri++)
		{
			CRcBridgeApp *pBriApp = GetBridge(nBri);
			if(pBriApp == NULL) continue;

			pBriApp->m_bApsDowelSeparateRebar = bSet;
		}
	}
	else
	{
		CRcBridgeApp *pBriApp = GetBridge(nBri);
		if(pBriApp)
			pBriApp->m_bApsDowelSeparateRebar = bSet;
	}	
}

void CARcBridgeDataStd::SeparateRebarListByDescription(CTypedPtrArray <CObArray, CRebar*> *pArrRebar1, CTypedPtrArray <CObArray, CRebar*> *pArrRebar2, CString sDescription)
{
	if(!pArrRebar1 || !pArrRebar2)	return;

	if(pArrRebar1->GetSize() == 0)	return;
	if(sDescription == _T(""))	return;

	CRebar *pRB = NULL;
	for(long i = 0; i < pArrRebar1->GetSize(); i++)
	{
		pRB = pArrRebar1->GetAt(i);

		if(pRB->m_sDescription.Find(sDescription) == -1)
			continue;

		pArrRebar2->Add(new CRebar(pRB));

		delete pRB;
		pArrRebar1->RemoveAt(i);
		i--;
	}
}

void CARcBridgeDataStd::SetFooting_Qa_Pa__Calculate()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());

	ECoefficientBaseIndex eIndex[3] = {eResistCoefficient_Service, eResistCoefficient_Ultimate, eResistCoefficient_Extreme};

	for(long nBri=0; nBri < GetBridgeSize(); nBri++)
	{
		CRcBridgeApp *pBri = GetBridge(nBri);
		if(pBri == NULL) continue;

		for(long nJijum = 0; nJijum <= pBri->m_nQtyJigan; nJijum++)
		{
			CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
			if(pFooting == NULL) continue;

			for(long nCase=0; nCase<3; nCase++)
			{
				double piR = 1.0, piU = 1.0;
				if(pBri->IsPileBase(nJijum))
				{
					CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
					if(pPile->m_nType == PILE_TYPE_HYUNJANG)
					{
						CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
						if(pBoring)
						{
							piR = pResistCoefCtl->GetCoefficient((pBoring->m_nTypeSoil == 0) ? eRESISTCOEF_SinglePileCompress_Drill_Support_Sandy : eRESISTCOEF_SinglePileCompress_Drill_Support_Rock, eIndex[nCase]);
						}
						piU = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Drill, eIndex[nCase]);
					}
					else// 타입말뚝
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePileCompress_Type, eIndex[nCase]);
						piU = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Type, eIndex[nCase]);
					}
				}
				else
				{
					if(pFooting->m_nTypeBaseGround != 2)
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Bearing_Sandy, eIndex[nCase]);	// (Φb)	사질토, SPT 사용
					}
					else
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Bearing_Rock, eIndex[nCase]);	// (Φb)	암반위에 설치된 기초
					}
				}
				pFooting->m_Qa_Calculate[nCase] = pFooting->m_Qa_Calculate[0] * piR;
				pFooting->m_Pa_Calculate[nCase] = pFooting->m_Pa_Calculate[0] * piU;
			}
		}
	}
}

BOOL CARcBridgeDataStd::IsKRL_2012()
{
	if(IsRailDesign2017() == FALSE) return FALSE;

	if(m_nTypeRailLoad==RAIL_LOAD_EL18_2017) return FALSE;

	if(m_nTypeRailLoad==RAIL_LOAD_KRL2012) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_KRL2012_P) return TRUE;

	if((m_nTypeRailLoad==RAIL_LOAD_USER_2017 && m_DistributionLoadFront>0))
		return TRUE;

	return FALSE;
}

CString CARcBridgeDataStd::GetStringBridgeDir(long nBri)
{
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(!pBri) return _T("제목없음");

	CString strBridgeDir = _T("");
	if(pBri->m_nSeparBri==0)
		strBridgeDir = m_strLineDirName + m_strLineRevDirName;
	else if(pBri->m_nSeparBri==1)
		strBridgeDir = m_strLineDirName;
	else if(pBri->m_nSeparBri==2)
		strBridgeDir = m_strLineRevDirName;

	return strBridgeDir;
}

CString CARcBridgeDataStd::GetStringTypeCoefficientGround( long nType )
{
	CString szType = _T("");

	switch (nType)
	{
	case 0:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("Ⅰ") : _T("S1");	break;
	case 1:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("Ⅱ") : _T("S2");	break;
	case 2:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("Ⅲ") : _T("S3");	break;
	case 3:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("Ⅳ") : _T("S4");	break;
	case 4:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("") : _T("S5");	break;
	default:
		szType = _T("");
	}

	return szType;
}

long CARcBridgeDataStd::GetTypeConcDesign()
{
	long nType = IsLsdDesign() ? DESIGN_TYPE_LSD : DESIGN_TYPE_LIMIT;
	if(IsDesignKDS2019())
	{
		nType = IsLsdDesign() ? DESIGN_TYPE_LSD_KDS_2019 : DESIGN_TYPE_LIMIT_KDS_2016;
	}

	return nType;
}
