// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WingData.h: interface for the CWingData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_)
#define AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���
#define WW_REBAR_SLOPE_A_IN					0	// A���� ��� ��� ö��
#define WW_REBAR_HOR_A_IN					1	// A���� ���� ö��
#define WW_REBAR_HOR_A_IN_VAR_SEC			2	// A���� ���� ���ܸ� ö��
#define WW_REBAR_HOR_A_IN_VAR_SEC_BLOCK		3	// A���� ���� ���ܸ� ���� ö��
#define WW_REBAR_HOR_B_IN_VAR				4	// B���� ���� ��ȭ ö�� 
#define WW_REBAR_HOR_B_IN					5	// B���� ���� ö��
#define WW_REBAR_HOR_B_IN_VAR_SEC			6	// B���� ���� ���ܸ� ö��
#define WW_REBAR_HOR_B_IN_VAR_SEC_VAR		7	// B���� ���� ���ܸ� ��ȭ ö��
#define WW_REBAR_HOR_B_IN_INC_FOOT			8	// B���� ���� ���ʾ���             *************
#define WW_REBAR_SLOPE_D_IN					9	// D���� ��� ��� ö��
#define WW_REBAR_HOR_D_IN					10	// D���� ���� ö��
#define WW_REBAR_HOR_D_IN_VAR				11	// D���� ���� ��ȭ ö��
#define WW_REBAR_HOR_D_IN_VAR_SEC			12	// D���� ���� ���ܸ� ö��
#define WW_REBAR_HOR_D_IN_VAR_SEC_BLOCK		13	// D���� ���� ���ܸ� ���� ö��
#define WW_REBAR_HOR_D_IN_VAR_UNDER			14	// D���� ���� ��ȭö�� ���ܸ鱸�� �Ʒ� ö��
#define WW_REBAR_HOR_D_IN_VAR2					15	// D���� ���� ��ȭ ö��2

#define WW_REBAR_VER_A_IN					18	// A���� ���� ö��
#define WW_REBAR_VER_A_IN_VAR_SEC			19	// A���� ���� ���ܸ� ö��
#define WW_REBAR_VER_C_IN					20	// C���� ���� ö��
#define WW_REBAR_VER_C_IN_EXC_FOOT			21	// C���� ���� ö�� ���� �ٱ���
#define WW_REBAR_VER_D_IN					22	// D���� ���� ö��

#define WW_REBAR_VER_D_IN_VAR_SEC			23	// D���� ���� ���ܸ� ö��		- ���ι��� ���ܸ��ϰ�� ���ܸ� ���� �Ʒ�
#define WW_REBAR_VER_D_IN_VAR_SEC_BLOCK		24	// D���� ���� ���ܸ� ���� ö��	- ���ι��� ���ܸ��ϰ�� ���ܸ� ������
#define WW_REBAR_TAIL_END_IN				25	// D���� ���� ���� ö��
#define WW_REBAR_VER_D_IN_VAR_HU			26  // D���� ���� ���ܸ� ��ġ ö��	- ���ܸ���ġ�� �ִºκ�
#define WW_REBAR_VER_D_IN_VAR_UP			27	// D���� ���� ���ܸ� ��� 		- ���ι��� ���ܸ��ϰ�� ���ܸ� ���� ��
#define WW_REBAR_VER_D_IN_FLAT				28	// D���� ���� �ϴ� ���� ���� ö��
#define WW_REBAR_VER_D_IN_VAR_SEC_FLAT		29	// D���� ���� ���ܸ� ö��		- ���ι��� ���ܸ��ϰ�� ���ܸ� ���� �Ʒ�
// ����
#define WW_REBAR_SLOPE_A_OUT				31	// A���� ��� ��� ö��
#define WW_REBAR_HOR_A_OUT					32	// A���� ���� ö��
#define WW_REBAR_HOR_A_OUT_VAR_SEC_BLOCK	33	// A���� ���� ���ܸ� ���� ö��
#define WW_REBAR_HOR_B_OUT_VAR				34	// B���� ���� ��ȭ ö��
#define WW_REBAR_HOR_B_OUT					35	// B���� ���� ö��
#define WW_REBAR_HOR_B_OUT_INC_FOOT			36	// B���� ���� ���ʾ���             *************
#define WW_REBAR_SLOPE_D_OUT				37	// D���� ��� ��� ö��
#define WW_REBAR_HOR_D_OUT					38	// D���� ���� ö��
#define WW_REBAR_HOR_D_OUT_VAR				39	// D���� ���� ��ȭ ö��
#define WW_REBAR_HOR_D_OUT_VAR_SEC_BLOCK	40	// D���� ���� ���ܸ� ���� ö��
#define WW_REBAR_HOR_D_OUT_VAR2				41	// D���� ���� ��ȭ ö��2

#define WW_REBAR_VER_A_OUT					46	// A���� ���� ö��
#define WW_REBAR_VER_C_OUT					47	// C���� ���� ö��
#define WW_REBAR_VER_C_OUT_EXC_FOOT			48	// C���� ���� ö�� ���� �ٱ���
#define WW_REBAR_VER_D_OUT					49	// D���� ���� ö��

#define WW_REBAR_VER_D_OUT_VAR_SEC_BLOCK	50	// D���� ���� ���ܸ� ���� ö��
#define WW_REBAR_TAIL_END_OUT				51	// D���� ���� ���� ö��
#define WW_REBAR_VER_D_OUT_FLAT				52	// D���� ���� �ϴ� ���� ���� ö��
// ��Ÿ
#define WW_REBAR_REINFORCE_A				54	// A���� �亮 ����ö��
#define WW_REBAR_REINFORCE_B				55	// B���� �亮 ����ö��
#define WW_REBAR_HUNCH_WALL_UP				56	// ��ü ��� ��ġ ö��
#define WW_REBAR_HUNCH_WALL_LO				57	// ��ü �Ϻ� ��ġ ö��
#define WW_REBAR_HUNCH_FOOT					58	// ���ʺ� ��ġ ö��
#define WW_REBAR_SHEAR_A					59	// A���� ���� ö��
#define WW_REBAR_SHEAR_B					60	// B���� ���� ö��
#define WW_REBAR_SHEAR_C					61	// C���� ���� ö��
#define WW_REBAR_SHEAR_D					62	// D���� ���� ö��
// �߰�

#define WW_REBAR_COUNT						64	// ������ ö�� �� ����

class CTwinVectorAreaShape;

class AFX_EXT_CLASS CWingWall : public CObject
{
public:
	CWingWall();
	virtual ~CWingWall();
	void Serialize(CArchive& ar);

public:
	BOOL IsValid();
//	BOOL IsBlockA();
	BOOL IsBlockB();
	BOOL IsBlockC();
//	BOOL IsBlockD();
	void Clear();
	CWingWall& operator=(const CWingWall& Obj);
	long GetCountRebarSlop(int nOut);						// ������ ö�ٸ�ũ�Ҷ� ���ö�ٰ����� �����ؾ� ��ġ�� ����� �־.. 
	double GetEndEl(BOOL bIncludeDancha=FALSE);

	// �Ϲ� �Է�
	BOOL m_bExist;	// ������ ��ġ ����
	BOOL m_bRAngEnd;	// ������ ���� ó��
	long m_nDirHunch;	// ��ġ ����(0:��ȭ ����, 1:����, 2:����)
	double m_dEL;	// ���������� EL
	double m_dL1;	// ���������ܿ��� ������������� �Ÿ�
	double m_dL2;	// ����������ġ���� �亮������ �Ÿ�
	double m_dHL;	// L1�� ���� ��� L1 �κ��� ����
	double m_dH2;	// ������ ��� �����ϴܿ����� ����
	double m_dH3;	// ������ ��ܿ����� ���� ����
	double m_dT1;	// ������ �β�
	double m_dT2;	// ������ ���ܸ��� �β�
	double m_dLT;	// ���ܸ� �β������� ����
	double m_dHuW1;	// ���ʿ� �̷�� ��ġ ��
	double m_dHuH1;	// ���ʿ� �̷�� ��ġ ����
	double m_dHuW2;	// ��ü�� �̷�� ��ġ ��
	double m_dHuH2;	// ��ü�� �̷�� ��ġ ����
	double m_dSW;	// L1�� ���� ��� L2�� �̷�� ������
	double m_dSH;	// ���ܸ��� ����� ���ܸ� ������
	CDPoint m_vAng;	// ������ ����(������ ���� ���������� ����)
	long m_nAttachPos;	// ���� ��ġ(0: ��ü ���, 1: ��ü ����)
	BOOL m_bSWHor;	// TRUE�� sw�� �������� ������ SW���� 999999�� ���� Display���� �ʴ´�.

	BOOL m_bUseUserOutput3D;
	double m_dMomentUltPlate[5];	// A, B, C, D, A'
	double m_dMomentUsePlate[5];
	double m_dMomentUseJudge_Plate[5];
	double m_dShearUltPlate[5];	// A, B, C, D, A'
	double m_dShearUsePlate[5];	// A, B, C, D, A'

	// ö�� ��ġ [2][2] // ���,���� - 1��,2��
	long m_nCountLayerA[2];
	long m_nCountLayerB[2];
	long m_nCountLayerC[2];
	long m_nCountLayerD[2];
	CDPointArray m_xyArrUpper[2][2];	// CDPoint(����, ����)[In or Out]
	CDPointArray m_xyArrLower[2][2];	// CDPoint(����, ����)[In or Out]
	CDPointArray m_xyArrSide[2][2];	// CDPoint(����, ����)[In or Out]
	double m_dDiaMainA[2][2];			// A���� ��ö�� ����[In or Out]
	double m_dDiaMainB[2][2];			// B���� ��ö�� ����[In or Out]
	double m_dDiaMainC[2][2];			// C���� ��ö�� ����[In or Out]
	double m_dDiaMainD[2][2];			// D���� ��ö�� ����[In or Out]
	double m_dDiaSupportA[2][2];		// A���� ���ö�� ����[In or Out]
	double m_dDiaSupportB[2][2];		// B���� ���ö�� ����[In or Out]
	double m_dDiaSupportC[2][2];		// C���� ���ö�� ����[In or Out]
	double m_dDiaSupportD[2][2];		// D���� ���ö�� ����[In or Out]
	double m_dDiaHuUpper;			// ��ü ��� ��ġö�� ����
	double m_dDiaHuLower;			// ��ü �Ϻ� ��ġö�� ����
	double m_dDiaHuFoot;			// ������ ��ġö�� ����
	double m_dDiaShearA;			// A���� ����ö�� ����
	double m_dDiaShearB;			// B���� ����ö�� ����
	double m_dDiaShearC;			// C���� ����ö�� ����
	double m_dDiaShearD;			// D���� ����ö�� ����
	double m_dSpaceShearA;			// A���� ����ö�� ����
	double m_dSpaceShearB;			// B���� ����ö�� ����
	double m_dSpaceShearC;			// C���� ����ö�� ����
	double m_dSpaceShearD;			// D���� ����ö�� ����
	double m_dStdVerCTC;			// ǥ�� ���� ö�� ��� ����
	double m_dMainCTCA;				// A���� ��ö�� ��ġ ����
	double m_dMainCTCB;				// B���� ��ö�� ��ġ ����
	int m_nHunchCTC;				// ��ġ ö�� ����(0:���, 1:����)
	int m_nCountShearBind;			// ����ö�� ��ٽ� ��ö�� �������� ����
	int m_nRebarCurve;				// C���� ��ö�� ����� ó��(0:�ù漭, 1:����, 2:�)
	BOOL m_bUpperRebarSlope;		// ������ ��� ��ö���� ���� ��ġ
	int m_nCountSlopeBlock[2];		// ������ ������κ����� ��� ö�� ���� ����
	BOOL m_bSelMainCTCD;				// D���� ��ö�� ������ �⺻ ������ 2���
	long m_nTypeShearBC;			// B, C ���� ����ö�ٹ�� ���� - 0: ����, 1: ����, 2: �����
	BOOL m_bAddRebarReinforce;		// �亮����ö�� �߰�

	// �Ҵ� �Է�
	double m_dDistNoriStart;	// �Ҵ� ���۰Ÿ�(������ ������)
	double m_dSlopeNoriFirst;	// ���� ����(1��)
	double m_dSlopeNoriNext;	// ���� ����(2���̻�)
	double m_dHeightMaxSodan;	// �Ѵ� �ִ� ����
	double m_dSlopeSodan;		// �Ҵ� ���
	double m_dWidthSodan;		// �Ҵ� ��
	double m_dWidthSodanWallFront;	// ��ü ���鿡 ���� �Ҵ� ��
	double m_dElSodanBottom;		// �Ҵ� �ٴ� EL.
	BOOL m_bRetainWallFront;	// �˺� ��ġ ����

	double m_dHeightSoil;		// ���� ���ǰ�

	// ������ ���� ���� -  tvArr�� ����鼭 �Ǵ� �Ǵ� ������ [Serializ�� ���� �ʴ´�]..
	BOOL m_bAttachBraket;			// ������ü�� ������� �������� �پ��ִ���.
	BOOL m_bMatchBottom;			// ������ �븮�� �ٴڼ� �������� ������ ����( BOXŸ�� �λ������ �������� )

	void SetDataDefaultSodan(double dEl);
	// ö������
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar;
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebar_User;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// ��ũ��Ʈ ����

	// ����ö�� Ÿ�� ����
	CRebarShearDetail m_RebarShearDetailA;
	CRebarShearDetail m_RebarShearDetailB;
	CRebarShearDetail m_RebarShearDetailC;
	CRebarShearDetail m_RebarShearDetailD;
};

#endif // !defined(AFX_WINGDATA_H__A0D6C541_823E_11D4_8AF7_00105A75D74A__INCLUDED_)
