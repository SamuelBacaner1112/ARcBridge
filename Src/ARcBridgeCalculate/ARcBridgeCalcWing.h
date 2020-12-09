// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeCalcWing.h: interface for the CARcBridgeCalcWing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGECALCWING_H__99806687_5097_4B40_A775_A544F8009657__INCLUDED_)
#define AFX_ARCBRIDGECALCWING_H__99806687_5097_4B40_A775_A544F8009657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WING_FACTOR_ULT  0
#define WING_FACTOR_USE  1
#define WING_FACTOR_USE_JUDGE  2
#define WING_CASE_COUNT 3

#define LIVE_LOAD_ES	0
#define LIVE_LOAD_LS	1
#define LIVE_LOAD_COUNT	2

class AFX_EXT_CLASS CARcBridgeCalcWing  
{
public:
	void CalculateAll(BOOL bStt, BOOL bLeft);
	CARcBridgeCalcWing();
	virtual ~CARcBridgeCalcWing();

	CARcBridgeCalcStd *m_pStd;

	CWingWall *m_pWing;

	BOOL m_bStt;
	BOOL m_bLeft;

	BOOL m_bIsA;
	BOOL m_bIsD;
	BOOL m_bIsA_;
	BOOL m_bIsA_Calc;

	// A, B, C, D, A'
	double m_S[5];
	double m_M[5];
	double m_Vu[5];
	double m_Mu[5];

	double m_S_Use[5];
	double m_M_Use[5];
	double m_Vu_Use[5];
	double m_Mu_Use[5];

	double m_S_Use_Judge[5];
	double m_M_Use_Judge[5];
	double m_Vu_Use_Judge[5];
	double m_Mu_Use_Judge[5];

	double GetLoadAlways_LS() { return m_dLoadAlways_LS; };
	void WriteDesignWingWall(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle);
private:
	CDPoint m_xyA;
	CDPoint m_xyB;
	CDPoint m_xyC;
	CDPoint m_xyD;
	CDPoint m_xyE;
	CDPoint m_xyF;
	CDPoint m_xyA_;
	CDPoint m_xyE_;

	// 조건
	double m_dLoadAlways;
	double m_dLoadAlways_LS;
	double m_dWeightBackFill;
	double m_dPi;
	double m_dK0;
	double m_dHs;
	double m_dFH;
	CDPoint m_xyBaseTop;

	BOOL m_bLsd;
	CString m_szCombo[WING_CASE_COUNT];
	double  m_dEta[WING_CASE_COUNT]; // 극한한계, 사용한계-검토, 사용한계-판정
	double m_dFactorToap[WING_CASE_COUNT];
	double m_dFactorLive[WING_CASE_COUNT];
	double m_dFactorLive_LS[WING_CASE_COUNT];

	void MakeArea();
	void Calculate_BlockA();
	void Calculate_BlockB();
	void Calculate_BlockC();
	void Calculate_BlockD();
	void Calculate_BlockA_Add();
	void SetConcData();

	double GetEquivalentHeightWall(double dHeightWall, long nLiveLoadDistToBackWall);
	double GetValueAreaDCut(double dh, long nType, BOOL bUse = FALSE, BOOL bJudge=FALSE);		// nType 0: Sd', 1: Md'
	void DrawWingWallArea(CDomyun *pDomP);

	void WriteForceAreaA(CXLFormatText *XLText, long nIndexSub);
	void WriteForceAreaB(CXLFormatText *XLText, long nIndexSub);
	void WriteForceAreaC(CXLFormatText *XLText, long nIndexSub);
	void WriteForceAreaD(CXLFormatText *XLText, long nIndexSub);
	void WriteForceAreaD_Flat(CXLFormatText *XLText, long nIndexSub);
	void WriteForceAreaA_Add(CXLFormatText *XLText, long nIndexSub);
	void WriteForce3DPlate(CXLFormatText *XLText, long nIndexSub);

	void WriteForceSummary(CXLFormatText *XLText, long nIndexSub, BOOL bCalc3D, BOOL bMax);
	void WriteCheckSection(CXLFormatText *XLText, long nIndexSmall);
	void WriteSupportCheckWingWall(CXLFormatText *XLText, CConcBasic *pConc, long nColStt);
	void WriteCalcLoad_Dminus(CXLFormatText *pXLText, long nColStt, double dh, BOOL bUse=FALSE);
	void WriteCalcAlwaysLiveLoad(CXLFormatText *pXLText);

	BOOL IsLiveLoad_ES();
	BOOL IsLiveLoad_LS();
	BOOL IsLiveLoad(BOOL bES);
	double GetLiveLoadAlways(BOOL bES);
	void WriteCalcLoad_qq(CXLFormatText *pXLText);
	CString GetStringTypeCase(long nTypeCase);
};

#endif // !defined(AFX_ARCBRIDGECALCWING_H__99806687_5097_4B40_A775_A544F8009657__INCLUDED_)
