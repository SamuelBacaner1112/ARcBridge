// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ModulusFoundation.h: interface for the CModulusFoundation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUDLUSFOUNDATION_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_)
#define AFX_MOUDLUSFOUNDATION_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MODULUS_FOUNDATION_NORMAL		0   // ���
#define MODULUS_FOUNDATION_EQ			1	// ������
#define MODULUS_FOUNDATION_MAX			2	// 

#define MODULUS_POS_LOWERSLAB			0
#define MODULUS_POS_STTWALL				1
#define MODULUS_POS_ENDWALL				2
#define MODULUS_POS_MAX					3

#define MODULUS_BORING_MAX	5

class CFootingApp;
class CBoringData;

// �����ؼ��� ���� ������ ������ ���
class AFX_EXT_CLASS CModulusFoundation
{
public:
	CModulusFoundation();
	virtual ~CModulusFoundation();

public: 
	CARcBridgeCalcStd *m_pStd;

	long m_nQtyCalc;
	CLongArray m_lArrIndex;
	CDoubleArray m_dArrElevationTop; // ���� ������ ��� EL
	CDoubleArray m_dArrElevationBottom; // ���� ������ �ϴ� EL

	double m_B[MODULUS_POS_MAX];	// ������౳�� ���� (������ ��ü ����)
	double m_L[MODULUS_POS_MAX];	// Ⱦ���Է� ��

	double m_dAlpah[MODULUS_FOUNDATION_MAX];
	double m_TerzaghiFactor;
	double m_KvPowFactor;

	double m_Bv[MODULUS_POS_MAX][MODULUS_FOUNDATION_MAX];
	double m_Kvo[MODULUS_POS_MAX][MODULUS_FOUNDATION_MAX][MODULUS_BORING_MAX];
	double m_KvBB[MODULUS_POS_MAX][MODULUS_FOUNDATION_MAX][MODULUS_BORING_MAX];
	double m_KvBL[MODULUS_POS_MAX][MODULUS_FOUNDATION_MAX][MODULUS_BORING_MAX];
	double m_Ks[MODULUS_POS_MAX][MODULUS_FOUNDATION_MAX][MODULUS_BORING_MAX];
	//
	long GetIndexBoringByElevation(double dElevation);

public: 
	void CalculateAll();

private:
	void InitVariables();
	void Calculate(long nTypeCalc);
	void CalculateValue(long nPos, long nTypeCalc, long nIdxBoring, CBoringData *pBoring);
};

#endif // !defined(AFX_MOUDLUSFOUNDATION_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_)
