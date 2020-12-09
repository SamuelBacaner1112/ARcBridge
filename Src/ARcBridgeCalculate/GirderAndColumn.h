// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GirderAndColumn.h: interface for the CGirderAndColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIRDERANDCOLUMN_H__97140CC4_BB9A_4C7E_8D10_A94C47FE2DD6__INCLUDED_)
#define AFX_GIRDERANDCOLUMN_H__97140CC4_BB9A_4C7E_8D10_A94C47FE2DD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RCBRIDGE_COL_CHECK 4
#define RCBRIDGE_COL_CHECK_LSD 5
#define RCBRIDGE_GIR_CHECK 2

#define RCBRIDGE_GIR_POS_END	0
#define RCBRIDGE_GIR_POS_MID	1
#define RCBRIDGE_GIR_CHECK_ENV_LSD	5		// eCountEnvTypeLsd

class AFX_EXT_CLASS CGirderAndColumn : public CObject
{
public:
	void CalulateForce();
	CGirderAndColumn();
	virtual ~CGirderAndColumn();

	// 
	long m_nJijum;
	BOOL m_bGirder;	// �Ŵ��� ���� ��� FALSE
	BOOL m_bColumn;	// ����� ��� TRUE
	BOOL m_bWall;	// ������ü�� ��� TRUE

	// ��....
	double m_dAxialForceTop[RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// ������ ����� ��� <= �𵨸��ؼ�, 0 : �����氣, 1 : �����氣
	double m_dAxialForceBot[RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// ������ �ϴ��� ��� <= �𵨸��ؼ�
	double m_dSpanLengthGirder[RCBRIDGE_GIR_CHECK];	// �Ŵ� ��������

	double m_GirMomentPlus[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// ��.�ϺΰŴ� �����Ʈ
	double m_GirMomentMinus[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// ��.�ϺΰŴ� �θ��Ʈ
	double m_GirShear[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];		// ��.�ϺΰŴ� ���ܷ�

	double m_dDivOutBeamPlus;
	double m_dDivOutBeamMinus;
	double m_dDivInBeamPlus;
	double m_dDivInBeamMinus;
	double m_dFactorShear;

	CString m_szCalcLn[RCBRIDGE_GIR_CHECK];

	// ���,�߰���ü
	double m_dCtcColumn;	// ������� CTC
	double m_dAxialBot[RCBRIDGE_COL_CHECK];		// ����ϴ��� ���	  <= �𵨸��ؼ�
	double m_dMomentBot[RCBRIDGE_COL_CHECK];	// ����ϴ��� ���Ʈ  <= �𵨸��ؼ�
	double m_dShearBot[RCBRIDGE_COL_CHECK];	// ����ϴ��� ���Ʈ  <= �𵨸��ؼ�
	double m_dColAxial[RCBRIDGE_COL_CHECK];		// ����� ���
	double m_dColMoment[RCBRIDGE_COL_CHECK];	// ����� ���Ʈ
	double m_dColShear[RCBRIDGE_COL_CHECK];		// ����� ���ܷ�
	long   m_nTypeDesign[RCBRIDGE_COL_CHECK];
	long   m_nCombo[RCBRIDGE_COL_CHECK];

	//
	CConcBasic *pConcGirEnd[2][3];		// �����Ѱ����, �شܻ�Ȳ�Ѱ����
	CConcBasic *pConcGirMid[2][3];
	CConcBasic *pConcColumn[RCBRIDGE_COL_CHECK];
	CConcBasic *pConcColumnLsd[4][RCBRIDGE_COL_CHECK_LSD];
};

#endif // !defined(AFX_GIRDERANDCOLUMN_H__97140CC4_BB9A_4C7E_8D10_A94C47FE2DD6__INCLUDED_)
