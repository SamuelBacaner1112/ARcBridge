// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutBM.h: interface for the COutBM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_)
#define AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 대분류
#define BM_BIG_BOHOBLOCK		0	// 교대보호블럭(콘크리트보다 위에 있는 이유 : 보호블럭 뒷채움 콘크리트 = 보호블럭 * 콘크리트두께)
#define BM_BIG_CONCRETE			1	// 콘크리트
#define BM_BIG_FORM					2	// 거푸집
#define BM_BIG_POST					3	// 동바리
#define BM_BIG_DECKFINISH			4	//데크피니셔 (동바리-다웰바사이에 끼워짐, ARCBRIDGE-3132)
#define BM_BIG_DOWELBAR			5	// 다웰바
#define BM_BIG_SCAFFOLD			6	// 비계
#define BM_BIG_SPACER				7	// 스페이셔
#define BM_BIG_SLABCARE			8	// 슬래브양생
#define BM_BIG_CONCCARE			9	// 콘크리트양생
#define BM_BIG_ASPHALTPROOF		10	// 아스팔트 방수
#define BM_BIG_REBAR				11	// 철근가공조립
#define BM_BIG_SURFACE				12	// 면 고르기
#define BM_BIG_ASCON				13	// 아스콘 포장
#define BM_BIG_SAND					14	// 모래 포장
#define BM_BIG_PERMEABLE			15	// 투스콘 포장
#define BM_BIG_TACKCOTTING		16	// 택 코팅
#define BM_BIG_SURFACEPROOF		17	// 교면 방수
#define BM_BIG_ANTIDROPPING		18	// 낙하물 방지
#define BM_BIG_HYUNTA				19	// 현장타설말뚝
#define BM_BIG_NAMEPANEL		20	// 교명판
#define BM_BIG_DESPANEL			21	// 설명판
#define BM_BIG_TBM					22	// TBM설치
#define BM_BIG_STYROFOAM			23	// 스티로폼
#define BM_BIG_FLEXIBLEJOINT		24	// 신축이음
#define BM_BIG_FLEXIBLEJOINT_COVER	25	// 신축이음 덮개
#define BM_BIG_SHRINKAGE			26	// 무수축 콘크리트
#define BM_BIG_COVERPLATE		27	// cover plate
#define BM_BIG_MONUMENT		28	// 교명주
#define BM_BIG_NOTCH				29	// NOTCH
#define BM_BIG_PILE					30	// 말뚝
#define BM_BIG_SCAFFOLD_LEG		31	// 비계다리 
#define BM_BIG_RUBBLEFILL			32	// 함형(Box) 잡석채움
//#define BM_BIG_DECKFINISH			32	//데크피니셔
#define BM_BIG_COUNT				33

// 중분류
#define BM_MID_CONCRETE_MAIN	0	// 콘크리트 (본체, 날개벽, 접속슬래브)
#define BM_MID_CONCRETE_LEAN	1	// 콘크리트 (버림콘크리트)
#define BM_MID_CONCRETE_MASS	2	// 콘크리트 (mass)
#define BM_MID_CONCRETE_BOHO	3	// 컨크리트 (보호블럭)
#define	BM_MID_FORM_3TIMES		4	// 매끈한 마감
#define BM_MID_FORM_PATTERN		5	// 무늬 거푸집
#define BM_MID_FORM_GANGJE		6	// 강재 거푸집
#define	BM_MID_FORM_PATTERN_S	7	// 설명된 특징있는 마감 (무늬거푸집) 
#define BM_MID_FORM_GANGJE_S	8	// 설명된 특징있는 마감 (강재거푸집)
#define BM_MID_FORM_COTTING_S	9	// 설명된 특징있는 마감 (코팅거푸집)
#define BM_MID_FORM_DECK_S		10	// 설명된 특징있는 마감 (DECK PLATE)
#define BM_MID_FORM_ONE_S		11	// 설명된 특징있는 마감 (1회 사용)
#define	BM_MID_FORM_CIRCLE		12	// 원형거푸집
#define	BM_MID_FORM_4TIMES		13	// 보통마감
#define BM_MID_FORM_6TIMES		14	// 거친마감
#define BM_MID_DOWELBAR			15	// 다웰바
#define BM_MID_SCAFFOLD_MAIN	16	// 비계(본체)
#define BM_MID_SCAFFOLD_WING	17	// 비계(날개벽)
#define BM_MID_SLABCARE			18	// 슬래브 양생
#define BM_MID_CONCCARE			19	// 콘크리트양생
#define BM_MID_REBAR			20	// 철근가공조립
#define BM_MID_ASCON			21	// 아스콘 포장
#define BM_MID_TACK_RSC1		22	// 택 코팅(RSC-1)
#define BM_MID_SURFACEPROOF_PERCOLATE	23	// 교면방수(침투식)
#define BM_MID_ASPHALTPROOF		24	// 아스팔트 방수
#define BM_MID_STYROFOAM		25	// 스티로폼
#define BM_MID_FORM_EURO		26	// 유로폼
#define BM_MID_FLEXIBLEJOINT	27	// 신축이음
#define BM_MID_SHRINKAGE		28	// 무수축 콘크리트
#define BM_MID_SCAFFOLD_WING_LEG	29	// 비계다리(날개벽) 
#define BM_MID_SCAFFOLD_MAIN_LEG	30	// 비계다리(본체) 
#define BM_MID_FLEXIBLEJOINT_COVER	31	// 신축이음덮개
#define BM_MID_RUBBLEFILL	32	//함형(Box) 잡석채움
#define BM_MID_CONCRETE_GUIDEWALL	33	//방호벽
#define BM_MID_CONCRETE_GWHALF	34	//중분대
#define BM_MID_CONCRETE_MONU	35	//콘크리트 교명주

// 여기서 부터 현타 말뚝
#define BM_MID_STEEL_INNER_DIGGING_BIG	32	// 강관내부굴착(대구경천공)
#define BM_MID_STEEL_INNER_DIGGING_RCD	33	// 강관내부굴착(RCD천공)
#define BM_MID_DIGGING_MACHINE_MOVE_DEPER	34	// 굴착장비 이동및 거치
#define BM_MID_PILE_BUILD	35				// 말뚝 조성공
#define BM_MID_SPACER_ALONE_HYUNTAPILE	36	// SPACER 제작 및 설치
#define BM_MID_REBAR_ANTI_RISE	37			// 철근 오름 방지
#define BM_MID_COPPING	38					// 두부 정리
#define BM_MID_REMAIN_SOIL	39				// 잔토처리
#define BM_MID_REBAR_ASSM_AHP	40			// 철근가공조립
#define BM_MID_WASTE	41					// 폐기물 처리
#define BM_MID_SLIME	42					// slime 제거
#define BM_MID_CONCRETE_AHP	43				// concrete 타설
#define BM_MID_SURFACE_AHP	44				// 면 정리
#define BM_MID_REBAR_NET	45				// 철근망 근입
// 여기까지 현타 말뚝


// 소분류
#define BM_SMALL_CONCRETE_FOOTING	0	// 콘크리트 (기초)
#define BM_SMALL_CONCRETE_MAIN		1	// 콘크리트 (본체)
#define BM_SMALL_CONCRETE_LEAN		2	// 콘크리트 (버림콘크리트)
#define BM_SMALL_CONCRETE_MASS		3	// 콘크리트 (MASS 콘크리트)
#define BM_SMALL_CONCRETE_WALL		4	// 콘크리트 (벽체)
#define BM_SMALL_CONCRETE_WING		5	// 콘크리트 (날개벽)
#define BM_SMALL_CONCRETE_APS		6	// 콘크리트 (접속슬래브)
#define BM_SMALL_CONCRETE_CNS		7	// 콘크리트 (완충슬래브)
#define BM_SMALL_CONCRETE_BOHO_FOOTING	8	// 콘크리트 (보호블럭 기초)
#define BM_SMALL_CONCRETE_BOHO_BACKFILL	9	// 콘크리트 (보호블럭 뒷채움)
#define BM_SMALL_FORM_VERTICAL		10	// 거푸집 (수직면)
#define BM_SMALL_FORM_SLOPE			11	// 거푸집 (경사면)
#define BM_SMALL_FORM_CURVE			12	// 거푸집 (곡면)
#define BM_SMALL_FORM_HORI			13	// 거푸집 (수평면)
#define BM_SMALL_POST_STEELPIPE		14	// 동바리(강관)
#define BM_SMALL_POST_WOOD			15	// 동바리(목재)
#define BM_SMALL_POST_BRIDGE_STEEL	16	// 동바리(육교용(강재))
#define BM_SMALL_POST_HORI_CONNECT	17	// 동바리수평연결재
#define BM_SMALL_DOWELBAR			18	// 다웰바
#define BM_SMALL_SCAFFOLD_MAIN		19	// 비계(본체)
#define BM_SMALL_SCAFFOLD_WING		20	// 비계(날개벽)
#define BM_SMALL_SPACER_WALL		21	// 스페이셔(벽체용)
#define BM_SMALL_SPACER_SLAB		22	// 스페이셔(슬래브용)
#define BM_SMALL_CONCCARE			23	// 콘크리트양생
#define BM_SMALL_ASPHALTPROOF		24	// 아스팔트 방수
#define BM_SMALL_REBAR_SIMPLE		25	// 철근가공조립(간단)
#define BM_SMALL_REBAR_NORMAL		26	// 철근가공조립(보통)
#define BM_SMALL_REBAR_COMPLEXITY1	27	// 철근가공조립(복잡)
#define BM_SMALL_REBAR_COMPLEXITY2	28	// 철근가공조립(매우복잡)
#define BM_SMALL_STYROFOAM			29	// 스티로폼
#define BM_SMALL_FLEXIBLEJOINT		30	// 신축이음
#define BM_SMALL_SHRINKAGE			31	// 무수축 콘크리트
#define BM_SMALL_SCAFFOLD_WING_LEG 32	// 비계다리(날개벽)
#define BM_SMALL_SCAFFOLD_MAIN_LEG		33	// 비계다리(본체)
#define BM_SMALL_FJ_NONE				34  // 차도	
#define BM_SMALL_FJ_GUARDFENCE			35	// 방호벽
#define BM_SMALL_FJ_CENTERGUARD			36	// 중분대
#define BM_SMALL_FJ_CENTERGUARDHALF		37	// 중분대1/2
#define BM_SMALL_FJ_FOOTWAY				38	// 보도
#define BM_SMALL_FJ_FOOTWAYBLOCK		39	// 보차도 경계석
#define BM_SMALL_FJ_SOUNDPROOF			40	// 방음벽 기초
#define BM_SMALL_FJ_RETAININGWALL		41	// 흙막이벽
#define BM_SMALL_FJ_HANDRAIL			42	// 난간기초
#define BM_SMALL_FJ_DIKE				43	// 다이크
#define BM_SMALL_RUBBLEFILL				44	// 함형(Box) 잡석채움

#define BM_SMALL_CONCRETE_FLOOR			50	// 바닥 및 보

/*
#define BM_SMALL_REBAR1_10			21	// 철근가공조립(연강 10)
#define BM_SMALL_REBAR1_13			22	// 철근가공조립(연강 13)
#define BM_SMALL_REBAR1_1625		23	// 철근가공조립(연강 16~25)
#define BM_SMALL_REBAR1_2935		24	// 철근가공조립(연강 29~35)
#define BM_SMALL_REBAR2_10			25	// 철근가공조립(고강 10)
#define BM_SMALL_REBAR2_13			26	// 철근가공조립(고강 13)
#define BM_SMALL_REBAR2_1625		27	// 철근가공조립(고강 16~25)
#define BM_SMALL_REBAR2_2935		28	// 철근가공조립(고강 29~35)
*/

// 아이템 분류
#define BM_ITEM_BOHOBLOCK_SF		0	// 보호블럭(시점전면)
#define BM_ITEM_BOHOBLOCK_SS		1	// 보호블럭(시점측면)
#define BM_ITEM_BOHOBLOCK_EF		2	// 보호블럭(종점전면)
#define BM_ITEM_BOHOBLOCK_ES		3	// 보호블럭(종점측면)
#define BM_ITEM_CONCRETE_FOOTING	4	// 콘크리트 (기초)
#define BM_ITEM_CONCRETE_MAIN		5	// 콘크리트 (본체)
#define BM_ITEM_CONCRETE_GUARDWALL	6	// 콘크리트 (방호벽)
#define BM_ITEM_CONCRETE_WING_SL	7	// 콘크리트 (날개벽시점좌측)
#define BM_ITEM_CONCRETE_WING_SR	8	// 콘크리트 (날개벽시점우측)
#define BM_ITEM_CONCRETE_WING_EL	9	// 콘크리트 (날개벽종점좌측)
#define BM_ITEM_CONCRETE_WING_ER	10	// 콘크리트 (날개벽종점우측)
#define BM_ITEM_CONCRETE_APS 		11	// 콘크리트 (접속슬래브)
#define BM_ITEM_FORM_WING_SLF		12	// 거푸집 (날개벽시점좌측 전면)
#define BM_ITEM_FORM_WING_SLB		13	// 거푸집 (날개벽시점좌측 배면)
#define BM_ITEM_FORM_WING_SRF		14	// 거푸집 (날개벽시점우측 전면)
#define BM_ITEM_FORM_WING_SRB		15	// 거푸집 (날개벽시점우측 배면)
#define BM_ITEM_FORM_WING_ELF		16	// 거푸집 (날개벽종점좌측 전면)
#define BM_ITEM_FORM_WING_ELB		17	// 거푸집 (날개벽종점좌측 배면)
#define BM_ITEM_FORM_WING_ERF		18	// 거푸집 (날개벽종점우측 전면)
#define BM_ITEM_FORM_WING_ERB		19	// 거푸집 (날개벽종점우측 배면)

class CBMData;
class CBMString;
class CBMSumString;
class CRebarBMData;
class CRebarBMString;
class CRcBridgeRebar;
class CGuardWallRC;

#define MAX_JIJUM_COUNT_RC 21
#define CON_DECKFINISHER 0.585 

const long nMaxQtyHdan = 30;	// MAX_QTY_HDAN
const long nMaxQtyHyunta = 100;	// MAX_QTY_Hyunta

class AFX_EXT_CLASS COutBM : public COutBMBase  
{
public:
	void MakeBMFormGuardWall();
	void DimTextFormOfSlabELCalc(CDomyun *pDom);
	void MakeBMNotch();
	void MakeBMMonument();
	long DrawFirstPageMonument(CHgBaseBMStd *pBaseBMStd);
	void MakeBMFormMonument();
	void MakeBMConcreteMonument();
	void MakeBMCoverPlate();
	void MakeBMNonShrinkageConc();
	void MakeBMFlexibleJoint();
	void MakeBMFlexibleJointCover();	//신축이음 덮개
	void DimTextLengthOfGuardWall(CDomyun *pDomP, CGuardWallRC *pGW, long nIdx, long nHDan);
	void DimTextArchRibConcAndForm(CDomyun *pDomP);
	void DimTextPostAreaOfArch(CDomyun *pDomP);
	double GetElBaseForForm(long nJ);
	void MakeBMAsphaltProof();
	void MakeBMRebar();
	void MakeBMRebarTable();
	long GetFormByPos(long nPos);
	// 함수 삭제 DataStd로 이동
	// void MakeRcBridgeForBM(BOOL bForTogong=FALSE);	// 수량을 구하기 위한 새로운 교량 생성
	void MakeBMPostWing();
	void MakeBMConcBohoBackFill();
	void MakeBMBohoBlock();
	void MakeBMConcBohoFooting();
	// 수량 생성
	void MakeBMConcreteMain(BOOL bForDeductOfReFill=FALSE);		// 콘크리트(본체)
	void MakeBMConcreteGuardWall();								// 콘크리트(방호벽)
		void MakeBMConcreteAps(BOOL bForDeductOfBackFill=FALSE, CTwinVector tvBackFillStt=CTwinVector(CVector(0, 0, 0), CVector(0, 0, 0)), CTwinVector tvBackFillEnd=CTwinVector(CVector(0, 0, 0), CVector(0, 0, 0)));	// 콘크리트(접속슬래브)
	void MakeBMFormMain();			// 거푸집(본체)
	void MakeBMPostMain();			// 동바리(본체)
	void MakeBMConcreteWing(long nForUse=0);		// 콘크리트(날개벽)
	void MakeBMHoriConnectMain();	// 동바리수평연결재(본체)
	void MakeBMHoriConnectWing();	// 동바리수평연결재(날개벽)
	void MakeBMDowelBar();			// 다웰바(접속슬래브)
	void MakeBMScaffoldMain();		// 비계(본체)
	void MakeBMScaffoldWing();		// 비계(날개벽)
	void MakeBMPile();
//	void MakeBMAntiDropping();
	void MakeBMSlabCare();
	void MakeBMSurfaceProof();
	void MakeBMTackCotting();
	void MakeBMAscon();
	void MakeBMSurface();
	void MakeBMRebar(long nType);
	void MakeBMConCareMain();
	void MakeBMConCareWing();
	void MakeBMSpacerMain();
	void MakeBMSpacerWing();
	void MakeBMFormWing();
	void MakeBMConCareAps();
	void MakeBMSpacerAps();
	void MakeBMStyrofoamAps();
	void MakeBMFormAps();
	void MakeBMTBM();
	void MakeBMDesPanel();
	void MakeBMNamePanel();
	void SetBMRebar(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nTypeComplex, long bIncludeFlexibleJointRebar=FALSE, CString sNote=_T(""));
	void MakeBMScaffoldLegWing(); //31724 //비계다리(날개벽)
	void MakeBMScaffoldLegMain(); //비계다리 (본체)
	void SetInsertDomyunStd();
	void MakeRubbleBM();//잡석기초 (일반 수량): 
	void MakeBMDeckFinisher(); //데크피니셔
	
	void CalculateBM(BOOL bNewCreateBM = TRUE);
	void ClearBM(BOOL bRemainStdBM=FALSE);
	void MakeBMStringStd();
	void MergeBMStd();
	COutBM(long nBri);
	virtual ~COutBM();

	// 수량
	DWORD m_dwCalBM;	// 산출되는 수량의 종류

	// 수집용(정제안된 순수 수량)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeStd;			// 교대본체(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallStd[nMaxQtyHdan];		// 방호벽(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonumentStd;			// 교명주(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingStd[2][2];		// 교대날개벽(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsStd[2][2];		// 접속슬래브(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsStd[2][2][2];	// 완충슬래브(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoStd[2];			// 보호블럭(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMHyuntaStd[nMaxQtyHyunta];	 // 현장타설(정제 안된것)

	// 출력용(정제된 수량)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridge;					// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBridge;			// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWall[nMaxQtyHdan];				// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringGuardWall[nMaxQtyHdan];		// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonument;				// 교명주(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringMonument;		// 교명주(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWing[2][2];				// 교대날개벽(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringWing[2][2];		// 교대날개벽(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMAps[2][2];				// 접속슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringAps[2][2];		// 접속슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCns[2][2][2];			// 완충슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringCns[2][2][2];	// 완충슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBoho[2];				// 보호블럭(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBoho[2];		// 보호블럭(정제 된것) - 산출근거 출력


	// 부위별집계표에 출력된 내용
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeForPartSum;		// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallForPartSum[nMaxQtyHdan];	// 교대본체(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonumentPartSum;				// 교명주(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingForPartSum[2][2];	// 교대날개벽(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsForPartSum[2][2];	// 접속슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsForPartSum[2][2][2];	// 완충슬래브(정제 된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoForPartSum[2];		// 보호블럭(정제 된것) - 산출근거 출력
	
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeSum;				// 교대본체 - 교량별 집계표 출력 
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBridgeSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallSum;			// 방호벽 - 교량별 집계표 출력 - 교명주 포함
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringGuardWallSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingSum;				// 교대날개벽 - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringWingSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsSum;					// 접속슬래브 - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringApsSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsSum;					// 완충슬래브 - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringCnsSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoSum;				// 보호블럭 - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBohoSum;

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAllSum;		// 교대본체(본체+날개벽+접속슬래브) - 교량별 집계표 출력 
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSum;					// 교량집계(정제 안된것) - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSumForTitle;			// 교량집계(정제 안된것) - 교량별 집계표 출력 - 타이틀용

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringSepBridgeSum;	// 상하행 분리 교량 집계표 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSepBridgeSum;


	// 철근집계표
	CBMRebarTable m_rebarTableBridge;							// 본체 철근 집계표
	CBMRebarTable m_rebarTableGuardWall[nMaxQtyHdan];		// 방호벽 철근집계표
	CBMRebarTable m_rebarTableMoument;						// 교명주 철근집계표
	CBMRebarTable m_rebarTableWing[2][2];						// 날개벽 철근집계표
	CBMRebarTable m_rebarTableAps[2][2];							// 접속슬래브 철근집계표
	CBMRebarTable m_rebarTableCns[2][2][2];						// 완충슬래브 철근집계표
	CBMRebarTable m_rebarTableBoho[2];							// 보호블럭 철근집계표
	CBMRebarTable m_rebarTableHuynta[nMaxQtyHyunta];		// 현장타설말뚝 철근집계표

	CBMRebarTable m_rebarTableBridgeSum;			// 본체 총괄 철근 집계표
	CBMRebarTable m_rebarTableGuardWallSum;		// 방호벽 총괄 철근집계표
	CBMRebarTable m_rebarTableMoumentSum;		// 교명주 총괄 철근집계표
	CBMRebarTable m_rebarTableWingSum;			// 날개벽 총괄 철근집계표
	CBMRebarTable m_rebarTableApsSum;				// 접속슬래브 총괄 철근집계표
	CBMRebarTable m_rebarTableCnsSum;				// 완충슬래브 총괄 철근집계표
	CBMRebarTable m_rebarTableBohoSum;			// 보호블럭 총괄 철근집계표
	CBMRebarTable m_rebarTableHuyntaSum;			// 현장타설말뚝 총괄 철근집계표

	CBMRebarTable m_rebarTableAll;				// 총괄 철근집계표

	CBMRebarTable m_rebarSepBridgeSum;			// 상하행 분리 교량 집계표

	// 철근집계
	// D바 H바 따로 집계함.
	// 옵션에 따라서 같이 할 수도 있음.
	// 0 : D바, 1 : H바
	void SetRebarBMData(CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataD, CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataH, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sNameBigClass);
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBridgeStd[2];			// 교대본체(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMGuardWallStd[nMaxQtyHdan][2];		// 방호벽(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMMonument[2];			// 교명주(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMWingStd[2][2][2];		// 교대날개벽(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMApsStd[2][2][2];		// 접속슬래브(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMCnsStd[2][2][2][2];	// 완충슬래브(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBohoStd[2][2];		// 보호블럭(정제 안된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMHyuntaStd[nMaxQtyHyunta][2];		// 현장타설(정제 안된것)

	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBridge[2];			// 교대본체(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMGuardWall[nMaxQtyHdan][2];			// 방호벽(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMMonumentStd[2];			// 교명주(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMWing[2][2][2];		// 교대날개벽(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMAps[2][2][2];			// 접속슬래브(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMCns[2][2][2][2];		// 완충슬래브(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBoho[2][2];			// 보호블럭(정제 된것)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMHyunta[nMaxQtyHyunta][2];			// 현장타설(정제 된것)

	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringBridge[2];			// 교대본체
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringGuardWall[nMaxQtyHdan][2];			// 방호벽
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringMonument[2];			// 교명주
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringWing[2][2][2];		// 교대날개벽
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringAps[2][2][2];			// 접속슬래브
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringCns[2][2][2][2];		// 완충슬래브
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringBoho[2][2];			// 보호블럭
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringHyunta[nMaxQtyHyunta][2];			// 현장타설

	CARcBridgeOutBMStd* m_pStd;

	// 삽도 출력에 관한 옵션들 /////////////////////////////////////////
	double m_dScaleDim_VertSection;		// 종단면도 스케일
	double m_dHeightText_VertSection;	// 종단면도 텍스트 높이
	////////////////////////////////////////////////////////////////////
	void DrawFirstPageMain(CHgBaseBMStd *pBaseBMStd, long nKindBM);
	long DrawFirstPageGuardWall(CHgBaseBMStd *pBaseBMStd, long nHDan, long nIdx);
	void DrawFirstPageWing(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft);
	void DrawFirstPageAps(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft);

	void CalAreaConc(CRcBridgeRebar *pBri, BOOL bForDeductOfReFill);
	void CalAreaConcWing(CRcBridgeRebar *pBri, long nForUse=0);

	CString GetStringBigClass(long nIdxBigClass);

private:
	void MakeBMFormBoho();

	void SetInsertDomyunScaffoldMain();
	
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// 임의로 사용 할 tvAreaShape

	long GetIdxSameBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CBMData *pBM, DWORD exceptionFlag);
	void GetInfoPosAreaConc(CTwinVectorAreaShape *pArea, long &nTypeLeft, long &nTypeRight);
	void GetInfoMeter(CBMData *pBM, double &dFirstMeter, double &dSecondMeter);
	BOOL IsPrintMeter(CBMData *pBM);
	void RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dFirstMeter, double dSecondMeter, long nIdxStt=-1, long nIdxEnd=-1, long nOpt=0);
	CString GetStringMidClass(long nIdxMidClass, CApsApp *pAps=NULL, long nTypeOfConcrete=0, CGuardWallRC *pGW=NULL, double dUserFck = 0);
	CString GetStringSmallClass(long &nIdxSmallClass, BOOL bStt=TRUE);
	CString GetStringItemClass(long nIdxItemClass);

	long GetLevelTotal(long nIdxBigClass);

	void SetInsertDomyunPlaneFooting();
	void SetInsertDomyunPost();
	void SetInsertDomyunCross();
	
	long GetTvAreaShapePostWing(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, long nBaseNum);
	long GetTvAreaShapePostBracket(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, long nBaseNum);
	long GetTvAreaShapePostJigan(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nJigan, long nBaseNum);
	long GetTvAreaShapeApsPlane(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, BOOL bLean=FALSE);
	long GetTvAreaShapeApsSection(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft);
	
	CRcBridgeRebar* GetRcBridgeRebar(BOOL bOrg=FALSE);
	
	void SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, BOOL bNewCreateBM = TRUE);
	void ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM);

	CBMData *CreateBMStyrotormForSide(CBMData *pBMForm);
	void AddBMStyroformBySide(CBMData *pBMForm, DWORD nSide);
	
	long m_nBri;
};

#endif // !defined(AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_)
