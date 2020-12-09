// ARcBridgeDrawingOptionData.cpp: implementation of the CARcBridgeDrawingOptionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgedrawingoption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeDrawingOptionData::CARcBridgeDrawingOptionData()
{
	DefaultDrawingOptionVerticalPlane();
	DefaultDrawingOptionGeneral();
	DefaultDrawingOptionRebar();
	DefaultDrawingOptionAps();
	DefaultDrawingOptionCns();
	DefaultDrawingOptionOthers();
	DefaultDrawingOptionShoeArrange();
	DefaultDrawingOptionBoring();
	DefaultDrawingOptionHyunTa();
	DefaultDrawingOptionSlabStandardOutput();
}

CARcBridgeDrawingOptionData::~CARcBridgeDrawingOptionData()
{

}

void CARcBridgeDrawingOptionData::Serialize(CArchive& ar)
{
	// 25 : m_bSepUpperAndLowerFootingRebar;	// 기초 구조도 상면 하면을 분리해서 표현
	//         m_bSepUpperAndLowerSlabRebar;	// 상부슬래브 구조도 상면 하면을 분리해서 표현
	// 26 : m_bRebarTableSepByFy;			// 철근재료표 fy별로 분리해서 표현
	//			m_bRebarTableSepByFyAps;	// 철근재료표 fy별로 분리해서 표현(접속슬래브)
	// 27 : DWORD  m_dwDrawRcStd;
	//		double m_dScaleRcStd1Base;	// 도면1 기본 스케일
	//		double m_dScaleRcStd2Base;	// 도면2 기본 스케일
	//		double m_dScaleRcStd3Base;	// 도면3 기본 스케일
	// 28 : m_dScaleRebarDetailPRF;		// 부상방지저판상세 
	// 29 : m_bSepUpperSlab2DanAll;
	// 30 : m_nTypeHyunTaVol; 
	// 31 : m_bDrawHunchRebarOnly
	// 32 : m_nBasePosPlaneSupportRebarDim
	long nFlag = 32;
	if(ar.IsStoring())
	{
		ar << nFlag;

		// 종평면도
		ar << m_szDxfPlanName;
		ar << m_dwDrawVP;
		ar << m_dScaleVPBase;
		ar << m_dScaleVPCross;
		ar << m_dScaleVPLocation;
		ar << m_bDrawVPCoordinateXY;

		// 일반도
		ar << m_dwDrawGen;
		long i=0; for(i=0; i<4; i++)
		{
			ar << m_bDrawGenWingWall[i][0];
			ar << m_bDrawGenWingWall[i][1];
			ar << m_bDrawGenWingWall[i][2];
		}
		ar << m_bDrawGenCoordinateXY;
		ar << m_bDimTableForPilePlane;	// 평면도 말뚝 간격 테이블 처리
		ar << m_dScaleGenBase;
		ar << m_dScaleGenDetailA;
		ar << m_dScaleGenNotch;
		ar << m_dScaleGenOrder;
		ar << m_dScaleGenWingWall;
		ar << m_dScaleGenDetailB;
		ar << m_dScaleGenConcJoint;
		ar << m_dScaleFXJoint;			// 신축이음 상세 스케일
		ar << m_dScaleGenCrack;		// 균열유발용 상세 스케일

		// 구조도
		ar << m_dwDrawRebar;
		for(i=0; i<4; i++)
		{
			ar << m_bDrawRebarWingWall[i][0];
			ar << m_bDrawRebarWingWall[i][1];
			ar << m_bDrawRebarWingWall[i][2];
			ar << m_bDrawRebarWingWall[i][3];
		}
		ar << m_nTypeMainRebarAssm;				// 주철근 조립도 표현 방법
		ar << m_dScaleRebarBase;
		ar << m_dScaleRebarDetail;				// 상세 스케일
		ar << m_dScaleRebarWingWall;			// 날개벽 스케일
		ar << m_dScaleBMD;					// BMD 스케일
		ar << m_dScaleSFD;					// SFD 스케일
		ar << m_dScaleAFD;
		ar << m_dScaleRebarDetailArchRibSpringging;	// 아치리브 스프링깅부 상세 스케일
		ar << m_dScaleRebarDetailColumnSection;	// 기둥 단면 상세 스케일
		ar << m_dScaleRebarDetailGirderSection;	// 거더 단면 상세 스케일
		ar << m_dScaleRebarDetailSlabAcute;		// 슬래브 둔각부 상세 스케일
		ar << m_dScaleRebarDetailWallObtuse;		// 벽체 예각부 상세 스케일
		ar << m_dScaleRebarDetailFootingAcute;		// 기초 둔각부 상세 스케일
		ar << m_dScaleRebarDetailPRF;				// 부상방지저판 상세 스케일
		ar << m_bSeparateRebarBMWing;			// 날개벽 철근수량 분리 출력
		ar << m_bSeparateRebarBMColumn;			// 보기둥 철근수량 분리 출력
		ar << m_bSeparateRebarBMOther;			// 기타철근 철근수량 분리 출력(방호벽, 교명주받침)
		ar << m_bSeparateOutputRebarBMMain;		// 본체 철근재료표 분리 출력(수량이 많을경우 2장까지 늘어남)
		ar << m_nTypeOutRebarTable;				// 철근재료표 출력방법(0:일체,1:주철근,기타철근구분,2:부위별)
		ar << m_bRebarTableSepByFy;			// 철근재료표 fy별로 분리해서 표현
		ar << m_dSpaceBetweenRebarDetail;		// 상세도간 간격
		ar << m_nCountRowEachTable;				// 철근재료표 테이블당 최대 row수
		ar << m_bMainRebarNumberingInStandardSection;	// 표준단면도에 주철근 넘버링 표기할지?
		ar << m_bLengthInfoInMainRebarNumbering;			// 주철근 조립도 철근 번호에 길이 정보 표현할지?
		ar << m_bExcludeDoupleMainRebarCrossSection;	// 횡단 구조도 출력시 겹치는 주철근 표시 안함(단 치수는 그대로 표현)
		ar << m_bSepFrontAndBackWallRebar;	// 벽체 구조도 전면 배면을 분리 해서 표현
		ar << m_bSepUpperAndLowerFootingRebar;	// 기초 구조도 상면 하면을 분리해서 표현
		ar << m_bSepUpperAndLowerSlabRebar;	// 상부슬래브 구조도 상면 하면을 분리해서 표현
		ar << m_nDirMarkUpperSlabMainRebar;	// 상부슬래브 주철근 구조도 철근 지시선 방향(0 : 시점각도 기준, 1 : 종점각도 기준, 2 : 자동)
		ar << m_bSepUpperSlab2DanAll;
		ar << m_bDrawHunchRebarOnly;
		ar << m_nBasePosPlaneSupportRebarDim;

		// 접속슬래브 구조도
		ar << m_dwDrawRebarAps;
		ar << m_dScaleRebarAps;					// 접속슬래브 구조도 스케일
		ar << m_dScaleDetailAps;				// 상세도 스케일
		ar << m_dSpaceBetweenRebarDetailAps;	// 상세도간 간격
		ar << m_bRebarTableSepByFyAps;	// 철근재료표 fy별로 분리해서 표현(접속슬래브)

		// 완충슬래브 구조도
		ar << m_dwDrawRebarCns;
		ar << m_nTypeCnsRebar;					// 완충슬래브 1, 2 표현방법(0 : 1,2를 함께표현, 1 : 분리해서 표현)
		ar << m_dScaleRebarCns;					// 완충슬래브 구조도 스케일
		ar << m_dSpaceBetweenRebarDetailCns;

		// 현장타설 말뚝
		ar << m_dwDrawHyunTa;
		ar << m_dScaleHyunTaFront;
		ar << m_dScaleHyunTaSection;
		ar << m_dScaleHyunTaDetailHold;
		ar << m_dScaleHyunTaDetailSpac;
		ar << m_nTypeHyunTaCut;
		ar << m_nTypeHyunTaVol;

		// 지질 주상도
		ar << m_dwDrawBoring;
		ar << m_dScaleBoring;

		// 기타 도면
		ar << m_dwDrawOthers;
		ar << m_dScaleRebarOthers;
		ar << m_dScaleDetailGuardWall;
		ar << m_dScaleDetailMonument;
		ar << m_dSpaceBetweenRebarDetailOthers;

		// RC 슬래브교 표준도
		ar << m_dwDrawRcStd;
		ar << m_dScaleRcStd1Base;	// 도면1 기본 스케일
		ar << m_dScaleRcStd2Base;	// 도면2 기본 스케일
		ar << m_dScaleRcStd3Base;	// 도면3 기본 스케일

	}
	else
	{
		ar >> nFlag;

		// 종평면도
		if(nFlag>6)
			ar >> m_szDxfPlanName;
		ar >> m_dwDrawVP;
		ar >> m_dScaleVPBase;
		ar >> m_dScaleVPCross;
		ar >> m_dScaleVPLocation;
		if(nFlag>11)
			ar >> m_bDrawVPCoordinateXY;

		// 일반도
		ar >> m_dwDrawGen;
		if(nFlag>9)
		{
			long i=0; for(i=0; i<4; i++)
			{
				ar >> m_bDrawGenWingWall[i][0];
				ar >> m_bDrawGenWingWall[i][1];
				ar >> m_bDrawGenWingWall[i][2];
			}
			ar >> m_bDrawGenCoordinateXY;
		}
		if(nFlag>10)
			ar >> m_bDimTableForPilePlane;	// 평면도 말뚝 간격 테이블 처리

		ar >> m_dScaleGenBase;
		ar >> m_dScaleGenDetailA;
		ar >> m_dScaleGenNotch;
		ar >> m_dScaleGenOrder;
		ar >> m_dScaleGenWingWall;
		if(nFlag > 4)
			ar >> m_dScaleGenDetailB; 
		if(nFlag > 5)
			ar >> m_dScaleGenConcJoint;
		if(nFlag > 21)
		{
			ar >> m_dScaleFXJoint;			// 신축이음 상세 스케일
			ar >> m_dScaleGenCrack;		// 균열유발용 상세 스케일
		}
		else
		{
			m_dScaleFXJoint	= m_dScaleGenDetailB;
			m_dScaleGenCrack	= m_dScaleGenConcJoint;
		}

		// 구조도
		ar >> m_dwDrawRebar;
		if(nFlag>9)
		{
			long i=0; for(i=0; i<4; i++)
			{
				ar >> m_bDrawRebarWingWall[i][0];
				ar >> m_bDrawRebarWingWall[i][1];
				ar >> m_bDrawRebarWingWall[i][2];
				if(nFlag > 16) ar >> m_bDrawRebarWingWall[i][3];
			}
		}
		ar >> m_nTypeMainRebarAssm;					// 주철근 조립도 표현 방법
		ar >> m_dScaleRebarBase;
		ar >> m_dScaleRebarDetail;					// 상세 스케일
		ar >> m_dScaleRebarWingWall;				// 날개벽 스케일
		if(nFlag > 2)
		{
			ar >> m_dScaleBMD;						// BMD 스케일
			ar >> m_dScaleSFD;						// SFD 스케일
		}
		if(nFlag > 18)
			ar >> m_dScaleAFD;

		if(nFlag > 19)
		{
			ar >> m_dScaleRebarDetailArchRibSpringging;	// 아치리브 스프링깅부 상세 스케일
			ar >> m_dScaleRebarDetailColumnSection;	// 기둥 단면 상세 스케일
			ar >> m_dScaleRebarDetailGirderSection;	// 거더 단면 상세 스케일
			ar >> m_dScaleRebarDetailSlabAcute;		// 슬래브 둔각부 상세 스케일
			ar >> m_dScaleRebarDetailWallObtuse;		// 벽체 예각부 상세 스케일
			ar >> m_dScaleRebarDetailFootingAcute;		// 기초 둔각부 상세 스케일
		}
		if(nFlag > 27)
			ar >> m_dScaleRebarDetailPRF;			// 부상방지저판 상세 스케일
		if(nFlag > 0)
		{
			ar >> m_bSeparateRebarBMWing;			// 날개벽 철근수량 분리 출력
			ar >> m_bSeparateRebarBMColumn;			// 보기둥 철근수량 분리 출력
		}
		else
		{
			m_bSeparateRebarBMWing		= TRUE;		// 날개벽 철근수량 분리 출력
			m_bSeparateRebarBMColumn	= TRUE;		// 보기둥 철근수량 분리 출력
		}

		if(nFlag > 14)
			ar >> m_bSeparateRebarBMOther;			// 기타철근 철근수량 분리 출력(방호벽, 교명주받침)
		else
			m_bSeparateRebarBMOther	= TRUE;			// 기타철근 철근수량 분리 출력(방호벽, 교명주받침)

		if(nFlag > 1)
			ar >> m_bSeparateOutputRebarBMMain;		// 본체 철근재료표 분리 출력(수량이 많을경우 2장까지 늘어남)
		else
			m_bSeparateOutputRebarBMMain	= FALSE;// 본체 철근재료표 분리 출력(수량이 많을경우 2장까지 늘어남)

		ar >> m_nTypeOutRebarTable;					// 철근재료표 출력방법(0:일체,1:주철근,기타철근구분,2:부위별)
		if(nFlag > 25)
			ar >> m_bRebarTableSepByFy;			// 철근재료표 fy별로 분리해서 표현
		ar >> m_dSpaceBetweenRebarDetail;			// 상세도간 간격
		ar >> m_nCountRowEachTable;					// 철근재료표 테이블당 최대 row수
		if(nFlag > 7) 
			ar >> m_bMainRebarNumberingInStandardSection;	// 표준단면도에 주철근 넘버링 표기할지?
		if(nFlag > 8)
			ar >> m_bLengthInfoInMainRebarNumbering;			// 주철근 조립도 철근 번호에 길이 정보 표현할지?
		if(nFlag > 17)
			ar >> m_bExcludeDoupleMainRebarCrossSection;	// 횡단 구조도 출력시 겹치는 주철근 표시 안함(단 치수는 그대로 표현)
		if(nFlag > 22)
			ar >> m_bSepFrontAndBackWallRebar;	// 벽체 구조도 전면 배면을 분리 해서 표현
		if(nFlag > 24)
		{
			ar >> m_bSepUpperAndLowerFootingRebar;	// 기초 구조도 상면 하면을 분리해서 표현
			ar >> m_bSepUpperAndLowerSlabRebar;	// 상부슬래브 구조도 상면 하면을 분리해서 표현
		}
		if(nFlag > 23)
			ar >> m_nDirMarkUpperSlabMainRebar;	// 상부슬래브 주철근 구조도 철근 지시선 방향(0 : 시점각도 기준, 1 : 종점각도 기준, 2 : 자동)
		if(nFlag > 28)
			ar >> m_bSepUpperSlab2DanAll;
		if(nFlag > 30)
			ar >> m_bDrawHunchRebarOnly;
		if(nFlag > 31)
			ar >> m_nBasePosPlaneSupportRebarDim;


		// 접속슬래브 구조도
		ar >> m_dwDrawRebarAps;
		ar >> m_dScaleRebarAps;						// 접속슬래브 구조도 스케일
		ar >> m_dScaleDetailAps;					// 상세도 스케일
		ar >> m_dSpaceBetweenRebarDetailAps;		// 상세도간 간격
		if(nFlag > 25)
			ar >> m_bRebarTableSepByFyAps;	// 철근재료표 fy별로 분리해서 표현(접속슬래브)

		// 완충슬래브 구조도
		ar >> m_dwDrawRebarCns;						// 시리얼추가
		ar >> m_nTypeCnsRebar;						// 완충슬래브 1, 2 표현방법(0 : 1,2를 함께표현, 1 : 분리해서 표현)	시리얼추가
		ar >> m_dScaleRebarCns;						// 완충슬래브 구조도 스케일	시리얼추가
		if(nFlag > 12)
			ar >> m_dSpaceBetweenRebarDetailCns;

		// 현장타설 말뚝
		ar >> m_dwDrawHyunTa;
		ar >> m_dScaleHyunTaFront;
		ar >> m_dScaleHyunTaSection;
		if(nFlag > 15)
		{
			ar >> m_dScaleHyunTaDetailHold;
			ar >> m_dScaleHyunTaDetailSpac;
		}
		ar >> m_nTypeHyunTaCut;
		if(nFlag > 29)
		{
			ar >> m_nTypeHyunTaVol;
		}

		// 지질 주상도
		if(nFlag > 3)
		{
			ar >> m_dwDrawBoring;
			ar >> m_dScaleBoring;
		}

		// 기타 도면
		if(nFlag > 13)
		{
			ar >> m_dwDrawOthers;
			ar >> m_dScaleRebarOthers;
			ar >> m_dScaleDetailGuardWall;
			if(nFlag > 20)
				ar >> m_dScaleDetailMonument;
			ar >> m_dSpaceBetweenRebarDetailOthers;
		}

		// RC 슬래브교 표준도
		if(nFlag > 26)
		{
			ar >> m_dwDrawRcStd;
			ar >> m_dScaleRcStd1Base;	// 도면1 기본 스케일
			ar >> m_dScaleRcStd2Base;	// 도면2 기본 스케일
			ar >> m_dScaleRcStd3Base;	// 도면3 기본 스케일
		}
	}
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionVerticalPlane()
{
	m_szDxfPlanName		= _T("");
	m_dwDrawVP			= 0xFFFFFF;
	m_dScaleVPBase		=  300.0;
	m_dScaleVPCross		=  100.0;
	m_dScaleVPLocation	= 1000.0;
	m_bDrawVPCoordinateXY = TRUE;
}

// 도면일반
void CARcBridgeDrawingOptionData::DefaultDrawingOptionGeneral()
{
	// 일반도1
	m_dwDrawGen			= 0xFFFFFF;
	long i=0; for(i=0; i<4; i++)
	{
		m_bDrawGenWingWall[i][0] = TRUE;
		m_bDrawGenWingWall[i][1] = TRUE;
		m_bDrawGenWingWall[i][2] = FALSE;
	}
	m_bDrawGenCoordinateXY  = TRUE;
	m_bDimTableForPilePlane	= FALSE;	// 평면도 말뚝 간격 테이블 처리
		
	m_dScaleGenBase		= 100.0;
	m_dScaleGenDetailA	= 20.0;
	m_dScaleGenNotch	= 2.0;
	m_dScaleGenOrder	= 200.0;
	m_dScaleGenWingWall = 100.0;
	m_dScaleGenDetailB	= 30.0;
	m_dScaleGenConcJoint = 5.0;
	m_dScaleGenCrack	= 5.0;		// 균열유발용 상세 스케일
	m_dScaleFXJoint	= 30.0;	// 신축이음 상세 스케일
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionRebar()
{
	m_dwDrawRebar				= 0xFFFFFF;
	long i=0; for(i=0; i<4; i++)
	{
		m_bDrawRebarWingWall[i][0] = TRUE;
		m_bDrawRebarWingWall[i][1] = TRUE;
		m_bDrawRebarWingWall[i][2] = FALSE;
		m_bDrawRebarWingWall[i][3] = FALSE;
	}

	m_nTypeMainRebarAssm		= 0;			// 주철근 조립도 표현 방법
	m_dScaleRebarBase			= 50.0;
	m_dScaleRebarDetail			= 20.0;			// 상세 스케일
	m_dScaleRebarWingWall		= 50.0;			// 날개벽 스케일
	m_dScaleBMD					= 100.0;		// BMD 스케일
	m_dScaleSFD					= 100.0;		// SFD 스케일
	m_dScaleAFD					= 100.0;
	m_dScaleRebarDetailArchRibSpringging	= 30.0;		// 아치리브 스프링깅부 상세 스케일
	m_dScaleRebarDetailColumnSection		= 30.0;		// 기둥 단면 상세 스케일
	m_dScaleRebarDetailGirderSection		= 30.0;		// 거더 단면 상세 스케일
	m_dScaleRebarDetailSlabAcute			= 30.0;		// 슬래브 둔각부 상세 스케일
	m_dScaleRebarDetailWallObtuse			= 30.0;		// 벽체 예각부 상세 스케일
	m_dScaleRebarDetailFootingAcute			= 30.0;		// 기초 둔각부 상세 스케일
	m_dScaleRebarDetailPRF					= 30.0;		// 부상방지저판 상세 스케일
	m_bSeparateRebarBMWing		= TRUE;			// 날개벽 철근수량 분리 출력
	m_bSeparateRebarBMColumn	= TRUE;			// 보기둥 철근수량 분리 출력
	m_bSeparateRebarBMOther		= TRUE;			// 기타철근 철근수량 분리 출력(방호벽, 교명주받침)
	m_bSeparateOutputRebarBMMain	= FALSE;	// 본체 철근재료표 분리 출력(수량이 많을경우 2장까지 늘어남)
	m_nTypeOutRebarTable		= 0;			// 철근재료표 출력방법(0:일체,1:주철근,기타철근구분,2:부위별)
	m_bRebarTableSepByFy	= TRUE;			// 철근재료표 fy별로 분리해서 표현
	m_nCountRowEachTable		= 70;			// 철근재료표 테이블당 최대 row수
	m_dSpaceBetweenRebarDetail	= 1000;			// 상세도간 간격
	m_bMainRebarNumberingInStandardSection	= FALSE;	// 표준단면도에 주철근 넘버링 표기할지?
	m_bLengthInfoInMainRebarNumbering		= TRUE;		// 주철근 조립도 철근 번호에 길이 정보 표현할지?
	m_bExcludeDoupleMainRebarCrossSection	= FALSE;	// 횡단 구조도 출력시 겹치는 주철근 표시 안함(단 치수는 그대로 표현)
	m_bSepFrontAndBackWallRebar	= FALSE;	// 벽체 구조도 전면 배면을 분리 해서 표현
	m_bSepUpperAndLowerFootingRebar	= FALSE;	// 기초 구조도 상면 하면을 분리해서 표현
	m_bSepUpperAndLowerSlabRebar	= FALSE;	// 상부슬래브 구조도 상면 하면을 분리해서 표현
	m_bSepUpperSlab2DanAll			= FALSE;
	m_bDrawHunchRebarOnly			= FALSE;
	m_nBasePosPlaneSupportRebarDim	= 0;
	m_nDirMarkUpperSlabMainRebar	= 2;	// 상부슬래브 주철근 구조도 철근 지시선 방향(0 : 시점각도 기준, 1 : 종점각도 기준, 2 : 자동)
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionAps()
{
	m_dwDrawRebarAps				= 0xFFFFFF;
	m_dScaleRebarAps				= 50.0;		// 접속슬래브 구조도 스케일
	m_dScaleDetailAps				= 20.0;		// 상세도 스케일
	m_dSpaceBetweenRebarDetailAps	= 1000;		// 상세도간 간격
	m_bRebarTableSepByFyAps	= TRUE;	// 철근재료표 fy별로 분리해서 표현(접속슬래브)
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionOthers()
{
	m_dwDrawOthers				= 0xFFFFFF;
	m_dScaleRebarOthers			= 50.0;				// 기본 스케일
	m_dScaleDetailGuardWall		= 20.0;				// 상세 스케일
	m_dScaleDetailMonument		= 10.0;				// 교명주 상세 스케일
	m_dSpaceBetweenRebarDetailOthers	= 1000;		// 상세도간 간격
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionShoeArrange()
{
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionBoring()
{
	m_dwDrawBoring					= 0xFFFFFF;
	m_dScaleBoring					= 100.0;
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionCns()
{
	m_dwDrawRebarCns	= 0xFFFFFF;
	m_nTypeCnsRebar		= 0;
	m_dScaleRebarCns	= 50.0;	// 완충슬래브 구조도 스케일
	m_dSpaceBetweenRebarDetailCns	= 1000;

}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionHyunTa()
{
	m_dwDrawHyunTa			= 0xFFFFFF;
	m_dScaleHyunTaFront		= 50.0;
	m_dScaleHyunTaSection	= 30.0;
	m_dScaleHyunTaDetailHold	= 30.0;
	m_dScaleHyunTaDetailSpac	= 30.0;
	m_nTypeHyunTaCut		= 1;
	m_nTypeHyunTaVol = 0;
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionSlabStandardOutput()
{
	m_dwDrawRcStd	= 0xFFFFFF;
	m_dScaleRcStd1Base	= 100;
	m_dScaleRcStd2Base	= 50;
	m_dScaleRcStd3Base	= 30;
}