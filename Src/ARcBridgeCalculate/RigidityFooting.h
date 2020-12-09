// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RigidityFooting.h: interface for the CRigidityFooting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGIDITYFOOTING_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_)
#define AFX_RIGIDITYFOOTING_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RIGIDITY_DISPX		0   // X축 변위강성
#define RIGIDITY_DISPY		1	// Y축 변위강성
#define RIGIDITY_DISPZ		2	// X축 변위강성
#define RIGIDITY_ROTATEX	3	// X축 회전강성
#define RIGIDITY_ROTATEY	4	// Y축 회전강성
#define RIGIDITY_ROTATEZ	5	// Z축 회전강성
#define RIGIDITY_COUNT		6

class CFootingApp;
class CBoringData;

// 내진해석을 위한 기초의 강성을 계산
class AFX_EXT_CLASS CRigidityFooting : public CObject    
{
public:
	CRigidityFooting(CFootingApp *pFooting, CBoringData *pBoring, long nJijum);
	virtual ~CRigidityFooting();

public: 
	CFootingApp *m_pFooting;
	CBoringData *m_pBoring;
	CARcBridgeCalcStd *m_pStd;

	long m_nJijum;

	double m_D;
	double m_B;
	double m_L;
	double m_Es_Base;
	double m_Es_Wall;
	double m_v;
	double m_G;
	//
	double m_Ro[RIGIDITY_COUNT];
	double m_Ko[RIGIDITY_COUNT];
	double m_alpha[RIGIDITY_COUNT];
	double m_beta[RIGIDITY_COUNT];
	double m_ValueK[RIGIDITY_COUNT];	// 기초의 강성값

	BOOL   m_bCombineValue;
	double m_CombK[RIGIDITY_COUNT];		// 뒷채움강성과 조합된 값

	// 뒷채움토(토사)의 강성
	double m_KsBackFill;
	double m_KrBackFill;
	double m_dHeightBackFill; // 뒷채움토사의 모델링상의높이
	double m_dWidthBackFill;  // 뒷채움토사의 폭 (벽체의 폭)
	double m_dH1;

public: 
	void CalculateRigidity();
	void CalculateBackFillAndCombine(double dHeightBackFill, double dWidthBackFill);

private:
	void CalculateRigidityPileBase();
	void CalculateRigidityDirectBase();
};

#endif // !defined(AFX_RIGIDITYFOOTING_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_)
