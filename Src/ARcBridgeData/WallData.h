// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WallData.h: interface for the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_)
#define AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WALL_TYPE_VER		0
#define WALL_TYPE_SLOPE		1
#define WALL_TYPE_V			2

#define MAX_COLUMN			30	// �� ��ü�� ���� ����� ���� (20060209 10������ 30���� �ø�)
#define MAX_REBAR_DAN	3
#define GAP_FOOTING_LOWER	0			// �����ϸ鿡 ������ ö���� �� 

// ö�ٸ�ũ ������ ���� ���� ���� ///////////////////////////////
#define MARK_LEFT	0	// ���� ö�� ��ũ
#define MARK_RIGHT	1	// ���� ö�� ��ũ
/////////////////////////////////////////////////////////////////

// ��Ÿö�� ////////////////////
// ���ö��
#define W_REBAR_S_L1		0	// ���ö�� ���� 1��.
#define W_REBAR_S_L2		1	// ���ö�� ���� 2��.
#define W_REBAR_S_L3		2	// ���ö�� ���� 3��.
#define W_REBAR_S_R1		3	// ���ö�� ���� 1��.
#define W_REBAR_S_R2		4	// ���ö�� ���� 2��.
#define W_REBAR_S_R3		5	// ���ö�� ���� 3��.
#define W_REBAR_S_SU		6	// ���ö�� ��

// ����ö��
#define W_REBAR_V			0	// ����ö��
#define W_REBAR_V_SU		1	// ����ö�� ����

// ��Ÿö��  
#define W_REBAR_ETC_SU		0	// ��Ÿö�� ����
////////////////////////////////


const double dRateWidthInWall			= 0.1;		// �ֻ�ܿ��� ���ϴܱ����� ���� �������� �β�
const double dRateWidthFootingIn		= 0.6;		// �ֻ�ܿ��� ���ϴܱ��� ���̿� ���� ������ �ʺ� ����(����)
const double dHeightFootingIn			= 800;		// ������ü�� �Ϲ����� ���� ����
class CRebarShearDetail;
class CFootingApp;
class CColumnApp;
class CFactor;

class AFX_EXT_CLASS CWallData : public CObject  
{
public:
	void SaveCountJointRebar();
	double GetAngleByVert(double dAngle, double dAngleApply);
	
	double GetAngleByAngle(double dAngle, double dAngleApply);
	BOOL GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2);
	double GetCrossSectionCenterOfRoad(BOOL bLeft);
	void SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr);
	CString GetMarkRebarShear(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	CString GetMarkRebarSupport(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	void CheckSerialize();
	CString GetMarkRebarColumn(long nType, BOOL bInput, BOOL bForBM, long nBaseNum=-1, CString sDescription=_T(""));
	CString GetCircleText(long nNumber);
	long GetCountCycleMainRebar();
	CString GetMarkRebarMain(long nType, BOOL bInput, long nCycle, BOOL bOnlyInit=FALSE, BOOL bForBM=TRUE, CString sDecription=_T(""), long nDan=0);
	double GetCoverUpper(BOOL bUpper, long nDan, BOOL bApplyVertForSlope=FALSE);
	double GetCoverFront(BOOL bFront, long nDan);
	void CopyPlaceRebar_ShearRebar(CWallData *pWall);
	void CopyPlaceRebar_SupportRebar(CWallData *pWall);
	void InitWall();
	CWallData& operator=(const CWallData& Obj);
	void Serialize(CArchive &ar);
	long m_nFlag;

	CRebar* GetRebarMainByDescription(CString sDescription);
	CRebar* GetRebarSupportByDescription(CString sDescription);
	CRebar* GetRebarShearByDescription(CString sDescription);
	CRebar* GetRebarColumnByDescription(CString sDescription);
	
	CWallData();
	virtual ~CWallData();

	double m_dWidthExp[2];	// ��±� ����� ���� ������ �����θ� �����ϱ� ����(��±ٹ�ٽø� �����)
	// (ARCBRIDGE-2819) ������ ������ ��ö�� �纸����ġ�� ������ ö�� ��� ��������ö�� ��� ������ ������ �ִ´�.
	double m_dWidthOverRebarExp[2];

	// ��ü���� ���ʴ� �ϳ��� �� �ִ�. ���� ���°�쵵 ���� �� ����.
	CFootingApp m_footing;
	CFootingApp m_footingByRotateToVert;	// �������� ���������� ����
	CTwinVectorArray m_tvArrFront;	// ��ü�� ��ǥ�� ������ �ִ� ����
	CTwinVectorArray m_tvArrPlan;	// ��ü�ܸ� ��ǥ
	CTwinVectorArray m_tvArrLowerLimit_Front;	// ��ü�� �Ϻ� �Ѱ�(������ ������, ��ġ���� ��� ��ü�Ϻθ� �߶� �ʿ䰡 ���� �� �ִ�.)
	CTwinVectorArray m_tvArrLowerLimit_FrontRebar;	// ��ü ö���� �Ϻ� �Ѱ�(������ ������, ��ġ���� ��� ��ü�Ϻθ� �߶� �ʿ䰡 ���� �� �ִ�.)
	double m_dAngleUpperSlab_Front;		// ��ν����� ����
	double m_dAngleUpperWall_Front;		// ��ü ��� ����
	// ��ü���� -------------------------------------------------------
	BOOL m_bIs;
	CDPoint m_xyOrg;		// ������ ��ġ
	long m_nType;			// ��ü ����(0 : ����, 1 : ���� �ִ°�, 2 : v����);
	BOOL m_bRoundRect;		// ����� ��ü����?(����� ������ ��쿡�� ����)
	BOOL m_bSideIsArc;		// �¿������� ��ũ����(��ġ������ ���� ���¸� ǥ���ϱ� ���� ������, ���� ��ü�� �̰� ���� ����)

	// �Ϲ�����
	double m_dELBDiff;		// ����ϴ� EL���� �߰��� ���ؾ� �Ǵ� el(�������)�Է¹޴� ����Ÿ�� �ƴϹǷ�
	double m_dDiffTop[2];		// Box�������� ��ܰ� �ϴ��� Diff���밪�� �޶����⶧���� ���� ������.
	// {������ġ, ����, ����} Box�������� ��ܰ� �ϴ��� Diff���밪�� �޶����⶧���� ���� ������.

	double m_dELB;			// ����ϴ� EL
	double m_dW;			// ��ü �ʺ�
	double m_dH;			// ��ü ����(����)
	double m_dAngle;		// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
	BOOL m_bApplyAngleEnd;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
		
	// V����
	double m_dELRT;			// V�� ������ Top
	double m_dELLT;			// V�� ������ Top
	double m_dWL;
	double m_dWR;
	double m_dLR;
	double m_dAngleV;		// V������ ���̰�
	double m_dR1;			// V�������� ���� �� ������(������� ���߿�)
	double m_dR2;			// V�������� ������ �� ������(������� ���߿�)

	// ��ս��� ������ġ�� ��ü�� ������ġ���� ������ �Ÿ��� ������ �Ǵ���.
	CColumnApp m_Column;				// ��ü �ܸ��� ��ս��� ��� ��ü��� �� ����� �����.
	long m_nCountColumn;				// ��ս��� ��� ��� ����
	double m_dDistColumn[MAX_COLUMN];	// �� ��հ��� �Ÿ�
	BOOL m_bRevDirPlacingColumn;		// ��� ��ġ ������ �н��� �ݴ��������...
	double m_dAnglePath;				// ��Ÿ������ ������ �簢(90������)

	// �������
	BOOL m_bTopIsColumn;			// ����� ��ս�����...?
	CTwinVector m_tvPathPlan;		// ���� ��ü�� ���� ��.
	CTwinVector m_tvPathPlanLower;	// ���󿡼� ��ü �ϸ��� ���� ��.
	double m_dMarginLeft;			// ��ü�� path���� ����ġ���� �̰ݵǴ� �Ÿ�
	double m_dMarginRight;			// ��ü�� path���� ������ġ���� �̰ݵǴ� �Ÿ�
	BOOL m_bRAngLeft;				// path�� ���ο� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	BOOL m_bRAngRight;				// Path�� ���ۺο� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	CDPoint m_vAngLeft;				// ������ ����(�����Ⱦ��� 0��)
	CDPoint m_vAngRight;			// ������ ����(�����Ⱦ��� 0��)
	double m_dOffsetStt;			// Path���ۺ� ��������� �������� �� �Ÿ�
	double m_dOffsetEnd;			// Path���� ��������� �������� �� �Ÿ�

	// �����鵵 ��� ����
	CTwinVectorArray m_tvArrLeftUpper;	// �����鵵 ��� ������ ����(��ü �ʺ�)
	CTwinVectorArray m_tvArrLeftUpperSlab;	// �����鵵 ��ν����� ���� ����
	double m_dDiffHLeftByCenter;		// ��ü��� �߽ɰ� �������� ����(�ø����������)
	double m_dDiffHRightByCenter;		// ��ü��� �߽ɰ� �������� ����(�ø����������)
	double m_dThickUppeSlab;			// ��ν����� �β�.
	double m_dThickHunch;				// ��ν����� ��ġ �β�.
	double m_dThickHunchL;				// ��ν����� ��ġ �β�.����
	double m_dThickHunchR;				// ��ν����� ��ġ �β�.����

	double m_dThickLowerSlab;			// �Ϻν����� �β�.
	double m_dThichLowerHunch;			// �Ϻν����� ��ġ �β�.(������)
	double m_dBottomElCorrect;			// �Ϻν����갡 ���� ��ġ�� ���°�� ��ü�� ª�� �׷����� ������ �ϴ�EL�� �༭ ��������.
	// ------------------------------------------------------ ��ü����

	// �Ǻ� -------------------------------------------------------
	double m_dCoverLeft[MAX_REBAR_DAN];	// ��ü ���� �Ǻ�
	double m_dCoverRight[MAX_REBAR_DAN];	// ��ü ������ �Ǻ�
	double m_dCoverFrontBack[MAX_REBAR_DAN];	// ��ü �����ĸ�
	//------------------------------------------------------- �Ǻ� 


	// ��� ----------------------------------------------------------
	BOOL m_bDetailInput23DanMainRebar;			// 2,3�� ��ö���� 2Cycle 1�� �Է��ϵ��� ���������� �����ؼ� �Է� �� �� �հ� �Ѵ�.
	BOOL m_bNotRebar;						// ö���� �ƹ��ŵ� ������ ����(���� ������ ����)
	long m_nTypeRebarFront;					// ���鵵 �׸��� Ÿ��(0:�¿�������, 1:�߽ɱ���) - �ø��� ���� �� ����
	long m_nIdxCTC_MainRebar;				// ��ö�� CTC
	double m_dCTC_MainRebar;				// index���� double�� ����(2005.04.25)
	BOOL m_bSameOffsetPlaceTAndCBlock;		// ������౸���� ���� �������� ��ġ		
	BOOL m_bPlacing23DanByStandardOffset;	// 2,3�� ��±� ǥ�� �������� ��ġ����?
	BOOL m_bPlacingInWallRebarOfCycle24;	// 2,4����Ŭ�� ���ؼ� ö���� ��������...??
	long m_nTypeCurveOfVertRebar;			// ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
	BOOL m_bApplyAngleToMainRebarCTC;		// ��ö�� CTC�� �簢�� ���� �Ǿ�� �ϴ���?(��ν����갡 �������� ��ġ��� ������ �Ǿ�� ��)
	double m_dRangeSupportRebarForSameUpperSlab;	// ��ν����� ó�� ���� ��±� ����
	CPlaceRebarByBlock m_placeByBlock_SupportRebarAddSameUpperSlab;	// ��ν����� ó�� ���� ��±��� �߰��� ����
	double m_dDistAddSupportRebarForSameUpperSlab[2];	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	BOOL m_bRebarLowerSettle[2];				// ��ü�ϴ� ö�� ����������


	CTwinVectorArray m_tvArrCuttingArea_Side_For1Dan;	// 1�� ö���� �Ĵ� �뵵�� �ı� ����(������ �� ��)

	// ���� ����
	long m_nCountJointMainRebar_Cycle1[2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_Cycle2[2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����

	// ���� ����(������ ������) - ���� ���� ����� ����� �κи� �ǰ���� �ؾ� �ϱ� ������(Seralize�� ���� ����) //////
	long m_nCountJointMainRebar_Cycle1_Old[2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	long m_nCountJointMainRebar_Cycle2_Old[2][MAX_REBAR_DAN];	// ������ü ������,�¿��� ���� ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// rebar info cycle
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> m_pArrRCMain[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �¿����� rebar info cycle
	
	// ��������
	CDoubleArray m_dArrDistRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �¿��� ��ü ö�� ���� ����

	// ö������
	CDoubleArray m_dArrDiaRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �¿��� ��ü ö�� ����

	// ö�����翩��
	CDoubleArray m_dArrExistRC[MAX_REBAR_CYCLE][2][MAX_REBAR_DAN];	// �¿��� ��ü ö�� ���� ����

	// ��ġ�ܼ� ����/ö������ ����
	CSetRebarDan m_setRebarDan[2];	// ��ü �¿��� �ܼ� ����

	double m_dCoverUpper_UpperSlab;				// ��ν����� ����� �Ǻ�
	double m_dCoverLower_LowerSlab;				// �Ϻν����� �ϸ��� �Ǻ�
	CTwinVectorArray m_tvArrUpperLimit_Front;	// ���鵵 �󿡼� ��ν����� ���
	CTwinVector m_tvUpperLimitOfMainRebar;		// ����� ��ö���� ��� �Ѱ�(RcBridge������ ��ν����� ��� ��ö�� 1���� ��ǥ�� �ǹ�)
	CTwinVector m_tvLowerLimitOfMainRebar;		// ����� ��ö���� gk�� �Ѱ�(RcBridge������ �Ϻν����� ��� ��ö�� 1���� ��ǥ�� �ǹ�)
	CTwinVector m_tvUpperLimitOfSupportRebar;	// ��±�/����ö���� ��� �Ѱ�(RcBridge������ ��ν����� �ϸ� ��ǥ)
	CTwinVector m_tvLowerLimitOfSupportRebar;	// ��±�/����ö���� ��� �Ѱ�(RcBridge������ ��ν����� �ϸ� ��ǥ)
	long m_nIdxCountCycleMainRebar;				// ��ö�� ����Ŭ ��.
	long m_nCountWrapMainRebar;					// ����ö���� ö���� ���δ� ������
	long m_nTypeDefaultCycle3;					// 3cycle ����Ÿ��
	
	// ǥ�شܸ� ö�� ��ġ ���� ������ (���ö��, ����ö��)
	CPlaceRebarByBlock m_placeByBlock_SupportRebar[2][MAX_REBAR_DAN]; // �¿���
	CPlaceRebarByBlock m_placeByBlock_ShearRebar;
	long m_nIdxCTC_SupportAndShearRebar;			// ���,����ö�� ǥ�ع�ġ����
	double m_dCTC_SupportRebar;				// index���� double�� ����
	double m_dCTC_ShearRebar;

	CRebarShearDetail m_rebarShearDetail;	// ����ö��

	// ������°��� �ɼ�
	long m_nTypeOutputMainRebar_Side;	// ���� ��ö�� ��� �ɼ�(0 : �⺻, 1 : �ı�)
	//---------------------------------------------------------- ��� 

	// ��Ÿ
	double m_Fck;
	double m_Fy;
	double m_FyShear;
	double m_dLengthRebarMax;
	CString m_sNameRight;	// ��ü ������ �ش��ϴ� �̸�
	CString m_sNameLeft;	// ��ü ������ �ش��ϴ� �̸�
	CBasicConcInfo *m_pBasicConcInfo;

	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// ��ũ��Ʈ ����
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarMain;			// ��ö��
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarSupport;		// ���ö��
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarShear;			// ����ö��
	CSafetyTypedPtrArray <CObArray, CRebar*> m_pArrRebarColumn;			// ��պ� ö��

	double m_dCenterOfWall;				// ��ü�߽� (�������� ����)

	// ġ�� ���Կ�
	CTwinVectorArray m_tvArrRebarMainSideForDim;	// ������ ��� �������� ��ö���� ��ġ�� ���� �ؾ� �ȴ�.(ġ�����Կ�)

	// �ӵ� ������
	CDPoint m_xyOrgOld;

	// ö�� ������ ���� �ɼ�
	CDockingLong m_nTypeAddRate;		// 0 : ���溰, 1 : ��ö��,��Ÿö��
	CDockingDouble m_dAddRateLower;		// ���� ������
	CDockingDouble m_dAddRateUpper;		// ���� ������
	CDockingDouble m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����
};

#endif // !defined(AFX_WALLDATA_H__82214966_1672_49E7_BC1C_812A6C90F4F5__INCLUDED_)
