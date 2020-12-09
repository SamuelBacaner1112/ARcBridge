// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcAps.h: interface for the CARcBridgeCalcAps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCAPS_H__067ECARcBridgeCalcAps1265_7599_4695_89FB_1616F56E5748__INCLUDED_)
#define AFX_ARCBRIDGECALCAPS_H__067E1265_7599_4695_89FB_1616F56E5748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define APS_CASE_ULT 0
#define APS_CASE_USE 1
#define APS_CASE_USE_JUDGE 2
#define APS_CASE_COUNT	3

class AFX_EXT_CLASS CARcBridgeCalcAps  
{
public:
	CARcBridgeCalcAps( CApsApp* pAps, BOOL bStt);
	virtual ~CARcBridgeCalcAps();

	CApsApp *m_pAps;
	long	m_nSelectApsShearCheck;
	double	m_dLoadFactorDeadLoad[APS_CASE_COUNT][3];	// [ULT/USE][DC/DW/EV]
	double	m_dLoadFactorLiveLoad[APS_CASE_COUNT];

	BOOL m_bLsd;
	CString m_szCombo[APS_CASE_COUNT];
	double  m_dEta[APS_CASE_COUNT]; // 사용한계, 극한한계
	double m_dMu[APS_CASE_COUNT];
	double m_dVu[APS_CASE_COUNT];
	
	BOOL IsCobel();
	double GetVForceCorbel(BOOL bDeadLoad, long nLoadFactor=-1);
	BOOL IsApplyApsAxleWeight(CRcBridgeRebar *pBri);
private:
	void CalculateCobel();
	
	BOOL m_bStt;
	double m_LenAps;		// 접속슬래브의 길이
	double m_dSpan;			// 설계지간
	double m_dWdp;			// 포장하중(DW)
	double m_dWdw;			// 자중(DC)
	double m_dWds;			// 연직토압(EV)
	double m_dDB;			// DB하중
	double m_dCo_i;			// 충격계수
	double m_dLe;			// 윤하중 분포폭
	double m_dD;			// 
	double m_dMd[3];		// 고정하중 모멘트  [DC/DW/EV]
	double m_dVd[3];		// 고정하중 전단력
	double m_dMl;			// 활하중 모멘트
	double m_dVl;			// 활하중 전단력
	double m_dVlDist;		// 전단력 산정위치 (활하중)
	double m_dPs;			// 윤하중
	double m_VuCobel;		// 코벨의 전단력	

	double m_UW_Ascon;		// 포장단위중량
	double m_UW_Conc;		// 콘크리트 단위중량
	
	//
	BOOL m_bConcAps;		// 콘크리트 포장
	
	void WriteCrackCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteDesignAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteRebarCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	void WriteDesignCobel(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne=TRUE);
	
public:
	void SetConcData();
	void CalculateAll();
	void CalculateAps();
	void WriteCalcAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bWriteOne=TRUE);
	
	CARcBridgeCalcStd *m_pStd;
	
	CConcBasic *m_pConc;
	CConcBasic *m_pConcCobel;
	CConcData  *m_pConcCobel_; // 임시..
};

#endif // !defined(AFX_ARCBRIDGECALCAPS_H__067E1265_7599_4695_89FB_1616F56E5748__INCLUDED_)
