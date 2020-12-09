// WallData.cpp: implementation of the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WallData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWallData::CWallData()
{
	m_nFlag = 0;

	InitWall();

	// 피복 -------------------------------------------------------
	long i = 0; for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverLeft[i]		= 60;	// 벽체 왼쪽 피복
		m_dCoverRight[i]	= 60;	// 벽체 오른쪽 피복
		m_dCoverFrontBack[i]	= 60;	// 벽체 전면후면
	}
	//------------------------------------------------------- 피복 

	// 배근 ----------------------------------------------------------
	m_bDetailInput23DanMainRebar	= FALSE;
	m_bNotRebar						= FALSE;
	m_nTypeRebarFront				= 1;					// 정면도 그리는 타입(0:좌우측기준, 1:중심기준)
	m_nIdxCTC_MainRebar				= 0;
	m_dCTC_MainRebar				= 250;	// index에서 double로 변경(2005.04.25)
	m_bSameOffsetPlaceTAndCBlock	= TRUE;	// 인장 압축구간을 동일 간격으로 배치
	m_bPlacing23DanByStandardOffset	= FALSE;
	m_bPlacingInWallRebarOfCycle24	= TRUE;
	m_nTypeCurveOfVertRebar			= 0;			// 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
	m_bApplyAngleToMainRebarCTC		= FALSE;		// 주철근 CTC에 사각이 적용 되어야 하는지?(상부슬래브가 선형방향 배치라면 적용이 되어야 함)
	m_dRangeSupportRebarForSameUpperSlab	= 0;	// 상부슬래브처럼 꺽을 배력근 범위
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.RemoveAll();
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.Add(CDPoint(0, 0));
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_nCountBlock	= 1;
	m_dDistAddSupportRebarForSameUpperSlab[iLEFT]	= 0;	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	m_dDistAddSupportRebarForSameUpperSlab[iRIGHT]	= 0;	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	m_bRebarLowerSettle[0] = TRUE;
	m_bRebarLowerSettle[1] = TRUE;

	long left = 0; for(left = 0; left < 2; left++)
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_nCountJointMainRebar_Cycle1[left][dan]	= 2;
			m_nCountJointMainRebar_Cycle2[left][dan]	= 2;
		}
	}

	for(left = 0; left < 2; left++)
	{
		m_setRebarDan[left].Init();
	}
		
	m_nIdxCountCycleMainRebar	= 0;			// 주철근 사이클 수.
	m_nCountWrapMainRebar		= 4;
	m_nTypeDefaultCycle3		= 0;				// 3cycle 설정타입

	m_dCTC_SupportRebar			= 300;	// index에서 double로 변경
	m_dCTC_ShearRebar			= 600;

	m_nTypeOutputMainRebar_Side	= 0;
	//---------------------------------------------------------- 배근 

	m_footing.m_exFooting.m_Pile.m_nTypeOrgDefault	= 0;
	m_footing.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;

	m_Fck	= 0;
	m_Fy	= 0;
	m_FyShear	= 0;
	m_dLengthRebarMax	= 8000;
	m_sNameRight		= "좌측";	// 벽체 좌측에 해당하는 이름
	m_sNameLeft			= "우측";	// 벽체 우측에 해당하는 이름
	m_pBasicConcInfo	= NULL;

	m_dCenterOfWall		= 0;				// 도로중심 (저장하지 않음)
	m_tvArrLowerLimit_Front.RemoveAll();
	m_tvArrLowerLimit_FrontRebar.RemoveAll();
}

CWallData::~CWallData()
{
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(long left = 0; left < 2; left++)
		{
			for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				AhTPADelete(&m_pArrRCMain[cycle][left][dan], (CRebarInfoCycle*)0);
			}
		}
	}

	AhTPADelete(&m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	AhTPADelete(&m_pArrRebarMain,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarSupport,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarShear,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarColumn,	(CRebar*)0);
}

void CWallData::Serialize(CArchive &ar)
{
	// 19 : m_tvLowerLimitOfMainRebar
	// 20 : BoxType 추가.
	// 22 : m_bRebarLowerSettle
	// 23 : m_FyShear
	m_nFlag = 23;
	long i(0), left(0), dan(0);

	if(ar.IsStoring())
	{
		ar << m_nFlag;
		m_footing.Serialize(ar);
		// 벽체형상 -------------------------------------------------------
		ar << m_bIs;
		ar << m_xyOrg;
		ar << m_nType;
		ar << m_bRoundRect;		// 라운드식 벽체인지?(현재는 수직인 경우에만 지원)
		ar << m_bSideIsArc;
			// 일반형상
		ar << m_dELB;		// 기둥하단 EL
		ar << m_dW;			// 벽체 너비
		ar << m_dH;			// 벽체 높이(길이)
		ar << m_dAngle;		// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
		ar << m_bApplyAngleEnd;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
			
			// V자형
		ar << m_dELRT;		// V자 우측팔 Top
		ar << m_dELLT;		// V자 좌측팔 Top

		ar << m_dWL;
		ar << m_dWR;
		ar << m_dLR;
		ar << m_dAngleV;		// V자형의 사이각
		ar << m_dR1;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
		ar << m_dR2;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)
		m_Column.Serialize(ar);		// 벽체 단면이 기둥식일 경우 벽체대신 이 기둥을 사용함.
		ar << m_nCountColumn;		// 기둥식일 경우 기둥 개수
		for(i = 0; i < m_nCountColumn; i++)
			ar << m_dDistColumn[i];	// 각 기둥간의 거리
		ar << m_bRevDirPlacingColumn;		// 기둥 배치 방향이 패스의 반대방향인지...
		ar << m_dAnglePath;

			// 평면형상
		ar << m_bTopIsColumn;			// 평면이 기둥식인지...?
		ar << m_tvPathPlan;			// 평면상에 벽체가 뻗는 길.
		ar << m_dMarginLeft;			// 벽체가 path에서 끝위치에서 이격되는 거리
		ar << m_dMarginRight;			// 벽체가 path에서 시작위치에서 이격되는 거리
		ar << m_bRAngRight;			// 우측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
		ar << m_bRAngLeft;			// 좌측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
		ar << m_vAngLeft;				// 좌측단 각도(선형안쓰면 0도)
		ar << m_vAngRight;				// 우측단 각도(선형안쓰면 0도)
		ar << m_dOffsetStt;				// Path시작부 직각적용시 안쪽으로 들어갈 거리
		ar << m_dOffsetEnd;				// Path끝부 직각적용시 안쪽으로 들어갈 거리

			// 좌측면도 상부 형상
		m_tvArrLeftUpper.Serialize(ar);	// 좌측면도 상부 슬래브 형상
		m_tvArrLeftUpperSlab.Serialize(ar);
		ar << m_dThickUppeSlab;			// 상부슬래브 두께.
		ar << m_dThickHunch;			// 상부슬래브 헌치 두께.
		ar << m_dThickLowerSlab;		// 하부슬래브 두께.
		ar << m_dThichLowerHunch;		// 하부슬래브 헌치 두께.

		// ------------------------------------------------------ 벽체형상		

		// 피복 -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar << m_dCoverLeft[i];	// 벽체 왼쪽 피복
			ar << m_dCoverRight[i];	// 벽체 오른쪽 피복
			ar << m_dCoverFrontBack[i];
		}
		//------------------------------------------------------- 피복 

		// 배근 ----------------------------------------------------------
		ar << m_bDetailInput23DanMainRebar;
		ar << m_bNotRebar;
		ar << m_nIdxCTC_MainRebar;
		ar << m_dCTC_MainRebar;	// index에서 double로 변경(2005.04.25)
		ar << m_bSameOffsetPlaceTAndCBlock;	// 인장 압축구간을 동일 간격으로 배치
		ar << m_bPlacing23DanByStandardOffset;
		ar << m_bPlacingInWallRebarOfCycle24;	// 2,4사이클에 대해서 철근을 생성할지...??
		ar << m_nTypeCurveOfVertRebar;			// 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
		ar << m_bApplyAngleToMainRebarCTC;		// 주철근 CTC에 사각이 적용 되어야 하는지?(상부슬래브가 선형방향 배치라면 적용이 되어야 함)
		ar << m_dRangeSupportRebarForSameUpperSlab;	// 상부슬래브처럼 꺽을 배력근 범위
		m_placeByBlock_SupportRebarAddSameUpperSlab.Serialize(ar);	// 상부슬래브 처럼 꺽을 배력근이 추가될 범위
		for(left = 0; left < 2; left++)
		{
			ar << m_dDistAddSupportRebarForSameUpperSlab[left];	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
		}
		ar << m_bRebarLowerSettle[0];
		ar << m_bRebarLowerSettle[1];
		for(left = 0; left < 2; left++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				ar << m_nCountJointMainRebar_Cycle1[left][dan];
				ar << m_nCountJointMainRebar_Cycle2[left][dan];
			}
		}
		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					SerializeDoubleArray(ar, &m_dArrDistRC[cycle][left][dan]);
					SerializeDoubleArray(ar, &m_dArrDiaRC[cycle][left][dan]);
					SerializeDoubleArray(ar, &m_dArrExistRC[cycle][left][dan]);
				}
			}
		}

		for(left = 0; left < 2; left++)
		{
			m_setRebarDan[left].Serialize(ar);
		}
	
		ar << m_tvUpperLimitOfMainRebar ;	// 축방향 주철근의 상부 한계(RcBridge에서는 상부슬래브 상면 주철근 1단의 좌표를 의미)
		ar << m_nIdxCountCycleMainRebar;			// 주철근 사이클 수.
		ar << m_nCountWrapMainRebar;
		ar << m_nTypeDefaultCycle3;				// 3cycle 설정타입

		ar << m_dCoverLower_LowerSlab;
		ar << m_tvLowerLimitOfMainRebar;
		ar << m_tvLowerLimitOfSupportRebar;

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_placeByBlock_SupportRebar[left][dan].Serialize(ar);
			}
		}
		m_placeByBlock_ShearRebar.Serialize(ar);
		ar << m_nIdxCTC_SupportAndShearRebar;
		ar << m_dCTC_SupportRebar;	// index에서 double로 변경
		ar << m_dCTC_ShearRebar;
		m_rebarShearDetail.Serialize(ar);
		ar << m_nTypeOutputMainRebar_Side;
		//---------------------------------------------------------- 배근 
		ar << m_Fck;
		ar << m_Fy;
		ar << m_FyShear;
		ar << m_dLengthRebarMax;

		AhTPASerial(ar, &m_pArrRebarMain, (CRebar*)0);	// 주철근
		AhTPASerial(ar, &m_pArrRebarSupport, (CRebar*)0);	// 배력철근
		AhTPASerial(ar, &m_pArrRebarShear, (CRebar*)0);	// 전단철근
		AhTPASerial(ar, &m_pArrRebarColumn, (CRebar*)0);	// 기타철근

		// 속도개선용
		ar << m_xyOrgOld;
	}
	else
	{
		ar >> m_nFlag;
		m_footing.Serialize(ar);
		// 벽체형상 -------------------------------------------------------
		ar >> m_bIs;
		ar >> m_xyOrg;
		ar >> m_nType;
		if(m_nFlag > 5)
			ar >> m_bRoundRect;		// 라운드식 벽체인지?(현재는 수직인 경우에만 지원)
		if(m_nFlag > 14)
			ar >> m_bSideIsArc;
			// 일반형상
		ar >> m_dELB;		// 기둥하단 EL
		ar >> m_dW;			// 벽체 너비
		ar >> m_dH;			// 벽체 높이(길이)
		ar >> m_dAngle;		// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
		ar >> m_bApplyAngleEnd;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
			
			// V자형
		ar >> m_dELRT;		// V자 우측팔 Top
		ar >> m_dELLT;		// V자 좌측팔 Top

		ar >> m_dWL;
		ar >> m_dWR;
		ar >> m_dLR;
		ar >> m_dAngleV;		// V자형의 사이각
		ar >> m_dR1;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
		ar >> m_dR2;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)
		m_Column.Serialize(ar);		// 벽체 단면이 기둥식일 경우 벽체대신 이 기둥을 사용함.
		ar >> m_nCountColumn;		// 기둥식일 경우 기둥 개수
		for(i = 0; i < m_nCountColumn; i++)
			ar >> m_dDistColumn[i];	// 각 기둥간의 거리
		ar >> m_bRevDirPlacingColumn;		// 기둥 배치 방향이 패스의 반대방향인지...
		ar >> m_dAnglePath;			// 업데이트이후추가

			// 평면형상
		ar >> m_bTopIsColumn;			// 평면이 기둥식인지...?
		ar >> m_tvPathPlan;		// 평면상에 벽체가 뻗는 길.
		ar >> m_dMarginLeft;			// 벽체가 path에서 끝위치에서 이격되는 거리
		ar >> m_dMarginRight;			// 벽체가 path에서 시작위치에서 이격되는 거리
		ar >> m_bRAngRight;			// Path의 시작부에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
		ar >> m_bRAngLeft;			// path의 끝부에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
		ar >> m_vAngLeft;				// 좌측단 각도(선형안쓰면 0도)
		ar >> m_vAngRight;				// 우측단 각도(선형안쓰면 0도)
		ar >> m_dOffsetStt;				// Path시작부 직각적용시 안쪽으로 들어갈 거리
		ar >> m_dOffsetEnd;				// Path끝부 직각적용시 안쪽으로 들어갈 거리

			// 좌측면도 상부 형상
		m_tvArrLeftUpper.Serialize(ar);	// 좌측면도 상부 슬래브 형상
		if(m_nFlag > 7)
			m_tvArrLeftUpperSlab.Serialize(ar);
		ar >> m_dThickUppeSlab;			// 상부슬래브 두께.
		if(m_nFlag > 2)
			ar >> m_dThickHunch;			// 상부슬래브 헌치 두께.
		if(m_nFlag > 19)
		{
			ar >> m_dThickLowerSlab;		// 하부슬래브 두께.
			ar >> m_dThichLowerHunch;		// 하부슬래브 헌치 두꼐.
		}

		// ------------------------------------------------------ 벽체형상		

		// 피복 -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar >> m_dCoverLeft[i];	// 벽체 왼쪽 피복
			ar >> m_dCoverRight[i];	// 벽체 오른쪽 피복
			if(m_nFlag > 17)
				ar >> m_dCoverFrontBack[i];
			else
				m_dCoverFrontBack[i]	= m_dCoverLeft[i];
		}
		//------------------------------------------------------- 피복 

		// 배근 ----------------------------------------------------------
		if(m_nFlag > 16)
			ar >> m_bDetailInput23DanMainRebar;
		if(m_nFlag > 13)
			ar >> m_bNotRebar;
		ar >> m_nIdxCTC_MainRebar;
		if(m_nFlag > 0)
			ar >> m_dCTC_MainRebar;	// index에서 double로 변경(2005.04.25)
		ar >> m_bSameOffsetPlaceTAndCBlock;		// 인장 압축구간을 동일 간격으로 배치
		if(m_nFlag > 11)
			ar >> m_bPlacing23DanByStandardOffset;
		ar >> m_bPlacingInWallRebarOfCycle24;	// 2,4사이클에 대해서 철근을 생성할지...?? 시리얼추가
		ar >> m_nTypeCurveOfVertRebar;			// 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
		if(m_nFlag > 3)
			ar >> m_bApplyAngleToMainRebarCTC;		// 주철근 CTC에 사각이 적용 되어야 하는지?(상부슬래브가 선형방향 배치라면 적용이 되어야 함)
		if(m_nFlag > 9)
			ar >> m_dRangeSupportRebarForSameUpperSlab;	// 상부슬래브처럼 꺽을 배력근 범위
		if(m_nFlag > 15)
		{
			m_placeByBlock_SupportRebarAddSameUpperSlab.Serialize(ar);
			for(left = 0; left < 2; left++)
			{
				ar >> m_dDistAddSupportRebarForSameUpperSlab[left];	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
			}
		}
		if(m_nFlag > 20)
			ar >> m_bRebarLowerSettle[0];
		if(m_nFlag > 21)
			ar >> m_bRebarLowerSettle[1];

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				if(m_nFlag > 16 || dan == 0)
				{
					ar >> m_nCountJointMainRebar_Cycle1[left][dan];
					if(m_nFlag > 8)
						ar >> m_nCountJointMainRebar_Cycle2[left][dan];
				}
			}
		}

		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					if(m_nFlag > 16 || dan == 0)
					{
						SerializeDoubleArray(ar, &m_dArrDistRC[cycle][left][dan]);
						SerializeDoubleArray(ar, &m_dArrDiaRC[cycle][left][dan]);
						if(m_nFlag > 8)
							SerializeDoubleArray(ar, &m_dArrExistRC[cycle][left][dan]);
					}
				}
			}
		}

		for(left = 0; left < 2; left++)
		{
			m_setRebarDan[left].Serialize(ar);
		}

		ar >> m_tvUpperLimitOfMainRebar ;			// 축방향 주철근의 상부 한계(RcBridge에서는 상부슬래브 상면 주철근 1단의 좌표를 의미)
		ar >> m_nIdxCountCycleMainRebar;			// 주철근 사이클 수.
		ar >> m_nCountWrapMainRebar;
		if(m_nFlag > 1)
			ar >> m_nTypeDefaultCycle3;				// 3cycle 설정타입

		if(m_nFlag > 18)
		{
			ar >> m_dCoverLower_LowerSlab;
			ar >> m_tvLowerLimitOfMainRebar;
			ar >> m_tvLowerLimitOfSupportRebar;
		}

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_placeByBlock_SupportRebar[left][dan].Serialize(ar);
			}
		}
		m_placeByBlock_ShearRebar.Serialize(ar);
		ar >> m_nIdxCTC_SupportAndShearRebar;
		if(m_nFlag > 0)
			ar >> m_dCTC_SupportRebar;	// index에서 double로 변경
		if(m_nFlag > 12)
			ar >> m_dCTC_ShearRebar;
		else
			m_dCTC_ShearRebar	= m_dCTC_SupportRebar;
		if(m_nFlag > 6)
			m_rebarShearDetail.Serialize(ar);
		if(m_nFlag > 4)
			ar >> m_nTypeOutputMainRebar_Side;
		//---------------------------------------------------------- 배근 
		ar >> m_Fck;
		ar >> m_Fy;
		if(m_nFlag >22)
			ar >> m_FyShear;
		else
			m_FyShear = m_Fy;

		ar >> m_dLengthRebarMax;

		AhTPASerial(ar, &m_pArrRebarMain, (CRebar*)0);	// 주철근
		AhTPASerial(ar, &m_pArrRebarSupport, (CRebar*)0);	// 배력철근
		AhTPASerial(ar, &m_pArrRebarShear, (CRebar*)0);	// 전단철근
		AhTPASerial(ar, &m_pArrRebarColumn, (CRebar*)0);	// 기타철근

		if(m_nFlag > 10)
			ar >> m_xyOrgOld;
	}

	CheckSerialize();
}

CWallData& CWallData::operator=(const CWallData& Obj)
{
	long i(0), dan(0);
	m_tvArrLowerLimit_Front	= Obj.m_tvArrLowerLimit_Front;
	m_tvArrLowerLimit_FrontRebar	= Obj.m_tvArrLowerLimit_FrontRebar;
	// 벽체형상 -------------------------------------------------------
	m_footing	= Obj.m_footing;
	m_bIs		= Obj.m_bIs;
	m_xyOrg		= Obj.m_xyOrg;
	m_nType		= Obj.m_nType;
	m_bRoundRect	= Obj.m_bRoundRect;		// 라운드식 벽체인지?(현재는 수직인 경우에만 지원)
	m_bSideIsArc	= Obj.m_bSideIsArc;
		// 일반형상
	m_dDiffTop[0]	= Obj.m_dDiffTop[0];
	m_dDiffTop[1]	= Obj.m_dDiffTop[1];
	m_dELBDiff	= Obj.m_dELBDiff;
	m_dELB		= Obj.m_dELB;		// 기둥하단 EL
	m_dW		= Obj.m_dW;			// 벽체 너비
	m_dH		= Obj.m_dH;			// 벽체 높이(길이)
	m_dAngle	= Obj.m_dAngle;		// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
	m_bApplyAngleEnd	= Obj.m_bApplyAngleEnd;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
		
		// V자형
	m_dELRT		= Obj.m_dELRT;		// V자 우측팔 Top
	m_dELLT		= Obj.m_dELLT;		// V자 좌측팔 Top

	m_dWL		= Obj.m_dWL;
	m_dWR		= Obj.m_dWR;
	m_dLR		= Obj.m_dLR;
	m_dAngleV	= Obj.m_dAngleV;		// V자형의 사이각
	m_dR1		= Obj.m_dR1;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
	m_dR2		= Obj.m_dR2;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)

	// 기둥식의 시작위치는 벽체의 시작위치에서 떨어진 거리를 가지고 판단함.
	m_Column		= Obj.m_Column;		// 벽체 단면이 기둥식일 경우 벽체대신 이 기둥을 사용함.
	m_nCountColumn	= Obj.m_nCountColumn;	// 기둥식일 경우 기둥 개수
	for(i = 0; i < MAX_COLUMN; i++)
		m_dDistColumn[i]	= Obj.m_dDistColumn[i];		// 각 기둥간의 거리
	m_bRevDirPlacingColumn	= Obj.m_bRevDirPlacingColumn;		// 기둥 배치 방향이 패스의 반대방향인지...
	m_dAnglePath			= Obj.m_dAnglePath;
		
	// 평면형상
	m_bTopIsColumn	= Obj.m_bTopIsColumn;			// 평면이 기둥식인지...?
	m_tvPathPlan	= Obj.m_tvPathPlan;		// 평면상에 벽체가 뻗는 길.
	m_dMarginLeft	= Obj.m_dMarginLeft;			// 벽체가 path에서 끝위치에서 이격되는 거리
	m_dMarginRight	= Obj.m_dMarginRight;			// 벽체가 path에서 시작위치에서 이격되는 거리
	m_bRAngLeft		= Obj.m_bRAngLeft;			// 좌측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	m_bRAngRight	= Obj.m_bRAngRight;			// 우측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	m_vAngLeft		= Obj.m_vAngLeft;				// 좌측단 각도(선형안쓰면 0도)
	m_vAngRight		= Obj.m_vAngRight;				// 우측단 각도(선형안쓰면 0도)
	m_dOffsetStt	= Obj.m_dOffsetStt;				// Path시작부 직각적용시 안쪽으로 들어갈 거리
	m_dOffsetEnd	= Obj.m_dOffsetEnd;				// Path끝부 직각적용시 안쪽으로 들어갈 거리

	// 좌측면도 상부 형상
	m_tvArrLeftUpper	= Obj.m_tvArrLeftUpper;		// 좌측면도 상부 슬래브 형상
	m_tvArrLeftUpperSlab	= Obj.m_tvArrLeftUpperSlab;
	m_dDiffHLeftByCenter	= Obj.m_dDiffHLeftByCenter;		// 벽체상면 중심과 좌측과의 차이
	m_dDiffHRightByCenter	= Obj.m_dDiffHRightByCenter;		// 벽체상면 중심과 우측과의 차이

	m_dThickUppeSlab	= Obj.m_dThickUppeSlab;		// 상부슬래브 두께.
	m_dThickHunch		= Obj.m_dThickHunch;		// 상부슬래브 헌치 두께.
	m_dThickHunchL		= Obj.m_dThickHunchL;				// 상부슬래브 헌치 두께.좌측
	m_dThickHunchR		= Obj.m_dThickHunchR;				// 상부슬래브 헌치 두께.우측

	m_dThickLowerSlab	= Obj.m_dThickLowerSlab;	// 하부슬래브 두께.
	m_dThichLowerHunch  = Obj.m_dThichLowerHunch;	// 하부슬래브 헌치 두께.
	// ------------------------------------------------------ 벽체형상

	// 피복 -------------------------------------------------------
	for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverLeft[i]	= Obj.m_dCoverLeft[i];	// 벽체 왼쪽 피복
		m_dCoverRight[i]	= Obj.m_dCoverRight[i];	// 벽체 오른쪽 피복
		m_dCoverFrontBack[i]	= Obj.m_dCoverFrontBack[i];
	}
	//------------------------------------------------------- 피복 

	// 배근 ----------------------------------------------------------
	m_bDetailInput23DanMainRebar	= Obj.m_bDetailInput23DanMainRebar;
	m_bNotRebar						= Obj.m_bNotRebar;
	m_nTypeRebarFront				= Obj.m_nTypeRebarFront;					// 정면도 그리는 타입(0:좌우측기준, 1:중심기준)
	m_nIdxCTC_MainRebar				= Obj.m_nIdxCTC_MainRebar;
	m_dCTC_MainRebar				= Obj.m_dCTC_MainRebar;	// index에서 double로 변경(2005.04.25)
	m_bSameOffsetPlaceTAndCBlock	= Obj.m_bSameOffsetPlaceTAndCBlock;	// 인장 압축구간을 동일 간격으로 배치
	m_bPlacing23DanByStandardOffset	= Obj.m_bPlacing23DanByStandardOffset;
	m_bPlacingInWallRebarOfCycle24	= Obj.m_bPlacingInWallRebarOfCycle24;	// 2,4사이클에 대해서 철근을 생성할지...??
	m_nTypeCurveOfVertRebar			= Obj.m_nTypeCurveOfVertRebar;			// 수직철근 절곡부 타입(0 : 자동, 1 : 곡선, 2 : 직각)
	m_bApplyAngleToMainRebarCTC		= Obj.m_bApplyAngleToMainRebarCTC;		// 주철근 CTC에 사각이 적용 되어야 하는지?(상부슬래브가 선형방향 배치라면 적용이 되어야 함)
	m_dRangeSupportRebarForSameUpperSlab	= Obj.m_dRangeSupportRebarForSameUpperSlab;	// 상부슬래브처럼 꺽을 배력근 범위
	m_placeByBlock_SupportRebarAddSameUpperSlab	= Obj.m_placeByBlock_SupportRebarAddSameUpperSlab;
	long left = 0;
	for(left = 0; left < 2; left++)
	{
		m_dDistAddSupportRebarForSameUpperSlab[left]	= Obj.m_dDistAddSupportRebarForSameUpperSlab[left];	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	}

	for(left = 0; left < 2; left++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_nCountJointMainRebar_Cycle1[left][dan]	= Obj.m_nCountJointMainRebar_Cycle1[left][dan];
			m_nCountJointMainRebar_Cycle2[left][dan]	= Obj.m_nCountJointMainRebar_Cycle2[left][dan];
		}
		m_bRebarLowerSettle[left] = Obj.m_bRebarLowerSettle[left];
	}

	long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				AhTPACopy(&m_pArrRCMain[cycle][left][dan], &Obj.m_pArrRCMain[cycle][left][dan], (CRebarInfoCycle*)0);	// 좌우측면 rebar info cycle
		}
	}
	

	for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_dArrDistRC[cycle][left][dan]	= Obj.m_dArrDistRC[cycle][left][dan];
				m_dArrDiaRC[cycle][left][dan]	= Obj.m_dArrDiaRC[cycle][left][dan];
				m_dArrExistRC[cycle][left][dan]	= Obj.m_dArrExistRC[cycle][left][dan];
			}
		}
	}
	for(left = 0; left < 2; left++)
	{
		m_setRebarDan[left]	= Obj.m_setRebarDan[left];
	}
	
	m_dCoverUpper_UpperSlab		= Obj.m_dCoverUpper_UpperSlab;
	m_tvUpperLimitOfMainRebar	= Obj.m_tvUpperLimitOfMainRebar ;	// 축방향 주철근의 상부 한계(RcBridge에서는 상부슬래브 상면 주철근 1단의 좌표를 의미)
	m_nIdxCountCycleMainRebar	= Obj.m_nIdxCountCycleMainRebar;	// 주철근 사이클 수.
	m_nCountWrapMainRebar		= Obj.m_nCountWrapMainRebar;
	m_nTypeDefaultCycle3		= Obj.m_nTypeDefaultCycle3;				// 3cycle 설정타입

	m_dCoverLower_LowerSlab		= Obj.m_dCoverLower_LowerSlab;
	m_tvLowerLimitOfMainRebar	= Obj.m_tvLowerLimitOfMainRebar;
	m_tvLowerLimitOfSupportRebar	= Obj.m_tvLowerLimitOfSupportRebar;

	for(left = 0; left < 2; left++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_placeByBlock_SupportRebar[left][dan]	= Obj.m_placeByBlock_SupportRebar[left][dan];
		}
	}
	m_placeByBlock_ShearRebar		= Obj.m_placeByBlock_ShearRebar;
	m_nIdxCTC_SupportAndShearRebar	= Obj.m_nIdxCTC_SupportAndShearRebar;
	m_dCTC_SupportRebar				= Obj.m_dCTC_SupportRebar;	// index에서 double로 변경
	m_dCTC_ShearRebar				= Obj.m_dCTC_ShearRebar;
	m_rebarShearDetail				= Obj.m_rebarShearDetail;
	m_nTypeOutputMainRebar_Side		= Obj.m_nTypeOutputMainRebar_Side;	

	// 철근 집계
	AhTPACopy(&m_pArrRebarMain, &Obj.m_pArrRebarMain, (CRebar*)0);			// 주철근
	AhTPACopy(&m_pArrRebarSupport, &Obj.m_pArrRebarSupport, (CRebar*)0);	// 배력철근
	AhTPACopy(&m_pArrRebarShear, &Obj.m_pArrRebarShear, (CRebar*)0);		// 전단철근
	AhTPACopy(&m_pArrRebarColumn, &Obj.m_pArrRebarColumn, (CRebar*)0);		// 기둥부 철근	
	//---------------------------------------------------------- 배근 
	m_Fck					= Obj.m_Fck;
	m_Fy					= Obj.m_Fy;
	m_FyShear				= Obj.m_FyShear;
	m_dLengthRebarMax		= Obj.m_dLengthRebarMax;

	return *this;
}

// 벽체 형상 초기화
void CWallData::InitWall()
{
	long i = 0;

	m_dWidthExp[0]	= 0;
	m_dWidthExp[1]	= 0;
	m_dWidthOverRebarExp[0] = 0;
	m_dWidthOverRebarExp[1] = 0;
	m_dAngleUpperSlab_Front	= 0;
	m_dAngleUpperWall_Front	= 0;

	// 벽체형상 -------------------------------------------------------
	m_bIs		= TRUE;
	m_xyOrg		= CDPoint(0, 0);
	m_nType		= WALL_TYPE_VER;
	m_bRoundRect	= FALSE;		// 라운드식 벽체인지?(현재는 수직인 경우에만 지원)
	m_bSideIsArc	= FALSE;

		// 일반형상
	m_dDiffTop[0]	= 0;
	m_dDiffTop[1]	= 0;
	m_dELBDiff	= 0;
	m_dELB		= 0;			// 기둥하단 EL
	m_dW		= 0;			// 벽체 너비
	m_dH		= 0;			// 벽체 높이(길이)
	m_dAngle	= 225;			// 벽체가 기울어진 각도(일반형상 : 190 ~ 350까지 지원)
	m_bApplyAngleEnd	= TRUE;	// 벽체가 기울어 졌을때 끝부분 기초에 맞춰서 직각처리 할지..
		
		// V자형
	m_dELRT		= 0;			// V자 우측팔 Top
	m_dELLT		= 0;			// V자 좌측팔 Top

	m_dWL		= 0;
	m_dWR		= 0;
	m_dLR		= 0;
	m_dAngleV	= 0;			// V자형의 사이각
	m_dR1		= 0;			// V자형에서 왼쪽 팔 반지름(곡선지원은 나중에)
	m_dR2		= 0;			// V자형에서 오른쪽 팔 반지름(곡선지원은 나중에)

		// 평면형상
	// 기둥식의 시작위치는 벽체의 시작위치에서 떨어진 거리를 가지고 판단함.
	m_Column.m_bIs = TRUE;				// 벽체 단면이 기둥식일 경우 벽체대신 이 기둥을 사용함.
	m_nCountColumn	= 3;				// 기둥식일 경우 기둥 개수
	for(i = 0; i < MAX_COLUMN; i++)
		m_dDistColumn[i] = 3000;		// 각 기둥간의 거리
	m_bRevDirPlacingColumn	= TRUE;		// 기둥 배치 방향이 패스의 반대방향인지...
	m_dAnglePath	= 90;

	m_bTopIsColumn	= FALSE;			// 평면이 기둥식인지...?
	m_dMarginLeft	= 0;				// 벽체가 path에서 끝위치에서 이격되는 거리
	m_dMarginRight	= 0;				// 벽체가 path에서 시작위치에서 이격되는 거리
	m_bRAngLeft = FALSE;				// 좌측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	m_bRAngRight = FALSE;				// 우측단에 직각적용 할지?(현재 안쓰는 거지만 혹시나 해서 만들어 놓음)
	m_vAngLeft	= CDPoint(1, 0);		// 좌측단 각도(선형안쓰면 0도)
	m_vAngRight	= CDPoint(1, 0);		// 우측단 각도(선형안쓰면 0도)
	m_dOffsetStt	= 0;				// Path시작부 직각적용시 안쪽으로 들어갈 거리
	m_dOffsetEnd	= 0;				// Path끝부 직각적용시 안쪽으로 들어갈 거리

		// 좌측면도 상부 형상
	m_tvArrLeftUpper.RemoveAll();		// 좌측면도 상부 슬래브 형상
	m_tvArrLeftUpperSlab.RemoveAll();
	m_dDiffHLeftByCenter	= 0;		// 벽체상면 중심과 좌측과의 차이
	m_dDiffHRightByCenter	= 0;		// 벽체상면 중심과 우측과의 차이

	m_dThickUppeSlab	= 0;			// 상부슬래브 두께.
	m_dThickHunch		= 0;			// 상부슬래브 헌치 두께.
	m_dThickHunchL		= 0;				// 상부슬래브 헌치 두께.좌측
	m_dThickHunchR		= 0;				// 상부슬래브 헌치 두께.우측

	m_dThickLowerSlab	= 0;			//하부슬래브 두께.
	m_dThichLowerHunch	= 0;			//하부슬래브 헌치 두께.
	// ------------------------------------------------------ 벽체형상

	m_tvArrRebarMainSideForDim.RemoveAll();
}

// 배력철근 배근 입력내용 복사
void CWallData::CopyPlaceRebar_SupportRebar(CWallData *pWall)
{
	if(!pWall) return;

	long left = 0; for(left = 0; left < 2; left++)
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			m_placeByBlock_SupportRebar[left][dan]	= pWall->m_placeByBlock_SupportRebar[left][dan];
	}
}

void CWallData::CopyPlaceRebar_ShearRebar(CWallData *pWall)
{
	if(!pWall) return;

	m_placeByBlock_ShearRebar	= pWall->m_placeByBlock_ShearRebar;
}

// 벽체 피복(좌우측은 입력 받지만, 앞뒤는 입력받지 않음)
double CWallData::GetCoverFront(BOOL bFront, long nDan)
{
	if(nDan < 0 || nDan >= MAX_REBAR_DAN) nDan = 0;
	return m_dCoverFrontBack[nDan];
}

// 벽체 피복(좌우측은 입력 받지만, 상하는 입력받지 않음)
// BOOL bApplyVertForSlope : 경사형 벽체를 세웠을때의 피복
double CWallData::GetCoverUpper(BOOL bUpper, long nDan, BOOL bApplyVertForSlope)
{
	if(nDan < 0 || nDan >= MAX_REBAR_DAN) nDan = 0;
	double dCover	= bUpper ? m_dCoverUpper_UpperSlab : m_footing.m_dCoverLower[nDan];

	if(!bUpper && !m_footing.m_bIs && !m_footing.IsVirtualFooting())
	{
		dCover = m_dCoverLower_LowerSlab;
	}
	if(bApplyVertForSlope && m_nType == WALL_TYPE_SLOPE)
	{
		double dA	= m_dAngle < 90 ?  90 - m_dAngle : m_dAngle - 90;
		dA	= 90 - dA;
		dCover	= dCover / sin(ToRadian(dA));
	}

	return dCover;
}

// 주철근에 대해서 마크 리턴함.
// bInit 이면 초기화만 한다.
// 그렇지 않은 경우 증가된 값을 리턴
/*// 철근마크 리턴을 위한 부위 정의 ///////////////////////////////
#define MARK_LEFT	0	// 좌측 철근 마크
#define MARK_RIGHT	1	// 우측 철근 마크
/////////////////////////////////////////////////////////////////*/
CString CWallData::GetMarkRebarMain(long nType, BOOL bInput, long nCycle, BOOL bOnlyInit, BOOL bForBM, CString sDescription, long nDan)
{
	static long nNum	= 1;
	if(bOnlyInit) 
	{
		nNum		= 1;
		return _T("");
	}

	CString sMark		= _T("");
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;

		long i = 0; for(i = 0; i < m_pArrRebarMain.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarMain.GetAt(i);
			if(pRB->m_sDescription == sDescription)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}
	
	switch(nType)
	{
	case MARK_LEFT:		sMark	= bInput ? "WL" : "IW"; break;
	case MARK_RIGHT:	sMark	= bInput ? "WR" : "IW"; break;
	default: break;
	}

	if(!bInput || nDan==0)
	{
		sMark	= sMark + COMMA(nNum);
		nNum++;
	}

	return sMark;
}


long CWallData::GetCountCycleMainRebar()
{
	long nCountCycle	= 1;
	if(m_nIdxCountCycleMainRebar == 0) nCountCycle	= 2;
	else if(m_nIdxCountCycleMainRebar == 1) nCountCycle	= 4;
	
	return nCountCycle;
}

// 이 함수는 CHgBaseBM에서 가져온 함수다
// CHgBaseBM이 엔진으로 승격되는날 이 함수는 제거되고 CHgBaseBM에서 직접 가져다 쓰게 될 것이다.
CString CWallData::GetCircleText(long nNumber)
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

	if(nNumber <= 0 || nNumber >= nCount) return "";
	return sCircleText[nNumber-1];
}

CString CWallData::GetMarkRebarColumn(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;
		long i = 0; for(i = 0; i < m_pArrRebarColumn.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarColumn.GetAt(i);
			if(pRB->m_sDescription == sDescription)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("C");	
	
	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}
// serialize를 한뒤 혹시나 flag나 버전업 등의 이유로 올바르지 못한 데이타가 들어오면 
// 에러 메시지를 보낸뒤, 초기화를 해서라도 바로 잡을수 있는건 바로 잡아 준다.
// 특히 철근 집계같은거..
void CWallData::CheckSerialize()
{

}

// long nBaseNum == -1 이면 이전값을 그대로 사용
// -1이 아니면 nBaseNum값으로 셋팅만 한뒤 리턴
CString CWallData::GetMarkRebarSupport(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") 
		{
			if(nType > -1 && nType < m_pArrRebarSupport.GetSize())
			{
				CRebar *pRB	= m_pArrRebarSupport.GetAt(nType);
				sMark	= pRB->m_sMark;
			}
			return sMark;
		}
		long i = 0; for(i = 0; i < m_pArrRebarSupport.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarSupport.GetAt(i);
			if(pRB->m_sDescription.Find(sDescription, 0) != -1)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("");	
	
	switch(nType)
	{
		case W_REBAR_S_L1 : sMark	= bInput ? "L1" : "D"; break;	// 배력철근 좌측 1단.
		case W_REBAR_S_L2 : sMark	= bInput ? "L2" : "D"; break;	// 배력철근 좌측 2단.
		case W_REBAR_S_L3 : sMark	= bInput ? "L3" : "D"; break;	// 배력철근 좌측 3단.
		case W_REBAR_S_R1 : sMark	= bInput ? "R1" : "D"; break;	// 배력철근 우측 1단.
		case W_REBAR_S_R2 : sMark	= bInput ? "R2" : "D"; break;	// 배력철근 우측 2단.
		case W_REBAR_S_R3 : sMark	= bInput ? "R3" : "D"; break;	// 배력철근 우측 3단.
	}

	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}

CString CWallData::GetMarkRebarShear(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;
		long i = 0; for(i = 0; i < m_pArrRebarShear.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarShear.GetAt(i);
			if(pRB->m_sDescription.Find(sDescription, 0) != -1)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("");	
	
	switch(nType)
	{
		case W_REBAR_V : sMark	= bInput ? "V" : "S"; break;	// 전단철근
	}

	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}

void CWallData::SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr)
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
		ar >> nSize;
		dArr->RemoveAll();

		long i = 0; for(i = 0; i < nSize; i++)
		{
			double d	= 0;

			ar >> d;
			dArr->Add(d);
		}
	}
}


// 도로 중심(배근시 기준이 되는 위치)
double CWallData::GetCrossSectionCenterOfRoad(BOOL bLeft)
{
	double dMid	= 0;
	if(!bLeft)
	{
		dMid	= m_dCenterOfWall + m_dCenterOfWall;
	}

	return dMid;
}

BOOL CWallData::GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2)
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

// 각도를 사각 적용했을 경우의 각도를 구해줌
double CWallData::GetAngleByAngle(double dAngle, double dAngleApply)
{
	if(dAngleApply == 90) return dAngle;

	double dLen		= 100;
	CDPoint xyTop	= CDPoint(0, 0);
	CDPoint xyAngle	= xyTop + ToDPointFrDegree(dAngle) * dLen;
	
	CDPoint vAng	= ToDPointFrDegree(dAngleApply);
	xyAngle.x		= xyAngle.x / vAng.y;

	dAngle			= (xyAngle - xyTop).Unit().GetAngleDegree();

	/*
	double dLen		= 100;
	CVector vTop	= CVector(0, 0, 0);
	CVector vLeft	= vTop + ToDPointFrDegree(180) * dLen;
	CVector vRight	= vTop + ToDPointFrDegree(180+dAngle) * dLen;

	// 사각 적용
	double rad	= ToRadian(dAngleApply);
	vLeft.z -= dLen * tan(rad);
	vRight.z += dLen * tan(rad); 

	double dA	= ~(vTop-vLeft);
	double dB	= ~(vTop-vRight);
	double dC	= ~(vLeft-vRight);


	dAngle	= ToDegree(acos(((dA*dA) + (dB*dB) - (dC*dC)) / (2 * dA * dB)));
	*/
	


	return dAngle;
}

// dAngleApply의 각도가 적용된 dAngle을 직으로 적용해서 리턴
double CWallData::GetAngleByVert(double dAngle, double dAngleApply)
{
	if(dAngleApply == 90) return dAngle;

	double dLen		= 100;
	CDPoint xyTop	= CDPoint(0, 0);
	CDPoint xyAngle	= xyTop + ToDPointFrDegree(dAngle) * dLen;
	
	CDPoint vAng	= ToDPointFrDegree(dAngleApply);
	xyAngle.x		= xyAngle.x * vAng.y;

	dAngle			= (xyAngle - xyTop).Unit().GetAngleDegree();

	return dAngle;
}

void CWallData::SaveCountJointRebar()
{
	if(m_nFlag <= 16)
	{
		// 이음 개수
		long left = 0; for(left = 0; left < 2; left++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_nCountJointMainRebar_Cycle1[left][dan]	= m_nCountJointMainRebar_Cycle1_Old[left][dan];	// 외측벽체 시종점,좌우측 이음 개수
				m_nCountJointMainRebar_Cycle2[left][dan]	= m_nCountJointMainRebar_Cycle2_Old[left][dan];	// 외측벽체 시종점,좌우측 이음 개수
			}
		}
	}

}

CRebar* CWallData::GetRebarMainByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarMain.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarMain.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarSupportByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarSupport.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarSupport.GetAt(i);
		// 벽체는 지점번호가 들어가기 때문에 find를 쓴다.
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarShearByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarShear.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarShear.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarColumnByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarColumn.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarColumn.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}
