// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcCornerAndSupport.h: interface for the CARcBridgeCalcCornerAndSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCCORNERANDSUPPORT_H__419D453A_902C_4405_840B_1CD4547E092E__INCLUDED_)
#define AFX_ARCBRIDGECALCCORNERANDSUPPORT_H__419D453A_902C_4405_840B_1CD4547E092E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CARcBridgeCalcCornerAndSupport  
{
public:
	void CalculateAll(BOOL bUpperSlab);
	CARcBridgeCalcCornerAndSupport();
	virtual ~CARcBridgeCalcCornerAndSupport();

	CARcBridgeCalcStd *m_pStd;
	BOOL m_bUpperSlab;

	void WriteCalcCorner(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle);
	void WriteCalcSupport(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle);

	double GetDoubleCheckConcSlab(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan);
	double GetDoubleCheckConcWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bWingWall);
//	double GetDoubleCheckConcArchRib(CConcBasic *pConcData, BOOL bReq, long nIndex);
	double GetDoubleCheckConcFooting(CConcBasic *pConcData, BOOL bReq, long nIndex);
	double GetDoubleCheckConcAps(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan);

	CString GetStringCheckConcSlab(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan, BOOL bOption=TRUE);
	CString GetStringCheckConcWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption=TRUE);
//	CString GetStringCheckConcArchRib(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption=TRUE);
	CString GetStringCheckConcFooting(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption=TRUE);
	CString GetStringCheckConcWingWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption=TRUE);
	CString GetStringCheckConcAps(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan, BOOL bOption=TRUE);

	double m_dFtmax[2];		// 인장응력의 최대값
	double m_dFtaUnitApply[2];		// 허용응력 CGS: 1.06√Fck, SI: 1/3√Fck
	double m_dAsReq[2];				// 필요 철근량
	double m_dAsUsed[2];			// 사용 철근량

	// 내측인장 휨모멘트 보강검토
	BOOL m_bIsInsideBending[2];		// 내측인장 휨모멘트 검도를 해야하는지...
	double m_dMomentInSide[2];		// 내측인장 휨모멘트
	double m_dTh1[2];
	double m_dTh2[2];
	double m_dAs_[2];				// 보강 철근량

	void WriteSupportCheckSlab(CXLFormatText *XLText, CConcBasic *pConc, double dLenSpan, BOOL bMain, BOOL bMainOver20, BOOL bSlabArea, BOOL bSlab300);
	
private:
	void CalculateCorner(BOOL bStt = TRUE);
	BOOL CalculateInsideBending(BOOL bStt = TRUE);
	void WriteCorner(CXLFormatText *XLText, long nIndexMiddle, long nIndexSmall, BOOL bStt);
	void WriteSupportCheck(CXLFormatText *XLText, long nIndexMiddle, long nIndexSmall);
	void WriteSupportCheckSlab(CXLFormatText *XLText, BOOL bUpperSlab);
	void WriteSupportCheckWall(CXLFormatText *XLText, BOOL bWingWall);
	void WriteSupportCheckInWall(CXLFormatText *XLText);
	void WriteSupportCheckLowerSlab(CXLFormatText *XLText);
	void WriteSupportCheckArchRib(CXLFormatText *XLText);
	void DrawCorner(CDomyun *pDomP, BOOL bStt);

	void MakeTableSupportSumSlab(CXLFormatText *XLText, long nIndexSub);
	void MakeTableSupportSumWall(CXLFormatText *XLText, long nIndexSub, BOOL bArchRib=FALSE);
	void MakeTableSupportSumInWall(CXLFormatText *XLText, long nIndexSub);
//	void MakeTableSupportSumArchrib(CXLFormatText *XLText, long nIndexSub);
	void MakeTableSupportSumLowerSlab(CXLFormatText *XLText, long nIndexSub, BOOL bPRF=FALSE);
	double GetSupportBarSlabRatio(double dFy);
	double GetSupportBarFootRatio(double dFy);
	// 우각부 검토   0: 시점측, 1: 종점측
	double m_dTs[2];		// 슬래브 두께
	double m_dTw[2];		// 벽체 두께
	double m_dHH[2];		// 헌치 높이
	double m_dHW[2];		// 헌치 폭
	double m_dAngle[2];	// 헌치 각도( Atan(HW/HH))
	double m_dDeltaL[2];
	double m_dA[2];
	double m_dB[2];
	double m_dR[2];
	double m_dW[2];			// 절점부의 구조물폭

	double m_dMomentUltTonM[2];
	double m_dMomentUltKgfcm[2];
	double m_dMomentTonM[2];		// ton.m - KN.m 단위..
	double m_dMomentKgfcm[2];		// kgf.cm  - MN.m 단위..
	
	double m_dFsa[2];				// 보강철근 허용응력
	double m_AsDanbu[2][3];			// 각 단 별 보강 철근량
	double m_AsMain[2][3];			// 주철근량

	long m_nCntDanbuRebar[2];		// 단부철근 사용 단수
	long m_nCntMainRebar[2];		// 주철근 사용 단수

	CString m_strDanbuDia[2][3][2];	// 단부철근 직경
	double m_dDanbuCtc[2][3];		// 단부철근 CTC

	double m_dMainCtc;				// 주철근 CTC
	long m_nCntMainCycle;			// 주철근 Cycle 수

	long m_nCntCycleByDan[2][3];	// 단별 Cycle수
	CString m_strMainDia[2][3][2];	// 주철근 직경
};

#endif // !defined(AFX_ARCBRIDGECALCCORNERANDSUPPORT_H__419D453A_902C_4405_840B_1CD4547E092E__INCLUDED_)
