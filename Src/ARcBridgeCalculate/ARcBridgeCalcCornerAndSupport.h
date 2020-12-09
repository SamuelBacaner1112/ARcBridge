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

	double m_dFtmax[2];		// ���������� �ִ밪
	double m_dFtaUnitApply[2];		// ������� CGS: 1.06��Fck, SI: 1/3��Fck
	double m_dAsReq[2];				// �ʿ� ö�ٷ�
	double m_dAsUsed[2];			// ��� ö�ٷ�

	// �������� �ڸ��Ʈ ��������
	BOOL m_bIsInsideBending[2];		// �������� �ڸ��Ʈ �˵��� �ؾ��ϴ���...
	double m_dMomentInSide[2];		// �������� �ڸ��Ʈ
	double m_dTh1[2];
	double m_dTh2[2];
	double m_dAs_[2];				// ���� ö�ٷ�

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
	// �찢�� ����   0: ������, 1: ������
	double m_dTs[2];		// ������ �β�
	double m_dTw[2];		// ��ü �β�
	double m_dHH[2];		// ��ġ ����
	double m_dHW[2];		// ��ġ ��
	double m_dAngle[2];	// ��ġ ����( Atan(HW/HH))
	double m_dDeltaL[2];
	double m_dA[2];
	double m_dB[2];
	double m_dR[2];
	double m_dW[2];			// �������� ��������

	double m_dMomentUltTonM[2];
	double m_dMomentUltKgfcm[2];
	double m_dMomentTonM[2];		// ton.m - KN.m ����..
	double m_dMomentKgfcm[2];		// kgf.cm  - MN.m ����..
	
	double m_dFsa[2];				// ����ö�� �������
	double m_AsDanbu[2][3];			// �� �� �� ���� ö�ٷ�
	double m_AsMain[2][3];			// ��ö�ٷ�

	long m_nCntDanbuRebar[2];		// �ܺ�ö�� ��� �ܼ�
	long m_nCntMainRebar[2];		// ��ö�� ��� �ܼ�

	CString m_strDanbuDia[2][3][2];	// �ܺ�ö�� ����
	double m_dDanbuCtc[2][3];		// �ܺ�ö�� CTC

	double m_dMainCtc;				// ��ö�� CTC
	long m_nCntMainCycle;			// ��ö�� Cycle ��

	long m_nCntCycleByDan[2][3];	// �ܺ� Cycle��
	CString m_strMainDia[2][3][2];	// ��ö�� ����
};

#endif // !defined(AFX_ARCBRIDGECALCCORNERANDSUPPORT_H__419D453A_902C_4405_840B_1CD4547E092E__INCLUDED_)
