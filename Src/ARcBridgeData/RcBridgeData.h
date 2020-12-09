// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RcBridgeData.h: interface for the CRcBridgeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_)
#define AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PILE_SU			100

#define MAX_INWALL_HUNCH	20
#define MAX_JIJUM_COUNT_RC		21	// ����� ����!!!  �ִ��������� ����� �ݵ�� ������ ���ǵ� ���ڵ� �����ؾ� �ȴ�!!!!
#define MAX_QTY_HDAN	30
#define MAX_REBAR_DAN	3
#define MAX_STRUP_REBAR		MAX_JIJUM_COUNT_RC-2
#define MAX_CNS		2
#define RCBRIDGE_ANCHOR_MAX 30

const double dRateWidthFootingOut		= 0.65;		// �ֻ�ܿ��� ���ϴܱ��� ���̿� ���� ������ �ʺ� ����(����)
const double dRateWidthOutWall			= 0.1;		// �ֻ�ܿ��� ���ϴܱ����� ���� �������� �β�
const double dHeightFootingOut			= 1000;		// ������ü�� �Ϲ����� ���� ����
const double dHeightHunch				= 300;		// ��ġ�� ����
const double dWidthHunch				= 900;		// ��ġ�� �ʺ�(�����걳�϶��� ���� * 5), ���� 507-5 �����걳 ��ġ�� 1/3~1/6�踦 �������� ��
const double dWidthFootingOutWallInner	= 1200;		// ������ ������ ���� �ʺ�
const double dDepthFromEL				= 1500;		// ���ݿ��� ���ʻ�ܱ����� ����
const double dDepthToBracket			= 800;		// �������� ����� ��ܱ����� ����
const double dSizeBracket				= 300;		// ������� �ʺ�/����(��ġ����)
const double dMaxHeight					= 15000;	// �ǰ�Ƚ� ������ �ִ� ����
const double dMinHeight					= 6000;		// �ǰ�Ƚ� ������ �ּ� ����
const double dSta_Check_NotUse_Minus	= -1000000;

#define BRIDGE_TYPE_PORTAL			0	// ���� ��౳
#define BRIDGE_TYPE_PI				1	// ���� ��౳
#define BRIDGE_TYPE_V				2	// V�� ��౳
#define BRIDGE_TYPE_TRAP			3	// ��米����౳
#define BRIDGE_TYPE_SLAB			4	// �����걳
#define BRIDGE_TYPE_ARCH			5	// ��ġ��
#define BRIDGE_TYPE_BOX				6	// �ڽ���
#define BRIDGE_TYPE_TRAP2			7	// ��米����౳2
#define BRIDGE_TYPE_SLAB_STANDARD	8 // ������ ǥ�ص� ����

#define LOWERSLAB_TYPE_LEAN			0	// �Ϲ� ���� ��ũ��Ʈ(��������)
#define LOWERSLAB_TYPE_MASS			1	// MASS
#define LOWERSLAB_TYPE_PILE			2	// ����
#define LOWERSLAB_TYPE_COBBSTONE	3	// �⼮����
#define LOWERSLAB_TYPE_ANCHOR		4	// Anchor

#define COMBO_SERVICE			0
#define COMBO_ULTIMATE			1
#define COMBO_ECCENTRIC			2
#define COMBO_EARTHQUAKE		3
#define COMBO_EARTHQUAKE2		4
#define RCBRIDGE_COMBO_COUNT	5
#define MAX_RCBRIDGE_COMBO    200

#define FOOTING_CHECK_AXIAL		0
#define FOOTING_CHECK_MOMENT_MINUS	1
#define FOOTING_CHECK_MOMENT_PLUS	2
#define FOOTING_CHECK_EQ		3
#define FOOTING_CHECK_LSD		4			// �Ѱ���� �߰� ������ : ��Ī�� �𸣰���...

#define OUTVALUE_AXIAL		0	// ���
#define OUTVALUE_MOMENT		1	// M2
#define OUTVALUE_SHEAR		2	// V3
#define OUTVALUE_DISP		3	// ����(X)
#define OUTVALUE_COUNT		4

#define MIN_JOINT_DIST	5.0

// 2D FRAME : JOINT
#define LONG_0_JOINT_TYPE			0
#define LONG_1_JOINT_JIJUMNO		1
#define LONG_2_JOINT_SECTION_NO		2
#define LONG_3_JOINT_HUNCHPOS		3	// -1 : �Ϲ�����, 0 : ��ġ������, 1 : ��ġ����
#define LONG_4_INDEX_SPRING		4	// Spring, NLLink�� ��ġ�� Index

#define BOOL_0_JOINT_SPRING		0 // ����������, �Ϻν�����(BOX��)
#define BOOL_1_JOINT_JIJUM_CONNECT	1
#define BOOL_2_JOINT_JIJUM_FIX		2
#define BOOL_3_JOINT_COLUMN_END		3
#define BOOL_4_JOINT_SHOE_POS		4

// 2D FRAME : ELEMENT
#define LONG_0_ELEMENT_TYPE		0
#define LONG_1_ELEMENT_JIJUMNO		1
#define LONG_2_ELEMENT_COLUMN_INDEX	2 

#define BOOL_0_ELEMENT_CHECK_STT	0
#define BOOL_1_ELEMENT_CHECK_END	1

// 3D FRAME
#define LONGSHELL_0_HDAN_NODE	 0
#define LONGSHELL_1_JIJUMNO	 1
#define LONGSHELL_2_SHOE_POS	 2

// 3D ������
#define LONGSHELL_0_AREA_POS	 0

#define WINGSHELL_AREA_A	 0
#define WINGSHELL_AREA_B	 1
#define WINGSHELL_AREA_C	 2
#define WINGSHELL_AREA_D	 3
#define WINGSHELL_AREA_A_	 4

#define BOOLSHELL_0_CHECKPOS_STT 0	// �������� ����,��ü�� ���
#define BOOLSHELL_1_CHECKPOS_END 1	// �������� ����,��ü�� �ϴ�
#define BOOLSHELL_2_CHECKPOS_MID 2	// �������� �߾�,��ü�� �߾� (��ü�� ��쿡 ���... ������� ��� ����)

#define SHELL_ELEMENT_UPPERSLAB		0
#define SHELL_ELEMENT_LOWERSLAB		1
#define SHELL_ELEMENT_WALL			2
#define SHELL_ELEMENT_ARCHRIB		3

// �������õ� ����
#define TYPE_POST_BRIDGE	0	// ���ٸ� Ÿ�� (������)
#define TYPE_POST_NORMAL	1	// ���ٸ� Ÿ�� (�Ϲݿ�)

// ������౳ : ��������,MASS����,���ұ���,�⼮����,�η¹��� Anchor
#define BOXFOOTING_TYPE_LEAN			0
#define BOXFOOTING_TYPE_MASS			1
#define BOXFOOTING_TYPE_PILE			2
#define BOXFOOTING_TYPE_STONE			3
#define BOXFOOTING_TYPE_ANCHOR			4

// STM ���� ���� ������ ����
#define STM_MODEL_FOOT_COUNT			4
#define STM_MODEL_FOOT_LSD_COUNT		5

enum eStmModelPosLsd { eLsdStmCornerUpper, eLsdStmCornerLower, eLsdStmBracketStt, eLsdStmBracketEnd, eLsdStmFootStt, eLsdStmFootEnd, eLsdStmFootMid, eLsdStmCount };
enum eStmModelPos{ eStmBracket, eStmCorner, eStmFoot, eStmMidFoot };
enum eStmModelExportPos{ eStmBracketExport, eStmCornerExport, eStmFootSttExport, eStmFootEndExport, eStmMidFootExport };
// �Ϲ��ؼ� �𵨰� ���� �ؾ� �ǹǷ� Key�� 10���� ��������.
#define STM_MODEL_BRACKET_STT			10
#define STM_MODEL_BRACKET_END			11
#define STM_MODEL_CORNER_STT_UPPER_MINUS	12
#define STM_MODEL_CORNER_END_UPPER_MINUS	13
#define STM_MODEL_CORNER_STT_LOWER_MINUS	14
#define STM_MODEL_CORNER_END_LOWER_MINUS	15
#define STM_MODEL_CORNER_STT_UPPER_PLUS	16
#define STM_MODEL_CORNER_END_UPPER_PLUS	17
#define STM_MODEL_CORNER_STT_LOWER_PLUS	18
#define STM_MODEL_CORNER_END_LOWER_PLUS	19
#define STM_MODEL_FOOT_STT_MAXP			20
#define STM_MODEL_FOOT_STT_MAXM			21
#define STM_MODEL_FOOT_STT_MINM			22
#define STM_MODEL_FOOT_STT_EARTH		23
#define STM_MODEL_FOOT_END_MAXP			24
#define STM_MODEL_FOOT_END_MAXM			25
#define STM_MODEL_FOOT_END_MINM			26
#define STM_MODEL_FOOT_END_EARTH		27
#define STM_MODEL_MIDFOOT_HEAD			28
// LSD - ���� ������ : ��� �ִ�, �θ��Ʈ �ִ�, �����Ʈ �ִ�, ����� �ִ�, ����� �ּ�
#define STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1	50		// �찢�� ExtremeI - 1
#define STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1	51
#define STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1	52
#define STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1	53
#define STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1		54
#define STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1		55
#define STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1		56
#define STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1		57
#define STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2	58		// �찢�� ExtremeI - 2
#define STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2	59
#define STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2	60
#define STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2	61
#define STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2		62
#define STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2		63
#define STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2		64
#define STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2		65
#define STM_MODEL_FOOT_STT_MAX_AXIAL				66		// ����
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT		67		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT		68		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE		69		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE		70		
#define STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1		71		// ExtremeI - 1
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_1	72		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_1	73		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_1	74		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1	75		
#define STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2		76		// ExtremeI - 2
#define STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_2	77		
#define STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_2	78		
#define STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_2	79		
#define STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2	80		
#define STM_MODEL_FOOT_END_MAX_AXIAL				81		// ����
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT		82		
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT		83		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE		84		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE		85		
#define STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1		86
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_1	87
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_1	88		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_1	89		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1	90		
#define STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2		91
#define STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_2	92		
#define STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_2	93		
#define STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_2	94		
#define STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2	95
#define STM_MODEL_FOOT_MID_MAX_AXIAL				96		// �߰�����
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT		97		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT		98		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE		99		
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE		100		
#define STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_1		101		
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_1	102		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_1	103		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_1	104		
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_1	105		
#define STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_2		106
#define STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_2	107		
#define STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_2	108		
#define STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_2	109
#define STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2	110

enum EPartOfBridge { ePartUpperSlab, ePartLowerSlab, ePartOutWallStt, ePartFootStt, ePartWingWallStt, ePartApsStt, ePartOutWallEnd, ePartFootEnd, ePartWingWallEnd, ePartApsEnd, ePartInWall, ePartInFoot, ePartCount };
enum ETypeEnvComboLsd { eEnvLimit, eEnvExtremeI, eEnvExtremeII, eEnvService, eEnvService_Crack, eCountEnvTypeLsd};
enum EFootingCheckLsd { eFootCheckAxial, eFootCheckMomentMinus, eFootCheckMomentPlus, eFootCheckHoriMax, eFootCheckHoriMin, eCountFootCheck};
enum EWallCheckLsd { eWallCheckAxialMax, eWallCheckMomentMax, eWallCheckEccMax, eWallCheckHoriMax, eWallCheckAxialMin, eCountWallCheck};
enum EGoalOfEarthQuake { eGoalEQ_None = -1, eGoalEQ_Collapse, eGoalEQ_Function, eGoalEQ_Immediately, eGoalEQ_LongTerm, eGoalEarthQuakeCount };

#define LSD_DESIGN_WINGWALL		0
#define LSD_DESIGN_APPROACH		1
#define LSD_DESIGN_WINGWALL_SERVICE_JUDGE	0
#define LSD_DESIGN_WINGWALL_SERVICE_CHECK	1
#define LSD_DESIGN_APPROACH_SERVICE_JUDGE	2
#define LSD_DESIGN_APPROACH_SERVICE_CHECK	3

#define SCOFFOLD_WING_CON		0 //������ ��������(��ü�� ������)
#define SCOFFOLD_WING_NOTCON	1 //������ ���������(��ü�� ������)

class CFactor;
class CARcBridgeDataStd;
class CParabola;
class CBoringData;
class CJijum;
class CWallData;
class CWallApp;
class CExFootingData;
class CExFootingApp;
class CFootingData;
class CFootingApp;
class CGuardWallRC;
class CFootway;
class CApsData;
class CApsApp;
class CWingWall;
class CSapData;
class CFEMManage;
class CConcBasic;
class CTypeDefault;
class CSaveCrossSection;
class CSaveXySlabUpperByLenFrSttStation;
class CSaveXyMatchLineAndSlabUpper;
class CSaveDistRigidEndSlab;
class CSaveDistRigidEndWall;
class CDesignExpansionJoint;
class CExpansionJointData;
class CLsdManagerBridge;
class CLsdRebarManager;
class CLsdEnviromentRating;
class CCheckCantileverData;

#include "TypeDefault.h"


#define iNORMAL	0
#define iEXP 1
#define COUNT_CONC_LSD		eCountEnvTypeLsd-1

class AFX_EXT_CLASS CRcBridgeData : public CObject  
{
public:
	double	GetCoverFooting(BOOL bStt, BOOL bUpper, long nDan=0);
	CString GetStringBridgeSpan();
//	void	SetHDanRoad(long nQtyHDan, long nIndex);
	long	GetCountCycleMainRebar();
	CFootingApp* GetFootingByJijumNumber(long nJijum);
	/// ���������� ���� ////////////////////////////////////////////////////
	CConcBasic *GetConcBasicByTitle(CSafetyTypedPtrArray <CObArray, CConcBasic*> *pArrConc, CString sTitle);
	long m_nSelectModelingCountEQ;
	BOOL m_bGoalEarthQuake[eGoalEarthQuakeCount];

	CFEMManage m_FemNormal;	// ���� (2D Frame)
	CFEMManage m_FemEQ[4];	// �����ؼ� (2D Frame),0-��������,1-���ϸ��(���������,���������) (ARCBRIDGE-3408) ����������� KDS 17 �߰��ϸ鼭 4���� �ø�.
	CFEMManage m_FemShell;	// 
	CFEMManage m_FemWingWall[2][2];	// ������ Plate �ؼ� ��.�� + ��.��

	CFEMManage* GetFemWingWall(long nType);
	CFEMManage* GetFemWingWall(BOOL bStt, BOOL bLeft);
	CFEMManage* GetFemManage(long nTypeDesign);

	// SHELL �ؼ� ��� ����� �Է�....
	BOOL m_bUseShellUserOutput;
	CMatrix m_MatShellSlab;
	CMatrix m_MatShellWallStt;
	CMatrix m_MatShellWallEnd;
	CMatrix m_MatShellWallMid[MAX_JIJUM_COUNT_RC];
	CMatrix m_MatShellPRF;
	//////////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcSlab[COUNT_CONC_LSD];			// ������ܸ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcGirder;			// �Ŵ��ܸ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWall[COUNT_CONC_LSD];			// ��ü,���
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcInWall[COUNT_CONC_LSD];			// ������ü
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFoot[COUNT_CONC_LSD];			// ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFootRev[COUNT_CONC_LSD];			// ���� ���� ��ġ �ݴ�
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcFootCheckAdd[COUNT_CONC_LSD];	// ���� �ձ����� ��ġ �߰�
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcPRF[COUNT_CONC_LSD];	// 

	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingSttLeft;		// ������ ���� ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingSttRight;	// ������ ���� ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingEndLeft;		// ������ ���� ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcWingEndRight;	// ������ ���� ����
	CSafetyTypedPtrArray <CObArray, CConcBasic*> m_pArrConcArchRib;			// ��ġ���� ���������

	//////////////////////////////////////////////////////////////////////////

	double	m_dFckMassConc;
	double	m_dFckLeanConc;

	// ö�������� ����
	double	m_ZzdRail_HeightLane;
	double	m_dRail_HeightTie;
	double	m_ZzdRail_HeightDosangUnderTie;
	double	m_dRail_LengthTie;
	double	m_dRail_WidthTie;
	double	m_dRail_DistBetweenWheel;
	double	m_dRail_DistBetweenRail;
	CLineInfoApp* m_pLineApp;	// ��౳�� ���� ����
	BOOL m_bQuickSearchLine;	// ���� ���� �˻� ����

	BOOL	IsCns(BOOL bStt, BOOL bLeft, long nIdx);
	BOOL	IsAps(BOOL bStt, BOOL bLeft);
	BOOL	IsAnyAps();
	long	GetIdxInWall(CWallApp *pWall);
	long	GetCountFooting();
	BOOL	IsFooting(BOOL bStt);
	BOOL	IsFootingJijum(long nJijum);
	BOOL IsBoxType();
	BOOL IsArchType();
	CTwinVectorArray m_tvArrVertSection;	// ���ܸ� �ܸ� ��ǥ�� �׻� �ֽ����� �����ϰ� �ִ� �༮.
	CTwinVectorArray m_tvArrBriSect;		// ������ ���鿡�� ���̴� ���ܸ� ��ǥ
	CTwinVectorArray m_tvArrVertSectionRib[2];	// ���꼱 ����.
	CTwinVectorArray m_tvArrVertSectionRibCen;	// �����༱
	BOOL	IsOutWall(BOOL bStt);
	void	InitWall(BOOL bStt);
	void	InitHunch(long nIdx);

	// ���κ�ü
	CWallApp* GetInWall(long nIdx);
	long	GetCountInWall(BOOL bNotColumn=FALSE);
	long	GetCountInWallReal(BOOL bCheckTypeWall=FALSE);	// ���� ������ü�� ������ ���Ѵ�.
//	long	GetCountInWallEach(BOOL bColumn); // ������ü������ ��ü�İ� ��ս��� ���� ����.
	BOOL	RemoveInWall(long nIdx);
	BOOL	MakeInWall();
	
	// �� ���� ������
	void	MakeJijum(long nQtyMake);
	CJijum* GetJijum(int nJijum);
	int		GetCountJijum();

	// ������
	CWingWall* GetWingWall(BOOL bStt, BOOL bLeft);
//	int GetCountWingWall();
	CTwinVectorArray m_tvArrPlaneWingWall;
	CTwinVectorArray m_tvArrFrontWingWall;

	// ����
	CFootingApp* GetFooting(int nFooting);
	BOOL m_bAutoGroundInfo[MAX_JIJUM_COUNT_RC];			// �������� - �ڵ�����
	BOOL m_bAutoCalcPileProperty[MAX_JIJUM_COUNT_RC];	// ����Ư��ġ �ڵ����
	long m_nSelect_Eo_VertType[MAX_JIJUM_COUNT_RC];		// ���ݺ������(Eo) �Է� ���
	long m_nSelect_Eo_HoriType[MAX_JIJUM_COUNT_RC];		// ���ݺ������(Eo) �Է� ���

	BOOL IsPiRahmen();
	BOOL IsRcSlab();

	CRcBridgeData();
	virtual ~CRcBridgeData();

	// common �Լ�
	CARcBridgeDataStd* m_pARcBridgeDataStd;
	void Serialize(CArchive &ar);
//	BOOL CheckDataValidity();
	CRcBridgeData& operator=(const CRcBridgeData& Obj);

	// ���� �Ʒ��� ����Ǵ� ������
	// �ø��� �÷���
	long	m_nFlag;

	// ���� ���� �� ����
	CString m_strBridgeName;		// ������
	long    m_nTypeBridge;			// ����, ����, ������, V��, �ڽ���, ��� ����, ������ ��..
	double  m_dStationBridgeStt;	// ���� �����̼�
	double  m_dLengthBridge;		// �ѱ���
	long    m_nQtyJigan;			// ������
	long	m_nSeparBri;			// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)

	CSafetyTypedPtrArray <CObArray, CJijum*> m_pArrJijum;
	BOOL	m_bUsePadShoe;			// ������ħ�� �ִ� ��� ź����ħ�� ����� ������?
	
	long m_nIdxBridge;

	// Ⱦ�� ��ġ �Է�
	long	m_nLineBase;			// ���� ����
	long	m_nLineLeft;			// ���� ����
	long	m_nLineRight;			// ���� ����
	double	m_dWidthSlabLeft;		// ������ ������ �Ÿ�
	double	m_dWidthSlabRight;		// ������ ������ �Ÿ�
	double	m_dDisExtLineSttLeft;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineSttRight;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineEndLeft;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineEndRight;	//Ȯ���� ��쿡�� ���

	double m_dDistRoadCenterLevelLeft;		// �����߽� �����Ÿ� : ����
	double m_dDistRoadCenterLevelRight;		// �����߽� �����Ÿ� : ����


	// ���� ���� �Է�
	BOOL	m_bExpJoint[MAX_JIJUM_COUNT_RC];	// ���� ���� ��ġ ����
	long	m_nExpJointType[2];	// ���� ���� ���� (����, ����)
	double	m_dLengthJigan[MAX_JIJUM_COUNT_RC-1];			// ���� ����
	// �Է� ��ȣ�� ������ ���̸� ����(�ø�������� ����). ġ������ ���.
	CStringArray m_arrStrJijumLen;
	CDoubleArray m_arrJijumLen;
	BOOL	m_bEWDlgUsed;
	// �������� CJijum Ŭ�������� ����.
	long	m_nTypeShoeAngle;
	// ��������
	CDesignExpansionJoint	*m_pDesignExpanJoint;

	// Ȯ�� ����
	BOOL	m_bIsExp;			// ����(����) ��������?
	double	m_dExpSttW[2];	// ������, [0]:��, [1]:��
	double	m_dExpSttH[2];
	double	m_dExpSttR[2];
	double	m_dExpEndW[2];	// ������, [0]:��, [1]:��
	double	m_dExpEndH[2];
	double	m_dExpEndR[2];
	BOOL	m_bExpCutStt[2];	// [0] : ��
	BOOL	m_bExpCutEnd[2];	// [1] : ��
	BOOL	m_bExpSttRoundIsTan[2];	// ���� ������ R Ÿ���� �����������?(TRUE : ����, FALSE : �׳� ������ ���� ��ũ����)
	BOOL	m_bExpEndRoundIsTan[2];


	// NOTCH �� �ð����� �԰�
	long	m_nNotchType;
	double	m_dNotchHeight;
	double	m_dNotchWidth1;
	double	m_dNotchWidth2;
	double	m_dConsJointHeight;
	double	m_dConsJointWidth1;
	double	m_dConsJointWidth2;
	
	// ���� ��ħ ��ġ �Է�
	// CJijum Ŭ������ CShoePos Ŭ�������� ����.

	CSafetyTypedPtrArray <CObArray, CGuardWallRC*> m_pArrGuardWall;		// ��ȣ�� �Է�
	CSafetyTypedPtrArray <CObArray, CGuardWallRC*> m_pArrGuardWallRoad;	// ��ȣ�� �Է�(���߶���� ���)
	CSafetyTypedPtrArray <CObArray, CFootway*> m_pArrFootway;			// ���� �Է�
	BOOL	m_bRoundGW;
	BOOL	m_bRoundFW;
	BOOL	m_bGuardWallDlgUsed;		//������ ��� ������ �Է�
	BOOL	m_bGuardWallUnderDlgUsed;	//���� ��� - ���� �μ� ������ �Է�(���� ���)
	BOOL	m_bSSDlgUsed;

	// ��ġ �β��� ���� ������ m_dHHS�� ��.
	double	m_dDH[2];			// ������ ���ܿ��� ��ġ������ �Ÿ�
	BOOL	m_bCantilSlab[2];	// ��ƿ���� 
	int		m_nSlabChemfer[2];	// ������ ���� ����� ����(0:����, 1:����, 2:�)
	double	m_dCantilH1[2];	// ��ƿ���� �����϶� ����1
	double	m_dCantilH2[2];	// ��ƿ���� �����϶� ����2
	double	m_dCantilB[2];	// ��ƿ���� �����϶� ���ܿ����� �Ÿ�
	double	m_dRE1[2];	// ������ �Ϻ� ���� ���� ó���� ������, [0]:��, [1]:��
	double	m_dRE2[2];	// ������ ��� ���� ���� ó���� ������, [0]:��, [1]:��
	double	m_dTCW1[2];	// ������ �Ϻ� ���� ����� ó���� ��ġ ��
	double	m_dTCH1[2];	// ������ �Ϻ� ���� ����� ó���� ��ġ ����
	double	m_dTCW2[2];	// ������ ��� ���� ����� ó���� ��ġ ��
	double	m_dTCH2[2];	// ������ ��� ���� ����� ó���� ��ġ ����

	// �������� -------------------------------------------------------
	double	m_dUnitWeigthBackFill;			// ��ä���� �����߷�
	double	m_dUnitWeigthBackFillWater;	// ��ä���� ���ߴ����߷�
	double	m_dAngleFrictionBackFill;		// ��ä���� ���� ������
	double	m_dLiveLoadAlways;			// ���� Ȱ����
	double	m_dDeadLoadAlways;			// ���� ��������
	long	m_nSelectLiveLoadHori;				// ����Ȱ���߿� ���� �������� ����
	double	m_dLiveLoadFootway;			// ����Ȱ����
	double	m_dUnitWeightHeapSoil;		// ��������� �����߷�
	double	m_dUnitWeightValidDepth;	// ���������� �����߷�
	double	m_dValidDepthBaseGround;	// ������ ��ȿ���� ����
	double  m_dEo_CalcWallSpringEQ;		// ������ ��ü�� SPRING����� ���� Eo
	double	m_dNValue_Eo_Hori_XXX;			// ���� ���ݺ������
	double	m_dNValue_Eo_Vert_XXX;			// ���� ���ݺ������
	long	m_nTypeToapSafeCheck;		// ����������� ��а���
	double	m_SafeFactorOverturning[2];	// ������ ���� ������(����, ������)
	double	m_SafeFactorSliding[2];		// Ȱ���� ���� ������(����, ������)
	long	m_nSelectFootForce;			// ������ �����ۿ�� �������� ��ļ���
	long	m_nSelect2DFrameTool;		// 2D Frame �ؼ� ��
	long	m_nSelect3DShellTool;		// 3D Shell �ؼ� ��
	long	m_nSelectMidasVer;			// MIDAS input ��������
	long	m_nSelect3DOutputValue;	// 3D �ؼ������ ���� ���
	long	m_nApplyWingWallCalcValueCount;		// 3D �ؼ������ ���� ���� ��.
	long m_nSelectWingCalc;		// ������ �ؼ����
	long	m_bApplyUserDispMax;		// �������� �ִ뺯�� ����� �Է�
	double	m_dUnitLoadDispMaxUser;
	long	m_nSelectWingLoad_LS_ES;
	long	m_nLiveLoadDistToBackWall;	// ������ ����Ȱ���� ������ ��鿡�� ������ �Ÿ�
	BOOL	m_bCorrectionFactor_LiveLoadAlways;	// ����Ȱ���� ������ �������
	double	m_dCorrectionFactor_LiveLoadAlways;
	long	m_nApplyWingWallHeightBasePos;		// ������ؼ��� ������ġ 

	// �Ѱ����
	double m_dEo_Input_Pile_XXX;			// ���� ���ݺ������ (Eo) : 0 = Nġ�� ���
	// ------------------------------------------------------ ��������

	//////////////////////////////////////////////////////////////////////////
	// ��������
	CMatrix	m_MatComboUlt;	// ������� ����
	CMatrix m_MatComboEcc;	// ������� ���� (�ִ����)
	CMatrix m_MatComboEQ;	// �������� ����
	CMatrix m_MatComboUse;	// ������� ����

	CLongArray m_lArrComboUlt; // �ؼ�����Ÿ�� ����� ���������� �Է»�(���� �������� Combo)�� Index
	CLongArray m_lArrComboEcc;
	CLongArray m_lArrComboUse;

	DWORD   m_dWLoadNormal;	// ���߼��� (����)
	DWORD   m_dWLoadEQ;		// ���߼��� (������)
	BOOL	m_bApplyLoad[BRIDGE_LOAD_BASIC];
	BOOL	m_bApplyLiveLoadEQ;	// ������ Ȱ��������
	BOOL	m_bApplyAnchorForce;	// ��Ŀ�� ����
	BOOL	m_bApplyCombo3D[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];	
	BOOL	m_bApplyGroundWater[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];
	double	m_dFactorComboVLoad;
	double m_dFactorAnchor[RCBRIDGE_COMBO_COUNT][MAX_RCBRIDGE_COMBO];

	// ���輱�û��� -------------------------------------------------------
	BOOL	m_bCheckMainSlabAndFootTemper;	// ������ �� ������ ����µ�ö�ٺ� ����
	BOOL	m_bSelectSupportBarWall;		// ����ö�ٷ������ ����� ��ġ
	
	BOOL	m_bCheckSuppoerBarSlabMain;		// ����ö�ٺ� ������ ��ö�ٺ� [��.�� 4.7.5.7(2)]
	BOOL	m_bCheckSuppoerBarSlabMainOver20;	// ����ö�ٺ� ������ ��ö�ٺ� 20% �̻�
	BOOL	m_bCheckSuppoerBarSlab300;		// ����ö�ٺ� ������ 300mm���̻�
	BOOL	m_bCheckSuppoerBarSlabArea;		// ����ö�ٺ� ������ �ܸ�����

	BOOL	m_bCheckSuppoerBarWall15hs;		// ����ö�ٺ� ��ü 0.0015hs
	BOOL	m_bCheckSuppoerBarWallMainOver20;	// ����ö�ٺ� ������ ��ö�ٺ� 20% �̻�
	BOOL	m_bCheckSuppoerBarWall300;		// ����ö�ٺ� ��ü 300mm���̻�
	BOOL	m_bCheckSuppoerBarWallArea;		// ����ö�ٺ� ��ü �ܸ�����

	BOOL	m_bCheckSuppoerBarFoot15hs;		// ����ö�ٺ� ���� 0.0015hs
	BOOL	m_bCheckSuppoerBarFootMainOver20;	// ����ö�ٺ� ������ ��ö�ٺ� 20% �̻�
	BOOL	m_bCheckSuppoerBarFoot300;		// ����ö�ٺ� ���� 300mm���̻�
	BOOL	m_bCheckSuppoerBarFootArea;		// ����ö�ٺ� ���� �ܸ�����

	BOOL	m_bCheckSuppoerBarWing15hs;		// ����ö�ٺ� ������ 0.0015hs
	BOOL	m_bCheckSuppoerBarWing300;		// ����ö�ٺ� ������ 300mm���̻�
	BOOL	m_bCheckSuppoerBarWingArea;		// ����ö�ٺ� ������ �ܸ�����
	double m_dCheckSuppoerBarWallArea;		// ����ö�ٺ� ��ü �ܸ�����
	double m_dCheckSuppoerBarWallAreaForOver400;		// ����ö�ٺ� ��ü �ܸ����� Fy�� 400�̻��̰� ������ 16�����ΰ��
	double m_dCheckSuppoerBarFootArea;		// ����ö�ٺ� ���� �ܸ�����
	double m_dCheckSuppoerBarSlabArea;		// ����ö�ٺ� ������ �ܸ�����
	double m_dCheckSuppoerBarWingArea;		// ����ö�ٺ� ������ �ܸ�����

	BOOL	m_bCheckSupportWallDryingShrinkage;	//�������� �� �µ�ö��(��ü)
	BOOL	m_bCheckSupportWallMainRate; //���� ö�ٷ��� 25%
	BOOL	m_bCheckSupportFootDryingShrinkage;	//�������� �� �µ�ö��(����)
	BOOL	m_bCheckSupportSlabDryingShrinkage;	//�������� �� �µ�ö��(������)
	BOOL	m_bCheckSupportSlabByReqRebar; //�ʿ�ö�ٷ�/���ö�ٷ�
	BOOL	m_bCheckSupportSlabMainRate; //��ö���� 20%
	BOOL	m_bCheckSupportSlabMainRateSpan; //�������⿡ ���� ��ö�ٺ�
	BOOL	m_bSkipAsMinCheckByHoriRebar;		// �������� �� �µ�ö������ �ּ�����ö�� ���� (���� 5.12.2.1(1)(��)) 

	long m_nSelectPileCalcSpringEQ;			// ���ұ��� ���ʽ������������� �������� 0 : FIX, 1:HINGE
	long m_nSelectApsShearCheck;			// ���ӽ����� ���ܰ���� ���߻��� ��ġ		

	BOOL	m_bCheckSuppoerBarArchRibMinDia1;		// ��ġ���� ���ö�� �ּ�����
	BOOL	m_bCheckSuppoerBarArchRibMinDia2;		// ��ġ���� ���ö�� ��ö�ٴ������
	BOOL	m_bCheckFrontFootTrap2;				// ��米2, ��ġ����౳�� �ձ�����(������ü����)

	BOOL	m_bApplyMinAvSlab;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bApplyMinAvWall;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bApplyMinAvFoot;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bApplyMinAvWing;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bApplyMinAvAps;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bApplyMinAvHyunTa;				// ���ܰ���� �ּ����ܰ���
	BOOL	m_bCheckMinCtcShearWing;		// ������ ����� �ּ�����ö�� ���� ����
	BOOL	m_bCheckMinCtcShearAps;		// ���ӽ����� ����� �ּ�����ö�� ���� ����
	BOOL	m_bCheckCornerInside;			// �찢�� ����� �������� ����ö�ٷ� ����
	BOOL	m_bApplyFootingGForce;			// ������ ���� ������ ����
	BOOL	m_bApplyMassCalcSafe;			// ��������� MASS �ۿ�� ����
	BOOL	m_bApplyFootingMinThick;		// ���ʵβ� ���Ѱ� ���� (��.��.�� p85)
	BOOL m_bCheckRebarRateFooting;		//������ö���� �µ�ö�ٺ� �ּ�ö�ٺ�� ����(���ΰ���)
	long		m_nSelectApplyFs;
	long		m_nApplyCalcShearDistFront;		// �ձ� ���ܰ���� ����ܸ� ��ġ
	long		m_nApplyCalcShearDistBack;		// �ޱ� ���ܰ���� ����ܸ� ��ġ
	long		m_nTypeCrackDesignSlab;			// ������ �νĿ� ���� ȯ������(��ν�����)
	long		m_nTypeCrackDesignWall;			// ������ �νĿ� ���� ȯ������(��ü)
	long		m_nTypeCrackDesignFoot;			// ������ �νĿ� ���� ȯ������(����(�Ϻν�����))
	long		m_nTypeCrackDesignWing;		// ������ �νĿ� ���� ȯ������(������)
	long		m_nTypeCrackDesignAps;			// ������ �νĿ� ���� ȯ������(���ӽ�����)
	long		m_nSelectJointUpperRange;		// �������, ������ ���� ����
	long		m_nIdxCountCycleMainRebar;		// ��ö�� ��ġ �ɼ�(CYCLE)
	double	m_dFactorToap;						// ������ ���� ��п����� ���߰��
	double	m_dFactorLive;							// ������ ���� �������߿����� ���߰��
	long		m_nSelFactorKhStt;					// ������ü ��а�� ������ ������а�� 0:�ڵ�, 1:0.5, 2:1.0, 3:1.5
	long		m_nSelFactorKhEnd;					// ������ü ��а�� ������ ���y��а�� 0:�ڵ�, 1:0.5, 2:1.0, 3:1.5
	long		m_nSelCalcCornerFt;					// ������ ����ö�� ������ �������� ft ����
	long		m_nSelectForceCheckPos;			// �ܸ���� ���� ��ġ
	long		m_nSelectRigidModeling;			// �𵨸��ؼ��� ���� ����...
	BOOL	m_bSelectRigidCalcHunch;		// ���� ������ ��ġ �β� ���� (�߰���ü)
	BOOL	m_bSelectTempLoadAddWall;	// ��ü�� �µ����� ����
	BOOL	m_bSelectShrinkLoadAddWall;	// ��ü�� �������� ���� ����
	BOOL m_bSelectEnvToapOne;			// ����� Env ����
	BOOL m_bSelectEnvSubsid;			// ����ħ�� Env ����
	BOOL m_bSelectHunchModeling;
	BOOL m_bSelectHunchDesignCheck;
	long		m_nSelectTempLoadSlab;			// �����꿡 �µ����� ����(�µ����� +/-, ���ϸ� �µ���)
	long		m_nSelSubsideType;					// �ѱ��ʿ� ��ü�� �ΰ� ������ ����ħ�� ����
	long		m_nSelectDesignRoadWidth;		// ���������� ����
	long		m_nSelectLiveLoadSpanLength;
	long		m_nSelectDesignSpanLength;	// ���������� ����
	long		m_nSelectDesignSpanApply;		// ��� �𵨸��� ������� ���
	BOOL	m_bSelectSpanLengthApplyShoe;	// �������� ������ ��ħ��ġ ���
	long		m_nSelectWallSectionDesign;		// ��ü �ܸ� ����
	long		m_nSelectInWallSectionDesign;	// ������ü �ܸ� ����(0 : ������, 1 : ��ü(�⺻��))
	long		m_nSelectFootingReactionForce;	// ��ü �ܸ� ����
	long		m_nArchRibSectionMomentPos;	// ��ġ���� ����� �ܸ�2�� ���Ʈ ����
	long		m_nSelectModelingHeight;		// �𵨸� ������ ��ü ����
	long		m_nSelectAlphaKhKv_XXX;				// ���� ������ ���� ���� �ᰪ ����
	BOOL	m_bCalcShearCenter;				// �����߾Ӻ� ���� ����
	long		m_nSelectStartLane;					// �������� ���ϱ��� ��ġ
	BOOL	m_bCheckUseri;						// Ȱ���� ���� ��ݰ�� ������Է� ����
	double	m_dCheckUseriValue;				// Ȱ���� ���� ������Է� ��ݰ��
	long		m_nSelectApsAxleWeight;		// ���ӽ����� Ȱ���߻����� ���� ����
	long		m_nSelectApsDeadLoad;		// ���ӽ����� Ȱ���߻����� ���� ����
	BOOL	m_bApsDowelSeparateRebar;			// ������ ö�� �и� ���� 
	BOOL	m_bOutWallUsebilityCheck;			// ��,������ ��ü �ھ������ ��뼺����
	// ------------------------------------------------------ ���輱�û���

	// �������û��� -------------------------------------------------------
	long		m_nFormType_ConcNormal;		// ��Ǫ�� : �Ϲ���ũ��Ʈ
	long		m_nFormType_Footing;				// ��Ǫ�� : ����
	long		m_nFormType_WallFront;			// ��Ǫ�� : ��ü ����
	long		m_nFormType_WallBack;			// ��Ǫ�� : ��ü �ĸ�
	long		m_nFormType_WingFront[2][2];	// ��Ǫ�� : ������ ����
	long		m_nFormType_WingBack[2][2];	// ��Ǫ�� : ������ ���
	long		m_nFormType_Aps;					// ��Ǫ�� : ���ӽ�����
	long		m_nFormType_MassConc;			// ��Ǫ�� : mass ��ũ��Ʈ
	long		m_nFormType_LeanConc;			// ��Ǫ�� : ���� ��ũ��Ʈ
	BOOL	m_bSelectBM_ConcCare_Main;		// ��ũ��Ʈ��� : ���뺻ü, ������
	BOOL	m_bSelectBM_ConcCare_Aps;			// ��ũ��Ʈ��� : ���ӽ�����, ���潽����
	long	m_nSelectBM_ConcCare_Cal;		// ��ũ��Ʈ ��� ���� ��� ( 0 : ���ʸ� ������ ��Ǫ������+��μ����, 1 : ���ʸ� ������ �����)
	long		m_nTypeRcBridgeBlock;			// ���뺸ȣ�� ��ġ(0 : ����, 1 : ��õ��, 2 : ������)
	double	m_dAngleRcBridgeBlock;			// ���뺸ȣ�� ������ġ����(��)
	BOOL	m_bSelectBM_ApsCnsInclude;			// �������迡 ���ӽ�����, ���潽���� ����
	double	m_dPavementThick;					// ��ä��� ��ũ��Ʈ ����β�
	double	m_dNosangThick;					// ��ä��� ���β�
	long		m_nTypeHeightWall;				// ���� ����� ��ü ���� ����(0 : �����߽� ����, 1 : ��ճ���((�ִ�+�ּ�)/2), 2 : �ִ����, 3 : ������ ��ճ���((�ִ�+�ּ�)/2)
	long		m_nTypeSlabLength;				// ���� ����� ������ ���� ����(0 : ���Ÿ�, 1 : �����Ÿ�)
	long		m_nTypeSurfaceProof;				// ������ Ÿ��(0 : ħ����, 1 : ��Ʈ���) - �ǰ�� -> ���Ǿ��¶��,�����걳�� ħ����, ���߶�౳�� ��Ʈ���
	long		m_nTypePost;						// ��ü ���ٸ� Ÿ��(������, �Ϲݿ�)
	long		m_nTypePostBaseLine;				// ���ٸ� ���ؼ�(0:���� ��� 1:���ݼ� 2:����� �Է�) 
	double	m_dPostLineEL;						// ����� �Է� ���ٸ� ���ؼ� EL
	long		m_nTypeScaffoldBaseLine;			// ��� ���ؼ�(0:���� ��� 1:���ݼ� 2:����� �Է�) 
	long		m_nTypeScaffoldSideLocation;			// ������ ���� ���ؼ� (SCOFFOLD_WING_CON(0): ��������, SCOFFOLD_WING_NOTCON(1):���� ������)
	double	m_dScaffoldLineEL;					// ����� �Է� ��� ���ؼ� EL
	double	m_dThickPostUpper;				// ������ ���ٸ� ��� �β�(�ڵ����� ���� ����)
	double	m_dThickPostSide;					// ������ ���ٸ� ���� �β�(������ 8�� ���� ������ ������ �ڵ� ����)
	double	m_dThickScaffold;					// ��� �β�
	double	m_dTimeConcForm;					// ��ũ��Ʈ ���, ��Ǫ�� ��ü �ð�
	BOOL m_bApplyYongSu;							//������ı�

	long m_nDeductNearFormBM;				// ���������� ��Ǫ�� ���� ����(0 : ����, 1 : ��������, 2 : ��������)
	long m_nDeductNearTupagiBM;				// ���������� ���ı� ���� ����(0 : ����, 1 : ��������, 2 : ��������)

	double	m_dAggregate_Footing;			// ��ũ��Ʈ �����ִ�ġ��(����)
	double	m_dAggregate_Main;				// ��ũ��Ʈ �����ִ�ġ��(��ü)
	double	m_dAggregate_Aps;				// ��ũ��Ʈ �����ִ�ġ��(���ӽ�����)
	double	m_dAggregate_Lean;				// ��ũ��Ʈ �����ִ�ġ��(����)
	double	m_dAggregate_Mass;				// ��ũ��Ʈ �����ִ�ġ��(MASS)
	double  m_dAggregate_Hyunta;				// ��ũ��Ʈ �����ִ�ġ��(����Ÿ������)
	double	m_dAggregate_Etc;					// ��ũ��Ʈ �����ִ�ġ��(��ȣ����)

	double	m_dSlump_Footing;					// ��ũ��Ʈ Slump(����)
	double	m_dSlump_Main;					// ��ũ��Ʈ Slump(��ü)
	double	m_dSlump_Aps;						// ��ũ��Ʈ Slump(���ӽ�����)
	double	m_dSlump_Lean;						// ��ũ��Ʈ Slump(����)
	double	m_dSlump_Mass;					// ��ũ��Ʈ Slump(MASS)
	double  m_dSlump_Hyunta;					// ��ũ��Ʈ Slump(����Ÿ������)
	double	m_dSlump_Etc;						// ��ũ��Ʈ Slump(��ȣ����)

	BOOL	m_bMonumenu;							// ������ ��ħ ��ġ(OLD)
	long		m_nMonumenu;						// ������ ��ħ ��ġ
	long		m_nDesPanel;						// ������ ��ġ����
	long		m_nNamePanel;						// ������ ��ġ����
	double	m_dAddLenghForGuardWallBM;	// ��ȣ�� ���� ����� �߰� ����(OLD)
	double	m_dAddLenghForGuardWall[2][2];	// ��ȣ�� ���� ����� �߰� ���� ������[������][�¿���] 
	long		m_nHeightForSlabBM;				// ������ ��Ǫ�� ���� ����� ���� ����	
	double	m_dTackCotting;						// �� ���� ����
	long		m_nTypePileMilkBM;				// ���α��������� ��ũ�� ���� �ɼ�(0 : ����+0.2 ��ü ä��, 1 : ����+0.2 - ���� �κи� ä��)
	BOOL		m_bExcludeApsDowelRebar;		//������ ö�� ö�ٰ������� ȣ�� ����(FALSE: ���Ծ��� �⺻��)
	double m_dSurplusSoil_HyunTaPile;  //����Ÿ������ ����ó��
	double m_dConcreteSoil_HyunTaPile; //����Ÿ������ ��ũ��Ʈ Ÿ��(���)
	double m_dConcreteRock_HyunTaPile; //����Ÿ������ ��ũ��Ʈ Ÿ��(�Ϲ�)

	// ------------------------------------------------------ �������û���
	// �Ǻ����� -------------------------------------------------------
	double	m_dCoverUpper_UpperSlab[MAX_REBAR_DAN];	// ��ν����� ���
	double	m_dCoverLower_UpperSlab[MAX_REBAR_DAN];	// ��ν����� �ϸ�
	double	m_dCoverSide_UpperSlab[MAX_REBAR_DAN];	// ��ν����� ����
	double	m_dCoverUpper_LowerSlab[MAX_REBAR_DAN];	// �Ϻν����� ���
	double	m_dCoverLower_LowerSlab[MAX_REBAR_DAN];	// �Ϻν����� �ϸ�
	double	m_dCoverWallOut[MAX_REBAR_DAN];		// ��ü�ܸ�
	double	m_dCoverWallIn[MAX_REBAR_DAN];		// ��ü ����
	double	m_dCoverWallMid[MAX_REBAR_DAN];		// �߰���ü
	double	m_dCoverColumn[MAX_REBAR_DAN];		// ���
	double	m_dCoverWingOut[MAX_REBAR_DAN];		// ������ ����
	double	m_dCoverWingIn[MAX_REBAR_DAN];		// ������ ���
	double	m_dCoverWingUpper;					// ������ ���
	double	m_dCoverWingEnd;						// ������ ����
	double	m_dCoverUpper_Footing[MAX_REBAR_DAN];	// ���� ���
	double	m_dCoverLower_Footing_Lean[MAX_REBAR_DAN];	// ���� �ϸ�(�������ʽ�)
	double	m_dCoverLower_Footing_Pile[MAX_REBAR_DAN];	// ���� �ϸ�(���ұ��ʽ�)
	double  m_dCoverRib[2][MAX_REBAR_DAN];	// ����
	double	m_dCoverSide_Footing[MAX_REBAR_DAN];	// ���� ����
	// ------------------------------------------------------ �Ǻ�����

	// ��Ÿ���� -------------------------------------------------------
	BOOL	m_bPileNvalueAuto;
	// ------------------------------------------------------ ��Ÿ����

	// ���ܸ����� -------------------------------------------------------
	long	m_nTypeJongdanForRebar;		// ���ܸ� Ÿ�� : 0 : ����, 1 : ���(��ٽø� �����), 2 : �Ϲݵ��� ���ؼ� ���ܼ������� �׸�
	long	m_nTypeWallForRebarGen;		// ��ü ǥ�� ���(�Ϲݵ� ������), 0 : ������ ����, 1 : ������, 2 : ����sta(�����鿡 �ش��ϴ�)
	long	m_nTypeOutWall;				// ������ü ����(�¿�)
	long	m_bApplyAngleJongdan;

	// ������ ��ü
	CFlexibleJoint m_fxJointStt;
	double	m_dFHS;
	double	m_dFLS;
	double	m_dFRS;
	double	m_dWS;
	double  m_dWSHW;	// ������ü ��ġ�ʺ�(��米2������ ����)
	double  m_dWSHH;	// ������ü ��ġ�ʺ�(��米2������ ����)
	double	m_dBHWS;
	double	m_dBHHS;
	double	m_dBHS;
	double	m_dBTHS;
	double	m_dBWS;
	double	m_dHWS;
	double	m_dHRS;		// ������ġ �ݰ�
	double	m_dELSB;
	CFootingApp m_footingStt;
	BOOL	m_bDivisionApsStt;	// ���ӽ����� �и�(Ⱦ����) ��ġ(�� ������ �¿��� ���� �и��� - �⺻�� ����)
	long	m_nCountCnsStt[2];	// ���潽���� ��
	CApsApp m_apsStt[2];
	CApsApp m_cnsStt[2][MAX_CNS];
	//CExFootingApp m_ExFootingStt;

	// ������ ��ü
	CFlexibleJoint m_fxJointEnd;
	double	m_dFHE;
	double	m_dFLE;
	double	m_dFRE;
	double	m_dWE;
	double  m_dWEHW;	// ������ü ��ġ�ʺ�(��米2������ ����)
	double  m_dWEHH;	// ������ü ��ġ�ʺ�(��米2������ ����)
	double	m_dBHWE;
	double	m_dBHHE;
	double	m_dBHE;
	double	m_dBTHE;
	double	m_dBWE;
	double	m_dHWE;	// (���, �Ϻ�) BoxŸ�� �߰�
	double	m_dHHE;	// (���, �Ϻ�) BoxŸ�� �߰�
	double	m_dHRE;			// ������ġ �ݰ�
	double	m_dELEB;
	CFootingApp m_footingEnd;
	BOOL	m_bDivisionApsEnd;	// ���ӽ����� �и�(Ⱦ����) ��ġ(�� ������ �¿��� ���� �и��� - �⺻�� ����)
	long	m_nCountCnsEnd[2];	// ���潽���� ��
	CApsApp m_apsEnd[2];
	CApsApp m_cnsEnd[2][MAX_CNS];
	//CExFootingApp m_ExFootingEnd;

	// ������ü��ġ ����
	long	m_nCountInWallHunch;		// ������ü ��ġ ����
	double	m_dHHL[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ ����(��������)	���, �Ϻ�
	double	m_dHHR[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ ����(��������)
	double	m_dHBH[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ�Ʒ� ���� ����
	double	m_dHLW[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ �ʺ�(����)
	double	m_dHRW[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ �ʺ�(������)
	double	m_dWL[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ ������ġ(����)
	double	m_dWR[MAX_INWALL_HUNCH][2];	// �� ��ü ��ġ ������ġ(������)
	double	m_dHLR[MAX_INWALL_HUNCH][2];	// ������ġ ���� �ݰ�
	double	m_dHRR[MAX_INWALL_HUNCH][2];	// ������ġ ������ �ݰ�

	// ��ġ�� ���� Ÿ��
	long m_nTypeRoundOfHunchStt;			// ���� ��ġ 0 : ����(R), 1 : ������ũ(R)
	long m_nTypeRoundOfHunchEnd;			// ���� ��ġ 0 : ����(R), 1 : ������ũ(R)
	long m_nTypeRoundOfHunchInWall[MAX_INWALL_HUNCH][2];	// ������ġ �¿���

	

	// ������(��������) 2�� (���� ����)
	BOOL m_bConnectRibAndUpperSlab;	// ��ġ����� ��ν����긦 ��������?
	CParabola m_pbRib[2];
	/*
	CDPoint m_xyRibCrown[2];
	double m_dRibLenTotal[2];
	double m_dRibHeightTotal[2];
	double m_dRibLenStt[2];
	double m_dRibLenEnd[2];
	double m_dRibFactorM[2];
	double m_dRibLenArchLine[2];
	double m_dRibOffsetFromUpper[2];	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�
	*/

	// ������(��������) �߽ɼ�
	CParabola m_pbRibCen;
	/*
	CDPoint m_xyRibCrownCen;
	double m_dRibLenTotalCen;
	double m_dRibHeightTotalCen;
	double m_dRibLenSttCen;
	double m_dRibLenEndCen;
	double m_dRibFactorMCen;
	double m_dRibLenArchLineCen;
	double m_dRibOffsetFromUpperCen;	// ���� �������� ���ػ�ܿ����� ������ �Ÿ�
	*/

	double m_dRibBlockW[MAX_STRUP_REBAR];	// ������ �簢 �� ��
	double m_dRibBlockH[MAX_STRUP_REBAR];	// ������ �簢 �� ����
	// ------------------------------------------------------ ���ܸ�����

	// ������ ���� �Է�
	CWingWall* m_pWingWallStt[2];		// ������ ������ [0]:��, [1]:��
	CWingWall* m_pWingWallEnd[2];		// ������ ������ [0]:��, [1]:��
	BOOL	m_bAutoWingEl;					// ������ ���� EL �ڵ� ���
	BOOL	m_bAutoWingHeightSoil;

	BOOL	m_bBracketLengthStt;			// ���� �귡Ŷ ���� ���� ����
	BOOL	m_bBracketLengthEnd;			// ���� �귡Ŷ ���� ���� ����
	double	m_dBrkLenStt[2];				// ���� �귡Ŷ �̰� �Ÿ� [0]:��, [1]:��
	double	m_dBrkLenEnd[2];				// ���� �귡Ŷ �̰� �Ÿ� [0]:��, [1]:��

	// ������ ��ħ ����
	double	m_dMonuH1;
	double	m_dMonuH2;
	double	m_dMonuH3;
	double	m_dMonuD;
	double	m_dMonuW;
	double	m_dMonuL;
	double	m_dMonuFck;
	double	m_dMonuR_Fy;
	double	m_dMonuR_DiaHor;
	double	m_dMonuR_DiaVer;
	double	m_dMonuR_H;
	double	m_dMonuR_L;
	int		m_nMonuR_nA;
	double	m_dMonuR_A;
	double	m_dMonuR_A1;
	int		m_nMonuR_nB;
	double	m_dMonuR_B;
	int		m_nMonuR_nC;
	double	m_dMonuR_C;
	double	m_dMonuR_C1;
	double	m_dMonuR_C2;
	double	m_dMonuR_C3;
	int		m_nMonuR_nD;
	double	m_dMonuR_D;
	double	m_dMonuR_D1;
	double	m_dMonuR_D2;
	double	m_dMonuR_D3;
	double	m_dMonuAggregate;

	// ��Ÿ : ���� ���� -> �� ��ȣ�� ���� Fck, Fy ���� ������
	double	m_dOther_Fck;
	double	m_dOther_Fy;

	CTypedPtrArray <CObArray, CTypeDefault*> m_pArrTypeDefault;
	// ��Ÿ
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;			// ��ũ��Ʈ ����
	
	// ���� �� ������ ����ü
	typedef struct STRUCT_REF_CELL
	{

	}structRefCell;

	double	m_dLengthRail[MAX_QTY_HDAN]	;		// ���� ����(����), ö�������� ���
	long	m_nLineNumber[MAX_QTY_HDAN]	;		//�ش� ������ ���ؼ���, ö�������� ���
	//Ⱦ�� ������ ����
	int		m_nRoadMaterial				;		// ���� ���� ����(0:�ƽ���, 1:LMC)
	double	m_dTP						;		// ���� �β�
	double	m_dTS						;		// ������ �β�
	double	m_dTSOld;	// ��簡 ������� ���� org�β�(���Ŀ��� ����׸� �� m_dTS�� �����ϴ� ��찡 ����)
	double	m_dHHS						;		// ������ġ �β� (���, �Ϻ�) BoxŸ�� �߰�
	BOOL	m_bLevelFrRoadCenterToEdge	;		// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	BOOL	m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	;		// --�����߽ɰ��� ���� ����(���߶���� ������ ����) 20071030

	// ö���� ������ �����Է�
	long	m_nQtyRail					;		// [������ ���� �Է�], ���� ���Ҽ�, ö�������� ���
	double  m_dDisBaseElevation			;		// F.L ������ġ(ö���� ������� ���� ��ġ)
	double  m_dDisBaseElevationRL		;		// --R.L ������ġ(ö���� ������� ���� ��ġ)//Ⱦ�� ������ �����Է� �߰����� 20050610
	double  m_dHeightRail				;		// --ö���� ���ϳ���//Ⱦ�� ������ �����Է� �߰����� 20050610
	double  m_dHeightDosangUnderTie		;		// ö���� �������
	double  m_dSlopeDosang				;		// ö���� ���� ����
	double	m_dUnitWeightRail			;		// �˵� �����߷�

	// ���߶�౳ ������ �����Է�
	BOOL	m_bUnderGroundRahmen		;		// ���� ��౳
	BOOL	m_bCalcOpen;						// ���߶�౳�� ���������� ���
	BOOL	m_bPrevApplyCalcOpen		;		// ���߶�౳�� ���������� ��� �ɼ� ����������..
	BOOL	m_bUnderGroundPaveDetail	;		// ���� ���� ����
	BOOL	m_bApplyLineSlope			;		// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610

	double	m_dWidthRoadLeft			;		// ���� ��� �Ÿ�
	double	m_dWidthRoadRight			;		// ���� ��� �Ÿ�
	double	m_dUnderGroundCenterELStt	;		// ��������EL (���ܽ����� EL)
	double	m_dUnderGroundCenterELEnd	;		// ��������EL (���������� EL)
	long	m_nUnderGroundDesignType	;		// ���Ȱ���� ����
	long	m_nUnderGroundDesignSelect	;		// ���Ȱ���� �������
	long	m_nUnderGroundDistribute	;		// ö�� �������� ������ ����

	double	m_dUnderGroundSlopeSlabLeft;		// ������ ��	
	double	m_dUnderGroundSlopeSlabRight;		// ������ ��
	//
	double	m_dUnderGroundNoriSlopeFirst;
	double	m_dUnderGroundNoriSlopeNext;
	double	m_dUnderGroundNoriMaxHeight;
	double	m_dUnderGroundNoriWidth;
	double	m_dUnderGroundNoriSolpeSodan;
	//
	//
	long	m_nTypePavement;
	double  m_dUnderGroundHeightPavement;		// ����β�
	BOOL	m_bUnderGroundDesignHeight;			// ������� �ڵ����
	double  m_dUnderGroundDesignHeight;			// �������
	double  m_dUnderGroundDesignLiveAlways;		// ���Ȱ����(��������)
	double  m_dUnderGroundHeightMortar;			// ��� ��Ż �β�
	double  m_dUnderGroundWeightMortar_xx;		// ��� ��Ż �����߷�(kgf/m3), // �����ڷ��� m_UWeightCement�� ���
	double  m_dUnderGroundPaveHeightPyo;		// ǥ��
	double  m_dUnderGroundPaveHeightGee;		// ����
	double  m_dUnderGroundPaveHeightMiddle;		// �߰���
	double  m_dUnderGroundPaveHeightBozo;		// ��������
	double  m_dUnderGroundPaveHeightDong;		// ���������
	double  m_dUnderGroundPaveHeightEps;		// EPS
	double  m_dUnderGroundPaveHeightTosa_XXX;	// ���				// (ARCBRIDGE-3184) ��� ���̸� ����ؼ� �������� �ʰ� �Ź� ����ؼ� ������ ����
	double  m_dUnderGroundPaveWeightPyo;		// ǥ��
	double  m_dUnderGroundPaveWeightGee;		// ����
	double  m_dUnderGroundPaveWeightMiddle;		// �߰���
	double  m_dUnderGroundPaveWeightBozo;		// ��������
	double  m_dUnderGroundPaveWeightDong;		// ��������
	double  m_dUnderGroundPaveWeightEps;		// EPS
	double  m_dUnderGroundPaveWeightTosa;		// ���
	double  m_dUnderGroundPaveWidthPyo;			// ǥ��
	double  m_dUnderGroundPaveWidthGee;			// ����
	double  m_dUnderGroundPaveWidthMiddle;		// �߰���
	double  m_dUnderGroundPaveWidthBozo;		// ��������
	double  m_dUnderGroundPaveWidthDong;		// ��������
	double  m_dUnderGroundPaveWidthEps;			// EPS
	double  m_dUnderGroundPaveWidthTosa;		// ���

	// �ڵ� ���� ���� ���� 
	double  m_dOptimizeDiaMinimum;	// ��ö�� �ּ�����
	double  m_dOptimizeDiaMaximum;	// ��ö�� �ִ�����
	double  m_dOptimizeSafeRate;	// ����ȭ�� ������
	BOOL    m_bOptimizeRebar2Dan;	// 2��ö���� ������ ������
	BOOL    m_bOptimizeRebarSplit;	// 1,2Cycle�� ������ �ٸ��Ե� ������ ������.
	BOOL    m_bOptimizeRebarMatch;	// ��Ī��ġ�� ö���� ���� ��ġ�ǵ��� 

	// ö�� ������ ���� �ɼ�
	long	m_nTypeAddRate;	// 0 : ���溰, 1 : ��ö��,��Ÿö��
	double	m_dAddRateLower;	// ���� ������
	double	m_dAddRateUpper;	// ���� ������
	double	m_dAddRateDiaOrg;	// ���溰�ϰ�� ���� ����

	// BOX Ÿ�� �߰�
	CFootingApp m_footingBox;

	BOOL	m_bApplyGroundWaterLevel;		// ���ϼ��� ����
	double	m_dGroundWaterLevel;			// ���ϼ��� EL
	double	m_dCoffKh_Sei;					// �����ؼ��� �������������� �����;
	BOOL	m_bExistProtectionRiseFooting;	// [�λ��������] ��ġ����
	double	m_dPRF_W;						// [�λ��������] ������ ���� ����
	double	m_dPRF_H1;						// [�λ��������] ���� ����
	double	m_dPRF_H2;						// [�λ��������] ������ ����
	BOOL	m_bPRF_SameCTC_Main;			// [�λ�������� ö��] TRUE: ��ö��CTC, FALSE: ��ö��CTC/2
	double	m_dPRF_CTC_Main;				// [�λ�������� ö��] ǥ�ذ���
	double	m_dPRF_CTC_Support;				// [�λ�������� ö��] ����ö�� ����ǥ�ذ���
	double	m_dPRF_CTC_SupportVert;			// [�λ�������� ö��] ����ö�� ����ǥ�ذ���
	double	m_dPRF_CTC_Shear;				// [�λ�������� ö��] ����ö�� ǥ�ذ���
	double	m_dPRF_Dia_Main;				// [�λ�������� ö��] ����
	double	m_dPRF_Dia_Support;				// [�λ�������� ö��] ����ö�� ����
	double	m_dPRF_Dia_Shear;				// [�λ�������� ö��] ����ö�� ����
	long	m_nPRF_Type_Shear;				// [�λ�������� ö��] ����ö�� Ÿ��
	long	m_nPRF_Count_Shear;				// [�λ�������� ö��] ����ö�� ��ö�� ���δ� ����

	// �Ϻν����� �Է�
	double	m_dTS_Lower;					// �Ϻν����� �β�
	long	m_nTypeLower;					// ���� ���� (��������,MASS����,���ұ���,�⼮����,�η¹��� Anchor)
	double	m_dThickCobbStone;				// �⼮���� �β�
	BOOL	m_bSlopeLowerSlab;				// �Ϻν����� �������� (FALSE), TRUE - �����߽ɱ���

	// ��Ŀ���� ����
	long	m_nCountAnchor;					// ��Ŀ ������ ����
	BOOL	m_bAnchorSameValue;				// ��Ŀ��,����,���� �������� (Ⱦ���� �Է½� 1���� �Է�)
	CDoubleArray	m_dArrDistJongAnchor;	// ��Ŀ ������ �Ÿ�
	CDoubleArray	m_dArrLoadAnchor;		// ��Ŀ��
	CDoubleArray	m_dArrLengthAnchor;	// ��Ŀ����
	CDoubleArray	m_dArrDiaAnchor;		// ��Ŀ����
	CDoubleArray	m_dArrEaAnchor;		// ��Ŀ���� (Ⱦ����)
	CDoubleArray	m_dArrDistHDanAnchor[RCBRIDGE_ANCHOR_MAX];	// ��Ŀ Ⱦ���� ����

	// ���ʺμ���
	long m_nGroundFactorKV;
	double m_dGroundFactorKV;
	long m_nGroundFactorBv;
	long m_nEquivalentApplyKsb;
	double m_dEquivalentApplyKsb;		// �������ݹݷ°�� ������ Ksb = �� �� Kv, �� = 1 / �Է°� 
	long m_n2DGeneralSpringType;
	long m_n2DEQSpringType;
	long m_nVerticalFactor;
	double m_dSpringSpaceBottom;
	double m_dSpringSpaceWall;
	BOOL m_bSpringSpaceIncludeWallTick;

	HGDOUBLE m_dMeshSizeArcCorner;		// �찢�� Mesh Size
	HGDOUBLE m_dMeshSizeBracket;		// �긮Ĺ Mesh Size	
	HGDOUBLE m_dMeshSizeBase;			// ���� Mesh Size
	HGDOUBLE m_dMeshSizeWing[2][2];			// ������ Plate Mesh Size ����, �¿�

	/////////////////////////////////////////////////////////////////////////

protected:
	CLineInfoApp *m_pLineBaseVert;	// ��ö�� ��ġ�� �纸����ġ�� ���ؼ� �ӽ÷� �����ϴ� baseline ��ü��

	// Ⱦ�� ���� �Է�
	long	m_nQtyHDan;							// Ⱦ�� ���� ����
	double	m_dLengthHDan[MAX_QTY_HDAN];		// �� Ⱦ�� ������ ����
	BOOL	m_bLevelHDan[MAX_QTY_HDAN];			// �� Ⱦ�� ������ ���� ó�� ����, m_bApplyLineSlope == TRUE �� ��쿡�� ��ȿ
	double	m_dSlopeHDan[MAX_QTY_HDAN]	;		// ���Ⱦ���, ö�������� ���
	long	m_nTypeHDan[MAX_QTY_HDAN];			// �� Ⱦ�� ������ ���� ����(��ȣ�� ��)
	BOOL	m_ZzbVariableLenHDan[MAX_QTY_HDAN];	// �� Ⱦ�� ������ ���� ���� ���� ����


public:
	double GetWidthHunchByJijum(long nJ, BOOL bUpper=TRUE);
	double GetWidthHunchBottomByJijum(long nJ, BOOL bUpper=TRUE);
	double GetHeightHunchByJijum(long nJ, BOOL bUpper=TRUE);
	BOOL IsRoundHunch(long nJ, BOOL bLeft, BOOL bUpperSlab);
	BOOL IsWall(long nJijum);
	BOOL GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2);
	// ���߶�౳���û���
	long	m_nQtyHDanRoad;						// Ⱦ�� ���� ����
	double	m_dLengthHDanRoad[MAX_QTY_HDAN];	// �� ������ ����
	BOOL	m_bLevelHDanRoad[MAX_QTY_HDAN];		// �� Ⱦ�� ������ ���� ó�� ����, m_bApplyLineSlope == TRUE �� ��쿡�� ��ȿ
	double	m_dSlopeHDanRoad[MAX_QTY_HDAN];		// �� ������ ����
	long	m_nTypeHDanRoad[MAX_QTY_HDAN];		// �� ������ ���� : ����,���,�ߺд�,����ũ
	BOOL	m_ZzbVariableLenHDanRoad[MAX_QTY_HDAN];	// �� Ⱦ�� ������ ���� ���� ���� ����
	long	m_ZznVariableLenHDanRoad[MAX_QTY_HDAN];	//������ ��. �� Ⱦ�� ������ ���� ���� ���� ����

	//��ȣ��
	void	MakeGuardWall(long nQtyMake, BOOL bRoadNotSlab=FALSE);
	long	GetQtyGuardWall(BOOL bRoadNotSlab=FALSE) const;	//200506 ���� GetQtyHDan() �� ������.
	CGuardWallRC* GetGuardWallByHDan(long nHDan, BOOL bRoadNotSlab=FALSE);
	CString GetGuardWallName(CGuardWallRC* pGW, BOOL bNameForExcelSheet=FALSE);

	CGuardWallRC* GetGuardWallRoadGuardFenceAndCentralWall(long nGW, BOOL bRoadNotSlab=FALSE);	// ��ȣ��, �ߺд�, �ߺд�1/2
	CGuardWallRC* GetGuardWallRoadFootway(long nGW, BOOL bRoadNotSlab=FALSE);			// ����
	CGuardWallRC* GetGuardWallRoadFootwayBlock(long nGW, BOOL bRoadNotSlab=FALSE);	// ������ ��輮
	CGuardWallRC* GetGuardWallRoadSoundProof(long nGW, BOOL bRoadNotSlab=FALSE);		// ������ ����
	CGuardWallRC* GetGuardWallRoadRetainingWall(long nGW, BOOL bRoadNotSlab=FALSE);	// �븷�̺�
	CGuardWallRC* GetGuardWallRoadHandRail(long nGW, BOOL bRoadNotSlab=FALSE);		// ��������	

	CGuardWallRC* GetGuardWallRailGuardFence(long nGW, BOOL bRoadNotSlab=FALSE);		// ����	
	CGuardWallRC* GetGuardWallRailDetectGravel(long nGW, BOOL bRoadNotSlab=FALSE);	// �ڰ�����(ö��)
	CGuardWallRC* GetGuardWallRailDetectGravel2(long nGW, BOOL bRoadNotSlab=FALSE);	// �ڰ�����(����)
	CGuardWallRC* GetGuardWallRailFootway(long nGW, BOOL bRoadNotSlab=FALSE);			// ����
	CGuardWallRC* GetGuardWallRailFootwayBlock(long nGW, BOOL bRoadNotSlab=FALSE);	// ����+����
	CGuardWallRC* GetGuardWallRailHole(long nGW, BOOL bRoadNotSlab=FALSE);			// ��ű�
	CGuardWallRC* GetGuardWallRailHoleFootway(long nGW, BOOL bRoadNotSlab=FALSE);		// ��ű�+����
	CGuardWallRC* GetGuardWallRoadRailDike(long nGW, BOOL bRoadNotSlab=FALSE);		// ��ű�+����

	long	GetQtyHDanTotalGuardFence(BOOL bRoadNotSlab=FALSE);

	long	GetQtyHDanRoadGuardFence(BOOL bRoadNotSlab=FALSE);			// ��ȣ��	
	long	GetQtyHDanRoadCentralWallFull(BOOL bRoadNotSlab=FALSE);		// �ߺд�
	long	GetQtyHDanRoadCentralWallHalf(BOOL bRoadNotSlab=FALSE);		// �ߺд�1/2
	long	GetQtyHDanRoadFootway(BOOL bRoadNotSlab=FALSE);				// ����
	long	GetQtyHDanRoadFootwayBlock(BOOL bRoadNotSlab=FALSE);		// ������ ��輮
	long	GetQtyHDanRoadSoundProof(BOOL bRoadNotSlab=FALSE);			// ������ ����
	long	GetQtyHDanRoadRetainingWall(BOOL bRoadNotSlab=FALSE);		// �븷�̺�	
	long	GetQtyHDanRoadHandRail(BOOL bRoadNotSlab=FALSE);			// ��������	
	long	GetQtyHDanRoadRailDike(BOOL bRoadNotSlab=FALSE);			// ����ũ	

	long	GetQtyHDanRailGuardFence(BOOL bRoadNotSlab=FALSE);			// ����	
	long	GetQtyHDanRailDetectGravel(BOOL bRoadNotSlab=FALSE);		// �ڰ�����(ö��)
	long	GetQtyHDanRailDetectGravel2(BOOL bRoadNotSlab=FALSE);		// �ڰ�����(����)
	long	GetQtyHDanRailFootway(BOOL bRoadNotSlab=FALSE);				// ����
//	long	GetQtyHDanRailDosangVer(BOOL bRoadNotSlab=FALSE);			// ����ĵ���
//	long	GetQtyHDanRailDosangGravel(BOOL bRoadNotSlab=FALSE);		// �ڰ�����
	long	GetQtyHDanRailFootwayBlock(BOOL bRoadNotSlab=FALSE);		// ����+����
	long	GetQtyHDanRailHole(BOOL bRoadNotSlab=FALSE);				// ��ű�
	long	GetQtyHDanRailHoleFootway(BOOL bRoadNotSlab=FALSE);			// ��ű�+����

	void	SetQtyHDan(long nQtyHDan) {m_nQtyHDan = nQtyHDan;}
	long	GetQtyHDan(BOOL bRoadNotSlab=FALSE) const;

	// Ⱦ�� ���� �Է�
	void	SetLengthHDan(long nHDan, double dHDanLen) { m_dLengthHDan[nHDan] = dHDanLen; }
	double	GetLengthHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;				//�� Ⱦ�� Ⱦ�� �Է� ����
	void	SetLevelHDan(long nHDan, BOOL bLevelHDan) {m_bLevelHDan[nHDan] = bLevelHDan;}
	void	SetSlopeHDan(long nHDan, double dSlopeHDan) { m_dSlopeHDan[nHDan] = dSlopeHDan; }
	double	GetSlopeHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;				//�� Ⱦ�� ����(����, �Է°�)
	void	SetTypeHDan(long nHDan, long nHDanType) { m_nTypeHDan[nHDan] = nHDanType; }
	long	GetTypeHDan(long nHDan, BOOL bRoadNotSlab=FALSE) const;					//�� Ⱦ�� Ⱦ�� ��ȣ�� Ÿ��

	void	SetQtyHDanRoad(long nQtyHDan) {m_nQtyHDanRoad = nQtyHDan;}
	void	SetLevelHDanRoad(long nHDan, BOOL bLevelHDan) {m_bLevelHDanRoad[nHDan] = bLevelHDan;}
	void	SetLengthHDanRoad(long nHDan, double dHDanLen) { m_dLengthHDanRoad[nHDan] = dHDanLen; }
	void	SetSlopeHDanRoad(long nHDan, double dSlopeHDanRoad) { m_dSlopeHDanRoad[nHDan] = dSlopeHDanRoad; }
	void	SetTypeHDanRoad(long nHDan, long nHDanType) { m_nTypeHDanRoad[nHDan] = nHDanType; }
	

	BOOL	IsLevelFrRoadCenterToEdge() const { return m_bLevelFrRoadCenterToEdge; }
	void	SetLevelFrRoadCenterToEdge(BOOL bLevelFrRoadCenterToEdge) { m_bLevelFrRoadCenterToEdge = bLevelFrRoadCenterToEdge; }
	void	SetLengthRailByLine(long nRail, double dRailLen) { m_dLengthRail[nRail] = dRailLen; }
	double	GetLengthRailByLine(long nRail) const { return m_dLengthRail[nRail]; }
	void	SetRailLineNumber(long nRail, long nLineNum) { m_nLineNumber[nRail] = nLineNum; }
	long	GetRailLineNumber(long nRail) const { return m_nLineNumber[nRail]; }
	long	GetQtyRailNode() const { return m_nQtyRail; }
	void	SetQtyRailNode(long nQtyRail) { m_nQtyRail = nQtyRail; }
	double	GetDisBaseElevation() const { return m_dDisBaseElevation; }
	void	SetDisBaseElevation(double dDisBaseElevation) { m_dDisBaseElevation = dDisBaseElevation; }
	double	GetSlopeDosang() const { return m_dSlopeDosang; }
	void	SetSlopeDosang(double dSlopeDosang) { m_dSlopeDosang = dSlopeDosang; }
	double	GetHeightDosangInput() const { return m_dHeightDosangUnderTie; }
	void	SetHeightDosang(double dHeightDosang) { m_dHeightDosangUnderTie = dHeightDosang; }

	double	GetDisBaseElevationRL() const { return m_dDisBaseElevationRL; }
	void	SetDisBaseElevationRL(double dDisBaseElevationRL) { m_dDisBaseElevationRL = dDisBaseElevationRL; }
	double	GetHeightRail() const { return m_dHeightRail; }
	void	SetHeightRail(double dHeightRail) { m_dHeightRail = dHeightRail; }
	BOOL	IsApplyLineSlope() const { return m_bApplyLineSlope; }
	void	SetApplyLineSlope(BOOL bApplyLineSlope) { m_bApplyLineSlope = bApplyLineSlope; }

	
	double	GetWidthHunch(long nJijum, BOOL bStt, BOOL bUpper=TRUE);
	double	GetThickHunch(long nJijum, BOOL bStt, BOOL bIncludeVertHunch, BOOL bUpper=TRUE);
	long	GetTypeColumnByJijum(long nJijum);
	double	GetThickPave(BOOL bRoadNotSlab=FALSE) const;

	
	BOOL	IsHunch(long nJ, BOOL bUpper=TRUE);
	void	SetDefaultedState(long nIDDialog, DWORD dwDefaultState);
	BOOL	IsDefaulted(long nIDDialog, DWORD dwDefault);
	BOOL	IsFirstDefault(long nIDDialog);
	double	GetLoadFactor(long nTypeCombo, long nCombo, long nLoad);
	BOOL	IsPileBase(long nJijum);

	CTypeDefault * GetTypeDefault(long nIDDialog);
	BOOL    GetXyMatchSegAndSegTwinVector(CDPoint xyStt,CDPoint xyEnd, CTwinVectorArray& tvArr, long nQtyXy, CDPoint &xyResult=CDPoint(0,0));
//	double	GetHeightDosangUnderTie();

protected:
	// ������ Ⱦ�� EL ����
	CDPoint m_Aslope[30];
	long m_nQtyHDanSlope;
	double m_dDisExpLeft;
	double m_dDisExpRight;
	BOOL m_bSetGagakDist;

private:
	CSafetyTypedPtrArray <CObArray, CWallApp*> m_pArrInWall;

public:
	BOOL IsIncludeEHP();
	CString GetEhpDxfNameByDia(long nKindDia);
	void Sort(CTwinVectorArray &tvArr, double dCorrectLevelOfDoubleLoacl=0.0002);
	BOOL IsApplyTosaHeight();
	long GetQtyHDanNonRebarConcrete(BOOL bRoadNotSlab=FALSE);
	BOOL IsApplyDisBaseElevationFL();
	void InitSaveDistRigidEndWall();
	void InitSaveDistRigidEndSlab();
	void InitSaveXyMatchLineAndSlabUpper();
	void InitSaveXySlabUpperByLenFrSttStation();
	void InitSaveOrgInWall();
	void InitSaveCrossSection();
	void SetDataDefaultBridgeType();
	BOOL IsBlockOut(BOOL bStt);
	void AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, double dDistStt, double dDistEnd, BOOL bPline=TRUE);
	void AddFromParabola(CTwinVectorArray &tvArr, CParabola &pb, BOOL bPline=TRUE);
	void AddFromArch(CTwinVectorArray &tvArr, CDPoint xyMove, double dLenTotal, double dHeightTotal, double dMFactor, double dLenStt, double dLenEnd, double dLenArchLine, BOOL bPline=TRUE);
	CString GetStringJigan();
	void SortTvArrByLinePlane(CTwinVectorArray &tvArr);
	long GetCountVariableLenHDan(BOOL bRoadNotSlab);
	void SetVariableLenHDan(long nDan, HGBOOL bRoadNotSlab, BOOL bVariable);
	BOOL IsVariableLenHDan(long nDan, HGBOOL bRoadNotSlab);
	BOOL IsOutWallHunch(BOOL bStt);
	void SetDataInitForChangeType();
	CBoringDataEx *GetBoringDataEx(long nJijum);

	double GetThickOutWallByLine(BOOL bStt) { return m_dWOutWallByLine[bStt ? iSTT : iEND]; }
	double m_dWOutWallByLine[2];

	// �ӵ������� ���ؼ� ���ܸ� �Է��� ������ ������� ����� �¿��߽ɼ������������� ���ܸ��� ��� ���� ���´�.
	// �׸��� ���� �׸��� ����� ���󼭵� ��� ���� ���´�.
	BOOL m_bSettingVertSection;	// ���������� ���õǾ� �ִ���
	BOOL m_bNeedSettingVertSection;	// ���������� ������ �ʿ�����?
	CTwinVectorArray m_tvArrVertSection_Vert_Left[3];
	CTwinVectorArray m_tvArrVertSection_Vert_Center[3];
	CTwinVectorArray m_tvArrVertSection_Vert_Right[3];

	// ������� ��� ������ ���� ����� ���ܸ鵵 ����ؾ� �ȴ�. 0 : �������, 1 : �����
	CTwinVectorArray m_tvArrVertSection_Ang_Left[3][2];
	CTwinVectorArray m_tvArrVertSection_Ang_Center[3][2];
	CTwinVectorArray m_tvArrVertSection_Ang_Right[3][2];

	// �������� ���� ������ǥ 
	// ��ġ�� ������
	// ��ġ���� m_tvArrVertSection�� ����/����������ؼ� ������ �ϹǷ� ���� ��ǥ�� �׻� �ʿ���
	CTwinVectorArray m_tvArrVertSection_NonModify_ForArch;

	// ������ ��� ��ǥ(���������� ������ �� �ǰ��� �Ѥ�)
	BOOL m_bSettingPlane;
	BOOL m_bNeedSettingPlane;
	CTwinVectorArray m_tvArrSttEndPlane[2][2];	// [2] : ������, [2] : ����
	CTwinVectorArray m_tvArrLeftRightPlane[2][2];	// [2] : �¿���, [2] : ����

	// Ⱦ�� ��ǥ ����
	CSafetyTypedPtrArray <CObArray, CSaveCrossSection*> m_pArrSaveCrossSection;
	// ������ü ������ ����
	CSafetyTypedPtrArray <CObArray, CSaveOrgInWall*> m_pArrSaveOrgInWall;
	//
	CSafetyTypedPtrArray <CObArray, CSaveXySlabUpperByLenFrSttStation*> m_pArrSaveXySlabUpperByLenFrSttStation;
	//
	CSafetyTypedPtrArray <CObArray, CSaveXyMatchLineAndSlabUpper*> m_pArrSaveXyMatchLineAndSlabUpper;
	//
	CSafetyTypedPtrArray <CObArray, CSaveDistRigidEndSlab*> m_pArrSaveDistRigidEndSlab;
	//
	CSafetyTypedPtrArray <CObArray, CSaveDistRigidEndWall*> m_pArrSaveDistRigidEndWall;

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	long m_nTypeJongdanForRebarOld;
	CTwinVectorArray m_tvArrVertSectionOld;
	BOOL m_bIsIncludeOnlyLine;	// ������ ������ �����ϴ���.

	long m_nLeftCurVertSection;	// ���� GetTvVertSection�� ���ؼ� ������ �ܸ��� �������� �������� �������� ���� 
	long m_nLeftCurVertSectionOld;	// ���� GetTvVertSection�� ���ؼ� ������ �ܸ��� �������� �������� �������� ���� 

	// �ߺ�ȣ�� ������
	long m_nLeftParaOld;
	BOOL m_bApplyAngleParaOld;
	BOOL m_bWingFrontParaOld;
	long m_nTypeJongdanParaOld;
	BOOL m_bForBMOfFootingParaOld;
	BOOL m_bFirstCalledGetTvVertSection_Std;
	BOOL m_bSecondCalledGetTvVertSection_Std;

	long m_nTypeJongdanForRebarOldOfOld;
	CTwinVectorArray m_tvArrVertSectionOldOfOld;
	long m_nLeftCurVertSectionOldOfOld;	// ���� GetTvVertSection�� ���ؼ� ������ �ܸ��� �������� �������� �������� ���� 
	CDPointArray m_xyArrOrdOldOfOld;
	CDoubleArray m_dArrElOldOfOld;

	// ���������� ���� ������� �������� - �������.
	BOOL m_bBridgeForBM;

	//////////////////////////////////////////////////////////////////////////
	// STM
	// �����(������), �찢��(������, ���ϴ�), ����������, �߰�����
	BOOL m_bApplyStmModelingBracket[2];	// �𵨸� Ÿ�� [����, ����]
	BOOL m_bStmTrussModelingBracket[2];	// Truss �𵨸� ���� (??, ����������) [����, ����]
	BOOL m_bApplyStmModelingCorner[2][3][2][2];	// [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][���, �ϴ�][-, +]
	BOOL m_bStmTrussModelingCorner[2][3][2][2];	// [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][���, �ϴ�][-, +]
	BOOL m_bApplyStmModelingFooting[2][3][5];	// [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������] - �����Ѱ�, 5�������ʹ� �Ѱ���¿����� ���
	BOOL m_bStmTrussModelingFooting[2][3][5];	// [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������] - �����Ѱ�, 5�������ʹ� �Ѱ���¿����� ���
	BOOL m_bApplyStmModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];	// [�������� ����][�����Ѱ�, �����Ѱ�1, �����Ѱ�2][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������]
	BOOL m_bStmTrussModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];	// [�������� ����][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������]
	// Stm - ��������
	CString m_strStmModelingCorner[2][3][2][2];						// �찢�� - [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][���, �ϴ�][-, +]
	CString m_strStmModelingBracket[2];								// �����
	CString m_strStmModelingFooting[2][3][5];						// ���� - ������ [����, ����][�����Ѱ�, �شܻ�Ȳ1, �شܻ�Ȳ2][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������]
	CString m_strStmModelingMidFooting[MAX_JIJUM_COUNT_RC][3][5];		// ���� - �߰����� [�������� ����][�����Ѱ�, �����Ѱ�1, �����Ѱ�2][����ִ�, �����Ʈ�ִ�, �θ��Ʈ�ִ�, ������]

	HGBOOL IsApplyStmBracket();
	HGBOOL IsApplyStmCorner();
	HGBOOL IsApplyStmFooting();
	HGBOOL IsApplyStmMidFooting();

	HGINT32 m_ixSolverStmTool;

	CStmModelForArcbridge *m_pStmSolverInput;
	std::map<int, CStmModelForArcbridge*> m_mStmSolver;
	std::map<int, CStmModelForArcbridge*> m_mStmSolverOut;

	void InitStmData();
	void DeleteStmData();
	void SerializeStm(CArchive &ar);
	//////////////////////////////////////////////////////////////////////////

	// (LSD:0000)
public:
	CLsdManagerBridge *m_plsdManager; //�������� �Ŵ���
	CLsdRebarManager *m_plsdRebar; //����,���� �Ŵ���
	CLsdEnviromentRating *m_plsdEnvRating; // ������, ������

	CLsdManagerBridge *m_plsdMngForCantilever; //�������� �Ŵ���

	HGBOOL m_bEtaCombinEach;					// ���߼������ ��ġ���� ���� ����

	double m_Fck[ePartCount];	
	double m_Fy[ePartCount];
	double m_Fvy[ePartCount];
	double m_Ec[ePartCount];
	double m_Fctk[eLsdStmCount];				// �Ѱ���� - ��ũ��Ʈ ���尭��
	HGBOOL m_bCheckCorner;						// �Ѱ���� - STM �찢�� �ִ� ���

	// �Ѱ���� ���� �߰�
	double m_dDepthApplySubSid;					// ����ħ�� ���밪
	long m_nSelectKindCoefficientKo;			// ������а�� ����
	long m_nSelectInteriorWaterPress;			// ���μ��� ����( EV, BP, WA )
	double m_dHeightInteriorWaterPress;			// ���μ��� �������
	double m_dTemperatureChangeLoadGap;			// �µ����� �µ���ȭ��
	double m_dTemperatureSlopeT[3];				// �µ���� [T1,T2,T3]
	long m_nApplyPavement;						// �µ���� - �Ϻ��� �µ��� ������ ���� : 0 : �ƽ���Ʈ����, 1 : ��ũ��Ʈ ����

	long m_nLoadEffeciveW;						// Ȱ���� - ��ȿ��
	long m_nLoadEquiStrip;						// Ȱ���� - ���Ʈ�� ���� �簢���
	HGBOOL m_bLoadMultiRoadSurConfficient;		// Ȱ���� - ��ȿ�� ���� ������ ���ϰ��
	long m_nLoadStandardLane;					// Ȱ���� - ǥ����������

	HGBOOL m_bCheckSupportSlabArea;			// ����ö�ٷ� ���� - ������
	HGBOOL m_bCheckSupportWallArea;			// ����ö�ٷ� ���� - ��ü
	HGBOOL m_bCheckSupportFootArea;			// ����ö�ٷ� ���� - ����
	HGBOOL m_bCheckSupportWingWallArea;		// ����ö�ٷ� ���� - ������
	HGBOOL m_bCheckSupportApsArea;			// ����ö�ٷ� ���� - ���ӽ�����

	HGBOOL m_bAggregate_SlumpDetail;	// ��� ����ġ �� �Է� - �������� �ɼ�
	HGDOUBLE m_dAggregate[ePartCount];	// ��ũ��Ʈ�����ִ�ġ��
	HGDOUBLE m_dAggregate_LeanLSD;		// ��ũ��Ʈ�����ִ�ġ��(����)
	HGDOUBLE m_dAggregate_MassLSD;		// ��ũ��Ʈ�����ִ�ġ��(Mass)
	HGDOUBLE m_dAggregate_HyuntaLSD;	// ��ũ��Ʈ�����ִ�ġ��(����Ÿ������)
	HGDOUBLE m_dAggregate_EtcLSD;		// ��ũ��Ʈ�����ִ�ġ��(��ȣ����)

	HGDOUBLE m_dSlump[ePartCount];		// ��ũ��ƮSlump
	HGDOUBLE m_dSlump_LeanLSD;			// ��ũ��ƮSlump(����)
	HGDOUBLE m_dSlump_MassLSD;			// ��ũ��ƮSlump(Mass)
	HGDOUBLE m_dSlump_HyuntaLSD;		// ��ũ��ƮSlump(����Ÿ������)
	HGDOUBLE m_dSlump_EtcLSD;			// ��ũ��ƮSlump(��ȣ����)

	HGDOUBLE m_dThetaShearDesign;				// ���ܰ���� �� : 0 = �ڵ����
	HGBOOL	m_bCheckDeltaTR;		// ���ܷ¿� ���� ������ �߰������ ����
	HGDOUBLE m_dElasticModularRatioUseCheck;	// ��뼺 ����� ��ȿź������� : 0 = ź�������
	long m_nSelectUsebilityCheck;				// ��뼺 ����� �տ�����
	long m_nTypeCrackWidthCondition;			// �տ������ 
	long m_nSelectExposureCoverCheck;			// �����Ǻ� ����
	long m_nSelectPminColumn;					// ��� �ּ� ö�ٺ�

	long m_nApplyCalcShearDistSlab;				// ������ ���ܰ���� ����ܸ� ��ġ
	long m_nApplyCalcShearDistWall;				// ��ü ���ܰ���� ����ܸ� ��ġ
	long m_nApplyCalcShearDistFrontPile;		// �ձ� ����� ����ܸ� ��ġ(���ұ���)
	long m_nApplyCalcShearDistBackPile;			// �ޱ� ����� ����ܸ� ��ġ(���ұ���)
	BOOL m_bApplyT1CalcWingWall;				// ������ D���� ö�ٷ� ������ ���� �β�

	std::vector<CBridgeUserLoadSet*> m_vpUserLoadSet[5];	// (ARCBRIDGE-3409) BOX�� ������ Case �߰�. 3->5
	CCheckCantileverData *m_pCheckCantilever;			// ĵƿ���� ���� - �ۿ�����
	
//	void InitFemManage();
	void InitLsdData();
	void DeleteLsdData();
	void SerializeLsd(CArchive &ar);
	BOOL IsUsePart(EPartOfBridge ePart, long nType);
	CString GetStringPart(EPartOfBridge ePart);
	CString GetStringPart(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	CString GetStringPartMerge(EPartOfBridge ePart);
	long GetLsdMatrialIndex(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	long GetLsdMatrialTypePos(EPartOfBridge ePart, HGBOOL bUpperOrFront);
	double GetCrackLimitWidth_LSD(EPartOfBridge ePart, HGBOOL bUpperOrFront, CString &sKind, CString &sDesingLevel);
	BOOL IsValidBridgePart(EPartOfBridge ePart);
	BOOL IsCheckPosEtaUseLoad(EPartOfBridge ePart);
	BOOL IsUseSelectLoadCaseWingOrApsMatchDesignRating(CString sGrade, EPartOfBridge ePart);
	BOOL IsUseLsdLoadFactor(long nIdexCombin);
	BOOL IsDesignRatingSelectAbutLoadCaseKind(ELSDCombKind eKind, EPartOfBridge ePart);
	BOOL IsValidComboEnvType(ETypeEnvComboLsd eTypeEnvLsd);
	CString GetStringComboEnv_Lsd(ELSDCombKind eKind, EPartOfBridge ePart, BOOL bMark=TRUE, BOOL bServiceCrack=FALSE);
	CString GetStringComboEta_Lsd(ELSDCombKind eKind, EPartOfBridge ePart, long nIndexCombo=-1, BOOL bOrigne=FALSE, long nIdxModel=0);
	CString GetStringComboEnv(long nTypeDesign, ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos);
	CString GetStringComboEnvType_Lsd(ETypeEnvComboLsd eTypeEnvLsd, BOOL bSubClass=TRUE, BOOL bTwoLine=FALSE);
	CString GetStringFootingCheckCase(EFootingCheckLsd eCheck);
	CString GetStringWallCheckCase(EWallCheckLsd eCheck);
	ELSDLoadGroupKind GetGroupKindByEnvType(ETypeEnvComboLsd eTypeEnvLsd);
	ELSDCombKind GetComboKindByEnvType(ETypeEnvComboLsd eTypeEnvLsd, EPartOfBridge ePart, long nPos);
	ELSDCombKind GetComboKindServiceByDesignGrade(EPartOfBridge ePart, long nPos);
	void SetDataDefaultSelectComboForWingWallAndAps(ELSDLoadGroupKind eType);
	long GetSizeComboByType(ETypeEnvComboLsd eEnvTypeLsd, EPartOfBridge ePart, CString &szComboFirst);
	long GetCountApplyEnvType(ETypeEnvComboLsd eCountEnvType = eCountEnvTypeLsd);

	HGINT32 GetCountUserLoadSetLsd(long nKind);
//	void ResizeUserLoadSetLsd(BOOL bNormal, HGINT32 icSize);
	void InsertUserLoadSetLsd(long nKind, HGINT32 Idx);
	void EraseUserLoadSetLsd(long nKind, HGINT32 Idx);
	CString GetStringUserLoadType(HGINT32 nType);

	CString GetStringCombo(ELSDCombKind eComboKind, long nIndexCombo, HGBOOL bForCantilever);
	CString GetLoadCaseName(ELSDLoadCaseKind eLoadCaseKind);

	HGBOOL &GetMaterialDetail();
	void SetMaterialDetail(HGBOOL bMaterialDetail);
	HGDOUBLE &GetRateDryAndShrinkage();
	void SetRateDryAndShrinkage(HGDOUBLE dRateDryAndShrinkage);
	HGDOUBLE &GetCoefficientExpansion();
	void SetCoefficientExpansion(HGDOUBLE dCoefficientExpansion);
	HGBOOL &GetDBar_ForDowelBar();
	void SetDBar_ForDowelBar(HGBOOL bIsDBar_ForDowelBar);
	long &GetTypeDesingEQ();
	void SetTypeDesingEQ(long nTypeDesingEQ);
	long &GetDegreeEarthQuake();
	void SetDegreeEarthQuake(long nDegreeEarthQuake);
	long &GetRegionEarthQuake();
	void SetRegionEarthQuake(long nRegionEarthQuake);
	HGDOUBLE &GetCoefficientGround();
	void SetCoefficientGround(double dCoefficientGround);
	long &GetTypeCoefficientGround();
	void SetTypeCoefficientGround(long dTypeGround);
	double &GetRateDAMP_EQ_CD();
	void SetRateDAMP_EQ_CD(double dDampRate);
	HGDOUBLE &GetGravity();
	void SetGravity(HGDOUBLE gValue);
	long &GetTypeRigidFootingEQ();
	void SetTypeRigidFootingEQ(long nTypeRigidFootingEQ);
	HGBOOL &GetApplyRigidWallEQRotate();
	void SetApplyRigidWallEQRotate(HGBOOL bApplyRigidWallEQRotate);
	long &GetTypeJointPosFootingEQ();
	void SetTypeJointPosFootingEQ(long nTypeJointPosFootingEQ);
	long &GetTypePropertyPosFootingEQ();
	void SetTypePropertyPosFootingEQ(long nTypePropertyPosFootingEQ);
	long &GetTypeSoilPressureEQ();
	void SetTypeSoilPressureEQ(long nTypeSoilPressureEQ);
	long &GetTypeUpperWeightEQ();
	void SetTypeUpperWeightEQ(long nTypeUpperWeightEQ);
	CString GetStringDegreeEarthQuake();

	// Stm
	HGDOUBLE &GetValueStmFck(long nStmType);
	HGDOUBLE &GetValueStmFctk(long nStmType);
	HGDOUBLE &GetValueStmEc(long nStmType);
	HGDOUBLE &GetValueStmFy(long nStmType);
	HGDOUBLE &GetValueStmFvy(long nStmType);
	
	CString GetStrLsdStmTitle(long nStmType);

	void InitAggregateAndSlump();

	HGDOUBLE GetValueAggregate(EPartOfBridge ePos, HGBOOL bNotLsd=FALSE) const;
	HGDOUBLE GetValueAggregateLean() const;
	HGDOUBLE GetValueAggregateMass() const;
	HGDOUBLE GetValueAggregateHyunta() const;
	HGDOUBLE GetValueAggregateEtc() const;
	
	HGDOUBLE GetValueSlump(EPartOfBridge ePos, HGBOOL bNotLsd=FALSE) const;
	HGDOUBLE GetValueSlumpLean() const;
	HGDOUBLE GetValueSlumpMass() const;
	HGDOUBLE GetValueSlumpHyunta() const;
	HGDOUBLE GetValueSlumpEtc() const;

	CString GetStringLoadTemperatureSlopeApply();

	long GetLsdEnvType(HGINT32 iKey);

	BOOL IsBoredPileASD(long nJijum);
	BOOL IsPhcPileASD(long nJijum);
	BOOL IsApplyEarthQuakeDesign();

	long GetEarthQuakeModelIndex(EGoalOfEarthQuake eGoalEq);
	long GetCountEarthQuakeModel(BOOL bResult=TRUE);
	EGoalOfEarthQuake GetEarthQuakeGoalByIndex(long nIndexEq);
	CString GetStringGoalEarthQuake(EGoalOfEarthQuake eGoalEq, BOOL bLong=TRUE);
	CString GetNameStringEarthQuakeModel(long nDesignType);

protected:
	void InitConcSlab();
	void InitConcWall();
	void InitConcInWall();
	void InitConcPRF();
	void InitConcFoot();
	void InitConcFootRev();
	void InitConcFootCheckAdd();

protected:
	HGBOOL m_bMaterialDetail;			// ��� ����ġ �� �Է�
	double  m_dRateDryAndShrinkage;		// ����������
	double  m_dCoefficientExpansion;	// ������ħ�� �̵��� ������ ����â���
	BOOL m_bIsDBar_ForDowelBar;	// ������ ����

	long m_nTypeDesingEQ;			// �����ؼ� ���
	long m_nDegreeEarthQuake;		// �������
	long m_nRegionEarthQuake;		// ��������
	double m_dCoefficientGround;	// ���ݰ��
	double m_gValue;				// �߷°��ӵ�
	long m_nTypeRigidFootingEQ;		// �����ؼ��� ���ʺ� �𵨸� ���
	BOOL m_bApplyRigidWallEQRotate;		// ��ü���� ����� ȸ������ ����
	long m_nTypeJointPosFootingEQ;		// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
	long m_nTypePropertyPosFootingEQ;	// �����ؼ��� ���� �߾����� �𵨸��ϴ� ����� ����
	long m_nTypeSoilPressureEQ;		// �����ؼ��� ��������� ����
	long m_nTypeUpperWeightEQ;		// �����ؼ��� ����߷� ����	
	long m_nTypeCoefficientGround;		// ��������
	double m_dRateDAMP_EQ_CD;		// �����(��)
};

#endif // !defined(AFX_RCBRIDGEDATA_H__309BC1BC_1309_46A5_85F4_48E62BBC506C__INCLUDED_)
