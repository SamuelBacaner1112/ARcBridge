// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDrawingOptionData.h: interface for the CARcBridgeDrawingOptionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_)
#define AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeDrawingOptionData : public CObject  
{
public:
	void DefaultDrawingOptionCns();
	CARcBridgeDrawingOptionData();
	virtual ~CARcBridgeDrawingOptionData();


	// 종평면도
	CString m_szDxfPlanName;
	DWORD m_dwDrawVP;
	double m_dScaleVPBase;			// 기본 스케일
	double m_dScaleVPCross;			// 횡단면도 스케일
	double m_dScaleVPLocation;
	BOOL   m_bDrawVPCoordinateXY;

	// 일반도
	DWORD m_dwDrawGen;				// 일반도
	BOOL   m_bDrawGenWingWall[4][3];// 날개벽도면, [4]시점 좌.우, 종점 좌.우, [3]-정면도,단면도(수직),단면도(수평)
	BOOL   m_bDrawGenCoordinateXY;
	BOOL   m_bDimTableForPilePlane;	// 평면도 말뚝 간격 테이블 처리

	double m_dScaleGenBase;			// 기본 스케일
	double m_dScaleGenDetailA;		// 상세 "A" 스케일(방호벽상세)
	double m_dScaleGenNotch;		// NOTCH 상세 스케일
	double m_dScaleGenOrder;		// 시공 순서도 스케일
	double m_dScaleGenWingWall;		// 날개벽 스케일
	double m_dScaleGenDetailB;		// 상세 "B" 스케일
	double m_dScaleGenConcJoint;	// 시공이음 상세 스케일
	double m_dScaleFXJoint;			// 신축이음 상세 스케일
	double m_dScaleGenCrack;		// 균열유발용 상세 스케일

	// 구조도
	DWORD  m_dwDrawRebar;
	BOOL   m_bDrawRebarWingWall[4][4];		// 날개벽도면, [4]시점 좌.우, 종점 좌.우, [4]-정면도,단면도(수직),단면도(수평), 흉벽보강상세
	long   m_nTypeMainRebarAssm;			// 주철근 조립도 표현 방법
	double m_dScaleRebarBase;				// 기본 스케일
	double m_dScaleRebarDetail;				// 상세 스케일		-----> 삭제... 
	double m_dScaleRebarWingWall;			// 날개벽 스케일
	double m_dScaleBMD;						// BMD 스케일
	double m_dScaleSFD;						// SFD 스케일
	double m_dScaleAFD;						// 축력도 스케일
	double m_dScaleRebarDetailArchRibSpringging;	// 아치리브 스프링깅부 상세 스케일
	double m_dScaleRebarDetailColumnSection;	// 기둥 단면 상세 스케일
	double m_dScaleRebarDetailGirderSection;	// 거더 단면 상세 스케일
	double m_dScaleRebarDetailSlabAcute;		// 슬래브 둔각부 상세 스케일
	double m_dScaleRebarDetailWallObtuse;		// 벽체 예각부 상세 스케일
	double m_dScaleRebarDetailFootingAcute;		// 기초 둔각부 상세 스케일
	double m_dScaleRebarDetailPRF;				// 부상방지저판 상세 스케일
	BOOL   m_bSeparateRebarBMWing;			// 날개벽 철근수량 분리 출력
	BOOL   m_bSeparateRebarBMColumn;		// 보기둥 철근수량 분리 출력
	BOOL   m_bSeparateRebarBMOther;			// 기타철근 철근수량 분리 출력(방호벽, 교명주받침)
	BOOL   m_bSeparateOutputRebarBMMain;	// 본체 철근재료표 분리 출력(수량이 많을경우 2장까지 늘어남)
	long   m_nTypeOutRebarTable;			// 철근재료표 출력방법(0:일체,1:주철근,기타철근구분,2:부위별)
	BOOL m_bRebarTableSepByFy;			// 철근재료표 fy별로 분리해서 표현
	double m_dSpaceBetweenRebarDetail;		// 상세도간 간격
	long   m_nCountRowEachTable;			// 철근재료표 테이블당 최대 row수
	BOOL   m_bMainRebarNumberingInStandardSection;		// 표준단면도에 주철근 넘버링 표기할지?
	BOOL   m_bLengthInfoInMainRebarNumbering;			// 주철근 조립도 철근 번호에 길이 정보 표현할지?
	BOOL   m_bExcludeDoupleMainRebarCrossSection;	// 횡단 구조도 출력시 겹치는 주철근 표시 안함(단 치수는 그대로 표현)
	BOOL   m_bSepFrontAndBackWallRebar;	// 벽체 구조도 전면 배면을 분리 해서 표현
	BOOL   m_bSepUpperAndLowerFootingRebar;	// 기초 구조도 상면 하면을 분리해서 표현
	BOOL   m_bSepUpperAndLowerSlabRebar;	// 상부슬래브 구조도 상면 하면을 분리해서 표현
	BOOL   m_bSepUpperSlab2DanAll;	// 상부슬래브 구조도 2단철근 모두 표현
	long   m_nDirMarkUpperSlabMainRebar;	// 상부슬래브 주철근 구조도 철근 지시선 방향(0 : 시점각도 기준, 1 : 종점각도 기준, 2 : 자동)
	BOOL	m_bDrawHunchRebarOnly;	// 헌치 철근 구조도 따로 그림.
	long	m_nBasePosPlaneSupportRebarDim;		// 슬래브 평면구조도 수평철근 치수 기준위치 (0: 슬래브 좌우 끝단, 1: 중심선형위치)

	// 접속 슬래브 구조도
	DWORD  m_dwDrawRebarAps;
	double m_dScaleRebarAps;	// 접속슬래브 구조도 스케일
	double m_dScaleDetailAps;	// 상세도 스케일
	double m_dSpaceBetweenRebarDetailAps;	// 상세도간 간격
	BOOL m_bRebarTableSepByFyAps;	// 철근재료표 fy별로 분리해서 표현(접속슬래브)

	// 완충 슬래브 구조도
	DWORD  m_dwDrawRebarCns;
	long   m_nTypeCnsRebar;		// 완충슬래브 1, 2 표현방법(0 : 1,2를 함께표현, 1 : 분리해서 표현)
	double m_dScaleRebarCns;	// 완충슬래브 구조도 스케일
	double m_dSpaceBetweenRebarDetailCns;	// 상세도간 간격

	// 현장타설 말뚝
	DWORD  m_dwDrawHyunTa;			// 일반도
	double m_dScaleHyunTaFront;		// 현장타설 말뚝 정면도 스케일
	double m_dScaleHyunTaSection;	// 현장타설 말뚝 단면도 스케일
	double m_dScaleHyunTaDetailHold;	// 철근올음 방지 철판 스케일
	double m_dScaleHyunTaDetailSpac;	// SPACER 상세 스케일
	long   m_nTypeHyunTaCut;		// 말뚝 길이 표현 방법( 0 : 실제길이, 1 : 같은 비율로, 2 : 한구간에서)
	long	m_nTypeHyunTaVol;		//말뚝 수량 표현 방법 (0: 말뚝1본당 수량 1. 말뚝 전체 수량) // ARCBRIDGE-2474

	// 지질 주상도
	DWORD m_dwDrawBoring;
	double m_dScaleBoring;

	// 기타 도면
	DWORD  m_dwDrawOthers;
	double m_dScaleRebarOthers;					// 방호벽 기본 스케일
	double m_dScaleDetailGuardWall;				// 방호벽 상세 스케일
	double m_dScaleDetailMonument;				// 교명주 상세 스케일
	double m_dSpaceBetweenRebarDetailOthers;	// 방호벽 상세도간 간격

	// RC 슬래브교 표준도
	DWORD  m_dwDrawRcStd;
	double m_dScaleRcStd1Base;	// 도면1 기본 스케일
	double m_dScaleRcStd2Base;	// 도면2 기본 스케일
	double m_dScaleRcStd3Base;	// 도면3 기본 스케일

	
	void Serialize(CArchive& ar);

	void DefaultDrawingOptionVerticalPlane();
	void DefaultDrawingOptionGeneral();
	void DefaultDrawingOptionRebar();
	void DefaultDrawingOptionAps();
	void DefaultDrawingOptionOthers();
	void DefaultDrawingOptionShoeArrange();
	void DefaultDrawingOptionBoring();
	void DefaultDrawingOptionHyunTa();
	void DefaultDrawingOptionSlabStandardOutput();
};

#endif // !defined(AFX_ARCBRIDGEDRAWINGOPTIONDATA_H__3DA1DE19_57F1_4B6E_A152_AF94AB16FBD4__INCLUDED_)
