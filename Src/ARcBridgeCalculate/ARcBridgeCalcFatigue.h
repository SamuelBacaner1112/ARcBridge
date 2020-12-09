// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcFatigue.h: interface for the CARcBridgeCalcFatigue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCFATIGUE_H__2FF5E406_E6FC_45C3_A10B_8F1000097B21__INCLUDED_)
#define AFX_ARCBRIDGECALCFATIGUE_H__2FF5E406_E6FC_45C3_A10B_8F1000097B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeCalcFatigue  
{
public:
	CARcBridgeCalcFatigue();
	virtual ~CARcBridgeCalcFatigue();

	CARcBridgeCalcStd *m_pStd;

	void CalculateAll();
	void WriteCalcSluggishAndFatigue(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle);
private:
	BOOL m_bOkSluggish[MAX_JIJUM_COUNT_RC];
	BOOL m_bOkFatigue[MAX_JIJUM_COUNT_RC];
	BOOL m_bOkFatigue_Jigan[MAX_JIJUM_COUNT_RC-1];		// Lsd 지간
	double m_dTmin;					// 처짐 검토 최소 두께
	double m_dFF[MAX_JIJUM_COUNT_RC];					// 응력 범위 - LSD 일경우 지점만 가진다.
	double m_dFF_Jigan[MAX_JIJUM_COUNT_RC-1];			// 응력 범위 LSD 지간
	
	// 처짐검토 LSD
	double m_dP0;
	double m_dP[MAX_JIJUM_COUNT_RC];
	double m_dP_[MAX_JIJUM_COUNT_RC];
	double m_dK[MAX_JIJUM_COUNT_RC];
	double m_dResultLsd[MAX_JIJUM_COUNT_RC];

	double m_dI[MAX_JIJUM_COUNT_RC];		// 한계지간
	double m_dD;		// 깊이-비
	double m_dRatio[MAX_JIJUM_COUNT_RC];	// 비율

	double m_dFs[MAX_JIJUM_COUNT_RC][2];		// 철근 응력 [최대, 최소]
	double m_dFs_Jigan[MAX_JIJUM_COUNT_RC-1][2];		// 철근 응력 [최대, 최소]

	void CalculateSluggish();
	void CalculateFatigue();
	void WriteCheckSluggish(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WriteCheckFatigue(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void MakeTableTmin(CXLFormatText *XLText);
	void MakeTableFf(CXLFormatText *XLText);

	// Lsd
	void WriteCheckSluggish_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WriteCheckFatigue_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri);
	void WriteCheckFatigueStress(CRcBridgeRebar *pBri, CXLFormatText *XLText, long nJigan, long nPos, HGBOOL bMax, HGBOOL bJigan=FALSE);
	// 피로검토 모멘트
	double GetMomentFatigue(CRcBridgeRebar *pBri, long nJigan, long nPos, HGBOOL bMax);		// ixCheckPos 검토위치 - 지간,지점 모두 포함
	void WriteStringArray(CXLFormatText *XLText, CStringArray &strArr);
};

#endif // !defined(AFX_ARCBRIDGECALCFATIGUE_H__2FF5E406_E6FC_45C3_A10B_8F1000097B21__INCLUDED_)
