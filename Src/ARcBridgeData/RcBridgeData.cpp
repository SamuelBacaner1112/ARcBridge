// RcBridgeData.cpp: implementation of the CRcBridgeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRcBridgeData::CRcBridgeData()
{
	long i(0), left(0), cns(0), stt(0);
	
	m_nIdxBridge = -1;

	// 여기서 부터는 저장되는 변수들
	m_nFlag = 0;

	// 교량 형식 및 제원
	m_strBridgeName	= _T("");
	m_nTypeBridge	= 0;	// 문형, ㅠ형, 슬래브, V형, 박스형, 경사 교각, 복합형 등..
	m_dStationBridgeStt	= 0;		// 시작 스테이션
	m_dLengthBridge	= 12000;		// 총길이
	m_nQtyJigan	= 1;
	m_nSeparBri = 0;

	// 횡단 위치 입력	
	m_dWidthSlabLeft = 0.0;			// 슬래브 좌측단 거리
	m_dWidthSlabRight = 0.0;		// 슬래브 우측단 거리
	m_nLineLeft = 0;			// 좌측 선형
	m_nLineBase = 0;			// 기준 선형
	m_nLineRight = 0;			// 우측 선형
	m_dDisExtLineSttLeft  = 0;
	m_dDisExtLineSttRight = 0;
	m_dDisExtLineEndLeft  = 0;
	m_dDisExtLineEndRight = 0;
	m_dDistRoadCenterLevelLeft	= 0;		// 도로중심 레벨거리 : 좌측
	m_dDistRoadCenterLevelRight	= 0;		// 도로중심 레벨거리 : 우측

	m_bUsePadShoe	  = FALSE;

	m_dFckMassConc	= 210;
	m_dFckLeanConc	= 150;

	m_bLevelFrRoadCenterToEdge	= TRUE; 
	m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	= TRUE;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
	m_dUnitWeightRail			= 0.15;
	m_dDisBaseElevationRL		= 0; 
	m_dHeightRail				= 145.6917; 
	m_bApplyLineSlope			= TRUE; 
	// 지중라멘교
	m_bUnderGroundRahmen		= FALSE;
	m_bCalcOpen					= FALSE;
	m_bPrevApplyCalcOpen		= FALSE;
	m_bUnderGroundPaveDetail	= TRUE;
	m_dUnderGroundSlopeSlabLeft	= 0;
	m_dUnderGroundSlopeSlabRight= 0;
	m_dUnderGroundCenterELStt	= 0;
	m_dUnderGroundCenterELEnd	= 0;
	m_dWidthRoadLeft	= 0;
	m_dWidthRoadRight	= 0;
	m_dUnderGroundNoriSlopeFirst= 0;
	m_dUnderGroundNoriSlopeNext	= 0;
	m_dUnderGroundNoriMaxHeight	= 0;
	m_dUnderGroundNoriWidth		= 0;
	m_dUnderGroundNoriSolpeSodan= 0;
	m_nQtyHDanRoad	= 0;
	for(i=0; i<MAX_QTY_HDAN; i++)
	{
		m_dLengthHDanRoad[i] = 0;
		m_bLevelHDanRoad[i]  = FALSE;
		m_dSlopeHDanRoad[i]  = -0.02;
		m_nTypeHDanRoad[i]   = 0;
		m_ZzbVariableLenHDan[i]	= FALSE;
		m_ZzbVariableLenHDanRoad[i] = FALSE;

		m_dLengthRail[i]	  = 0;
		m_nLineNumber[i]	  = 0;
		m_bLevelHDan[i]  = FALSE;
		m_dSlopeHDan[i]		  = -0.02;
	}
	m_dRail_HeightTie	 = 0;
	m_dRail_LengthTie	 = 0;
	m_dRail_WidthTie	 = 0;
	m_dRail_DistBetweenWheel = 0;
	m_dRail_DistBetweenRail = 0;;

	m_nTypePavement					= 0;
	m_dUnderGroundHeightPavement	= 0;
	m_dUnderGroundDesignHeight		= 0;
	m_bUnderGroundDesignHeight		= TRUE;
	m_nUnderGroundDesignType		= 0;
	m_nUnderGroundDesignSelect		= 0;
	m_nUnderGroundDistribute		= 1;
	m_dUnderGroundDesignLiveAlways	= 0;
	m_dUnderGroundHeightMortar		= 0;
	m_dUnderGroundWeightMortar_xx	= 0;
	m_dUnderGroundPaveHeightPyo		= 0;
	m_dUnderGroundPaveHeightGee		= 0;
	m_dUnderGroundPaveHeightMiddle	= 0;
	m_dUnderGroundPaveHeightBozo	= 0;
	m_dUnderGroundPaveHeightDong	= 0;
	m_dUnderGroundPaveHeightEps		= 0;
	// m_dUnderGroundPaveHeightTosa_XXX	= 0;
	m_dUnderGroundPaveWeightPyo		= 0;
	m_dUnderGroundPaveWeightGee		= 0;
	m_dUnderGroundPaveWeightMiddle	= 0;
	m_dUnderGroundPaveWeightBozo	= 0;
	m_dUnderGroundPaveWeightDong	= 0;
	m_dUnderGroundPaveWeightEps		= 0;
	m_dUnderGroundPaveWeightTosa	= 0;
	m_dUnderGroundPaveWidthPyo		= 0;
	m_dUnderGroundPaveWidthGee		= 0;
	m_dUnderGroundPaveWidthMiddle	= 0;
	m_dUnderGroundPaveWidthBozo		= 0;
	m_dUnderGroundPaveWidthDong		= 0;
	m_dUnderGroundPaveWidthEps		= 0;
	m_dUnderGroundPaveWidthTosa		= 0;
	// 지간 구성 입력
	for(i=0; i<MAX_JIJUM_COUNT_RC-1; i++)
	{
		m_dLengthJigan[i] = 0;				// 지간 길이
	}
	for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		m_bExpJoint[i] = FALSE;				// 신축 이음 설치 유무
	}
	m_nExpJointType[iSTT] = TYPE_ABUT;
	m_nExpJointType[iEND] = TYPE_ABUT;

	m_arrStrJijumLen.RemoveAll();
	m_arrJijumLen.RemoveAll();
	m_bEWDlgUsed = FALSE;
	m_nTypeShoeAngle = 0;
	m_bIsExp		= FALSE;			// 가각(학폭) 지원할지?
	m_pDesignExpanJoint = new CDesignExpansionJoint;
	// 확폭 제원
	for(i=0; i<2; i++)
	{
		m_dExpSttW[i] = 0;	// 시점부
		m_dExpSttH[i] = 0;
		m_dExpSttR[i] = 0;
		m_dExpEndW[i] = 0;	// 종점부
		m_dExpEndH[i] = 0;
		m_dExpEndR[i] = 0;
		m_bExpCutStt[i]	= FALSE;	// [0] : 좌
		m_bExpCutEnd[i]	= FALSE;	// [1] : 우
		m_bExpSttRoundIsTan[i]	= TRUE;	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
		m_bExpEndRoundIsTan[i]	= TRUE;
	}

	m_nNotchType	 =  0;
	m_dNotchHeight	 = 20;
	m_dNotchWidth1	 = 30;
	m_dNotchWidth2	 = 30;
	m_dConsJointHeight  =  50;
	m_dConsJointWidth1	=  50;
	m_dConsJointWidth2	= 100;

	// 교량 받침 위치 입력
	// CJijum 클래스와 CShoePos 클래스에서 받음.

	// 횡단 구성 입력
	m_nQtyHDan = 0;							// 횡단 구성 개수
	for(i=0; i<MAX_QTY_HDAN; i++)
	{
		m_dLengthHDan[i] = 0.0;		// 각 횡단 구간의 길이
		m_dSlopeHDan[i] = -0.02;    // 
		m_nTypeHDan[i] = -1;		// 각 구간의 구성 형태

		m_dLengthRail[i] = 0;   // 철도교
		m_nLineNumber[i] = 0;   // 철도교 레일 기준 선형
	}
	m_bGuardWallDlgUsed = FALSE;
	m_bGuardWallUnderDlgUsed = FALSE;
	m_bSSDlgUsed = FALSE;

	m_nRoadMaterial = 0;	// 도로 포장 재질
	m_dTP = 80.0;			// 포장 두께
	m_dTS = 800.0;			// 슬래브 두께
	m_dTSOld = 800.0;
	for(i=0; i<2; i++)
	{
		m_dDH[i] = 165.0;	// 슬래브 끝단에서 헌치까지의 거리
		m_bCantilSlab[i] = FALSE;
		m_nSlabChemfer[i] = 2;	// 슬래브 끝단 형태
		m_dCantilH1[i] = 0.0;
		m_dCantilH2[i] = 0.0;
		m_dCantilB[i] = 0.0;
		m_dRE1[i] = 160.0;	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dRE2[i] = 0;	// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dTCW1[i] = 0;	// 슬래브 하부 끝단 헌치 처리시 헌치 폭
		m_dTCH1[i] = 0;	// 슬래브 하부 끝단 헌치 처리시 헌치 높이
		m_dTCW2[i] = 0;	// 슬래브 상부 끝단 헌치 처리시 헌치 폭
		m_dTCH2[i] = 0;	// 슬래브 상부 끝단 헌치 처리시 헌치 높이
	}

	// 설계조건 -------------------------------------------------------
	m_dUnitWeigthBackFill			= 0;		// 뒷채움의 단위중량
	m_dUnitWeigthBackFillWater		= 0;
	m_dAngleFrictionBackFill		= 0;		// 뒷채움의 내부 마찰각
	m_dLiveLoadAlways				= 0;		// 상재하중
	m_dDeadLoadAlways				= 0;
	m_nSelectLiveLoadHori			= 0;		// 
	m_dLiveLoadFootway				= 0;
	m_dUnitWeightHeapSoil			= 2.00;		// 성토재료의 단위중량
	m_dUnitWeightValidDepth			= 0;		// 근입지반의 단위중량
	m_dValidDepthBaseGround			= 0;		// 기초의 유효근입 깊이
	m_dEo_CalcWallSpringEQ			= 0;
	m_nTypeToapSafeCheck			= 0;		// 안정검토시의 토압검토
	m_SafeFactorOverturning[0]		= 0;		// 전도에 대한 안정률(평상시)
	m_SafeFactorOverturning[1]		= 0;		// 전도에 대한 안정률(지진시)
	m_SafeFactorSliding[0]			= 0;		// 활동에 대한 안정률(평상시)
	m_SafeFactorSliding[1]			= 0;		// 활동에 대한 안정률(지진시)
	m_nSelectModelingCountEQ		= 0;		// 좌측편토압에 대해서만 모델링
	m_bGoalEarthQuake[eGoalEQ_Collapse]		= TRUE;
	m_bGoalEarthQuake[eGoalEQ_Function]		= FALSE;
	m_bGoalEarthQuake[eGoalEQ_Immediately]	= FALSE;
	m_bGoalEarthQuake[eGoalEQ_LongTerm]		= FALSE;
	m_nSelectFootForce				= 0;
	m_nSelect2DFrameTool			= SOLVE_SAP_6AND7;
	m_nSelect3DShellTool			= 0;
	m_nSelectMidasVer				= 0;
	m_nSelectWingCalc				= 0;
	m_nSelect3DOutputValue			= 0;
	m_nApplyWingWallCalcValueCount	= 0;
	m_bApplyUserDispMax				= FALSE;
	m_dUnitLoadDispMaxUser			= 0.0;
	m_nSelectWingLoad_LS_ES			= 1;
	m_nLiveLoadDistToBackWall		= 0;
	m_nApplyWingWallHeightBasePos	= 1;
	m_bCorrectionFactor_LiveLoadAlways	= FALSE;
	m_dCorrectionFactor_LiveLoadAlways	= 1.0;

	// ------------------------------------------------------ 설계조건
	m_MatComboUlt.Resize(0,0);	// 계수하중 조합계수
	m_MatComboUse.Resize(0,0);	// 사용하중 조합계수	
	m_MatComboEcc.Resize(0,0);	// 편심하중 조합계수	
	m_MatComboEQ.Resize(0,0);	// 지진하중 조합계수	
	m_dWLoadNormal	= 0x00;		// 하중선택 (평상시)
	m_dWLoadEQ		= 0x00;		// 하중선택 (지진시)
	for(i=0; i<BRIDGE_LOAD_BASIC; i++)
	{
		m_bApplyLoad[i] = FALSE;
	}
	m_bApplyLiveLoadEQ = FALSE;	// 지진시 활하중재하
	m_bApplyAnchorForce = FALSE;
	for(i=0; i<RCBRIDGE_COMBO_COUNT; i++)
	{
		for(long j=0; j<MAX_RCBRIDGE_COMBO; j++)
		{
			m_bApplyCombo3D[i][j] = FALSE;
			m_bApplyGroundWater[i][j] = FALSE;
			m_dFactorAnchor[i][j] = 1.0;
		}
	}
	m_dFactorComboVLoad	= 1.0;

	// 설계선택사항 -------------------------------------------------------
	m_bCheckMainSlabAndFootTemper			= FALSE;
	m_bSelectSupportBarWall						= FALSE;			// 수평철근량검토시 노출면 배치
	m_bCheckSuppoerBarSlabMain				= TRUE;
	m_bCheckSuppoerBarSlabMainOver20		= FALSE;
	m_bCheckSuppoerBarSlab300					= TRUE;
	m_bCheckSuppoerBarWall15hs				= TRUE;
	m_bCheckSuppoerBarWallMainOver20		= FALSE;
	m_bCheckSuppoerBarWall300					= TRUE;
	m_bCheckSuppoerBarWallArea				= TRUE;
	m_bCheckSuppoerBarFoot15hs				= TRUE;
	m_bCheckSuppoerBarFootMainOver20	= FALSE;
	m_bCheckSuppoerBarFoot300					= TRUE;
	m_bCheckSuppoerBarFootArea				= TRUE;
	m_bCheckSuppoerBarWing15hs				= TRUE;		// 수평철근비 날개벽 0.0015hs
	m_bCheckSuppoerBarWing300				= TRUE;		// 수평철근비 날개벽 300mm²이상
	m_bCheckSuppoerBarWingArea				= TRUE;		// 수평철근비 날개벽 단면적비
	m_dCheckSuppoerBarWallArea				= 0.0025;
	m_dCheckSuppoerBarWallAreaForOver400 = 0.0025;
	m_dCheckSuppoerBarFootArea				= 0.0020;
	m_dCheckSuppoerBarSlabArea				= 0.0020;
	m_dCheckSuppoerBarWingArea				= 0.0020;
	m_bCheckSupportWallDryingShrinkage	= TRUE;
	m_bCheckSupportWallMainRate				= FALSE;
	m_bCheckSupportFootDryingShrinkage	= TRUE;
	m_bCheckSupportSlabDryingShrinkage	= TRUE;
	m_bCheckSupportSlabByReqRebar			= FALSE;
	m_bCheckSupportSlabMainRate				= FALSE;
	m_bCheckSupportSlabMainRateSpan		= FALSE;
	m_bSkipAsMinCheckByHoriRebar				= FALSE;
	m_nSelectPileCalcSpringEQ			= 0;
	m_nSelectApsShearCheck			= 0;
	m_bApplyMinAvSlab					= FALSE;		// 전단검토시 최소전단검토
	m_bApplyMinAvWall					= FALSE;		// 전단검토시 최소전단검토
	m_bApplyMinAvFoot					= FALSE;		// 전단검토시 최소전단검토
	m_bApplyMinAvWing					= FALSE;		// 전단검토시 최소전단검토
	m_bApplyMinAvAps					= FALSE;
	m_bApplyMinAvHyunTa				= FALSE;
	m_bCheckMinCtcShearWing		= TRUE;
	m_bCheckMinCtcShearAps			= TRUE;
	m_bCheckCornerInside				= TRUE;
	m_bApplyFootingGForce				= TRUE;			// 지진시 기초 관성력 적용
	m_bApplyMassCalcSafe				= TRUE;
	m_bApplyFootingMinThick			= FALSE;
	m_bCheckRebarRateFooting		= FALSE;
	m_nSelectApplyFs				= 0;
	m_nApplyCalcShearDistFront		= 0;			// 앞굽 전단검토시 위험단면 위치
	m_nApplyCalcShearDistBack		= 0;			// 뒷굽 전단검토시 위험단면 위치
	m_nTypeCrackDesignSlab			= 0;			// 강재의 부식에 대한 환경조건(상부슬래브)
	m_nTypeCrackDesignWall			= 0;			// 강재의 부식에 대한 환경조건(벽체)
	m_nTypeCrackDesignFoot			= 0;			// 강재의 부식에 대한 환경조건(기초(하부슬래브))
	m_nTypeCrackDesignWing			= 0;			// 강재의 부식에 대한 환경조건(기초(하부슬래브))
	m_nTypeCrackDesignAps			= 0;
	m_nSelectJointUpperRange			= 0;			// 상부이음, 정착장 적용 범위
	m_nIdxCountCycleMainRebar		= 0;			// 주철근 배치 옵션(CYCLE)
	m_dFactorToap							= 1.70;			// 날개벽 계산시 토압에대한 하중계수
	m_dFactorLive							= 1.70;			// 날개벽 계산시 상재하중에대한 하중계수
	m_nSelFactorKhStt						= 1;			// 시점벽체 토압계수 산정시 수평토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
	m_nSelFactorKhEnd						= 1;			// 종점벽체 토압계수 산정시 수푱토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
	m_nSelCalcCornerFt					= 0;
	m_nSelectForceCheckPos			= 0;			// 단면검토 절점 위치 (0-강역,1-벽체내측)
	m_nSelectRigidModeling				= 1;
	m_bSelectRigidCalcHunch			= TRUE;
	m_bSelectTempLoadAddWall		= TRUE;
	m_bSelectShrinkLoadAddWall		= FALSE;
	m_bSelectEnvToapOne				= TRUE;	
	m_bSelectEnvSubsid					= TRUE;
	m_bSelectHunchModeling			= TRUE;
	m_bSelectHunchDesignCheck		= TRUE;;

	m_nSelectTempLoadSlab			= 0;
	m_nSelSubsideType					= 0;
	m_nSelectDesignRoadWidth		= 0;			// 설계차로폭 선택	  (0-3.0, 1-W계산)
	m_nSelectLiveLoadSpanLength	= 1;			// 활하중산정시 적용지간 (0: 설계지간 선택에 따라, 1: 경사지간, 2: 직각지간)
	m_nSelectDesignSpanLength		= 0;
	m_nSelectDesignSpanApply			= 0;
	m_bSelectSpanLengthApplyShoe	= FALSE;
	m_nSelectWallSectionDesign		= 0;
	m_nSelectInWallSectionDesign		= 1;
	m_nSelectFootingReactionForce	= 2;
	m_nArchRibSectionMomentPos	= 1;
	m_nSelectModelingHeight			= 0;
	m_bCheckSuppoerBarArchRibMinDia1	= FALSE;
	m_bCheckSuppoerBarArchRibMinDia2	= FALSE;
	m_bCheckFrontFootTrap2			= FALSE;
	m_bCalcShearCenter					= TRUE;
	m_nSelectStartLane						= 0;
	m_bCheckUseri							= FALSE;		
	m_dCheckUseriValue					= 0.250;
	m_nSelectApsAxleWeight			= 1;
	m_nSelectApsDeadLoad				= 0;
	m_bApsDowelSeparateRebar		= FALSE;
	m_bOutWallUsebilityCheck		= FALSE;
	// ------------------------------------------------------ 설계선택사항

	// 수량선택사항 -------------------------------------------------------
	m_nFormType_ConcNormal			= 2;			// 거푸집 : 일반콘크리트
	m_nFormType_Footing				= 1;			// 거푸집 : 기초
	m_nFormType_WallFront				= 2;			// 거푸집 : 벽체 전면
	m_nFormType_WallBack				= 2;			// 거푸집 : 벽체 후면
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			m_nFormType_WingFront[stt][left]	= 2;			// 거푸집 : 날개벽 전면
			m_nFormType_WingBack[stt][left]	= 2;			// 거푸집 : 날개벽 배면
		}
	}
	m_nFormType_Aps					= 2;			// 거푸집 : 접속슬래브
	m_nFormType_MassConc		= 0;			// 거푸집 : mass 콘크리트
	m_nFormType_LeanConc			= 0;			// 거푸집 : 버림 콘크리트
	m_bSelectBM_ConcCare_Main	= FALSE;	// 콘크리트양생 : 교대본체, 날개벽
	m_bSelectBM_ConcCare_Aps	= TRUE;		// 콘크리트양생 : 접속슬래브, 완충슬래브
	m_nSelectBM_ConcCare_Cal	= 0;			// 콘크리트양생 산출방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
	m_nTypeRcBridgeBlock			= 0;			// 교대보호블럭 설치
	m_dAngleRcBridgeBlock			= 15;			// 교대보호블럭 외측설치각도(도)
	m_bSelectBM_ApsCnsInclude	= TRUE;		// 수량집계에 접속슬래브, 완충슬래브 포함
	m_dPavementThick					= 400;		// 뒷채움시 콘크리트 포장두께
	m_dNosangThick					= 1000;		// 뒷채움시 노상두께
	m_nTypeHeightWall				= 1;			// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이
	m_nTypeSlabLength				= 0;			// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
	m_nTypeSurfaceProof				= 0;			// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
	m_nTypePost							= 0;			// 본체 동바리 타입(육교용, 일반용)
	m_nTypePostBaseLine				= 0;			// 동바리 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	m_dPostLineEL						= 0;			// 사용자 입력 동바리 기준선 EL
	m_nTypeScaffoldBaseLine		= 0;			// 비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	m_nTypeScaffoldSideLocation	= SCOFFOLD_WING_CON;
	m_dScaffoldLineEL					= 0;			// 사용자 입력 비계 기준선 EL
	m_dThickPostUpper				= 1200;		// 육교용 동바리 상부 두께
	m_dThickPostSide					= 1200;		// 육교용 동바리 좌우측 두께
	m_dThickScaffold					= 1200;		// 비계 두께
	m_dTimeConcForm					= 120;
	m_bApplyYongSu						= FALSE;	//용수터파기 

	m_nDeductNearFormBM				= 0;			// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
	m_nDeductNearTupagiBM			= 0;			// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)

	m_dAggregate_Footing				= 25;			// 콘크리트 골재최대치수(기초)
	m_dAggregate_Main					= 25;			// 콘크리트 골재최대치수(본체)
	m_dAggregate_Aps					= 25;			// 콘크리트 골재최대치수(접속슬래브)
	m_dAggregate_Lean					= 40;			// 콘크리트 골재최대치수(버림)
	m_dAggregate_Mass					= 40;			// 콘크리트 골재최대치수(MASS)
	m_dAggregate_Hyunta				= 25;			// 콘크리트 골재최대치수(현장타설말뚝)
	m_dAggregate_Etc						= 19;			// 콘크리트 골재최대치수(방호벽등)

	m_dSlump_Footing						= 15;			// 콘크리트 Slump(기초)
	m_dSlump_Main							= 15;			// 콘크리트 Slump(본체)
	m_dSlump_Aps							= 8;			// 콘크리트 Slump(접속슬래브)
	m_dSlump_Lean							= 8;			// 콘크리트 Slump(버림)
	m_dSlump_Mass						= 8;			// 콘크리트 Slump(MASS)
	m_dSlump_Hyunta						= 8;			// 콘크리트 Slump(현장타설말뚝)
	m_dSlump_Etc							= 15;			// 콘크리트 Slump(방호벽등)

	m_bMonumenu								= TRUE;		// 교명주 받침 설치(OLD)
	m_nMonumenu								= 4;			// 교명주 받침 설치
	m_nDesPanel									= 2;			// 설명판 설치개수
	m_nNamePanel								= 2;			// 교명판 설치개수
	m_dAddLenghForGuardWallBM		= 0;			// 방호벽 수량 산출시 추가 길이(OLD)
	m_dAddLenghForGuardWall[0][0]	= 0;			// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[0][1]	= 0;			// 방호벽 수량 산출시 추가 길이 종점 우측
	m_dAddLenghForGuardWall[1][0]	= 0;			// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[1][1]	= 0;			// 방호벽 수량 산출시 추가 길이 종점 우측
	m_nHeightForSlabBM						= 0;			// 슬래브 거푸집 수량 산출시 높이 기준 
	m_dTackCotting								= 1.5;		// 택 코팅 (ARCBRIDGE-1570) 2.0 -> 1.5 변경요청
	m_nTypePileMilkBM						= 0;
	m_bExcludeApsDowelRebar			= FALSE;
	m_dTimeConcForm							= 120;
	m_bApplyYongSu								= FALSE;
	m_dSurplusSoil_HyunTaPile				= 100;		//현장타설말뚝 잔토처리
	m_dConcreteSoil_HyunTaPile			= 8;			//현장타설말뚝 콘크리트 타설(토사)
	m_dConcreteRock_HyunTaPile			= 14;			//현장타설말뚝 콘크리트 타설(암반)	
	// ------------------------------------------------------ 수량선택사항

	// 피복설정 -------------------------------------------------------
	for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverUpper_UpperSlab[i]	= 0;	// 상부슬래브 상면
		m_dCoverLower_UpperSlab[i]	= 0;	// 상부슬래브 하면
		m_dCoverSide_UpperSlab[i]	= 0;	// 상부슬래브 측면
		m_dCoverWallOut[i]	= 0;	// 벽체외면
		m_dCoverWallIn[i]	= 0;	// 벽체 내면
		m_dCoverWallMid[i]	= 0;	// 중간벽체
		m_dCoverColumn[i]	= 0;	// 기둥
		m_dCoverWingOut[i]	= 0;	// 날개벽 외면
		m_dCoverWingIn[i]	= 0;	// 날개벽 내면
		m_dCoverUpper_Footing[i]		= 0;	// 기초 상면
		m_dCoverLower_Footing_Lean[i]	= 0;	// 기초 하면(직접기초시)
		m_dCoverLower_Footing_Pile[i]	= 0;	// 기초 하면(말뚝기초시)
		m_dCoverRib[iUPPER][i]	= 0;
		m_dCoverRib[iLOWER][i]	= 0;
		m_dCoverSide_Footing[i] = 0;		//기초 측면
	}
	m_dCoverWingUpper = 0;				// 날개벽 상면
	m_dCoverWingEnd = 0;				// 날개벽 끝단
	// ------------------------------------------------------ 피복설정
	m_bPileNvalueAuto = FALSE;

	// 종단면형상 -------------------------------------------------------
	m_nTypeJongdanForRebar		= 0;
	m_nTypeWallForRebarGen		= 0;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
	m_nTypeOutWall		= 2;	// 외측벽체 유무(좌우)(0 : 없음 -1 : 시점측, 1 : 종점측, 2 : 둘다)

		// 시점측 벽체
	m_dFHS		= 0;
	m_dFLS		= 0;
	m_dFRS		= 0;
	m_dWS		= 0;
	m_dWSHW		= 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dWSHH		= 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWS		= 0;
	m_dBHHS		= 0;
	m_dBHS		= 0;
	m_dBTHS		= 0;
	m_dBWS		= 0;
	m_dHWS		= 0;
	m_dHWS		= 0;
	m_dHRS		= 0;			// 시점헌치 반경
	m_nQtyRail		= 2;
	m_dUnitWeightRail		= 0.15;
	m_dDisBaseElevation		= 0;
	m_dSlopeDosang		= 0;
	m_dHeightDosangUnderTie		= 0;

	m_dELSB		= 0;
	m_footingStt.m_nType = FOOTING_TYPE_BASIC;
	m_footingStt.m_bLeftIsFrontGup	= TRUE;
	m_footingStt.m_exFooting.m_Pile.m_nTypeOrgDefault	= -1;
	m_footingStt.m_exFooting.m_Pile.m_bLeftIsOrgStt		= FALSE;
	m_footingStt.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;
	m_bDivisionApsStt	= FALSE;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsStt[left]		= 0;	// 완충슬래브 수
		m_apsStt[left].m_bIs		= left == iLEFT ? TRUE : FALSE;
		m_apsStt[left].m_bLeft		= TRUE;
		m_apsStt[left].m_bOrgLeft	= FALSE;
		for(cns = 0; cns < MAX_CNS; cns++)
		{
			m_cnsStt[left][cns].m_bLeft		= m_apsStt[left].m_bLeft;
			m_cnsStt[left][cns].m_bOrgLeft	= m_apsStt[left].m_bOrgLeft;
			m_cnsStt[left][cns].m_nType		= TYPE_APS_CNS;
		}
	}

	
	//m_ExFootingStt.m_nType = EXFOOTING_TYPE_LEAN;

		// 종점측 벽체
	m_dFHE		= 0;
	m_dFLE		= 0;
	m_dFRE		= 0;
	m_dWE		= 0;
	m_dWEHW		= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dWEHH		= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWE		= 0;
	m_dBHHE		= 0;
	m_dBHE		= 0;
	m_dBTHE		= 0;
	m_dBWE		= 0;
	m_dHWE		= 0;
	m_dHHE		= 0;
	m_dHRE		= 0;			// 종점헌치 반경
	m_dELEB		= 0;
	m_footingEnd.m_nType = FOOTING_TYPE_BASIC;
	m_footingEnd.m_bLeftIsFrontGup	= FALSE;
	m_footingEnd.m_exFooting.m_Pile.m_nTypeOrgDefault	= -1;
	m_footingEnd.m_exFooting.m_Pile.m_bLeftIsOrgStt		= TRUE;
	m_footingEnd.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;
	m_footingEnd.m_exFooting.m_bLeftOrgOfMass			= TRUE;
	m_bDivisionApsEnd	= FALSE;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsEnd[left]		= 0;		// 완충슬래브 수
		m_apsEnd[left].m_bIs		= left == iLEFT ? TRUE : FALSE;
		m_apsEnd[left].m_bLeft		= FALSE;
		m_apsEnd[left].m_bOrgLeft	= TRUE;
		for(cns = 0; cns < 2; cns++)
		{
			m_cnsEnd[left][cns].m_bLeft		= m_apsEnd[left].m_bLeft;
			m_cnsEnd[left][cns].m_bOrgLeft	= m_apsEnd[left].m_bOrgLeft;
			m_cnsEnd[left][cns].m_nType		= TYPE_APS_CNS;
		}
	}
	//m_ExFootingEnd.m_nType = EXFOOTING_TYPE_LEAN;

		// 내측벽체
	m_nCountInWallHunch	= 0;
	for(long upper=0; upper<2; upper++)
	{
		for(i = 0; i < MAX_INWALL_HUNCH; i++)
		{
			m_dHHL[i][upper]	= 0;
			m_dHHR[i][upper]	= 0;
			m_dHBH[i][upper]	= 0;
			m_dHLW[i][upper]	= 0;
			m_dHRW[i][upper]	= 0;
			m_dWL[i][upper]		= 0;
			m_dWR[i][upper]		= 0;
			m_dHLR[i][upper]	= 0;	// 내측헌치 왼쪽 반경
			m_dHRR[i][upper]	= 0;	// 내측헌치 오른쪽 반경
		}
	}

	m_nTypeRoundOfHunchStt	= 0;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
	m_nTypeRoundOfHunchEnd	= 0;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
	for(i = 0; i < MAX_INWALL_HUNCH; i++)
	{
		for(left = 0; left < 2; left++)
			m_nTypeRoundOfHunchInWall[i][left]	= 0;	// 내측헌치 좌우측
	}

	m_bConnectRibAndUpperSlab	= TRUE;
	/*
	// 포물선(리브정의) 2개 (상판 하판)
	for(stt = 0; stt < 2; stt++)
	{
		m_xyRibCrown[stt]		= CDPoint(0, 0);
		m_dRibLenTotal[stt]		= 0;
		m_dRibHeightTotal[stt]	= 0;
		m_dRibLenStt[stt]		= 0;
		m_dRibLenEnd[stt]		= 0;
		m_dRibFactorM[stt]		= 0;
		m_dRibLenArchLine[stt]	= 0;
		m_dRibOffsetFromUpper[stt]	= 0;
	}

	// 포물선(리브정의) 중심선
	m_xyRibCrownCen				= CDPoint(0, 0);
	m_dRibLenTotalCen			= 0;
	m_dRibHeightTotalCen		= 0;
	m_dRibLenSttCen				= 0;
	m_dRibLenEndCen				= 0;
	m_dRibFactorMCen			= 0;
	m_dRibLenArchLineCen		= 0;
	m_dRibOffsetFromUpperCen	= 0;	// 현재 포물선의 기준상단에서의 떨어진 거리
	*/

	for(i = 0; i < MAX_STRUP_REBAR; i++)
	{
		m_dRibBlockW[i]	= 0;	// 리브위 사각 블럭 폭
		m_dRibBlockH[i]	= 0;	// 리브위 사각 블럭 높이
	}
	// ------------------------------------------------------ 종단면형상

	// 내부 벽체
	AhTPADelete(&m_pArrInWall, (CWallApp*)0);

	// 날개벽 제원 입력
	m_pWingWallStt[0] = new CWingWall;
	m_pWingWallStt[1] = new CWingWall;
	m_pWingWallEnd[0] = new CWingWall;
	m_pWingWallEnd[1] = new CWingWall;
	m_bAutoWingEl = FALSE;
	m_bAutoWingHeightSoil = TRUE;

	// 브래킷 길이
	m_bBracketLengthStt = FALSE;
	m_bBracketLengthEnd = FALSE;
	for(i=0; i<2; i++)
	{
		m_dBrkLenStt[i] = 0.0;
		m_dBrkLenEnd[i] = 0.0;
	}

	// 교명주 받침 제원
	m_dMonuH1 = 0;
	m_dMonuH2 = 0;
	m_dMonuH3 = 0;
	m_dMonuD = 0;
	m_dMonuW = 0;
	m_dMonuL = 0;
	m_dMonuFck = 0;
	m_dMonuR_Fy = 0;
	m_dMonuR_DiaHor = 0;
	m_dMonuR_DiaVer = 0;
	m_dMonuR_H = 0;
	m_dMonuR_L = 0;
	m_nMonuR_nA = 0;
	m_dMonuR_A = 0;
	m_dMonuR_A1 = 0;
	m_nMonuR_nB = 0;
	m_dMonuR_B = 0;
	m_nMonuR_nC = 0;
	m_dMonuR_C = 0;
	m_dMonuR_C1 = 0;
	m_dMonuR_C2 = 0;
	m_dMonuR_C3 = 0;
	m_nMonuR_nD = 0;
	m_dMonuR_D = 0;
	m_dMonuR_D1 = 0;
	m_dMonuR_D2 = 0;
	m_dMonuR_D3 = 0;
	m_dMonuAggregate = 19;
	m_pArrTypeDefault.RemoveAll();

	// 기타 : 쓰지 않음 -> 각 방호벽 별로 Fck, Fy 값을 저장함
	m_dOther_Fck = 0;
	m_dOther_Fy  = 0;
	
	m_pLineApp = new CLineInfoApp;
	m_pLineBaseVert	= new CLineInfoApp;
	m_bQuickSearchLine	= TRUE;	// 빠른 선형 검색 적용

	// 자동 최적 설계 관련 
	m_dOptimizeDiaMinimum =  22;	// 주철근 최소직경
	m_dOptimizeDiaMaximum =  29;	// 주철근 최대직경
	m_dOptimizeSafeRate	  = 120;	// 최적화시 안전율
	m_bOptimizeRebar2Dan  = TRUE;	// 2단철근을 적용할 것인지
	m_bOptimizeRebarSplit = FALSE;	// 1,2Cycle의 직경을 다르게도 적용할 것인지.
	m_bOptimizeRebarMatch = FALSE;	// 1,2Cycle의 직경을 다르게도 적용할 것인지.

	m_nTypeAddRate		= 0;	// 0 : 직경별, 1 : 주철근,기타철근
	m_dAddRateLower		= 3.0;	// 하위 할증률
	m_dAddRateUpper		= 6.0;	// 상위 할증률
	m_dAddRateDiaOrg	= 19.0;	// 직경별일경우 하위 직경

	// Box타입
	m_footingBox.m_nType = FOOTING_TYPE_BOX;
	m_footingBox.m_bLeftIsFrontGup	= FALSE;
	m_footingBox.m_exFooting.m_Pile.m_nTypeOrgDefault	= -1;
	m_footingBox.m_exFooting.m_Pile.m_bLeftIsOrgStt		= TRUE;
	m_footingBox.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;
	m_footingBox.m_exFooting.m_bLeftOrgOfMass			= TRUE;
	
	m_bApplyGroundWaterLevel	= FALSE;
	m_dGroundWaterLevel			= 0;
	m_dCoffKh_Sei				= 100;
	m_bExistProtectionRiseFooting = FALSE;
	m_dPRF_W = 1000;						
	m_dPRF_H1 = 500;						
	m_dPRF_H2 = 500;						
	m_bPRF_SameCTC_Main = TRUE;			
	m_dPRF_CTC_Main = 250;				
	m_dPRF_CTC_Support = 150;				
	m_dPRF_CTC_SupportVert = 150;			
	m_dPRF_CTC_Shear = 150;				
	m_dPRF_Dia_Main = 25;				
	m_dPRF_Dia_Support = 16;				
	m_dPRF_Dia_Shear = 16;				
	m_nPRF_Type_Shear = 0;				
	m_nPRF_Count_Shear = 2;		

	// 하부슬래브 입력
	m_dTS_Lower		= 1000;
	m_nTypeLower		= 0;
	m_dThickCobbStone	= 2000;
	m_bSlopeLowerSlab	= FALSE;

	m_nCountAnchor		= 0;					// 앵커 종방향 개수
	m_bAnchorSameValue	= 0;				// 앵커력,길이,직경 동일적용 (횡방향 입력시 1열만 입력)
//	m_dArrDistJongAnchor;	// 앵커 종방향 거리
//	m_dArrLoadAnchor;		// 앵커력
//	m_dArrLengthAnchor;		// 앵커길이
//	m_dArrDiaAnchor;		// 앵커직경
//	m_dArrEaAnchor
//	m_dArrDistHDanAnchor[RCBRIDGE_ANCHOR_MAX];	// 앵커 횡방향 간격

	m_nGroundFactorKV = 0;
	m_dGroundFactorKV = 0;
	m_nGroundFactorBv = 0;
	m_nEquivalentApplyKsb = 0;
	m_dEquivalentApplyKsb = 3;
	m_n2DGeneralSpringType = 0;
	m_n2DEQSpringType = 0;
	m_nVerticalFactor = 0;
	m_dSpringSpaceBottom = 0;
	m_dSpringSpaceWall = 0;
	m_bSpringSpaceIncludeWallTick = FALSE;
	m_dMeshSizeArcCorner = 0.100;
	m_dMeshSizeBracket = 0.100;
	m_dMeshSizeBase = 0.200;
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			m_dMeshSizeWing[stt][left] = 500;
		}
	}
	// SHELL 해석 결과 사용자 입력....
	m_bUseShellUserOutput	= FALSE;
	m_MatShellSlab.Resize(0,0);
	m_MatShellWallStt.Resize(0,0);
	m_MatShellWallEnd.Resize(0,0);
	for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
		m_MatShellWallMid[i].Resize(0,0);
	m_MatShellPRF.Resize(0,0);

	// 속도개선을 위해서 종단면 입력이 끝나면 직방향과 사방향 좌우중심선형기준으로한 종단면을 모두 구해 놓는다.
	m_bSettingVertSection		= FALSE;	// 종단제원이 셋팅되어 있는지
	m_bNeedSettingVertSection	= TRUE;	// 종단제원이 셋팅이 필요한지?
	m_bSettingPlane				= FALSE;
	m_bNeedSettingPlane			= TRUE;
	
	// 가각부 횡단 EL 계산용
	for(i=0; i<30; i++)
		m_Aslope[i] = CDPoint(0,0);

	m_nLeftParaOld	= 0;
	m_bApplyAngleParaOld	= TRUE;
	m_bWingFrontParaOld	= TRUE;
	m_nTypeJongdanParaOld	= 0;
	m_bForBMOfFootingParaOld	= TRUE;
	m_bFirstCalledGetTvVertSection_Std	= FALSE;
	m_bSecondCalledGetTvVertSection_Std	= FALSE;

	m_bBridgeForBM = FALSE;
	// 한계상태 : 기초지반 - 자동설정
	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix )
	{
		m_bAutoGroundInfo[ix] = TRUE;
		m_bAutoCalcPileProperty[ix] = FALSE;
		m_nSelect_Eo_VertType[ix] = 0;
		m_nSelect_Eo_HoriType[ix] = 0;
	}

	InitStmData();
	InitLsdData();
}

CRcBridgeData::~CRcBridgeData()
{
//	delete m_pRailLoadCtl;

	delete m_pDesignExpanJoint;

	AhTPADelete(&m_pArrInWall, (CWallApp*)0);
	AhTPADelete(&m_pArrJijum, (CJijum*)0);
	AhTPADelete(&m_pArrGuardWall, (CGuardWallRC*)0);
	AhTPADelete(&m_pArrGuardWallRoad, (CGuardWallRC*)0);
	AhTPADelete(&m_pArrFootway, (CFootway*)0);

	InitConcSlab();
	AhTPADelete(&m_pArrConcGirder, (CConcBasic*)0);
	InitConcWall();
	InitConcInWall();
	InitConcFoot();
	InitConcFootRev();
	InitConcFootCheckAdd();
	delete m_pWingWallStt[0];
	delete m_pWingWallStt[1];
	delete m_pWingWallEnd[0];
	delete m_pWingWallEnd[1];
	AhTPADelete(&m_pArrConcArchRib,   (CConcBasic*)0);

	AhTPADelete(&m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	delete m_pLineApp;
	delete m_pLineBaseVert;

	AhTPADelete(&m_pArrTypeDefault,   (CTypeDefault*)0);

	InitSaveCrossSection();
	InitSaveOrgInWall();
	InitSaveXySlabUpperByLenFrSttStation();
	InitSaveXyMatchLineAndSlabUpper();
	InitSaveDistRigidEndSlab();
	InitSaveDistRigidEndWall();

	DeleteStmData();
	DeleteLsdData();
}

// 유효한 값들인지 체크
// BOOL CRcBridgeData::CheckDataValidity()
// {
// 	return TRUE;
// }

void CRcBridgeData::Serialize(CArchive &ar)
{
	// 44 : 속도개선용 tvvectorarray detail save 시작.
	// 62 : 방호벽의 난간기초배근중 상부슬래브 피복위치에 자동으로 박히는 철근 제거용.
	// 64 : 우각부 검토시 내측인장보강철근량 검토옵션 추가
	// 65 : 접속슬래브 부식환경 선택옵션 추가
	// 66 : 설계지간의 선택.. 2006.08.16
	// 67 : 경사교각(TYPE-2), 아치형 라멘교 기초부등침하 가정
	// 68 : 수량 옵션 추가(택코팅, 말뚝 밀크량 산출 기준)
	// 69 : 지진시 기초작용력 선택옵션변수 추가
	// 70 : 지중라멘 하중계산을 노출라멘으로 계산
	// 71 : 가각의 R을 접선방식과 아닌 방식으로 구분해서 지원
	// 72 : 비계기준위치 입력 옵션 추가
	// 73 : 아치교 전용 가공되지 않은 종단 좌표 보관
	// 74 : 말뚝N치 사용자입력 변수추가
	// 75 : 기초 최소 두께 적용
	// 76 : 날개벽 배력근 검토 옵션추가
	// 77 : 교명주, 설명판 개수 옵션 추가
	// 78 : 방호벽 골재 최대 치수를 각각 입력할 수 있도록 함
	// 79 : CJijum에 슈 최대 개수 조정하면서 SerizlieNew를 새로 만듬(Flag가 없었음 ㅠㅠ)
	// 80 : BOOL	m_nSelectRigidModeling;			// 모델링해석시 강역 설정...
	// 81 : long    m_nSelectWallSectionDesign;		// 벽체 단면 검토
	// 82 : m_dDeadLoadAlways
	// 83 : m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
	// 84 : m_nSelectModelingHeight;	모델링 구성시 벽체 높이
	// 85 : m_nSelectAlphaKhKv;말뚝 스프링 정수 계산시 α값 적용
	// 86 : m_bCalcShearCenter; 부재 중앙부 전단검토
	// 87 : m_dFactorComboVLoad; kci2007 적용시 연직토압 계수
	// 88 : m_bSelectTempLoadAddWall
	// 89 : m_bSelectSpanLengthApplyShoe
	// 90 : m_dCheckSuppoerBarWallAreaForOver400
	// 91 : double m_dDistRoadCenterLevelLeft;		// 도로중심 레벨거리 : 좌측
	//		double m_dDistRoadCenterLevelRight;		// 도로중심 레벨거리 : 우측
	// 92 : m_nTypeSlabLength;				// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
	// 93 : m_nSelectFootingReactionForce;
	// 94 : m_nSelectDesignSpanApply;
	// 95 : m_nArchRibSectionMomentPos;
	// 96 : m_nSelectInWallSectionDesign;
	// 97 : m_dTSOld
	// 98 : m_bCheckFrontFootTrap2;
	// 99 : m_bApplyUserDispMax;
	// 100 : m_nSelectTempLoadSlab;
	// 101 : m_nSelectStartLane
	// 102 : m_bCheckUseri, m_dCheckUseriValue;		
	// 103 : Box타입 작업
	// 104 : Box타입 작업, m_bCheckRebarRateFooting
	// 105 : Box타입 작업, m_bApplyAnchorForce
	// 106 : MAX_RCBRIDGE_COMBO 100 -> 200
	// 107 : m_nSelectLiveLoadSpanLength
	// 108 : STM
	// 109 : LSD
	// 110 : m_bUnderGroundDesignHeight 추가
	// 111 : m_dEo_Input_Pile_XXX			// 수평 지반변형계수 (Eo) : 0 = N치로 계산
	// 112 : BOOL m_bAutoGroundInfo[MAX_JIJUM_COUNT_RC] - 기초지반 자동설정
	// 113 : m_dAggregate_Hyunta, m_dSlump_Hyunta 추가
	// 114 : m_nDeductNearFormBM, m_nDeductNearTupagiBM 추가
	// 115 : m_nApplyWingWallCalcValueCount
	// 116 : m_dNValue_Eo_Hori_XXX 제거 - CFootingData에 값 복사해줘야함.
	// 117 : m_dCoverSide_Footing
	// 118 : m_nSelectApsAxleWeight
	// 119 : m_nSelectWingLoad_LS_ES
	// 120 : 한계상태 수평철근량 검토
	// 121 : m_nLiveLoadDistToBackWall, m_bCorrectionFactor_LiveLoadAlways
	// 122 : m_bSkipAsMinCheckByHoriRebar
	// 123 : m_bApsDowelSeparateRebar
	// 124 : m_nSelectMidasVer
	// 125 : m_bAutoWingHeightSoil
	// 126 :m_nSelectApsDeadLoad
	// 127 : m_bCheckSuppoerBarSlabMainOver20
	// 128 : m_bApplyT1CalcWingWall
	// 129 : m_dUnitWeightRail
	// 130 : m_bApplyMinAvHyunTa
	// 131 : m_dMonuAggregate
	// 132 : m_bExcludeApsDowelRebar
	// 133 : m_dTimeConcForm
	// 134 : m_MatShellPRF
	// 135 : m_bSpringSpaceIncludeWallTick
	// 136 : m_nUnderGroundDistribute
	// 137 : m_nSelectBM_ConcCare_Cal
	// 138 : m_nTypeScaffoldSideLocation
	// 139 : m_bGoalEarthQuake
	// 140 : m_nApplyWingWallHeightBasePos
	// 141 : m_dEquivalentApplyKsb
	// 142 : m_dSurplusSoil_HyunTaPile , m_dConcreteSoil_HyunTaPile, m_dConcreteRock_HyunTaPile
	// 143 : m_nSelectApplyFs
	// 144: m_bApplyYongSu
	// 145 : m_bOutWallUsebilityCheck
	// 146 : m_bAutoCalcPileProperty, m_nSelect_Eo_VertType, m_nSelect_Eo_HoriType;


	m_nFlag = 146;
	long i(0), left(0), cns(0), stt(0);
	long nSize = 0;
// 	long nLong = 0;

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		// 교량 형식 및 제원
		ar << m_nIdxBridge;
		ar << m_strBridgeName;
		ar << m_nTypeBridge;		// 문형, ㅠ형, 슬래브, V형, 박스형, 경사 등..
		ar << m_dStationBridgeStt;	// 시작 스테이션
		ar << m_dLengthBridge;		// 총길이
		ar << m_nQtyJigan;
		ar << m_nSeparBri;

		// 횡단 위치 입력
		ar << m_dWidthSlabLeft;		// 슬래브 좌측단 거리
		ar << m_dWidthSlabRight;	// 슬래브 우측단 거리
		ar << m_nLineLeft;			// 좌측 선형
		ar << m_nLineBase;			// 기준 선형
		ar << m_nLineRight;			// 우측 선형
		ar << m_dDisExtLineSttLeft;
		ar << m_dDisExtLineSttRight;
		ar << m_dDisExtLineEndLeft;
		ar << m_dDisExtLineEndRight;
		ar << m_dDistRoadCenterLevelLeft;		// 도로중심 레벨거리 : 좌측
		ar << m_dDistRoadCenterLevelRight;		// 도로중심 레벨거리 : 우측


		ar << m_bUsePadShoe;

		ar << m_dFckMassConc;
		ar << m_dFckLeanConc;

		// 지중라멘교
		m_pLineApp->Serialize(ar);
		ar << m_bQuickSearchLine;	// 빠른 선형 검색 적용
		ar << m_bUnderGroundRahmen;
		ar << m_bCalcOpen;
		ar << m_bPrevApplyCalcOpen;
		ar << m_bUnderGroundPaveDetail;
		ar << m_dUnderGroundSlopeSlabLeft;
		ar << m_dUnderGroundSlopeSlabRight;
		ar << m_dUnderGroundCenterELStt;
		ar << m_dUnderGroundCenterELEnd;
		ar << m_dWidthRoadLeft;
		ar << m_dWidthRoadRight;
		ar << m_dUnderGroundNoriSlopeFirst;
		ar << m_dUnderGroundNoriSlopeNext;
		ar << m_dUnderGroundNoriMaxHeight;
		ar << m_dUnderGroundNoriWidth;
		ar << m_dUnderGroundNoriSolpeSodan;
		ar << m_nQtyHDanRoad;
		for(i=0; i<MAX_QTY_HDAN; i++)
		{
			ar << m_dLengthHDanRoad[i];
			ar << m_bLevelHDanRoad[i];
			ar << m_dSlopeHDanRoad[i];
			ar << m_nTypeHDanRoad[i];
			ar << m_ZzbVariableLenHDanRoad[i];
		}
		ar << m_ZzdRail_HeightLane;
		ar << m_dRail_HeightTie;
		ar << m_ZzdRail_HeightDosangUnderTie;
		ar << m_dRail_LengthTie;
		ar << m_dRail_WidthTie;
		ar << m_dRail_DistBetweenWheel;
		ar << m_dRail_DistBetweenRail;

		ar << m_nTypePavement;
		ar << m_dUnderGroundHeightPavement;
		ar << m_bUnderGroundDesignHeight;
		ar << m_dUnderGroundDesignHeight;
		ar << m_nUnderGroundDesignType;			// 노면활하중 종류
		ar << m_nUnderGroundDesignSelect;			// 노면활하중 설계기준
		ar << m_nUnderGroundDistribute;
		ar << m_dUnderGroundDesignLiveAlways;
		ar << m_dUnderGroundHeightMortar;
		ar << m_dUnderGroundWeightMortar_xx;
		ar << m_dUnderGroundPaveHeightPyo;
		ar << m_dUnderGroundPaveHeightGee;
		ar << m_dUnderGroundPaveHeightMiddle;
		ar << m_dUnderGroundPaveHeightBozo;
		ar << m_dUnderGroundPaveHeightDong;
		ar << m_dUnderGroundPaveHeightEps;
		ar << m_dUnderGroundPaveHeightTosa_XXX;
		ar << m_dUnderGroundPaveWeightPyo;
		ar << m_dUnderGroundPaveWeightGee;
		ar << m_dUnderGroundPaveWeightMiddle;
		ar << m_dUnderGroundPaveWeightBozo;
		ar << m_dUnderGroundPaveWeightDong;
		ar << m_dUnderGroundPaveWeightEps;
		ar << m_dUnderGroundPaveWeightTosa;
		ar << m_dUnderGroundPaveWidthPyo;
		ar << m_dUnderGroundPaveWidthGee;
		ar << m_dUnderGroundPaveWidthMiddle;
		ar << m_dUnderGroundPaveWidthBozo;
		ar << m_dUnderGroundPaveWidthDong;
		ar << m_dUnderGroundPaveWidthEps;
		ar << m_dUnderGroundPaveWidthTosa;

		ar << m_dOptimizeDiaMinimum;
		ar << m_dOptimizeDiaMaximum;
		ar << m_dOptimizeSafeRate;
		ar << m_bOptimizeRebar2Dan;
		ar << m_bOptimizeRebarSplit;
		ar << m_bOptimizeRebarMatch;

		ar << m_nTypeAddRate;	// 0 : 직경별, 1 : 주철근,기타철근
		ar << m_dAddRateLower;	// 하위 할증률
		ar << m_dAddRateUpper;	// 상위 할증률
		ar << m_dAddRateDiaOrg;	// 직경별일경우 하위 직경

		m_footingBox.Serialize(ar);
		ar << m_bApplyGroundWaterLevel;
		ar << m_dGroundWaterLevel;
		ar << m_dCoffKh_Sei;
		ar << m_bExistProtectionRiseFooting;	
		ar << m_dPRF_W;						
		ar << m_dPRF_H1;						
		ar << m_dPRF_H2;						
		ar << m_bPRF_SameCTC_Main;			
		ar << m_dPRF_CTC_Main;				
		ar << m_dPRF_CTC_Support;				
		ar << m_dPRF_CTC_SupportVert;			
		ar << m_dPRF_CTC_Shear;				
		ar << m_dPRF_Dia_Main;				
		ar << m_dPRF_Dia_Support;				
		ar << m_dPRF_Dia_Shear;				
		ar << m_nPRF_Type_Shear;				
		ar << m_nPRF_Count_Shear;	

		ar << m_dTS_Lower;
		ar << m_nTypeLower;
		ar << m_dThickCobbStone;
		ar << m_bSlopeLowerSlab;
		ar << m_nCountAnchor;					// 앵커 종방향 개수
		ar << m_bAnchorSameValue;				// 앵커력,길이,직경 동일적용 (횡방향 입력시 1열만 입력)
		m_dArrDistJongAnchor.Serialize(ar);	// 앵커 종방향 거리
		m_dArrLoadAnchor.Serialize(ar);		// 앵커력
		m_dArrLengthAnchor.Serialize(ar);		// 앵커길이
		m_dArrDiaAnchor.Serialize(ar);		// 앵커직경
		m_dArrEaAnchor.Serialize(ar);
		for(i=0; i<m_nCountAnchor; i++)
		{
			m_dArrDistHDanAnchor[i].Serialize(ar);	// 앵커 횡방향 간격
		}

		ar << m_nGroundFactorKV;
		ar << m_dGroundFactorKV;
		ar << m_nGroundFactorBv;
		ar << m_nEquivalentApplyKsb;
		ar << m_dEquivalentApplyKsb;
		ar << m_n2DGeneralSpringType;
		ar << m_n2DEQSpringType;
		ar << m_nVerticalFactor;
		ar << m_dSpringSpaceBottom;
		ar << m_dSpringSpaceWall;
		ar << m_bSpringSpaceIncludeWallTick;

		// 지간 구성 입력
		for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
		{
			ar << m_bExpJoint[i];			// 신축 이음 설치 유무
		}
		ar << m_nExpJointType[iSTT];
		ar << m_nExpJointType[iEND];
		for(i=0; i<MAX_JIJUM_COUNT_RC-1; i++)
		{
			ar << m_dLengthJigan[i];			// 지간 길이
		}
		m_arrStrJijumLen.Serialize(ar);
		m_arrJijumLen.Serialize(ar);
		ar << m_bEWDlgUsed;
		ar << m_nTypeShoeAngle;
		ar << m_bIsExp;			// 가각(학폭) 지원할지?
		m_pDesignExpanJoint->Serialize(ar);
		// 확폭 제원
		for(i=0; i<2; i++)
		{
			ar << m_dExpSttW[i];	// 시점부
			ar << m_dExpSttH[i];
			ar << m_dExpSttR[i];
			ar << m_dExpEndW[i];	// 종점부
			ar << m_dExpEndH[i];
			ar << m_dExpEndR[i];
			ar << m_bExpCutStt[i];	// [0] : 좌
			ar << m_bExpCutEnd[i];	// [1] : 우
			ar << m_bExpSttRoundIsTan[i];	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
			ar << m_bExpEndRoundIsTan[i];
		}

		// 교량 받침 위치 입력
		ar << m_pArrJijum.GetSize();
		for(i = 0; i < m_pArrJijum.GetSize(); i++)
			m_pArrJijum.GetAt(i)->SerializeNew(ar);
		/*
		AhTPASerial(ar, &m_pArrJijum, (CJijum*)0);	// 지점 데이터
		// CJijum에 Flag가 없어서 여기서 저장...
		for(i=0; i<m_pArrJijum.GetSize(); i++)
		{
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
			{
				ar << m_pArrJijum.GetAt(i)->m_dKh[0][j];
				ar << m_pArrJijum.GetAt(i)->m_dKh[1][j];
				ar << m_pArrJijum.GetAt(i)->m_dKv[0][j];
				ar << m_pArrJijum.GetAt(i)->m_dKv[1][j];
			}
		}
		// CJijum 클래스와 CShoePos 클래스에서 받음.
		*/

		// 횡단 구성 입력
		ar << m_nQtyHDan;			// 횡단 구성 개수
		for(i=0; i<MAX_QTY_HDAN; i++)
		{
			ar << m_dLengthHDan[i];			// 각 횡단 구간의 길이
			ar << m_bLevelHDan[i];			// 각 구간의 레벨 처리 유무
			ar << m_nTypeHDan[i];			// 각 구간의 구성 형태
			ar << m_ZzbVariableLenHDan[i];	// 가변 구간 설정 유무
	
			ar << m_dLengthRail[i];
			ar << m_nLineNumber[i];
			ar << m_dSlopeHDan[i];
		}
		AhTPASerial(ar, &m_pArrGuardWall, (CGuardWallRC*)0);
		AhTPASerial(ar, &m_pArrGuardWallRoad, (CGuardWallRC*)0);
		AhTPASerial(ar, &m_pArrFootway, (CFootway*)0);
		ar << m_bRoundGW;
		ar << m_bRoundFW;
		ar << m_bGuardWallDlgUsed;
		ar << m_bGuardWallUnderDlgUsed;
		ar << m_bSSDlgUsed;

		ar << m_nRoadMaterial;
		ar << m_dTP;	// 포장 두께
		ar << m_dTS;	// 슬래브 두께
		ar << m_dTSOld;
		for(i=0; i<2; i++)
		{
			ar << m_dDH[i];		// 슬래브 끝단에서 헌치까지의 거리
			ar << m_bCantilSlab[i];
			ar << m_nSlabChemfer[i];
			ar << m_dCantilH1[i];
			ar << m_dCantilH2[i];
			ar << m_dCantilB[i];
			ar << m_dRE1[i];	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
			ar << m_dRE2[i];	// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
			ar << m_dTCW1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 폭
			ar << m_dTCH1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 높이
			ar << m_dTCW2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 폭
			ar << m_dTCH2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 높이
		}

		// 설계조건 -------------------------------------------------------
		ar << m_dUnitWeigthBackFill;		// 뒷채움의 단위중량
		ar << m_dUnitWeigthBackFillWater;
		ar << m_dAngleFrictionBackFill;	// 뒷채움의 내부 마찰각
		ar << m_dLiveLoadAlways;			// 상재하중
		ar << m_dDeadLoadAlways;
		ar << m_nSelectLiveLoadHori;
		ar << m_dLiveLoadFootway;
		ar << m_dUnitWeightHeapSoil;		// 성토재료의 단위중량
		ar << m_dUnitWeightValidDepth;		// 근입지반의 단위중량
		ar << m_dValidDepthBaseGround;		// 기초의 유효근입 깊이
		ar << m_dEo_CalcWallSpringEQ;
		ar << m_dNValue_Eo_Hori_XXX;			// 수평 지반변형계수
		ar << m_dNValue_Eo_Vert_XXX;			// 수직 지반변형계수
		ar << m_nTypeToapSafeCheck;			// 안정검토시의 토압검토
		ar << m_SafeFactorOverturning[0];	// 전도에 대한 안정률(평상시)
		ar << m_SafeFactorOverturning[1];	// 전도에 대한 안정률(지진시)
		ar << m_SafeFactorSliding[0];		// 활동에 대한 안정률(평상시)
		ar << m_SafeFactorSliding[1];		// 활동에 대한 안정률(지진시)
		ar << m_nSelectModelingCountEQ;
		for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
			ar << m_bGoalEarthQuake[ix];
		ar << m_nSelectFootForce;
		ar << m_nSelect2DFrameTool;
		ar << m_nSelect3DShellTool;
		ar << m_nSelectMidasVer;
		ar << m_nSelectWingCalc;
		ar << m_nSelect3DOutputValue;
		ar << m_nApplyWingWallCalcValueCount;
		ar << m_bApplyUserDispMax;
		ar << m_dUnitLoadDispMaxUser;
		ar << m_nSelectWingLoad_LS_ES;
		ar << m_nLiveLoadDistToBackWall;
		ar << m_bCorrectionFactor_LiveLoadAlways;
		ar << m_dCorrectionFactor_LiveLoadAlways;
		ar << m_nApplyWingWallHeightBasePos;
		// 한계상태
		ar << m_dEo_Input_Pile_XXX;			// 수평 지반변형계수 (Eo) : 0 = N치로 계산
		// 기초지반 - 자동설정
		for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
		{
			ar << m_bAutoGroundInfo[ix];
			ar << m_bAutoCalcPileProperty[ix];
			ar << m_nSelect_Eo_VertType[ix];
			ar << m_nSelect_Eo_HoriType[ix];
		}
		
		// ------------------------------------------------------ 설계조건
		m_MatComboUse.Serialize(ar);
		m_MatComboUlt.Serialize(ar);
		m_MatComboEcc.Serialize(ar);
		m_MatComboEQ.Serialize(ar);
		ar << m_dWLoadNormal;
		ar << m_dWLoadEQ;
		long i=0; for(i=0; i<BRIDGE_LOAD_BASIC; i++)
			ar << m_bApplyLoad[i];
		ar << m_bApplyLiveLoadEQ;	// 지진시 활하중재하
		ar << m_bApplyAnchorForce;
		for(i=0; i<RCBRIDGE_COMBO_COUNT; i++)
		{
			for(long j=0; j<MAX_RCBRIDGE_COMBO; j++)
			{
				ar << m_bApplyCombo3D[i][j];
				ar << m_bApplyGroundWater[i][j];
				ar << m_dFactorAnchor[i][j];
			}
		}

		ar << m_dFactorComboVLoad;

		// 설계선택사항 -------------------------------------------------------
		ar << m_bCheckMainSlabAndFootTemper;
		ar << m_bSelectSupportBarWall;			// 수평철근량검토시 노출면 배치
		ar << m_bCheckSuppoerBarSlabMain;		// 수평철근비 슬래브 주철근비
		ar << m_bCheckSuppoerBarSlab300;		// 수평철근비 슬래브 300mm²이상
		ar << m_bCheckSuppoerBarSlabArea;		// 수평철근비 슬래브 단면적비
		ar << m_bCheckSuppoerBarWall15hs;		// 수평철근비 벽체 0.0015hs
		ar << m_bCheckSuppoerBarWall300;		// 수평철근비 벽체 300mm²이상
		ar << m_bCheckSuppoerBarWallArea;		// 수평철근비 벽체 단면적비
		ar << m_bCheckSuppoerBarFoot15hs;		// 수평철근비 기초 0.0015hs
		ar << m_bCheckSuppoerBarFoot300;		// 수평철근비 기초 300mm²이상
		ar << m_bCheckSuppoerBarFootArea;		// 수평철근비 기초 단면적비
		ar << m_bCheckSuppoerBarWing15hs;		// 수평철근비 날개벽 0.0015hs
		ar << m_bCheckSuppoerBarWing300;		// 수평철근비 날개벽 300mm²이상
		ar << m_bCheckSuppoerBarWingArea;		// 수평철근비 날개벽 단면적비
		ar << m_dCheckSuppoerBarWallArea;		// 수평철근비 벽체 단면적비
		ar << m_dCheckSuppoerBarWallAreaForOver400;
		ar << m_dCheckSuppoerBarFootArea;		// 수평철근비 기초 단면적비
		ar << m_dCheckSuppoerBarSlabArea;		// 수평철근비 슬래브 단면적비
		ar << m_dCheckSuppoerBarWingArea;		// 수평철근비 날개벽 단면적비
		ar << m_bCheckSupportWallDryingShrinkage;
		ar << m_bCheckSupportWallMainRate;
		ar << m_bCheckSupportFootDryingShrinkage;
		ar << m_bCheckSupportSlabDryingShrinkage;
		ar << m_bCheckSupportSlabByReqRebar;
		ar << m_bCheckSupportSlabMainRate;
		ar << m_bCheckSupportSlabMainRateSpan;

		ar << m_bCheckSuppoerBarSlabMainOver20;
		ar << m_bCheckSuppoerBarWallMainOver20;
		ar << m_bCheckSuppoerBarFootMainOver20;

		ar << m_bApplyT1CalcWingWall;

		ar << m_bSkipAsMinCheckByHoriRebar;
		ar << m_nSelectPileCalcSpringEQ;
		ar << m_nSelectApsShearCheck;
		ar << m_bApplyMinAvSlab;				// 전단검토시 최소전단검토
		ar << m_bApplyMinAvWall;				// 전단검토시 최소전단검토
		ar << m_bApplyMinAvFoot;				// 전단검토시 최소전단검토
		ar << m_bApplyMinAvWing;				// 전단검토시 최소전단검토
		ar << m_bApplyMinAvAps;
		ar << m_bApplyMinAvHyunTa;
		
		ar << m_bCheckMinCtcShearWing;
		ar << m_bCheckMinCtcShearAps;
		ar << m_bCheckCornerInside;
		ar << m_bApplyFootingGForce;			// 지진시 기초 관성력 적용
		ar << m_bApplyMassCalcSafe;
		ar << m_bApplyFootingMinThick;
		ar << m_bCheckRebarRateFooting;
		ar << m_nSelectApplyFs;
		ar << m_nApplyCalcShearDistFront;		// 앞굽 전단검토시 위험단면 위치
		ar << m_nApplyCalcShearDistBack;		// 뒷굽 전단검토시 위험단면 위치
		ar << m_nTypeCrackDesignSlab;			// 강재의 부식에 대한 환경조건(상부슬래브)
		ar << m_nTypeCrackDesignWall;			// 강재의 부식에 대한 환경조건(벽체)
		ar << m_nTypeCrackDesignFoot;			// 강재의 부식에 대한 환경조건(기초(하부슬래브))
		ar << m_nTypeCrackDesignWing;
		ar << m_nTypeCrackDesignAps;
		ar << m_nSelectJointUpperRange;			// 상부이음, 정착장 적용 범위
		ar << m_nIdxCountCycleMainRebar;		// 주철근 배치 옵션(CYCLE)
		ar << m_dFactorToap;					// 날개벽 계산시 토압에대한 하중계수
		ar << m_dFactorLive;					// 날개벽 계산시 상재하중에대한 하중계수
		ar << m_nSelFactorKhStt;				// 시점벽체 토압계수 산정시 수평토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
		ar << m_nSelFactorKhEnd;				// 종점벽체 토압계수 산정시 수푱토압계수 0:자동, 1:0.5, 2:1.0, 3:1.5
		ar << m_nSelCalcCornerFt;
		ar << m_nSelectForceCheckPos;			// 단면검토 절점 위치 (0-강역,1-벽체내측)
		ar << m_nSelectRigidModeling;
		ar << m_bSelectRigidCalcHunch;
		ar << m_bSelectTempLoadAddWall;
		ar << m_bSelectShrinkLoadAddWall;
		ar << m_bSelectEnvToapOne;	
		ar << m_bSelectEnvSubsid;
		ar << m_bSelectHunchModeling;
		ar << m_bSelectHunchDesignCheck;
		ar << m_nSelectTempLoadSlab;
		ar << m_nSelSubsideType;
		ar << m_nSelectDesignRoadWidth;			// 설계차로폭 선택	  (0-3.0, 1-W계산)
		ar << m_nSelectLiveLoadSpanLength;
		ar << m_nSelectDesignSpanLength;
		ar << m_nSelectDesignSpanApply;
		ar << m_bSelectSpanLengthApplyShoe;
		ar << m_nSelectWallSectionDesign;
		ar << m_nSelectInWallSectionDesign;
		ar << m_nSelectFootingReactionForce;
		ar << m_nArchRibSectionMomentPos;
		ar << m_nSelectModelingHeight;
		ar << m_nSelectAlphaKhKv_XXX;
		ar << m_bCalcShearCenter;
		ar << m_nSelectStartLane;
		ar << m_bCheckUseri;		
		ar << m_dCheckUseriValue;
		ar << m_nSelectApsAxleWeight;
		ar << m_nSelectApsDeadLoad;
		ar << m_bApsDowelSeparateRebar;
		ar << m_bOutWallUsebilityCheck;
		
		ar << m_bCheckSuppoerBarArchRibMinDia1;
		ar << m_bCheckSuppoerBarArchRibMinDia2;
		ar << m_bCheckFrontFootTrap2;
		// ------------------------------------------------------ 설계선택사항

		// 수량선택사항 -------------------------------------------------------
		ar << m_nFormType_ConcNormal;			// 거푸집 : 일반콘크리트
		ar << m_nFormType_Footing;				// 거푸집 : 기초
		ar << m_nFormType_WallFront;			// 거푸집 : 벽체 전면
		ar << m_nFormType_WallBack;				// 거푸집 : 벽체 후면
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				ar << m_nFormType_WingFront[stt][left];	// 거푸집 : 날개벽 전면
				ar << m_nFormType_WingBack[stt][left];	// 거푸집 : 날개벽 배면
			}
		}
		ar << m_nFormType_Aps;					// 거푸집 : 접속슬래브
		ar << m_nFormType_MassConc;				// 거푸집 : mass 콘크리트
		ar << m_nFormType_LeanConc;				// 거푸집 : 버림 콘크리트
		ar << m_bSelectBM_ConcCare_Main;		// 콘크리트양생 : 교대본체, 날개벽
		ar << m_bSelectBM_ConcCare_Aps;			// 콘크리트양생 : 접속슬래브, 완충슬래브
		ar << m_nSelectBM_ConcCare_Cal;			// 콘크리트양생 산출방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
		ar << m_nTypeRcBridgeBlock;				// 교대보호블럭 설치
		ar << m_dAngleRcBridgeBlock;			// 교대보호블럭 외측설치각도(도)
		ar << m_bSelectBM_ApsCnsInclude;		// 수량집계에 접속슬래브, 완충슬래브 포함
		ar << m_dPavementThick;					// 뒷채움시 콘크리트 포장두께
		ar << m_dNosangThick;					// 뒷채움시 노상두께
		ar << m_nTypeHeightWall;				// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이
		ar << m_nTypeSlabLength;				// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
		ar << m_nTypeSurfaceProof;				// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
		ar << m_nTypePost;						// 본체 동바리 타입(육교용, 일반용)
		ar << m_nTypePostBaseLine;				// 동바리 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
		ar << m_dPostLineEL;					// 사용자 입력 동바리 기준선 EL
		ar << m_nTypeScaffoldBaseLine;			// 비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
		ar << m_dScaffoldLineEL;				// 사용자 입력 비계 기준선 EL
		ar << m_nTypeScaffoldSideLocation;	//날개벽 비계 기준선
		ar << m_dThickPostUpper;				// 육교용 동바리 상부 두께
		ar << m_dThickPostSide;					// 육교용 동바리 좌우측 두께
		ar << m_dThickScaffold;					// 비계 두께

		ar << m_nDeductNearFormBM;				// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
		ar << m_nDeductNearTupagiBM;			// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)

		ar << m_dAggregate_Footing;			// 콘크리트 골재최대치수(기초)
		ar << m_dAggregate_Main;			// 콘크리트 골재최대치수(본체)
		ar << m_dAggregate_Aps;				// 콘크리트 골재최대치수(접속슬래브)
		ar << m_dAggregate_Lean;			// 콘크리트 골재최대치수(버림)
		ar << m_dAggregate_Mass;			// 콘크리트 골재최대치수(MASS)
		ar << m_dAggregate_Hyunta;			// 콘크리트 골재최대치수(현장타설말뚝)
		ar << m_dAggregate_Etc;				// 콘크리트 골재최대치수(방호벽등)

		ar << m_dSlump_Footing;				// 콘크리트 Slump(기초)
		ar << m_dSlump_Main;				// 콘크리트 Slump(본체)
		ar << m_dSlump_Aps;					// 콘크리트 Slump(접속슬래브)
		ar << m_dSlump_Lean;				// 콘크리트 Slump(버림)
		ar << m_dSlump_Mass;				// 콘크리트 Slump(MASS)
		ar << m_dSlump_Hyunta;				// 콘크리트 Slump(현장타설말뚝)
		ar << m_dSlump_Etc;					// 콘크리트 Slump(방호벽등)
		
		ar << m_nMonumenu;					// 교명주 받침 설치
		ar << m_nDesPanel;				// 설명판 설치개수
		ar << m_nNamePanel;				// 교명판 설치개수
		ar << m_dAddLenghForGuardWallBM;	// 방호벽 수량 산출시 추가 길이(OLD)
		ar << m_dAddLenghForGuardWall[0][0];// 방호벽 수량 산출시 추가 길이 시점 좌측
		ar << m_dAddLenghForGuardWall[0][1];// 방호벽 수량 산출시 추가 길이 종점 우측
		ar << m_dAddLenghForGuardWall[1][0];// 방호벽 수량 산출시 추가 길이 시점 좌측
		ar << m_dAddLenghForGuardWall[1][1];// 방호벽 수량 산출시 추가 길이 종점 우측
		ar << m_nHeightForSlabBM;			// 슬래브 거푸집 수량 산출시 높이 기준 
		ar << m_dTackCotting;
		ar << m_nTypePileMilkBM;
		ar << m_bExcludeApsDowelRebar;
		ar << m_dTimeConcForm;
		ar << m_bApplyYongSu;
		ar << m_dSurplusSoil_HyunTaPile;		
		ar << m_dConcreteSoil_HyunTaPile;		
		ar << m_dConcreteRock_HyunTaPile;	
		// ------------------------------------------------------ 수량선택사항

		// 피복설정 -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar << m_dCoverUpper_UpperSlab[i];	// 상부슬래브 상면
			ar << m_dCoverLower_UpperSlab[i];	// 상부슬래브 하면
			ar << m_dCoverSide_UpperSlab[i];	// 상부슬래브 측면
			ar << m_dCoverUpper_LowerSlab[i];	// 하부슬래브 상면
			ar << m_dCoverLower_LowerSlab[i];	// 하부슬래브 하면
			ar << m_dCoverWallOut[i];	// 벽체외면
			ar << m_dCoverWallIn[i];	// 벽체 내면
			ar << m_dCoverWallMid[i];	// 중간벽체
			ar << m_dCoverColumn[i];	// 기둥
			ar << m_dCoverWingOut[i];	// 날개벽 외면
			ar << m_dCoverWingIn[i];	// 날개벽 내면
			ar << m_dCoverUpper_Footing[i];	// 기초 상면
			ar << m_dCoverLower_Footing_Lean[i];	// 기초 하면(직접기초시)
			ar << m_dCoverLower_Footing_Pile[i];	// 기초 하면(말뚝기초시)
			ar << m_dCoverRib[iUPPER][i];
			ar << m_dCoverRib[iLOWER][i];
		}
		ar << m_dCoverWingUpper;
		ar << m_dCoverWingEnd;
		// ------------------------------------------------------ 피복설정
		ar << m_bPileNvalueAuto;

		// 종단면형상 -------------------------------------------------------
		ar << m_nTypeJongdanForRebar;
		ar << m_nTypeWallForRebarGen;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
		ar << m_nTypeOutWall;	// 외측벽체 유무(좌우)
		m_fxJointStt.Serialize(ar);

			// 시점측 벽체
		ar << m_dFHS;
		ar << m_dFLS;
		ar << m_dFRS;
		ar << m_dWS;
		ar << m_dWSHW;	// 시점벽체 헌치너비(경사교2에서만 지원)
		ar << m_dWSHH;	// 시점벽체 헌치너비(경사교2에서만 지원)
		ar << m_dBHWS;
		ar << m_dBHHS;
		ar << m_dBHS;
		ar << m_dBTHS;
		ar << m_dBWS;
		ar << m_dHWS;
		ar << m_dHHS;
		ar << m_dHRS;			// 시점헌치 반경
		ar << m_nQtyRail;
		ar << m_dDisBaseElevation;
		ar << m_dUnitWeightRail;
		ar << m_dSlopeDosang;
		ar << m_dHeightDosangUnderTie;
		ar << m_bLevelFrRoadCenterToEdge	;
		ar << m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
		ar << m_dDisBaseElevationRL		;
		ar << m_dHeightRail				;
		ar << m_bApplyLineSlope			;
		ar << m_dELSB;
		m_footingStt.Serialize(ar);
		ar << m_bDivisionApsStt;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측);
		for(left = 0; left < 2; left++)
		{
			ar << m_nCountCnsStt[left];	// 완충슬래브 수
			m_apsStt[left].Serialize(ar);
			for(cns = 0; cns < MAX_CNS; cns++)
				m_cnsStt[left][cns].Serialize(ar);
		}
		
		//m_ExFootingStt.Serialize(ar);

		// 종점측 벽체
		m_fxJointEnd.Serialize(ar);
		ar << m_dFHE;
		ar << m_dFLE;
		ar << m_dFRE;
		ar << m_dWE;
		ar << m_dWEHW;	// 종점벽체 헌치너비(경사교2에서만 지원)
		ar << m_dWEHH;	// 종점벽체 헌치너비(경사교2에서만 지원)
		ar << m_dBHWE;
		ar << m_dBHHE;
		ar << m_dBHE;
		ar << m_dBTHE;
		ar << m_dBWE;
		ar << m_dHWE;
		ar << m_dHHE;
		ar << m_dHRE;			// 종점헌치 반경
		ar << m_dELEB;
		m_footingEnd.Serialize(ar);
		ar << m_bDivisionApsEnd;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
		for(left = 0; left < 2; left++)
		{
			ar << m_nCountCnsEnd[left];	// 완충슬래브 수
			m_apsEnd[left].Serialize(ar);
			for(cns = 0; cns < MAX_CNS; cns++)
				m_cnsEnd[left][cns].Serialize(ar);
		}

		// 내측벽체
		ar << m_nCountInWallHunch;
		for(long upper=0; upper<2; upper++)
		{
			for(i = 0; i < m_nCountInWallHunch; i++)
			{
				ar << m_dHHL[i][upper];
				ar << m_dHHR[i][upper];
				ar << m_dHBH[i][upper];
				ar << m_dHLW[i][upper];
				ar << m_dHRW[i][upper];
				ar << m_dWL[i][upper];
				ar << m_dWR[i][upper];
				ar << m_dHLR[i][upper];	// 내측헌치 왼쪽 반경
				ar << m_dHRR[i][upper];	// 내측헌치 오른쪽 반경
			}
		}

		ar << m_nTypeRoundOfHunchStt;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
		ar << m_nTypeRoundOfHunchEnd;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
		for(i = 0; i < m_nCountInWallHunch; i++)
		{
			for(left = 0; left < 2; left++)
				ar << m_nTypeRoundOfHunchInWall[i][left];	// 내측헌치 좌우측
		}

		// 포물선(리브정의) 2개 (상판 하판)
		ar << m_bConnectRibAndUpperSlab;
		long upper = 0; for(upper = 0; upper < 2; upper++)
			m_pbRib[upper].Serialize(ar);
		m_pbRibCen.Serialize(ar);
		/*
		for(stt = 0; stt < 2; stt++)
		{
			ar << m_xyRibCrown[stt];
			ar << m_dRibLenTotal[stt];
			ar << m_dRibHeightTotal[stt];
			ar << m_dRibLenStt[stt];
			ar << m_dRibLenEnd[stt];
			ar << m_dRibFactorM[stt];
			ar << m_dRibLenArchLine[stt];
			ar << m_dRibOffsetFromUpper[stt];
		}

		// 포물선(리브정의) 중심선
		ar << m_xyRibCrownCen;
		ar << m_dRibLenTotalCen;
		ar << m_dRibHeightTotalCen;
		ar << m_dRibLenSttCen;
		ar << m_dRibLenEndCen;
		ar << m_dRibFactorMCen;
		ar << m_dRibLenArchLineCen;
		ar << m_dRibOffsetFromUpperCen;	// 현재 포물선의 기준상단에서의 떨어진 거리
		*/

		for(i = 0; i < MAX_STRUP_REBAR; i++)
		{
			ar << m_dRibBlockW[i];	// 리브위 사각 블럭 폭
			ar << m_dRibBlockH[i];	// 리브위 사각 블럭 높이
		}
		// ------------------------------------------------------ 종단면형상

		// 내부 벽체
		nSize = m_pArrInWall.GetSize();
		ar << nSize;
		for(i = 0; i < nSize; i++)
			m_pArrInWall[i]->Serialize(ar);

		// 날개벽 제원 입력
		m_pWingWallStt[0]->Serialize(ar);
		m_pWingWallStt[1]->Serialize(ar);
		m_pWingWallEnd[0]->Serialize(ar);
		m_pWingWallEnd[1]->Serialize(ar);
		ar << m_bAutoWingEl;
		ar << m_bAutoWingHeightSoil;

		// 브래킷 길이
		ar << m_bBracketLengthStt;
		ar << m_bBracketLengthEnd;
		for(i=0; i<2; i++)
		{
			ar << m_dBrkLenStt[i];
			ar << m_dBrkLenEnd[i];
		}

		// 교명주 받침 제원 입력
		ar << m_dMonuH1;
		ar << m_dMonuH2;
		ar << m_dMonuH3;
		ar << m_dMonuD;
		ar << m_dMonuW;
		ar << m_dMonuL;
		ar << m_dMonuFck;
		ar << m_dMonuR_Fy;
		ar << m_dMonuR_DiaHor;
		ar << m_dMonuR_DiaVer;
		ar << m_dMonuR_H;
		ar << m_dMonuR_L;
		ar << m_nMonuR_nA;
		ar << m_dMonuR_A;
		ar << m_dMonuR_A1;
		ar << m_nMonuR_nB;
		ar << m_dMonuR_B;
		ar << m_nMonuR_nC;
		ar << m_dMonuR_C;
		ar << m_dMonuR_C1;
		ar << m_dMonuR_C2;
		ar << m_dMonuR_C3;
		ar << m_nMonuR_nD;
		ar << m_dMonuR_D;
		ar << m_dMonuR_D1;
		ar << m_dMonuR_D2;
		ar << m_dMonuR_D3;
		ar << m_dMonuAggregate;

		AhTPASerial(ar, &m_pArrTypeDefault, (CTypeDefault*)0);

		// 기타
		ar << m_dOther_Fck;
		ar << m_dOther_Fy;

		// 속도개선용(저장을 안 하려 했지만, 읽을때 너무 오래 걸린다는 민원이 들어옴) ////////////
		// 속도개선을 위해서 종단면 입력이 끝나면 직방향과 사방향 좌우중심선형기준으로한 종단면을 모두 구해 놓는다.
		// 그리고 종단 그리는 방법에 따라서도 모두 구해 놓는다.
		ar << m_bSettingVertSection;	// 종단제원이 셋팅되어 있는지
		ar << m_bNeedSettingVertSection;	// 종단제원이 셋팅이 필요한지?
		for(long jong = 0; jong < 3; jong++)
		{
			m_tvArrVertSection_Vert_Left[jong].Serialize(ar, TRUE);
			m_tvArrVertSection_Vert_Center[jong].Serialize(ar, TRUE);
			m_tvArrVertSection_Vert_Right[jong].Serialize(ar, TRUE);

			// 사방향일 경우 날개벽 각도 고려한 종단면도 고려해야 된다. 0 : 고려안함, 1 : 고려함
			for(long wing = 0; wing < 2; wing++)
			{
				m_tvArrVertSection_Ang_Left[jong][wing].Serialize(ar, TRUE);
				m_tvArrVertSection_Ang_Center[jong][wing].Serialize(ar, TRUE);
				m_tvArrVertSection_Ang_Right[jong][wing].Serialize(ar, TRUE);
			}
			m_tvArrVertSection_NonModify_ForArch.Serialize(ar, TRUE);
		}

		// 슬래브 평면 좌표(가각때문에 도저히 안 되겟으 ㅡㅡ)
		ar << m_bSettingPlane;
		ar << m_bNeedSettingPlane;
		for(stt = 0; stt < 2; stt++)
		{
			for(long gagak = 0; gagak < 2; gagak++)
			{
				m_tvArrSttEndPlane[stt][gagak].Serialize(ar, TRUE);
				m_tvArrLeftRightPlane[stt][gagak].Serialize(ar, TRUE);
			}
		}
		ar << m_bUseShellUserOutput;
		m_MatShellSlab.Serialize(ar);
		m_MatShellWallStt.Serialize(ar);
		m_MatShellWallEnd.Serialize(ar);
		for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
			m_MatShellWallMid[i].Serialize(ar);
		m_MatShellPRF.Serialize(ar);

		for(i = 0; i < MAX_REBAR_DAN; i++)
			ar << m_dCoverSide_Footing[i];

		
	}
	else
	{
		ar >> m_nFlag;
	
		// 교량 형식 및 제원
		if(m_nFlag>8)
			ar >> m_nIdxBridge;
		ar >> m_strBridgeName;
		ar >> m_nTypeBridge;	// 문형, ㅠ형, 슬래브, V형, 박스형, 경사 등..
		ar >> m_dStationBridgeStt;		// 시작 스테이션
		ar >> m_dLengthBridge;		// 총길이
		ar >> m_nQtyJigan;
		ar >> m_nSeparBri;

		// 횡단 위치 입력
		ar >> m_dWidthSlabLeft;		// 슬래브 좌측단 거리
		ar >> m_dWidthSlabRight;	// 슬래브 우측단 거리
		ar >> m_nLineLeft;			// 좌측 선형
		ar >> m_nLineBase;			// 기준 선형
		ar >> m_nLineRight;			// 우측 선형
		ar >> m_dDisExtLineSttLeft;
		ar >> m_dDisExtLineSttRight;
		ar >> m_dDisExtLineEndLeft;
		ar >> m_dDisExtLineEndRight;
		if(m_nFlag > 90)
		{
			ar >> m_dDistRoadCenterLevelLeft;		// 도로중심 레벨거리 : 좌측
			ar >> m_dDistRoadCenterLevelRight;		// 도로중심 레벨거리 : 우측
		}
		if(m_nFlag>34)
		{
			ar >> m_bUsePadShoe;
			if(m_nFlag < 103)
			{
				m_bUsePadShoe = FALSE; // 전에는 사용하지 않았고 기본값은 FALSE로 적용
			}
		}
		if(m_nFlag>2)
		{
			ar >> m_dFckMassConc;
			ar >> m_dFckLeanConc;
		}
		// 지중라멘교
		m_pLineApp->Serialize(ar);
		CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
		m_pLineApp->SetDataIp(strErrorMsg);
		if(m_nFlag>25)
			ar >> m_bQuickSearchLine;	// 빠른 선형 검색 적용
		ar >> m_bUnderGroundRahmen;
		if(m_nFlag>69)
		{
			ar >> m_bCalcOpen;
			ar >> m_bPrevApplyCalcOpen;
		}
		else
		{
			m_bCalcOpen = FALSE;
			m_bPrevApplyCalcOpen = TRUE;
		}

		ar >> m_bUnderGroundPaveDetail;
		ar >> m_dUnderGroundSlopeSlabLeft;
		ar >> m_dUnderGroundSlopeSlabRight;
		ar >> m_dUnderGroundCenterELStt;
		ar >> m_dUnderGroundCenterELEnd;
		ar >> m_dWidthRoadLeft;
		ar >> m_dWidthRoadRight;
		ar >> m_dUnderGroundNoriSlopeFirst;
		ar >> m_dUnderGroundNoriSlopeNext;
		ar >> m_dUnderGroundNoriMaxHeight;
		ar >> m_dUnderGroundNoriWidth;
		ar >> m_dUnderGroundNoriSolpeSodan;
		ar >> m_nQtyHDanRoad;
		for(i=0; i<MAX_QTY_HDAN; i++)
		{
			ar >> m_dLengthHDanRoad[i];
			if(m_nFlag > 20)
				ar >> m_bLevelHDanRoad[i];
			ar >> m_dSlopeHDanRoad[i];
			if(m_nFlag < 22)
				m_dSlopeHDanRoad[i] /=100;

			ar >> m_nTypeHDanRoad[i];
			if(m_nFlag < 16)
			{
				ar >> m_ZznVariableLenHDanRoad[i];
				if(m_ZznVariableLenHDanRoad[i] == 0)
					m_ZzbVariableLenHDanRoad[i] = FALSE;
				else
					m_ZzbVariableLenHDanRoad[i] = TRUE;

			}
			else
				ar >> m_ZzbVariableLenHDanRoad[i];
		}
		if(m_nFlag>12)
		{
			ar >> m_ZzdRail_HeightLane;
			ar >> m_dRail_HeightTie;
			ar >> m_ZzdRail_HeightDosangUnderTie;
			ar >> m_dRail_LengthTie;
			ar >> m_dRail_WidthTie;
			ar >> m_dRail_DistBetweenWheel;
			ar >> m_dRail_DistBetweenRail;
		}

		ar >> m_nTypePavement;
		ar >> m_dUnderGroundHeightPavement;
		if (m_nFlag > 109)
		{
			ar >> m_bUnderGroundDesignHeight;
		}
		ar >> m_dUnderGroundDesignHeight;
		if(m_nFlag>11)
		{
			ar >> m_nUnderGroundDesignType;			// 노면활하중 종류
			if(m_nUnderGroundDesignType<0)
				m_nUnderGroundDesignType = 0;
			ar >> m_nUnderGroundDesignSelect;			// 노면활하중 설계기준
			if(m_nUnderGroundDesignSelect<0)
				m_nUnderGroundDesignSelect = 0;
			if(m_nFlag > 135)
				ar >> m_nUnderGroundDistribute;
			ar >> m_dUnderGroundDesignLiveAlways;
		}
		ar >> m_dUnderGroundHeightMortar;
		if(m_nFlag>22)
			ar >> m_dUnderGroundWeightMortar_xx;
		ar >> m_dUnderGroundPaveHeightPyo;
		ar >> m_dUnderGroundPaveHeightGee;
		ar >> m_dUnderGroundPaveHeightMiddle;
		ar >> m_dUnderGroundPaveHeightBozo;
		ar >> m_dUnderGroundPaveHeightDong;
		ar >> m_dUnderGroundPaveHeightEps;
		if(m_nFlag>56) ar >> m_dUnderGroundPaveHeightTosa_XXX;
		ar >> m_dUnderGroundPaveWeightPyo;
		ar >> m_dUnderGroundPaveWeightGee;
		ar >> m_dUnderGroundPaveWeightMiddle;
		ar >> m_dUnderGroundPaveWeightBozo;
		ar >> m_dUnderGroundPaveWeightDong;
		ar >> m_dUnderGroundPaveWeightEps;
		if(m_nFlag>56)
		{
			ar >> m_dUnderGroundPaveWeightTosa;
			ar >> m_dUnderGroundPaveWidthPyo;
			ar >> m_dUnderGroundPaveWidthGee;
			ar >> m_dUnderGroundPaveWidthMiddle;
			ar >> m_dUnderGroundPaveWidthBozo;
			ar >> m_dUnderGroundPaveWidthDong;
			ar >> m_dUnderGroundPaveWidthEps;
			ar >> m_dUnderGroundPaveWidthTosa;
		}
		if(m_nFlag>30)
		{
			ar >> m_dOptimizeDiaMinimum;
			ar >> m_dOptimizeDiaMaximum;
			ar >> m_dOptimizeSafeRate;
			ar >> m_bOptimizeRebar2Dan;
			ar >> m_bOptimizeRebarSplit;
			if(m_nFlag>31)
			{
				ar >> m_bOptimizeRebarMatch;
			}
		}


		if(m_nFlag > 45)
		{
			ar >> m_nTypeAddRate;	// 0 : 직경별, 1 : 주철근,기타철근
			ar >> m_dAddRateLower;	// 하위 할증률
			ar >> m_dAddRateUpper;	// 상위 할증률
			ar >> m_dAddRateDiaOrg;	// 직경별일경우 하위 직경
		}

		if(m_nFlag > 102)
		{
			m_footingBox.Serialize(ar);
			ar >> m_bApplyGroundWaterLevel;
			ar >> m_dGroundWaterLevel;
			ar >> m_dCoffKh_Sei;
			ar >> m_bExistProtectionRiseFooting;	
			ar >> m_dPRF_W;						
			ar >> m_dPRF_H1;						
			ar >> m_dPRF_H2;						
			ar >> m_bPRF_SameCTC_Main;			
			ar >> m_dPRF_CTC_Main;				
			ar >> m_dPRF_CTC_Support;				
			ar >> m_dPRF_CTC_SupportVert;			
			ar >> m_dPRF_CTC_Shear;				
			ar >> m_dPRF_Dia_Main;				
			ar >> m_dPRF_Dia_Support;				
			ar >> m_dPRF_Dia_Shear;				
			ar >> m_nPRF_Type_Shear;				
			ar >> m_nPRF_Count_Shear;

			ar >> m_dTS_Lower;
			ar >> m_nTypeLower;
			ar >> m_dThickCobbStone;
			ar >> m_bSlopeLowerSlab;

			ar >> m_nCountAnchor;					// 앵커 종방향 개수
			ar >> m_bAnchorSameValue;				// 앵커력,길이,직경 동일적용 (횡방향 입력시 1열만 입력)
			m_dArrDistJongAnchor.Serialize(ar);	// 앵커 종방향 거리
			m_dArrLoadAnchor.Serialize(ar);		// 앵커력
			m_dArrLengthAnchor.Serialize(ar);		// 앵커길이
			m_dArrDiaAnchor.Serialize(ar);		// 앵커직경
			m_dArrEaAnchor.Serialize(ar);
			for(i=0; i<m_nCountAnchor; i++)
			{
				m_dArrDistHDanAnchor[i].Serialize(ar);	// 앵커 횡방향 간격
			}

			ar >> m_nGroundFactorKV;
			ar >> m_dGroundFactorKV;
			ar >> m_nGroundFactorBv;
			ar >> m_nEquivalentApplyKsb;
			if(m_nFlag > 140)
				ar >> m_dEquivalentApplyKsb;
			ar >> m_n2DGeneralSpringType;
			ar >> m_n2DEQSpringType;
			ar >> m_nVerticalFactor;
			ar >> m_dSpringSpaceBottom;
			ar >> m_dSpringSpaceWall;

			if(m_nFlag > 134)
				ar >> m_bSpringSpaceIncludeWallTick;
		}

		// 지간 구성 입력
		for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
		{
			ar >> m_bExpJoint[i];			// 신축 이음 설치 유무
		}
		if(m_nFlag > 102)
		{
			ar >> m_nExpJointType[iSTT];
			ar >> m_nExpJointType[iEND];
		}
		for(i=0; i<MAX_JIJUM_COUNT_RC-1; i++)
		{
			ar >> m_dLengthJigan[i];			// 지간 길이
		}
		m_arrStrJijumLen.Serialize(ar);
		m_arrJijumLen.Serialize(ar);
		ar >> m_bEWDlgUsed;
		if(m_nFlag >= 7)
			ar >> m_nTypeShoeAngle;
		if(m_nFlag > 14)
			ar >> m_bIsExp;			// 가각(학폭) 지원할지?
		if(m_nFlag > 102)
		{
			m_pDesignExpanJoint->Serialize(ar);
		}
		// 확폭 제원
		for(i=0; i<2; i++)
		{
			ar >> m_dExpSttW[i];	// 시점부
			ar >> m_dExpSttH[i];
			ar >> m_dExpSttR[i];
			ar >> m_dExpEndW[i];	// 종점부
			ar >> m_dExpEndH[i];
			ar >> m_dExpEndR[i];
			if(m_nFlag > 57)
			{
				ar >> m_bExpCutStt[i];	// [0] : 좌
				ar >> m_bExpCutEnd[i];	// [1] : 우
			}
			if(m_nFlag > 70)
			{
				ar >> m_bExpSttRoundIsTan[i];	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
				ar >> m_bExpEndRoundIsTan[i];
			}
		}

		// 교량 받침 위치 입력
		if(m_nFlag < 79)
		{
			AhTPASerial(ar, &m_pArrJijum, (CJijum*)0);		// 지점 데이터
			// CJijum에 Flag가 없어서 여기서 저장...
			if(m_nFlag>34) 
			{
				for(i=0; i<m_pArrJijum.GetSize(); i++)
				{
					int j=0; for(j=0; j<MAX_SHOE_COUNT_RC_OLD; j++)
					{
						ar >> m_pArrJijum.GetAt(i)->m_dKh[0][j];
						ar >> m_pArrJijum.GetAt(i)->m_dKh[1][j];
						ar >> m_pArrJijum.GetAt(i)->m_dKv[0][j];
						ar >> m_pArrJijum.GetAt(i)->m_dKv[1][j];
					}
				}
			}
			// CJijum 클래스와 CShoePos 클래스에서 받음.		
		}
		else
		{
			long nCount	= 0;
			ar >> nCount;
			AhTPAMake(nCount, &m_pArrJijum, (CJijum*)0);
			for(i = 0; i < m_pArrJijum.GetSize(); i++)
				m_pArrJijum.GetAt(i)->SerializeNew(ar);
		}



		// 횡단 구성 입력
		ar >> m_nQtyHDan;			// 횡단 구성 개수
		for(i=0; i<MAX_QTY_HDAN; i++)
		{
			ar >> m_dLengthHDan[i];		// 각 횡단 구간의 길이
			ar >> m_bLevelHDan[i];	// 각 구간의 레벨 처리 유무
			ar >> m_nTypeHDan[i];		// 각 구간의 구성 형태
			ar >> m_ZzbVariableLenHDan[i];		// 가변 구간 설정 유무
			ar >> m_dLengthRail[i];
			ar >> m_nLineNumber[i];
			ar >> m_dSlopeHDan[i];
			if(m_nFlag < 17 && !m_pARcBridgeDataStd->IsRailDesign())
			{
				if(m_nTypeHDan[i] == 2)			//방호벽(우) 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_GUARDFENCE;
				else if(m_nTypeHDan[i] == 3)	//중분대 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_CENTERGUARD;
				else if(m_nTypeHDan[i] == 4)	//중분대(1/2좌) 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_CENTERGUARDHALF;
				else if(m_nTypeHDan[i] == 5)	//중분대(1/2우) 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_CENTERGUARDHALF;
				else if(m_nTypeHDan[i] == 6)	//보도 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_FOOTWAY;
				else if(m_nTypeHDan[i] == 7)	//보차도 경계석 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_FOOTWAYBLOCK;
				else if(m_nTypeHDan[i] == 8)	//방음벽 기초 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_SOUNDPROOF;
				else if(m_nTypeHDan[i] == 9)	//흙막이벽 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_RETAININGWALL;
				else if(m_nTypeHDan[i] == 10)	//난간기초 일 경우
					m_nTypeHDan[i] = HDAN_TYPE_ROAD_HANDRAIL;
			}

		}
		AhTPASerial(ar, &m_pArrGuardWall, (CGuardWallRC*)0);
		if(m_nFlag>17)
			AhTPASerial(ar, &m_pArrGuardWallRoad, (CGuardWallRC*)0);
		AhTPASerial(ar, &m_pArrFootway, (CFootway*)0);
		ar >> m_bRoundGW;
		ar >> m_bRoundFW;
		ar >> m_bGuardWallDlgUsed;
		if(m_nFlag>19)
			ar >> m_bGuardWallUnderDlgUsed;
		ar >> m_bSSDlgUsed;

		ar >> m_nRoadMaterial;
		ar >> m_dTP;	// 포장 두께
		ar >> m_dTS;	// 슬래브 두께
		if(m_nFlag < 97)	// 기존의 파일은 m_dTS로 셋팅해준다. 
			m_dTSOld = m_dTS;
		else
			ar >> m_dTSOld;

		for(i=0; i<2; i++)
		{
			ar >> m_dDH[i];		// 슬래브 끝단에서 헌치까지의 거리
			ar >> m_bCantilSlab[i];
			ar >> m_nSlabChemfer[i];
			ar >> m_dCantilH1[i];
			ar >> m_dCantilH2[i];
			ar >> m_dCantilB[i];
			ar >> m_dRE1[i];	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
			ar >> m_dRE2[i];	// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
			ar >> m_dTCW1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 폭
			ar >> m_dTCH1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 높이
			ar >> m_dTCW2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 폭
			ar >> m_dTCH2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 높이
		}

		// 설계조건 -------------------------------------------------------
		ar >> m_dUnitWeigthBackFill;		// 뒷채움의 단위중량
		if(m_nFlag > 102)
		{
			ar >> m_dUnitWeigthBackFillWater;
		}
		ar >> m_dAngleFrictionBackFill;		// 뒷채움의 내부 마찰각
		ar >> m_dLiveLoadAlways;			// 상재하중
		if(m_nFlag>81)
			ar >> m_dDeadLoadAlways;
		if(m_nFlag>50)
			ar >> m_nSelectLiveLoadHori;
		ar >> m_dLiveLoadFootway;
		ar >> m_dUnitWeightHeapSoil;		// 성토재료의 단위중량
		ar >> m_dUnitWeightValidDepth;		// 근입지반의 단위중량
		ar >> m_dValidDepthBaseGround;		// 기초의 유효근입 깊이
		if(m_nFlag>24)
			ar >> m_dEo_CalcWallSpringEQ;

		long nLong = 0;

		if(m_nFlag<29)
			ar >> nLong;
		ar >> m_dNValue_Eo_Hori_XXX;			// 수평 지반변형계수
		if(m_nFlag<29)
			ar >> nLong;
		ar >> m_dNValue_Eo_Vert_XXX;			// 수직 지반변형계수
		ar >> m_nTypeToapSafeCheck;			// 안정검토시의 토압검토
		ar >> m_SafeFactorOverturning[0];	// 전도에 대한 안정률(평상시)
		ar >> m_SafeFactorOverturning[1];	// 전도에 대한 안정률(지진시)
		ar >> m_SafeFactorSliding[0];		// 활동에 대한 안정률(평상시)
		ar >> m_SafeFactorSliding[1];		// 활동에 대한 안정률(지진시)
		ar >> m_nSelectModelingCountEQ;
		if(m_nFlag > 138)
		{
			for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
				ar >> m_bGoalEarthQuake[ix];
		}
		else
		{
			m_bGoalEarthQuake[eGoalEQ_Collapse] == (m_nSelectModelingCountEQ==0 || m_nSelectModelingCountEQ==2) ? TRUE : FALSE;
			m_bGoalEarthQuake[eGoalEQ_Function] == (m_nSelectModelingCountEQ==1 || m_nSelectModelingCountEQ==2) ? TRUE : FALSE;
		}
		if(m_nFlag >68) ar >> m_nSelectFootForce;
		ar >> m_nSelect2DFrameTool;
		if(m_nFlag < 61 && m_nSelect2DFrameTool>0)
			m_nSelect2DFrameTool+=3;

		ar >> m_nSelect3DShellTool;
		if(m_nFlag > 123)
			ar >> m_nSelectMidasVer;

		if(m_nFlag > 102)
		{
			ar >> m_nSelectWingCalc;
		}
		if(m_nFlag>35)
		{
			ar >> m_nSelect3DOutputValue;
			if(m_nSelect3DOutputValue<0 || m_nSelect3DOutputValue>2) m_nSelect3DOutputValue = 2;

			if(m_nFlag > 114)
				ar >> m_nApplyWingWallCalcValueCount;
		}
		if(m_nFlag > 98)
		{
			ar >> m_bApplyUserDispMax;
			ar >> m_dUnitLoadDispMaxUser;
		}
		if(m_nFlag > 118)
			ar >> m_nSelectWingLoad_LS_ES;
		if(m_nFlag > 120)
		{
			ar >> m_nLiveLoadDistToBackWall;
			ar >> m_bCorrectionFactor_LiveLoadAlways;
			ar >> m_dCorrectionFactor_LiveLoadAlways;
			if(m_nFlag > 139)
				ar >> m_nApplyWingWallHeightBasePos;
		}

		if( m_nFlag > 110)
			ar >> m_dEo_Input_Pile_XXX;

		if( m_nFlag > 111 )
		{
			for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
			{
				ar >> m_bAutoGroundInfo[ix];
				if(m_nFlag > 145)
				{
					ar >> m_bAutoCalcPileProperty[ix];
					ar >> m_nSelect_Eo_VertType[ix];
					ar >> m_nSelect_Eo_HoriType[ix];
				}
			}
		}
		// ------------------------------------------------------ 설계조건
		m_MatComboUse.Serialize(ar);
		m_MatComboUlt.Serialize(ar);
		m_MatComboEcc.Serialize(ar);
		m_MatComboEQ.Serialize(ar);
		ar >> m_dWLoadNormal;
		ar >> m_dWLoadEQ;
		long nLoadBasic = (m_nFlag > 102) ? BRIDGE_LOAD_BASIC : BRIDGE_LOAD_BASIC - 2;
		long i=0; 
		for(i=0; i<nLoadBasic; i++)
		{
			ar >> m_bApplyLoad[i];
		}
		if(m_nFlag>7)
		{
			ar >> m_bApplyLiveLoadEQ;	// 지진시 활하중재하
			if(m_nFlag > 104)
			{
				ar >> m_bApplyAnchorForce;
			}
			
			for(i=0; i<RCBRIDGE_COMBO_COUNT; i++)
			{
				long nMaxCombo = (m_nFlag < 106) ? 100 : MAX_RCBRIDGE_COMBO;
				for(long j=0; j<nMaxCombo; j++)
				{
					ar >> m_bApplyCombo3D[i][j];
					if(m_nFlag > 102)
					{
						ar >> m_bApplyGroundWater[i][j];
						if(m_nFlag > 104)
						{
							ar >> m_dFactorAnchor[i][j];
						}
					}
				}
			}
		}

		if(m_nFlag > 86) ar >> m_dFactorComboVLoad;

		// 설계선택사항 -------------------------------------------------------
		if(m_nFlag > 44) ar >> m_bCheckMainSlabAndFootTemper;
		ar >> m_bSelectSupportBarWall;			// 수평철근량검토시 노출면 배치
		ar >> m_bCheckSuppoerBarSlabMain;
		ar >> m_bCheckSuppoerBarSlab300;
		ar >> m_bCheckSuppoerBarSlabArea;
		ar >> m_bCheckSuppoerBarWall15hs;
		ar >> m_bCheckSuppoerBarWall300;
		ar >> m_bCheckSuppoerBarWallArea;
		ar >> m_bCheckSuppoerBarFoot15hs;
		ar >> m_bCheckSuppoerBarFoot300;
		ar >> m_bCheckSuppoerBarFootArea;
		if(m_nFlag > 75)
		{
			ar >> m_bCheckSuppoerBarWing15hs;		// 수평철근비 날개벽 0.0015hs
			ar >> m_bCheckSuppoerBarWing300;		// 수평철근비 날개벽 300mm²이상
			ar >> m_bCheckSuppoerBarWingArea;		// 수평철근비 날개벽 단면적비
		}
		ar >> m_dCheckSuppoerBarWallArea;
		if(m_nFlag > 89)
		{
			ar >> m_dCheckSuppoerBarWallAreaForOver400;
		}
		ar >> m_dCheckSuppoerBarFootArea;
		ar >> m_dCheckSuppoerBarSlabArea;
		if(m_nFlag > 75)
		{
			ar >> m_dCheckSuppoerBarWingArea;
		}
		if(m_nFlag > 119)
		{
			ar >> m_bCheckSupportWallDryingShrinkage;
			ar >> m_bCheckSupportWallMainRate;
			ar >> m_bCheckSupportFootDryingShrinkage;
			ar >> m_bCheckSupportSlabDryingShrinkage;
			ar >> m_bCheckSupportSlabByReqRebar;
			ar >> m_bCheckSupportSlabMainRate;
			ar >> m_bCheckSupportSlabMainRateSpan;
			if(m_nFlag > 126)
			{
				ar >> m_bCheckSuppoerBarSlabMainOver20;
				ar >> m_bCheckSuppoerBarWallMainOver20;
				ar >> m_bCheckSuppoerBarFootMainOver20;
			}
			if(m_nFlag > 127)
			{
				ar >> m_bApplyT1CalcWingWall;
			}
			if(m_nFlag > 121)
			{
				ar >> m_bSkipAsMinCheckByHoriRebar;
			}
		}
		if(m_nFlag>0)
		{
			ar >> m_nSelectPileCalcSpringEQ;
		}
		if(m_nFlag > 102)
		{
			ar >> m_nSelectApsShearCheck;
		}
		ar >> m_bApplyMinAvSlab;				// 전단검토시 최소전단검토
		ar >> m_bApplyMinAvWall;			// 전단검토시 최소전단검토
		ar >> m_bApplyMinAvFoot;			// 전단검토시 최소전단검토
		ar >> m_bApplyMinAvWing;			// 전단검토시 최소전단검토
		if(m_nFlag>26)
			ar >> m_bApplyMinAvAps;
		if(m_nFlag > 129)
			ar >> m_bApplyMinAvHyunTa;
		if(m_nFlag>32)
			ar >> m_bCheckMinCtcShearWing;
		if(m_nFlag>62)
			ar >> m_bCheckMinCtcShearAps;
		if(m_nFlag>63)
			ar >> m_bCheckCornerInside;
		ar >> m_bApplyFootingGForce;
		if(m_nFlag>27)
			ar >> m_bApplyMassCalcSafe;
		if(m_nFlag>74)
			ar >> m_bApplyFootingMinThick;
		if(m_nFlag > 103)
		{
			ar >> m_bCheckRebarRateFooting;

			if(m_nFlag > 142)
				ar >> m_nSelectApplyFs;
			else
				m_nSelectApplyFs = 1;
		}
		ar >> m_nApplyCalcShearDistFront;		// 앞굽 전단검토시 위험단면 위치
		ar >> m_nApplyCalcShearDistBack;		// 뒷굽 전단검토시 위험단면 위치
		ar >> m_nTypeCrackDesignSlab;		// 강재의 부식에 대한 환경조건(상부슬래브)
		ar >> m_nTypeCrackDesignWall;			// 강재의 부식에 대한 환경조건(벽체)
		ar >> m_nTypeCrackDesignFoot;		// 강재의 부식에 대한 환경조건(기초(하부슬래브))
		ar >> m_nTypeCrackDesignWing;
		if(m_nFlag > 64)
			ar >> m_nTypeCrackDesignAps;
		ar >> m_nSelectJointUpperRange;			// 상부이음, 정착장 적용 범위
		ar >> m_nIdxCountCycleMainRebar;		// 주철근 배치 옵션(CYCLE)
		if(m_nFlag >10)
		{
			ar >> m_dFactorToap;
			ar >> m_dFactorLive;
		}
		if(m_nFlag >38)
		{
			ar >> m_nSelFactorKhStt;
			ar >> m_nSelFactorKhEnd;

			// (ARCBRIDGE-3535) 0번 선택 삭제
			if(m_nSelFactorKhStt == 0)
				m_nSelFactorKhStt = 1;
			if(m_nSelFactorKhEnd == 0)
				m_nSelFactorKhEnd = 1;
		}
		if(m_nFlag > 41) ar >> m_nSelCalcCornerFt;
		if(m_nFlag > 53)
		{
			ar >> m_nSelectForceCheckPos;			// 단면검토 절점 위치 (0-강역,1-벽체내측)
			if(m_nFlag>58)
				ar >> m_nSelectRigidModeling;
			if(m_nFlag>79)
				ar >> m_bSelectRigidCalcHunch;
			if(m_nFlag > 87)
			{
				ar >> m_bSelectTempLoadAddWall;
				ar >> m_bSelectShrinkLoadAddWall;
				if(m_nFlag > 102)
				{
					ar >> m_bSelectEnvToapOne;
					ar >> m_bSelectEnvSubsid;
					ar >> m_bSelectHunchModeling;
					ar >> m_bSelectHunchDesignCheck;
				}
			}
			if(m_nFlag > 99)
				ar >> m_nSelectTempLoadSlab;
			if(m_nFlag>66)
				ar >> m_nSelSubsideType;
			ar >> m_nSelectDesignRoadWidth;			// 설계차로폭 선택	  (0-3.0, 1-W계산)
			if(m_nFlag>106)
				ar >> m_nSelectLiveLoadSpanLength;
			if(m_nFlag>65)
				ar >> m_nSelectDesignSpanLength;
			if(m_nFlag>93)
				ar >> m_nSelectDesignSpanApply;
			if(m_nFlag>88)
				ar >> m_bSelectSpanLengthApplyShoe;
			if(m_nFlag>80)
				ar >> m_nSelectWallSectionDesign;
			if(m_nFlag > 95)
				ar >> m_nSelectInWallSectionDesign;
			if(m_nFlag>92)
				ar >> m_nSelectFootingReactionForce;
			if(m_nFlag>94)
				ar >> m_nArchRibSectionMomentPos;
			if(m_nFlag>83)
				ar >> m_nSelectModelingHeight;
			if(m_nFlag>84)
				ar >> m_nSelectAlphaKhKv_XXX;
			if(m_nFlag>85)
				ar >> m_bCalcShearCenter;
			else m_bCalcShearCenter = FALSE;
			if(m_nFlag>100)
				ar >> m_nSelectStartLane;
			if(m_nFlag>101)
			{
				ar >> m_bCheckUseri;		
				ar >> m_dCheckUseriValue;
			}
			if(m_nFlag > 117)
				ar >> m_nSelectApsAxleWeight;
			if(m_nFlag > 125)
				ar >> m_nSelectApsDeadLoad;
			if(m_nFlag > 122)
				ar >> m_bApsDowelSeparateRebar;
			if(m_nFlag > 144)
				ar >> m_bOutWallUsebilityCheck;
		}
		if(m_nFlag > 55)
		{
			ar >> m_bCheckSuppoerBarArchRibMinDia1;
			ar >> m_bCheckSuppoerBarArchRibMinDia2;
		}
		if(m_nFlag > 97)
			ar >> m_bCheckFrontFootTrap2;
		// ------------------------------------------------------ 설계선택사항

		// 수량선택사항 -------------------------------------------------------
		ar >> m_nFormType_ConcNormal;			// 거푸집 : 일반콘크리트
		ar >> m_nFormType_Footing;				// 거푸집 : 기초
		ar >> m_nFormType_WallFront;			// 거푸집 : 벽체 전면
		ar >> m_nFormType_WallBack;				// 거푸집 : 벽체 후면
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				ar >> m_nFormType_WingFront[stt][left];			// 거푸집 : 날개벽 전면
				ar >> m_nFormType_WingBack[stt][left];				// 거푸집 : 날개벽 배면
			}
		}
		ar >> m_nFormType_Aps;					// 거푸집 : 접속슬래브
		ar >> m_nFormType_MassConc;				// 거푸집 : mass 콘크리트
		ar >> m_nFormType_LeanConc;				// 거푸집 : 버림 콘크리트
		ar >> m_bSelectBM_ConcCare_Main;		// 콘크리트양생 : 교대본체, 날개벽
		ar >> m_bSelectBM_ConcCare_Aps;			// 콘크리트양생 : 접속슬래브, 완충슬래브
		if(m_nFlag > 136)
			ar >> m_nSelectBM_ConcCare_Cal;			// 콘크리트양생 산출방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
		ar >> m_nTypeRcBridgeBlock;				// 교대보호블럭 설치
		ar >> m_dAngleRcBridgeBlock;			// 교대보호블럭 외측설치각도(도)
		ar >> m_bSelectBM_ApsCnsInclude;		// 수량집계에 접속슬래브, 완충슬래브 포함
		ar >> m_dPavementThick;					// 뒷채움시 콘크리트 포장두께
		ar >> m_dNosangThick;					// 뒷채움시 노상두께
		if(m_nFlag > 5)
		{
			ar >> m_nTypeHeightWall;			// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이
			if(m_nFlag > 91)
				ar >> m_nTypeSlabLength;				// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
			ar >> m_nTypeSurfaceProof;			// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
		}
		ar >> m_nTypePost;						// 본체 동바리 타입(육교용, 일반용)
		ar >> m_nTypePostBaseLine;				// 동바리 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
		if(m_nFlag > 46)
			ar >> m_dPostLineEL;				// 사용자 입력 동바리 기준선 EL
		if(m_nFlag > 71)
		{
			ar >> m_nTypeScaffoldBaseLine;		// 비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
			ar >> m_dScaffoldLineEL;				// 사용자 입력 비계 기준선 EL
		}
		if(m_nFlag > 137)
			ar >> m_nTypeScaffoldSideLocation;
		else
			m_nTypeScaffoldSideLocation = SCOFFOLD_WING_NOTCON;
		ar >> m_dThickPostUpper;				// 육교용 동바리 상부 두께
		ar >> m_dThickPostSide;					// 육교용 동바리 좌우측 두께
		ar >> m_dThickScaffold;					// 비계 두께

		if (m_nFlag > 113)
		{
			ar >> m_nDeductNearFormBM;			// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
			ar >> m_nDeductNearTupagiBM;		// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
		}

		if(m_nFlag > 13)
		{
			ar >> m_dAggregate_Footing;			// 콘크리트 골재최대치수(기초)
			ar >> m_dAggregate_Main;			// 콘크리트 골재최대치수(본체)
			ar >> m_dAggregate_Aps;				// 콘크리트 골재최대치수(접속슬래브)
			ar >> m_dAggregate_Lean;			// 콘크리트 골재최대치수(버림)
			ar >> m_dAggregate_Mass;			// 콘크리트 골재최대치수(MASS)
			if (m_nFlag > 112)
			{
				ar >> m_dAggregate_Hyunta;		// 콘크리트 골재최대치수(현장타설말뚝)
			}
			ar >> m_dAggregate_Etc;				// 콘크리트 골재최대치수(방호벽등)

			ar >> m_dSlump_Footing;				// 콘크리트 Slump(기초)
			ar >> m_dSlump_Main;				// 콘크리트 Slump(본체)
			ar >> m_dSlump_Aps;					// 콘크리트 Slump(접속슬래브)
			ar >> m_dSlump_Lean;				// 콘크리트 Slump(버림)
			ar >> m_dSlump_Mass;				// 콘크리트 Slump(MASS)
			if (m_nFlag > 112)
			{
				ar >> m_dSlump_Hyunta;			// 콘크리트 Slump(현장타설말뚝)
			}
			ar >> m_dSlump_Etc;					// 콘크리트 Slump(방호벽등)
		}

		if(m_nFlag > 23)
		{
			if(m_nFlag<38)
			{
				ar >> m_bMonumenu;				// 교명주 받침 설치(OLD)
				if(m_bMonumenu)
					m_nMonumenu = 4;
				else
					m_nMonumenu = 0;
			}
			else
				ar >> m_nMonumenu;				// 교명주 받침 설치

			if(m_nFlag > 76)
			{
				ar >> m_nDesPanel;				// 설명판 설치개수
				ar >> m_nNamePanel;				// 교명판 설치개수
			}

			ar >> m_dAddLenghForGuardWallBM;	// 방호벽 수량 산출시 추가 길이(OLD)
			if(m_nFlag>48)
			{
				ar >> m_dAddLenghForGuardWall[0][0];// 방호벽 수량 산출시 추가 길이 시점 좌측
				ar >> m_dAddLenghForGuardWall[0][1];// 방호벽 수량 산출시 추가 길이 종점 우측
				ar >> m_dAddLenghForGuardWall[1][0];// 방호벽 수량 산출시 추가 길이 시점 좌측
				ar >> m_dAddLenghForGuardWall[1][1];// 방호벽 수량 산출시 추가 길이 종점 우측
			}
			else
			{
				m_dAddLenghForGuardWall[0][0] = m_dAddLenghForGuardWallBM;
				m_dAddLenghForGuardWall[0][1] = m_dAddLenghForGuardWallBM;
				m_dAddLenghForGuardWall[1][0] = m_dAddLenghForGuardWallBM;
				m_dAddLenghForGuardWall[1][1] = m_dAddLenghForGuardWallBM;
			}
		}
		
		if(m_nFlag>36)
		{
			ar >> m_nHeightForSlabBM;			// 슬래브 거푸집 수량 산출시 높이 기준 
		}
		if(m_nFlag > 67)
		{
			ar >> m_dTackCotting;
			ar >> m_nTypePileMilkBM;
		}
		else
		{
			m_dTackCotting	= 2;
			m_nTypePileMilkBM	= 0;			
		}

		if(m_nFlag > 131)	ar >> m_bExcludeApsDowelRebar;
		else					m_bExcludeApsDowelRebar = FALSE;

		if(m_nFlag > 132)	ar >> m_dTimeConcForm;
		else					m_dTimeConcForm = 0;

		if(m_nFlag > 143)
			ar >> m_bApplyYongSu;

		if(m_nFlag > 141)
		{
			ar >> m_dSurplusSoil_HyunTaPile;		
			ar >> m_dConcreteSoil_HyunTaPile;		
			ar >> m_dConcreteRock_HyunTaPile;	
		}
		else
		{
			m_dSurplusSoil_HyunTaPile = 70;		
			m_dConcreteSoil_HyunTaPile = 8;		
			m_dConcreteRock_HyunTaPile = 14;	
		}

		// ------------------------------------------------------ 수량선택사항

		// 피복설정 -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar >> m_dCoverUpper_UpperSlab[i];	// 상부슬래브 상면
			ar >> m_dCoverLower_UpperSlab[i];	// 상부슬래브 하면
			if(m_nFlag > 4)
				ar >> m_dCoverSide_UpperSlab[i];	// 상부슬래브 측면
			else
				m_dCoverSide_UpperSlab[i]	= m_dCoverUpper_UpperSlab[i];
			ar >> m_dCoverUpper_LowerSlab[i];	// 하부슬래브 상면
			ar >> m_dCoverLower_LowerSlab[i];	// 하부슬래브 하면
			ar >> m_dCoverWallOut[i];	// 벽체외면
			ar >> m_dCoverWallIn[i];	// 벽체 내면
			ar >> m_dCoverWallMid[i];	// 중간벽체
			ar >> m_dCoverColumn[i];	// 기둥
			ar >> m_dCoverWingOut[i];	// 날개벽 외면
			ar >> m_dCoverWingIn[i];	// 날개벽 내면
			ar >> m_dCoverUpper_Footing[i];	// 기초 상면
			ar >> m_dCoverLower_Footing_Lean[i];	// 기초 하면(직접기초시)
			ar >> m_dCoverLower_Footing_Pile[i];	// 기초 하면(말뚝기초시)
			if(m_nFlag > 52)
			{
				ar >> m_dCoverRib[iUPPER][i];
				ar >> m_dCoverRib[iLOWER][i];
			}
		}
		ar >> m_dCoverWingUpper;
		ar >> m_dCoverWingEnd;
		// ------------------------------------------------------ 피복설정
		if(m_nFlag > 73)
		{
			ar >> m_bPileNvalueAuto;

			// 변수가 m_bPileNvalueUser => m_bPileNvalueAuto 로 변경 되면서 저장된 값을 바꿔준다.
			if(m_nFlag < 109)
				m_bPileNvalueAuto = !m_bPileNvalueAuto;
		}
		else m_bPileNvalueAuto = FALSE;

		// 종단면형상 -------------------------------------------------------
		ar >> m_nTypeJongdanForRebar;
		m_nTypeJongdanForRebar = MIN(1,m_nTypeJongdanForRebar);
		m_nTypeJongdanForRebar = MAX(0,m_nTypeJongdanForRebar);

		if(m_nFlag > 29)
			ar >> m_nTypeWallForRebarGen;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
		ar >> m_nTypeOutWall;	// 외측벽체 유무(좌우)
		if(m_nFlag > 59)
			m_fxJointStt.Serialize(ar);

		// 시점측 벽체
		ar >> m_dFHS;
		ar >> m_dFLS;
		ar >> m_dFRS;
		ar >> m_dWS;
		if(m_nFlag > 39)
		{
			ar >> m_dWSHW;	// 시점벽체 헌치너비(경사교2에서만 지원)
			ar >> m_dWSHH;	// 시점벽체 헌치너비(경사교2에서만 지원)
		}
		ar >> m_dBHWS;
		ar >> m_dBHHS;
		ar >> m_dBHS;
		ar >> m_dBTHS;
		ar >> m_dBWS;
		ar >> m_dHWS;
		ar >> m_dHHS;

		if(m_nFlag<10)
		{
			if(m_nFlag > 1)
				ar >> m_dHHS;			// 시점벽체가 없는 경우의 헌치 높이(고정이 아니기 때문에 새로 추가함)
		}
		if(m_nFlag > 3)
			ar >> m_dHRS;			// 시점헌치 반경
		ar >> m_nQtyRail;
		ar >> m_dDisBaseElevation;
		if(m_nFlag > 128)
			ar >> m_dUnitWeightRail;
		ar >> m_dSlopeDosang;
		ar >> m_dHeightDosangUnderTie;
		if(m_nFlag > 18)
		{
			ar >> m_bLevelFrRoadCenterToEdge;
			if(m_nFlag > 82)
				ar >> m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
			ar >> m_dDisBaseElevationRL;
			ar >> m_dHeightRail;
			ar >> m_bApplyLineSlope;
		}
		ar >> m_dELSB;
		m_footingStt.Serialize(ar);
		ar >> m_bDivisionApsStt;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
		for(left = 0; left < 2; left++)
		{
			ar >> m_nCountCnsStt[left];	// 완충슬래브 수
			m_apsStt[left].Serialize(ar);
			for(cns = 0; cns < MAX_CNS; cns++)
				m_cnsStt[left][cns].Serialize(ar);
		}
		
		//m_ExFootingStt.Serialize(ar);

		// 종점측 벽체
		if(m_nFlag > 59)
			m_fxJointEnd.Serialize(ar);
		ar >> m_dFHE;
		ar >> m_dFLE;
		ar >> m_dFRE;
		ar >> m_dWE;
		if(m_nFlag > 39)
		{
			ar >> m_dWEHW;	// 종점벽체 헌치너비(경사교2에서만 지원)
			ar >> m_dWEHH;	// 종점벽체 헌치너비(경사교2에서만 지원)
		}
		ar >> m_dBHWE;
		ar >> m_dBHHE;
		ar >> m_dBHE;
		ar >> m_dBTHE;
		ar >> m_dBWE;
		ar >> m_dHWE;
		ar >> m_dHHE;

		if(m_nFlag<10)
		{
			if(m_nFlag > 1) // m_dHHE2 삭제
				ar >> m_dHHE;			// 종점벽체가 없는 경우의 헌치 높이(고정이 아니기 때문에 새로 추가함)
		}
		if(m_nFlag > 3)
			ar >> m_dHRE;			// 종점헌치 반경
		ar >> m_dELEB;
		m_footingEnd.Serialize(ar);
		ar >> m_bDivisionApsEnd;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
		for(left = 0; left < 2; left++)
		{
			ar >> m_nCountCnsEnd[left];	// 완충슬래브 수
			m_apsEnd[left].Serialize(ar);
			for(cns = 0; cns < MAX_CNS; cns++)
				m_cnsEnd[left][cns].Serialize(ar);
		}
		//m_ExFootingEnd.Serialize(ar);

		// 내측벽체
		ar >> m_nCountInWallHunch;

		long nCntUL = (m_nFlag > 102)? 2 : 1;
		for(long upper=0; upper<nCntUL; upper++)
		{
			for(i = 0; i < m_nCountInWallHunch; i++)
			{
				ar >> m_dHHL[i][upper];
				if(m_nFlag > 5)
					ar >> m_dHHR[i][upper];
				else
					m_dHHR[i][upper] = m_dHHL[i][upper];
				ar >> m_dHBH[i][upper];
				ar >> m_dHLW[i][upper];
				ar >> m_dHRW[i][upper];
				ar >> m_dWL[i][upper];
				ar >> m_dWR[i][upper];
				if(m_nFlag > 3)
				{
					ar >> m_dHLR[i][upper];	// 내측헌치 왼쪽 반경
					ar >> m_dHRR[i][upper];	// 내측헌치 오른쪽 반경
				}
			}
		}

		if(m_nFlag > 33)
		{
			ar >> m_nTypeRoundOfHunchStt;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
			ar >> m_nTypeRoundOfHunchEnd;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
			for(i = 0; i < m_nCountInWallHunch; i++)
			{
				for(left = 0; left < 2; left++)
					ar >> m_nTypeRoundOfHunchInWall[i][left];	// 내측헌치 좌우측
			}
		}

		if(m_nFlag > 54)
			ar >> m_bConnectRibAndUpperSlab;
		if(m_nFlag > 40)
		{
			if(m_nFlag > 51)
			{
				long upper = 0; for(upper = 0; upper < 2; upper++)
					m_pbRib[upper].Serialize(ar);
				m_pbRibCen.Serialize(ar);
			}
			else
			{
				for(stt = 0; stt < 2; stt++)
				{
					// 포물선(리브정의) 2개 (상판 하판)
					ar >> m_pbRib[stt].m_xyCrown;
					ar >> m_pbRib[stt].m_dDistTotal;
					ar >> m_pbRib[stt].m_dHeightTotal;
					ar >> m_pbRib[stt].m_dDistStt;
					ar >> m_pbRib[stt].m_dDistEnd;
					ar >> m_pbRib[stt].m_dFactorM;
					ar >> m_pbRib[stt].m_dLenArchLine;
					ar >> m_pbRib[stt].m_dOffsetFromUpper;
				}

				if(m_nFlag > 49)
				{
					// 포물선(리브정의) 중심선
					ar >> m_pbRibCen.m_xyCrown;
					ar >> m_pbRibCen.m_dDistTotal;
					ar >> m_pbRibCen.m_dHeightTotal;
					ar >> m_pbRibCen.m_dDistStt;
					ar >> m_pbRibCen.m_dDistEnd;
					ar >> m_pbRibCen.m_dFactorM;
					ar >> m_pbRibCen.m_dLenArchLine;
					ar >> m_pbRibCen.m_dOffsetFromUpper;	// 현재 포물선의 기준상단에서의 떨어진 거리
				}
			}

			for(i = 0; i < MAX_STRUP_REBAR; i++)
			{
				ar >> m_dRibBlockW[i];	// 리브위 사각 블럭 폭
				ar >> m_dRibBlockH[i];	// 리브위 사각 블럭 높이
			}
		}

		// ------------------------------------------------------ 종단면형상

		// 내부 벽체
		AhTPADelete(&m_pArrInWall, (CWallApp*)0);
		ar >> nSize;
		for(i = 0; i < nSize; i++)
		{
			m_pArrInWall.Add(new CWallApp);
			m_pArrInWall[i]->Serialize(ar);
		}

		// 날개벽 제원 입력
		m_pWingWallStt[0]->Serialize(ar);
		m_pWingWallStt[1]->Serialize(ar);
		m_pWingWallEnd[0]->Serialize(ar);
		m_pWingWallEnd[1]->Serialize(ar);
		ar >> m_bAutoWingEl;
		if(m_nFlag > 124)
			ar >> m_bAutoWingHeightSoil;

		// 브래킷 길이
		ar >> m_bBracketLengthStt;
		ar >> m_bBracketLengthEnd;
		for(i=0; i<2; i++)
		{
			ar >> m_dBrkLenStt[i];
			ar >> m_dBrkLenEnd[i];
		}

		// 교명주 받침 제원 입력
		ar >> m_dMonuH1;
		ar >> m_dMonuH2;
		ar >> m_dMonuH3;
		ar >> m_dMonuD;
		ar >> m_dMonuW;
		ar >> m_dMonuL;
		ar >> m_dMonuFck;
		ar >> m_dMonuR_Fy;
		ar >> m_dMonuR_DiaHor;
		ar >> m_dMonuR_DiaVer;
		ar >> m_dMonuR_H;
		ar >> m_dMonuR_L;
		ar >> m_nMonuR_nA;
		ar >> m_dMonuR_A;
		ar >> m_dMonuR_A1;
		ar >> m_nMonuR_nB;
		ar >> m_dMonuR_B;
		ar >> m_nMonuR_nC;
		ar >> m_dMonuR_C;
		ar >> m_dMonuR_C1;
		ar >> m_dMonuR_C2;
		ar >> m_dMonuR_C3;
		ar >> m_nMonuR_nD;
		ar >> m_dMonuR_D;
		ar >> m_dMonuR_D1;
		ar >> m_dMonuR_D2;
		ar >> m_dMonuR_D3;
		if(m_nFlag > 130)
			ar >> m_dMonuAggregate;
		AhTPASerial(ar, &m_pArrTypeDefault, (CTypeDefault*)0);
		
		// 기타
		if(m_nFlag > 37)
		{
			ar >> m_dOther_Fck;
			ar >> m_dOther_Fy;
		}

		// 속도개선용(저장을 안 하려 했지만, 읽을때 너무 오래 걸린다는 민원이 들어옴) ////////////
		if(m_nFlag > 42)
		{
			// 속도개선을 위해서 종단면 입력이 끝나면 직방향과 사방향 좌우중심선형기준으로한 종단면을 모두 구해 놓는다.
			// 그리고 종단 그리는 방법에 따라서도 모두 구해 놓는다.
			ar >> m_bSettingVertSection;	// 종단제원이 셋팅되어 있는지
			ar >> m_bNeedSettingVertSection;	// 종단제원이 셋팅이 필요한지?
			for(long jong = 0; jong < 3; jong++)
			{
				m_tvArrVertSection_Vert_Left[jong].Serialize(ar, m_nFlag > 43);
				m_tvArrVertSection_Vert_Center[jong].Serialize(ar, m_nFlag > 43);
				m_tvArrVertSection_Vert_Right[jong].Serialize(ar, m_nFlag > 43);

				// 사방향일 경우 날개벽 각도 고려한 종단면도 고려해야 된다. 0 : 고려안함, 1 : 고려함
				for(long wing = 0; wing < 2; wing++)
				{
					m_tvArrVertSection_Ang_Left[jong][wing].Serialize(ar, m_nFlag > 43);
					m_tvArrVertSection_Ang_Center[jong][wing].Serialize(ar, m_nFlag > 43);
					m_tvArrVertSection_Ang_Right[jong][wing].Serialize(ar, m_nFlag > 43);
				}
				if(m_nFlag > 72)
					m_tvArrVertSection_NonModify_ForArch.Serialize(ar, m_nFlag > 43);
			}

			// 슬래브 평면 좌표(가각때문에 도저히 안 되겟으 ㅡㅡ)
			ar >> m_bSettingPlane;
			ar >> m_bNeedSettingPlane;
			for(stt = 0; stt < 2; stt++)
			{
				for(long gagak = 0; gagak < 2; gagak++)
				{
					m_tvArrSttEndPlane[stt][gagak].Serialize(ar, m_nFlag > 43);
					m_tvArrLeftRightPlane[stt][gagak].Serialize(ar, m_nFlag > 43);
				}
			}
		}
		if(m_nFlag>47)
		{
			ar >> m_bUseShellUserOutput;
			m_MatShellSlab.Serialize(ar);
			m_MatShellWallStt.Serialize(ar);
			m_MatShellWallEnd.Serialize(ar);
			for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
				m_MatShellWallMid[i].Serialize(ar);
			
			if(m_nFlag > 133)
				m_MatShellPRF.Serialize(ar);
		}

		if(m_nFlag < 116)
		{
			m_footingStt.m_dNValue_Eo_Hori = m_dNValue_Eo_Hori_XXX;
			m_footingStt.m_dNValue_Eo_Vert = m_dNValue_Eo_Vert_XXX;
			m_footingStt.m_dEo_Input_Hori	= m_dEo_Input_Pile_XXX;

			m_footingEnd.m_dNValue_Eo_Hori = m_dNValue_Eo_Hori_XXX;
			m_footingEnd.m_dNValue_Eo_Vert = m_dNValue_Eo_Vert_XXX;
			m_footingEnd.m_dEo_Input_Hori	= m_dEo_Input_Pile_XXX;

			m_footingBox.m_dNValue_Eo_Hori = m_dNValue_Eo_Hori_XXX;
			m_footingBox.m_dNValue_Eo_Vert = m_dNValue_Eo_Vert_XXX;
			m_footingBox.m_dEo_Input_Hori	= m_dEo_Input_Pile_XXX;
		}


		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			if(m_nFlag > 116)
				ar >> m_dCoverSide_Footing[i];
			else
				m_dCoverSide_Footing[i] = m_dCoverUpper_Footing[i];

		}

	}

	if(ar.IsLoading() && m_nFlag < 62)
	{
		long nQty	= GetQtyGuardWall();
		long i = 0; for(i = 0; i < nQty; i++)
		{
			CGuardWallRC *pGW	= GetGuardWallByHDan(i);
			if(!pGW) continue;
			if(pGW->m_nTypeGuardWall == 8)
			{
				double dH	= pGW->GetHeight() - pGW->m_ddc1;
				double dTot	= 0;
				long s = 0; for(s = 0; s < pGW->m_nSGugan; s++)
					dTot += (pGW->m_nS[s] * pGW->m_dS[s]);

				pGW->m_nS[pGW->m_nSGugan] = 1;
				pGW->m_dS[pGW->m_nSGugan] = dH - dTot + m_dCoverUpper_UpperSlab[0];;
				pGW->m_nSGugan += 1;
			}
		}
	}

	if(ar.IsLoading() && m_nFlag < 78)
	{
		long nQty	= GetQtyGuardWall();
		long i = 0; for(i = 0; i < nQty; i++)
		{
			CGuardWallRC *pGW	= GetGuardWallByHDan(i);
			if(!pGW) continue;
			pGW->m_dAggregate	= m_dAggregate_Etc;
		}
	}

	BOOL bExistExpJoint = (IsRcSlab() || IsPiRahmen()) ? TRUE : FALSE;
	if(ar.IsLoading() && m_nFlag > 102 && bExistExpJoint)
	{
		for(long nExp=0; nExp<m_pDesignExpanJoint->GetSize(); ++nExp)
		{
			CExpansionJointData	*pExpData = m_pDesignExpanJoint->GetExpansionJointData(nExp);
			CJijum *pJijum = GetJijum(nExp == 0 ? 0 : m_nQtyJigan);
			if(pJijum == NULL) continue;

			pExpData->m_pBlockW[PRE_SECTION]	= &pJijum->m_dWidBlockOut[PRE_SECTION];
			pExpData->m_pBlockH[PRE_SECTION]	= &pJijum->m_dHeightBlockOut[PRE_SECTION];
			pExpData->m_pBlockW[POST_SECTION]	= &pJijum->m_dWidBlockOut[POST_SECTION];
			pExpData->m_pBlockH[POST_SECTION]	= &pJijum->m_dHeightBlockOut[POST_SECTION];
		}
	}

	if (ar.IsLoading())
	{
		if(m_nFlag < 113)
		{
			m_dAggregate_Hyunta = m_dAggregate_Main;
			m_dSlump_Hyunta = m_dSlump_Main;
		}
		if(m_nFlag < 136)
		{
			if(m_pARcBridgeDataStd->IsRailDesign2017())
				m_nUnderGroundDistribute = 0;
		}

		// 기초부설정
		if(m_nFlag < 146)
		{
			for (HGINT32 ix = 0; ix < m_nQtyJigan+1; ++ix )
			{
				CFootingApp *pFooting = GetFootingByJijumNumber(ix);
				if(pFooting == NULL) continue;

				m_nSelect_Eo_HoriType[ix] = pFooting->m_dEo_Input_Hori==0 ? 0 : 1;

				if(IsBoxType())
				{
					m_nSelect_Eo_VertType[ix] = pFooting->m_dEo_Input_Vert==0 ? 0 : 1;
				}
				else
				{
					m_nSelect_Eo_VertType[ix] = 1;
					pFooting->m_dEo_Input_Vert = pFooting->m_dBoringEo;
				}

				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
				{
					pFooting->m_dFactorAlpha[0]	= (m_nSelectAlphaKhKv_XXX==1 || m_nSelectAlphaKhKv_XXX==2) ? 4 : 1;
					pFooting->m_dFactorAlpha[1]	= (m_nSelectAlphaKhKv_XXX==1 || m_nSelectAlphaKhKv_XXX==2) ? 8 : 2;
				}
			}
		}
	}

	if(ar.IsStoring() || m_nFlag > 107)
		SerializeStm(ar);

	if(ar.IsStoring() || m_nFlag > 108)
	{
		SerializeLsd(ar);
	}
}

CRcBridgeData& CRcBridgeData::operator=(const CRcBridgeData& Obj)
{
	long i(0), left(0), cns(0), stt(0);

	m_dFckMassConc			= Obj.m_dFckMassConc;
	m_dFckLeanConc			= Obj.m_dFckLeanConc;

	// 교량 형식 및 제원
//	m_nIdxBridge			= Obj.m_nIdxBridge; // SET x
	m_strBridgeName			= Obj.m_strBridgeName;
	m_nTypeBridge			= Obj.m_nTypeBridge;		// 문형, ㅠ형, 슬래브, V형, 박스형, 경사 등..
	m_dStationBridgeStt		= Obj.m_dStationBridgeStt;	// 시작 스테이션
	m_dLengthBridge			= Obj.m_dLengthBridge;		// 총길이
	m_nQtyJigan				= Obj.m_nQtyJigan;			// 지간수
	m_nSeparBri				= Obj.m_nSeparBri;
	
	// 횡단 위치 입력
	m_dWidthSlabLeft		= Obj.m_dWidthSlabLeft;		// 슬래브 좌측단 거리
	m_dWidthSlabRight		= Obj.m_dWidthSlabRight;	// 슬래브 우측단 거리
	m_nLineLeft				= Obj.m_nLineLeft;			// 좌측 선형
	m_nLineBase				= Obj.m_nLineBase;			// 기준 선형
	m_nLineRight			= Obj.m_nLineRight;			// 우측 선형
	m_dDisExtLineSttLeft	= Obj.m_dDisExtLineSttLeft;
	m_dDisExtLineSttRight	= Obj.m_dDisExtLineSttRight;
	m_dDisExtLineEndLeft	= Obj.m_dDisExtLineEndLeft;
	m_dDisExtLineEndRight	= Obj.m_dDisExtLineEndRight;
	m_dDistRoadCenterLevelLeft	= Obj.m_dDistRoadCenterLevelLeft;		// 도로중심 레벨거리 : 좌측
	m_dDistRoadCenterLevelRight	= Obj.m_dDistRoadCenterLevelRight;		// 도로중심 레벨거리 : 우측

	m_bUsePadShoe			= Obj.m_bUsePadShoe;


	// 지간 구성 입력
	for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		m_bExpJoint[i] = Obj.m_bExpJoint[i];				// 신축 이음 설치 유무
	}
	for(i=0; i<MAX_JIJUM_COUNT_RC-1; i++)
	{
		m_dLengthJigan[i] = Obj.m_dLengthJigan[i];					// 지간 길이
	}


	// 교량 받침 위치 입력
	AhTPACopy(&m_pArrJijum, &Obj.m_pArrJijum, (CJijum*)0);		// 지점 데이터
	m_arrStrJijumLen.RemoveAll();
	for(i=0; i<Obj.m_arrStrJijumLen.GetSize(); i++)
	{
		m_arrStrJijumLen.Add(Obj.m_arrStrJijumLen.GetAt(i));
	}
	m_arrJijumLen = Obj.m_arrJijumLen;
	m_bEWDlgUsed = Obj.m_bEWDlgUsed;
	m_nTypeShoeAngle = Obj.m_nTypeShoeAngle;
	m_bIsExp		= Obj.m_bIsExp;			// 가각(학폭) 지원할지?
	// 확폭 제원
	for(i=0; i<2; i++)
	{
		m_dExpSttW[i] = Obj.m_dExpSttW[i];	// 시점부
		m_dExpSttH[i] = Obj.m_dExpSttH[i];
		m_dExpSttR[i] = Obj.m_dExpSttR[i];
		m_dExpEndW[i] = Obj.m_dExpEndW[i];	// 종점부
		m_dExpEndH[i] = Obj.m_dExpEndH[i];
		m_dExpEndR[i] = Obj.m_dExpEndR[i];
		m_bExpCutStt[i]	= Obj.m_bExpCutStt[i];	// [0] : 좌
		m_bExpCutEnd[i]	= Obj.m_bExpCutEnd[i];	// [1] : 우
		m_bExpSttRoundIsTan[i]	= Obj.m_bExpSttRoundIsTan[i];	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
		m_bExpEndRoundIsTan[i]	= Obj.m_bExpEndRoundIsTan[i];
	}

	// CJijum 클래스와 CShoePos 클래스에서 받음.

	// 횡단 구성 입력
	m_nQtyHDan = Obj.m_nQtyHDan;		// 횡단 구성 개수
	for(i=0; i<MAX_QTY_HDAN; i++)
	{
		m_dLengthHDan[i] = Obj.m_dLengthHDan[i];	// 각 횡단 구간의 길이
		m_nTypeHDan[i] = Obj.m_nTypeHDan[i];		// 각 구간의 구성 형태

		m_dLengthRail[i] = Obj.m_dLengthRail[i];	// 차선 간격(좌측)
		m_nLineNumber[i] = Obj.m_nLineNumber[i];	// 철도교
		m_dSlopeHDan[i] = Obj.m_dSlopeHDan[i];		// 철도교 레일 기준 선형
	}  	
	if(Obj.m_pARcBridgeDataStd->IsRailDesign())
		AhTPACopy(&m_pArrGuardWall, &Obj.m_pArrGuardWallRoad, (CGuardWallRC*)0);	// 철도교
	else
		AhTPACopy(&m_pArrGuardWall, &Obj.m_pArrGuardWall, (CGuardWallRC*)0);
	
	m_bGuardWallDlgUsed = Obj.m_bGuardWallDlgUsed;
	m_bGuardWallUnderDlgUsed = Obj.m_bGuardWallUnderDlgUsed;
	m_bSSDlgUsed = Obj.m_bSSDlgUsed;

	m_nRoadMaterial = Obj.m_nRoadMaterial;
	m_dTP = Obj.m_dTP;		// 포장 두께
	m_dTS = Obj.m_dTS;		// 슬래브 두께
	m_dTSOld = Obj.m_dTSOld;
	for(i=0; i<2; i++)
	{
		m_dDH[i] = Obj.m_dDH[i];		// 슬래브 끝단에서 헌치까지의 거리
		m_bCantilSlab[i] = Obj.m_bCantilSlab[i];
		m_nSlabChemfer[i] = Obj.m_nSlabChemfer[i];
		m_dCantilH1[i] = Obj.m_dCantilH1[i];
		m_dCantilH2[i] = Obj.m_dCantilH2[i];
		m_dCantilB[i] = Obj.m_dCantilB[i];
		m_dRE1[i] = Obj.m_dRE1[i];		// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dRE2[i] = Obj.m_dRE2[i];		// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dTCW1[i] = Obj.m_dTCW1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 폭
		m_dTCH1[i] = Obj.m_dTCH1[i];	// 슬래브 하부 끝단 헌치 처리시 헌치 높이
		m_dTCW2[i] = Obj.m_dTCW2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 폭
		m_dTCH2[i] = Obj.m_dTCH2[i];	// 슬래브 상부 끝단 헌치 처리시 헌치 높이
	}

	// 설계조건 -------------------------------------------------------
	m_dUnitWeigthBackFill		= Obj.m_dUnitWeigthBackFill;		// 뒷채움의 단위중량
	m_dUnitWeigthBackFillWater = Obj.m_dUnitWeigthBackFillWater;
	m_dAngleFrictionBackFill	= Obj.m_dAngleFrictionBackFill;		// 뒷채움의 내부 마찰각
	m_dLiveLoadAlways			= Obj.m_dLiveLoadAlways;			// 상재하중
	m_dDeadLoadAlways			= Obj.m_dDeadLoadAlways;			// 상재하중
	m_nSelectLiveLoadHori		= Obj.m_nSelectLiveLoadHori;
	m_dLiveLoadFootway			= Obj.m_dLiveLoadFootway;
	m_dUnitWeightHeapSoil		= Obj.m_dUnitWeightHeapSoil;		// 성토재료의 단위중량
	m_dUnitWeightValidDepth		= Obj.m_dUnitWeightValidDepth;		// 근입지반의 단위중량
	m_dValidDepthBaseGround		= Obj.m_dValidDepthBaseGround;		// 기초의 유효근입 깊이
	m_dEo_CalcWallSpringEQ		= Obj.m_dEo_CalcWallSpringEQ;
	m_nTypeToapSafeCheck		= Obj.m_nTypeToapSafeCheck;			// 안정검토시의 토압검토
	m_SafeFactorOverturning[0]	= Obj.m_SafeFactorOverturning[0];	// 전도에 대한 안정률(평상시)
	m_SafeFactorOverturning[1]	= Obj.m_SafeFactorOverturning[1];	// 전도에 대한 안정률(지진시)
	m_SafeFactorSliding[0]		= Obj.m_SafeFactorSliding[0];		// 활동에 대한 안정률(평상시)
	m_SafeFactorSliding[0]		= Obj.m_SafeFactorSliding[0];		// 활동에 대한 안정률(지진시)
	m_nSelectModelingCountEQ	= Obj.m_nSelectModelingCountEQ;
	m_nSelectFootForce			= Obj.m_nSelectFootForce;
	m_nSelect2DFrameTool		= Obj.m_nSelect2DFrameTool;
	m_nSelect3DShellTool		= Obj.m_nSelect3DShellTool;
	m_nSelectMidasVer			= Obj.m_nSelectMidasVer;
	m_nSelectWingCalc			= Obj.m_nSelectWingCalc;

	for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
		m_bGoalEarthQuake[ix] = Obj.m_bGoalEarthQuake[ix];

	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
	{
		m_bAutoGroundInfo[ix] = Obj.m_bAutoGroundInfo[ix];
		m_bAutoCalcPileProperty[ix] = Obj.m_bAutoCalcPileProperty[ix];
		m_nSelect_Eo_VertType[ix] = Obj.m_nSelect_Eo_VertType[ix];
		m_nSelect_Eo_HoriType[ix] = Obj.m_nSelect_Eo_HoriType[ix];
	}
	// ------------------------------------------------------ 설계조건

	// 설계선택사항 -------------------------------------------------------
	m_bCheckMainSlabAndFootTemper	= Obj.m_bCheckMainSlabAndFootTemper;
	m_bSelectSupportBarWall		= Obj.m_bSelectSupportBarWall	;	
	m_bCheckSuppoerBarSlabMain	= Obj.m_bCheckSuppoerBarSlabMain;	
	m_bCheckSuppoerBarSlab300	= Obj.m_bCheckSuppoerBarSlab300	;
	m_bCheckSuppoerBarWall15hs	= Obj.m_bCheckSuppoerBarWall15hs;	
	m_bCheckSuppoerBarWall300	= Obj.m_bCheckSuppoerBarWall300	;
	m_bCheckSuppoerBarWallArea	= Obj.m_bCheckSuppoerBarWallArea;	
	m_bCheckSuppoerBarFoot15hs	= Obj.m_bCheckSuppoerBarFoot15hs;	
	m_bCheckSuppoerBarFoot300	= Obj.m_bCheckSuppoerBarFoot300	;
	m_bCheckSuppoerBarFootArea	= Obj.m_bCheckSuppoerBarFootArea;	
	m_bCheckSuppoerBarWing15hs	= Obj.m_bCheckSuppoerBarWing15hs;
	m_bCheckSuppoerBarWing300	= Obj.m_bCheckSuppoerBarWing300;
	m_bCheckSuppoerBarWingArea	= Obj.m_bCheckSuppoerBarWingArea;
	m_dCheckSuppoerBarWallArea	= Obj.m_dCheckSuppoerBarWallArea;	
	m_dCheckSuppoerBarWallAreaForOver400	= Obj.m_dCheckSuppoerBarWallAreaForOver400;
	m_dCheckSuppoerBarFootArea	= Obj.m_dCheckSuppoerBarFootArea;	
	m_dCheckSuppoerBarSlabArea	= Obj.m_dCheckSuppoerBarSlabArea;	
	m_dCheckSuppoerBarWingArea	= Obj.m_dCheckSuppoerBarWingArea;
	m_bCheckSupportWallDryingShrinkage = Obj.m_bCheckSupportWallDryingShrinkage;
	m_bCheckSupportWallMainRate = Obj.m_bCheckSupportWallMainRate;
	m_bCheckSupportFootDryingShrinkage = Obj.m_bCheckSupportFootDryingShrinkage;
	m_bCheckSupportSlabDryingShrinkage = Obj.m_bCheckSupportSlabDryingShrinkage;
	m_bCheckSupportSlabByReqRebar = Obj.m_bCheckSupportSlabByReqRebar;
	m_bCheckSupportSlabMainRate = Obj.m_bCheckSupportSlabMainRate;
	m_bCheckSupportSlabMainRateSpan = Obj.m_bCheckSupportSlabMainRateSpan;
	m_bCheckSuppoerBarSlabMainOver20 = Obj.m_bCheckSuppoerBarSlabMainOver20;
	m_bCheckSuppoerBarWallMainOver20 = Obj.m_bCheckSuppoerBarWallMainOver20;
	m_bCheckSuppoerBarFootMainOver20 = Obj.m_bCheckSuppoerBarFootMainOver20;
	m_bSkipAsMinCheckByHoriRebar = Obj.m_bSkipAsMinCheckByHoriRebar;
	m_nSelectPileCalcSpringEQ	= Obj.m_nSelectPileCalcSpringEQ	;	
	m_nSelectApsShearCheck		= Obj.m_nSelectApsShearCheck;
	m_bApplyMinAvSlab			= Obj.m_bApplyMinAvSlab			;
	m_bApplyMinAvWall			= Obj.m_bApplyMinAvWall			;
	m_bApplyMinAvFoot			= Obj.m_bApplyMinAvFoot			;
	m_bApplyMinAvWing			= Obj.m_bApplyMinAvWing			;
	m_bApplyMinAvAps			= Obj.m_bApplyMinAvAps;
	m_bApplyMinAvHyunTa			= Obj.m_bApplyMinAvHyunTa;
	m_bCheckMinCtcShearWing		= Obj.m_bCheckMinCtcShearWing;
	m_bCheckMinCtcShearAps		= Obj.m_bCheckMinCtcShearAps;
	m_bCheckCornerInside		= Obj.m_bCheckCornerInside;
	m_bApplyFootingGForce		= Obj.m_bApplyFootingGForce		;
	m_bApplyMassCalcSafe		= Obj.m_bApplyMassCalcSafe;
	m_bApplyFootingMinThick		= Obj.m_bApplyFootingMinThick;
	m_nApplyCalcShearDistFront	= Obj.m_nApplyCalcShearDistFront;	
	m_bCheckRebarRateFooting	= Obj.m_bCheckRebarRateFooting;
	m_nSelectApplyFs			= Obj.m_nSelectApplyFs;
	m_nApplyCalcShearDistBack	= Obj.m_nApplyCalcShearDistBack	;
	m_nTypeCrackDesignSlab		= Obj.m_nTypeCrackDesignSlab	;	
	m_nTypeCrackDesignWall		= Obj.m_nTypeCrackDesignWall	;	
	m_nTypeCrackDesignFoot		= Obj.m_nTypeCrackDesignFoot	;	
	m_nTypeCrackDesignWing		= Obj.m_nTypeCrackDesignWing	;
	m_nTypeCrackDesignAps		= Obj.m_nTypeCrackDesignAps;
	m_nSelectJointUpperRange	= Obj.m_nSelectJointUpperRange	;
	m_nIdxCountCycleMainRebar	= Obj.m_nIdxCountCycleMainRebar	;
	m_dFactorToap				= Obj.m_dFactorToap;
	m_dFactorLive				= Obj.m_dFactorLive;
	m_nSelFactorKhStt			= Obj.m_nSelFactorKhStt;
	m_nSelFactorKhEnd			= Obj.m_nSelFactorKhEnd;
	m_nSelCalcCornerFt			= Obj.m_nSelCalcCornerFt;
	m_nSelectForceCheckPos		= Obj.m_nSelectForceCheckPos;
	m_nSelectRigidModeling		= Obj.m_nSelectRigidModeling;
	m_bSelectRigidCalcHunch		= Obj.m_bSelectRigidCalcHunch;
	m_bSelectTempLoadAddWall	= Obj.m_bSelectTempLoadAddWall;
	m_bSelectShrinkLoadAddWall	= Obj.m_bSelectShrinkLoadAddWall;
	m_bSelectEnvToapOne = Obj.m_bSelectEnvToapOne;
	m_bSelectEnvSubsid = Obj.m_bSelectEnvSubsid;
	m_bSelectHunchModeling = Obj.m_bSelectHunchModeling;
	m_bSelectHunchDesignCheck = Obj.m_bSelectHunchDesignCheck;
	m_nSelectTempLoadSlab		= Obj.m_nSelectTempLoadSlab;
	m_nSelSubsideType			= Obj.m_nSelSubsideType;
	m_nSelectDesignRoadWidth	= Obj.m_nSelectDesignRoadWidth;
	m_nSelectLiveLoadSpanLength	= Obj.m_nSelectLiveLoadSpanLength;
	m_nSelectDesignSpanLength	= Obj.m_nSelectDesignSpanLength;
	m_nSelectDesignSpanApply	= Obj.m_nSelectDesignSpanApply;
	m_bSelectSpanLengthApplyShoe	 = Obj.m_bSelectSpanLengthApplyShoe;
	m_nSelectWallSectionDesign	= Obj.m_nSelectWallSectionDesign;
	m_nSelectInWallSectionDesign	= Obj.m_nSelectInWallSectionDesign;
	m_nSelectFootingReactionForce	= Obj.m_nSelectFootingReactionForce;
	m_nArchRibSectionMomentPos	= Obj.m_nArchRibSectionMomentPos;
	m_nSelectModelingHeight		= Obj.m_nSelectModelingHeight;

	m_bCheckSuppoerBarArchRibMinDia1	= Obj.m_bCheckSuppoerBarArchRibMinDia1;
	m_bCheckSuppoerBarArchRibMinDia2	= Obj.m_bCheckSuppoerBarArchRibMinDia2;
	m_bCheckFrontFootTrap2	= Obj.m_bCheckFrontFootTrap2;
	// ------------------------------------------------------ 설계선택사항

	m_MatComboEQ  = Obj.m_MatComboEQ;
	m_MatComboEcc = Obj.m_MatComboEcc;
	m_MatComboUlt = Obj.m_MatComboUlt;
	m_MatComboUse = Obj.m_MatComboUse;
	m_dWLoadNormal= Obj.m_dWLoadNormal;
	m_dWLoadEQ	  = Obj.m_dWLoadEQ;
	for(i=0; i<BRIDGE_LOAD_BASIC; i++)
	{
		m_bApplyLoad[i] = Obj.m_bApplyLoad[i];
	}
	m_bApplyLiveLoadEQ = Obj.m_bApplyLiveLoadEQ;	// 지진시 활하중재하
	m_bApplyAnchorForce = Obj.m_bApplyAnchorForce;
	for(i=0; i<RCBRIDGE_COMBO_COUNT; i++)
	{
		for(long j=0; j<MAX_RCBRIDGE_COMBO; j++)
		{
			m_bApplyCombo3D[i][j] = Obj.m_bApplyCombo3D[i][j];
			m_bApplyGroundWater[i][j] = Obj.m_bApplyGroundWater[i][j];
			m_dFactorAnchor[i][j] = Obj.m_dFactorAnchor[i][j];
		}
	}

	m_dFactorComboVLoad	= Obj.m_dFactorComboVLoad;

	// 수량선택사항 -------------------------------------------------------
	m_nFormType_ConcNormal		= Obj.m_nFormType_ConcNormal;		// 거푸집 : 일반콘크리트
	m_nFormType_Footing		= Obj.m_nFormType_Footing;			// 거푸집 : 기초
	m_nFormType_WallFront		= Obj.m_nFormType_WallFront;		// 거푸집 : 벽체 전면
	m_nFormType_WallBack		= Obj.m_nFormType_WallBack;		// 거푸집 : 벽체 후면
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			m_nFormType_WingFront[stt][left]	= Obj.m_nFormType_WingFront[stt][left];			// 거푸집 : 날개벽 전면
			m_nFormType_WingBack[stt][left]		= Obj.m_nFormType_WingBack[stt][left];			// 거푸집 : 날개벽 배면
		}
	}
	m_nFormType_Aps						= Obj.m_nFormType_Aps;					// 거푸집 : 접속슬래브
	m_nFormType_MassConc				= Obj.m_nFormType_MassConc;				// 거푸집 : mass 콘크리트
	m_nFormType_LeanConc				= Obj.m_nFormType_LeanConc;				// 거푸집 : 버림 콘크리트
	m_bSelectBM_ConcCare_Main			= Obj.m_bSelectBM_ConcCare_Main;		// 콘크리트양생 : 교대본체, 날개벽
	m_bSelectBM_ConcCare_Aps			= Obj.m_bSelectBM_ConcCare_Aps;			// 콘크리트양생 : 접속슬래브, 완충슬래브
	m_nSelectBM_ConcCare_Cal			= Obj.m_nSelectBM_ConcCare_Cal;			// 콘크리트양생 산출방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
	m_nTypeRcBridgeBlock				= Obj.m_nTypeRcBridgeBlock;				// 교대보호블럭 설치
	m_dAngleRcBridgeBlock				= Obj.m_dAngleRcBridgeBlock;			// 교대보호블럭 외측설치각도(도)
	m_bSelectBM_ApsCnsInclude			= Obj.m_bSelectBM_ApsCnsInclude;		// 수량집계에 접속슬래브, 완충슬래브 포함
	m_dPavementThick					= Obj.m_dPavementThick;					// 뒷채움시 콘크리트 포장두께
	m_dNosangThick						= Obj.m_dNosangThick;;					// 뒷채움시 노상두께
	m_nTypeHeightWall					= Obj.m_nTypeHeightWall;				// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이
	m_nTypeSlabLength					= Obj.m_nTypeSlabLength;				// 수량 산출시 슬래브 길이 기준(0 : 직거리, 1 : 선형거리)
	m_nTypeSurfaceProof					= Obj.m_nTypeSurfaceProof;				// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
	m_nTypePost							= Obj.m_nTypePost;						// 본체 동바리 타입(육교용, 일반용)
	m_nTypePostBaseLine					= Obj.m_nTypePostBaseLine;				// 동바리 기준선(0 : 지반선, 1 : 기초, 2 : 자동) 
	m_dPostLineEL						= Obj.m_dPostLineEL;										// 사용자 입력 동바리 기준선 EL
	m_nTypeScaffoldBaseLine				= Obj.m_nTypeScaffoldBaseLine;					// 비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	m_nTypeScaffoldSideLocation				= Obj.m_nTypeScaffoldSideLocation;		// 날개벽비계 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
	m_dScaffoldLineEL					= Obj.m_dScaffoldLineEL;				// 사용자 입력 비계 기준선 EL
	m_dThickPostUpper					= Obj.m_dThickPostUpper;				// 육교용 동바리 상부 두께
	m_dThickPostSide					= Obj.m_dThickPostSide;					// 육교용 동바리 좌우측 두께
	m_dThickScaffold					= Obj.m_dThickScaffold;					// 비계 두께

	m_nDeductNearFormBM					= Obj.m_nDeductNearFormBM;				// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
	m_nDeductNearTupagiBM				= Obj.m_nDeductNearTupagiBM;			// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)

	m_dAggregate_Footing				= Obj.m_dAggregate_Footing;				// 콘크리트 골재최대치수(기초)
	m_dAggregate_Main					= Obj.m_dAggregate_Main;				// 콘크리트 골재최대치수(본체)
	m_dAggregate_Aps					= Obj.m_dAggregate_Aps;					// 콘크리트 골재최대치수(접속슬래브)
	m_dAggregate_Lean					= Obj.m_dAggregate_Lean;				// 콘크리트 골재최대치수(버림)
	m_dAggregate_Mass					= Obj.m_dAggregate_Mass;				// 콘크리트 골재최대치수(MASS)
	m_dAggregate_Hyunta					= Obj.m_dAggregate_Hyunta;				// 콘크리트 골재최대치수(현장타설말뚝)
	m_dAggregate_Etc					= Obj.m_dAggregate_Etc;					// 콘크리트 골재최대치수(방호벽등)

	m_dSlump_Footing					= Obj.m_dSlump_Footing;					// 콘크리트 Slump(기초)
	m_dSlump_Main						= Obj.m_dSlump_Main;					// 콘크리트 Slump(본체)
	m_dSlump_Aps						= Obj.m_dSlump_Aps;						// 콘크리트 Slump(접속슬래브)
	m_dSlump_Lean						= Obj.m_dSlump_Lean;					// 콘크리트 Slump(버림)
	m_dSlump_Mass						= Obj.m_dSlump_Mass;					// 콘크리트 Slump(MASS)
	m_dSlump_Hyunta						= Obj.m_dSlump_Hyunta;					// 콘크리트 Slump(현장타설말뚝)
	m_dSlump_Etc						= Obj.m_dSlump_Etc;						// 콘크리트 Slump(방호벽등)

	for (HGINT32 ix = 0; ix < ePartCount; ++ix)
	{
		m_dAggregate[ix] = Obj.m_dAggregate[ix];								// 콘크리트골재최대치수
		m_dSlump[ix] = Obj.m_dSlump[ix];										// 콘크리트Slump
	}

	m_bAggregate_SlumpDetail			= Obj.m_bAggregate_SlumpDetail;			// 재료 물성치 상세 입력 - 수량산출 옵션
	m_dAggregate_LeanLSD				= Obj.m_dAggregate_LeanLSD;				// 콘크리트골재최대치수(버림)
	m_dAggregate_MassLSD				= Obj.m_dAggregate_MassLSD;				// 콘크리트골재최대치수(Mass)
	m_dAggregate_HyuntaLSD				= Obj.m_dAggregate_HyuntaLSD;			// 콘크리트골재최대치수(현장타설말뚝)
	m_dAggregate_EtcLSD					= Obj.m_dAggregate_EtcLSD;				// 콘크리트골재최대치수(방호벽등)

	m_dSlump_LeanLSD					= Obj.m_dSlump_LeanLSD;					// 콘크리트Slump(버림)
	m_dSlump_MassLSD					= Obj.m_dSlump_MassLSD;					// 콘크리트Slump(Mass)
	m_dSlump_HyuntaLSD					= Obj.m_dSlump_HyuntaLSD;				// 콘크리트Slump(현장타설말뚝)
	m_dSlump_EtcLSD						= Obj.m_dSlump_EtcLSD;					// 콘크리트Slump(방호벽등)

	m_nMonumenu							= Obj.m_nMonumenu;						// 교명주 받침 설치
	m_nDesPanel							= Obj.m_nDesPanel;				// 설명판 설치개수
	m_nNamePanel						= Obj.m_nNamePanel;				// 교명판 설치개수
	m_dAddLenghForGuardWallBM			= Obj.m_dAddLenghForGuardWallBM;		// 방호벽 수량 산출시 추가 길이(OLD)
	m_dAddLenghForGuardWall[0][0]		= Obj.m_dAddLenghForGuardWall[0][0];	// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[0][1]		= Obj.m_dAddLenghForGuardWall[0][1];	// 방호벽 수량 산출시 추가 길이 종점 우측
	m_dAddLenghForGuardWall[1][0]		= Obj.m_dAddLenghForGuardWall[1][0];	// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[1][1]		= Obj.m_dAddLenghForGuardWall[1][1];	// 방호벽 수량 산출시 추가 길이 종점 우측
	m_nHeightForSlabBM					= Obj.m_nHeightForSlabBM;				// 슬래브 거푸집 수량 산출시 높이 기준 
	m_dTackCotting							= Obj.m_dTackCotting;
	m_nTypePileMilkBM					= Obj.m_nTypePileMilkBM;
	m_bExcludeApsDowelRebar		= Obj.m_bExcludeApsDowelRebar;
	m_dTimeConcForm						= Obj.m_dTimeConcForm;
	m_bApplyYongSu							= Obj.m_bApplyYongSu;	
	m_dSurplusSoil_HyunTaPile			= Obj.m_dSurplusSoil_HyunTaPile;
	m_dConcreteSoil_HyunTaPile		= Obj.m_dConcreteSoil_HyunTaPile;
	m_dConcreteRock_HyunTaPile		= Obj.m_dConcreteRock_HyunTaPile;
	// ------------------------------------------------------ 수량선택사항

	// 피복설정 -------------------------------------------------------
	for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverUpper_UpperSlab[i]	= Obj.m_dCoverUpper_UpperSlab[i];	// 상부슬래브 상면
		m_dCoverLower_UpperSlab[i]	= Obj.m_dCoverLower_UpperSlab[i];	// 상부슬래브 하면
		m_dCoverSide_UpperSlab[i]	= Obj.m_dCoverSide_UpperSlab[i];	// 상부슬래브 측면
		m_dCoverWallOut[i]	= Obj.m_dCoverWallOut[i];	// 벽체외면
		m_dCoverWallIn[i]	= Obj.m_dCoverWallIn[i];	// 벽체 내면
		m_dCoverWallMid[i]	= Obj.m_dCoverWallMid[i];	// 중간벽체
		m_dCoverColumn[i]	= Obj.m_dCoverColumn[i];	// 기둥
		m_dCoverUpper_Footing[i]	= Obj.m_dCoverUpper_Footing[i];	// 기초 상면
		m_dCoverLower_Footing_Lean[i]	= Obj.m_dCoverLower_Footing_Lean[i];	// 기초 하면(직접기초시)
		m_dCoverLower_Footing_Pile[i]	= Obj.m_dCoverLower_Footing_Pile[i];	// 기초 하면(말뚝기초시)
		m_dCoverRib[iUPPER][i]	= Obj.m_dCoverRib[iUPPER][i];
		m_dCoverRib[iLOWER][i]	= Obj.m_dCoverRib[iLOWER][i];
		m_dCoverWingIn[i]	= Obj.m_dCoverWingIn[i];
		m_dCoverWingOut[i]	= Obj.m_dCoverWingOut[i];
		m_dCoverSide_Footing[i]	= Obj.m_dCoverSide_Footing[i]; // 기초 측면
	}
	m_dCoverWingEnd		= Obj.m_dCoverWingEnd;
	m_dCoverWingUpper	= Obj.m_dCoverWingUpper;
	// ------------------------------------------------------ 피복설정
	m_bPileNvalueAuto	= Obj.m_bPileNvalueAuto;

	// 종단면형상 -------------------------------------------------------
	m_nTypeJongdanForRebar		= Obj.m_nTypeJongdanForRebar;
	m_nTypeWallForRebarGen		= Obj.m_nTypeWallForRebarGen;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
	m_nTypeOutWall		= Obj.m_nTypeOutWall;	// 외측벽체 유무(좌우)(0 : 좌우, 1 : 좌, 2 : 우, -1 : 없음)
	m_fxJointStt		= Obj.m_fxJointStt;

		// 시점측 벽체
	m_dFHS		= Obj.m_dFHS;
	m_dFLS		= Obj.m_dFLS;
	m_dFRS		= Obj.m_dFRS;
	m_dWS		= Obj.m_dWS;
	m_dWSHW		= Obj.m_dWSHW;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dWSHH		= Obj.m_dWSHH;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWS		= Obj.m_dBHWS;
	m_dBHHS		= Obj.m_dBHHS;
	m_dBHS		= Obj.m_dBHS;
	m_dBTHS		= Obj.m_dBTHS;
	m_dBWS		= Obj.m_dBWS;
	m_dHWS		= Obj.m_dHWS;
	m_dHHS		= Obj.m_dHHS;
	m_dHRS		= Obj.m_dHRS;			// 시점헌치 반경
	m_nQtyRail	= Obj.m_nQtyRail;
	m_dUnitWeightRail			= Obj.m_dUnitWeightRail;
	m_dDisBaseElevation			= Obj.m_dDisBaseElevation;
	m_dSlopeDosang				= Obj.m_dSlopeDosang;
	m_dHeightDosangUnderTie		= Obj.m_dHeightDosangUnderTie;
	m_bLevelFrRoadCenterToEdge	= Obj.m_bLevelFrRoadCenterToEdge;
	m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	= m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
	m_dDisBaseElevationRL		= Obj.m_dDisBaseElevationRL;
	m_dHeightRail				= Obj.m_dHeightRail;
	m_bApplyLineSlope			= Obj.m_bApplyLineSlope;

	m_dELSB		= Obj.m_dELSB;
	m_footingStt	= Obj.m_footingStt;
	m_bDivisionApsStt	= Obj.m_bDivisionApsStt;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsStt[left]	= Obj.m_nCountCnsStt[left];	// 완충슬래브 수
		m_apsStt[left]			= Obj.m_apsStt[left];
		for(cns = 0; cns < MAX_CNS; cns++)
			m_cnsStt[left][cns]	= Obj.m_cnsStt[left][cns];
	}
	//m_ExFootingStt	= Obj.m_ExFootingStt;

		// 종점측 벽체
	m_fxJointEnd	= Obj.m_fxJointEnd;
	m_dFHE		= Obj.m_dFHE;
	m_dFLE		= Obj.m_dFLE;
	m_dFRE		= Obj.m_dFRE;
	m_dWE		= Obj.m_dWE;
	m_dWEHW		= Obj.m_dWEHW;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dWEHH		= Obj.m_dWEHH;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWE		= Obj.m_dBHHE;
	m_dBHHE		= Obj.m_dBHHE;
	m_dBHE		= Obj.m_dBHE;
	m_dBTHE		= Obj.m_dBTHE;
	m_dBWE		= Obj.m_dBWE;
	m_dHWE		= Obj.m_dHWE;
	m_dHHE		= Obj.m_dHHE;
	m_dHRE		= Obj.m_dHRE;			// 종점헌치 반경
	m_dELEB		= Obj.m_dELEB;
	m_footingEnd	= Obj.m_footingEnd;
	m_bDivisionApsEnd	= Obj.m_bDivisionApsEnd;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsEnd[left]	= Obj.m_nCountCnsEnd[left];	// 완충슬래브 수
		m_apsEnd[left]			= Obj.m_apsEnd[left];
		for(cns = 0; cns < MAX_CNS; cns++)
			m_cnsEnd[left][cns]	= Obj.m_cnsEnd[left][cns];
	}
	//m_ExFootingEnd	= Obj.m_ExFootingEnd;


		// 내측벽체
	m_nCountInWallHunch	= Obj.m_nCountInWallHunch;
	for(long upper=0; upper<2; upper++)
	{
		for(i = 0; i < m_nCountInWallHunch; i++)
		{
			m_dHHL[i][upper]	= Obj.m_dHHL[i][upper];
			m_dHHR[i][upper]	= Obj.m_dHHR[i][upper];
			m_dHBH[i][upper]	= Obj.m_dHBH[i][upper];
			m_dHLW[i][upper]	= Obj.m_dHLW[i][upper];
			m_dHRW[i][upper]	= Obj.m_dHRW[i][upper];
			m_dWL[i][upper]		= Obj.m_dWL[i][upper];
			m_dWR[i][upper]		= Obj.m_dWR[i][upper];
			m_dHLR[i][upper]	= Obj.m_dHLR[i][upper];	// 내측헌치 왼쪽 반경
			m_dHRR[i][upper]	= Obj.m_dHRR[i][upper];	// 내측헌치 오른쪽 반경
		}
	}

	m_nTypeRoundOfHunchStt	= Obj.m_nTypeRoundOfHunchStt;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
	m_nTypeRoundOfHunchEnd	= Obj.m_nTypeRoundOfHunchEnd;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
	for(i = 0; i < m_nCountInWallHunch; i++)
	{
		for(left = 0; left < 2; left++)
			m_nTypeRoundOfHunchInWall[i][left]	= Obj.m_nTypeRoundOfHunchInWall[i][left];	// 내측헌치 좌우측
	}

	// 포물선(리브정의) 2개 (상판 하판)
	m_bConnectRibAndUpperSlab	= Obj.m_bConnectRibAndUpperSlab;
	long upper = 0; for(upper = 0; upper < 2; upper++)
		m_pbRib[upper]	= Obj.m_pbRib[upper];
	m_pbRibCen	= Obj.m_pbRibCen;
	/*
	for(stt = 0; stt < 2; stt++)
	{
		m_xyRibCrown[stt]		= Obj.m_xyRibCrown[stt];
		m_dRibLenTotal[stt]		= Obj.m_dRibLenTotal[stt];
		m_dRibHeightTotal[stt]	= Obj.m_dRibHeightTotal[stt];
		m_dRibLenStt[stt]		= Obj.m_dRibLenStt[stt];
		m_dRibLenEnd[stt]		= Obj.m_dRibLenEnd[stt];
		m_dRibFactorM[stt]		= Obj.m_dRibFactorM[stt];
		m_dRibLenArchLine[stt]	= Obj.m_dRibLenArchLine[stt];
		m_dRibOffsetFromUpper[stt]	= Obj.m_dRibOffsetFromUpper[stt];
	}

	// 포물선(리브정의) 중심선
	m_xyRibCrownCen			= Obj.m_xyRibCrownCen;
	m_dRibLenTotalCen		= Obj.m_dRibLenTotalCen;
	m_dRibHeightTotalCen	= Obj.m_dRibHeightTotalCen;
	m_dRibLenSttCen			= Obj.m_dRibLenSttCen;
	m_dRibLenEndCen			= Obj.m_dRibLenEndCen;
	m_dRibFactorMCen		= Obj.m_dRibFactorMCen;
	m_dRibLenArchLineCen	= Obj.m_dRibLenArchLineCen;
	m_dRibOffsetFromUpperCen	= Obj.m_dRibOffsetFromUpperCen;	// 현재 포물선의 기준상단에서의 떨어진 거리
	*/

	for(i = 0; i < MAX_STRUP_REBAR; i++)
	{
		m_dRibBlockW[i]	= Obj.m_dRibBlockW[i];	// 리브위 사각 블럭 폭
		m_dRibBlockH[i]	= Obj.m_dRibBlockH[i];	// 리브위 사각 블럭 높이
	}
	// ------------------------------------------------------ 종단면형상

	AhTPACopy(&m_pArrInWall, &Obj.m_pArrInWall, (CWallApp*)0);

	// 날개벽 제원 입력
	*m_pWingWallStt[0] = *Obj.m_pWingWallStt[0];
	*m_pWingWallStt[1] = *Obj.m_pWingWallStt[1];
	*m_pWingWallEnd[0] = *Obj.m_pWingWallEnd[0];
	*m_pWingWallEnd[1] = *Obj.m_pWingWallEnd[1];
	m_bAutoWingEl = Obj.m_bAutoWingEl;
	m_bAutoWingHeightSoil = Obj.m_bAutoWingHeightSoil;

	// 브래킷 길이
	m_bBracketLengthStt = Obj.m_bBracketLengthStt;
	m_bBracketLengthEnd = Obj.m_bBracketLengthEnd;
	for(i=0; i<2; i++)
	{
		m_dBrkLenStt[i] = Obj.m_dBrkLenStt[i];
		m_dBrkLenEnd[i] = Obj.m_dBrkLenEnd[i];
	}

	// 교명주 받침 제원 입력
	m_dMonuH1 = Obj.m_dMonuH1;
	m_dMonuH2 = Obj.m_dMonuH2;
	m_dMonuH3 = Obj.m_dMonuH3;
	m_dMonuD = Obj.m_dMonuD;
	m_dMonuW = Obj.m_dMonuW;
	m_dMonuL = Obj.m_dMonuL;
	m_dMonuFck = Obj.m_dMonuFck;
	m_dMonuR_Fy = Obj.m_dMonuR_Fy;
	m_dMonuR_DiaHor = Obj.m_dMonuR_DiaHor;
	m_dMonuR_DiaVer = Obj.m_dMonuR_DiaVer;
	m_dMonuR_H = Obj.m_dMonuR_H;
	m_dMonuR_L = Obj.m_dMonuR_L;
	m_nMonuR_nA = Obj.m_nMonuR_nA;
	m_dMonuR_A = Obj.m_dMonuR_A;
	m_dMonuR_A1 = Obj.m_dMonuR_A1;
	m_nMonuR_nB = Obj.m_nMonuR_nB;
	m_dMonuR_B = Obj.m_dMonuR_B;
	m_nMonuR_nC = Obj.m_nMonuR_nC;
	m_dMonuR_C = Obj.m_dMonuR_C;
	m_dMonuR_C1 = Obj.m_dMonuR_C1;
	m_dMonuR_C2 = Obj.m_dMonuR_C2;
	m_dMonuR_C3 = Obj.m_dMonuR_C3;
	m_nMonuR_nD = Obj.m_nMonuR_nD;
	m_dMonuR_D = Obj.m_dMonuR_D;
	m_dMonuR_D1 = Obj.m_dMonuR_D1;
	m_dMonuR_D2 = Obj.m_dMonuR_D2;
	m_dMonuR_D3 = Obj.m_dMonuR_D3;
	m_dMonuAggregate = Obj.m_dMonuAggregate;

	// Box타입 추가
	m_footingBox				= Obj.m_footingBox;
	m_bApplyGroundWaterLevel	= Obj.m_bApplyGroundWaterLevel;
	m_dGroundWaterLevel			= Obj.m_dGroundWaterLevel;
	m_dCoffKh_Sei				= Obj.m_dCoffKh_Sei;
	m_bExistProtectionRiseFooting	=Obj.m_bExistProtectionRiseFooting;	
	m_dPRF_W						=Obj.m_dPRF_W;						
	m_dPRF_H1						=Obj.m_dPRF_H1;									
	m_dPRF_H2						=Obj.m_dPRF_H2;									
	m_bPRF_SameCTC_Main				=Obj.m_bPRF_SameCTC_Main;			
	m_dPRF_CTC_Main					=Obj.m_dPRF_CTC_Main;					
	m_dPRF_CTC_Support				=Obj.m_dPRF_CTC_Support;				
	m_dPRF_CTC_SupportVert			=Obj.m_dPRF_CTC_SupportVert;		
	m_dPRF_CTC_Shear				=Obj.m_dPRF_CTC_Shear;					
	m_dPRF_Dia_Main					=Obj.m_dPRF_Dia_Main;					
	m_dPRF_Dia_Support				=Obj.m_dPRF_Dia_Support;				
	m_dPRF_Dia_Shear				=Obj.m_dPRF_Dia_Shear;					
	m_nPRF_Type_Shear				=Obj.m_nPRF_Type_Shear;					
	m_nPRF_Count_Shear				=Obj.m_nPRF_Count_Shear;				

	m_dTS_Lower			= Obj.m_dTS_Lower;
	m_nTypeLower		= Obj.m_nTypeLower;
	m_dThickCobbStone	= Obj.m_dThickCobbStone;
	m_bSlopeLowerSlab	= Obj.m_bSlopeLowerSlab;

	m_nCountAnchor		= Obj.m_nCountAnchor;
	m_bAnchorSameValue	= Obj.m_bAnchorSameValue;
	m_dArrDistJongAnchor	= Obj.m_dArrDistJongAnchor;CDoubleArray
	m_dArrLoadAnchor		= Obj.m_dArrLoadAnchor;
	m_dArrLengthAnchor		= Obj.m_dArrLengthAnchor;
	m_dArrDiaAnchor			= Obj.m_dArrDiaAnchor;
	m_dArrEaAnchor = Obj.m_dArrEaAnchor;
	for(i=0; i<m_nCountAnchor; i++)
		m_dArrDistHDanAnchor[i] = Obj.m_dArrDistHDanAnchor[i];

	m_nGroundFactorKV = Obj.m_nGroundFactorKV;
	m_dGroundFactorKV = Obj.m_dGroundFactorKV;
	m_nGroundFactorBv = Obj.m_nGroundFactorBv;
	m_nEquivalentApplyKsb = Obj.m_nEquivalentApplyKsb;
	m_dEquivalentApplyKsb = Obj.m_dEquivalentApplyKsb;
	m_n2DGeneralSpringType = Obj.m_n2DGeneralSpringType;
	m_n2DEQSpringType = Obj.m_n2DEQSpringType;
	m_nVerticalFactor = Obj.m_nVerticalFactor;
	m_dSpringSpaceBottom = Obj.m_dSpringSpaceBottom;
	m_dSpringSpaceWall = Obj.m_dSpringSpaceWall;
	m_bSpringSpaceIncludeWallTick = Obj.m_bSpringSpaceIncludeWallTick;

	m_dThetaShearDesign = Obj.m_dThetaShearDesign;
	m_bCheckDeltaTR = Obj.m_bCheckDeltaTR;

	m_dElasticModularRatioUseCheck = Obj.m_dElasticModularRatioUseCheck;
	m_nSelectUsebilityCheck = Obj.m_nSelectUsebilityCheck;
	m_nTypeCrackWidthCondition = Obj.m_nTypeCrackWidthCondition;
	m_nSelectExposureCoverCheck = Obj.m_nSelectExposureCoverCheck;
	m_nSelectPminColumn = Obj.m_nSelectPminColumn;
	
	m_nApplyCalcShearDistSlab = Obj.m_nApplyCalcShearDistSlab;
	m_nApplyCalcShearDistWall = Obj.m_nApplyCalcShearDistWall;
	m_nApplyCalcShearDistFrontPile = Obj.m_nApplyCalcShearDistFrontPile;
	m_nApplyCalcShearDistBackPile = Obj.m_nApplyCalcShearDistBackPile;
	m_bApplyT1CalcWingWall = Obj.m_bApplyT1CalcWingWall;

	AhTPACopy(&m_pArrTypeDefault, &Obj.m_pArrTypeDefault, (CTypeDefault*)0);
	
	*m_pLineApp = *Obj.m_pLineApp;
	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
	m_pLineApp->SetDataIp(strErrorMsg);
	return *this;

//	*m_plsdManager	= *Obj.m_plsdManager;
//	*m_plsdRebar	= *Obj.m_plsdRebar;
//	*m_plsdEnvRating	= Obj.m_plsdEnvRating;
}


// 내부벽체 만들기
BOOL CRcBridgeData::MakeInWall()
{
	m_pArrInWall.Add(new CWallApp);
	m_pArrInWall.GetAt(m_pArrInWall.GetSize()-1)->m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;

	return TRUE;
}

// 내부 벽체 제거
BOOL CRcBridgeData::RemoveInWall(long nIdx)
{
	long nSize = m_pArrInWall.GetSize();
	if(nSize <= nIdx) return FALSE;

	delete m_pArrInWall.GetAt(nIdx);
	m_pArrInWall.RemoveAt(nIdx);

	return TRUE;
}
// 내부벽체 개수 리턴
long CRcBridgeData::GetCountInWall(BOOL bNotColumn)
{
	long nSize = m_pArrInWall.GetSize();
	if(bNotColumn)
	{
		for(long ix=0; ix<m_pArrInWall.GetSize(); ++ix)
		{
			CWallApp *pWall = m_pArrInWall.GetAt(ix);
			if(pWall->m_bTopIsColumn)
				--nSize;
		}
	}
	
	return nSize;
}

// 실제 내부벽체 개수를 리턴.
long CRcBridgeData::GetCountInWallReal(BOOL bCheckTypeWall)
{
	long nSize = m_pArrInWall.GetSize();
	long nCount = 0;
	long i=0; for(i=0; i<nSize; i++)
	{
		if(m_pArrInWall.GetAt(i)->m_bIs)
		{
			if(bCheckTypeWall && m_nTypeBridge < BRIDGE_TYPE_TRAP)
			{
				if(m_pArrInWall.GetAt(i)->m_bTopIsColumn)	// 벽체식, 기둥식
					nCount++;
			}
			else
				nCount++;
		}
	}

	return nCount;
}

// long CRcBridgeData::GetCountInWallEach(BOOL bColumn)
// {
// 	long nRet = 0;
// 	long nTot = GetCountInWall();
// 	long i=0; for(i=0; i<nTot; i++)
// 	{
// 		CWallApp *pWall = GetInWall(i);
// 		if(!pWall->m_bIs) continue;
// 
// 		if(bColumn == pWall->m_bTopIsColumn) nRet++;  
// 	}
// 
// 	return nRet;
// }

// 내부벽체 가져오기
CWallApp* CRcBridgeData::GetInWall(long nIdx)
{
	long nSize = m_pArrInWall.GetSize();
	if(nSize <= nIdx || nIdx < 0) return NULL;

	return m_pArrInWall.GetAt(nIdx);
}

void CRcBridgeData::InitHunch(long nIdx)
{
	if(nIdx >= MAX_INWALL_HUNCH) return;

	for(long upper=0; upper<2; upper++)
	{
		m_dHHL[nIdx][upper]	= 0;		// 각 벽체 헌치 높이(좌)
		m_dHHR[nIdx][upper]	= 0;		// 각 벽체 헌치 높이(우)
		m_dHBH[nIdx][upper]	= 0;		// 각 벽체 헌치아래 돌출 높이
		m_dHLW[nIdx][upper]	= 0;		// 각 벽체 헌치 너비(왼쪽)
		m_dHRW[nIdx][upper]	= 0;		// 각 벽체 헌치 너비(오른쪽)
		m_dWL[nIdx][upper]	= 0;		// 각 벽체 헌치 시작위치(왼쪽)
		m_dWR[nIdx][upper]	= 0;		// 각 벽체 헌치 시작위치(오른쪽)
		m_dHLR[nIdx][upper]	= 0;		// 내측헌치 왼쪽 반경
		m_dHRR[nIdx][upper]	= 0;		// 내측헌치 오른쪽 반경
	}

	m_nTypeRoundOfHunchInWall[nIdx][iLEFT]	= 0;	// 내측헌치 좌우측
	m_nTypeRoundOfHunchInWall[nIdx][iRIGHT]	= 0;
}

// 벽체를 없앤다.
void CRcBridgeData::InitWall(BOOL bStt)
{
	if(bStt)
	{
		m_dBHS	= 0;
		m_dBTHS	= 0;
		m_dBWS	= 0;
		m_dBHHS	= 0;
		m_dBHWS	= 0;
		m_dHHS	= 0;
		m_dHWS	= 0;
		m_dWS	= 0;
		m_dWSHW	= 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
		m_dWSHH = 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
		m_dFRS	= 0;
		m_dFLS	= 0;
		m_dFHS	= 0;
	}
	else
	{
		m_dBHE	= 0;
		m_dBTHE	= 0;
		m_dBWE	= 0;
		m_dBHHE	= 0;
		m_dBHWE	= 0;
		m_dHHE	= 0;
		m_dHWE	= 0;
		m_dWE	= 0;
		m_dWEHW	= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
		m_dWEHH	= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
		m_dFLE	= 0;
		m_dFRE	= 0;
		m_dFHE	= 0;
	}
}

BOOL CRcBridgeData::IsWall(long nJijum)
{
	if(nJijum==0) 
		return IsOutWall(TRUE);
	else if(nJijum==m_nQtyJigan)
		return IsOutWall(FALSE);
	else
	{
		CWallApp *pWall = GetInWall(nJijum-1);
		if(pWall)
			return pWall->m_bIs;
		else
			return FALSE;
	}
}

// 벽체가 있는지...
// 0 : 없음, -1 : 시점측만, 1 : 종점측만, 2 : 둘다.
BOOL CRcBridgeData::IsOutWall(BOOL bStt)
{
	// 무조건 옵션으로 체크함.
	if(m_nTypeOutWall == 2) return TRUE;
	else if(m_nTypeOutWall == 0) return FALSE;
	else if(m_nTypeOutWall == -1 && bStt) return TRUE;
	else if(m_nTypeOutWall == 1 && !bStt) return TRUE;

	return FALSE;
}

void CRcBridgeData::MakeJijum(long nQtyMake)
{
	AhTPAMake(nQtyMake, &m_pArrJijum, (CJijum*)0);
}

CJijum* CRcBridgeData::GetJijum(int nJijum)
{
	long nSize = m_pArrJijum.GetSize();
	if(nSize <= nJijum || nJijum < 0) 
	{
		if(nSize > 0 && nSize <= nJijum)
			return m_pArrJijum.GetAt(nSize-1);
		else
			return NULL;
	}

	// 지점 값을 깔끔하게 정리한다.
	CJijum *pJ	= m_pArrJijum.GetAt(nJijum);
	CWallApp *pWall	= nJijum > 0 && nJijum < m_nQtyJigan ? GetInWall(nJijum-1) : NULL;
	if(pWall)
	{
		if(!pWall->m_bIs) pWall	= NULL;
	}

	if((nJijum == 0 && IsOutWall(TRUE)) || (nJijum == m_nQtyJigan && IsOutWall(FALSE)) || pWall)
	{
		pJ->m_dLengthSlabToShoe[0]	= 0;
		pJ->m_dLengthSlabToShoe[1]	= 0;
		pJ->m_dLengthUgan[0]	= 0;
		pJ->m_dLengthUgan[1]	= 0;
	}

	return m_pArrJijum.GetAt(nJijum);
}

int CRcBridgeData::GetCountJijum()
{
	return m_pArrJijum.GetSize();
}

//<summary>
//	도로교인 경우
//		<보도 번호> 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">보도 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadFootway(long nGW, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	long nGWAct = -1;
	long n = 0; for(n = 0; n < nQtyGW; n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadFootway())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}

//<summary>
//	도로교인 경우
//		<방호벽 및 중분대 번호>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">방호벽 및 중분대 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadGuardFenceAndCentralWall(long nGW, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	long nGWAct = -1;
	long n = 0; for(n = 0; n < nQtyGW; n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	도로교인 경우
//		<보차도 경계석 번호> 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">보차도 경계석 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadFootwayBlock(long nGW, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	long nGWAct = -1;
	long n = 0; for(n = 0; n < nQtyGW; n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadFootwayBlock())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	도로교인 경우
//		<방음벽 기초 번호>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">방음벽 기초 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadSoundProof(long nGW, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	long nGWAct = -1;
	long n = 0; for(n = 0; n < nQtyGW; n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadSoundProof())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	도로교인 경우
//		<흙막이벽 번호>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">흙막이벽 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadRetainingWall(long nGW, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	long nGWAct = -1;
	long n = 0; for(n = 0; n < nQtyGW; n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRetainingWall())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	도로교	<난간기초 번호>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">난간기초 번호, 횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadHandRail(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadHandRail())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}

//<summary>
//	철도교	<연석>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailGuardFence(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailGuardFence())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<자갈막이(철근)>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailDetectGravel(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailDetectGravel())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<자갈막이(무근)>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailDetectGravel2(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailDetectGravel2())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<보도>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailFootway(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootway())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<연석+보도>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailFootwayBlock(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootwayBlock())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<통신구>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailHole(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHole())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<통신구+보도>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRailHoleFootway(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHoleFootway())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}
//<summary>
//	철도교	<통신구+보도>로 방호벽 객체를 가져옴.
//</summary>
//<param name="nGW">횡단 번호가 아님</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallRoadRailDike(long nGW, BOOL bRoadNotSlab)
{
	long nGWAct = -1;
	long n = 0; for(n = 0; n < GetQtyGuardWall(bRoadNotSlab); n++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRailDike())
		{
			nGWAct++;
			if(nGWAct == nGW)
				return GetGuardWallByHDan(n,bRoadNotSlab);
		}
	}
	return NULL;
}



//<summary>
// 방호벽 타입의 개소수를 리턴
// 방호벽 타입 - 사용자 입출력이외에 도면생성, 계산 등 프로그램 내부에서 사용 
//	도로교인 경우 : 프로그램 내부적으로는 <방호벽 타입, 보도>로만 구분하여 사용
//		방호벽 및 중분대
//		보차도 경계석
//		방음벽 기초
//		흙막이벽
//		난간기초
//
//	철도교인 경우 : 프로그램 내부적으로는 <방호벽 타입(연석), 도상>로만 구분하여 사용
//		연석
//		자갈막이
//		연석+보도
//		통신구
//		통신구+보도
//  
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanTotalGuardFence(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nHDan = 0; nHDan < GetQtyHDan(bRoadNotSlab); nHDan++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardFence())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교		방호벽의 개수
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadGuardFence(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadGuardFence())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		중분대의 개수, (중분대 1/2 는 아님)
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadCentralWallFull(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadCentralWallFull())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교		중분대1/2의 개수
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadCentralWallHalf(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadCentralWallHalf())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		보도
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadFootway(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadFootway())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		방음벽 기초
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadSoundProof(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadSoundProof())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		흙막이벽
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadRetainingWall(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRetainingWall())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		난간기초
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadHandRail(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadHandRail())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		난간기초
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadRailDike(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRailDike())
			nQty++;
	}
	return nQty;
}
//<summary>
//	도로교인 경우
//		보차도 경계석
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRoadFootwayBlock(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadFootwayBlock())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		연석
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailGuardFence(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailGuardFence())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		자갈막이(철근)
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailDetectGravel(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailDetectGravel())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		자갈막이(무근)
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailDetectGravel2(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailDetectGravel2())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		보도
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailFootway(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootway())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		직결식도상
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// long CRcBridgeData::GetQtyHDanRailDosangVer(BOOL bRoadNotSlab)
// {
// 	long nQty = 0;
// 	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
// 	{
// 		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
// 		if(!pGW) continue;
// 		if(pGW->IsTypeHDanRailDosangVer())
// 			nQty++;
// 	}
// 	return nQty;
// }
//<summary>
//	철도교		자갈도상
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// long CRcBridgeData::GetQtyHDanRailDosangGravel(BOOL bRoadNotSlab)
// {
// 	long nQty = 0;
// 	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
// 	{
// 		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
// 		if(!pGW) continue;
// 		if(pGW->IsTypeHDanRailDosangGravel())
// 			nQty++;
// 	}
// 	return nQty;
// }
//<summary>
//	철도교		연석+보도
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailFootwayBlock(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootwayBlock())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		통신구
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailHole(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHole())
			nQty++;
	}
	return nQty;
}
//<summary>
//	철도교		통신구+보도
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDanRailHoleFootway(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nGW = 0; nGW < GetQtyGuardWall(bRoadNotSlab); nGW++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nGW,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHoleFootway())
			nQty++;
	}
	return nQty;
}


CWingWall* CRcBridgeData::GetWingWall(BOOL bStt, BOOL bLeft)
{
	if(bStt)
		return bLeft ? m_pWingWallStt[0] : m_pWingWallStt[1];
	else
		return bLeft ? m_pWingWallEnd[0] : m_pWingWallEnd[1];
}

CFootingApp* CRcBridgeData::GetFootingByJijumNumber(long nJijum)
{
	// BOX형에서는 시점 기초 하나만 쓴다.
	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		return &m_footingBox;
	}

	long nQtyJijum = m_nQtyJigan+1;
	if(nJijum==0)
	{
		return m_footingStt.m_bIs ? &m_footingStt : NULL;
	}
	else if(nJijum==nQtyJijum-1)
	{
		return m_footingEnd.m_bIs ? &m_footingEnd : NULL;
	}
	else
	{
		long nIdxWall	= nJijum-1;
		CWallApp *pWall = GetInWall(nIdxWall);
		if(pWall)
			return pWall->m_footing.m_bIs ? &pWall->m_footing : NULL;
		else 
			return NULL;
	}
	
}

// 실제로 존재 하는 것 중의 인덱스를 사용.
CFootingApp* CRcBridgeData::GetFooting(int nFooting)
{
	int nCountInWall = GetCountInWall();
	int nCountFooting = GetCountFooting();

	if(IsBoxType())
		return &m_footingBox;

	if(nFooting>nCountFooting-1)
		return NULL;

	long nIdxSttInWallFooting	= IsOutWall(TRUE) ? 1 : 0;
	
	if(nFooting==0) 
	{
		if(IsOutWall(TRUE))
		{
			return m_footingStt.m_bIs ? &m_footingStt : NULL;
		}
		else
		{
			CWallApp *pWall	= GetInWall(0);
			if(pWall)
			{
				if(pWall->m_footing.m_bIs) return &pWall->m_footing;
			}
		}
	}
	else if(nFooting==nCountFooting-1)
	{
		if(IsOutWall(FALSE))
		{
			return m_footingEnd.m_bIs ? &m_footingEnd : NULL;
		}
		else
		{
			CWallApp *pWall = GetInWall(nCountInWall-1-nIdxSttInWallFooting);
			if(pWall)
			{
				if(pWall->m_footing.m_bIs) return &pWall->m_footing;
			}
		}
	}
	else
	{
		if(nFooting < nCountInWall+nIdxSttInWallFooting && nFooting >= nIdxSttInWallFooting)
		{
			CWallApp *pWall = GetInWall(nFooting-nIdxSttInWallFooting);
			if(pWall->m_bIs && pWall->m_footing.m_bIs)
				return &pWall->m_footing;
		}
	}

	return NULL;
}

BOOL CRcBridgeData::IsBoxType()
{
	return m_nTypeBridge == BRIDGE_TYPE_BOX;
}

BOOL CRcBridgeData::IsArchType()
{
	return m_nTypeBridge == BRIDGE_TYPE_ARCH;
}

// 기초가 있는지 체크
BOOL CRcBridgeData::IsFooting(BOOL bStt)
{
	if(bStt)
	{
		if(IsOutWall(TRUE) && m_footingStt.m_bIs) return TRUE;
	}
	else
	{
		if(IsOutWall(FALSE) && m_footingEnd.m_bIs) return TRUE;
	}

	return FALSE;
}

BOOL CRcBridgeData::IsFootingJijum(long nJijum)
{
	BOOL bExistFoot = FALSE;
	if(IsBoxType() == TRUE)
	{
		bExistFoot = (nJijum == 0)? TRUE : FALSE;
	}
	else
	{
		CFootingApp *pFooting	= GetFootingByJijumNumber(nJijum);
		if(pFooting && pFooting->m_bIs)
		{
			bExistFoot = TRUE;
		}
	}

	return bExistFoot;
}

// 기초 개수 리턴
long CRcBridgeData::GetCountFooting()
{
	if(IsBoxType())
	{
		// 입력 Data 때문에 m_footingbox가 있는것이지 기초가 있는건 아니다.
		return 1;
	}

	long nCountFooting	= 0;
	if(IsOutWall(TRUE) && m_footingStt.m_bIs) nCountFooting++;
	if(IsOutWall(FALSE) && m_footingEnd.m_bIs) nCountFooting++;

	long nCountInWall	= GetCountInWall();
	for(long wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= GetInWall(wall);
		if(pWall->m_footing.m_bIs && !pWall->m_footing.IsVirtualFooting()) nCountFooting++;
	}

	return nCountFooting;
}

// 중간벽체 포인터로 몇 번째 중간벽체인지 알아낸다.
long CRcBridgeData::GetIdxInWall(CWallApp *pWall)
{
	long nCountInWall	= GetCountInWall();
	long nIdx	= -1;
	long i = 0; for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWallCur	= GetInWall(i);
		if(pWall == pWallCur)
		{
			nIdx	= i;
			break;
		}
	}

	return nIdx;
}

// 접속슬래브 존재 유무
BOOL CRcBridgeData::IsAps(BOOL bStt, BOOL bLeft)
{
	long left		= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &m_apsStt[left] : &m_apsEnd[left];

	BOOL bResult	= pAps->m_bIs;

	return bResult;
}

// 접속 슬래브가 하나라도 있는지
BOOL CRcBridgeData::IsAnyAps()
{
	if(IsAps(TRUE, TRUE))
		return TRUE;
	if(IsAps(TRUE, FALSE))
		return TRUE;
	if(IsAps(FALSE, TRUE))
		return TRUE;
	if(IsAps(FALSE, FALSE))
		return TRUE;

	return FALSE;
}

// 완충슬래브 존재 유무
BOOL CRcBridgeData::IsCns(BOOL bStt, BOOL bLeft, long nIdx)
{
	BOOL bResult	= FALSE;
	// 일단 접속슬래브가 있어야 완충슬래브도 있다. 음..
	if(IsAps(bStt, bLeft) && nIdx > -1 && nIdx < 2)
	{
		long left		= bLeft ? iLEFT : iRIGHT;
		CApsApp *pCns	= bStt ? &m_cnsStt[left][nIdx] : &m_cnsEnd[left][nIdx];

		bResult			= pCns->m_bIs;
	}

	return bResult;
}

long CRcBridgeData::GetCountCycleMainRebar()
{
	long nCountCycle	= 2;
	if(m_nIdxCountCycleMainRebar == 0) nCountCycle	= 2;
	else if(m_nIdxCountCycleMainRebar == 1) nCountCycle	= 4;

	return nCountCycle;
}
// void CRcBridgeData::SetHDanRoad(long nQtyHDan, long nIndex)
// {
// 	if(nIndex<0 || nIndex>=m_nQtyHDanRoad) return;
// 	long nQtyHDanOld = m_nQtyHDanRoad;
// 	m_nQtyHDanRoad = nQtyHDan;
// 	if(m_nQtyHDanRoad==nQtyHDanOld) return;
// 	BOOL bAddBlock = (nQtyHDanOld<m_nQtyHDanRoad) ? TRUE : FALSE;
// 	double dBlockLength[MAX_QTY_HDAN];
// 	double dBlockSlope[MAX_QTY_HDAN];
// 	long   nBlockType[MAX_QTY_HDAN];
// 	long nHDan=0; for(nHDan=0; nHDan<MAX_QTY_HDAN; nHDan++)
// 	{
// 		dBlockLength[nHDan]	  = m_dLengthHDanRoad[nHDan];
// 		dBlockSlope[nHDan]	  = m_dSlopeHDanRoad[nHDan];
// 		nBlockType[nHDan]	  = m_nTypeHDanRoad[nHDan];
// 		if(nHDan>nIndex)
// 		{
// 			m_dLengthHDanRoad[nHDan]	= 0;
// 			m_dSlopeHDanRoad[nHDan]	= 0;
// 			m_nTypeHDanRoad[nHDan]		= 0;
// 		}
// 	}
// 	if(bAddBlock)
// 	{
// 		nHDan = nIndex;
// 		m_dLengthHDanRoad[nHDan++] = m_dLengthHDanRoad[nIndex]/2;
// 		m_dLengthHDanRoad[nHDan]	= m_dLengthHDanRoad[nHDan-1];
// 		m_dSlopeHDanRoad[nHDan]	= m_dSlopeHDanRoad[nHDan-1];
// 		m_nTypeHDanRoad[nHDan]		= m_nTypeHDanRoad[nHDan-1];
// 		for(long j=0; nHDan<MAX_QTY_HDAN; nHDan++,j++)
// 		{
// 			m_dLengthHDanRoad[nHDan] = dBlockLength[nIndex+j+1];
// 			m_dSlopeHDanRoad[nHDan]  = dBlockSlope[nIndex+j+1];
// 			m_nTypeHDanRoad[nHDan]	  = nBlockType[nIndex+j+1];
// 		}
// 	}
// 	else
// 	{
// 		long nIdxCopy = nIndex + 2;
// 		if(nIndex==nQtyHDanOld-1)
// 		{
// 			nHDan = nIndex-1;
// 			m_dLengthHDanRoad[nHDan] = dBlockLength[nHDan] + dBlockLength[nHDan+1];
// 			return;
// 		}
// 		else
// 		{
// 			nHDan = nIndex;
// 			m_dLengthHDanRoad[nHDan] = dBlockLength[nHDan] + dBlockLength[nHDan+1];
// 			nHDan++;
// 		}
// 		for(long j=0; nHDan<MAX_QTY_HDAN; nHDan++,j++)
// 		{
// 			m_dLengthHDanRoad[nHDan] = dBlockLength[nIdxCopy+j];
// 			m_dSlopeHDanRoad[nHDan]  = dBlockSlope[nIdxCopy+j];
// 			m_nTypeHDanRoad[nHDan]	  = nBlockType[nIdxCopy+j];
// 		}
// 	}
// }
CString CRcBridgeData::GetStringBridgeSpan()
{
	CARcBridgeDataStd* pData = m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CString strSpan = _T("");
	double dLenCurr = m_dLengthJigan[0];
	double dLenPrev = bLsd? m_dLengthJigan[0] : toM(m_dLengthJigan[0]);
	
	CString strBridgeSpan = _T("");
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		strBridgeSpan.Format("%d Spandrel", m_nQtyJigan);
	}
	else
	{
		CStringArray sArrSpan;

		long nEaSpan = 1;

		for(long nSpan=1; nSpan<m_nQtyJigan; nSpan++)
		{
			dLenCurr = bLsd? m_dLengthJigan[nSpan] : toM(m_dLengthJigan[nSpan]);
			if(dLenCurr!=dLenPrev)
			{
				if(nEaSpan>1)
				{
					strSpan.Format("%d@%s",nEaSpan, pData->GetStrDouble(dLenPrev, UNIT_CGS_M));
					sArrSpan.Add(strSpan);
				}
				//			else
				//				strSpan.Format("%.3f",toM(dLenCurr));
				//			if(nSpan==1)
				else
				{
					strSpan.Format("%s", pData->GetStrDouble(dLenPrev, UNIT_CGS_M));
					sArrSpan.Add(strSpan);
				}
				//			sArrSpan.Add(strSpan);
				nEaSpan = 1;
			}
			else
			{
				nEaSpan++;
			}
			
			if(nSpan==m_nQtyJigan-1)
			{
				if(nEaSpan>1) strSpan.Format("%d@%s",nEaSpan, pData->GetStrDouble(dLenCurr, UNIT_CGS_M));
				else strSpan.Format("%s", pData->GetStrDouble(dLenCurr, UNIT_CGS_M));
				sArrSpan.Add(strSpan);
			}
			dLenPrev = dLenCurr;
		}
		
		long i=0; for(i=0; i<sArrSpan.GetSize(); i++)
		{
			strSpan = sArrSpan.GetAt(i);
			if(i==0)
				strBridgeSpan += strSpan;
			else
				strBridgeSpan += (" + " + strSpan);
		}
	}
	
	return strBridgeSpan;
}


// int CRcBridgeData::GetCountWingWall()
// {
// 	int nCount = 0;
// 
// 	CWingWall* pWing = GetWingWall(TRUE, TRUE);
// 	if(pWing->m_bExist)
// 		nCount++;
// 	pWing = GetWingWall(TRUE, FALSE);
// 	if(pWing->m_bExist)
// 		nCount++;
// 	pWing = GetWingWall(FALSE, TRUE);
// 	if(pWing->m_bExist)
// 		nCount++;
// 	pWing = GetWingWall(FALSE, FALSE);
// 	if(pWing->m_bExist)
// 		nCount++;
// 
// 	return nCount;
// }

double CRcBridgeData::GetCoverFooting(BOOL bStt, BOOL bUpper, long nDan)
{
	return bStt ? (bUpper ? m_footingStt.m_dCoverUpper[nDan] : m_footingStt.m_dCoverLower[nDan]) : 
				  (bUpper ? m_footingEnd.m_dCoverUpper[nDan] : m_footingEnd.m_dCoverLower[nDan]);
}
BOOL CRcBridgeData::IsPileBase(long nJijum)
{
	if(IsBoxType())
	{
		return m_nTypeLower == BOXFOOTING_TYPE_PILE;
	}

	BOOL bPileBase = FALSE;

	long nJijumStt = (nJijum!=-1) ? nJijum : 0;
	long nJijumEnd = (nJijum!=-1) ? nJijum : GetCountJijum() - 1;
	long i=0; for(i=nJijumStt; i<=nJijumEnd; i++)
	{
		CFootingApp *pFooting  = GetFootingByJijumNumber(i);
		if(pFooting)
		{
			bPileBase = (pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_PILE) ? TRUE : FALSE;
		}
		if(bPileBase) break;
	}
	return bPileBase;
}

double CRcBridgeData::GetLoadFactor(long nTypeCombo, long nCombo, long nLoad)
{
	CMatrix MatCombo;
	if(nTypeCombo==LOAD_COMBINE_USE) MatCombo = m_MatComboUse;
	if(nTypeCombo==LOAD_COMBINE_ULT) MatCombo = m_MatComboUlt;
	if(nTypeCombo==LOAD_COMBINE_ECC) MatCombo = m_MatComboEcc;
	if(nTypeCombo==LOAD_COMBINE_EQ)  MatCombo = m_MatComboEQ;
	
	if(nCombo >= MatCombo.GetRow() || nCombo < 0) return 0;
	if(nLoad >= MatCombo.GetCol() || nLoad < 0) return 0;

	return MatCombo(nCombo,nLoad);
}

// nIDDialog 입력창에서 dwDefault 형태의 권고안을 해야 될지?
// 만약 배열에서 입력창을 찾지 못 했다면 추가한뒤 권고안을 하도록 해준다.
// 주의!! 여기서 권고안 여부를 체크하면 자동으로 권고안이 되었다고 체크가 된다.
// 따라서 권고안 체크를 할 경우 반드시 권고안을 진행해야 된다.
BOOL CRcBridgeData::IsDefaulted(long nIDDialog, DWORD dwDefault)
{
	CTypeDefault *pTD	= GetTypeDefault(nIDDialog);
	if(!pTD) return FALSE;

	BOOL bDefaulted	= pTD->IsDefaulted(dwDefault);
	pTD->m_dwDefaulted |= dwDefault;
	return bDefaulted;
}

// 완전 초기화 상태인지? (해당 다이얼로그에 대해서만 검사)
// 여기서 체크를 당하면 모든 플래그가 ON으로 된다.
BOOL CRcBridgeData::IsFirstDefault(long nIDDialog)
{
	CTypeDefault *pTD	= GetTypeDefault(nIDDialog);
	if(!pTD) return TRUE;

	return pTD->m_dwDefaulted == 0x00000000;
}

void CRcBridgeData::SetDefaultedState(long nIDDialog, DWORD dwDefaultState)
{
	CTypeDefault *pTD	= GetTypeDefault(nIDDialog);
	if(pTD)
	{
		pTD->m_dwDefaulted	= dwDefaultState;
	}
}

CTypeDefault* CRcBridgeData::GetTypeDefault(long nIDDialog)
{
	long i = 0; for(i = 0; i < m_pArrTypeDefault.GetSize(); i++)
	{
		CTypeDefault *pTypeDefault	= m_pArrTypeDefault.GetAt(i);

		if(pTypeDefault->m_nIDDialog == nIDDialog)
		{
			return pTypeDefault;
		}
	}

	CTypeDefault *pTD	= new CTypeDefault;
	pTD->m_nIDDialog	= nIDDialog;
	pTD->m_dwDefaulted	= 0x00000000;

	m_pArrTypeDefault.Add(pTD);
	return pTD;
}
// 지점별로 헌치 철근이 들어갈 수 있는지 검사 있는지 검사
BOOL CRcBridgeData::IsHunch(long nJ, BOOL bUpper)
{
	double dHB = 0;
	double dH	= GetHeightHunchByJijum(nJ, bUpper);	// 헌치 높이
	double dW	= GetWidthHunchByJijum(nJ, bUpper);	// 헌치 하면 너비

	//중간에 있는헌치일경우 헌치아래 돌출 높이가 있어도 헌치가 존재.
	if(nJ > 0 && nJ <= m_nQtyJigan)
		dHB	= m_dHBH[nJ-1][bUpper ? iUPPER : iLOWER];

	if((dH > 0 && dW > 0) || dHB > 0) return TRUE;

	return FALSE;
}


// 경사부분에 해당하는 헌치 높이
// BOOL bIncludeVertHunch ; 경사 아래 수직 부분 헌치를 포함할지?
double CRcBridgeData::GetThickHunch(long nJijum, BOOL bStt, BOOL bIncludeVertHunch, BOOL bUpper)
{
	if(nJijum==0)			return bUpper? m_dHHS : m_dWSHH;
	if(nJijum==m_nQtyJigan) return bUpper? m_dHHE : m_dWEHH;

	double dHeight	= bStt ? m_dHHL[nJijum-1][bUpper? iUPPER:iLOWER] : m_dHHR[nJijum-1][bUpper? iUPPER:iLOWER];
	if(bIncludeVertHunch && nJijum > 0 && nJijum < m_nQtyJigan)
	{
		dHeight += m_dHBH[nJijum-1][bUpper? iUPPER:iLOWER];
	}

	return dHeight;
}

// 경사부분에 해당하는 헌치 폭
double CRcBridgeData::GetWidthHunch(long nJijum, BOOL bStt, BOOL bUpper)
{
	if(nJijum==0)			return bUpper? m_dHWS : m_dWSHW;
	if(nJijum==m_nQtyJigan) return bUpper? m_dHWE : m_dWEHW;

	return bStt ? m_dHLW[nJijum-1][bUpper? iUPPER:iLOWER] : m_dHRW[nJijum-1][bUpper? iUPPER:iLOWER];
}


//#define COL_TYPE_CIRCLE		0
//#define COL_TYPE_RECTANGLE	1
//#define COL_TYPE_TRACK		2
//#define COL_TYPE_POLY			3
// -1 : 벽체가 없거나 기둥이 아님...
long CRcBridgeData::GetTypeColumnByJijum(long nJijum)
{
	if(nJijum==0)			return -1;
	if(nJijum==m_nQtyJigan) return -1;

	CWallApp *pWall = GetInWall(nJijum-1);

	if(pWall->m_bTopIsColumn==FALSE) 
		return -1;
	else
		return pWall->m_Column.m_nType;
}

//<summary>
//선분1과 선분2의 교차점을 구하는 함수=====>OK
//선분1과 선분2를 직선으로 하여 교차점을 구한 값이 xyResult이고
//교차점이 두 선분위에 있을 때 TRUE를 return 한다.
// 가능하면 <LineAndAngle.h> 로 옮길 것, ACommon 으로 옮길 것
//</summary>
//<param name="xyStt">선분1의 시점</param>
//<param name="xyEnd">선분1의 종점</param>
//<param name="A[30]">선분 Array</param>
//<param name="xyResult">선분1과 선분2의 교점좌표</param>
//<return>선분1과 선분2의 교차여부</return>
BOOL CRcBridgeData::GetXyMatchSegAndSegTwinVector(CDPoint xyStt,CDPoint xyEnd, CTwinVectorArray& tvArr, long nQtyXy, CDPoint &xyResult)
{
	if(xyStt == xyEnd)	return FALSE;
	CDPoint xyReal;
	double dis = 0;
	double disOld = 0;
	BOOL bMatch = FALSE;
	long n=0; for(n=0; n < nQtyXy; n++)
	{
		if(!GetXyMatchSegAndSeg(tvArr.GetAt(n).m_v1, tvArr.GetAt(n).m_v2,xyStt,xyEnd,xyReal))
			continue;

		dis = ~(xyStt - xyReal);
		if(bMatch == FALSE)		//처음으로 만났을 경우
		{
			xyResult = xyReal;
			disOld = dis;
		}
		else
		{
			if(dis < disOld)	//거리가 가장 짧은 경우를 찾음
			{
				xyResult = xyReal;
				disOld = dis;
			}

		}
		bMatch = TRUE;
	}


	return bMatch;
	
}

//횡단개수만큼 방호벽을 생성, 방호벽 번호, 횡단 번호가 같음
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeData::MakeGuardWall(long nQtyMake, BOOL bRoadNotSlab)
{
	if(bRoadNotSlab)
		AhTPAMake(nQtyMake, &m_pArrGuardWallRoad, (CGuardWallRC*)0);
	else
		AhTPAMake(nQtyMake, &m_pArrGuardWall, (CGuardWallRC*)0);
}

//<summary>
// 모든 방호벽 객체를 가져옴.
// 방호벽 타입 - 사용자 입출력이외에 도면생성, 계산 등 프로그램 내부에서 사용 
//	도로교인 경우 : 프로그램 내부적으로는 <방호벽 타입, 보도>로만 구분하여 사용
//		방호벽 및 중분대
//		보차도 경계석
//		방음벽 기초
//		흙막이벽
//		난간기초
//		제외 항목 : 차도, 보도
//	철도교인 경우 : 프로그램 내부적으로는 <방호벽 타입(연석), 도상>로만 구분하여 사용
//		연석
//		자갈막이
//		연석+보도
//		통신구
//		통신구+보도
//		제외 항목 : 없음, 교측보도, 직결식도상, 자갈도상
//</summary>
//<param name="nHDan">횡단개수만큼 방호벽을 생성, 방호벽 번호, 횡단 번호가 같음</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CGuardWallRC* CRcBridgeData::GetGuardWallByHDan(long nHDan, BOOL bRoadNotSlab)
{
	long nQtyGW = GetQtyGuardWall(bRoadNotSlab);
	if(nQtyGW <= nHDan || nHDan < 0) return NULL;

	if(bRoadNotSlab)
		return m_pArrGuardWallRoad.GetAt(nHDan);

	return m_pArrGuardWall.GetAt(nHDan);
}

// BOOL bNameForExcelSheet : Excel Sheet명으로 사용될경우 / * < 등의 특수문자는 사용하면 안된다.
CString CRcBridgeData::GetGuardWallName(CGuardWallRC* pGW, BOOL bNameForExcelSheet)
{
	CString str(_T(""));
	
	if(!m_pARcBridgeDataStd->IsRailDesign())
	{
		if(pGW->IsTypeHDanOnlyNone())					str = _T("차도");
		else if(pGW->IsTypeHDanRoadGuardFence())		str = _T("방호벽");
		else if(pGW->IsTypeHDanRoadCentralWallFull())	str = _T("중분대");
		else if(pGW->IsTypeHDanRoadCentralWallHalf())	str = bNameForExcelSheet? _T("중분대1_2") : _T("중분대1/2");
		else if(pGW->IsTypeHDanRoadFootway())			str = _T("보도");
		else if(pGW->IsTypeHDanRoadFootwayBlock())		str = _T("보차도 경계석");
		else if(pGW->IsTypeHDanRoadSoundProof())		str = _T("방음벽 기초");
		else if(pGW->IsTypeHDanRoadRetainingWall())		str = _T("흙막이벽");
		else if(pGW->IsTypeHDanRoadHandRail())			str = _T("난간기초");
		else if(pGW->IsTypeHDanRoadRailDike())			str = _T("다이크");
	}
	else
	{
		if(pGW->IsTypeHDanOnlyNone())					str = _T("없음");
		else if(pGW->IsTypeHDanRailGuardFence())		str = _T("연석");
		else if(pGW->IsTypeHDanRailDetectGravel())		str = _T("자갈막이(철근)");
		else if(pGW->IsTypeHDanRailDetectGravel2())		str = _T("자갈막이(무근)");
		else if(pGW->IsTypeHDanRailFootway())			str = _T("보도");
		else if(pGW->IsTypeHDanRailDosangVer())			str = _T("직결식도상");
		else if(pGW->IsTypeHDanRailDosangGravel())		str = _T("자갈도상");
		else if(pGW->IsTypeHDanRailFootwayBlock())		str = _T("연석+보도");
		else if(pGW->IsTypeHDanRailHole())				str = _T("통신구");
		else if(pGW->IsTypeHDanRailHoleFootway())		str = _T("통신구+보도");
		else if(pGW->IsTypeHDanRoadRailDike())			str = _T("다이크");
	}

	return str;
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyGuardWall(BOOL bRoadNotSlab) const
{
	if(bRoadNotSlab)
		return m_pArrGuardWallRoad.GetSize();

	return m_pArrGuardWall.GetSize();
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetQtyHDan(BOOL bRoadNotSlab) const
{
	if(bRoadNotSlab)
		return m_nQtyHDanRoad;

	return m_nQtyHDan;
}


BOOL CRcBridgeData::IsRcSlab()
{
	return (m_nTypeBridge==BRIDGE_TYPE_SLAB);
}

BOOL CRcBridgeData::IsPiRahmen()
{
	return (m_nTypeBridge==BRIDGE_TYPE_PI || m_nTypeBridge==BRIDGE_TYPE_TRAP);
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeData::GetLengthHDan(long nHDan, BOOL bRoadNotSlab) const
{
	if(bRoadNotSlab)
		return m_dLengthHDanRoad[nHDan];

	return m_dLengthHDan[nHDan];
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeData::GetTypeHDan(long nHDan, BOOL bRoadNotSlab) const
{
	if(bRoadNotSlab)
		return m_nTypeHDanRoad[nHDan];

	return m_nTypeHDan[nHDan];
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeData::GetSlopeHDan(long nHDan, BOOL bRoadNotSlab) const
{
	if(bRoadNotSlab)
		return m_dSlopeHDanRoad[nHDan];

	return m_dSlopeHDan[nHDan];
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeData::GetThickPave(BOOL bRoadNotSlab) const
{
	if(m_bUnderGroundRahmen)
	{
		if(bRoadNotSlab)		//지중라멘교이고 노면 포장 두께를 가지고 올 경우
			return m_dUnderGroundHeightPavement;
		return m_dUnderGroundHeightMortar;//지중라멘교이고 슬래브 방수 몰탈를 두께를 가지고 올 경우
	}
	else
	{
		if(m_pARcBridgeDataStd->IsRailDesign())
			return m_dUnderGroundHeightMortar;
	}

	return m_dTP;
}


// 두 직선에 접하는 아크의 중심좌표와 시작점 끝점 리턴
BOOL CRcBridgeData::GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2)
{
	CRebarPlacing rb;

	if(Compare(dR, 0.0, "="))
	{
		xyArc1	= xyOrg;
		xyArc2	= xyOrg;
		xyCen	= xyOrg;
		return FALSE;
	}

	double dAng1	= vAng1.GetAngleDegree();
	double dAng2	= vAng2.GetAngleDegree();
	double dAngMid	= dAng1>dAng2 ? ((dAng1+dAng2)/2) : ((dAng1+360+dAng2)/2);
	CDPoint vAngMid	= ToDPointFrDegree(dAngMid);

	BOOL bLeft		= TRUE;	// vAng1이 vAng2의 왼쪽에 있는지 판단.
	if(rb.IsIncludeAngle(dAng1, dAng2, dAng2+180)) bLeft = TRUE;
	else bLeft	= FALSE;

	CTwinVector tvLineOrg;	// 모든 중심점은 이 선상에 잇게 된다.
	tvLineOrg.m_v1	= xyOrg;
	tvLineOrg.m_v2	= xyOrg + (vAng2 * 1000);
	Offset(tvLineOrg.m_v1, tvLineOrg.m_v2, bLeft ? -dR : dR);

	// 중심좌표 구함.
	GetXyMatchLineAndLine(xyOrg, vAngMid, tvLineOrg.m_v1, tvLineOrg.GetXyDir(), xyCen);

	// 첫번째 점(중심에서 첫번째 점에 수직으로 뻗음)
	GetXyMatchLineAndLine(xyCen, vAng1.Rotate90(), xyOrg, vAng1, xyArc1);

	// 두번째 점(중심에서 두번째 점에 수직으로 뻗음)
	GetXyMatchLineAndLine(xyCen, vAng2.Rotate90(), xyOrg, vAng2, xyArc2);

	return TRUE;
}
// 침목아래 도상높이
// double CRcBridgeData::GetHeightDosangUnderTie()
// {
// 	return m_dHeightDosangUnderTie;// 침목아래 도상높이
// // 	return TRUE;
// }

CConcBasic *CRcBridgeData::GetConcBasicByTitle(CSafetyTypedPtrArray <CObArray, CConcBasic*> *pArrConc, CString sTitle)
{
	if(!pArrConc) return NULL;

	CConcBasic *pConc	= NULL;
	CConcBasic *pConcTmp	= NULL;
	long nSize	= pArrConc->GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		pConcTmp	= pArrConc->GetAt(i);
		if(pConcTmp->m_sTitle == sTitle) 
		{
			pConc	= pConcTmp;
			break;
		}
	}

	return pConc;
}

CFEMManage* CRcBridgeData::GetFemWingWall(long nType)
{
	if(nType == 0)
		return &m_FemWingWall[iSTT][iLEFT];
	else if(nType == 1)
		return &m_FemWingWall[iSTT][iRIGHT];
	else if(nType == 2)
		return &m_FemWingWall[iEND][iLEFT];
	else if(nType == 3)
		return &m_FemWingWall[iEND][iRIGHT];

	return NULL;
}

CFEMManage* CRcBridgeData::GetFemManage(long nTypeDesign)
{
	CFEMManage *pFEM = NULL;
	if(nTypeDesign == COMBO_SERVICE || nTypeDesign == COMBO_ULTIMATE || nTypeDesign == COMBO_ECCENTRIC)
	{
		pFEM = &m_FemNormal;
	}
	else if(IsBoxType())
	{
		long nIndexEq = nTypeDesign - COMBO_EARTHQUAKE;
		if(nIndexEq > -1 && nIndexEq < 4)
			pFEM = &m_FemEQ[nIndexEq];
	}else
	{
		if(nTypeDesign == COMBO_EARTHQUAKE)
		{
			pFEM = &m_FemEQ[1];
		}
		else if(nTypeDesign == COMBO_EARTHQUAKE2)
		{
			if(m_nSelectModelingCountEQ == 2)
				pFEM = &m_FemEQ[2];
		}
	}

	return pFEM;
}


CFEMManage* CRcBridgeData::GetFemWingWall(BOOL bStt, BOOL bLeft)
{
	return &m_FemWingWall[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];
}


BOOL CRcBridgeData::IsRoundHunch(long nJ, BOOL bLeft, BOOL bUpperSlab)
{
	BOOL bRound	= FALSE;

	// BOX형은 라운드 헌지 지원안함.
	if(m_nTypeBridge == BRIDGE_TYPE_BOX) return bRound;

	if(nJ == 0)
	{
		if(m_dHRS != 0) bRound	= TRUE;
	}
	else if(nJ == m_nQtyJigan)
	{
		if(m_dHRE != 0) bRound	= TRUE;
	}
	else
	{
		long nUD = bUpperSlab ? iUPPER : iLOWER;
		long nIdxHunch	= nJ-1;
		if(bLeft == TRUE && m_dHLR[nIdxHunch][nUD] != 0 )
			bRound	= TRUE;
		else if(bLeft == FALSE && m_dHRR[nIdxHunch][nUD] != 0)
			bRound	= TRUE;
	}

	return bRound;
}

double CRcBridgeData::GetHeightHunchByJijum(long nJ, BOOL bUpper)
{
	double dH	= 0;

	if(nJ == 0 || nJ == m_nQtyJigan)
	{
		BOOL bStt	= nJ == 0;
		if(bStt)
		{
			dH	= IsOutWall(bStt) ?  (bUpper? m_dHHS : m_dWSHH) : (bUpper? m_dHHS : m_dWSHH);
		}
		else
		{
			dH	= IsOutWall(bStt) ? (bUpper? m_dHHE : m_dWEHH) : (bUpper? m_dHHE : m_dWEHH);
		}
	}
	else
	{
		long nIdxHunch	= nJ-1;
		dH	= min(m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER], m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER]);
	}

	return dH;
}

double CRcBridgeData::GetWidthHunchByJijum(long nJ, BOOL bUpper)
{
	double dW	= 0;

	if(nJ == 0 || nJ == m_nQtyJigan)
	{
		BOOL bStt	= nJ == 0;
		if(bStt)
		{
			dW	= bUpper? m_dHWS : m_dWSHW;
		}
		else
		{
			dW	= bUpper ? m_dHWE : m_dWEHW;
		}
	}
	else
	{
		long nIdxHunch	= nJ-1;
		dW	= m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER] + m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER];
	}

	return dW;
}

double CRcBridgeData::GetWidthHunchBottomByJijum(long nJ, BOOL bUpper)
{
	double dW	= 0;

	if(nJ == 0 || nJ == m_nQtyJigan)
		return 0;
	else
	{
		long nIdxHunch	= nJ-1;
		dW	= m_dWL[nIdxHunch][bUpper? iUPPER:iLOWER] + m_dWR[nIdxHunch][bUpper? iUPPER:iLOWER];
	}

	return dW;
}

// 외측 벽체 헌치가 있는지 (경사교2의 아래쪽 헌치)
BOOL CRcBridgeData::IsOutWallHunch(BOOL bStt)
{
	if(!IsOutWall(bStt)) return FALSE;
	if(IsBoxType()) return FALSE;
	// Box타입은 변수는 사용하지만 하부슬래브 헌치이다.

	if(bStt)
	{
		if(m_dWSHH > 0 && m_dWSHW > 0) return TRUE;
	}
	else
	{
		if(m_dWEHH > 0 && m_dWEHW > 0) return TRUE;
	}
	return FALSE;
}

BOOL CRcBridgeData::IsVariableLenHDan(long nDan, HGBOOL bRoadNotSlab)
{
	if(bRoadNotSlab)
		return m_ZzbVariableLenHDanRoad[nDan];
	else
		return m_ZzbVariableLenHDan[nDan];
}

void CRcBridgeData::SetVariableLenHDan(long nDan, HGBOOL bRoadNotSlab, BOOL bVariable)
{
	if(bRoadNotSlab)
		m_ZzbVariableLenHDanRoad[nDan]	= bVariable;
	else
		m_ZzbVariableLenHDan[nDan]		= bVariable;
}

long CRcBridgeData::GetCountVariableLenHDan(BOOL bRoadNotSlab)
{
	long nCount	= 0;
	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	long i = 0; for(i = 0; i < nQtyHDan; i++)
	{
		if(IsVariableLenHDan(i, bRoadNotSlab))
			nCount++;
	}

	return nCount;
}

void CRcBridgeData::SortTvArrByLinePlane(CTwinVectorArray &tvArr)
{
	long nCount	= tvArr.GetSize();
	CTwinVector tv1, tv2;
// 	double dDist1(0), dDist2(0);
	long i = 0; for(i = 0; i < nCount-1; i++)
	{
		tv1	= tvArr.GetAt(i);
		double dDist1	= atof(LPCSTR(tv1.m_sInfoHor));
		long j = 0; for(j = i + 1; j < nCount; j++)
		{
			tv2	= tvArr.GetAt(j);
			double dDist2	= atof(LPCSTR(tv2.m_sInfoHor));
			
			if(dDist1 > dDist2)
			{
				tvArr.SetAt(i, tv2);
				tvArr.SetAt(j, tv1);
				break;
			}
		}
	}
}

// 지간이라는 글자를 리턴
// 단, 아치교는 지간대신 스팬드럴이라는 용어를 씀.
CString CRcBridgeData::GetStringJigan()
{
	return m_nTypeBridge == BRIDGE_TYPE_ARCH ? "스팬드럴" : "지간";
}

// tvArr에 포물선 추가하기
void CRcBridgeData::AddFromArch(CTwinVectorArray &tvArr, CDPoint xyMove, double dLenTotal, double dHeightTotal, double dMFactor, double dLenStt, double dLenEnd, double dLenArchLine, BOOL bPline)
{
	if(dLenTotal == 0) return;
	CDPoint xyStt, xyEnd;
	BOOL bFirst = TRUE;

	CTwinVector tv;
	tv.m_v1.z	= 0;
	tv.m_v2.z	= 0;
	for(double l = dLenStt;; l += dLenArchLine)
	{
        xyStt = GetXyInArchByCatenary(dLenTotal, dHeightTotal, dMFactor, xyMove, l);
		if(bFirst)
		{
			tv.m_v1	= xyStt;
			tv.m_v2.z	= 10;	// pLine 시작
			bFirst = FALSE;
		}
		else
			tv.m_v2.z	= 11;	// pLine 진행중

		if( l + dLenArchLine >= dLenEnd)
		{
			xyEnd = GetXyInArchByCatenary(dLenTotal, dHeightTotal, dMFactor, xyMove, dLenEnd);
			tv.m_v2	= xyEnd;
			tvArr.Add(tv);
			tv.m_v1.x	= tv.m_v2.x;
			tv.m_v1.x	= tv.m_v2.y;
			break;
		}
		else
		{
			xyEnd = GetXyInArchByCatenary(dLenTotal, dHeightTotal, dMFactor, xyMove, l + dLenArchLine);
		}
		tv.m_v2	= xyEnd;
		tvArr.Add(tv);
		tv.m_v1.x	= tv.m_v2.x;
		tv.m_v1.y	= tv.m_v2.y;
	}

	long nSize	= tvArr.GetSize();
	if(nSize > 0)
	{
		tv	= tvArr.GetAt(tvArr.GetSize()-1);
		tv.m_v2.z	= nSize == 1 ? 0 : 12;	// pLine 끝
		tvArr.SetAt(nSize-1, tv);
	}
}

void CRcBridgeData::AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, BOOL bPline)
{
	AddFromArch(tvArr, pb.m_xyCrown, pb.m_dDistTotal, pb.m_dHeightTotal, pb.m_dFactorM, pb.m_dDistStt, pb.m_dDistEnd, pb.m_dLenArchLine, bPline);
}

void CRcBridgeData::AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, double dDistStt, double dDistEnd, BOOL bPline)
{
	AddFromArch(tvArr, pb.m_xyCrown, pb.m_dDistTotal, pb.m_dHeightTotal, pb.m_dFactorM, dDistStt, dDistEnd, pb.m_dLenArchLine, bPline);	
}

BOOL CRcBridgeData::IsBlockOut(BOOL bStt)
{
	if(IsOutWall(bStt)) return FALSE;

	BOOL bFlexibleJoint	= FALSE;
	long nJ	= bStt ? 0 : m_nQtyJigan;
	if(!m_bExpJoint[nJ]) return FALSE;

	if(bStt)
	{
		bFlexibleJoint	= m_fxJointStt.m_dWidth > 0 && m_fxJointStt.m_dHeight > 0;
	}
	else
	{
		bFlexibleJoint	= m_fxJointEnd.m_dWidth > 0 && m_fxJointEnd.m_dHeight > 0;
	}

	return bFlexibleJoint;
}

// 기본타입 셋팅
// 문형이 꼭 아닐 수 있으므로 지원 되는 것 중에 첫번째 것으로 셋팅해둠
void CRcBridgeData::SetDataDefaultBridgeType()
{
	if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_PORTAL)) m_nTypeBridge	= BRIDGE_TYPE_PORTAL;	// 문형 라멘교
	else if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_PI)) m_nTypeBridge	= BRIDGE_TYPE_PI;	// ㅠ형 라멘교
	else if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_SLAB)) m_nTypeBridge	= BRIDGE_TYPE_SLAB;	// 슬래브교
	else if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_TRAP)) m_nTypeBridge	= BRIDGE_TYPE_TRAP;	// 경사교각라멘교
	else if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_TRAP2)) m_nTypeBridge	= BRIDGE_TYPE_TRAP2;	// 경사교각라멘교2
	else if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_ARCH)) m_nTypeBridge	= BRIDGE_TYPE_ARCH;	// 아치교

	// 슬래브 표준도 작업중인경우 무조건 슬래브교로 권고안 됨
	if(m_pARcBridgeDataStd->m_nTypeProcess == TYPE_PROCESS_SLAB_STANDARD_OUTPUT)
		m_nTypeBridge	= BRIDGE_TYPE_SLAB;
}

void CRcBridgeData::InitSaveCrossSection()
{
	AhTPADelete(&m_pArrSaveCrossSection, (CSaveCrossSection*)0);
}

void CRcBridgeData::InitSaveOrgInWall()
{
	AhTPADelete(&m_pArrSaveOrgInWall, (CSaveOrgInWall*)0);
}	

void CRcBridgeData::InitSaveXySlabUpperByLenFrSttStation()
{
	AhTPADelete(&m_pArrSaveXySlabUpperByLenFrSttStation, (CSaveXySlabUpperByLenFrSttStation*)0);
}
void CRcBridgeData::InitSaveXyMatchLineAndSlabUpper()
{
	AhTPADelete(&m_pArrSaveXyMatchLineAndSlabUpper, (CSaveXyMatchLineAndSlabUpper*)0);
}

void CRcBridgeData::InitSaveDistRigidEndSlab()
{
	AhTPADelete(&m_pArrSaveDistRigidEndSlab, (CSaveDistRigidEndSlab*)0);
}

void CRcBridgeData::InitSaveDistRigidEndWall()
{
	AhTPADelete(&m_pArrSaveDistRigidEndWall, (CSaveDistRigidEndWall*)0);
}


// FL 적용 가능하면..
BOOL CRcBridgeData::IsApplyDisBaseElevationFL()
{
	return m_dDisBaseElevation != 0;
}

long CRcBridgeData::GetQtyHDanNonRebarConcrete(BOOL bRoadNotSlab)
{
	long nQty = 0;
	for(long nHDan = 0; nHDan < GetQtyHDan(bRoadNotSlab); nHDan++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanNonRebarConcrete())
			nQty++;
	}
	return nQty;
}

// 평면도는 선형계산오차로 0.0001 정도의 오차가 있음
// 따라서 sort함수를 따로 만들어서 소트해서 offset함.
void CRcBridgeData::Sort(CTwinVectorArray &tvArr, double dCorrectLevelOfDoubleLoacl)
{
	if(tvArr.GetSize() < 2) return;

	CTwinVector tvPrev, tvNext;
	long nSize	= tvArr.GetSize();
	
	BOOL bFirstStt	= FALSE;
	long nCountRep	= 0;
	long i = 0; for(i = 0; i < nSize-1; i++)
	{
		tvPrev	= tvArr.GetAt(i);
		long j = 0; for(j = i + 1; j < nSize; j++)
		{
			tvNext	= tvArr.GetAt(j);

			// 둘중 하나라도 같으면..
			if(ComparePt(tvPrev.m_v2, tvNext.m_v1, "=", dCorrectLevelOfDoubleLoacl) || ComparePt(tvPrev.m_v2, tvNext.m_v2, "=", dCorrectLevelOfDoubleLoacl))
			{
				bFirstStt	= TRUE;
				if(ComparePt(tvPrev.m_v2, tvNext.m_v2, "=", dCorrectLevelOfDoubleLoacl))
				{
					GetSwap(tvNext.m_v1, tvNext.m_v2);
					GetSwap(tvNext.m_v1.z, tvNext.m_v2.z);
					if(tvNext.m_v1.z == -1 || tvNext.m_v1.z == 1)
					{
						tvNext.m_v1.z *= -1;
						tvNext.m_v2.z *= -1;
					}
				}

				// tvPrev의 바로 다음 인덱스가 아니면 인덱스를 교체함.
				if(j != i+1)
				{
					CTwinVector tmp	= tvArr.GetAt(i+1);
					tvArr.SetAt(i+1, tvNext);
					tvArr.SetAt(j, tmp);
				}
				else
				{
					tvArr.SetAt(j, tvNext);
				}

				break;
			}
		}

		// 첫번째 것이 시작이 아니면 한칸씩 밀어서 다시 검사.
		if(!bFirstStt && nCountRep < nSize)
		{
			nCountRep++;
			CTwinVectorArray tvArrTmp;
			long i = 0; for(i = 0; i < nSize-1; i++)
			{
				tvArrTmp.Add(tvArr.GetAt(i+1));
			}

			tvArrTmp.Add(tvArr.GetAt(0));
			tvArr.RemoveAll();
			tvArr = tvArrTmp;
			i	= 0;
		}
	}
}

BOOL CRcBridgeData::IsIncludeEHP()
{
	long nCountFooting	= GetCountFooting();

	for(long footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= GetFooting(footing);
		if(!pFooting) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			if(pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_EHP)
				return TRUE;
		}
	}

	return FALSE;
}

CString CRcBridgeData::GetEhpDxfNameByDia(long nKindDia)
{
	CString sSpcDxfName = _T("");

	if(nKindDia == EHP_KIND_550)
		sSpcDxfName = _T("내진말뚝550.dxf");
	else if(nKindDia == EHP_KIND_700)
		sSpcDxfName = _T("내진말뚝700.dxf");
	else if(nKindDia == EHP_KIND_900)
		sSpcDxfName = _T("내진말뚝900.dxf");
	else if(nKindDia == EHP_KIND_1200)
		sSpcDxfName = _T("내진말뚝1200.dxf");
	else if(nKindDia == EHP_KIND_1450)
		sSpcDxfName = _T("내진말뚝1450.dxf");
	
	return sSpcDxfName;
}

void CRcBridgeData::SetDataInitForChangeType()
{
	long i(0), left(0), cns(0);
	
	// 교량 형식 및 제원

	// 횡단 위치 입력	
	m_bLevelFrRoadCenterToEdge	= TRUE; 
	m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	= TRUE;		// --도로중심간격 레벨 적용(지중라멘의 슬래브 전용) 20071030
	m_dDisBaseElevationRL		= 0; 
	m_dUnitWeightRail			= 0.15;
	m_dHeightRail				= 145.6917; 
	m_bApplyLineSlope			= TRUE; 
	// 지중라멘교
	m_bUnderGroundRahmen		= FALSE;
	m_bCalcOpen					= FALSE;
	m_bPrevApplyCalcOpen		= FALSE;
	m_bUnderGroundPaveDetail	= TRUE;
	m_dUnderGroundSlopeSlabLeft	= 0;
	m_dUnderGroundSlopeSlabRight= 0;
	m_dUnderGroundCenterELStt	= 0;
	m_dUnderGroundCenterELEnd	= 0;
	m_dWidthRoadLeft	= 0;
	m_dWidthRoadRight	= 0;
	m_dUnderGroundNoriSlopeFirst= 0;
	m_dUnderGroundNoriSlopeNext	= 0;
	m_dUnderGroundNoriMaxHeight	= 0;
	m_dUnderGroundNoriWidth		= 0;
	m_dUnderGroundNoriSolpeSodan= 0;
	m_nQtyHDanRoad	= 0;
	for(i=0; i<MAX_QTY_HDAN; i++)
	{
		m_dLengthHDanRoad[i] = 0;
		m_bLevelHDanRoad[i]  = FALSE;
		m_dSlopeHDanRoad[i]  = -0.02;
		m_nTypeHDanRoad[i]   = 0;
		m_ZzbVariableLenHDan[i]	= FALSE;
		m_ZzbVariableLenHDanRoad[i] = FALSE;

		m_dLengthRail[i]	  = 0;
		m_nLineNumber[i]	  = 0;
		m_bLevelHDan[i]  = FALSE;
		m_dSlopeHDan[i]		  = -0.02;
	}
	m_dRail_HeightTie	 = 0;
	m_dRail_LengthTie	 = 0;
	m_dRail_WidthTie	 = 0;
	m_dRail_DistBetweenWheel = 0;
	m_dRail_DistBetweenRail = 0;;

	m_nTypePavement					= 0;
	m_dUnderGroundHeightPavement	= 0;
	m_bUnderGroundDesignHeight		= TRUE;
	m_dUnderGroundDesignHeight		= 0;
	m_nUnderGroundDesignType		= 0;
	m_nUnderGroundDesignSelect		= 0;
	m_dUnderGroundDesignLiveAlways	= 0;
	m_dUnderGroundHeightMortar		= 0;
	m_dUnderGroundWeightMortar_xx	= 0;
	m_dUnderGroundPaveHeightPyo		= 0;
	m_dUnderGroundPaveHeightGee		= 0;
	m_dUnderGroundPaveHeightMiddle	= 0;
	m_dUnderGroundPaveHeightBozo	= 0;
	m_dUnderGroundPaveHeightDong	= 0;
	m_dUnderGroundPaveHeightEps		= 0;
	m_dUnderGroundPaveHeightTosa_XXX	= 0;
	m_dUnderGroundPaveWeightPyo		= 0;
	m_dUnderGroundPaveWeightGee		= 0;
	m_dUnderGroundPaveWeightMiddle	= 0;
	m_dUnderGroundPaveWeightBozo	= 0;
	m_dUnderGroundPaveWeightDong	= 0;
	m_dUnderGroundPaveWeightEps		= 0;
	m_dUnderGroundPaveWeightTosa	= 0;
	m_dUnderGroundPaveWidthPyo		= 0;
	m_dUnderGroundPaveWidthGee		= 0;
	m_dUnderGroundPaveWidthMiddle	= 0;
	m_dUnderGroundPaveWidthBozo		= 0;
	m_dUnderGroundPaveWidthDong		= 0;
	m_dUnderGroundPaveWidthEps		= 0;
	m_dUnderGroundPaveWidthTosa		= 0;
	// 지간 구성 입력
	for(i=0; i<MAX_JIJUM_COUNT_RC-1; i++)
	{
		m_dLengthJigan[i] = 0;				// 지간 길이
	}
	for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		m_bExpJoint[i] = FALSE;				// 신축 이음 설치 유무
	}
	m_arrStrJijumLen.RemoveAll();
	m_arrJijumLen.RemoveAll();
	m_bEWDlgUsed = FALSE;
	m_nTypeShoeAngle = 0;
	m_bIsExp		= FALSE;			// 가각(학폭) 지원할지?
	// 확폭 제원
	for(i=0; i<2; i++)
	{
		m_dExpSttW[i] = 0;	// 시점부
		m_dExpSttH[i] = 0;
		m_dExpSttR[i] = 0;
		m_dExpEndW[i] = 0;	// 종점부
		m_dExpEndH[i] = 0;
		m_dExpEndR[i] = 0;
		m_bExpCutStt[i]	= FALSE;	// [0] : 좌
		m_bExpCutEnd[i]	= FALSE;	// [1] : 우
		m_bExpSttRoundIsTan[i]	= TRUE;	// 가각 시점부 R 타입이 접선방식인지?(TRUE : 접선, FALSE : 그냥 직선을 이은 아크인지)
		m_bExpEndRoundIsTan[i]	= TRUE;
	}

	m_nNotchType	 =  0;
	m_dNotchHeight	 = 20;
	m_dNotchWidth1	 = 30;
	m_dNotchWidth2	 = 30;
	m_dConsJointHeight  =  50;
	m_dConsJointWidth1	=  50;
	m_dConsJointWidth2	= 100;

	// 교량 받침 위치 입력
	// CJijum 클래스와 CShoePos 클래스에서 받음.

	// 횡단 구성 입력
	m_nQtyHDan = 0;							// 횡단 구성 개수
	for(i=0; i<MAX_QTY_HDAN; i++)
	{
		m_dLengthHDan[i] = 0.0;		// 각 횡단 구간의 길이
		m_dSlopeHDan[i] = -0.02;    // 
		m_nTypeHDan[i] = -1;		// 각 구간의 구성 형태

		m_dLengthRail[i] = 0;   // 철도교
		m_nLineNumber[i] = 0;   // 철도교 레일 기준 선형
	}
	m_bGuardWallDlgUsed = FALSE;
	m_bGuardWallUnderDlgUsed = FALSE;
	m_bSSDlgUsed = FALSE;

	m_nRoadMaterial = 0;	// 도로 포장 재질
	m_dTP = 80.0;			// 포장 두께
	m_dTS = 800.0;			// 슬래브 두께
	m_dTSOld = 800.0;
	for(i=0; i<2; i++)
	{
		m_dDH[i] = 165.0;	// 슬래브 끝단에서 헌치까지의 거리
		m_bCantilSlab[i] = FALSE;
		m_nSlabChemfer[i] = 2;	// 슬래브 끝단 형태
		m_dCantilH1[i] = 0.0;
		m_dCantilH2[i] = 0.0;
		m_dCantilB[i] = 0.0;
		m_dRE1[i] = 160.0;	// 슬래브 하부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dRE2[i] = 0;	// 슬래브 상부 끝단 라운드 처리시 반지름, [0]:좌, [1]:우
		m_dTCW1[i] = 0;	// 슬래브 하부 끝단 헌치 처리시 헌치 폭
		m_dTCH1[i] = 0;	// 슬래브 하부 끝단 헌치 처리시 헌치 높이
		m_dTCW2[i] = 0;	// 슬래브 상부 끝단 헌치 처리시 헌치 폭
		m_dTCH2[i] = 0;	// 슬래브 상부 끝단 헌치 처리시 헌치 높이
	}

	// 설계조건 -------------------------------------------------------
	m_dUnitWeigthBackFill			= 0;		// 뒷채움의 단위중량
	m_dAngleFrictionBackFill		= 0;		// 뒷채움의 내부 마찰각
	m_dLiveLoadAlways				= 0;		// 상재하중
	m_dDeadLoadAlways				= 0;
	m_nSelectLiveLoadHori			= 0;		// 
	m_dLiveLoadFootway				= 0;
	m_dUnitWeightHeapSoil			= 2.00;		// 성토재료의 단위중량
	m_dUnitWeightValidDepth			= 0;		// 근입지반의 단위중량
	m_dValidDepthBaseGround			= 0;		// 기초의 유효근입 깊이
	m_dEo_CalcWallSpringEQ			= 0;
	m_nTypeToapSafeCheck			= 0;		// 안정검토시의 토압검토
	m_SafeFactorOverturning[0]		= 0;		// 전도에 대한 안정률(평상시)
	m_SafeFactorOverturning[1]		= 0;		// 전도에 대한 안정률(지진시)
	m_SafeFactorSliding[0]			= 0;		// 활동에 대한 안정률(평상시)
	m_SafeFactorSliding[1]			= 0;		// 활동에 대한 안정률(지진시)
	m_nSelectModelingCountEQ		= 0;		// 좌측편토압에 대해서만 모델링
	m_nSelectFootForce				= 0;
	m_nSelect2DFrameTool			= SOLVE_SAP_6AND7;
	m_nSelect3DShellTool			= 0;
	m_nSelectMidasVer				= 0;
	m_nSelect3DOutputValue			= 0;
	m_bApplyUserDispMax				= FALSE;
	m_dUnitLoadDispMaxUser			= 0.0;
	// ------------------------------------------------------ 설계조건
	m_MatComboUlt.Resize(0,0);	// 계수하중 조합계수
	m_MatComboUse.Resize(0,0);	// 사용하중 조합계수	
	m_MatComboEcc.Resize(0,0);	// 편심하중 조합계수	
	m_MatComboEQ.Resize(0,0);	// 지진하중 조합계수	
	m_dWLoadNormal	= 0x00;		// 하중선택 (평상시)
	m_dWLoadEQ		= 0x00;		// 하중선택 (지진시)
	for(i=0; i<BRIDGE_LOAD_BASIC; i++)
		m_bApplyLoad[i] = FALSE;
	m_bApplyLiveLoadEQ	= FALSE;	// 지진시 활하중재하
	m_bApplyAnchorForce = FALSE;
	for(i=0; i<RCBRIDGE_COMBO_COUNT; i++)
	{
		for(long j=0; j<MAX_RCBRIDGE_COMBO; j++)
			m_bApplyCombo3D[i][j] = FALSE;
	}
	m_dFactorComboVLoad	= 1.0;

	m_bMonumenu							= TRUE;			// 교명주 받침 설치(OLD)
	m_nMonumenu							= 4;				// 교명주 받침 설치
	m_nDesPanel								= 2;				// 설명판 설치개수
	m_nNamePanel							= 2;				// 교명판 설치개수
	m_dAddLenghForGuardWallBM			= 0;		// 방호벽 수량 산출시 추가 길이(OLD)
	m_dAddLenghForGuardWall[0][0]		= 0;		// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[0][1]		= 0;		// 방호벽 수량 산출시 추가 길이 종점 우측
	m_dAddLenghForGuardWall[1][0]		= 0;		// 방호벽 수량 산출시 추가 길이 시점 좌측
	m_dAddLenghForGuardWall[1][1]		= 0;		// 방호벽 수량 산출시 추가 길이 종점 우측
	m_nHeightForSlabBM					= 0;				// 슬래브 거푸집 수량 산출시 높이 기준 
	m_dTackCotting							= 1.5;			// 택 코팅	(ARCBRIDGE-1570) 2.0 -> 1.5 변경요청
	m_nTypePileMilkBM					= 0;
	m_bExcludeApsDowelRebar		= FALSE;
	m_dTimeConcForm						= 120;
	m_bApplyYongSu							= FALSE;
	m_dSurplusSoil_HyunTaPile			= 100;		
	m_dConcreteSoil_HyunTaPile		= 8;		
	m_dConcreteRock_HyunTaPile		= 14;
	// ------------------------------------------------------ 수량선택사항

	// 피복설정 -------------------------------------------------------
	for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverUpper_UpperSlab[i]	= 0;	// 상부슬래브 상면
		m_dCoverLower_UpperSlab[i]	= 0;	// 상부슬래브 하면
		m_dCoverSide_UpperSlab[i]	= 0;	// 상부슬래브 측면
		m_dCoverWallOut[i]	= 0;	// 벽체외면
		m_dCoverWallIn[i]	= 0;	// 벽체 내면
		m_dCoverWallMid[i]	= 0;	// 중간벽체
		m_dCoverColumn[i]	= 0;	// 기둥
		m_dCoverWingOut[i]	= 0;	// 날개벽 외면
		m_dCoverWingIn[i]	= 0;	// 날개벽 내면
		m_dCoverUpper_Footing[i]		= 0;	// 기초 상면
		m_dCoverLower_Footing_Lean[i]	= 0;	// 기초 하면(직접기초시)
		m_dCoverLower_Footing_Pile[i]	= 0;	// 기초 하면(말뚝기초시)
		m_dCoverRib[iUPPER][i]	= 0;
		m_dCoverRib[iLOWER][i]	= 0;
		m_dCoverSide_Footing[i] = 0; //기초 측면
	}
	m_dCoverWingUpper = 0;				// 날개벽 상면
	m_dCoverWingEnd = 0;				// 날개벽 끝단
	// ------------------------------------------------------ 피복설정
	m_bPileNvalueAuto = m_pARcBridgeDataStd->IsLsdDesign()? FALSE : TRUE;

	// 종단면형상 -------------------------------------------------------
	m_nTypeJongdanForRebar		= 0;
	m_nTypeWallForRebarGen		= 0;		// 벽체 표현 방법(일반도 구조도), 0 : 슬래브 끝단, 1 : 시종점, 2 : 실제sta(전면배면에 해당하는)
	m_nTypeOutWall		= 2;	// 외측벽체 유무(좌우)(0 : 없음 -1 : 시점측, 1 : 종점측, 2 : 둘다)

		// 시점측 벽체
	m_dFHS		= 0;
	m_dFLS		= 0;
	m_dFRS		= 0;
	m_dWS		= 0;
	m_dWSHW		= 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dWSHH		= 0;	// 시점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWS		= 0;
	m_dBHHS		= 0;
	m_dBHS		= 0;
	m_dBTHS		= 0;
	m_dBWS		= 0;
	m_dHWS		= 0;
	m_dHHS		= 0;
	m_dHRS		= 0;			// 시점헌치 반경
	m_nQtyRail		= 2;
	m_dUnitWeightRail		= 0.15;
	m_dDisBaseElevation		= 0;
	m_dSlopeDosang		= 0;
	m_dHeightDosangUnderTie		= 0;

	m_dELSB		= 0;
	m_footingStt.m_nType = FOOTING_TYPE_BASIC;
	m_footingStt.m_bLeftIsFrontGup	= TRUE;
	m_footingStt.m_exFooting.m_Pile.m_nTypeOrgDefault	= -1;
	m_footingStt.m_exFooting.m_Pile.m_bLeftIsOrgStt		= FALSE;
	m_footingStt.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;
	m_bDivisionApsStt	= FALSE;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsStt[left]		= 0;	// 완충슬래브 수
		m_apsStt[left].m_bIs		= left == iLEFT ? TRUE : FALSE;
		m_apsStt[left].m_bLeft		= TRUE;
		m_apsStt[left].m_bOrgLeft	= FALSE;
		for(cns = 0; cns < MAX_CNS; cns++)
		{
			m_cnsStt[left][cns].m_bLeft		= m_apsStt[left].m_bLeft;
			m_cnsStt[left][cns].m_bOrgLeft	= m_apsStt[left].m_bOrgLeft;
			m_cnsStt[left][cns].m_nType		= TYPE_APS_CNS;
		}
	}

	
	//m_ExFootingStt.m_nType = EXFOOTING_TYPE_LEAN;

		// 종점측 벽체
	m_dFHE		= 0;
	m_dFLE		= 0;
	m_dFRE		= 0;
	m_dWE		= 0;
	m_dWEHW		= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dWEHH		= 0;	// 종점벽체 헌치너비(경사교2에서만 지원)
	m_dBHWE		= 0;
	m_dBHHE		= 0;
	m_dBHE		= 0;
	m_dBTHE		= 0;
	m_dBWE		= 0;
	m_dHWE		= 0;
	m_dHHE		= 0;
	m_dHRE		= 0;			// 종점헌치 반경
	m_dELEB		= 0;
	m_footingEnd.m_nType = FOOTING_TYPE_BASIC;
	m_footingEnd.m_bLeftIsFrontGup	= FALSE;
	m_footingEnd.m_exFooting.m_Pile.m_nTypeOrgDefault	= -1;
	m_footingEnd.m_exFooting.m_Pile.m_bLeftIsOrgStt		= TRUE;
	m_footingEnd.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;
	m_footingEnd.m_exFooting.m_bLeftOrgOfMass			= TRUE;
	m_bDivisionApsEnd	= FALSE;	// 접속슬래브 분리(횡방향) 설치(각 슬래의 좌우측 으로 분리됨 - 기본은 좌측)
	for(left = 0; left < 2; left++)
	{
		m_nCountCnsEnd[left]		= 0;		// 완충슬래브 수
		m_apsEnd[left].m_bIs		= left == iLEFT ? TRUE : FALSE;
		m_apsEnd[left].m_bLeft		= FALSE;
		m_apsEnd[left].m_bOrgLeft	= TRUE;
		for(cns = 0; cns < 2; cns++)
		{
			m_cnsEnd[left][cns].m_bLeft		= m_apsEnd[left].m_bLeft;
			m_cnsEnd[left][cns].m_bOrgLeft	= m_apsEnd[left].m_bOrgLeft;
			m_cnsEnd[left][cns].m_nType		= TYPE_APS_CNS;
		}
	}
	//m_ExFootingEnd.m_nType = EXFOOTING_TYPE_LEAN;

		// 내측벽체
	m_nCountInWallHunch	= 0;
	for(i = 0; i < MAX_INWALL_HUNCH; i++)
	{
		for(long upper=0; upper<2; upper++)
		{
			m_dHHL[i][upper]	= 0;
			m_dHHR[i][upper]	= 0;
			m_dHBH[i][upper]	= 0;
			m_dHLW[i][upper]	= 0;
			m_dHRW[i][upper]	= 0;
			m_dWL[i][upper]	= 0;
			m_dWR[i][upper]	= 0;
			m_dHLR[i][upper]	= 0;	// 내측헌치 왼쪽 반경
			m_dHRR[i][upper]	= 0;	// 내측헌치 오른쪽 반경
		}
	}
	m_nTypeRoundOfHunchStt	= 0;			// 시점 헌치 0 : 접선(R), 1 : 자유아크(R)
	m_nTypeRoundOfHunchEnd	= 0;			// 종점 헌치 0 : 접선(R), 1 : 자유아크(R)
	for(i = 0; i < MAX_INWALL_HUNCH; i++)
	{
		for(left = 0; left < 2; left++)
			m_nTypeRoundOfHunchInWall[i][left]	= 0;	// 내측헌치 좌우측
	}

	m_bConnectRibAndUpperSlab	= TRUE;
	
	for(i = 0; i < MAX_STRUP_REBAR; i++)
	{
		m_dRibBlockW[i]	= 0;	// 리브위 사각 블럭 폭
		m_dRibBlockH[i]	= 0;	// 리브위 사각 블럭 높이
	}
	// ------------------------------------------------------ 종단면형상

	// 내부 벽체
	AhTPADelete(&m_pArrInWall, (CWallApp*)0);

	// 날개벽 제원 입력
	m_pWingWallStt[0] = new CWingWall;
	m_pWingWallStt[1] = new CWingWall;
	m_pWingWallEnd[0] = new CWingWall;
	m_pWingWallEnd[1] = new CWingWall;
	m_bAutoWingEl = FALSE;
	m_bAutoWingHeightSoil = TRUE;

	// 브래킷 길이
	m_bBracketLengthStt = FALSE;
	m_bBracketLengthEnd = FALSE;
	for(i=0; i<2; i++)
	{
		m_dBrkLenStt[i] = 0.0;
		m_dBrkLenEnd[i] = 0.0;
	}

	// 교명주 받침 제원
	m_dMonuH1 = 0;
	m_dMonuH2 = 0;
	m_dMonuH3 = 0;
	m_dMonuD = 0;
	m_dMonuW = 0;
	m_dMonuL = 0;
	m_dMonuFck = 0;
	m_dMonuR_Fy = 0;
	m_dMonuR_DiaHor = 0;
	m_dMonuR_DiaVer = 0;
	m_dMonuR_H = 0;
	m_dMonuR_L = 0;
	m_nMonuR_nA = 0;
	m_dMonuR_A = 0;
	m_dMonuR_A1 = 0;
	m_nMonuR_nB = 0;
	m_dMonuR_B = 0;
	m_nMonuR_nC = 0;
	m_dMonuR_C = 0;
	m_dMonuR_C1 = 0;
	m_dMonuR_C2 = 0;
	m_dMonuR_C3 = 0;
	m_nMonuR_nD = 0;
	m_dMonuR_D = 0;
	m_dMonuR_D1 = 0;
	m_dMonuR_D2 = 0;
	m_dMonuR_D3 = 0;
	m_dMonuAggregate = 0;
}

void CRcBridgeData::InitStmData()
{
	m_pStmSolverInput = new CStmModelForArcbridge;

	for(long ix=0; ix<2; ix++)
	{
		m_bApplyStmModelingBracket[ix]	= FALSE;
		m_bStmTrussModelingBracket[ix]	= FALSE;

		m_strStmModelingBracket[ix] = _T("");

		for(long jx=0; jx<3; jx++)		// 극한상황
		{
			for(long kx=0; kx<2; kx++)
			{
				for(long lx=0; lx<2; ++lx)
				{
					m_bApplyStmModelingCorner[ix][jx][kx][lx] 	= FALSE;
					m_bStmTrussModelingCorner[ix][jx][kx][lx] 	= FALSE;

					m_strStmModelingCorner[ix][jx][kx][lx] = _T("");
				}
			}
		}

		for(long jx=0; jx<3; jx++)
		{
			for(long kx=0; kx<5; kx++)
			{
				m_bApplyStmModelingFooting[ix][jx][kx]	= FALSE;
				m_bStmTrussModelingFooting[ix][jx][kx]	= FALSE;

				m_strStmModelingFooting[ix][jx][kx] = _T("");
			}
		}
	}

	for(long ix=0; ix<MAX_JIJUM_COUNT_RC; ix++)
	{
		for(long jx=0; jx<3; jx++)
		{
			for (long kx = 0; kx < 5; kx++ )
			{
				m_bApplyStmModelingMidFooting[ix][jx][kx]	= FALSE;
				m_bStmTrussModelingMidFooting[ix][jx][kx]	= FALSE;

				m_strStmModelingMidFooting[ix][jx][kx] = _T("");
			}
		}
	}

	m_ixSolverStmTool = 2;
}

void CRcBridgeData::SerializeStm( CArchive &ar )
{
	// 1 : m_bApplyStmModelingFooting, m_bApplyStmModelingMidFooting - 한계상태로 데이터 변경
	// 2 : m_bApplyStmModelingFooting, m_bApplyStmModelingMidFooting - 한계상태 데이터 1개 추가
	// 3 : m_bApplyStmModelingCorner 우각부 배열 극단상황 추가
	// 4 : m_bApplyStmModelingCorner 우각부 배열 극단상황 2 추가, Stm 하중조합 문자열 추가
	// 5 : m_dMeshSizeWing stm 관련 데이터는 아니지만 Mesh size들이 여기 있어서 그냥 여기에 추가.
	// 6 : m_dMeshSizeWing 날개벽별로 입력받도록 수정
	long nFlagStm(6);
	
	if(ar.IsStoring())
	{
		ar << nFlagStm;

		ar << m_dMeshSizeArcCorner;
		ar << m_dMeshSizeBracket;
		ar << m_dMeshSizeBase;
		for(long iSE = iSTT; iSE <=iEND; ++iSE)
		{
			for(long iLR = iLEFT; iLR <=iRIGHT; ++iLR)
			{
				ar << m_dMeshSizeWing[iSE][iLR];
			}
		}

		for(long ix=0; ix<2; ix++)
		{
			ar << m_bApplyStmModelingBracket[ix];
			ar << m_bStmTrussModelingBracket[ix];

			ar << m_strStmModelingBracket[ix];

			for(long jx=0; jx<3; jx++)
			{
				for(long kx=0; kx<2; kx++)
				{
					for (long lx=0; lx<2; ++lx)
					{
						ar << m_bApplyStmModelingCorner[ix][jx][kx][lx];
						ar << m_bStmTrussModelingCorner[ix][jx][kx][lx];

						ar << m_strStmModelingCorner[ix][jx][kx][lx];
					}
				}
			}

			for(long jx=0; jx<3; jx++)
			{
				for(long kx=0; kx<5; kx++)
				{
					ar << m_bApplyStmModelingFooting[ix][jx][kx];
					ar << m_bStmTrussModelingFooting[ix][jx][kx];

					ar << m_strStmModelingFooting[ix][jx][kx];
				}
			}
		}

		for(long ix=0; ix<MAX_JIJUM_COUNT_RC; ix++)
		{
			for(long jx=0; jx<3; jx++)
			{
				for (long kx = 0; kx < 5; ++kx)
				{
					ar << m_bApplyStmModelingMidFooting[ix][jx][kx];
					ar << m_bStmTrussModelingMidFooting[ix][jx][kx];

					ar << m_strStmModelingMidFooting[ix][jx][kx];
				}
			}
		}

		ar << m_ixSolverStmTool;

		m_pStmSolverInput->Serialize(ar);
		StoreMapPtr(ar, m_mStmSolver);
	}
	else
	{
		ar >> nFlagStm;

		ar >> m_dMeshSizeArcCorner;
		ar >> m_dMeshSizeBracket;
		ar >> m_dMeshSizeBase;
		if(nFlagStm > 4)
		{
			ar >> m_dMeshSizeWing[iSTT][iLEFT];

			if(nFlagStm > 5)
			{
				ar >> m_dMeshSizeWing[iSTT][iRIGHT];
				ar >> m_dMeshSizeWing[iEND][iLEFT];
				ar >> m_dMeshSizeWing[iEND][iRIGHT];
			}
			else
			{
				double dMeshSize = m_dMeshSizeWing[iSTT][iLEFT];
				m_dMeshSizeWing[iSTT][iRIGHT] = dMeshSize;
				m_dMeshSizeWing[iEND][iLEFT] = dMeshSize;
				m_dMeshSizeWing[iEND][iRIGHT] = dMeshSize;
			}
		}

		for(long ix=0; ix<2; ix++)
		{
			ar >> m_bApplyStmModelingBracket[ix];
			ar >> m_bStmTrussModelingBracket[ix];

			if( nFlagStm > 3 )
				ar >> m_strStmModelingBracket[ix];

			for(long jx=0; jx<((nFlagStm<4) ? 2 : 3); jx++)
			{
				for(long kx=0; kx<2; kx++)
				{
					if( nFlagStm < 3 )
					{
						ar >> m_bApplyStmModelingCorner[ix][0][jx][kx];
						ar >> m_bStmTrussModelingCorner[ix][0][jx][kx];
					}
					else
					{
						for (long lx=0; lx<2; ++lx)
						{
							ar >> m_bApplyStmModelingCorner[ix][jx][kx][lx];
							ar >> m_bStmTrussModelingCorner[ix][jx][kx][lx];

							if( nFlagStm > 3 )
								ar >> m_strStmModelingCorner[ix][jx][kx][lx];
						}
					}
				}
			}

			if( nFlagStm > 1)
			{
				for(long jx=0; jx<3; jx++)
				{
					for (long kx = 0; kx < 5; kx++)
					{
						ar >> m_bApplyStmModelingFooting[ix][jx][kx];
						ar >> m_bStmTrussModelingFooting[ix][jx][kx];

						if( nFlagStm > 3)
							ar >> m_strStmModelingFooting[ix][jx][kx];
					}
				}
			}
			else if( nFlagStm == 0 )
			{
				for (long kx = 0; kx < 4; kx++)
				{
					ar >> m_bApplyStmModelingFooting[ix][0][kx];
					ar >> m_bStmTrussModelingFooting[ix][0][kx];
				}
			}
			else if( nFlagStm == 1 )
			{
				for(long jx=0; jx<3; jx++)
				{
					for (long kx = 0; kx < 4; kx++)
					{
						ar >> m_bApplyStmModelingFooting[ix][jx][kx];
						ar >> m_bStmTrussModelingFooting[ix][jx][kx];
					}
				}
			}
		}

		if( nFlagStm > 1)
		{
			for(long ix=0; ix<MAX_JIJUM_COUNT_RC; ix++)
			{
				for(long jx=0; jx<3; jx++)
				{
					for (long kx = 0; kx < 5; ++kx)
					{
						ar >> m_bApplyStmModelingMidFooting[ix][jx][kx];
						ar >> m_bStmTrussModelingMidFooting[ix][jx][kx];

						if( nFlagStm > 3 )
							ar >> m_strStmModelingMidFooting[ix][jx][kx];
					}
				}
			}
		}
		else if ( nFlagStm == 0 )
		{
			for(long ix=0; ix<MAX_JIJUM_COUNT_RC; ix++)
			{
				for(long jx=0; jx<4; jx++)
				{
					ar >> m_bApplyStmModelingMidFooting[ix][0][jx];
					ar >> m_bStmTrussModelingMidFooting[ix][0][jx];
				}
			}
		}
		else if( nFlagStm == 1 )
		{
			for(long ix=0; ix<MAX_JIJUM_COUNT_RC; ix++)
			{
				for(long jx=0; jx<3; jx++)
				{
					for (long kx = 0; kx < 4; ++kx)
					{
						ar >> m_bApplyStmModelingMidFooting[ix][jx][kx];
						ar >> m_bStmTrussModelingMidFooting[ix][jx][kx];
					}
				}
			}
		}

		ar >> m_ixSolverStmTool;

		m_pStmSolverInput->Serialize(ar);
		LoadMapPtr(ar, m_mStmSolver);
	}
}

void CRcBridgeData::DeleteStmData()
{
	delete m_pStmSolverInput;
	ClearMapPtr(m_mStmSolver);
	ClearMapPtr(m_mStmSolverOut);
}

HGBOOL CRcBridgeData::IsApplyStmBracket()
{
	HGBOOL bBracket= FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
	{
		if (m_bApplyStmModelingBracket[ix] == TRUE)
		{
			bBracket = TRUE;
			break;
		}
	}

	return bBracket;
}

HGBOOL CRcBridgeData::IsApplyStmCorner()
{
	HGBOOL bArcCorner = FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
	{
		for (HGINT32 jx = 0; jx < 3; ++jx)
		{
			for (HGINT32 kx = 0; kx < 2; ++kx)
			{
				for (HGINT32 lx=0; lx<2; ++lx)
				{
					if (m_bApplyStmModelingCorner[ix][m_pARcBridgeDataStd->IsLsdDesign() ? jx : 0][kx][lx] == TRUE)
					{
						bArcCorner = TRUE;
						break;
					}
				}
			}
		}
	}

	return bArcCorner;
}

HGBOOL CRcBridgeData::IsApplyStmFooting()
{
	HGBOOL bSTFoot = FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
	{
		for (HGINT32 jx = 0; jx < 3; ++jx)
		{
			for (HGINT32 kx = 0; kx < 5; ++kx)
			{
				if (m_bApplyStmModelingFooting[ix][jx][kx] == TRUE)
				{
					bSTFoot = TRUE;
					break;
				}
			}
		}
	}

	return bSTFoot;
}

HGBOOL CRcBridgeData::IsApplyStmMidFooting()
{
	HGBOOL bMidFoot = FALSE;

	HGINT32 nInWallSize = GetCountInWall();
	for (HGINT32 ix = 0; ix < nInWallSize; ++ix)
	{
		for (HGINT32 jx = 0; jx < 3; ++jx)
		{
			for (HGINT32 kx = 0; kx < 5; ++kx)
			{
				if (m_bApplyStmModelingMidFooting[ix][jx][kx] == TRUE)
				{
					bMidFoot = TRUE;
					break;
				}
			}
		}
	}

	return bMidFoot;
}

void CRcBridgeData::InitLsdData()
{
	m_plsdManager = new CLsdManagerBridge(static_cast<CRcBridgeRebar*>(this));
	m_plsdRebar = new CLsdRebarManager;
	m_plsdEnvRating = new CLsdEnviromentRating;

	m_plsdMngForCantilever = new CLsdManagerBridge(static_cast<CRcBridgeRebar*>(this), TRUE);
	
	m_pCheckCantilever = new CCheckCantileverData;
	
	m_bEtaCombinEach = FALSE;
	for(HGINT32 ix=0; ix<ePartCount; ++ix)
	{
		m_Fck[ix]	= 240;	
		m_Fy[ix]	= 3000;
		m_Fvy[ix]	= 3000;
		m_Ec[ix]	= 0;
	}
	
	for (HGINT32 ix = 0; ix < eLsdStmCount; ++ix )
	{
		HGDOUBLE dFck = 0.;

		switch(ix)
		{
		case eLsdStmCornerUpper:		// 우각부 - 상부
			dFck = m_Fck[ePartUpperSlab];
			break;
		case eLsdStmCornerLower:		// 우각부 - 하부
			dFck = m_Fck[ePartLowerSlab];
			break;
		case eLsdStmBracketStt:		// 브라켓 - 시점부
			dFck = m_Fck[ePartWingWallStt];
			break;
		case eLsdStmBracketEnd:		// 브라켓 - 종점부
			dFck = m_Fck[ePartWingWallEnd];
			break;
		case eLsdStmFootStt:			// 기초 - 시점
			dFck = m_Fck[ePartFootStt];
			break;
		case eLsdStmFootEnd:			// 기초 - 종점
			dFck = m_Fck[ePartFootEnd];
			break;
		case eLsdStmFootMid:			// 기초 - 중간지점
			dFck = m_Fck[ePartInFoot];
			break;
		}
		
//		m_Fctk[ix] = GetValueFctk(GetValueUnitChange(dFck, UNIT_CGS_KGF_CM2, GetStringUnitByType(UNIT_CGS_KGF_CM2, 1)));
//		m_Fctk[ix] = GetValueUnitChange(m_Fctk[ix], GetStringUnitByType(UNIT_CGS_KGF_CM2, 1), UNIT_CGS_KGF_CM2);
	}

	m_bCheckCorner = FALSE;			// 우각부 검토

	m_dDepthApplySubSid		= 10;					// 지점침하 적용값
	m_nSelectKindCoefficientKo	= 0;			// 정지토압계수 선택
	m_nSelectInteriorWaterPress	= 0;			// 내부수압 선택( EV, BP, WA )
	m_dHeightInteriorWaterPress	= 0;			// 내부수압 적용높이
	m_dTemperatureChangeLoadGap	= 5;			// 온도하중 온도변화차

	// 온도경사하중 T값
	m_dTemperatureSlopeT[0] = 23;			// T-1
	m_dTemperatureSlopeT[1] = 6;			// T-2
	m_dTemperatureSlopeT[2] = 3;			// T-3
	m_nApplyPavement = 0;					// 온도경사하중 하부의 온도가 높을때 옵션

	m_nLoadEffeciveW = 0;					// 활하중 - 유효폭
	m_nLoadEquiStrip = 0;					// 활하중 - 등가스트립 폭에 사각고려
	m_bLoadMultiRoadSurConfficient = FALSE;	// 활하중 - 유효폭 계산시 다차로 재하계수
	m_nLoadStandardLane = 0;				// 활하중 - 표준차로하중
	
	m_bMaterialDetail = FALSE;				// 재료 물성치 상세 입력
	m_dRateDryAndShrinkage = 0.00015;		// 건조수축율
	m_dCoefficientExpansion = 0.000010;		// 가동받침의 이동량 산정시
	m_bIsDBar_ForDowelBar = TRUE;			// 다웰바 강종
	
	m_nTypeDesingEQ = 1;					// 내진해석 방법
	m_nDegreeEarthQuake = 0;				// 내진등급
	m_nRegionEarthQuake = 0;				// 지진구역
	m_dCoefficientGround = 1.0;				// 지반계수
	m_nTypeCoefficientGround = 1;
	m_dRateDAMP_EQ_CD = 5;
	m_gValue = 9.810;						// 중력가속도
	m_nTypeRigidFootingEQ = 0;				// 내진해석시 기초부 모델링 방
	m_bApplyRigidWallEQRotate = FALSE;		// 벽체강성 적용시 회전강성
	m_nTypeJointPosFootingEQ = 0;			// 내진해석시 기초부 절점 
	m_nTypePropertyPosFootingEQ = 0;		// 내진해석시 기초 중앙으로
	m_nTypeSoilPressureEQ = 0;				// 내진해석시 수평토압의 재하
	m_nTypeUpperWeightEQ = 0;				// 내진해석시 상부중량 설정

	m_bCheckSupportSlabArea = TRUE;
	m_bCheckSupportWallArea = TRUE;
	m_bCheckSupportFootArea = TRUE;
	m_bCheckSupportWingWallArea = TRUE;
	m_bCheckSupportApsArea = TRUE;

	m_dThetaShearDesign = 0;				// 전단검토시 Θ : 0 = 자동계산
	m_bCheckDeltaTR = TRUE;
	m_dElasticModularRatioUseCheck = 0;		// 사용성 검토시 유효탄성계수비 : 0 = 탄성계수비
	m_nSelectUsebilityCheck = 1;			// 사용성 검토시 균열검토
	m_nTypeCrackWidthCondition = 0;
	m_nSelectExposureCoverCheck = 1;				// 설계피복 산정
	m_nSelectPminColumn = 0;				// 기둥 최소 철근비
	
	m_nApplyCalcShearDistSlab = 0;			// 슬래브 전단검토시 위험단면 위치
	m_nApplyCalcShearDistWall = 0;			// 벽체 전단검토시 위험단면 위치
	m_nApplyCalcShearDistFrontPile	= 1;	// 앞굽 검토시 위험단면 위치(말뚝기초)
	m_nApplyCalcShearDistBackPile	= 1;	// 뒷굽 검토시 위험단면 위치(말뚝기초)

	m_bApplyT1CalcWingWall = TRUE;


	InitAggregateAndSlump();
}

void CRcBridgeData::DeleteLsdData()
{
	SAFE_DELETE(m_plsdManager);
	SAFE_DELETE(m_plsdRebar);
	SAFE_DELETE(m_plsdEnvRating);
	SAFE_DELETE(m_plsdMngForCantilever);
	ClearVectorPtr(m_vpUserLoadSet[0]);
	ClearVectorPtr(m_vpUserLoadSet[1]);
	ClearVectorPtr(m_vpUserLoadSet[2]);
	ClearVectorPtr(m_vpUserLoadSet[3]);
	ClearVectorPtr(m_vpUserLoadSet[4]);

	SAFE_DELETE(m_pCheckCantilever);
}

void CRcBridgeData::SerializeLsd( CArchive &ar )
{
	// 1 : 하중입력 추가
	// 2 : m_bMaterialDetail, m_dRateDryAndShrinkage, m_dCoefficientExpansion, m_bIsDBar_ForDowelBar, m_nTypeDesingEQ, m_nDegreeEarthQuake, m_nRegionEarthQuake, m_dCoefficientGround
	//		m_gValue, m_nTypeRigidFootingEQ, m_bApplyRigidWallEQRotate, m_nTypeJointPosFootingEQ, m_nTypePropertyPosFootingEQ, m_nTypeSoilPressureEQ, m_nTypeUpperWeightEQ 추가
	// 3 : 	m_nLoadEffeciveW, m_nLoadEquiStrip, m_bLoadMultiRoadSurConfficient
	// 4 : m_nLoadStandardLane 추가
	// 5 : m_bCheckSupportSlabArea, m_bCheckSupportWallArea, m_bCheckSupportFootArea, m_bCheckSupportWingWallArea, m_bCheckSupportApsArea 추가	
	// 6 : m_bAggregate_SlumpDetail, m_dAggregate, m_dAggregate_LeanLSD, m_dAggregate_MassLSD, m_dAggregate_HyuntaLSD, m_dAggregate_EtcLSD, 
	//	   m_dSlump, m_dSlump_LeanLSD, m_dSlump_MassLSD, m_dSlump_HyuntaLSD, m_dSlump_EtcLSD 추가
	// 7 : m_Fctk - Stm 콘크리트 인장강도, m_bCheckCorner - Stm 우각부 검토 추가
	// 8 : m_dThetaShearDesign, m_dElasticModularRatioUseCheck, m_nSelectUsebilityCheck, m_nSelectPminColumn, 
	//	   m_nApplyCalcShearDistSlab, m_nApplyCalcShearDistWall, m_nApplyCalcShearDistFrontPile, m_nApplyCalcShearDistBackPile 추가
	// 9 : 온도경사TG 변수 추가 : m_dTemperatureSlopeT[3], m_nApplyPavement
	// 10 : m_vpUserLoadSet
	// 11 : 캔틸레버검토 - 작용하중선택 m_pWorkingLoad 
	// 12 : 캔틸레버검토 데이터 구조 변경  m_pCheckCantilever
	// 13 : m_plsdMngForCantilever
	// 14 : m_nSelectExposureCoverCheck
	// 15 : m_bCheckDeltaTR
	// 16 : m_nTypeCrackWidthCondition
	// 17 : m_bEtaCombinEach
	// 18 : m_vpUserLoadSet 배열갯수 2 -> 3
	// 19 : 피로한계상태 조합 삭제
	// 20 : m_nTypeCoefficientGround
	// 21 : m_vpUserLoadSet[] 배열갯수 늘림 3 -> 5;
	// 22 : m_dRateDAMP_EQ_CD
	
	long nFlag(22);
	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_bEtaCombinEach;
		for(HGINT32 ix=0; ix<ePartCount; ++ix)
		{
			ar << m_Fck[ix];	
			ar << m_Fy[ix];
			ar << m_Fvy[ix];
			ar << m_Ec[ix];
		}
		
		ar << m_dDepthApplySubSid;
		ar << m_nSelectKindCoefficientKo;
		ar << m_nSelectInteriorWaterPress;
		ar << m_dHeightInteriorWaterPress;
		ar << m_dTemperatureChangeLoadGap;
		
		for (HGINT32 ix=0; ix<3; ++ix)
		{
			ar << m_dTemperatureSlopeT[ix];
		}
		ar << m_nApplyPavement;

		ar << m_bMaterialDetail;
		ar << m_dRateDryAndShrinkage;
		ar << m_dCoefficientExpansion;
		ar << m_bIsDBar_ForDowelBar;
		ar << m_nTypeDesingEQ;
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

		ar << m_nLoadEffeciveW;
		ar << m_nLoadEquiStrip;
		ar << m_bLoadMultiRoadSurConfficient;
		ar << m_nLoadStandardLane;

		ar << m_bCheckSupportSlabArea;
		ar << m_bCheckSupportWallArea;
		ar << m_bCheckSupportFootArea;
		ar << m_bCheckSupportWingWallArea;
		ar << m_bCheckSupportApsArea;

		for (HGINT32 ix = 0; ix < eLsdStmCount; ++ix )
		{
			ar << m_Fctk[ix];
		}

		ar << m_bCheckCorner;

		ar << m_bAggregate_SlumpDetail;
		StoreArray(ar, m_dAggregate, ePartCount);
		ar << m_dAggregate_LeanLSD;
		ar << m_dAggregate_MassLSD;
		ar << m_dAggregate_HyuntaLSD;
		ar << m_dAggregate_EtcLSD;

		StoreArray(ar, m_dSlump, ePartCount);
		ar << m_dSlump_LeanLSD;
		ar << m_dSlump_MassLSD;
		ar << m_dSlump_HyuntaLSD;
		ar << m_dSlump_EtcLSD;

		ar << m_dThetaShearDesign;
		ar << m_bCheckDeltaTR;
		ar << m_dElasticModularRatioUseCheck;
		ar << m_nSelectUsebilityCheck;
		ar << m_nTypeCrackWidthCondition;
		ar << m_nSelectExposureCoverCheck;
		ar << m_nSelectPminColumn;

		ar << m_nApplyCalcShearDistSlab;
		ar << m_nApplyCalcShearDistWall;
		ar << m_nApplyCalcShearDistFrontPile;
		ar << m_nApplyCalcShearDistBackPile;

		StoreVectorPtr(ar, m_vpUserLoadSet[0]);
		StoreVectorPtr(ar, m_vpUserLoadSet[1]);
		StoreVectorPtr(ar, m_vpUserLoadSet[2]);
		StoreVectorPtr(ar, m_vpUserLoadSet[3]);
		StoreVectorPtr(ar, m_vpUserLoadSet[4]);

		m_pCheckCantilever->Serialize(ar);
	}
	else
	{
		ar >> nFlag;

		if(nFlag > 16)
			ar >> m_bEtaCombinEach;

		for(HGINT32 ix=0; ix<ePartCount; ++ix)
		{
			ar >> m_Fck[ix];	
			ar >> m_Fy[ix];
			ar >> m_Fvy[ix];
			ar >> m_Ec[ix];
		}

		if(nFlag > 0)
		{
			ar >> m_dDepthApplySubSid;
			ar >> m_nSelectKindCoefficientKo;
			ar >> m_nSelectInteriorWaterPress;
			ar >> m_dHeightInteriorWaterPress;
			ar >> m_dTemperatureChangeLoadGap;
		}

		if( nFlag > 8)
		{
			for (HGINT32 ix=0; ix<3; ++ix)
			{
				ar >> m_dTemperatureSlopeT[ix];
			}
			ar >> m_nApplyPavement;
		}

		if (nFlag > 1)
		{
			ar >> m_bMaterialDetail;
			ar >> m_dRateDryAndShrinkage;
			ar >> m_dCoefficientExpansion;
			ar >> m_bIsDBar_ForDowelBar;
			ar >> m_nTypeDesingEQ;
			ar >> m_nDegreeEarthQuake;
			ar >> m_nRegionEarthQuake;
			ar >> m_dCoefficientGround;
			if(nFlag > 19)
				ar >> m_nTypeCoefficientGround;
			if(nFlag > 21)
				ar >> m_dRateDAMP_EQ_CD;
			ar >> m_gValue;
			ar >> m_nTypeRigidFootingEQ;
			ar >> m_bApplyRigidWallEQRotate;
			ar >> m_nTypeJointPosFootingEQ;
			ar >> m_nTypePropertyPosFootingEQ;
			ar >> m_nTypeSoilPressureEQ;
			ar >> m_nTypeUpperWeightEQ;
		}

		if( nFlag > 2 )
		{
			ar >> m_nLoadEffeciveW;
			ar >> m_nLoadEquiStrip;
			ar >> m_bLoadMultiRoadSurConfficient;
		}

		if (nFlag > 3)
		{
			ar >> m_nLoadStandardLane;
		}

		if (nFlag > 4)
		{
			ar >> m_bCheckSupportSlabArea;
			ar >> m_bCheckSupportWallArea;
			ar >> m_bCheckSupportFootArea;
			ar >> m_bCheckSupportWingWallArea;
			ar >> m_bCheckSupportApsArea;
		}

		if( nFlag > 6)
		{
			for (HGINT32 ix = 0; ix < eLsdStmCount; ++ix )
			{
				ar >> m_Fctk[ix];
			}

			ar >> m_bCheckCorner;
		}

		if (nFlag > 5)
		{
			ar >> m_bAggregate_SlumpDetail;
			LoadArray(ar, m_dAggregate, ePartCount);
			ar >> m_dAggregate_LeanLSD;
			ar >> m_dAggregate_MassLSD;
			ar >> m_dAggregate_HyuntaLSD;
			ar >> m_dAggregate_EtcLSD;

			LoadArray(ar, m_dSlump, ePartCount);
			ar >> m_dSlump_LeanLSD;
			ar >> m_dSlump_MassLSD;
			ar >> m_dSlump_HyuntaLSD;
			ar >> m_dSlump_EtcLSD;
		}

		if (nFlag > 7)
		{
			ar >> m_dThetaShearDesign;
			if(nFlag > 14)
			{
				ar >> m_bCheckDeltaTR;
			}
			ar >> m_dElasticModularRatioUseCheck;
			ar >> m_nSelectUsebilityCheck;
			if(nFlag > 15)
			{
				ar >> m_nTypeCrackWidthCondition;
			}
			else if(nFlag < 15)
			{
				// RCGHOS-428에서 옵션처리(flag)를 하지않아서 가장 가까운 날짜를 기준으로 적용
				// m_nTypeCrackWidthCondition : 2/16
				// m_bCheckDeltaTR : 2/3
				m_nTypeCrackWidthCondition = 1;
			}
			if(nFlag > 13)
			{
				ar >> m_nSelectExposureCoverCheck;
			}
			ar >> m_nSelectPminColumn;

			ar >> m_nApplyCalcShearDistSlab;
			ar >> m_nApplyCalcShearDistWall;
			ar >> m_nApplyCalcShearDistFrontPile;
			ar >> m_nApplyCalcShearDistBackPile;
		}
		
		if(nFlag > 9)
		{
			LoadVectorPtr(ar, m_vpUserLoadSet[0]);
			LoadVectorPtr(ar, m_vpUserLoadSet[1]);
			if(nFlag > 17)
				LoadVectorPtr(ar, m_vpUserLoadSet[2]);
			else
			{
				CopyVectorPtr(m_vpUserLoadSet[2], m_vpUserLoadSet[1]);
			}
			if(nFlag > 20)
			{
				LoadVectorPtr(ar, m_vpUserLoadSet[3]);
				LoadVectorPtr(ar, m_vpUserLoadSet[4]);
			}
		}

		if( nFlag == 11 )
		{
			std::vector<CCheckCantileverData*> pTemp;
			LoadVectorPtr(ar, pTemp);
		}
		if( nFlag > 11 )	
			m_pCheckCantilever->Serialize(ar);
	}

	m_plsdManager->Serialize(ar);
	m_plsdRebar->Serialize(ar);
	m_plsdEnvRating->Serialize(ar);

	if(ar.IsStoring() || nFlag > 12)
	{
		m_plsdMngForCantilever->Serialize(ar);
	}

	if (ar.IsLoading())
	{
		if (nFlag < 6)
		{
			for (HGINT32 ix = 0; ix < ePartCount; ++ix)
			{
				m_dAggregate[ix] = GetValueAggregate((EPartOfBridge)ix, TRUE);
				m_dSlump[ix] = GetValueSlump((EPartOfBridge)ix, TRUE);
			}

			m_dAggregate_LeanLSD = m_dAggregate_Lean;
			m_dAggregate_MassLSD = m_dAggregate_Mass;
			m_dAggregate_HyuntaLSD = m_dAggregate_Hyunta;
			m_dAggregate_EtcLSD = m_dAggregate_Etc;
			
			m_dSlump_LeanLSD = m_dSlump_Lean;
			m_dSlump_MassLSD = m_dSlump_Mass;
			m_dSlump_HyuntaLSD = m_dSlump_Hyunta;
			m_dSlump_EtcLSD = m_dSlump_Etc;
		}
		if(nFlag < 19)
		{
			// (ARCBRIDGE-3232) 피로 검토 조합 제거. 파일을 읽었을때 남아있다면 제거 한다.
			// (*vecLsdLoadFactor.find(eLSD_COMBINE_STRESS)).second->SetUse(TRUE);
			m_plsdManager->DeleteLoadData(eLSD_COMBINE_STRESS);
			m_plsdManager->SetDataDefaultLimitCombineSet(eLSD_COMBINATION_FATIGUE, eLSD_LOAD_LOADING_DIR_BOTH, FALSE);
		}
	}
}

BOOL CRcBridgeData::IsUsePart(EPartOfBridge ePart, long nType)
{
	BOOL bUse(TRUE);

	if(nType == 0)	// 하중수정계수 적용
	{
		if(ePart >= ePartOutWallEnd && ePart <= ePartApsEnd)
			bUse = FALSE;
	}
	else if(nType == 1) // Env조합
	{
		bUse = (ePart == ePartOutWallStt || ePart == ePartUpperSlab || ePart == ePartLowerSlab || ePart == ePartInWall)? TRUE : FALSE;
	}
	else if(nType == 2) // Eta 조합
	{
		bUse = (ePart == ePartOutWallStt || ePart == ePartInWall || ePart == ePartFootStt || ePart == ePartInFoot)? TRUE : FALSE;
	}
	
	return bUse;
}

CString CRcBridgeData::GetStringPartMerge(EPartOfBridge ePart)
{
	CString szPart(_T(""));

	switch (ePart)
	{
	case ePartUpperSlab:
		szPart = _T("상부슬래브");	break;
	case ePartLowerSlab:
		szPart = _T("하부슬래브");	break;
	case ePartOutWallStt:
	case ePartOutWallEnd:
		szPart = _T("벽체");	break;
	case ePartFootStt:
	case ePartFootEnd:
		szPart = _T("기초");	break;
	case ePartWingWallStt:
	case ePartWingWallEnd:
		szPart = _T("날개벽");	break;
	case ePartApsStt:
	case ePartApsEnd:
		szPart = _T("접속슬래브");	break;
	case ePartInWall:
		szPart = _T("중간벽체");	break;
	case ePartInFoot:
		szPart = _T("중간기초");	break;
	default:
		break;
	}

	return szPart;
}

CString CRcBridgeData::GetStringPart( EPartOfBridge ePart )
{
	CString szPart(_T(""));

	switch (ePart)
	{
	case ePartUpperSlab:
		szPart = _T("상부슬래브");	break;
	case ePartLowerSlab:
		szPart = _T("하부슬래브");	break;
	case ePartOutWallStt:
		szPart = _T("시점부:벽체");	break;
	case ePartFootStt:
		szPart = _T("시점부:기초");	break;
	case ePartWingWallStt:
		szPart = _T("시점부:날개벽");	break;
	case ePartApsStt:
		szPart = _T("시점부:접속슬래브");	break;
	case ePartOutWallEnd:
		szPart = _T("종점부:벽체");	break;
	case ePartFootEnd:
		szPart = _T("종점부:기초");	break;
	case ePartWingWallEnd:
		szPart = _T("종점부:날개벽");	break;
	case ePartApsEnd:
		szPart = _T("종점부:접속슬래브");	break;
	case ePartInWall:
		szPart = _T("중간벽체");	break;
	case ePartInFoot:
		szPart = _T("중간기초");	break;
	default:
		break;
	}

	return szPart;
}

CString CRcBridgeData::GetStringPart(EPartOfBridge ePart, HGBOOL bUpperOrFront)
{
	CString szPart(GetStringPart(ePart));

	switch (ePart)
	{
	case ePartUpperSlab:
	case ePartLowerSlab:
	case ePartFootStt:
	case ePartFootEnd:
	case ePartInFoot:
	case ePartApsStt:
	case ePartApsEnd:
		if (bUpperOrFront)
			szPart += _T("\n상면");
		else
			szPart += _T("\n하면");
		break;
	case ePartOutWallStt:
	case ePartOutWallEnd:
	case ePartWingWallStt:
	case ePartWingWallEnd:
		if (bUpperOrFront)
			szPart += _T("\n전면");
		else
			szPart += _T("\n배면");
		break;
	case ePartInWall:
		szPart.Replace(_T("벽체"), _T(""));
		szPart += _T("\n벽체");
		break;
	default:
		break;
	}

	return szPart;
}

long CRcBridgeData::GetLsdMatrialIndex( EPartOfBridge ePart, HGBOOL bUpperOrFront )
{
	long nIdex(-1);
	CString sType = GetStringPart(ePart, bUpperOrFront);
	CLsdExposureRatingCtl *pExposure = &m_plsdEnvRating->GetLsdExposureRatingCtl();
	long nSizeMat = pExposure->GetMaterialSize();

	for (long ix=0; ix < nSizeMat; ++ix)
	{
		CString strMaterial = pExposure->GetMaterialTitle(ix);	

		if(strMaterial== sType)
		{
			nIdex = ix;
			break;
		}
	}

	return nIdex;
}

long CRcBridgeData::GetLsdMatrialTypePos( EPartOfBridge ePart, HGBOOL bUpperOrFront )
{
	long nType = -1;
	
	std::vector<SDesignMaterialTuple> vecDesignMaterialTuple;
	m_plsdEnvRating->GetMaterialDesignRatingTuple(vecDesignMaterialTuple, TRUE);

	long nSizeMat = vecDesignMaterialTuple.size();
	long nIdex = GetLsdMatrialIndex(ePart, bUpperOrFront);
	if(nIdex < 0 || nIdex >= nSizeMat)
		return -1;

	nType = vecDesignMaterialTuple[nIdex].third;

	return nType;
}

double CRcBridgeData::GetCrackLimitWidth_LSD( EPartOfBridge ePart, HGBOOL bUpperOrFront, CString &sKind, CString &sDesingLevel )
{
	double dCrackLT = 0;

	CLsdExposureRatingCtl *pExposureCtl = &(m_plsdEnvRating->GetLsdExposureRatingCtl());
	CLsdDesignRatingCtl *pDesignRatingCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
	HGINT32 iRatingSize = pDesignRatingCtl->GetRatingSize();

	if (iRatingSize <= 0)
		return dCrackLT;

	const CString strPart = GetStringPart(ePart, bUpperOrFront);
	HGINT32 iIndex = pExposureCtl->GetIndexByMaterialTitle(strPart);
	
	for (HGINT32 ix = eLSD_COMBINE_SERVICE_I; ix < eLSD_COMBINE_TOTAL_COUNT-1; ++ix)
	{
		ELSDCombKind eKind = static_cast<ELSDCombKind>(ix);

		HGINT32 iGrade = pDesignRatingCtl->GetMaterialGrade(iIndex);

		if(iGrade == 0)
		{
			dCrackLT = 0;
			sDesingLevel = _T("A");
			break;
		}
		else if(iGrade == 1)
		{
			if(eKind == eLSD_COMBINE_SERVICE_I) 
			{
				dCrackLT = pDesignRatingCtl->GetLimitCrackWidth(iGrade);
				sKind = _T("사용하중조합-Ⅰ");
				sDesingLevel = _T("B");
				break;
			}
		}
		else if(iGrade == 2 || iGrade == 3)
		{
			if(eKind == eLSD_COMBINE_SERVICE_III || eKind == eLSD_COMBINE_SERVICE_IV)
			{
				dCrackLT = pDesignRatingCtl->GetLimitCrackWidth(iGrade);
				sKind = _T("사용하중조합-Ⅲ/Ⅳ");
				sDesingLevel = iGrade == 2? _T("C") : _T("D");
				break;
			}
		}
		else
		{
			if(eKind == eLSD_COMBINE_SERVICE_V )
			{
				dCrackLT = pDesignRatingCtl->GetLimitCrackWidth(iGrade);
				sKind = _T("사용하중조합-Ⅴ");
				sDesingLevel = _T("E");
				break;
			}				
		}
	}

	return dCrackLT;
}

BOOL CRcBridgeData::IsCheckPosEtaUseLoad( EPartOfBridge ePart )
{
	HGBOOL bOk = FALSE;

	CLsdExposureRatingCtl *pExposureCtl = &(m_plsdEnvRating->GetLsdExposureRatingCtl());
	CLsdDesignRatingCtl *pDesignRatingCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
	HGINT32 iRatingSize = pDesignRatingCtl->GetRatingSize();

	if (iRatingSize <= 0)
		return bOk;

	///사용성 검토에서 사용해야할 종류
	//모든 케이스를 배면 기준으로 하자!
	const CString strPart = GetStringPart(ePart, FALSE);
	HGINT32 iIndex = pExposureCtl->GetIndexByMaterialTitle(strPart);

	for (HGINT32 ix=eLSD_COMBINE_SERVICE_I; ix < eLSD_COMBINE_TOTAL_COUNT; ++ix)
	{
		ELSDCombKind eKind = static_cast<ELSDCombKind>(ix);
		
		HGINT32 iGrade = pDesignRatingCtl->GetMaterialGrade(iIndex);

		if(iGrade == 0)
		{
			return FALSE;
		}
		else if(iGrade == 1)
		{
			if(eKind == eLSD_COMBINE_SERVICE_I) 
			{
				BOOL bUseCheck = IsUseLsdLoadFactor(eLSD_COMBINE_SERVICE_I);
				if(bUseCheck)
				{
					if(IsDesignRatingSelectAbutLoadCaseKind(eKind, ePart))
						bOk  = TRUE;
				}
				break;
			}
		}
		else if(iGrade == 2 || iGrade == 3)
		{
			if(eKind == eLSD_COMBINE_SERVICE_III || eKind == eLSD_COMBINE_SERVICE_IV)
			{
				BOOL bSer3 = FALSE, bSer4 = FALSE;

				BOOL bUseCheck1 = IsUseLsdLoadFactor(eLSD_COMBINE_SERVICE_III);
				if(bUseCheck1)
				{
					if(IsDesignRatingSelectAbutLoadCaseKind(eLSD_COMBINE_SERVICE_III, ePart))
						bSer3  = TRUE;
				}


				BOOL bUseCheck2 = IsUseLsdLoadFactor(eLSD_COMBINE_SERVICE_IV);
				if(bUseCheck2)
				{
					if(IsDesignRatingSelectAbutLoadCaseKind(eLSD_COMBINE_SERVICE_IV, ePart))
						bSer4  = TRUE;
				}

				if(bSer3 || bSer4)
					bOk = TRUE;

				break;
			}
		}
		else
		{
			if(eKind == eLSD_COMBINE_SERVICE_V )
			{
				BOOL bUseCheck = IsUseLsdLoadFactor(eLSD_COMBINE_SERVICE_V);
				if(bUseCheck)
				{
					if(IsDesignRatingSelectAbutLoadCaseKind(eKind, ePart))
						bOk  = TRUE;
				}
				break;
			}				
		}
	}


	return bOk;
}

BOOL CRcBridgeData::IsUseLsdLoadFactor(long nIdexCombin)
{
	BOOL bCheckUse = FALSE;

	ELSDCombKind eKind = static_cast<ELSDCombKind>(nIdexCombin);

	CLsdLoad *pLoad = m_plsdManager->GetLoad(eKind);
	if(pLoad == NULL)
		return FALSE;

	if(pLoad->GetUse() == TRUE)
		bCheckUse = TRUE;

	return bCheckUse;
}

//사용성 검토에서 에타집계표 집계시 해당 단면의 작용부재력중 ELSDCombKind 의 어느것을 사용하는지 검토
BOOL CRcBridgeData::IsDesignRatingSelectAbutLoadCaseKind(ELSDCombKind eKind, EPartOfBridge ePart)
{
	BOOL bSelect = FALSE;

	CLsdExposureRatingCtl *pExposureCtl = &(m_plsdEnvRating->GetLsdExposureRatingCtl());
	CLsdDesignRatingCtl *pDesignRatingCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
	HGINT32 iRatingSize = pDesignRatingCtl->GetRatingSize();

	if (iRatingSize <= 0)
		return bSelect;

	// 	CLsdExposureRatingCtl * pExposureRatingCtl = &(pAbut->m_plsdEnvRating->GetLsdExposureRatingCtl());
	// 	long nMaterialSize = pExposureRatingCtl->GetMaterialSize();

	///사용성 검토에서 사용해야할 종류
	//모든 케이스를 배면 기준으로 하자!
	const CString strPart = GetStringPart(ePart, FALSE);
	HGINT32 iIndex = pExposureCtl->GetIndexByMaterialTitle(strPart);

	HGINT32 iGrade = pDesignRatingCtl->GetMaterialGrade(iIndex);

	if(iGrade == 0)
	{
		bSelect = FALSE;
	}
	else if(iGrade == 1)
	{
		if(eKind == eLSD_COMBINE_SERVICE_I) 
			bSelect = TRUE;
		else
			bSelect = FALSE;
	}
	else if(iGrade == 2 || iGrade == 3)
	{
		if(eKind == eLSD_COMBINE_SERVICE_III || eKind == eLSD_COMBINE_SERVICE_IV)
			bSelect = TRUE;
		else 
			bSelect = FALSE;
	}
	else
	{
		if(eKind == eLSD_COMBINE_SERVICE_V)
			bSelect = TRUE;
		else 
			bSelect = FALSE;
	}

	return bSelect;
}

BOOL CRcBridgeData::IsUseSelectLoadCaseWingOrApsMatchDesignRating( CString sGrade, EPartOfBridge ePart )
{
	HGBOOL bMatch = FALSE;

	if(sGrade == _T("A"))
		return bMatch;

	//구하는 방법(뭔가 문제가 있는 방법이라 설명씀->그리드 자체의 오류가 있음)
	//현재 Manager 한계상태 하중 조합에서 라디오 버튼을 체크를 찾으려면
	//해당 조합의 맨마지막 Loadcase가 해당 Index를 가지고 있다 
	//1. 사용하중의 맨 끝의 Index를 찾는다
	//2. index가 몇번쨰 LoadCase인지 찾는다
	//3. 해당 Loadcase가 무슨 하중조합이며 sGrade와 맞는 하중조합인지 확인

	if(ePart == ePartApsStt || ePart == ePartApsEnd || ePart == ePartWingWallStt || ePart == ePartWingWallEnd)
	{
		long nCheck(-1);
		long nExtra = (ePart== ePartWingWallStt || ePart== ePartWingWallEnd)? LSD_DESIGN_WINGWALL_SERVICE_CHECK : LSD_DESIGN_APPROACH_SERVICE_CHECK;
		// 사용한계상태 날개벽 이나 접속슬래브 일때 추가된 항목이 초기화되지 않은경우 처리
		for(long nComboKind = eLSD_COMBINE_SERVICE_I; nComboKind <= eLSD_COMBINE_SERVICE_V; nComboKind++)
		{
			CLsdLoadCase *pLoadCase = m_plsdManager->GetLsdLoadCase((ELSDCombKind)nComboKind, eLSD_LOADCASE_DIR_BRI, 0);
			if(pLoadCase && pLoadCase->GetSizeExtraValueArray() <= LSD_DESIGN_APPROACH_SERVICE_CHECK)
			{
				if(nExtra == LSD_DESIGN_WINGWALL_SERVICE_CHECK) nExtra = LSD_DESIGN_WINGWALL;
				if(nExtra == LSD_DESIGN_APPROACH_SERVICE_CHECK) nExtra = LSD_DESIGN_APPROACH;
				break;
			}
		}

		ELSDLoadGroupKind eGroupKind = eLSD_COMBINATION_SERVICE;

		//1 맨 끝의 라디오 버튼의 Index를 구한다.
		for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
		{			
			if(m_plsdManager->GetLoadGroupKindByCombKind(static_cast<ELSDCombKind>(nL)) != eGroupKind)
				continue;

			std::vector<CLsdLoadCase *> vecLoadCase = m_plsdManager->GetLsdLoadCaseArray((ELSDCombKind)nL, eLSD_LOADCASE_DIR_BRI);

			long nSizeCombo = vecLoadCase.size();
			for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
			{
				CLsdLoadCase *pLoadCase = vecLoadCase[nCombo];

				if(eGroupKind >= eLSD_COMBINE_TOTAL_COUNT) continue;

				if(pLoadCase->GetSizeExtraValueArray() < nExtra) continue;

				nCheck = pLoadCase->GetExtraValueArray()[nExtra];
			}
		}

		//2 Index가 몇번째 LoadCase인지 찾는다.
		long nFindInsex = 0;
		for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
		{			
			if(m_plsdManager->GetLoadGroupKindByCombKind(static_cast<ELSDCombKind>(nL)) != eGroupKind)
				continue;

			std::vector<CLsdLoadCase *> vecLoadCase = m_plsdManager->GetLsdLoadCaseArray((ELSDCombKind)nL, eLSD_LOADCASE_DIR_BRI);

			long nSizeCombo = vecLoadCase.size();
			for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
			{
				CLsdLoadCase *pLoadCase = vecLoadCase[nCombo];

				if(eGroupKind >= eLSD_COMBINE_TOTAL_COUNT) continue;

				if(pLoadCase->GetSizeExtraValueArray() < nExtra) continue;

				if(nFindInsex == nCheck)
				{
					if(sGrade == _T("B") && nL == eLSD_COMBINE_SERVICE_I)
					{
						bMatch = TRUE; //사용하중조합1
						return bMatch;
					}
					else if(sGrade == _T("C")  && (nL == eLSD_COMBINE_SERVICE_III || nL == eLSD_COMBINE_SERVICE_IV))
					{
						bMatch = TRUE; //사용하중조합3/4
						return bMatch;
					}
					else if(sGrade == _T("D") && (nL == eLSD_COMBINE_SERVICE_III || nL == eLSD_COMBINE_SERVICE_IV))
					{
						bMatch = TRUE; //사용하중조합 3/4
						return bMatch;
					}
					else if(sGrade == _T("E") && nL == eLSD_COMBINE_SERVICE_V)
					{
						bMatch = TRUE; //사용하중조합 5
						return bMatch;
					}
				}
				nFindInsex++;
			}
		}
	}


	return bMatch;
}

HGBOOL & CRcBridgeData::GetMaterialDetail()
{
	return m_bMaterialDetail;
}

void CRcBridgeData::SetMaterialDetail(HGBOOL bMaterialDetail)
{
	m_bMaterialDetail = bMaterialDetail;
}

HGDOUBLE &CRcBridgeData::GetRateDryAndShrinkage()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_dRateDryAndShrinkage;
	}

	return m_dRateDryAndShrinkage;
}

void CRcBridgeData::SetRateDryAndShrinkage(HGDOUBLE dRateDryAndShrinkage)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_dRateDryAndShrinkage = dRateDryAndShrinkage;
	else
		m_dRateDryAndShrinkage = dRateDryAndShrinkage;
}


HGDOUBLE &CRcBridgeData::GetCoefficientExpansion()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_dCoefficientExpansion;
	}

	return m_dCoefficientExpansion;
}

void CRcBridgeData::SetCoefficientExpansion(HGDOUBLE dCoefficientExpansion)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_dCoefficientExpansion = dCoefficientExpansion;
	else
		m_dCoefficientExpansion = dCoefficientExpansion;
}

HGBOOL &CRcBridgeData::GetDBar_ForDowelBar()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_bIsDBar_ForDowelBar;
	}

	return m_bIsDBar_ForDowelBar;
}

void CRcBridgeData::SetDBar_ForDowelBar(HGBOOL bIsDBar_ForDowelBar)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_bIsDBar_ForDowelBar = bIsDBar_ForDowelBar;
	else
		m_bIsDBar_ForDowelBar = bIsDBar_ForDowelBar;
}

long & CRcBridgeData::GetTypeDesingEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeDesingEQ;
	}

	return m_nTypeDesingEQ;
}

BOOL CRcBridgeData::IsApplyEarthQuakeDesign()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeDesingEQ != 0;
	}

	return m_nTypeDesingEQ != 0;
}

void CRcBridgeData::SetTypeDesingEQ(long nTypeDesingEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeDesingEQ = nTypeDesingEQ;
	else
		m_nTypeDesingEQ = nTypeDesingEQ;
}

long & CRcBridgeData::GetDegreeEarthQuake()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nDegreeEarthQuake;
	}

	return m_nDegreeEarthQuake;
}

void CRcBridgeData::SetDegreeEarthQuake(long nDegreeEarthQuake)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nDegreeEarthQuake = nDegreeEarthQuake;
	else
		m_nDegreeEarthQuake = nDegreeEarthQuake;
}

long & CRcBridgeData::GetRegionEarthQuake()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nRegionEarthQuake;
	}

	return m_nRegionEarthQuake;
}

void CRcBridgeData::SetRegionEarthQuake(long nRegionEarthQuake)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nRegionEarthQuake = nRegionEarthQuake;
	else
		m_nRegionEarthQuake = nRegionEarthQuake;
}

HGDOUBLE & CRcBridgeData::GetCoefficientGround()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_dCoefficientGround;
	}

	return m_dCoefficientGround;
}

void CRcBridgeData::SetCoefficientGround(double dCoefficientGround)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_dCoefficientGround = dCoefficientGround;
	else
		m_dCoefficientGround = dCoefficientGround;
}

HGDOUBLE & CRcBridgeData::GetGravity()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_gValue;
	}

	return m_gValue;
}

void CRcBridgeData::SetGravity(HGDOUBLE gValue)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_gValue = gValue;
	else
		m_gValue = gValue;
}

long & CRcBridgeData::GetTypeRigidFootingEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeRigidFootingEQ;
	}

	return m_nTypeRigidFootingEQ;
}

void CRcBridgeData::SetTypeRigidFootingEQ(long nTypeRigidFootingEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeRigidFootingEQ = nTypeRigidFootingEQ;
	else
		m_nTypeRigidFootingEQ = nTypeRigidFootingEQ;
}

HGBOOL & CRcBridgeData::GetApplyRigidWallEQRotate()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_bApplyRigidWallEQRotate;
	}

	return m_bApplyRigidWallEQRotate;
}

void CRcBridgeData::SetApplyRigidWallEQRotate(HGBOOL bApplyRigidWallEQRotate)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_bApplyRigidWallEQRotate = bApplyRigidWallEQRotate;
	else
		m_bApplyRigidWallEQRotate = bApplyRigidWallEQRotate;
}

long & CRcBridgeData::GetTypeJointPosFootingEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeJointPosFootingEQ;
	}

	return m_nTypeJointPosFootingEQ;
}

void CRcBridgeData::SetTypeJointPosFootingEQ(long nTypeJointPosFootingEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeJointPosFootingEQ = nTypeJointPosFootingEQ;
	else
		m_nTypeJointPosFootingEQ = nTypeJointPosFootingEQ;
}

long & CRcBridgeData::GetTypePropertyPosFootingEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypePropertyPosFootingEQ;
	}

	return m_nTypePropertyPosFootingEQ;
}

void CRcBridgeData::SetTypePropertyPosFootingEQ(long nTypePropertyPosFootingEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypePropertyPosFootingEQ = nTypePropertyPosFootingEQ;
	else
		m_nTypePropertyPosFootingEQ = nTypePropertyPosFootingEQ;
}

long & CRcBridgeData::GetTypeSoilPressureEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeSoilPressureEQ;
	}

	return m_nTypeSoilPressureEQ;
}

void CRcBridgeData::SetTypeSoilPressureEQ(long nTypeSoilPressureEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeSoilPressureEQ = nTypeSoilPressureEQ;
	else
		m_nTypeSoilPressureEQ = nTypeSoilPressureEQ;
}

long & CRcBridgeData::GetTypeUpperWeightEQ()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeUpperWeightEQ;
	}
	
	return m_nTypeUpperWeightEQ;
}

void CRcBridgeData::SetTypeUpperWeightEQ(long nTypeUpperWeightEQ)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeUpperWeightEQ = nTypeUpperWeightEQ;
	else
		m_nTypeUpperWeightEQ = nTypeUpperWeightEQ;
}

HGDOUBLE & CRcBridgeData::GetValueStmFck(long nStmType)
{
	switch( nStmType )
	{
	case eLsdStmCornerUpper:
		return m_Fck[ePartUpperSlab];
		break;
	case eLsdStmCornerLower:
		return m_Fck[ePartLowerSlab];
		break;
	case eLsdStmBracketStt:
		return m_Fck[ePartWingWallStt];
		break;
	case eLsdStmBracketEnd:
		return m_Fck[ePartWingWallEnd];
		break;
	case eLsdStmFootStt:
		return m_Fck[ePartFootStt];
		break;
	case eLsdStmFootEnd:
		return m_Fck[ePartFootEnd];
		break;
	default:		// eLsdStmFootMid
		return m_Fck[ePartInFoot];
		break;
	}
}

HGDOUBLE & CRcBridgeData::GetValueStmFctk(long nStmType)
{
	return m_Fctk[nStmType];
}

HGDOUBLE & CRcBridgeData::GetValueStmEc(long nStmType)
{
	switch( nStmType )
	{
	case eLsdStmCornerUpper:
		return m_Ec[ePartUpperSlab];
		break;
	case eLsdStmCornerLower:
		return m_Ec[ePartLowerSlab];
		break;
	case eLsdStmBracketStt:
		return m_Ec[ePartWingWallStt];
		break;
	case eLsdStmBracketEnd:
		return m_Ec[ePartWingWallEnd];
		break;
	case eLsdStmFootStt:
		return m_Ec[ePartFootStt];
		break;
	case eLsdStmFootEnd:
		return m_Ec[ePartFootEnd];
		break;
	default:		// eLsdStmFootMid
		return m_Ec[ePartInFoot];
		break;
	}
}

HGDOUBLE & CRcBridgeData::GetValueStmFy(long nStmType)
{
	switch( nStmType )
	{
	case eLsdStmCornerUpper:
		return m_Fy[ePartUpperSlab];
		break;
	case eLsdStmCornerLower:
		return m_Fy[ePartLowerSlab];
		break;
	case eLsdStmBracketStt:
		return m_Fy[ePartWingWallStt];
		break;
	case eLsdStmBracketEnd:
		return m_Fy[ePartWingWallEnd];
		break;
	case eLsdStmFootStt:
		return m_Fy[ePartFootStt];
		break;
	case eLsdStmFootEnd:
		return m_Fy[ePartFootEnd];
		break;
	default:		// eLsdStmFootMid
		return m_Fy[ePartInFoot];
		break;
	}
}

HGDOUBLE & CRcBridgeData::GetValueStmFvy(long nStmType)
{
	switch( nStmType )
	{
	case eLsdStmCornerUpper:
		return m_Fvy[ePartUpperSlab];
		break;
	case eLsdStmCornerLower:
		return m_Fvy[ePartLowerSlab];
		break;
	case eLsdStmBracketStt:
		return m_Fvy[ePartWingWallStt];
		break;
	case eLsdStmBracketEnd:
		return m_Fvy[ePartWingWallEnd];
		break;
	case eLsdStmFootStt:
		return m_Fvy[ePartFootStt];
		break;
	case eLsdStmFootEnd:
		return m_Fvy[ePartFootEnd];
		break;
	default:		// eLsdStmFootMid
		return m_Fvy[ePartInFoot];
		break;
	}
}

CString CRcBridgeData::GetStrLsdStmTitle(long nStmType)
{
	switch( nStmType )
	{
	case eLsdStmCornerUpper:
		{
			if( IsBoxType() )
				return _T("우각부 - 상면");
			
			return _T("우각부");
		}
		break;
	case eLsdStmCornerLower:
		{
			if( IsBoxType() )
				return _T("우각부 - 하면");

			return _T("우각부");
		}
		break;
	case eLsdStmBracketStt:
		return _T("브라켓 - 시점");
		break;
	case eLsdStmBracketEnd:
		return _T("브라켓 - 종점");
		break;
	case eLsdStmFootStt:
		return _T("기초 - 시점");
		break;
	case eLsdStmFootEnd:
		return _T("기초 - 종점");
		break;
	default:		// eLsdStmFootMid
		return _T("기초 - 중간지점");
		break;
	}
}

void CRcBridgeData::InitAggregateAndSlump()
{
	m_bAggregate_SlumpDetail = TRUE;
	for (HGINT32 ix = 0; ix < ePartCount; ++ix)
	{
		if (ix == ePartApsStt || ix == ePartApsEnd)
		{
			m_dSlump[ix] = 8;
		}
		else
		{
			m_dSlump[ix] = 15;
		}

		m_dAggregate[ix] = 25;
	}

	m_dAggregate_LeanLSD = 40;
	m_dAggregate_MassLSD = 40;
	m_dAggregate_HyuntaLSD = 25;
	m_dAggregate_EtcLSD = 19;

	m_dSlump_LeanLSD = 8;
	m_dSlump_MassLSD = 	8;	
	m_dSlump_HyuntaLSD = 8;
	m_dSlump_EtcLSD = 15;
}

HGDOUBLE CRcBridgeData::GetValueAggregate(EPartOfBridge ePos, HGBOOL bNotLsd) const
{
	if(m_pARcBridgeDataStd->IsLsdDesign() && bNotLsd == FALSE)
	{
		return m_dAggregate[ePos];
	}

	HGDOUBLE dAggregate = 0;
	switch (ePos)
	{
	case ePartUpperSlab:
	case ePartLowerSlab:
	case ePartOutWallStt:
	case ePartWingWallStt:
	case ePartOutWallEnd:
	case ePartWingWallEnd:
	case ePartInWall:
		dAggregate = m_dAggregate_Main;
		break;
	case ePartFootStt:
	case ePartFootEnd:
	case ePartInFoot:
		dAggregate = m_dAggregate_Footing;
		break;
	case ePartApsStt:
	case ePartApsEnd:
		dAggregate = m_dAggregate_Aps;
		break;
	}

	return dAggregate;
}

HGDOUBLE CRcBridgeData::GetValueAggregateLean() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dAggregate_LeanLSD;
	}

	return m_dAggregate_Lean;
}

HGDOUBLE CRcBridgeData::GetValueAggregateMass() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dAggregate_MassLSD;
	}

	return m_dAggregate_Mass;
}

HGDOUBLE CRcBridgeData::GetValueAggregateHyunta() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dAggregate_HyuntaLSD;
	}

	return m_dAggregate_Hyunta;
}

HGDOUBLE CRcBridgeData::GetValueAggregateEtc() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dAggregate_EtcLSD;
	}

	return m_dAggregate_Etc;
}

HGDOUBLE CRcBridgeData::GetValueSlump(EPartOfBridge ePos, HGBOOL bNotLsd) const
{
	if(m_pARcBridgeDataStd->IsLsdDesign() && bNotLsd == FALSE)
	{
		return m_dSlump[ePos];
	}

	HGDOUBLE dSlump = 0;
	switch (ePos)
	{
	case ePartUpperSlab:
	case ePartLowerSlab:
	case ePartOutWallStt:
	case ePartWingWallStt:
	case ePartOutWallEnd:
	case ePartWingWallEnd:
	case ePartInWall:
		dSlump = m_dSlump_Main;
		break;
	case ePartFootStt:
	case ePartFootEnd:
	case ePartInFoot:
		dSlump = m_dSlump_Footing;
		break;
	case ePartApsStt:
	case ePartApsEnd:
		dSlump = m_dSlump_Aps;
		break;
	}

	return dSlump;
}

HGDOUBLE CRcBridgeData::GetValueSlumpLean() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dSlump_LeanLSD;
	}

	return m_dSlump_Lean;
}

HGDOUBLE CRcBridgeData::GetValueSlumpMass() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dSlump_MassLSD;
	}

	return m_dSlump_Mass;
}

HGDOUBLE CRcBridgeData::GetValueSlumpHyunta() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dSlump_HyuntaLSD;
	}

	return m_dSlump_Hyunta;
}

HGDOUBLE CRcBridgeData::GetValueSlumpEtc() const
{
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		return m_dSlump_EtcLSD;
	}

	return m_dSlump_Etc;
}

CString CRcBridgeData::GetStringComboEnv_Lsd( ELSDCombKind eKind, EPartOfBridge ePart, BOOL bMark, BOOL bServiceCrack )
{
	CString szKind(_T(""));
	CString szPart(_T(""));

	switch(eKind)
	{
	case eLSD_COMBINE_LIMIT_I:
	case eLSD_COMBINE_LIMIT_II:
	case eLSD_COMBINE_LIMIT_III:
	case eLSD_COMBINE_LIMIT_IV:
	case eLSD_COMBINE_LIMIT_V:
		szKind = bMark? _T("ST") : _T("극한한계상태");	break;
	case eLSD_COMBINE_EXTREME_I:
		szKind = bMark? _T("EX1") : _T("극단상황한계상태1");	break;
	case eLSD_COMBINE_EXTREME_II:
		szKind = bMark? _T("EX2") : _T("극단상황한계상태2");	break;
	case eLSD_COMBINE_SERVICE_I:
		szKind = bMark? _T("SEB") : _T("사용한계상태1");	break;
	case eLSD_COMBINE_SERVICE_II:
		szKind = bMark? _T("SE2") : _T("사용한계상태2");	break;
	case eLSD_COMBINE_SERVICE_III:
	case eLSD_COMBINE_SERVICE_IV:
		szKind = bMark? _T("SECD") : _T("사용한계상태3-4");	break;
	case eLSD_COMBINE_SERVICE_V:
		szKind = bMark? _T("SEE") : _T("사용한계상태5");	break;
	case eLSD_COMBINE_STRESS:
		szKind = bMark? _T("FG") : _T("피로한계상태");	break;
	default:
		break;
	}

	if(bServiceCrack && eKind >= eLSD_COMBINE_SERVICE_I && eKind <= eLSD_COMBINE_SERVICE_V)
	{
		szKind = szKind.Left(2);
	}

	if(bMark)
	{
		if(m_bEtaCombinEach)
		{
			switch (ePart)
			{
			case ePartUpperSlab:
				szPart = _T("TOP");	break;
			case ePartLowerSlab:
				szPart = _T("BOT");	break;
			case ePartOutWallStt:
			case ePartOutWallEnd:
				szPart = _T("WALL");	break;
			case ePartInWall:
				szPart = _T("MWALL");	break;
			default:
				break;
			}
		}
		else
		{
			szPart = _T("GEN");
		}
	}
	else
	{
		if(m_bEtaCombinEach)
		{
			szPart = GetStringPartMerge(ePart);
		}
		else
		{
			szPart = _T("");
		}
	}

	CString szRet = bMark? (szKind + szPart) : (szKind + _T(" ") + szPart);

	// (ARCBRIDGE-2692) 조합 이름이 길어지면 어짜피 8글자에서 짤리기 때문에 나중에 조합을 찾지 못하게 된다. 미리 잘라준다.
	if(bMark && szRet.GetLength() > 8)
		szRet = szRet.Left(8);

	return szRet;
}

CString CRcBridgeData::GetStringComboEta_Lsd( ELSDCombKind eKind, EPartOfBridge ePart, long nIndexCombo, BOOL bOrigne, long nIdxModel)
{
	CString szKind(_T(""));
	CString szPart(_T(""));

	switch(eKind)
	{
	case eLSD_COMBINE_LIMIT_I:
	case eLSD_COMBINE_LIMIT_II:
	case eLSD_COMBINE_LIMIT_III:
	case eLSD_COMBINE_LIMIT_IV:
	case eLSD_COMBINE_LIMIT_V:
		szKind = _T("ST");	break;
	case eLSD_COMBINE_EXTREME_I:
		szKind = _T("EX1");	break;
	case eLSD_COMBINE_EXTREME_II:
		szKind = _T("EX2");	break;
	case eLSD_COMBINE_SERVICE_I:
		szKind = _T("SE1");	break;
	case eLSD_COMBINE_SERVICE_II:
		szKind = _T("SE2");	break;
	case eLSD_COMBINE_SERVICE_III:
		szKind = _T("SE3");	break;
	case eLSD_COMBINE_SERVICE_IV:
		szKind = _T("SE4");	break;
	case eLSD_COMBINE_SERVICE_V:
		szKind = _T("SE5");	break;
	default:
		break;
	}

	if(m_bEtaCombinEach)
	{
		switch (ePart)
		{
		case ePartOutWallStt:
		case ePartOutWallEnd:
			szPart = _T("W");	break;
		case ePartInWall:
			szPart = _T("MW");	break;
		case ePartFootStt:
		case ePartFootEnd:
			szPart = _T("F");	break;
		case ePartInFoot:
			szPart = _T("MF");	break;
		default:
			break;
		}
	}
	else
	{
		szPart = _T("G");
	}

	long nComboEq = COMBO_EARTHQUAKE;
	if(nIdxModel > 1)
	{
		nComboEq = COMBO_EARTHQUAKE2;
	}

	CString szCombo(_T(""));
	if(nIndexCombo > 0)
	{
		szCombo.Format(_T("%s%s%03d"), szKind, szPart, nIndexCombo);
	}
	else
	{
		szCombo = szKind + szPart;
	}

	if(bOrigne && nIndexCombo > 0)
	{
		// 원래 조합 이름을 찾아서 Return한다.
		BOOL bEarthQuake = (eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II)? TRUE : FALSE;
		CFEMManage* pFem = GetFemManage(bEarthQuake? nComboEq : COMBO_ULTIMATE);
		if(pFem)
		{
			CCombo *pCombo = pFem->GetComboByName(szCombo);
			if(pCombo && pCombo->m_szLoadName.GetSize() > 0)
			{
				szCombo = pCombo->m_szLoadName.GetAt(0);
			}
		}
	}

	return szCombo;
}

CString CRcBridgeData::GetStringComboEnv( long nTypeDesign, ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos )
{
	CString szCombo(_T(""));

	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		// COMBO_ENV_LSD_SERVICE 일때는 설계등급 선택에 따라 조합을 선택해 준다. ePart와 nPos에 의해서 설계등급을 찾고 거기에 해당하는 조합을 찾자.
		
		ELSDCombKind eComboKind = GetComboKindByEnvType(eTypeEnvLsd, ePart, nPos);

		szCombo = GetStringComboEnv_Lsd(eComboKind, ePart, TRUE, eTypeEnvLsd == eEnvService_Crack) + _T(" MAX");
	}
	else
	{
		switch(nTypeDesign)
		{
		case COMBO_SERVICE:
			szCombo = _T("ENVUSE MAX");	break;
		case COMBO_ULTIMATE:
			szCombo = _T("ENVULT MAX");	break;
		case COMBO_ECCENTRIC:
			szCombo = _T("ENVECC MAX");	break;
		case COMBO_EARTHQUAKE:
		case COMBO_EARTHQUAKE2:
			szCombo = _T("ENV MAX");	break;
		default:
			break;
		}
	}

	return szCombo;
}

ELSDCombKind CRcBridgeData::GetComboKindServiceByDesignGrade( EPartOfBridge ePart, long nPos )
{
	
	CLsdDesignRatingCtl * pDesignRatingCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
	if(pDesignRatingCtl == NULL) 
		return eLSD_COMBINE_SERVICE_I;

	BOOL bUpAndFront(FALSE);
	switch(ePart)
	{
	case ePartUpperSlab:
		bUpAndFront = (nPos == CHECK_POS_CENTER)? FALSE : TRUE;	break;
	case ePartLowerSlab:
	case ePartOutWallStt:
	case ePartOutWallEnd:
		bUpAndFront = (nPos == CHECK_POS_CENTER)? TRUE : FALSE;	break;
	default:
		break;
	}
	
	long nIndex = GetLsdMatrialIndex(ePart, bUpAndFront);
	long nGrade = pDesignRatingCtl->GetMaterialGrade(nIndex);

	ELSDCombKind eComboService(eLSD_COMBINE_SERVICE_I);

	switch (nGrade)
	{
	case 0: eComboService = eLSD_COMBINE_SERVICE_II; break;		// A등급일때 집계안함. ENV조합이 없는 2로...
	case 1: eComboService = eLSD_COMBINE_SERVICE_I; break;
	case 2: eComboService = eLSD_COMBINE_SERVICE_III; break;
	case 3: eComboService = eLSD_COMBINE_SERVICE_III; break;
	case 4: eComboService = eLSD_COMBINE_SERVICE_V; break;
	}

	return eComboService;
}

BOOL CRcBridgeData::IsValidBridgePart( EPartOfBridge ePart )
{
	BOOL bValid(TRUE);

	if(ePart == ePartLowerSlab && IsBoxType() == FALSE)
		bValid = FALSE;
	else if((ePart == ePartOutWallStt || ePart == ePartOutWallEnd) && IsOutWall(TRUE) == FALSE)
		bValid = FALSE;
	else if(ePart == ePartInWall)
	{
		if(GetCountInWall() == 0)
			bValid = FALSE;
	}
	else if(ePart == ePartInFoot)
	{
		if(GetCountInWall() == 0)
			bValid = FALSE;
		else if(IsArchType() || m_nTypeBridge == BRIDGE_TYPE_TRAP2 || IsBoxType())
			bValid = FALSE;
	}

	return bValid;
}

CString CRcBridgeData::GetStringComboEnvType_Lsd( ETypeEnvComboLsd eTypeEnvLsd, BOOL bSubClass, BOOL bTwoLine )
{
	CString szName(_T(""));

	switch(eTypeEnvLsd)
	{
	case eEnvLimit:
		szName = _T("극한한계상태");
		break;
	case eEnvExtremeI:
		szName = _T("극단상황한계상태Ⅰ");
		break;
	case eEnvExtremeII:
		szName = _T("극단상황한계상태Ⅱ");
		break;
	case eEnvService:
		szName =  _T("사용한계상태");
		break;
	case eEnvService_Crack:
		szName =  _T("사용한계상태");
		if(bSubClass)
		{
			szName += bTwoLine? _T("\n(균열판정)") : _T(" - 균열판정");
		}
		break;
	default:
		break;
	}

	return szName;
}

BOOL CRcBridgeData::IsValidComboEnvType( ETypeEnvComboLsd eTypeEnvLsd )
{
	// 극한한계상태와 사용한계상태는 무조건 있어야 계산이 진행 되는걸로.
	BOOL bValid(TRUE);
	if(eTypeEnvLsd == eEnvExtremeI)
	{
		if(m_plsdManager->IsUsedComboKind(eLSD_COMBINE_EXTREME_I) == FALSE)
			bValid = FALSE;
	}
	else if(eTypeEnvLsd == eEnvExtremeII)
	{
		if(m_plsdManager->IsUsedComboKind(eLSD_COMBINE_EXTREME_II) == FALSE)
			bValid = FALSE;
	}

	return bValid;
}

/*
BOOL bApplyServiceGrade : 사용하중 조합이 안정검토 일때와 사용성검토 일때 설계등급 고려 여부에 따라 집계되는 조합이 달라지기때문에 분리했다.
*/

long CRcBridgeData::GetSizeComboByType( ETypeEnvComboLsd eEnvTypeLsd, EPartOfBridge ePart, CString &szComboFirst )
{
	long nCount(0);
	for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
	{	
		ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

		if(eEnvTypeLsd == eEnvLimit)
		{
			if(eKind > eLSD_COMBINE_LIMIT_V) break;
		}
		else if(eEnvTypeLsd == eEnvExtremeI)
		{
			if(eKind != eLSD_COMBINE_EXTREME_I) continue;
		}
		else if(eEnvTypeLsd == eEnvExtremeII)
		{
			if(eKind != eLSD_COMBINE_EXTREME_II) continue;
		}
		else if(eEnvTypeLsd == eEnvService || eEnvTypeLsd == eEnvService_Crack)
		{
			if(eKind < eLSD_COMBINE_SERVICE_I) continue;
			if(eKind > eLSD_COMBINE_SERVICE_V) break;

			if(eEnvTypeLsd == eEnvService)
			{
				ELSDCombKind eKindService = GetComboKindServiceByDesignGrade(ePart, CHECK_POS_CENTER);
				if(eKindService == eLSD_COMBINE_SERVICE_III)
				{
					if(eKind != eLSD_COMBINE_SERVICE_III && eKind != eLSD_COMBINE_SERVICE_IV) continue;
				}
				else
				{
					if(eKind != eKindService) continue;
				}
			}
		}

		std::vector<CLsdLoadCase *> LCArray = m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
		if(nCount == 0 && LCArray.size() > 0)
		{
			szComboFirst = LCArray[0]->GetName();
		}

		nCount += LCArray.size();
	}

	return nCount;
}

ELSDCombKind CRcBridgeData::GetComboKindByEnvType( ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos )
{
	ELSDCombKind eComboKind(eLSD_COMBINE_LIMIT_I);
	switch(eTypeEnvLsd)
	{
	case eEnvLimit:
		eComboKind = eLSD_COMBINE_LIMIT_I;	break;
	case eEnvExtremeI:
		eComboKind = eLSD_COMBINE_EXTREME_I;	break;
	case eEnvExtremeII:
		eComboKind = eLSD_COMBINE_EXTREME_II;	break;
	case eEnvService:
		eComboKind = GetComboKindServiceByDesignGrade(ePart, nPos);	break;
	case eEnvService_Crack:
		{
			for(long ix=eLSD_COMBINE_SERVICE_I; ix<=eLSD_COMBINE_SERVICE_V; ++ix)
			{
				eComboKind = static_cast<ELSDCombKind>(ix);
				if(m_plsdManager->IsUsedComboKind(eComboKind))
				{
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	return eComboKind;
}

void CRcBridgeData::InitConcSlab()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcSlab[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcWall()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcWall[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcInWall()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcInWall[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcPRF()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcPRF[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcFoot()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcFoot[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcFootRev()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcFootRev[ix],   (CConcBasic*)0);
	}
}

void CRcBridgeData::InitConcFootCheckAdd()
{
	for (HGINT32 ix = 0; ix < COUNT_CONC_LSD; ++ix)
	{
		AhTPADelete(&m_pArrConcFootCheckAdd[ix],   (CConcBasic*)0);
	}
}

long CRcBridgeData::GetCountApplyEnvType(ETypeEnvComboLsd eCountEnvType)
{
	long nCountEnvType(0);
	for(long ix=0; ix<eCountEnvType; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		++nCountEnvType;
	}

	return nCountEnvType;
}

CString CRcBridgeData::GetStringFootingCheckCase( EFootingCheckLsd eCheck )
{
	CString str(_T(""));
	switch(eCheck)
	{
	case eFootCheckAxial:
		str = _T("축력 최대");	break;
	case eFootCheckMomentMinus:
		str = _T("부모멘트 최대");	break;
	case eFootCheckMomentPlus:
		str = _T("정모멘트 최대");	break;
	case eFootCheckHoriMax:
		str = _T("수평력 최대");	break;
	case eFootCheckHoriMin:
		str = _T("수평력 최소");	break;
	default:
		break;
	}

	return str;
}

CString CRcBridgeData::GetStringWallCheckCase( EWallCheckLsd eCheck )
{
	CString str(_T(""));
	switch(eCheck)
	{
	case eWallCheckAxialMax:
		str = _T("축력 최대");	break;
	case eWallCheckMomentMax:
		str = _T("모멘트 최대");	break;
	case eWallCheckEccMax:
		str = _T("편심 최대");	break;
	case eWallCheckHoriMax:
		str = _T("수평력 최대");	break;
	case eWallCheckAxialMin:
		str = _T("축력 최소");	break;
	default:
		break;
	}

	return str;
}

CString CRcBridgeData::GetStringLoadTemperatureSlopeApply()
{
	switch(m_nApplyPavement)
	{
	case 0:
		return _T("아스팔트 포장");
		break;
	case 1:
		return _T("콘크리트 포장");
		break;
	}

	return _T("");
}

HGINT32 CRcBridgeData::GetCountUserLoadSetLsd( long nKind )
{
	return m_vpUserLoadSet[nKind].size();
}

// void CRcBridgeData::ResizeUserLoadSetLsd( BOOL bNormal, HGINT32 icSize )
// {
// 	ResizeVectorPtr(m_vpUserLoadSet[bNormal? 0 : 1], icSize);
// }

void CRcBridgeData::InsertUserLoadSetLsd( long nKind, HGINT32 Idx )
{
	InsertVectorPtr(m_vpUserLoadSet[nKind], Idx);
}

void CRcBridgeData::EraseUserLoadSetLsd( long nKind, HGINT32 Idx )
{
	if ( Idx >= static_cast<HGINT32>(m_vpUserLoadSet[nKind].size()) || Idx < 0 ) return;

	EraseVectorPtrElement(m_vpUserLoadSet[nKind], Idx);
}

CString CRcBridgeData::GetStringUserLoadType( HGINT32 nType )
{
	CString str;
	switch(nType)
	{
	case USER_LOAD_JOINT_V:
		str = hggettext("절점하중(V)");
		break;
	case USER_LOAD_JOINT_M:
		str = hggettext("절점하중(M)");
		break;
	case USER_LOAD_ELEMENT_V:
		str = hggettext("부재하중(V)");
		break;
	case USER_LOAD_ELEMENT_WX:
		str = hggettext("부재하중(Wx)");
		break;
	case USER_LOAD_ELEMENT_WY:
		str = hggettext("부재하중(Wy)");
		break;
	case USER_LOAD_ELEMENT_WZ:
		str = hggettext("부재하중(Wz)");
		break;
	}

	return str;
}

long CRcBridgeData::GetLsdEnvType(HGINT32 iKey)
{
	if(iKey >= STM_MODEL_FOOT_MID_MAX_AXIAL)
	{
		long iKeyTemp = iKey - STM_MODEL_FOOT_MID_MAX_AXIAL;
		long nKel = iKeyTemp % (5*3);
		iKey = STM_MODEL_FOOT_MID_MAX_AXIAL + nKel;
	}

	if( iKey < STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1 || ( STM_MODEL_FOOT_END_MAX_AXIAL <= iKey && iKey < STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1 ) || 
		( STM_MODEL_FOOT_MID_MAX_AXIAL <= iKey && iKey < STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_1 ))
		return eEnvLimit;
	else if( (STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1 <= iKey && iKey < STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2) || ( STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1 <= iKey && iKey < STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2 ) || 
		( STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_1 <= iKey && iKey < STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_2 ))
		return eEnvExtremeI;
	else
		return eEnvExtremeII;
}

CString CRcBridgeData::GetStringCombo(ELSDCombKind eComboKind, long nIndexCombo, HGBOOL bForCantilever)
{
	CString strCombo("");

	CLsdManagerBridge *plsdManager = bForCantilever? m_plsdMngForCantilever : m_plsdManager;
	std::vector<CLsdLoadCase *> vecLoadCase = plsdManager->GetLsdLoadCaseArray(eComboKind, bForCantilever? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);

	long nSizeCombo = vecLoadCase.size();
	if(nSizeCombo == 0 || nIndexCombo >= nSizeCombo) return strCombo;

	CLsdLoadCase *pLoadCase = vecLoadCase[nIndexCombo];

	CString strFactor, strLoad;
	BOOL bFirstCombo = TRUE;
	for(long nLoadCase = 0; nLoadCase < eLSD_LOADCASE_SIZE; nLoadCase++)
	{
		double dFactor = pLoadCase->GetLoadFactor(ELSDLoadCaseKind(nLoadCase));
		if(dFactor == 0) continue;
		strLoad = m_plsdManager->GetDesignNameByIndex(ELSDLoadCaseKind(nLoadCase));

		// 숫자 짜르기
		if(strLoad.Find("DC") != -1) strLoad = "DC";
		if(strLoad.Find("DW") != -1) strLoad = "DW";
		if(strLoad.Find("LL") != -1) strLoad = "LL";
		if(strLoad.Find("WS") != -1) strLoad = "WS";
		if(strLoad.Find("CT") != -1) strLoad = "CT";

		if(Compare(dFactor * 10 - (long)(dFactor * 10), 0.0, _T("=")))
		{
			strFactor.Format(_T("%.1f%s"), dFactor, strLoad);
		}
		else
		{
			strFactor.Format(_T("%.2f%s"), dFactor, strLoad);
		}

		if(bFirstCombo == FALSE)
		{
			strCombo += _T(" + ");
		}

		bFirstCombo = FALSE;
		strCombo += strFactor;
	}


	return strCombo;
}

CString CRcBridgeData::GetLoadCaseName(ELSDLoadCaseKind eLoadCaseKind)
{
	CString sResult = m_plsdManager->GetDesignNameByIndex(eLoadCaseKind);

	switch(eLoadCaseKind)
	{
	case eLSD_LOADCASE_DC1:
		sResult = _T("DC"); break;
	case eLSD_LOADCASE_DW1:
		sResult = _T("DW"); break;
	case eLSD_LOADCASE_LL1:
		sResult = _T("LL"); break;
	case eLSD_LOADSELECT_IM:
		sResult = _T("IM"); break;
	case eLSD_LOADSELECT_PL:
		sResult = _T("PL"); break;
	case eLSD_LOADCASE_CF:
		sResult = _T("CF"); break;
	case eLSD_LOADCASE_WS11:
		sResult = _T("WS"); break;
	case eLSD_LOADCASE_CT11:
		sResult = _T("CT"); break;
	}

	return sResult;
}

// void CRcBridgeData::InitFemManage()
// {
// 	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
// 
// 	CFEMManage::EDecimal eDec = bLsd? CFEMManage::eDec6 : CFEMManage::eDec3;
// 
// 	m_FemNormal.m_eDecimal = eDec;
// 	m_FemEQ[0].m_eDecimal = eDec;
// 	m_FemEQ[1].m_eDecimal = eDec;
// 	m_FemEQ[2].m_eDecimal = eDec;
// 	m_FemShell.m_eDecimal = eDec;
// 	m_FemWingWall[0][0].m_eDecimal = eDec;
// 	m_FemWingWall[0][1].m_eDecimal = eDec;
// 	m_FemWingWall[1][0].m_eDecimal = eDec;
// 	m_FemWingWall[1][1].m_eDecimal = eDec;
// }

BOOL CRcBridgeData::IsBoredPileASD(long nJijum)
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE) return FALSE;

	if(m_pARcBridgeDataStd->IsApplyBoredPileASD())
	{
		CFootingApp *pFooting = GetFootingByJijumNumber(nJijum);
		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			return pFooting->m_exFooting.m_Pile.IsBoredPile(); // 매입말뚝의 허용응력설계법 적용 
		}
	}

	return FALSE;
}

BOOL CRcBridgeData::IsPhcPileASD( long nJijum )
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE) return FALSE;

	if(m_pARcBridgeDataStd->IsApplyPhcPileASD())
	{
		CFootingApp *pFooting = GetFootingByJijumNumber(nJijum);
		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			return pFooting->m_exFooting.m_Pile.IsPHC(); // PHC말뚝의 허용응력설계법 적용 
		}
	}

	return FALSE;
}

void CRcBridgeData::SetDataDefaultSelectComboForWingWallAndAps( ELSDLoadGroupKind eType )
{
	long nIdxWingWall(0), nIdxAps(0);
	if(eType == eLSD_COMBINATION_SERVICE)
	{
		ELSDCombKind eComboKindWingWall = GetComboKindServiceByDesignGrade(ePartWingWallStt, 1);
		ELSDCombKind eComboKindWingAps = GetComboKindServiceByDesignGrade(ePartApsStt, 1);

		long nComboKindStt = eLSD_COMBINE_SERVICE_I;
		long nComboKindEnd = eLSD_COMBINE_SERVICE_V;

		long nIndex(0);
		for(long nComboKind = nComboKindStt; nComboKind <= nComboKindEnd; nComboKind++)
		{
			std::vector<CLsdLoadCase *> vecLoadCase = m_plsdManager->GetLsdLoadCaseArray((ELSDCombKind)nComboKind, eLSD_LOADCASE_DIR_BRI);

			long nSizeCombo = vecLoadCase.size();
			if(nSizeCombo == 0) continue;

			if(nComboKind == eComboKindWingWall)
				nIdxWingWall = nIndex;
			if(nComboKind == eComboKindWingAps)
				nIdxAps = nIndex;

			nIndex += nSizeCombo;
		}
	}

	if(eType == eLSD_COMBINATION_SERVICE)
	{
		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("날개벽\n판정시"), eExtraRadio, 0);
		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("날개벽\n검토시"), eExtraRadio, nIdxWingWall);

		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("접속슬래브\n판정시"), eExtraRadio, 0);
		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("접속슬래브\n검토시"), eExtraRadio, nIdxAps);
	}
	else
	{
		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("날개벽"), eExtraRadio, nIdxWingWall);
		m_plsdManager->AddExtraColumn(eType, eLSD_LOADCASE_DIR_BRI, hggettext_be("접속슬래브"), eExtraRadio, nIdxAps);
	}
}


CBoringDataEx *CRcBridgeData::GetBoringDataEx(long nJijum)
{
	CFootingApp *pFooting = GetFootingByJijumNumber(nJijum);
	if( NULL == pFooting ) return NULL;

	CBoringDataEx *pBoring = NULL;
	CBoringDataEx *pBoringChange = NULL;

	HGINT32 ixBoring = pFooting->GetBoringIndex();
	if( ixBoring < 0 ) return NULL;

	long nSizeBoring = m_pARcBridgeDataStd->m_pArrBoringEx.GetSize();
	if( nSizeBoring < 1 || nSizeBoring < ixBoring ) return NULL;

	pBoring = m_pARcBridgeDataStd->m_pArrBoringEx.GetAt(ixBoring);

	pBoringChange = GetMapPtrElementKey(const_cast<std::map<HGINT32,CBoringDataEx*> &>(m_pARcBridgeDataStd->m_mpBoring), (HGINT32)nJijum);
	if(pBoringChange == NULL)
	{
		pBoringChange = new CBoringDataEx();
		m_pARcBridgeDataStd->m_mpBoring.insert(std::make_pair(nJijum, pBoringChange));
	}

	*(pBoringChange) = *pBoring;

	pBoringChange->m_EL += pFooting->GetBoringEL();
	return pBoringChange;
}

ELSDLoadGroupKind CRcBridgeData::GetGroupKindByEnvType( ETypeEnvComboLsd eTypeEnvLsd )
{
	ELSDLoadGroupKind eGroupKind(eLSD_COMBINATION_LIMIT);
	switch(eTypeEnvLsd)
	{
	case eEnvLimit:
		eGroupKind = eLSD_COMBINATION_LIMIT;	break;
	case eEnvExtremeI:
		eGroupKind = eLSD_COMBINATION_EXTREME;	break;
	case eEnvExtremeII:
		eGroupKind = eLSD_COMBINATION_EXTREME;	break;
	case eEnvService:
		eGroupKind = eLSD_COMBINATION_SERVICE;	break;
	case eEnvService_Crack:
		eGroupKind = eLSD_COMBINATION_FATIGUE;	break;
	default:
		break;
	}

	return eGroupKind;
}

CString CRcBridgeData::GetStringDegreeEarthQuake()
{
	CString strRet(_T(""));

	long nDegree = GetDegreeEarthQuake();
	switch(nDegree)
	{
	case 0:
		strRet = _T("I 등급");
		break;
	case 1:
		strRet = _T("II 등급");
		break;
	case 2:
		strRet = _T("특등급(2400년)");
		break;
	case 3:
		strRet = _T("특등급(4800년)");
		break;
	default:
		break;
	}

	return strRet;
}

long & CRcBridgeData::GetTypeCoefficientGround()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_nTypeCoefficientGround;
	}

	return m_nTypeCoefficientGround;
}

void CRcBridgeData::SetTypeCoefficientGround( long dTypeGround )
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_nTypeCoefficientGround = dTypeGround;
	else
		m_nTypeCoefficientGround = dTypeGround;
}

long CRcBridgeData::GetEarthQuakeModelIndex( EGoalOfEarthQuake eGoalEq )
{
	long nIdx(-1);
	for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
	{
		if(ix > eGoalEq) break;

		if(m_bGoalEarthQuake[ix])
			++nIdx;
	}

	return nIdx;
}

long CRcBridgeData::GetCountEarthQuakeModel(BOOL bResult)
{
	long nCount(0);
	if(GetTypeDesingEQ() == 0 || IsRcSlab())
	{
		nCount = 0;
	}
	else if(IsBoxType())
	{
		for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
		{
			if(m_bGoalEarthQuake[ix])
				++nCount;
		}
	}
	else
	{
		nCount = m_nSelectModelingCountEQ == 2 ? 2 : 1;
		if(bResult == FALSE)
			nCount += 1;			// 단일모드 포함.
	}

	return nCount;
}

EGoalOfEarthQuake CRcBridgeData::GetEarthQuakeGoalByIndex( long nIndexEq )
{
	EGoalOfEarthQuake eGoalEq = eGoalEQ_None;
	if(IsBoxType() == FALSE)
		return eGoalEq;

	long nIdx(-1);
	for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
	{
		if(m_bGoalEarthQuake[ix])
			++nIdx;

		if(nIdx == nIndexEq)
		{
			eGoalEq = static_cast<EGoalOfEarthQuake>(ix);
			break;
		}
	}

	return eGoalEq;
}

CString CRcBridgeData::GetStringGoalEarthQuake( EGoalOfEarthQuake eGoalEq, BOOL bLong )
{
	CString str(_T(""));

	if(IsBoxType())
	{
		switch(eGoalEq)
		{
		case eGoalEQ_Collapse:
			str = _T("붕괴방지");	break;
		case eGoalEQ_Function:
			str = _T("기능수행");	break;
		case eGoalEQ_Immediately:
			str = _T("즉시복구");	break;
		case eGoalEQ_LongTerm:
			str = bLong ? _T("장기복구/인명보호") : _T("장기_인명");	break;
		default:
			break;
		}

		if(bLong)
			str += _T("수준");
	}

	return str;
}

CString CRcBridgeData::GetNameStringEarthQuakeModel( long nDesignType )
{
	CString strEq(_T(""));
	if(IsBoxType() == FALSE)
	{
		if(m_nSelectModelingCountEQ==2)
		{
			if(nDesignType == 1)
				strEq = _T("단위하중");
			else
				strEq = nDesignType == 2 ? _T("단일모드_좌측토압") : _T("단일모드_우측토압");
		}
		else
		{
			strEq = nDesignType == 1 ? _T("단위하중") : _T("단일모드");
		}
	}
	else
	{
		EGoalOfEarthQuake eGoalEq = GetEarthQuakeGoalByIndex(nDesignType-1);
		strEq = GetStringGoalEarthQuake(eGoalEq, FALSE);
	}

	return strEq;
}

double & CRcBridgeData::GetRateDAMP_EQ_CD()
{
	if (m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
	{
		return m_pARcBridgeDataStd->m_dRateDAMP_EQ_CD;
	}

	return m_dRateDAMP_EQ_CD;
}

void CRcBridgeData::SetRateDAMP_EQ_CD( double dDampRate )
{
	if(m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_pARcBridgeDataStd->m_dRateDAMP_EQ_CD = dDampRate;
	else
		m_dRateDAMP_EQ_CD = dDampRate;
}
