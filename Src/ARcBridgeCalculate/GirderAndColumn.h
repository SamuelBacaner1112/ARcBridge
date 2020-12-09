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
	BOOL m_bGirder;	// 거더가 없는 경우 FALSE
	BOOL m_bColumn;	// 기둥인 경우 TRUE
	BOOL m_bWall;	// 내측벽체인 경우 TRUE

	// 보....
	double m_dAxialForceTop[RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// 지점부 상단의 축력 <= 모델링해석, 0 : 외측경간, 1 : 내측경간
	double m_dAxialForceBot[RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// 지점부 하단의 축력 <= 모델링해석
	double m_dSpanLengthGirder[RCBRIDGE_GIR_CHECK];	// 거더 적용지간

	double m_GirMomentPlus[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// 상.하부거더 정모멘트
	double m_GirMomentMinus[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];	// 상.하부거더 부모멘트
	double m_GirShear[2][RCBRIDGE_GIR_CHECK][RCBRIDGE_GIR_CHECK_ENV_LSD];		// 상.하부거더 전단력

	double m_dDivOutBeamPlus;
	double m_dDivOutBeamMinus;
	double m_dDivInBeamPlus;
	double m_dDivInBeamMinus;
	double m_dFactorShear;

	CString m_szCalcLn[RCBRIDGE_GIR_CHECK];

	// 기둥,중간벽체
	double m_dCtcColumn;	// 기둥적용 CTC
	double m_dAxialBot[RCBRIDGE_COL_CHECK];		// 기둥하단의 축력	  <= 모델링해석
	double m_dMomentBot[RCBRIDGE_COL_CHECK];	// 기둥하단의 모멘트  <= 모델링해석
	double m_dShearBot[RCBRIDGE_COL_CHECK];	// 기둥하단의 모멘트  <= 모델링해석
	double m_dColAxial[RCBRIDGE_COL_CHECK];		// 기둥의 축력
	double m_dColMoment[RCBRIDGE_COL_CHECK];	// 기둥의 모멘트
	double m_dColShear[RCBRIDGE_COL_CHECK];		// 기둥의 전단력
	long   m_nTypeDesign[RCBRIDGE_COL_CHECK];
	long   m_nCombo[RCBRIDGE_COL_CHECK];

	//
	CConcBasic *pConcGirEnd[2][3];		// 극한한계상태, 극단상황한계상태
	CConcBasic *pConcGirMid[2][3];
	CConcBasic *pConcColumn[RCBRIDGE_COL_CHECK];
	CConcBasic *pConcColumnLsd[4][RCBRIDGE_COL_CHECK_LSD];
};

#endif // !defined(AFX_GIRDERANDCOLUMN_H__97140CC4_BB9A_4C7E_8D10_A94C47FE2DD6__INCLUDED_)
