// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// LoadFooting.h: interface for the CLoadFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_)
#define AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VER_LOAD_LIVE		0
#define VER_LOAD_FOOTW		1
#define VER_LOAD_PAVE		2
#define VER_LOAD_BACKFILL	3
#define VER_LOAD_DEAD		4
#define VER_LOAD_COUNT		5

class CFootingApp;

class AFX_EXT_CLASS CLoadFooting : public CObject
{
public:
	CLoadFooting(CARcBridgeCalcStd *pStd, CRcBridgeRebar *pBri, long nJijum, long nIdex, BOOL bUseLoad, ETypeEnvComboLsd eEnvType=eEnvService);	// 0: 앞굽최대, 1: 뒷굽최대, 2: 축력최대, 3: 지진시	
	virtual ~CLoadFooting();

	CARcBridgeCalcStd *m_pStd;
	CRcBridgeRebar *m_pBri;
	
	CFootingApp *m_pFooting;
	
	CString m_sTitle;
	CString m_sTitleSub;
	CString m_sCombo;
	long m_nCombo;					// 적용된 하중조합 Index
	long m_nJijum;
	long m_nIndex;					// 0: 축력최대, 1: 부모멘트 최대, 2: 정모멘트 최대, 3: 지진시
	BOOL m_bUseLoad;
	BOOL m_bEarthQuake;
	BOOL m_bApplyHoriEarQ;			// 지신시 관성력 적용 여부
	BOOL m_bPile;					// 말뚝기초면 TRUE, 직접기초면 FALSE

	ETypeEnvComboLsd m_eEnvComboType;

	double m_dHightBackFill;		// 뒷채움 높이
	double m_dFactorDeadLoad;		// 고정하중 계수
	double m_dFactorSoilHor;		// 연직토압 계수
	double m_dFactorPave;			// 포장하중 계수
	double m_dFactorLiveLoad;		// 활하중 계수
	// 연직력, 수직력
	// 0: 노면 활하중, 1: 기초자중, 2: 포장, 3: 배면토사
	double m_dVerLoad[VER_LOAD_COUNT];
	double m_dHorLoad[VER_LOAD_COUNT];

	double m_dQmaxMass;				// 지지력 검토에 MASS반력을 포함 시킬때
	// Xi, Yi : 기준점에서 작용점 까지의 거리
	double m_dXiVerLoad[VER_LOAD_COUNT];
	double m_dYiHorLoad[VER_LOAD_COUNT];
	double m_dMr[VER_LOAD_COUNT];
	double m_dMo[VER_LOAD_COUNT];
	double m_dA;						// 수평관성력 계수

	double m_dVerSmallTotal;		// 소계
	double m_dHorSmallTotal;		// 소계
	double m_dMrSmallTotal;
	double m_dMoSmallTotal;

	double m_dHightTosa;			// 기초토사 높이

	// 상부 전달하중
	double m_dUpperLoadV;			// 사용하중 연직력
	double m_dUpperLoadH;			// 사용하중 수직력
	double m_dUpperLoadM;			// 사용하중 모멘트

	double m_dXiUpperLoadV;
	double m_dYiUpperLoadH;

	double m_dMrUpperV;
	double m_dMoUpperV;
	double m_dMrUpperH;
	double m_dMoUpperH;
	double m_dMrUpperM;
	double m_dMoUpperM;

	// 교량형식이 BRIDGE_TYPE_TRAP2일때 상부 하중이 하나더 추가 된다.
	BOOL m_bTwoUpperLoad;
	double m_dUpperLoadV_Add;
	double m_dUpperLoadH_Add;
	double m_dUpperLoadM_Add;
	long m_nCombo_Add;
	
	double m_dXiUpperLoadV_Add;
	double m_dYiUpperLoadH_Add;

	double m_dMrUpperV_Add;
	double m_dMoUpperV_Add;
	double m_dMrUpperH_Add;
	double m_dMoUpperH_Add;
	double m_dMrUpperM_Add;
	double m_dMoUpperM_Add;

	// 총계
	double m_dVerTotal;
	double m_dHorTotal;
	double m_dMoTotal;
	double m_dMrTotal;

	double m_dDistCenttoDosim;
	double m_dPileLoadV;			// 파일기초 도심 작용력
	double m_dPileLoadH;
	double m_dPileLoadM;
	double m_dPileLoadMCen;			// 기초 중심에서의 작용 모멘트

	// 파일 반력
	CDoubleArray m_dArrPileFoce;
	CDoubleArray m_dArrDistPile;	// 기준점에서 파일까지의 거리
	CStringArray m_szAddPileNo;
	CStringArray m_szAddPileFoceString;
	CDoubleArray m_dAddPileFoce;

	double m_dE;					// 편심
	double m_dLx;					// 바닥면 반력의 작용폭
	double m_dQmax;					// 기초바닥면에서의 최대지반반력
	double m_dQmin;					// 기초바닥면에서의 최소지반반력
	double m_dQa;					// 기초바닥면에서의 최대허용지지력
	double m_dHu;					// 허용 수평력
	BOOL m_bQTriangle;				// TRUE: 삼각형 분포, FALSE: 사다리꼴 분포
	BOOL m_bQmaxFront;				// TRUE: Qmax가 앞굽쪽, FALSE: Qmax가 뒷굽쪽
	BOOL m_bOkSupportForce;			// 지지력에 대한 검토결과
	BOOL m_bOkSliding;				// 활동에 대한 검토결과
	BOOL m_bOkOverturning;			// 전도에 대한 검토결과

	double m_ScF;			// 기초 자중에 의한 단면력
	double m_ScB;
	double m_McF;
	double m_McF2;
	double m_McF3;
	double m_McB;
	double m_SqF;			// 지반 반력에 의한 단면력
	double m_SqB;
	double m_MqF;
	double m_MqF2;
	double m_MqF3;
	double m_MqB;
	double m_SsB;			// 뒷채움 토사에 의한 단면력
	double m_MsB;
	double m_SpB;			// 뒷채움 포장에 의한 단면력
	double m_MpB;
	double m_SlB;			// 노면활하중에 의한 단면력
	double m_MlB;			

	// 앞굽,뒷굽에 작용하중(설계 단면력)
	double m_dDistMf;
	double m_dDistMb;
	double m_dDistMf2;
	double m_dDistMf3;
	double m_dDistSf;				// 기준점으로부터의 앞굽 계산단면 거리
	double m_dDistSb;				// 기준점으로부터의 뒷굽 계산단면 거리
	double m_dHgtSf;				// 앞굽 전단검토 높이;
	BOOL m_bOutBoundFootFront;
	BOOL m_bOutBoundFootBack;

	double m_dQmF;
	double m_dQmF2;
	double m_dQmF3;
	double m_dQmB;
	double m_dQvF;
	double m_dQvB;
	double m_dVuFront;
	double m_dVuBack;
	double m_dMuFront;
	double m_dMuFront2;
	double m_dMuFront3;
	double m_dMuBack;

	// 허용응력
	double m_dQaEquation;
	double m_dA_Equation;
	
	void CalculateAll();
	void MakeTableOtherLoad(CXLFormatText *XLText);
	void MakeTableOtherLoadLsd(CXLFormatText *XLText);
	void MakeTableLoadAll(CXLFormatText *XLText, long nIndexTiny, BOOL bTable=TRUE);
	void WriteCheckOverturning(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteCheckSliding(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteCheckSupportForce(CXLFormatText *XLText, long nIndexSub, BOOL bFirst);
	void WriteSectionDesign(CXLFormatText *XLText, BOOL bUse, BOOL bFirst);
	void WriteSectionDesignFront(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack(CXLFormatText * XLText, long nIndexSub, BOOL bUse);
	void WriteSectionDesignFront_Lsd(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack_Lsd(CXLFormatText * XLText, long nIndexSub, BOOL bUse);
	void WriteSectionDesign_Pile(CXLFormatText *XLText, BOOL bUse, BOOL bFirst);
	void WriteSectionDesign_PileText(CXLFormatText * XLText, long nIndexSub, HGBOOL bFront);
	void WriteSectionDesignFront_Pile(CXLFormatText *XLText, long n, long nIndexSub, BOOL bUse);
	void WriteSectionDesignBack_Pile(CXLFormatText *XLText, BOOL bUse);

	void WriteForceCenter_Pile(CXLFormatText *XLText);
	void CalculateBaseLoad();
	void CalculateApplyLoad();
	void CalulatePileCentroidLoad();
	void CalulateQaMax();
	void CalculateSupportForce();
	void CalculateSlidingCheck();
	void CalculateOverturningCheck();
	// nType = 0: 전체, 1: 앞굽, 2: 뒷굽 3: 앞굽전단위치, 4: 뒷굽전단위치
	void GetPileForcArr(long nType = 0, BOOL bUnitM = TRUE);				// 말뚝 반력을 가져온다.
	void CalculateSectionDesign();
	void GetDistFrontTrap2AndArch(double &dDist, double &dHeight);

	void MakeTableFootCenter(CXLFormatText *XLText);
	void DrawSupportForce(CDomyun *pDomP, long nType, BOOL bSection);
	CString GetStringVertLoad(HGINT32 iIndexLoad, HGBOOL bMark=TRUE);
private:
	void WriteCalcQubyHansen(CXLFormatText *XLText, BOOL bFirst);
	void WriteCalcQubyTerzaghi(CXLFormatText *XLText, BOOL bFirst);
	void WriteCalcQubyMeyerhof(CXLFormatText *XLText, BOOL bFirst);
	double GetPileFootingCentroid(long nJijum);	// 말뚝 기초의 도심
	double GetDistSlopeBack();  // 경사 기초일때 기준점에서 기초 뒷굽 시작지점 까지의 거리
};

#endif // !defined(AFX_LOADFOOTING_H__C9C5F5AD_2751_4444_8F8A_D3C0DD21FFF6__INCLUDED_)
