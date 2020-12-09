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

#define RIGIDITY_DISPX		0   // X�� ��������
#define RIGIDITY_DISPY		1	// Y�� ��������
#define RIGIDITY_DISPZ		2	// X�� ��������
#define RIGIDITY_ROTATEX	3	// X�� ȸ������
#define RIGIDITY_ROTATEY	4	// Y�� ȸ������
#define RIGIDITY_ROTATEZ	5	// Z�� ȸ������
#define RIGIDITY_COUNT		6

class CFootingApp;
class CBoringData;

// �����ؼ��� ���� ������ ������ ���
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
	double m_ValueK[RIGIDITY_COUNT];	// ������ ������

	BOOL   m_bCombineValue;
	double m_CombK[RIGIDITY_COUNT];		// ��ä�򰭼��� ���յ� ��

	// ��ä����(���)�� ����
	double m_KsBackFill;
	double m_KrBackFill;
	double m_dHeightBackFill; // ��ä������� �𵨸����ǳ���
	double m_dWidthBackFill;  // ��ä������� �� (��ü�� ��)
	double m_dH1;

public: 
	void CalculateRigidity();
	void CalculateBackFillAndCombine(double dHeightBackFill, double dWidthBackFill);

private:
	void CalculateRigidityPileBase();
	void CalculateRigidityDirectBase();
};

#endif // !defined(AFX_RIGIDITYFOOTING_H__D13D6404_C2B1_4E43_BF13_A2FD69D0E0EA__INCLUDED_)
