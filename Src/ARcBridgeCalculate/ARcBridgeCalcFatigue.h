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
	BOOL m_bOkFatigue_Jigan[MAX_JIJUM_COUNT_RC-1];		// Lsd ����
	double m_dTmin;					// ó�� ���� �ּ� �β�
	double m_dFF[MAX_JIJUM_COUNT_RC];					// ���� ���� - LSD �ϰ�� ������ ������.
	double m_dFF_Jigan[MAX_JIJUM_COUNT_RC-1];			// ���� ���� LSD ����
	
	// ó������ LSD
	double m_dP0;
	double m_dP[MAX_JIJUM_COUNT_RC];
	double m_dP_[MAX_JIJUM_COUNT_RC];
	double m_dK[MAX_JIJUM_COUNT_RC];
	double m_dResultLsd[MAX_JIJUM_COUNT_RC];

	double m_dI[MAX_JIJUM_COUNT_RC];		// �Ѱ�����
	double m_dD;		// ����-��
	double m_dRatio[MAX_JIJUM_COUNT_RC];	// ����

	double m_dFs[MAX_JIJUM_COUNT_RC][2];		// ö�� ���� [�ִ�, �ּ�]
	double m_dFs_Jigan[MAX_JIJUM_COUNT_RC-1][2];		// ö�� ���� [�ִ�, �ּ�]

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
	// �Ƿΰ��� ���Ʈ
	double GetMomentFatigue(CRcBridgeRebar *pBri, long nJigan, long nPos, HGBOOL bMax);		// ixCheckPos ������ġ - ����,���� ��� ����
	void WriteStringArray(CXLFormatText *XLText, CStringArray &strArr);
};

#endif // !defined(AFX_ARCBRIDGECALCFATIGUE_H__2FF5E406_E6FC_45C3_A10B_8F1000097B21__INCLUDED_)
