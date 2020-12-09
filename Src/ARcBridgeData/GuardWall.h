// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GuardWall.h: interface for the CGuardWallRC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDWALLRC_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
#define AFX_GUARDWALLRC_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GW_REBAR_VERT_1		0	// ��ȣ�� ����ö��
#define GW_REBAR_VERT_2		1
#define GW_REBAR_VERT_3		2
#define GW_REBAR_VERT_4		3
#define GW_REBAR_VERT_5		4
#define GW_REBAR_HORI_1 	5	// ��ȣ�� ����ö��
#define GW_REBAR_SHEAR_1	6	// ��ȣ�� ����ö��
#define GW_REBAR_SHEAR_2	7	
#define GW_REBAR_SHEAR_3	8
#define GW_REBAR_EXT_1		9	
#define GW_REBAR_EXT_2	   10	
#define GW_REBAR_EXT_3	   11	
#define GW_REBAR_EXT_4	   12	
#define GW_REBAR_COUNT	   13

#define GW_REBAR_GUGAN	 	5

class AFX_EXT_CLASS CGuardWallRC : public CObject  
{
public:
	CGuardWallRC();
	virtual ~CGuardWallRC();

public:
	BOOL IsForm();
	BOOL IsTypeHDanNonRebarConcrete();
	CString GetStringDia(double dDia);
	int     m_nTypeGuardWall;	// ��ȣ�� Ÿ��, ���α�, ö����, ���α� ���߶��, ö���� ���߶�� �� ��� ���� �ٸ�.
	long	m_nNumberHDan;		// ��ȣ���� �����ִ� Ⱦ�� ��ȣ
	BOOL	m_bRailRoad;		// TRUE=ö���� ��ȣ��, FALSE=���α� ��ȣ��
	BOOL	m_bLeftGuardFence;	// ���� ��ȣ�� ���� �����Ͽ� ������ �߾ӿ��� ������ �ִ����� ����
	double	m_dWidhtHDan;		// Ⱦ�ܿ��� �Է¹��� �ʺ�
	CDPoint m_ZzxyGW;			// ������� ����. ������ ��. ��ȣ�� ������(�¿���:������ �ϴ�, ��:��� �ϴ�)�� ��ǥ
	BOOL	m_bSoundProof;		// ��ȣ��,�ߺд�,�ߺд�1/2,�������� Ÿ���� ���������ʿ� ���� �������?

	double  m_dDistFrLeft;		// �Է¹��� Ⱦ�� �ʺ� �������κ��� ��ȣ������ �̰ݰŸ�
	double	m_dW1;
	double	m_dW2;
	double	m_dW3;
	double	m_dW4;
	double	m_dW5;
	double	m_dH1;
	double	m_dH2;
	double	m_dH3;			// ���� ������, m_dH1 ��� ��ġ
	double	m_dR1;			// ���� ������, m_dH2 ��� ��ġ
	double	m_dR2;			// ���� ������, m_dH3 ��� ��ġ
	double	m_dR3;			// ���� ������, m_dH3 ��� ��ġ
	double	m_dC;			// ��� �����, m_dH3 ��� ��ġ
	double	m_dW6;			// ��ȣ���� Ȧ�� ���� ���, Ⱦ���������� �̰ݰŸ�
	double	m_dW7;			// ��ȣ���� Ȧ�� ���� ���, Ȧ �ʺ�
	double	m_dH4;			// ��ȣ���� Ȧ�� ���� ���, Ⱦ���ϸ���� ����
	double	m_dH5;			// ��ȣ���� Ȧ�� ���� ���, Ȧ ����
	//Rebar
	double	m_dHR;			// ����ö�� ����
	double	m_dVR;			// ����ö�� ����
	double	m_dSR;			// ����ö�� ����
	double	m_dEC;			// ����ö���� ��ö�� ������
	double	m_ddc1;			// ���� �Ǻ�
	double	m_ddc2;			// ���� �Ǻ�
	double	m_ddc3;			// ������ �Ǻ�
	double	m_dF1;			// Slab�� ���Ե� ö�� ���� 1	,	ö���� �϶� �������� ���� ��� ö�� ����
	double	m_dF2;			// ö�� ���� 2					,	ö���� �϶� �������� �Ʒ��� ��� ö�� ����
	double	m_dF3;			// ö�� ���� 3					,	ö���� �϶� ������� ��� ö�� ����
	double	m_dE;			// V ����ö�� ����
	double	m_dEH;			// H ����ö�� ����
	double	m_dF;			// V ����ö�� ���� ����			,	ö���� �϶� ������� ������ ��� ö�� ����
	double	m_dFH;			// H ����ö�� ���� ����
	double	m_dL1;			// Slab�� ���Ե� ��
	double	m_dCTC;			// ��ö�� ����
	double	m_dSC;			// ����ö�� ����
	double	m_dCrossF2;
	double	m_dCrossF3;
	double	m_dForce;
	long	m_nUGugan;
	long	m_nSGugan;
	long	m_nU[GW_REBAR_GUGAN];
	long	m_nS[GW_REBAR_GUGAN];
	double	m_dU[GW_REBAR_GUGAN];
	double	m_dS[GW_REBAR_GUGAN];

	long	m_nTypePave;				// ���� ����
	long	m_nTypeInnerFill;			// ��ä�� ����
	double	m_dUnitWeightPave;			// ���� �����߷�
	double	m_dUnitWeightInnerFill;		// ��ä�� �����߷�
	double	m_dThickPaveFootway;		// ���� ���� �β�
	double	m_dSlopeUpper;				// ���� ���

	double	m_dFck;
	double	m_dFy;
	double  m_dAggregate;				// ���� �ִ� ġ��

	BOOL	m_bGWhalf;					// �ߺд� ��ü�� ���
	long	m_nTypeRebar;				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)
	BOOL	m_bDWload;					// DW �������� ����
	
	long GetNumInnerFill(CString strName);
	long GetNumPave(CString strName);
	CString GetStringInnerFill(long nIndex);
	CString GetStringPave(long nIndex);

	CStringArray m_sArrTypePave;	// ���� ���� ���� (������, ��ũ��Ʈ)
	CStringArray m_sArrInnerFill;	// ���� ä���� ���� (��ũ��Ʈ, ��Ż, ��)

	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// ��ũ��Ʈ ����
	
	void Serialize(CArchive& ar);
	CGuardWallRC& operator=(const CGuardWallRC& Obj);
	BOOL operator==(const CGuardWallRC& rhs);

	double	GetWidth() { return m_dW1+m_dW2+m_dW3+m_dW4+m_dW5; }	//���� �Ÿ�(m_dDistFrLeft)�� �������� ���� ��ȣ������ �ʺ�
	double	GetHeight() { return m_dH1+m_dH2+m_dH3; }
//	void	GetXyGuardFenceBase(CDPoint A[9]);

	BOOL	IsTypeHDanOnlyNone();				// ���� ����, ö�� ����(����, ö��)
	BOOL	IsTypeHDanRoadGuardFence();			// ��ȣ��	
	BOOL	IsTypeHDanRoadCentralWallFull();	// �ߺд�
	BOOL	IsTypeHDanRoadCentralWallHalf();	// �ߺд�1/2
	BOOL	IsTypeHDanRoadFootway();			// ����
	BOOL	IsTypeHDanRoadFootwayBlock();		// ������ ��輮
	BOOL	IsTypeHDanRoadSoundProof();			// ������ ����
	BOOL	IsTypeHDanRoadRetainingWall();		// �븷�̺�	
	BOOL	IsTypeHDanRoadHandRail();			// ��������	
	BOOL	IsTypeHDanRoadDike();				// ����ũ

	BOOL	IsTypeHDanRailGuardFence();			// ����	
	BOOL	IsTypeHDanRailDetectGravel();		// �ڰ�����(ö��)
	BOOL	IsTypeHDanRailDetectGravel2();		// �ڰ�����(����)
	BOOL	IsTypeHDanRailFootway();			// ����
	BOOL	IsTypeHDanRailDosangVer();			// ����ĵ���
	BOOL	IsTypeHDanRailDosangGravel();		// �ڰ�����
	BOOL	IsTypeHDanRailFootwayBlock();		// ����+����
	BOOL	IsTypeHDanRailHole();				// ��ű�
	BOOL	IsTypeHDanRailHoleFootway();		// ��ű�+����
	BOOL	IsTypeHDanRoadRailDike();			// ����, ö�� ����ũ
	
	BOOL	IsTypeHDanRoadTotalGuardWall();						// ���α� ���� �� ������ ��� ��, ö������ ��� ��� TRUE
	BOOL	IsTypeHDanRoadTotalGuardFence();					// ���α� ����, ���� �� ������ ��� ��, ö������ ��� ��� TRUE
	BOOL	IsTypeHDanRoadTotalGuardFenceAndCentralWall();		// ���α� ��ȣ��, �ߺд�, �ߺд�1/2, ö������ ��� ��� TRUE
	BOOL	IsTypeHDanRoadTotalCentralWall();					// ���α� �ߺд�, �ߺд�1/2, ö������ ��� ��� TRUE

	BOOL	IsTypeHDanRailTotalHole();			// ö���� ��ű�, ��ű�+����
	BOOL	IsTypeHDanRailTotalDosang();		// ö���� ����ĵ���, �ڰ�����

	BOOL	IsLeftGuardFence();
	BOOL	IsRound();			// m_dR3>0 �ϰ�쿡�� TRUE
	BOOL	IsPaveOnHDan();		// Ⱦ�ܿ��� ����, �����Ż �� ������ ����

	void	SetDataDefaultGuardWall(double thickPave, BOOL bUnderGroundRahmen = FALSE);
	void	SetDataDefaultGuardWallDoro(double thickPave, BOOL bUnderGroundRahmen);
	void	SetDataDefaultGuardWallRail(double thickPave, BOOL bUnderGroundRahmen);

	void	GetTvGuardWallBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer=TRUE, BOOL bForBM=FALSE, double dSlopeSlab=0.0);
	void	GetTvFootwayBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer=TRUE, BOOL bInclPave=TRUE, double dSlopeSlab=0.0);
	void	DimGuardWallBase(CDomyun* pDomP, BOOL bInput, BOOL bWriteLeft, BOOL bWriteRigh);	//	��ȣ���� ġ�� �׸�
	void	ArrowDirectionForSlope(CDomyun *pDomP, CDPoint xy, CString str, CDPoint vDir);	// ����, ��ȣ���� ��� ��縦 ǥ��(ȭ��ǥ �� �ؽ�Ʈ)
};

#endif // !defined(AFX_GUARDWALL_H__46E1A811_2F9A_4AEC_8253_2A4EF414F596__INCLUDED_)
