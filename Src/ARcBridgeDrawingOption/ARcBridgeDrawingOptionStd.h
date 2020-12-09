// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDrawingOptionStd.h: interface for the CARcBridgeDrawingOptionStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_)
#define AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLR_BACK	 RGB(253, 250, 255)

// 도면 일반
#define COMMON_TABLESHADE	0x00000001
#define COMMON_TITLEBOX		0x00000002
#define COMMON_NOHCAD		0x00000004
#define COMMON_DISPLAY_ANG_LEN_BASE	0x00000008	// 사거리 표기 기준 표시

// 종평면도
#define VP_PLANE			0x00000001
#define VP_CROSS			0x00000002
#define VP_LONGI			0x00000004
#define VP_LOCATION			0x00000008
#define VP_CANT				0x00000010
#define VP_LONGI_SLOPE		0x00000020
#define VP_LEGEND			0x00000040
#define VP_NOTE				0x00000080
// 일반도
#define GEN_PLANE			0x00000001
#define GEN_CROSS			0x00000002
#define GEN_LONGI			0x00000004
#define GEN_DETAIL_A		0x00000008
#define GEN_NOTCH			0x00000010
#define GEN_ORDER			0x00000020
#define GEN_FRONT			0x00000040
#define GEN_FOOT_PLANE		0x00000080
#define GEN_WING_BL			0x00000100
#define GEN_WING_ER			0x00000200
#define GEN_WING_BR			0x00000400
#define GEN_WING_EL			0x00000800
#define GEN_DETAIL_B		0x00001000
#define GEN_NOTE			0x00002000
#define GEN_CONCJOINT		0x00004000
#define GEN_WALL_PLANE		0x00008000
#define GEN_DETAIL_FXJOINT	0x00010000
#define GEN_DETAIL_CRACK	0x00020000
// 구조도
#define REBAR_STD_SECT		0x00000001
#define REBAR_MAIN_ASSM		0x00000002
#define REBAR_BMD			0x00000004
#define REBAR_SFD			0x00000008
#define REBAR_UPPER_SLAB	0x00000010
#define REBAR_BEGIN_WALL	0x00000020
#define REBAR_END_WALL		0x00000040
#define REBAR_INWALL_FRONT	0x00000080
#define REBAR_INWALL_SECT	0x00000100
#define REBAR_FOOTING_PLANE	0x00000200
#define REBAR_COLUMN_PLANE	0x00000400
#define REBAR_GIRDER_SECT	0x00000800
#define REBAR_DETAIL		0x00001000
#define REBAR_WING_BL		0x00002000
#define REBAR_WING_ER		0x00004000
#define REBAR_WING_BR		0x00008000
#define REBAR_WING_EL		0x00010000
#define REBAR_NUMMB_STD_SECT		0x00020000	// 표준단면도에 주철근 넘버링 표기
#define REBAR_DETAIL_CHAIR_BLOCK	0x00040000	// chair block 상세도 
#define REBAR_CROSSSECTION	0x00080000	// 횡단면 구조도
#define REBAR_AFD			0x00100000	// 아치리브 축력도
#define REBAR_DETAIL_COUPLER	0x00200000	// 커플러 상세도
#define REBAR_LOWER_SLAB	0x00400000
#define REBAR_DETAIL_PRF	0x00800000	//부상방지저판 상세도

// 접속 슬래브
#define REBAR_APS_SIDE		0x00000001	// 단면도
#define REBAR_APS_FRONT		0x00000002	// 정면도
#define REBAR_APS_PLANE		0x00000004	// 평면도
#define REBAR_APS_DETAIL_RB	0x00000008	// 철근상세
#define REBAR_APS_DETAIL_A	0x00000010	// 상세A
#define REBAR_APS_TABLE_M	0x00000020	// 재료표
#define REBAR_APS_TABLE_RB	0x00000040	// 철근 재료표
#define REBAR_APS_NOTE		0x00000080	// 기본노트

//완충슬래브
#define REBAR_CNS_SIDE		0x00000001	// 단면도
#define REBAR_CNS_FRONT		0x00000002	// 정면도
#define REBAR_CNS_PLANE		0x00000004	// 평면도
#define REBAR_CNS_DETAIL_RB	0x00000008	// 철근상세
#define REBAR_CNS_DETAIL_A	0x00000010	// 상세A
#define REBAR_CNS_TABLE_M	0x00000020	// 재료표
#define REBAR_CNS_TABLE_RB	0x00000040	// 철근 재료표
#define REBAR_CNS_NOTE		0x00000080	// 기본노트


// 기타 도면 - 방호벽
// 기타 도면 - 현장타설 말뚝
#define HYUNTA_FRONT		0x00000001
#define HYUNTA_SECT_A		0x00000002
#define HYUNTA_SECT_B		0x00000004
#define HYUNTA_SECT_C		0x00000008
#define HYUNTA_DETAIL_HOLD	0x00000010
#define HYUNTA_DETAIL_SPAC	0x00000020
#define HYUNTA_NOTE			0x00000040
// 교량 받침 배치도
// 지질 주상도
#define BORING_VERSECT		0x00000001
#define BORING_FORCE		0x00000002
#define BORING_LEGEND		0x00000004
#define BORING_NOTE			0x00000008

#define RCSTD1_DESIGN_CONDITION	0x00000001	// 도면1_설계조건
#define RCSTD1_STANDARD_OUTLINE	0x00000002	// 도면1_표준단면도 외곽
#define RCSTD1_CROSS			0x00000004	// 도면1_횡단면도
#define RCSTD1_PLAN				0x00000008	// 도면1_평면도
#define RCSTD1_NOTE				0x00000010	// 도면1_노트
#define RCSTD1_COUNT			5
#define RCSTD2_STANDARD			0x00000020	// 도면2_표준단면도
#define RCSTD2_PLAN				0x00000040	// 도면2_평면도
#define RCSTD2_BMD				0x00000080	// 도면2_BMD
#define RCSTD2_SFD				0x00000100	// 도면2_SFD
#define RCSTD2_MAINASSM_1		0x00000200	// 도면2_주철근조립도1Cycle
#define RCSTD2_MAINASSM_2		0x00000400	// 도면2_주철근조립도2Cycle
#define RCSTD2_OBTUS_DETAIL		0x00000800	// 도면2_둔각부 보강상세
#define RCSTD2_COUNT			7
#define RCSTD3_GIRDER_DETAIL	0x00001000	// 도면3_거더상세
#define RCSTD3_GUARDWALL_DETAIL	0x00002000	// 도면3_방호벽상세
#define RCSTD3_NOTE				0x00004000	// 도면3_노트
#define RCSTD3_SPACER			0x00008000	// 도면3_간격유지재
#define RCSTD3_DESIGN_METHOD	0x00010000	// 도면3_설계요령
#define RCSTD3_COUNT			5

class CARcBridgeDrawingOptionData;

class AFX_EXT_CLASS CARcBridgeDrawingOptionStd : public CObject  
{
public:
	CARcBridgeDrawingOptionStd();
	virtual ~CARcBridgeDrawingOptionStd();

	void Serialize(CArchive& ar);
	void DefaultDrawingOptionCommon();
	void MakeDrawingOption(long nCountOpt=-1);
	CARcBridgeDrawingOptionData* GetDrawingOption(long nBri);
	void SyncApsDowelRebarSeperateRebarOption(long nBir = -1);

	CDocument* m_pDoc;
	long m_nCurPropPage;

	CTypedPtrArray <CObArray, CARcBridgeDrawingOptionData*> m_pArrDrawingOption;

	// 도면 일반
		// In Tree
	DWORD m_dwDrawCommon;
	long m_nTypeEL;		// Elevation 표기 방법 0 : EL. 15.000, 1 : EL.:1.5000, 2 : EL.=15.000
	long m_nSelectFckDomText;	// 콘크리트 강도 도면 표기 방법(0 : 콘크리트 강도, 1 : 굵은 골재 최대치수)
	long   m_nTypeTitleSeparatorBridge;	// 분리형 교량 대제목 타입(0:도면명, 1:교량명+도면명, 2:교량명+선형/반대방향명+도면명)
	long   m_nTypeTitleBridge;			// 일체형 교량 대제목 타입(0:도면명, 1:교량명+도면명)
	long  m_nTypeRebarTable;			// 0 : 기본타입, 1 : 간소화타입
	BOOL m_bAddRawRebarTable;	// 미가공 철근 재료표 추가

		// In Grid
	CString	m_strDgkName;
	CString	m_strXRefPath;
	CString	m_strDomNumHead;
	CStringArray m_strDgkAttrPrompt;
	CStringArray m_strDgkAttrText;
	double m_dWidthRebarTableRect; // 철근재료표 간소화 타입에서 철근상세 Rect하나의 폭
	double m_dHeightRebarTableRect; // 철근재료표 간소화 타입에서 철근상세 Rect하나의 높이
};

#endif // !defined(AFX_ARCBRIDGEDRAWINGOPTIONSTD_H__FF288642_E27F_4EF9_A031_7591A844E550__INCLUDED_)
