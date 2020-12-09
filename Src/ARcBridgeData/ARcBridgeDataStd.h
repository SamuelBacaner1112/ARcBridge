// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDataStd.h: interface for the CARcBridgeDataStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_)
#define AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ARcBridge Lock Version
#define  ARCBRIDGE_LOCK_CODE_PORTAL		0x0001
#define  ARCBRIDGE_LOCK_CODE_PI			0x0002
#define  ARCBRIDGE_LOCK_CODE_SLAB		0x0004
#define  ARCBRIDGE_LOCK_CODE_TRAP		0x0008
#define  ARCBRIDGE_LOCK_CODE_TRAP2		0x0010
#define  ARCBRIDGE_LOCK_CODE_ARCH		0x0020
#define  ARCBRIDGE_LOCK_CODE_BOX		0x0040
#define  ARCBRIDGE_LOCK_CODE_SLAB_STD	0x0080
#define  ARCBRIDGE_LOCK_CODE_STM		0x0100
#define  ARCBRIDGE_LOCK_CODE_LSD		0x0200

#define DESIGN_METHOD_STRENGTH		0
#define DESIGN_METHOD_LSD			1

#define DESIGN_CONDITION_CONCRETE	0
#define DESIGN_CONDITION_ROAD		1
#define DESIGN_CONDITION_RAIL		2
#define MAX_CONDITION_TYPE			3

#define DESIGN_CODE_KCI_2003		0
#define DESIGN_CODE_KCI_2007		1
#define DESIGN_CODE_KCI_2012		2

#define DESIGN_ROAD_2008			0
#define DESIGN_ROAD_2010			1
#define DESIGN_ROAD_2015			2
#define DESIGN_ROAD_KDS_2019		3

#define DESIGN_RAIL_2004			0
#define DESIGN_RAIL_2017			1
#define DESIGN_RAIL_KDS_2018		2

//���� ö�� 
#define JOINT_BEBD_RADIUS	0
#define JOINT_BEBD_CHAMFER	1

#define MAX_REBAR_DETAIL	45

#define REBAR_RADIUS		22.5

#define MAX_UPPER_TYPE		6
#define MAX_PIER_TYPE		7
#define MAX_CONDITION_TYPE	3
#define MAX_CRACK_ENV_TYPE	9

#define BMTYPE_NORMAL		0	// �Ϲݼ���
#define BMTYPE_RESULT		1	// ��������
#define MAX_BM_FORM_TYPE		8
#define MAX_BM_FORM_TYPE_NORMAL	7


// ���༱, ���༱
#define UPPER_LINE 0
#define LOWER_LINE 1

// ������ ���� 
#define UNIT_TYPE_CGS	0
#define UNIT_TYPE_SI	1
#define UNIT_TYPE_MAX	2

#define UNIT_KIND_MAX	20

//����
#define MAX_JIBAN_COUNT 20

#define JOINT_UPPERSLAB	0
#define JOINT_OUTWALL		1
#define JOINT_MIDWALL		2
#define JOINT_NLLINK		3
#define JOINT_ARCHRIB		4
#define JOINT_LOWERSLAB	5

#define ELEMENT_UPPERSLAB	0
#define ELEMENT_STTWALL	1
#define ELEMENT_ENDWALL	2
#define ELEMENT_MIDWALL	3
#define ELEMENT_RIGID		4
#define ELEMENT_ARCHRIB	5
#define ELEMENT_LOWERSLAB	6

//////////////////////////////////////////////////////////////////////////
#define LOAD_CASE_DEAD			0x00000001 // ��������, �������
#define LOAD_CASE_SOIL			0x00000002 // ��������, �������
#define LOAD_CASE_LIVE_FULL	0x00000004 // Ȱ���� (����)
#define LOAD_CASE_LIVE_ECC		0x00000008 // Ȱ���� (����)
#define LOAD_CASE_TANK			0x00000010 // ��ũ �� Ư�� ����������
#define LOAD_CASE_TOAP_BOTH	0x00000020 // �����
#define LOAD_CASE_TOAP_ONE		0x00000040 // �����
#define LOAD_CASE_TEMP1		0x00000080 // �µ�����(+)
#define LOAD_CASE_TEMP2		0x00000100 // �µ�����(-)
#define LOAD_CASE_SHRINK		0x00000200 // ��������
#define LOAD_CASE_SUBSID		0x00000400 // ����ħ��
#define LOAD_CASE_SB			0x00000800 // �õ� �� ��������
#define LOAD_CASE_LR			0x00001000 // ��뷹�� ������
#define LOAD_CASE_EQ			0x00002000 // ��������
#define LOAD_CASE_WATERHORI	0x00004000 // �������
#define LOAD_CASE_WATERVERT	0x00008000 // ��������

// LOAD_CASE�� ���� ������ ����.... //////////////////////////////////////
#define BRIDGE_LOAD_DEAD		 0 // ��������
#define BRIDGE_LOAD_SOIL		 1 // �������
#define BRIDGE_LOAD_LIVE_FULL	 2 // Ȱ���� (����)
#define BRIDGE_LOAD_LIVE_ECC	 3 // Ȱ���� (����)
#define BRIDGE_LOAD_TANK		 4 // ��ũ �� Ư�� ����������
#define BRIDGE_LOAD_TOAP_BOTH	 5 // �����
#define BRIDGE_LOAD_TOAP_ONE	 6 // �����
#define BRIDGE_LOAD_TEMP1		 7 // �µ�����(+)
#define BRIDGE_LOAD_TEMP2		 8 // �µ�����(-)
#define BRIDGE_LOAD_SHRINK		 9 // ��������
#define BRIDGE_LOAD_SUBSID		10 // ����ħ��
#define BRIDGE_LOAD_SB			11 // �õ� �� ��������
#define BRIDGE_LOAD_LR			12 // ��뷹�� ������
#define BRIDGE_LOAD_EQ			13 // ��������
#define BRIDGE_LOAD_WATERHORI	14 // �������
#define BRIDGE_LOAD_WATERVERT	15 // ��������
#define BRIDGE_LOAD_BASIC		16 // �⺻������ ����
//////////////////////////////////////////////////////////////////////////

#define LOAD_COMBINE_USE	0
#define LOAD_COMBINE_ULT	1
#define LOAD_COMBINE_ECC	2
#define LOAD_COMBINE_EQ	3

// ������ġ
#define CHECK_POS_LEFT		0	
#define CHECK_POS_TOP		0
#define CHECK_POS_CENTER	1
#define CHECK_POS_RIGHT	2
#define CHECK_POS_BOTTOM	2

// ������ (���ϸ��)
#define EARTHQUAKE_LOAD_DEAD		0 // ��������
#define EARTHQUAKE_LOAD_TRAP_X		1 // ������ TRAP����
#define EARTHQUAKE_LOAD_TRAP_Y		2 // Ⱦ���� TRAP����
#define EARTHQUAKE_LOAD_TOAP		3 // �������,�������
#define EARTHQUAKE_LOAD_INERTIAL	4 // ������

// ö������ //////////////////////////////////////////////////////////////////////////
#define RAIL_LOAD_KRL2012	0
#define RAIL_LOAD_KRL2012_P	1
#define RAIL_LOAD_EL18_2017	2
#define RAIL_LOAD_USER_2017	3
#define MAX_RAIL_LOAD_2017	4

#define RAIL_LOAD_LS25	0
#define RAIL_LOAD_LS22	1
#define RAIL_LOAD_LS18	2
#define RAIL_LOAD_EL25	3
#define RAIL_LOAD_EL22	4
#define RAIL_LOAD_EL18	5
#define RAIL_LOAD_HL25	6
#define RAIL_LOAD_USER	7
#define MAX_RAIL_LOAD	8

#define RAIL_LOADTYPE_PD	0
#define RAIL_LOADTYPE_P		1
#define RAIL_LOADTYPE_W		2

#define UNIT_METER	1.000
//////////////////////////////////////////////////////////////////////////

// ���� ����.
#define TYPE_PROCESS_NORMAL					0	// �Ϲ�
#define TYPE_PROCESS_SLAB_STANDARD_OUTPUT	1	// ������ǥ�ص�

//��ö�ٱ�����
#define CUL_MAX_REBAR	40	

// LSD STM Footing
#define LSD_STM_FOOTING_ULT		0
#define LSD_STM_FOOTING_EXT1	1
#define LSD_STM_FOOTING_EXT2	2
#define LSD_STM_FOOTING_MAX		3

struct SStxOptionItem
{
	CString name;
	HGBOOL check;

	HGINT32 designType;
	HGINT32 iRcBridge;
	HGINT32 ixJijum;

	SStxOptionItem()
	{
		name = NOTRANS("");
		check = TRUE;

		designType = 0;
		iRcBridge = 0;
		ixJijum = 0;
	}

	BOOL operator==(const CString &compare_name)
	{
		if(name == compare_name)
			return TRUE;
		return FALSE;
	}
};

class CLineInfoApp;
class CLineInfo;
class CRcBridgeRebar;
class CRcBridgeApp;
class CRcBridgeData;
class CWallApp;
class CColumnApp;
class CBoringDataEx;
class CExpJointRefArr;
class CLsdEnviromentCoef;
class CLsdRebarManager;
class CUserBmData;

class AFX_EXT_CLASS CARcBridgeDataStd : public CObject
{
public:
	void SerializeBase(CArchive &ar);
	BOOL IsSupportTypeStd(long nType);
	void MakeLinefoBridge(long nBridge);
	BOOL IsEnableSlabStandardOutput();
	void SetLineAppToBridge();
	CString GetFileNameForExcel(long nBri);
	void SetVersionFlag(DWORD dwType);	
	BOOL IsLockMaster();
	BOOL IsLockRcBridgePlus();	
	BOOL IsLockRcBridgeBasic();
	BOOL IsLockRcBridgePortal();
	BOOL IsLockRcBridgeBoxModule();
	BOOL IsLockRcBridgeBox();
	BOOL IsLockRcBridgeStmModule();
	BOOL IsLockRcBridgeLsdModule();
	BOOL m_bOnlyProjectInfoLoad;	// Ư���� �ɼ�.. ������Ʈ ������ �о� ����??
	double GetValueBasic(double dDia, long nType);
	double GetValueBasic(long nDia, long nType);
	BOOL IsRailLoadEL();
	BOOL IsRailLoadLS();
	void SetStringUnitType();
	BOOL IsSI();
	double GetUnitWeightReinforcedConcrete();
	double GetValueUnitType(double dValue, CString csOrg, CString csChg);
	CString GetStringUnitType(CString sCGS, long nIdxUnitApply=0);

	CString GetBridgeTotalName(long nBri);
	void CopyToCoupleBridge_RebarStandardSection(long nBri);
	void CopyDoubleArray(CDoubleArray *dArrTarget, CDoubleArray *dArrSource);
	void CopyArrayOfDoubleArray(CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrTarget, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrSource);
	void CopyArrayOfSetRebarJoint(CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrSource);
	void CopyArrayOfSetRebarDan(CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrSource);
	void CopyToCoupleBridge_RebarMain(long nBri);
	void CopyToCoupleBridge_VertSection(long nBri);
	void CopyToCoupleBridge_SpanConst(long nBri);
	long GetIdxMatchBridge(long nBri);
	CRcBridgeApp* GetBridgeMatch(CRcBridgeApp* pBri);
	CRcBridgeRebar* GetBridgeMatchRebar(CRcBridgeRebar* pBri);
	CRcBridgeApp* GetBridge(long nBridge=0);
	CRcBridgeRebar* GetBridgeRebar(long nBridge=0);
	CLineInfoApp* GetLineApp();
	BOOL MakeRcBridgeForBM(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM, BOOL bForTogong=FALSE);
	void SyncBridgeFactor(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM);
	// rcbridge
	long GetBridgeSize(BOOL bBridgeCount=FALSE);

	// common
	void GetPointArrayCountAndValue(CDPointArray& xyArr, CDoubleArray& dArr);
	void SerializeArrayOfDoubleArray(CArchive &ar, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArr);
	void SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr);
	double GetDia(CString str);
	CString GetStringDia(double dDia, double dFy=0);
	BOOL IsValidLine() const;

	double GetValueBendReBar(long nDia, int nTypeValue);
	double GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueJoint (double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
	double GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp);
	double GetValueSettle(double nDia, long nType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);

	CString GetStringBridgeOrder(int nBri);
	CString GetStringCrackDesign(long nType);
	CString GetStringConditionApply(long nType);
	CString GetStringBM_FormType(long nPart);
	CString GetStringCircleNum(long nNum);
	CString GetStringBridgeDir(long nBri);
	CString GetStringTypeCoefficientGround(long nType);
	
//	long GetTypeCrackDesign(CString strType);
//	long GetTypeConditionApply(CString strType);

	BOOL IsExistMainRebarStraightHorz(CColumnApp *pColumn, long nDan);
	BOOL IsExistMainRebarStraightVert(CColumnApp *pColumn, long nDan);
	CString GetStringLoadDB(BOOL bDL=FALSE);

	void SeparateRebarListByDescription(CTypedPtrArray <CObArray, CRebar*> *pArrRebar1, CTypedPtrArray <CObArray, CRebar*> *pArrRebar2, CString sDescription);
	// ���α� ������� 2010������ �Ǵ�.

	static CString m_sConditionApply[MAX_CONDITION_TYPE];
	static CString m_sEnvCrack[MAX_CRACK_ENV_TYPE];
	static CString m_sBM_FormType[MAX_BM_FORM_TYPE];
	static CString m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL];
	
//	BOOL CheckDataValidity();

	CARcBridgeDataStd();
	BOOL MakeRcBridge(long nTotal);
	void MakeSoilLayer(int nBoring, int nCountLayer);

	virtual ~CARcBridgeDataStd();
	void Serialize(CArchive &ar);
	
	CDocument* m_pDoc;	// doc
	
	CString m_strTempName;	// ���� �ʱ�ȭ�� ���� �ӽ� ����	

	// �ø��� �÷���
	long m_nFlag;

	// ������Ʈ ����
	CString m_strProjectName;		// �����
	CString m_strOwnerName;			// ����ó
	CString m_strConstructorName;	// �ð���
	CString m_strSpotName;			// �����
	CString m_strEngName;			// �����
	CString m_strLineDirName;		// ���� ����
	CString m_strLineRevDirName;	// ���� �ݴ� ����

	// ���α׷� ���� ����
	long m_nTypeProcess;	// �������� ( 0 : �Ϲ�, 1 : ������ ǥ�ص�)

	CBasicConcInfo *m_pBasicConcInfo;
	CExpJointRefArr *m_pExpJointRefArr;		// ������������

	// ��°���
	BOOL	m_bExcelDefaultSetting;
	CString m_szFontPrint;
	double m_dFontSizePrint;
	double m_dOffsetLeftPrint;
	double m_dOffsetRightPrint;
	double m_dOffsetTopPrint;
	double m_dOffsetBottomPrint;
	long m_nPageRow;
	// �ɹ� Ŭ����
	CLineInfoApp* m_pLineApp;		// ���� ����
	CTypedPtrArray <CObArray, CRcBridgeRebar*> m_pRcBridgeArray;	// �긮�� ��ü

	// ���� ���� �Է�
	CSafetyTypedPtrArray <CObArray, CBoringDataEx*> m_pArrBoringEx;
	std::map<HGINT32, CBoringDataEx*> m_mpBoring;		// EL���� ���� ����
	// ����ȯ�� ------------------------------------------------------- 
	long m_nTypeConditionApply;	// ���뼳�����
	long m_nTypeKciCode;			// KCI CODE
	long m_nTypeRoadCode;			// ���α� �������	(0 : 2008, 1 : 2010)
	long m_nTypeRailCode;			// ö���� �������	(0 : 2004, 1 : 2017)
	long m_nDegreeBridge;			// ���豳���� ���
	long m_nTypeUnit;				// ������ ����
	long m_nApplyBoredPileASD;			// ���Ը��� ����� ������¼���� ���� 
	long m_nApplyPhcPileASD;			// PHC���� ����� ������¼���� ����
	long m_nTypeCheckSteelPile;		// �������� ������� 
	BOOL m_bPrintConditionApply;			//���������� ��¹��(TRUE: �⺻��������, FALSE: ������������)
	long m_nTypeEarthQuakeApply;	// �����������

	double m_Fck;
	double m_Fy;
	BOOL m_bIsDBar_ForDowelBar;	// ������ ����
	double m_Ec;
	double m_Es;
	double m_Pi_F;
	double m_Pi_C;
	double m_Pi_V;
	double m_Pi_F_Cobel;				// �ں��� ���ܷ�
	double m_RatePoasong;				// ��ũ��Ʈ�� ���ƽ���
	double m_PI_B;						// ��ũ��Ʈ�� ����
	double m_Es_Steel;					// ���� ź�����
	double m_NValue_Steel;				// ����� ��ũ��Ʈ�� ź�������
	double m_Gs_Steel;					// ������ ����ź�����
	double  m_dDeltaTemperature;		// ������ħ�� �̵��� ������ �µ���ȭ��
	double  m_dRateDryAndShrinkage;		// ����������
	double  m_dCoefficientExpansion;	// ������ħ�� �̵��� ������ ����â���

	long m_nSelectPmin_Hyunta_Extreme;	// �شܻ�Ȳ�Ѱ���¥��� ����Ÿ������ ö�ٺ�, AAB-5626

	double m_dLengthRebarMax;		// ö�� ������ ���� ����
	long m_nDegreeEarthQuake;		// �������
	long m_nRegionEarthQuake;		// ��������
	double m_dCoefficientGround;	// ���ݰ��
	long m_nTypeCoefficientGround;	// ��������
	double m_dRateDAMP_EQ_CD;		// �����(��)
	double m_gValue;				// �߷°��ӵ�
	long m_nTypeDesingEQ;			// �����ؼ� ���
	long m_nTypeRigidFootingEQ;		// �����ؼ��� ���ʺ� �𵨸� ���
	BOOL m_bApplyRigidWallEQRotate;		// ��ü���� ����� ȸ������ ����
	long m_nTypeJointPosFootingEQ;		// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
	long m_nTypePropertyPosFootingEQ;	// �����ؼ��� ���� �߾����� �𵨸��ϴ� ����� ����
	long m_nTypeUpperWeightEQ;		// �����ؼ��� ����߷� ����
	long m_nTypeComboEQ;			// �����ؼ������ ���� ���� ���
	long m_nTypeSoilPressureEQ;		// �����ؼ��� ��������� ����
	double m_dRateDAMP_EQ;			// �����ؼ��� �����
	long m_nTypeRebarConnect;		// ������ �� ������ ������
	long m_nTypeBMApply;			// �������� �������(0:�Ϲݼ����������, 1:���������������(��������и�), 2:���������������(�����������))
	BOOL m_bSepFootingForSiljuk;	// ������������� ���ʸ� �ٴڹ׺����� �и��ؼ� ����
	BOOL m_bRoundRebarConnect_XX;	// ö�� ������, �������� �ݿø�
	BOOL m_bIncludeSumZero;			// �������迡 �����(zero) ����
	BOOL m_bApplyAllowInc;			// �������� ����� ������� ������� ����
	long m_nTypeFileNameForExcel;	// ���� ��½� ���ϸ�(0 : ���ϸ�_������_����, 1 : ������_����, 2 : ���ϸ�_����, 3 : ���ϸ�_������, 4 : ������)
	long m_nTypeCalcSlabEffWidth;	// ö���� ������ ��ȿ�� ������ ��� // 0 - ������ �β� ����, 1- ������ �β� ����
	long m_nTypeFootingSafetyApplyCoefficientImpact;		// ö���� ��������� Ȱ���� ��� ����
	// ------------------------------------------------------ ����ȯ�� 

	// ���� ����
	int m_nHowLineInput;			// ���� �Է� ��� 0 : ����, 1 : ���� �Է�
	CString m_strLineFilePath;		// ���� ���� ���
	BOOL m_bMakeDrawingOptionUsed;	// MakeDrawingOption�� �� ���� ���ֱ� ���� ����

	//////////////////////////////////////////////////////////////////////////
	// ö�� ����
	long	m_nDegreeRail;
	long	m_nTypeRailLoad;
	long	m_nTypeRailQty;
	long	m_nRailQtyCar;	// EL���� ������� ������
	CString m_sNameRailLoad;			// ������ �̸�,LS-25.....
	double  m_DistributionLoadFront;	// �������� ��������
	double  m_DistributionLoadBack;		// �������� ��������
	double  m_RailLoadWheel;			// ��������
	double  m_RailDistWheel;			// ���
	double  m_RailDistWheelMid;			// ��� (�߰��κ�)
	CDoubleArray m_dArrTypeLoadTrainWheel;		// ���������� ����
	CDoubleArray m_dArrDistTrainWheel;	// ���������� �Ÿ�, �������߰��� �Ÿ�
	CDoubleArray m_dArrLoadTrainWheel;	// ���������� ũ��, �������߸�....

	// ��ũ �� ������ ����
	double m_dTank_i;				
	double m_dTank_WeightTank;
	double m_dTank_TankLength;		
	double m_dTank_TankWidth_Side;
	double m_dTank_TankWidth_Inner;
	
	double m_dTank_TrailerL1;		
	double m_dTank_TrailerL2;		
	double m_dTank_TrailerL3;		
	double m_dTank_TrailerL4;
	double m_dTank_TrailerWidth_Side;
	double m_dTank_TrailerWidth_Inner;
	double m_dTank_Ptrailer1;
	double m_dTank_Ptrailer2;
	double m_dTank_Ptrailer3;	

	// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
	long m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�) (��������)
	long m_nTypePosBaseLine;	// ���ٸ� ���ؼ�(0 : ���ݼ�, 2 : ����) (��������)
	double m_dThickPostUpper;	// ������ ���ٸ� ��� �β� (��������)
	double m_dThickPostSide;	// ������ ���ٸ� �¿��� �β� (��������)
	double m_dThickScaffold;	// ��� �β� (��������)	
	
	BOOL m_bFootLenInputFloat;	// ���ʱ��� �Է¹����� �Ҽ��� �Է�.

	static CString m_sRailLoadType[MAX_RAIL_LOAD];
	static CString m_sRailLoadType2017[MAX_RAIL_LOAD_2017];

	BOOL IsRailDesign();
	BOOL IsRailDesign2017();
	BOOL IsRailDesignKDS2018();
	BOOL IsKRL_2012();
	BOOL IsDoro2010();
	BOOL IsDesignKDS2019();
	double GetEs_Steel();
	CString GetStringRailLoad(long nLoadType);
//	long GetTypeRailLoad(CString strLoad);
	long GetTypeConcDesign();

	CString GetStringLoadCase(long nLoadCase, BOOL bEarthQuake=FALSE);
	void SetRailLoad();
	BOOL GetRailLoadWheelInfo(long nIdxWheel, long &nType, double &dWeight, double &dDist);
//	void SetRailLoadWheelInfo(long nIdxWheel, long nType, double dWeight, double dDist);
	double GetRailLoadWeightWheel(long nSttWheel, double dLengthSpan);
//	double GetRailLoadDistWheel(long nWheel);
	double GetRailLoadLengthWheel(long nSttWheel);
	long GetCountRailLoadWheel();
	double GetPmaxColumn();

	CString m_szKgf;
	CString m_szTonf;
	CString m_szKgfM;
	CString m_szTonfM;
	CString m_szCM;
	CString m_szCM2;
	CString m_szCM3;
	CString m_szKgf_CM2;
	CString m_szKgf_CM3;
	CString m_szKgf_M2;
	CString m_szKgf_M3;
	CString m_szTonf_M;
	CString m_szTonf_M2;
	CString m_szTonf_M3;
	CString m_szM;
	CString m_szM2;
	CString m_szM3;
	CString m_szM4;

	// COutBridgeResister���� ���� �Լ� ����.
	CString GetStringFootTypeAll(CRcBridgeRebar *pBri);
	CString GetStringExcelFileForm();

	void SyncApsDowelRebarSeperateRebarOption(BOOL bSet, long nBri = -1);

private:
	DWORD m_dwTypeARcBridge;	// ARcBridgeŸ��	

	void SetFooting_Qa_Pa__Calculate();

// (LSD:0000)
public:
	std::vector<SStxOptionItem> m_vStxOption;		// STM ��������

	long m_nTypeDesignMethod;	// ��������� or �Ѱ���¼����
	long m_nLSDRoadType;		//LSD :������ ���� (0: �Ϲݵ���, 1: ��ӵ���)
	HGDOUBLE m_dDesignRoadWidth; //LSD : ���������� ��(Wp)

	CLsdEnviromentCoef  *m_plsdEnv; //���ǥ �Ŵ���
	CLsdRebarManager *m_plsdRebar; //����,���� �Ŵ���		

	void SerializeLsd(CArchive &ar);
	// ���������(����)���� �Ѱ���¼�������� �Ǵ�
	BOOL IsLsdDesign();
	void SetChangeDesignMethod();
	void SetDataDefaultMatResistFactor();
	void SetDataDefaultBaseResistFactor();
	void SetDataDefaultBaseResistFactor_SteelPile();

	BOOL IsApplyBoredPileASD(CExPileApp *pPile = NULL);
	BOOL IsApplyPhcPileASD(CExPileApp *pPile = NULL);

	CString GetStrDouble(double dValue, CString sUnitCGS, BOOL bLong=TRUE, long nDec=-1, BOOL bDf=FALSE, long nSpace=-1, BOOL bExponent=FALSE);
	long GetDecimal(CString sUnitCGS, long nDec=-1, BOOL bLong=TRUE);
	CString GetStrDoubleUnitChange(double dValue, CString csUnitOrg, CString csUnitChg, BOOL bLong=TRUE, long nDec=-1, BOOL bDf=FALSE, long nSpace=-1, BOOL bExponent=FALSE);

	CUserBmData *m_pUserBmData;
};

#endif // !defined(AFX_ARCBRIDGEDATASTD_H__AF6DC252_6495_4B2F_9E7C_FC5E3C68F4B3__INCLUDED_)
