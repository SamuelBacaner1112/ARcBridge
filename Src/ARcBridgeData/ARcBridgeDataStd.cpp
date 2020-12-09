// ARcBridgeDataStd.cpp: implementation of the CARcBridgeDataStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CARcBridgeDataStd::m_sRailLoadType2017[MAX_RAIL_LOAD_2017] = 
{			
	"KRL-2012",
	"KRL-2012(��������)",
	"EL-18",
	"���������"
};

CString CARcBridgeDataStd::m_sRailLoadType[MAX_RAIL_LOAD] = 
{			
	"LS-25",
	"LS-22",
	"LS-18",
	"EL-25",
	"EL-22",
	"EL-18",
	"HL-25",
	"���������"
};

CString CARcBridgeDataStd::m_sConditionApply[MAX_CONDITION_TYPE] = 
	{			
		"��ũ��Ʈ �����������",
		"���α� �������",
		"ö�� �������"
		//"���ö�� �������"
	};
CString CARcBridgeDataStd::m_sEnvCrack[MAX_CRACK_ENV_TYPE] = 
	{			
		"���� ȯ��",
		"���� ȯ��",
		"�νļ� ȯ��",
		"��νļ� ȯ��",
		"������ ���� �Ǵ� ��ȣ��",
		"����, ���� ����, ��",
		"���� ȭ����",
		"�ؼ� �� �ؼ���� ; �ǽ�",
		"���� ��� ������(�з��� ���� ��Ʈ ����)"

	};
CString CARcBridgeDataStd::m_sBM_FormType[MAX_BM_FORM_TYPE] = 
	{			
		"��ģ����",
		"���븶��",
		"�Ų��Ѹ���",
		"����� Ư¡�ִ� ���� (���̰�Ǫ��)",
		"����� Ư¡�ִ� ���� (�����Ǫ��)",
		"����� Ư¡�ִ� ���� (���ð�Ǫ��)",
		"����� Ư¡�ִ� ���� (DECK PLATE)",
		"����� Ư¡�ִ� ���� (1ȸ ���)"
	};

CString CARcBridgeDataStd::m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL] = 
	{			
		"����6ȸ",
		"����4ȸ",
		"����3ȸ",
		"���̰�Ǫ��",
		"�����Ǫ��",
		"������Ǫ��",
		"������"
	};


CARcBridgeDataStd::CARcBridgeDataStd()
{
	m_bOnlyProjectInfoLoad	= FALSE;	// Ư���� �ɼ�.. ������Ʈ ������ �о� ����??
	m_strTempName = _T("�⺻ ����");	// ���� �ʱ�ȭ�� ���� �ӽ� ����

	m_nFlag		= 0;
	// �޸𸮵����Ҵ� ��� ���°�.
	// ������ ������� �Ҹ��ڿ��� �޸������� ������ ���ֱ� ���� �Ѱ��� ��Ƶ�.
	m_pLineApp = new CLineInfoApp;
	m_pBasicConcInfo = new CBasicConcInfo(FALSE);
	m_pExpJointRefArr = new CExpJointRefArr;

	// �׽�Ʈ
	MakeRcBridge(0);

	// ������Ʈ ����
	m_strProjectName	 = _T("");	// �����
	m_strOwnerName		 = _T("");	// ����ó
	m_strConstructorName = _T("");	// �ð���
	m_strSpotName		 = _T("");	// �����
	m_strEngName		 = _T("");	// �����
	m_strLineDirName	 = _T("");	// ���� ����
	m_strLineRevDirName	 = _T("");	// ���� �ݴ� ����
	m_nTypeProcess	= TYPE_PROCESS_NORMAL;	// �������� ( 0 : �Ϲ�, 1 : ������ ǥ�ص�)

	// ����ȯ�� ------------------------------------------------------- 
	m_nTypeConditionApply		= -1;
	m_nTypeKciCode			= DESIGN_CODE_KCI_2003;
	m_nTypeRoadCode			= DESIGN_ROAD_2008;
	m_nTypeRailCode			= DESIGN_RAIL_2004;
	m_nDegreeBridge			= 0;
	m_nTypeUnit				= UNIT_TYPE_SI;		// ������
	m_RatePoasong				= 0.16667;			// ��ũ��Ʈ�� ���ƽ���
	m_Fck						= 270;
	m_Fy						= 4000;
	m_bIsDBar_ForDowelBar		= TRUE;	// ������ ����
	m_Ec						= GetValueEcConc(m_Fck); // ���α�������� 38������
	m_Es						= 2000000;
	m_Es_Steel					= GetEs_Steel();
	m_Pi_F						= 0.85;
	m_Pi_C						= 0.70;
	m_Pi_V						= 0.80;
	m_Pi_F_Cobel				= 0.80;
	m_PI_B						= 0;				// ��ũ��Ʈ�� ����
	m_dDeltaTemperature			= 20;				// ������ħ�� �̵��� ������ �µ���ȭ��
	m_dCoefficientExpansion		= 0.000010;			// ����â���
	m_dRateDryAndShrinkage		= 0.00015;			// ����������
	m_dLengthRebarMax			= 8000;				// ö�� ������ ���� ����
	m_nDegreeEarthQuake			= 0;				// �������
	m_nRegionEarthQuake			= 0;				// ��������
	m_dCoefficientGround		= 1.0;				// ���ݰ��
	m_nTypeCoefficientGround	= 1;
	m_dRateDAMP_EQ_CD			= 5;
	m_gValue					= 9.810;			// �߷°��ӵ�
	m_nTypeDesingEQ			= 1;				// �����ؼ� ���
	m_nTypeRigidFootingEQ		= 0;				// �����ؼ��� ���ʺ� �𵨸� ���
	m_bApplyRigidWallEQRotate	= FALSE;
	m_nTypeJointPosFootingEQ	= 0;				// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
	m_nTypePropertyPosFootingEQ = 0;
	m_nTypeUpperWeightEQ		= 0;				// �����ؼ��� ����߷� ����
	m_nTypeComboEQ				= 0;				// �����ؼ������ ���� ���� ���
	m_nTypeSoilPressureEQ		= 0;
	m_dRateDAMP_EQ				= 0.05;				// �����ؼ��� �����
	m_nTypeRebarConnect			= 1;				// ������ �� ������ ������
	m_nTypeBMApply				= 0;				// �������� �������
	m_bSepFootingForSiljuk		= FALSE;			// ������������� ���ʸ� �ٴڹ׺����� �и��ؼ� ����
	m_bRoundRebarConnect_XX		= TRUE;				// ö�� ������, �������� �ݿø�
	m_bIncludeSumZero			= TRUE;				// �������迡 �����(zero) ����
	m_bApplyAllowInc			= TRUE;				// �������� ����� ������� ������� ����
	m_nTypeFileNameForExcel		= 0;	// ���� ��½� ���ϸ�(0 : ���ϸ�_������_����, 1 : ������_����, 2 : ���ϸ�_����, 3 : ���ϸ�_������, 4 : ������)
	m_nTypeCalcSlabEffWidth		= 0;
	m_nTypeFootingSafetyApplyCoefficientImpact = 0;
	m_nApplyBoredPileASD		= 0;
	m_nApplyPhcPileASD			= 0;
	m_nTypeCheckSteelPile		= 0;
	m_nSelectPmin_Hyunta_Extreme = 0;
	m_bPrintConditionApply = TRUE;	//���������� ��¹��(TRUE: �⺻��������, FALSE: ������������)
	m_nTypeEarthQuakeApply = 0;
	// ------------------------------------------------------ ����ȯ�� 

	// ���� ����
	m_nHowLineInput = 1;			// ���� �Է� ��� 0 : ����, 1 : ���� �Է�
	m_strLineFilePath = _T("");		// ���� ���� ���

	m_bMakeDrawingOptionUsed = FALSE;

	// ö�� ����
	m_nDegreeRail	 = 0;
	m_nTypeRailLoad	 = 1;
	m_nTypeRailQty	 = 0;
	m_nRailQtyCar	 = 6;

	m_sNameRailLoad			= _T("");	 // ������ �̸�,LS-25.....
	m_DistributionLoadFront	= 0;		 // �������� ��������
	m_DistributionLoadBack	= 0;		 // �������� ��������
	m_RailLoadWheel			= 0;		 // ��������
	m_RailDistWheel			= 2.10;		 // ���
	m_RailDistWheelMid		= 0;		 // ��� (�߰��κ�)
	m_dArrTypeLoadTrainWheel.RemoveAll();// ���������� ����
	m_dArrDistTrainWheel.RemoveAll();	 // ���������� �Ÿ�, �������߰��� �Ÿ�
	m_dArrLoadTrainWheel.RemoveAll();	 // ���������� ũ��, �������߸�....

	// ��ũ �� ������ ����
	m_dTank_i				= 0.15;				
	m_dTank_WeightTank		= 54.42;
	m_dTank_TankLength		= 4.500;		
	m_dTank_TankWidth_Side	= 0.700;
	m_dTank_TankWidth_Inner	= 1.900;
	
	m_dTank_TrailerL1		= 3.6;		
	m_dTank_TrailerL2		= 1.5;		
	m_dTank_TrailerL3		= 4.5;		
	m_dTank_TrailerL4		= 1.2;
	m_dTank_TrailerWidth_Side	= 0.975;
	m_dTank_TrailerWidth_Inner	= 1.250;
	m_dTank_Ptrailer1			= 4.0815;
	m_dTank_Ptrailer2			= 8.163;
	m_dTank_Ptrailer3			= 5.8955;	

	// ������ �ʴ� ����(CRcBridgeData�� ��������� ��ü��) - �赵��20051103 
	m_nTypePost				= 0;	// ���ٸ� Ÿ��(������, �Ϲݿ�)
	m_nTypePosBaseLine		= 0;	// ���ٸ� ���ؼ�(0 : ���ݼ�, 2 : ����)
	m_dThickPostUpper		= 0;	// ������ ���ٸ� ��� �β�
	m_dThickPostSide		= 0;	// ������ ���ٸ� �¿��� �β�
	m_dThickScaffold		= 0;	// ��� �β�
	m_bFootLenInputFloat	= FALSE;

	// ������꼭 ��� �ɼ�
	m_bExcelDefaultSetting  = TRUE;
	m_szFontPrint		    = "����ü";
	m_dFontSizePrint	    = 9;
	m_dOffsetLeftPrint		= 1.9;
	m_dOffsetRightPrint		= 1.9;
	m_dOffsetTopPrint		= 2.5;
	m_dOffsetBottomPrint	= 2.5;
	m_nPageRow				= 46;

	//m_dwEnableFunction	= 0x0000;	// 0x0001 : ������ ǥ�ص� ���

	// (LSD:0000)
	m_nTypeDesignMethod = DESIGN_METHOD_STRENGTH;
	m_nLSDRoadType		= 0;
	m_dDesignRoadWidth	= 3600;

	m_plsdEnv  = new CLsdEnviromentCoef;
	m_plsdRebar = new CLsdRebarManager;
	m_pBasicConcInfo->SetLsdRebar(m_plsdRebar);

	m_pUserBmData = new CUserBmData;

	m_vStxOption.clear();
}

CARcBridgeDataStd::~CARcBridgeDataStd()
{
	AhTPADelete(&m_pRcBridgeArray, (CRcBridgeRebar*)0);
	AhTPADelete(&m_pArrBoringEx, (CBoringDataEx*)0);

	// (LSD:0000)
	SAFE_DELETE(m_pBasicConcInfo);
	SAFE_DELETE(m_pLineApp);
	SAFE_DELETE(m_pExpJointRefArr);
	SAFE_DELETE(m_plsdEnv);
	SAFE_DELETE(m_plsdRebar);
	SAFE_DELETE(m_pUserBmData);

	ClearMapPtr(m_mpBoring);
}

// ����Ÿ�� ��ȿ���� �Ǵ���
// ������ ���̰� ���� ������ �ԷµǴ°� ����ϱ� ����.
// BOOL CARcBridgeDataStd::CheckDataValidity()
// {
// 	return TRUE;
// }

// long CARcBridgeDataStd::GetTypeConditionApply(CString strType)
// {
// 	long i=0; for(i=0; i<MAX_CONDITION_TYPE; i++)
// 		if(strType==m_sConditionApply[i])
// 			return i;
// 
// 	return -1;
// }

CString CARcBridgeDataStd::GetStringConditionApply(long nType)
{
	if(nType>=0 && nType<MAX_CONDITION_TYPE)
		return m_sConditionApply[nType];
	return _T("");
}

// long CARcBridgeDataStd::GetTypeCrackDesign(CString strType)
// {
// 	long i=0; for(i=0; i<MAX_CRACK_ENV_TYPE; i++)
// 		if(strType==m_sEnvCrack[i])
// 			return i;
// 
// 	return -1;
// }

CString CARcBridgeDataStd::GetStringCrackDesign(long nType)
{
	if(nType>=0 && nType<MAX_CRACK_ENV_TYPE)
		return m_sEnvCrack[nType];
	return _T("");

}

double CARcBridgeDataStd::GetValueSettle(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	m_pBasicConcInfo->m_nTypeRebarConnect	= m_nTypeRebarConnect;
	return m_pBasicConcInfo->GetValueSettle(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

double CARcBridgeDataStd::GetValueSettleByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	return m_pBasicConcInfo->GetValueSettleByCalc(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

double CARcBridgeDataStd::GetValueJoint(double nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	m_pBasicConcInfo->m_nTypeRebarConnect	= m_nTypeRebarConnect;
	return m_pBasicConcInfo->GetValueJoint(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

// nType : 0 - �Ϲ� - A��, 1 - �Ϲ� B��, 2 - ��� A��, 3 - ��� B��, 4 - �Ϲ�, 5 - ���� ��ö��, 6 - ����ö��
double CARcBridgeDataStd::GetValueJointByCalc(long nDia, long nType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	return m_pBasicConcInfo->GetValueJointByCalc(m_Fck,m_Fy,nDia,nType,dCover,dRebarSpace,nBeta,nIp);
}

//#define JOINT_BEBD_RADIUS		0
//#define JOINT_BEBD_CHAMFER	1
double CARcBridgeDataStd::GetValueBendReBar(long nDia, int nTypeValue)
{
	if(nTypeValue==JOINT_BEBD_RADIUS)
		return m_pBasicConcInfo->GetRebarInfo(nDia, BASIC_RADIUS);
	if(nTypeValue==JOINT_BEBD_CHAMFER)
		return m_pBasicConcInfo->GetRebarInfo(nDia, BASIC_CHAMFER);

	return 0;
}

BOOL CARcBridgeDataStd::IsValidLine() const
{ 
// 	BOOL bValid = FALSE;

	BOOL bValid = m_pLineApp->IsValidLineInuptData();

	return bValid; 
}

CLineInfoApp* CARcBridgeDataStd::GetLineApp()
{
	return m_pLineApp;
}

void CARcBridgeDataStd::Serialize(CArchive &ar)
{
	// �÷���
	// 13 : m_bIsDBar_ForDowelBar;	// ������ ����
	// 14 : m_nTypeProcess;	// �������� ( 0 : �Ϲ�, 1 : ������ ǥ�ص�)
//	m_nFlag = 14;	// 0���� Ⱦ����ġ�Է�â���� �ڵ� �ǰ�� �Ǵ� ���� ������ ���ؼ� �����.
	// 4���� ���� �ݿø� �ɼ� �ε��� ����.
	// 15 : m_bApplyRigidWallEQRotate;
	// 16 : m_nPageRow;
	// 17 : m_bSepFootingForSiljuk;		// ������������� ���ʸ� �ٴڹ׺����� �и��ؼ� ����
	// 18 : m_Pi_F_Cobel
	// 19 : m_pExpJointRefArr ( BOX_TYPE );
	// 19 :m_nTypeRoadCode, m_nTypeRailCode ( BOX_TYPE );
	// 20 : m_pBasicConcInfo ��� �⺻�� ����
	// 21 : SerializeLsd() �߰�
	// 22 : m_pUserBmData ����� ���� ����
	// 23 : m_bApplyBoredPileASD
	// 24 : m_nApplyPhcPileASD
	// 25 : m_nTypeCheckSteelPile
	// 26 : ARCBRIDGE-2826
	// 27 : m_nSelectPmin_Hyunta_Extreme
	// 28 : ARCBRIDGE-2942
	// 29 : ARCBRIDGE-2969
	// 30 : m_bPrintConditionApply
	// 31 : m_nTypeFootingSafetyApplyCoefficientImpact
	// 32 : m_nTypeEarthQuakeApply, m_nTypeCoefficientGround
	// 33 : m_dRateDAMP_EQ_CD

	m_nFlag = 33;

	// �������� ����ϴ� ������
	long i(0);
	long nRcBridgeSize = m_pRcBridgeArray.GetSize();

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		// ������Ʈ ����
		ar << m_strProjectName;		// �����
		ar << m_strOwnerName;		// ����ó
		ar << m_strConstructorName;	// �ð���
		ar << m_strSpotName;		// �����
		ar << m_strEngName;			// �����
		ar << m_strLineDirName;		// ���� ����
		ar << m_strLineRevDirName;	// ���� �ݴ� ����
		ar << m_nTypeProcess;	// �������� ( 0 : �Ϲ�, 1 : ������ ǥ�ص�)

		m_pBasicConcInfo->Serialize(ar);
		m_pExpJointRefArr->Serialize(ar);

		// ����ȯ�� ------------------------------------------------------- 
		ar << m_nTypeConditionApply;	// ���뼳�����
		ar << m_nTypeKciCode;
		ar << m_nTypeRoadCode;
		ar << m_nTypeRailCode;
		ar << m_nDegreeBridge;			// ���豳���� ���
		ar << m_nTypeUnit;				// ������
		ar << m_RatePoasong;			// ��ũ��Ʈ�� ���ƽ���
		ar << m_Fck;
		ar << m_Fy;
		ar << m_bIsDBar_ForDowelBar;	// ������ ����
		ar << m_Ec;
		ar << m_Es;
		ar << m_Pi_F;
		ar << m_Pi_C;
		ar << m_Pi_V;
		ar << m_PI_B;					// ��ũ��Ʈ�� ����
		ar << m_Pi_F_Cobel;
		ar << m_Es_Steel;				// ���� ź�����
		ar << m_NValue_Steel;			// ����� ��ũ��Ʈ�� ź�������
		ar << m_Gs_Steel;				// ������ ����ź�����
		ar << m_dDeltaTemperature;
		ar << m_dCoefficientExpansion;
		ar << m_dRateDryAndShrinkage;
		ar << m_dLengthRebarMax;		// ö�� ������ ���� ����
		ar << m_nDegreeEarthQuake;		// �������
		ar << m_nRegionEarthQuake;		// ��������
		ar << m_dCoefficientGround;		// ���ݰ��
		ar << m_nTypeCoefficientGround;
		ar << m_dRateDAMP_EQ_CD;
		ar << m_gValue;					// �߷°��ӵ�
		ar << m_nTypeDesingEQ;			// �����ؼ� ���
		ar << m_nTypeRigidFootingEQ;		// �����ؼ��� ���ʺ� �𵨸� ���
		ar << m_bApplyRigidWallEQRotate;
		ar << m_nTypeJointPosFootingEQ;				// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
		ar << m_nTypePropertyPosFootingEQ;
		ar << m_nTypeUpperWeightEQ;				// �����ؼ��� ����߷� ����
		ar << m_nTypeComboEQ;			// �����ؼ������ ���� ���� ���
		ar << m_nTypeSoilPressureEQ;
		ar << m_dRateDAMP_EQ;			// �����ؼ��� �����
		ar << m_nTypeRebarConnect;		// ������ �� ������ ������
		ar << m_nTypeBMApply;			// �������� �������
		ar << m_bRoundRebarConnect_XX;	// ö�� ������, �������� �ݿø�
		ar << m_bIncludeSumZero;		// �������迡 �����(zero) ����
		ar << m_bApplyAllowInc;			// �������� ����� ������� ������� ����
		ar << m_nTypeFileNameForExcel;	// ���� ��½� ���ϸ�(0 : ���ϸ�_������_����, 1 : ������_����, 2 : ���ϸ�_����, 3 : ���ϸ�_������, 4 : ������)
		ar << m_nTypeCalcSlabEffWidth;
		ar << m_nTypeFootingSafetyApplyCoefficientImpact;
		ar << m_nApplyBoredPileASD;
		ar << m_nApplyPhcPileASD;
		ar << m_nTypeCheckSteelPile;
		ar << m_nSelectPmin_Hyunta_Extreme;
		ar << m_bPrintConditionApply;
		ar << m_nTypeEarthQuakeApply;
		// ------------------------------------------------------ ����ȯ�� 

		// ö�� ����
		ar << m_nDegreeRail;
		ar << m_nTypeRailLoad;
		ar << m_nTypeRailQty;
		ar << m_nRailQtyCar;
		ar << m_sNameRailLoad;			// ������ �̸�,LS-25.....
		ar << m_DistributionLoadFront;	// �������� ��������
		ar << m_DistributionLoadBack;		// �������� ��������
		ar << m_RailLoadWheel;			// ��������
		ar << m_RailDistWheel;			// ���
		ar << m_RailDistWheelMid;			// ��� (�߰��κ�)
		m_dArrTypeLoadTrainWheel.Serialize(ar);		// ���������� ����
		m_dArrDistTrainWheel.Serialize(ar);	// ���������� �Ÿ�, �������߰��� �Ÿ�
		m_dArrLoadTrainWheel.Serialize(ar);	// ���������� ũ��, �������߸�....
		// ��ũ �� ������ ����
		ar << m_dTank_i;				
		ar << m_dTank_WeightTank;
		ar << m_dTank_TankLength;		
		ar << m_dTank_TankWidth_Side;
		ar << m_dTank_TankWidth_Inner;
		
		ar << m_dTank_TrailerL1;		
		ar << m_dTank_TrailerL2;		
		ar << m_dTank_TrailerL3;		
		ar << m_dTank_TrailerL4;
		ar << m_dTank_TrailerWidth_Side;
		ar << m_dTank_TrailerWidth_Inner;
		ar << m_dTank_Ptrailer1;
		ar << m_dTank_Ptrailer2;
		ar << m_dTank_Ptrailer3;	

		// �������� �ɼ�
		ar << m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�)
		ar << m_nTypePosBaseLine;	// ���ٸ� ���ؼ�(0 : ���ݼ�, 2 : ����)
		ar << m_dThickPostUpper;	// ������ ���ٸ� ��� �β�
		ar << m_dThickPostSide;		// ������ ���ٸ� �¿��� �β�
		ar << m_dThickScaffold;		// ��� �β�
		ar << m_bFootLenInputFloat;

		// ���� ����
		ar << m_nHowLineInput;			// ���� �Է� ��� 0 : ����, 1 : ���� �Է�
		ar << m_strLineFilePath;		// ���� ���� ���
		m_pLineApp->Serialize(ar);

		// �� ������ ���� ����
		ar << nRcBridgeSize;
		for(i = 0; i < nRcBridgeSize; i++)
			m_pRcBridgeArray[i]->Serialize(ar);
		AhTPASerial(ar,&m_pArrBoringEx, (CBoringDataEx*)0);

		ar << m_bExcelDefaultSetting;
		ar << m_szFontPrint;
		ar << m_dFontSizePrint;
		ar << m_dOffsetLeftPrint;
		ar << m_dOffsetRightPrint;
		ar << m_dOffsetTopPrint;
		ar << m_dOffsetBottomPrint;
		ar << m_nPageRow;

		ar << m_bSepFootingForSiljuk;		// ������������� ���ʸ� �ٴڹ׺����� �и��ؼ� ����
	}
	else
	{
		ar >> m_nFlag;

		// ������Ʈ ����
		ar >> m_strProjectName;		// �����
		ar >> m_strOwnerName;		// ����ó
		ar >> m_strConstructorName;	// �ð���
		ar >> m_strSpotName;		// �����
		ar >> m_strEngName;			// �����
		ar >> m_strLineDirName;		// ���� ����
		ar >> m_strLineRevDirName;	// ���� �ݴ� ����
		if(m_nFlag > 13)
			ar >> m_nTypeProcess;	// �������� ( 0 : �Ϲ�, 1 : ������ ǥ�ص�)

		if(m_bOnlyProjectInfoLoad) return;

		m_pBasicConcInfo->Serialize(ar);
		if(m_nFlag < 20)
		{
			for(long nRow = 0; nRow < 12; nRow++)
			{
				m_pBasicConcInfo->m_RebarWeight[nRow][5] = m_pBasicConcInfo->m_RebarWeight[nRow][4];
			}
		}
		if(m_nFlag > 18)
		{
			m_pExpJointRefArr->Serialize(ar);
		}
		if(m_nFlag < 4)
		{
			switch(m_pBasicConcInfo->m_nRoundRebarConnect)
			{
			case 0: m_pBasicConcInfo->m_nRoundRebarConnect	= 2; break;
			case 1: m_pBasicConcInfo->m_nRoundRebarConnect	= 1; break;
			case 2: m_pBasicConcInfo->m_nRoundRebarConnect	= 0; break;
			}
		}

		// ����ȯ�� ------------------------------------------------------- 
		ar >> m_nTypeConditionApply;	// ���뼳�����
		if(m_nFlag>11)
		{
			ar >> m_nTypeKciCode;
		}
		if(m_nFlag > 18)
		{
			ar >> m_nTypeRoadCode;
			ar >> m_nTypeRailCode;
		}
		ar >> m_nDegreeBridge;			// ���豳���� ���
		ar >> m_nTypeUnit;				// ������
		ar >> m_RatePoasong;			// ��ũ��Ʈ�� ���ƽ���
		ar >> m_Fck;
		ar >> m_Fy;
		if(m_nFlag > 12)
			ar >> m_bIsDBar_ForDowelBar;	// ������ ����
		ar >> m_Ec;
		ar >> m_Es;
		ar >> m_Pi_F;
		ar >> m_Pi_C;
		ar >> m_Pi_V;
		ar >> m_PI_B;	
		if(m_nFlag > 17)
			ar >> m_Pi_F_Cobel;
		ar >> m_Es_Steel;				// ���� ź�����
		if(m_Es_Steel == 0)
			m_Es_Steel = 2100000;
		ar >> m_NValue_Steel;			// ����� ��ũ��Ʈ�� ź�������
		ar >> m_Gs_Steel;				// ������ ����ź�����
		ar >> m_dDeltaTemperature;
		ar >> m_dCoefficientExpansion;
		ar >> m_dRateDryAndShrinkage;
		ar >> m_dLengthRebarMax;		// ö�� ������ ���� ����
		ar >> m_nDegreeEarthQuake;		// �������
		ar >> m_nRegionEarthQuake;		// ��������
		ar >> m_dCoefficientGround;		// ���ݰ��
		if(m_nFlag > 31)
			ar >> m_nTypeCoefficientGround;
		if(m_nFlag > 32)
			ar >> m_dRateDAMP_EQ_CD;
		ar >> m_gValue;					// �߷°��ӵ�
		ar >> m_nTypeDesingEQ;			// �����ؼ� ���
		if(m_nFlag < 19)
		{
			m_nTypeDesingEQ = 1;
		}
		ar >> m_nTypeRigidFootingEQ;	// �����ؼ��� ���ʺ� �𵨸� ���
		if(m_nFlag > 14)
			ar >> m_bApplyRigidWallEQRotate;
		if(m_nFlag>1)
		{
			ar >> m_nTypeJointPosFootingEQ;	// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
			if(m_nFlag>8)
				ar >> m_nTypePropertyPosFootingEQ;
			ar >> m_nTypeUpperWeightEQ;		// �����ؼ��� ����߷� ����
		}
		ar >> m_nTypeComboEQ;			// �����ؼ������ ���� ���� ���
		if(m_nFlag>9)
			ar >> m_nTypeSoilPressureEQ;
		ar >> m_dRateDAMP_EQ;			// �����ؼ��� �����
		ar >> m_nTypeRebarConnect;		// ������ �� ������ ������
		ar >> m_nTypeBMApply;			// �������� �������
		ar >> m_bRoundRebarConnect_XX;	// ö�� ������, �������� �ݿø�
		ar >> m_bIncludeSumZero;		// �������迡 �����(zero) ����
		if(m_nFlag>5)
		{
			ar >> m_bApplyAllowInc;			// �������� ����� ������� ������� ����
		}
		if(m_nFlag > 7)
			ar >> m_nTypeFileNameForExcel;	// ���� ��½� ���ϸ�(0 : ���ϸ�_������_����, 1 : ������_����, 2 : ���ϸ�_����, 3 : ���ϸ�_������, 4 : ������)
		if(m_nFlag>10)
			ar >> m_nTypeCalcSlabEffWidth;
		if(m_nFlag > 30)
			ar >> m_nTypeFootingSafetyApplyCoefficientImpact;
		if(m_nFlag > 22)
			ar >> m_nApplyBoredPileASD;
		if(m_nFlag > 23)
			ar >> m_nApplyPhcPileASD;
		if(m_nFlag > 24)
			ar >> m_nTypeCheckSteelPile;
		if(m_nFlag > 26)
			ar >> m_nSelectPmin_Hyunta_Extreme;
		if(m_nFlag > 29)
			ar >> m_bPrintConditionApply;
		else
			m_bPrintConditionApply = FALSE;

		if(m_nFlag > 31)
			ar >> m_nTypeEarthQuakeApply;

		// ------------------------------------------------------ ����ȯ�� 

		// ö�� ����
		ar >> m_nDegreeRail;
		ar >> m_nTypeRailLoad;
		ar >> m_nTypeRailQty;
		ar >> m_nRailQtyCar;
		ar >> m_sNameRailLoad;				// ������ �̸�,LS-25.....
		ar >> m_DistributionLoadFront;		// �������� ��������
		ar >> m_DistributionLoadBack;		// �������� ��������
		ar >> m_RailLoadWheel;				// ��������
		ar >> m_RailDistWheel;				// ���
		ar >> m_RailDistWheelMid;			// ��� (�߰��κ�)
		m_dArrTypeLoadTrainWheel.Serialize(ar);		// ���������� ����
		m_dArrDistTrainWheel.Serialize(ar);	// ���������� �Ÿ�, �������߰��� �Ÿ�
		m_dArrLoadTrainWheel.Serialize(ar);	// ���������� ũ��, �������߸�....

		if(m_nFlag > 4)
		{
			// ��ũ �� ������ ����
			ar >> m_dTank_i;				
			ar >> m_dTank_WeightTank;
			ar >> m_dTank_TankLength;		
			ar >> m_dTank_TankWidth_Side;
			ar >> m_dTank_TankWidth_Inner;
			
			ar >> m_dTank_TrailerL1;		
			ar >> m_dTank_TrailerL2;		
			ar >> m_dTank_TrailerL3;		
			ar >> m_dTank_TrailerL4;
			ar >> m_dTank_TrailerWidth_Side;
			ar >> m_dTank_TrailerWidth_Inner;
			ar >> m_dTank_Ptrailer1;
			ar >> m_dTank_Ptrailer2;
			ar >> m_dTank_Ptrailer3;	
		}
		else
		{
			// ��ũ �� ������ ����
			m_dTank_i				= 0.15;				
			m_dTank_WeightTank		= 58.0;
			m_dTank_TankLength		= 4.5;
			m_dTank_TankWidth_Side	= 0.7;
			m_dTank_TankWidth_Inner	= 1.9;
			
			m_dTank_TrailerL1		= 3.6;
			m_dTank_TrailerL2		= 1.5;		
			m_dTank_TrailerL3		= 4.5;		
			m_dTank_TrailerL4		= 1.2;
			m_dTank_TrailerWidth_Side	= 0.975;
			m_dTank_TrailerWidth_Inner	= 1.25;
			m_dTank_Ptrailer1		= 4.5;
			m_dTank_Ptrailer2		= 9.0;
			m_dTank_Ptrailer3		= 6.5;	
		}

		// �������� �ɼ�
		ar >> m_nTypePost;			// ���ٸ� Ÿ��(������, �Ϲݿ�)
		ar >> m_nTypePosBaseLine;	// ���ٸ� ���ؼ�(0 : ���ݼ�, 2 : ����)
		ar >> m_dThickPostUpper;	// ������ ���ٸ� ��� �β�
		ar >> m_dThickPostSide;		// ������ ���ٸ� �¿��� �β�
		ar >> m_dThickScaffold;		// ��� �β�
		if(m_nFlag > 6) ar >> m_bFootLenInputFloat;

		// ���� ����
		ar >> m_nHowLineInput;			// ���� �Է� ��� 0 : ����, 1 : ���� �Է�
		ar >> m_strLineFilePath;		// ���� ���� ���
		m_pLineApp->Serialize(ar);
		CString strErrorMsg = _T("���� �����Ͱ� �ùٸ��� �ʽ��ϴ�.  ");
		m_pLineApp->SetDataIp(strErrorMsg);

		// �� ���� �ҷ�����
		ar >> nRcBridgeSize;
		MakeRcBridge(nRcBridgeSize);
		for(i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeRebar *pBri	= m_pRcBridgeArray[i];
			pBri->Serialize(ar);
		}
		AhTPASerial(ar,&m_pArrBoringEx, (CBoringDataEx*)0);
		ar >> m_bExcelDefaultSetting;
		ar >> m_szFontPrint;
		ar >> m_dFontSizePrint;
		ar >> m_dOffsetLeftPrint;
		ar >> m_dOffsetRightPrint;
		ar >> m_dOffsetTopPrint;
		ar >> m_dOffsetBottomPrint;
		if(m_nFlag > 15)
			ar >> m_nPageRow;
		
		if(m_nFlag > 16)
			ar >> m_bSepFootingForSiljuk;		// ������������� ���ʸ� �ٴڹ׺����� �и��ؼ� ����
	}

	if(ar.IsStoring() || m_nFlag > 20)
		SerializeLsd(ar);

	if(ar.IsStoring() || m_nFlag > 21)
		m_pUserBmData->Serialize(ar);

	if(ar.IsLoading())
	{
		SetStringUnitType();

		// ������ Sync�����ִ� �κ��� Serialize�� ��� ������ �� �ڷ� �̵��ߴ�.
		nRcBridgeSize = m_pRcBridgeArray.GetSize();
		for(i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeRebar *pBri	= m_pRcBridgeArray[i];

			if(pBri->m_nIdxBridge==-1)
			{
				pBri->m_nIdxBridge = i;
			}
			pBri->m_bIsIncludeOnlyLine	= pBri->IsIncludeOnlyLineInBridge();

			BOOL bVertDir	= pBri->IsVertDir();
			long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
			pBri->m_bSettingVertSection		= FALSE;
			pBri->m_nTypeJongdanForRebar	= 2;
			pBri->SyncBridge(0, !bVertDir);						// ������ �о� �� �ڿ��� �� ��ü�� ����ȭ�� �ѹ� �����ش�.
			pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;

			if(pBri->m_bNeedSettingVertSection || !pBri->m_bSettingVertSection || pBri->m_nFlag < 44)
			{
				pBri->SettingVertSection();							// �ӵ������� ���� �۾�
				pBri->m_bNeedSettingVertSection	= FALSE;
			}
			if(pBri->m_bNeedSettingPlane || !pBri->m_bSettingPlane || pBri->m_nFlag < 44)
			{
				pBri->SettingPlane();
				pBri->m_bNeedSettingPlane	= FALSE;
			}
			pBri->SyncBridgeRebar(FALSE, 0, TRUE);	// ��ٰ��õ� ����Ÿ�� ����ȭ ����.

			if(ar.IsLoading() && pBri->m_nFlag < 12)
			{
				pBri->SetDataDefaultRebarShear_VertSection_Bracket();
			}

			if(pBri->m_bAutoWingHeightSoil)
			{
				pBri->SetDataDefaultWingWallHeightSoil();
			}

			// �ڵ� �ǰ�� ������.
			if(m_nFlag < 1)
			{
				pBri->SetDefaultedState(7092, 0x00000001);	// Ⱦ����ġ �Է� ����.(7092)
			}
		}
	}

	if(m_nFlag < 26 && ar.IsLoading() && IsLsdDesign())
	{
		SetFooting_Qa_Pa__Calculate();
	}

	if(m_nFlag < 28)
	{
		nRcBridgeSize = m_pRcBridgeArray.GetSize();
		for(long i = 0; i < nRcBridgeSize; i++)
		{
			CRcBridgeApp *pBri = GetBridge(i);

			for(long nJ=0; nJ<pBri->m_nQtyJigan+1; nJ++)
			{
				if(pBri->IsPileBase(nJ))
				{
					CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJ);
					CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
					if(pPile->m_nType == PILE_TYPE_HYUNJANG)
					{
						CBoringDataEx *pBoringEx = pBri->GetBoringDataEx(nJ);
						if(pBoringEx)
						{
							double dElTop = pFooting->m_dEL - pFooting->GetHeight();
							CBoringData *pBoring = pBoringEx->GetBoringByElevation(toM(dElTop));

							pFooting->SetHyunTaPileByBoring(pBoring);
						}
					}
				}
			}
		}
	}

	if(m_nFlag < 29 && ar.IsLoading() && IsLsdDesign())
	{
		long nSizeBorEx = m_pArrBoringEx.GetSize();
		for(long n=0; n<nSizeBorEx; n++)
		{
			CBoringDataEx *pBorEx = m_pArrBoringEx.GetAt(n);
			long nSizeBor = pBorEx->m_pArrBoring.GetSize();
			for(long nB=0; nB<nSizeBor; nB++)
			{
				CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(nB);
				if(pBor->GetBoringType() > TOSA)
				{
					pBor->m_nTypeSoil = 2; // �Ϲ�
				}
			}
		}
	}
}

CString CARcBridgeDataStd::GetStringDia(double dDia, double dFy)
{
	if(dFy == 0)
		dFy = m_Fy;

	CString sFy = GetCharRebarMark(dFy);
	if(dDia==0) return _T("");
	
	return sFy+Comma(dDia);
}

// �׸��� ����Ÿ���� ������ �о��.
double CARcBridgeDataStd::GetDia(CString str)
{
	str.Replace("D", "");
	str.Replace("H", "");
	str.Replace("S", "");
	str.Replace("U", "");
	str.Replace("d", "");
	str.Replace("h", "");
	str.Replace("s", "");
	str.Replace("u", "");

	return atof(str);
}

// bBridgeCount = TRUE -> ��ü���� �ƴ� ������ ������ ����
long CARcBridgeDataStd::GetBridgeSize(BOOL bBridgeCount)
{
	if(!bBridgeCount)
		return m_pRcBridgeArray.GetSize();

	long nQtyBridge = 0;
	long nQtyInput = m_pRcBridgeArray.GetSize();
	long n=0; for(n=0 ;n<nQtyInput; n++)
	{
		CRcBridgeApp *pBri = m_pRcBridgeArray.GetAt(n);
		if(pBri->m_nSeparBri!=2) nQtyBridge++;
	}

	return nQtyBridge;
}

CRcBridgeApp* CARcBridgeDataStd::GetBridge(long nBridge)
{
	if(nBridge < 0 || nBridge >= m_pRcBridgeArray.GetSize()) 
		return NULL;

	return dynamic_cast<CRcBridgeApp *>(m_pRcBridgeArray[nBridge]);
}

CRcBridgeRebar* CARcBridgeDataStd::GetBridgeRebar(long nBridge)
{
	if(nBridge < 0 || nBridge >= m_pRcBridgeArray.GetSize()) 
		return NULL;

	return m_pRcBridgeArray[nBridge];
}

BOOL CARcBridgeDataStd::MakeRcBridge(long nTotal)
{
	long nCount	= m_pRcBridgeArray.GetSize();
	AhTPAMake(nTotal, &m_pRcBridgeArray, (CRcBridgeRebar*)0);
	long i = 0; for(i = 0; i < m_pRcBridgeArray.GetSize(); i++)
	{
		CRcBridgeRebar *pBri = m_pRcBridgeArray.GetAt(i);
		pBri->m_pARcBridgeDataStd = this;
		pBri->m_Util.m_pARcBridgeDataStd	= this;
		if(i >= nCount)
			pBri->SetDataDefaultBridgeType();

		if(!pBri->m_bUnderGroundRahmen)
		{
			m_pLineApp->ReSetSearchIp();
			*pBri->m_pLineApp = *m_pLineApp;
			CString strErrorMsg = _T("���� �����Ͱ� �ùٸ��� �ʽ��ϴ�.  ");
			pBri->m_pLineApp->SetDataIp(strErrorMsg);
		}
	}

	return TRUE;
}

void CARcBridgeDataStd::MakeSoilLayer(int nBoring, int nCountLayer)
{
	if(nBoring<0) return;

	CBoringDataEx *pBoring = m_pArrBoringEx.GetAt(nBoring);

	AhTPAMake(nCountLayer, &pBoring->m_pArrBoring, (CBoringData*)0);
}

// nPart : ���� 
CString CARcBridgeDataStd::GetStringBM_FormType(long nPart)
{
	if(m_nTypeBMApply == BMTYPE_NORMAL)	// �Ϲݼ���
	{
		if(nPart >= 0 && nPart < MAX_BM_FORM_TYPE_NORMAL)
		return m_sBM_FormTypeNormal[nPart];
	}
	else								// ��������
	{
		if(nPart >= 0 && nPart < MAX_BM_FORM_TYPE)
		return m_sBM_FormType[nPart];
	}

	return _T("");
}

// �� �Լ��� CHgBaseBM���� ������ �Լ���
// CHgBaseBM�� �������� �°ݵǴ³� �� �Լ��� ���ŵǰ� CHgBaseBM���� ���� ������ ���� �� ���̴�.
CString CARcBridgeDataStd::GetStringCircleNum(long nNum)
{
	const long nCount = 41;
	CString sCircleText[nCount] = 
	{
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
		"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", 
		"��"
	};

	if(nNum <= 0 || nNum >= nCount) return "";
	return sCircleText[nNum-1];
}

void CARcBridgeDataStd::SerializeArrayOfDoubleArray(CArchive &ar, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArr)
{
	if(!pdArr) return;
	long nSizeRoot	= pdArr->GetSize();

	if(ar.IsStoring())
	{
		ar << nSizeRoot;
	}
	else
	{
		ar >> nSizeRoot;
		AhTPAMake(nSizeRoot, pdArr, (CDoubleArray*)0);
	}

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CDoubleArray *dArr;
		dArr	= pdArr->GetAt(i);
		SerializeDoubleArray(ar, dArr);
	}
}

void CARcBridgeDataStd::SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr)
{
	long nSize	= 0;
	
	if(ar.IsStoring())
	{
		nSize = dArr->GetSize();
		ar << nSize;
		long i = 0; for(i = 0; i < nSize; i++)
		{
			ar << dArr->GetAt(i);
		}
	}
	else
	{
		double d	= 0;
		ar >> nSize;
		dArr->RemoveAll();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			ar >> d;
			dArr->Add(d);
		}
	}
}

// �����̿� �ִ� ���� CDPoint(����, ��)���� ��ȯ
void CARcBridgeDataStd::GetPointArrayCountAndValue(CDPointArray& xyArr, CDoubleArray& dArr)
{
	xyArr.RemoveAll();
	
	int nSize = dArr.GetSize();
	int nCount = 1;
// 	double d1 = 0, d2 = 0;
	int i=0; for(i=0; i<nSize; i++)
	{
		double d1 = dArr.GetAt(i);
		if(i==nSize-1)
		{
			xyArr.Add(CDPoint(nCount, d1));
			break;
		}
		double d2 = dArr.GetAt(i+1);

		if(d1==d2)
		{
			nCount++;
			continue;
		}
		else
		{
			xyArr.Add(CDPoint(nCount, d1));
			nCount = 1;
		}
	}
}

CString CARcBridgeDataStd::GetStringBridgeOrder(int nBri)
{
	CString str = _T("");

// 	int nSeparBri = 0;
	int nOrder = 0;
	int nSize = GetBridgeSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		int nSeparBri = GetBridge(i)->m_nSeparBri;
		if(nSeparBri<=1) nOrder++;
		if(i==nBri)
		{
			str.Format(_T("���� %d"), nOrder);

			// ���� �������� ǥ�� �� ��� ����ڰ� ���� �Ǵܽ� �򰥸� �� �־ �ܼ� �ε����θ� ǥ����.
			if(nSeparBri==1)
				str += _T("-1");//_T("(����)");
			if(nSeparBri==2)
				str += _T("-2");//_T("(����)");
			break;
		}
	}

	return str;
}

// nBri�� ¦���Ǵ� bri�� �ε��� ����
// nBri�� ���༱�̸� ���༱, �����̸� ���༱�� �ε����� �����ϰ�, ������ �и��� �ƴ� ���
// -1�� ����
long CARcBridgeDataStd::GetIdxMatchBridge(long nBri)
{
	long nIdx	= -1;
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(pBri)
	{
		if(pBri->m_nSeparBri == 1)	// �����̸� ���� ����
			nIdx	= nBri + 1;
		else if(pBri->m_nSeparBri == 2) // �����̸� ���� ����
			nIdx	= nBri - 1;
	}

	if(nIdx >= GetBridgeSize()) nIdx = -1;

	return nIdx;
}

CRcBridgeApp* CARcBridgeDataStd::GetBridgeMatch(CRcBridgeApp* pBri)
{
	CRcBridgeApp *pBriMatch = NULL;

	if(pBri->m_nSeparBri!=0)
	{
		// �����̸� ���� ����
		if(pBri->m_nSeparBri == 1)	
			pBriMatch = GetBridge(pBri->m_nIdxBridge+1);
		// �����̸� ���� ����
		else if(pBri->m_nSeparBri == 2) 
			pBriMatch = GetBridge(pBri->m_nIdxBridge-1);
	}

	return pBriMatch;
}

CRcBridgeRebar* CARcBridgeDataStd::GetBridgeMatchRebar(CRcBridgeRebar* pBri)
{
	CRcBridgeRebar *pBriMatch = NULL;

	if(pBri->m_nSeparBri!=0)
	{
		// �����̸� ���� ����
		if(pBri->m_nSeparBri == 1)	
			pBriMatch = GetBridgeRebar(pBri->m_nIdxBridge+1);
		// �����̸� ���� ����
		else if(pBri->m_nSeparBri == 2) 
			pBriMatch = GetBridgeRebar(pBri->m_nIdxBridge-1);
	}

	return pBriMatch;
}

void CARcBridgeDataStd::CopyToCoupleBridge_SpanConst(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri<0) return;

	CRcBridgeApp *pBriSou = GetBridge(nBri);
	CRcBridgeApp *pBriTar = GetBridge(nIdxCoupleBri);
	if(pBriSou && pBriTar) 
	{
		CJijum* pJSou = NULL;
//		CJijum* pJTar = NULL;

		int nCountJijumSou = pBriSou->GetCountJijum();
		int nCountJijumTar = pBriTar->GetCountJijum();
		if(nCountJijumSou==nCountJijumTar)
		{
			int i=0; for(i=0; i<nCountJijumSou; i++)
			{
				pJSou = pBriSou->GetJijum(i);
				CJijum* pJTar = pBriTar->GetJijum(i);

				pBriTar->m_bExpJoint[i] = pBriSou->m_bExpJoint[i];
				pJTar->m_strJijumShoe[0] = pJSou->m_strJijumShoe[0];
				pJTar->m_strJijumShoe[1] = pJSou->m_strJijumShoe[1];
				pJTar->m_vAngle = pJSou->m_vAngle;

				pJTar->m_dLengthUgan[0] = pJSou->m_dLengthUgan[0];
				pJTar->m_dLengthUgan[1] = pJSou->m_dLengthUgan[1];
				pJTar->m_dLengthSlabToShoe[0] = pJSou->m_dLengthSlabToShoe[0];
				pJTar->m_dLengthSlabToShoe[1] = pJSou->m_dLengthSlabToShoe[1];
				pBriTar->m_dLengthJigan[i] = pBriSou->m_dLengthJigan[i];
			}
			
			pBriTar->m_arrStrJijumLen.RemoveAll();
			pBriTar->m_arrJijumLen.RemoveAll();
			int nSize = pBriSou->m_arrStrJijumLen.GetSize();
			for(i=0; i<nSize; i++)
			{
				pBriTar->m_arrStrJijumLen.Add(pBriSou->m_arrStrJijumLen.GetAt(i));
				pBriTar->m_arrJijumLen.Add(pBriSou->m_arrJijumLen.GetAt(i));
			}

			pBriSou->SyncBridge();
			pBriTar->SyncBridge();
		}
	}
}

// ���ܸ� ������ ���ؼ� ¦��bri���� ����Ÿ�� �����Ѵ�.
// ���ʸ� ������ ��� ����Ÿ�� �����Ѵ�.
// �Է¹޴� �����Ϳ� ���ؼ��� ���縦 �ϸ� ��.
void CARcBridgeDataStd::CopyToCoupleBridge_VertSection(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeApp *pBriSource	= GetBridge(nBri);
	CRcBridgeApp *pBriTarget	= GetBridge(nIdxCoupleBri);
	if(pBriSource && pBriTarget) 
	{
		pBriTarget->m_nTypeJongdanForRebar	= pBriSource->m_nTypeJongdanForRebar;		// ���ܸ� Ÿ�� : 0 : ����, 1 : ���(��ٽø� �����), 2 : �Ϲݵ��� ���ؼ� ���ܼ������� �׸�
		pBriTarget->m_nTypeOutWall	= pBriSource->m_nTypeOutWall;		// ������ü ����(�¿�)

		// ��� ������/��ġ
		pBriTarget->m_nCountInWallHunch	= pBriSource->m_nCountInWallHunch;		// ������ü ��ġ ����
		for(long upper=0; upper<2; upper++)
		{
			for(long hunch = 0; hunch < MAX_INWALL_HUNCH; hunch++)
			{
				pBriTarget->m_dHHL[hunch][upper]	= pBriSource->m_dHHL[hunch][upper];		// �� ��ü ��ġ ����(��)
				pBriTarget->m_dHHR[hunch][upper]	= pBriSource->m_dHHR[hunch][upper];		// �� ��ü ��ġ ����(��)
				pBriTarget->m_dHBH[hunch][upper]	= pBriSource->m_dHBH[hunch][upper];	// �� ��ü ��ġ�Ʒ� ���� ����
				pBriTarget->m_dHLW[hunch][upper]	= pBriSource->m_dHLW[hunch][upper];	// �� ��ü ��ġ �ʺ�(����)
				pBriTarget->m_dHRW[hunch][upper]	= pBriSource->m_dHRW[hunch][upper];	// �� ��ü ��ġ �ʺ�(������)
				pBriTarget->m_dHLR[hunch][upper]	= pBriSource->m_dHLR[hunch][upper];	// �� ��ü ��ġ ���� R
				pBriTarget->m_dHRR[hunch][upper]	= pBriSource->m_dHRR[hunch][upper];	// �� ��ü ��ġ ������ R
				pBriTarget->m_dWL[hunch][upper]		= pBriSource->m_dWL[hunch][upper];		// �� ��ü ��ġ ������ġ(����)
				pBriTarget->m_dWR[hunch][upper]		= pBriSource->m_dWR[hunch][upper];		// �� ��ü ��ġ ������ġ(������)
			}
		}
		
		// ������ ��ü
		pBriTarget->m_dFHS	= pBriSource->m_dFHS;
		pBriTarget->m_dFLS	= pBriSource->m_dFLS;
		pBriTarget->m_dFRS	= pBriSource->m_dFRS;
		pBriTarget->m_dWS	= pBriSource->m_dWS;
		pBriTarget->m_dBHWS	= pBriSource->m_dBHWS;
		pBriTarget->m_dBHHS	= pBriSource->m_dBHHS;
		pBriTarget->m_dBHS	= pBriSource->m_dBHS;
		pBriTarget->m_dBTHS	= pBriSource->m_dBTHS;
		pBriTarget->m_dBWS	= pBriSource->m_dBWS;
		pBriTarget->m_dHWS	= pBriSource->m_dHWS;
		pBriTarget->m_dHHS	= pBriSource->m_dHHS;
		pBriTarget->m_dWSHW	= pBriSource->m_dWSHW;
		pBriTarget->m_dWSHH	= pBriSource->m_dWSHH;
		pBriTarget->m_dHRS	= pBriSource->m_dHRS;
		//pBriTarget->m_dELSB	= pBriSource->m_dELSB;

		// ������ ��ü
		pBriTarget->m_dFHE	= pBriSource->m_dFHE;
		pBriTarget->m_dFLE	= pBriSource->m_dFLE;
		pBriTarget->m_dFRE	= pBriSource->m_dFRE;
		pBriTarget->m_dWE	= pBriSource->m_dWE;
		pBriTarget->m_dBHWE	= pBriSource->m_dBHWE;
		pBriTarget->m_dBHHE	= pBriSource->m_dBHHE;
		pBriTarget->m_dBHE	= pBriSource->m_dBHE;		
		pBriTarget->m_dBTHE	= pBriSource->m_dBTHE;
		pBriTarget->m_dBWE	= pBriSource->m_dBWE;
		pBriTarget->m_dHWE	= pBriSource->m_dHWE;
		pBriTarget->m_dHHE	= pBriSource->m_dHHE;
		pBriTarget->m_dWEHW	= pBriSource->m_dWEHW;
		pBriTarget->m_dWEHH	= pBriSource->m_dWEHH;
		pBriTarget->m_dHRE	= pBriSource->m_dHRE;
		//pBriTarget->m_dELEB	= pBriSource->m_dELEB;

		// ������ü
		long nCountInWall1	= pBriSource->GetCountInWall();
		long nCountInWall2	= pBriTarget->GetCountInWall();
		if(nCountInWall1 == nCountInWall2)
		{
			for(long wall = 0; wall < nCountInWall1; wall++)
			{
				CWallApp *pWallSource	= pBriSource->GetInWall(wall);
				CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
				if(pWallSource && pWallTarget)
				{
					pWallTarget->m_bTopIsColumn	= pWallSource->m_bTopIsColumn;
					pWallTarget->m_nType	= pWallSource->m_nType;				// ��ü ����(0 : ����, 1 : ���� �ִ°�, 2 : v����);
						// �Ϲ�����
					//pWallTarget->m_dELB		= pWallSource->m_dELB;			// ����ϴ� EL
					pWallTarget->m_dW		= pWallSource->m_dW;			// ��ü �ʺ�
					pWallTarget->m_dH		= pWallSource->m_dH;			// ��ü ����(����)
					pWallTarget->m_dAngle	= pWallSource->m_dAngle;		// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
					pWallTarget->m_bApplyAngleEnd	= pWallSource->m_bApplyAngleEnd;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
						
						// V����
					pWallTarget->m_dELRT	= pWallSource->m_dELRT;	// V�� ������ Top
					pWallTarget->m_dELLT	= pWallSource->m_dELLT;	// V�� ������ Top
					pWallTarget->m_dWL		= pWallSource->m_dWL;
					pWallTarget->m_dWR		= pWallSource->m_dWR;
					pWallTarget->m_dLR		= pWallSource->m_dLR;
					pWallTarget->m_dAngleV	= pWallSource->m_dAngleV;		// V������ ���̰�
					pWallTarget->m_dR1		= pWallSource->m_dR1;			// V�������� ���� �� ������(������� ���߿�)
					pWallTarget->m_dR2		= pWallSource->m_dR2;			// V�������� ������ �� ������(������� ���߿�)
				}
			}
		}
	}
}

// ��ö�� ��ġ�� ���� ¦��bri���� ����Ÿ�� �����Ѵ�.
// ���ʸ� ������ ��� ����Ÿ�� �����Ѵ�.
void CARcBridgeDataStd::CopyToCoupleBridge_RebarMain(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeRebar *pBriSource	= GetBridgeRebar(nBri);
	CRcBridgeRebar *pBriTarget	= GetBridgeRebar(nIdxCoupleBri);
	if(pBriSource == NULL || pBriTarget == NULL) return;
	
	if(!pBriSource->m_bApplySameMainRebar) return;

	long upper(0),  stt(0), left(0), inner(0),  dan(0);

	// ��ö�� ��ġ ///////////////////////////////////////////////////////////////////////////////////////////////////
	// ö�� ��ġ �ɼ� ����
	pBriTarget->m_bRoundAtEdgeRebar	= pBriSource->m_bRoundAtEdgeRebar;		// ��ġ�� ö���� ���� ��������?

	// ö�� ��ġ ���� index
	pBriTarget->m_nIdxCTC_MainRebar	= pBriSource->m_nIdxCTC_MainRebar;

	// ����, ���౸���� ���ϰ������� ��ġ
	pBriTarget->m_bSameOffsetPlaceTAndCBlock	= pBriSource->m_bSameOffsetPlaceTAndCBlock;	
		
	// ���� ����
	for(long jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBriTarget->m_nCountJointMainRebar_UpperSlab_Cycle1[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_UpperSlab_Cycle1[jijum][upper][dan];	// ��ν����� ��,�ϸ� �� ������ ���� ����
				pBriTarget->m_nCountJointMainRebar_UpperSlab_Cycle2[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_UpperSlab_Cycle2[jijum][upper][dan];	// ��ν����� ��,�ϸ� �� ������ ���� ����
				pBriTarget->m_nCountJointMainRebar_LowerSlab_Cycle1[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_LowerSlab_Cycle1[jijum][upper][dan];	// ��ν����� ��,�ϸ� �� ������ ���� ����
				pBriTarget->m_nCountJointMainRebar_LowerSlab_Cycle2[jijum][upper][dan]	= pBriSource->m_nCountJointMainRebar_LowerSlab_Cycle2[jijum][upper][dan];	// ��ν����� ��,�ϸ� �� ������ ���� ����
			}
		}
	}
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBriTarget->m_nCountJointMainRebar_OutWall_Cycle1[stt][left][dan]	= pBriSource->m_nCountJointMainRebar_OutWall_Cycle1[stt][left][dan];	// ������ü ������,�¿��� ���� ����
				pBriTarget->m_nCountJointMainRebar_OutWall_Cycle2[stt][left][dan]	= pBriSource->m_nCountJointMainRebar_OutWall_Cycle2[stt][left][dan];	// ������ü ������,�¿��� ���� ����
			}
		}
	}

	// ������ ���(�Լ��� ����)
	// ******************* ���⼭���� �Ʒ� ǥ�ü� ������ �ݵ�� SerializeRebar�Լ����� ���Ͽ� ������. ***************************
	// CDoubleArray�� CDoubleArray*�� �;� �ȴ�.
	// ���� ����� ������ ������� ������ ���� ȥ���� �����ϱ� ���� �ϳ��� �Լ����� �ϰ������� ó���ϱ� ������..
		
	// CYCLE1~4 ///////////////////////////////////////////////////////////////////
	// rebar info cycle ����
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		// ���� ����
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDistUpperSlab[cycle][upper][dan], &pBriSource->m_pArrDistUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDistLowerSlab[cycle][upper][dan], &pBriSource->m_pArrDistLowerSlab[cycle][upper][dan]);
			}
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					CopyDoubleArray(&pBriTarget->m_pArrDistOutWall[cycle][stt][left][dan], &pBriSource->m_pArrDistOutWall[cycle][stt][left][dan]);
				}
			}
		}

		// ö������
		for(upper = 0; upper < 2; upper++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDiaUpperSlab[cycle][upper][dan], &pBriSource->m_pArrDiaUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrDiaLowerSlab[cycle][upper][dan], &pBriSource->m_pArrDiaLowerSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrExistUpperSlab[cycle][upper][dan], &pBriSource->m_pArrExistUpperSlab[cycle][upper][dan]);
				CopyArrayOfDoubleArray(&pBriTarget->m_pArrExistLowerSlab[cycle][upper][dan], &pBriSource->m_pArrExistLowerSlab[cycle][upper][dan]);
			}
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					CopyDoubleArray(&pBriTarget->m_pArrDiaOutWall[cycle][stt][left][dan], &pBriSource->m_pArrDiaOutWall[cycle][stt][left][dan]);
					CopyDoubleArray(&pBriTarget->m_pArrExistOutWall[cycle][stt][left][dan], &pBriSource->m_pArrExistOutWall[cycle][stt][left][dan]);
				}
			}
		}

		CopyDoubleArray(&pBriTarget->m_dArrDiaUpperSlabHunch[cycle], &pBriSource->m_dArrDiaUpperSlabHunch[cycle]);
	}
	/////////////////////////////////////////////////////////////////// CYCLE1~4 //

	// ö�ٿ���� ������ ����
	for(upper = 0; upper < 2; upper++)
	{
		CopyArrayOfSetRebarJoint(&pBriTarget->m_pArrSetRebarJointUpperSlab[upper], &pBriSource->m_pArrSetRebarJointUpperSlab[upper]);
		CopyArrayOfSetRebarJoint(&pBriTarget->m_pArrSetRebarJointLowerSlab[upper], &pBriSource->m_pArrSetRebarJointLowerSlab[upper]);
	}
	for(stt = 0; stt < 2; stt++)
	{
		pBriTarget->m_pArrSetRebarJointOutWall_Outter[stt]	= pBriSource->m_pArrSetRebarJointOutWall_Outter[stt];
		for(upper = 0; upper < 2; upper++)
			pBriTarget->m_pArrSetRebarJointOutWall_Inner[stt][upper]	= pBriSource->m_pArrSetRebarJointOutWall_Inner[stt][upper];	// ������, ����, ���Ʒ� ������ü  2����Ŭ ö�� ���� �� ��������
	}

	// ö�ٹ�ġ �ɼ�
	for(inner = 0; inner < 2; inner++)
	{
		pBriTarget->m_bPlacingOutWallInnerRebar[inner]	= pBriSource->m_bPlacingOutWallInnerRebar[inner];	// �� ����������ü ����ö�� ��ġ
	}

	pBriTarget->m_bPlacingInWallRebarOfCycle24	= pBriSource->m_bPlacingInWallRebarOfCycle24;		// �߰���ü ö�� ��ġ
	pBriTarget->m_bBendingDanBuRebar			= pBriSource->m_bBendingDanBuRebar;					// �� ������ ö�� ���η��� ��ġ
	pBriTarget->m_bPlacingStirrupAtDanBuRebar	= pBriSource->m_bPlacingStirrupAtDanBuRebar;			// �� ������ ROUND�ο� ��Ʈ�� ��ġ
	pBriTarget->m_bPlacingSupportAtHunchRebar	= pBriSource->m_bPlacingSupportAtHunchRebar;		// ��ġö������ ���ö�� ��ġ
	pBriTarget->m_bPlacingSupportAtDanBuRebar	= pBriSource->m_bPlacingSupportAtDanBuRebar;		// �� ������ ROUND�ο� ���ö�� ��ġ
	pBriTarget->m_bPlacing23DanByStandardOffset	= pBriSource->m_bPlacing23DanByStandardOffset;		// 2,3�� ö���� ǥ�ع�ġ�������� ��ġ
	pBriTarget->m_nCountWrapMainRebarUpperSlab			= pBriSource->m_nCountWrapMainRebarUpperSlab;				// ��ö���� ���δ� ����
	pBriTarget->m_nCountWrapMainRebarLowerSlab			= pBriSource->m_nCountWrapMainRebarLowerSlab;
		
	// ö�� �ܼ� ����
	for(upper = 0; upper < 2; upper++)
	{
		CopyArrayOfSetRebarDan(&pBriTarget->m_pArrSetRebarDanUpperSlab[upper], &pBriSource->m_pArrSetRebarDanUpperSlab[upper]);
		CopyArrayOfSetRebarDan(&pBriTarget->m_pArrSetRebarDanLowerSlab[upper], &pBriSource->m_pArrSetRebarDanLowerSlab[upper]);
	}

	for(stt = 0; stt < 2; stt++)
	{
		for(inner = 0; inner < 2; inner++)
		{
			pBriTarget->m_pArrSetRebarDanOutWall[stt][inner]	= pBriSource->m_pArrSetRebarDanOutWall[stt][inner];
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////// ��ö�� ��ġ //

	// ������ü�� ���ؼ�...
	// ������ü
	long nCountInWall1	= pBriSource->GetCountInWall();
	long nCountInWall2	= pBriTarget->GetCountInWall();
	if(nCountInWall1 == nCountInWall2)
	{
		for(long wall = 0; wall < nCountInWall1; wall++)
		{
			CWallApp *pWallSource	= pBriSource->GetInWall(wall);
			CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
			if(pWallSource && pWallTarget)
			{
				// ��� ----------------------------------------------------------
				pWallTarget->m_nIdxCTC_MainRebar			= pWallSource->m_nIdxCTC_MainRebar;	// ��ö�� CTC
				pWallTarget->m_bSameOffsetPlaceTAndCBlock	= pWallSource->m_bSameOffsetPlaceTAndCBlock;	// ������౸���� ���� �������� ��ġ		

				// ��ġ�ܼ� ����/ö������ ����
				for(left = 0; left < 2; left++)
					pWallTarget->m_setRebarDan[left]		= pWallSource->m_setRebarDan[left];	// ��ü �¿��� �ܼ� ����
											
				pWallTarget->m_nIdxCountCycleMainRebar		= pWallSource->m_nIdxCountCycleMainRebar;	// ��ö�� ����Ŭ ��.
				pWallTarget->m_nTypeCurveOfVertRebar		= pWallSource->m_nTypeCurveOfVertRebar;
				//---------------------------------------------------------- ��� 
			}
		}
	}
}

// doublearray���� ����.
void CARcBridgeDataStd::CopyDoubleArray(CDoubleArray *dArrTarget, CDoubleArray *dArrSource)
{
	if(!dArrTarget || !dArrSource) return;

	dArrTarget->RemoveAll();

	long nSize	= dArrSource->GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
		dArrTarget->Add(dArrSource->GetAt(i));
}

void CARcBridgeDataStd::CopyArrayOfDoubleArray(CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrTarget, CSafetyTypedPtrArray <CObArray, CDoubleArray*> *pdArrSource)
{
	if(!pdArrTarget || !pdArrSource) return;
	long nSizeRoot	= pdArrSource->GetSize();

	AhTPAMake(nSizeRoot, pdArrTarget, (CDoubleArray*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CDoubleArray *dArrTarget	= pdArrTarget->GetAt(i);
		CDoubleArray *dArrSource	= pdArrSource->GetAt(i);
		CopyDoubleArray(dArrTarget, dArrSource);
	}
}

void CARcBridgeDataStd::CopyArrayOfSetRebarJoint(CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarJoint*> *pArrSource)
{
	if(!pArrTarget || !pArrSource) return;
	long nSizeRoot	= pArrSource->GetSize();

	AhTPAMake(nSizeRoot, pArrTarget, (CSetRebarJoint*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CSetRebarJoint *pTarget		= pArrTarget->GetAt(i);
		CSetRebarJoint *pSource		= pArrSource->GetAt(i);
		
		*pTarget	= *pSource;
	}
}

void CARcBridgeDataStd::CopyArrayOfSetRebarDan(CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrTarget, CSafetyTypedPtrArray <CObArray, CSetRebarDan*> *pArrSource)
{
	if(!pArrTarget || !pArrSource) return;
	long nSizeRoot	= pArrSource->GetSize();

	AhTPAMake(nSizeRoot, pArrTarget, (CSetRebarDan*)0);

	long i = 0; for(i = 0; i < nSizeRoot; i++)
	{
		CSetRebarDan *pTarget	= pArrTarget->GetAt(i);
		CSetRebarDan *pSource	= pArrSource->GetAt(i);
		
		*pTarget	= *pSource;
	}
}

// ǥ�شܸ� ��ٿ� ���� ¦��bri���� ����Ÿ�� �����Ѵ�.
// ���ʸ� ������ ��� ����Ÿ�� �����Ѵ�.
void CARcBridgeDataStd::CopyToCoupleBridge_RebarStandardSection(long nBri)
{
	long nIdxCoupleBri	= GetIdxMatchBridge(nBri);
	if(nIdxCoupleBri < 0) return;

	CRcBridgeRebar *pBriSource	= GetBridgeRebar(nBri);
	CRcBridgeRebar *pBriTarget	= GetBridgeRebar(nIdxCoupleBri);
	if(pBriSource == NULL || pBriTarget == NULL) return;

	if(!pBriSource->m_bApplySameSupportRebar) return;

	long stt(0);

	// ǥ�شܸ� ö�� ��ġ ���� ������ (���ö��, ����ö��)
	for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			// ���ö��
			pBriTarget->m_placeByBlock_SupportRebar_UpperSlab[upper][dan]	= pBriSource->m_placeByBlock_SupportRebar_UpperSlab[upper][dan];											// ��ν����� ���ϸ�
		}

		for(stt = 0; stt < 2; stt++)
		{
			for(long left = 0; left < 2; left++)
			{
				pBriTarget->m_placeByBlock_SupportRebar_OutWall[stt][left][dan]	= pBriSource->m_placeByBlock_SupportRebar_OutWall[stt][left][dan];										// ������ü ������, ������
			}
		}
	}

	// ����ö��
	pBriTarget->m_placeByBlock_ShearRebar_UpperSlab	= pBriSource->m_placeByBlock_ShearRebar_UpperSlab;		// ��ν����� ����ö�� 
	for(stt = 0; stt < 2; stt++)
	{
		pBriTarget->m_placeByBlock_ShearRebar_OutWall[stt]	= pBriSource->m_placeByBlock_ShearRebar_OutWall[stt];	// ����ü ������ ����ö��
	}
	pBriTarget->m_nIdxCTC_SupportAndShearRebar	= pBriSource->m_nIdxCTC_SupportAndShearRebar;						// ���,����ö�� ǥ�� ��
	

	// ������ü�� ���ؼ�...
	// ������ü
	long nCountInWall1	= pBriSource->GetCountInWall();
	long nCountInWall2	= pBriTarget->GetCountInWall();
	if(nCountInWall1 == nCountInWall2)
	{
		for(long wall = 0; wall < nCountInWall1; wall++)
		{
			CWallApp *pWallSource	= pBriSource->GetInWall(wall);
			CWallApp *pWallTarget	= pBriTarget->GetInWall(wall);
			if(pWallSource && pWallTarget)
			{
				pWallTarget->CopyPlaceRebar_SupportRebar(pWallSource);
				pWallTarget->CopyPlaceRebar_ShearRebar(pWallSource);
				pWallTarget->m_nIdxCTC_SupportAndShearRebar	= pWallSource->m_nIdxCTC_SupportAndShearRebar;		// ���,����ö�� ǥ�ع�ġ����
			}
		}
	}
}

CString CARcBridgeDataStd::GetBridgeTotalName(long nBri)
{
	CRcBridgeApp* pBri = GetBridge(nBri);
	if(!pBri) return _T("");

// 	CString str	= _T("");
	CString str = GetStringBridgeOrder(nBri) + _T(" : ") + pBri->m_strBridgeName
		+ _T(" [") + pBri->GetStringBridgeType() + _T("]");

	return str;
}


//////////////////////////////////////////////////////////////////////////
// ö������ ���� �߰��Լ�....
// railload ���� �о� ��.
BOOL CARcBridgeDataStd::GetRailLoadWheelInfo(long nIdxWheel, long &nType, double &dWeight, double &dDist)
{
	// Ÿ��
	long nQtyWheel	= m_dArrTypeLoadTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	nType	= (long)m_dArrTypeLoadTrainWheel.GetAt(nIdxWheel);

	// ����
	nQtyWheel	= m_dArrLoadTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	dWeight	= m_dArrLoadTrainWheel.GetAt(nIdxWheel);

	// �������߰��� �Ÿ�
	nQtyWheel	= m_dArrDistTrainWheel.GetSize();
	if(nQtyWheel <= nIdxWheel) nIdxWheel = nQtyWheel-1;
	if(nIdxWheel < 0) return FALSE;

	dDist	= m_dArrDistTrainWheel.GetAt(nIdxWheel);

	return TRUE;
}

// railload ������ ������.
// void CARcBridgeDataStd::SetRailLoadWheelInfo(long nIdxWheel, long nType, double dWeight, double dDist)
// {
// 	// Ÿ��
// 	long nQtyWheel	= m_dArrTypeLoadTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrTypeLoadTrainWheel.Add((double)nType);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrTypeLoadTrainWheel.SetAt(nIdxWheel, (double)nType);
// 
// 	// ����
// 	nQtyWheel	= m_dArrLoadTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrLoadTrainWheel.Add(dWeight);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrLoadTrainWheel.SetAt(nIdxWheel, dWeight);
// 
// 	// �������߰��� �Ÿ�
// 	nQtyWheel	= m_dArrDistTrainWheel.GetSize();
// 	if(nQtyWheel <= nIdxWheel)
// 	{
// 		long i = 0; for(i = 0; i < nIdxWheel-nQtyWheel+1; i++)
// 			m_dArrDistTrainWheel.Add(dDist);
// 	}
// 	if(nIdxWheel >= 0)
// 		m_dArrDistTrainWheel.SetAt(nIdxWheel, dDist);
// }

double CARcBridgeDataStd::GetRailLoadWeightWheel(long nSttWheel, double dLengthSpan)
{
	long nQtyWheel = m_dArrLoadTrainWheel.GetSize();

	double dLoadSum = 0;
	double dDistSum = 0;
	long i=0; for(i=nSttWheel ;i<nQtyWheel; i++)
	{
		if(dDistSum>dLengthSpan) break;
		if(m_dArrTypeLoadTrainWheel.GetAt(i)==2) continue;
		// ó�� ������ �ɸ�. �Ÿ��� ���� �������� �Ÿ���....
		dDistSum += m_dArrDistTrainWheel.GetAt(i);
		dLoadSum += m_dArrLoadTrainWheel.GetAt(i);
	}

	return dLoadSum;
}

// double CARcBridgeDataStd::GetRailLoadDistWheel(long nWheel)
// {
// 	double dDistSum = 0;
// 	long i=0; for(i=0 ;i<nWheel; i++)
// 	{
// 		dDistSum += m_dArrDistTrainWheel.GetAt(i);
// 	}
// 
// 	return dDistSum;
// }

double CARcBridgeDataStd::GetRailLoadLengthWheel(long nSttWheel)
{
	long nQtyWheel = m_dArrDistTrainWheel.GetSize();

	double dDistSum = 0;
	long i=0; for(i=nSttWheel ;i<nQtyWheel; i++)
	{
		if(i>0 && m_dArrTypeLoadTrainWheel.GetAt(i)==2) 
		{
			dDistSum -= m_dArrDistTrainWheel.GetAt(i-1);
			continue;
		}
		dDistSum += m_dArrDistTrainWheel.GetAt(i);
	}

	return dDistSum;
}

// ö�����߿� ������ ���� ���� ���� ����
long CARcBridgeDataStd::GetCountRailLoadWheel()
{
	return (long)m_dArrLoadTrainWheel.GetSize();
}

BOOL CARcBridgeDataStd::IsRailDesign()
{
	if(m_nTypeConditionApply<DESIGN_CONDITION_RAIL)
		return FALSE;
	else 
		return TRUE;
}

BOOL CARcBridgeDataStd::IsRailDesign2017()
{
	if(IsRailDesign() == FALSE) return FALSE;

	// (ARCBRIDGE-3484) KDS 2018�� �����ص� 2017�� ���Ե� ������� �ݿ��ǵ���
	return m_nTypeRailCode >= DESIGN_RAIL_2017;
}

BOOL CARcBridgeDataStd::IsRailDesignKDS2018()
{
	if(IsRailDesign() == FALSE) return FALSE;

	return m_nTypeRailCode == DESIGN_RAIL_KDS_2018;
}

BOOL CARcBridgeDataStd::IsDoro2010()
{
	if( m_nTypeConditionApply ==DESIGN_CONDITION_ROAD && m_nTypeRoadCode == DESIGN_ROAD_2010 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CARcBridgeDataStd::IsDesignKDS2019()
{
	return m_nTypeRoadCode == DESIGN_ROAD_KDS_2019 ? TRUE : FALSE;
}

CString CARcBridgeDataStd::GetStringRailLoad(long nLoadType)
{
	if(nLoadType < 0) return _T("");

	CString str("");
	if(IsRailDesign2017())
	{
		if(nLoadType>=MAX_RAIL_LOAD_2017) return _T("");

		str = m_sRailLoadType2017[nLoadType];
	}
	else
	{
		if(nLoadType>=MAX_RAIL_LOAD) return _T("");

		str = m_sRailLoadType[nLoadType];
	}

	return str;
}

// long CARcBridgeDataStd::GetTypeRailLoad(CString strLoad)
// {
// 	long i=0; for(i=0; i<MAX_RAIL_LOAD; i++)
// 		if(strLoad==m_sRailLoadType[i])
// 			return i;
// 
// 	return -1;
// }

void CARcBridgeDataStd::SetRailLoad()
{
	m_sNameRailLoad			= _T("");	// ������ �̸�,LS-25.....
	m_DistributionLoadFront	= 0;		// �������� ��������
	m_DistributionLoadBack	= 0;		// �������� ��������
	m_RailLoadWheel			= 0;
	m_dArrTypeLoadTrainWheel.RemoveAll();	// ���������� ����
	m_dArrDistTrainWheel.RemoveAll();		// ���������� �Ÿ�, �������߰��� �Ÿ�
	m_dArrLoadTrainWheel.RemoveAll();		// ���������� ũ��, �������߸�....

	if(!IsRailDesign()) return;
	m_sNameRailLoad = GetStringRailLoad(m_nTypeRailLoad);

	if(IsRailDesign2017())
	{
		if(m_nTypeRailLoad==RAIL_LOAD_KRL2012 || m_nTypeRailLoad==RAIL_LOAD_KRL2012_P)
		{
			BOOL bKRL_P = m_nTypeRailLoad==RAIL_LOAD_KRL2012_P ? TRUE : FALSE;
			m_RailLoadWheel = bKRL_P ? 16.5 : 22.0; // ton
			m_RailDistWheel	= 3.0;
			m_RailDistWheelMid	= 3.0;
			m_DistributionLoadFront = bKRL_P ? 6.0 : 8.0;
			m_DistributionLoadBack = bKRL_P ? 6.0 : 8.0;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(m_RailDistWheel);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);
		}

		// EL18 ������ ����
		if(m_nTypeRailLoad==RAIL_LOAD_EL18_2017)
		{
			m_RailLoadWheel = 18.0;
			m_RailDistWheel	= 2.10;
			m_RailDistWheelMid	= 1.75*2;

			for(long i=0; i<m_nRailQtyCar; i++)
			{
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(10.30);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	
				if(i==m_nRailQtyCar-1)
					m_dArrDistTrainWheel.Add(0);
				else
					m_dArrDistTrainWheel.Add(3.500);
			}
		}
	}
	else
	{
		if(m_nTypeRailLoad==RAIL_LOAD_LS25 || m_nTypeRailLoad==RAIL_LOAD_LS22 || m_nTypeRailLoad==RAIL_LOAD_LS18)
		{
			if(m_nTypeRailLoad==RAIL_LOAD_LS25) m_RailLoadWheel = 25.0;
			if(m_nTypeRailLoad==RAIL_LOAD_LS22) m_RailLoadWheel = 22.0;
			if(m_nTypeRailLoad==RAIL_LOAD_LS18) m_RailLoadWheel = 18.0;
			//
			m_RailDistWheel		= 1.50;
			m_RailDistWheelMid	= 1.50;

			double dLoad1 = m_RailLoadWheel/2;
			double dLoad2 = m_RailLoadWheel*2.0/3.0;

			m_DistributionLoadBack	= m_RailLoadWheel/3;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad1);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.700);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);	// 50/3
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad1);	m_dArrDistTrainWheel.Add(2.400);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.700);
			//
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);	// 50/3
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_P);	m_dArrLoadTrainWheel.Add(dLoad2);	m_dArrDistTrainWheel.Add(1.500);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadBack);	m_dArrDistTrainWheel.Add(0);
		}

		if(m_nTypeRailLoad==RAIL_LOAD_EL25 || m_nTypeRailLoad==RAIL_LOAD_EL22 || m_nTypeRailLoad==RAIL_LOAD_EL18)
		{
			if(m_nTypeRailLoad==RAIL_LOAD_EL25) m_RailLoadWheel = 25.0;
			if(m_nTypeRailLoad==RAIL_LOAD_EL22) m_RailLoadWheel = 22.0;
			if(m_nTypeRailLoad==RAIL_LOAD_EL18) m_RailLoadWheel = 18.0;
			m_RailDistWheel		= 2.10;
			m_RailDistWheelMid	= 1.75*2;

			long i=0; for(i=0; i<m_nRailQtyCar; i++)
			{
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(10.30);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(2.100);
				m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	
				if(i==m_nRailQtyCar-1)
					m_dArrDistTrainWheel.Add(0);
				else
					m_dArrDistTrainWheel.Add(3.500);
			}
		}

		if(m_nTypeRailLoad==RAIL_LOAD_HL25)
		{
			m_RailLoadWheel			= 25.0;
			m_RailDistWheel			= 1.60;
			m_RailDistWheelMid		= 1.60;
			m_DistributionLoadFront	= 8.00;
			m_DistributionLoadBack	= 8.00;

			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadFront);	m_dArrDistTrainWheel.Add(0.8);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(1.600);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_PD);	m_dArrLoadTrainWheel.Add(m_RailLoadWheel);	m_dArrDistTrainWheel.Add(0.800);
			m_dArrTypeLoadTrainWheel.Add(RAIL_LOADTYPE_W);	m_dArrLoadTrainWheel.Add(m_DistributionLoadFront);	m_dArrDistTrainWheel.Add(0.0);
		}
		// ����� ����
		if(m_nTypeRailLoad == RAIL_LOAD_USER)
		{
		}
	}	
}

// ���� ��ȭ�� ���� ������ ��ȭ�� ���ϱ�
// csOrg : ���� ����
// csChg : ��ȯ ����
// ��� �����ʹ� CGS ������ ����ǹǷ� ���� ������ CGS �����̸� ó�� ���ص� ��.
double CARcBridgeDataStd::GetValueUnitType(double dValue,CString csOrg,CString csChg)
{
	if( m_nTypeUnit == 0 && IsLsdDesign() == FALSE ) return dValue;

	dValue = GetValueUnitChange(dValue,csOrg,csChg);

	return dValue;
}

// CGS�����迡 �ش��ϴ� ������ �������� ����� ����ǥ�⸦ return;
// nIdxUnitApply : ������ ������ �������ִ� ����� Index
CString CARcBridgeDataStd::GetStringUnitType(CString sCGS, long nIdxUnitApply)
{
	CString sUnit(_T(""));

	if(IsLsdDesign())
	{
		if(sCGS==UNIT_CGS_TONF)
			sUnit = _T("N");
		else if(sCGS==UNIT_CGS_KGFM || sCGS==UNIT_CGS_KGFCM || sCGS==UNIT_CGS_TONFM)
			sUnit = _T("N.mm");
		else if(sCGS==UNIT_CGS_KGF_M || sCGS==UNIT_CGS_TONF_M)
			sUnit = _T("N/mm");
		else if(sCGS==UNIT_CGS_KGF_M2 || sCGS==UNIT_CGS_TONF_M2 || sCGS == UNIT_CGS_KGF_CM2 )
			sUnit = _T("N/mm��");
		else if(sCGS==UNIT_CGS_KGF_M3 || sCGS==UNIT_CGS_TONF_M3 || sCGS == UNIT_CGS_KGF_CM3 )
			sUnit = _T("N/mm��");
		else if(sCGS == UNIT_CGS_CM || sCGS == UNIT_CGS_M)
			sUnit = _T("mm");
		else if(sCGS == UNIT_CGS_CM2 || sCGS == UNIT_CGS_M2)
			sUnit = _T("mm��");
	}
	else
	{
		sUnit = GetStringUnitByType(sCGS,m_nTypeUnit,nIdxUnitApply);
	}

	return sUnit;
}

double CARcBridgeDataStd::GetUnitWeightReinforcedConcrete()
{
	if( m_nTypeUnit == 0 ) return m_pBasicConcInfo->m_UWeightSteelConcrete;
	CString szKgf_M3_CGS = "kgf/m��";
	CString szKgf_M3_APP = GetStringUnitType(szKgf_M3_CGS);
	return GetValueUnitType(m_pBasicConcInfo->m_UWeightSteelConcrete,szKgf_M3_CGS,szKgf_M3_APP);
}

BOOL CARcBridgeDataStd::IsSI()
{
	return (m_nTypeUnit==1);
}

void CARcBridgeDataStd::SetStringUnitType()
{
	BOOL bLsd = IsLsdDesign();
	m_szKgf	    = GetStringUnitType(UNIT_CGS_KGF);
	m_szTonf    = GetStringUnitType(UNIT_CGS_TONF);
	m_szKgfM    = GetStringUnitType(UNIT_CGS_KGFM);
	m_szTonfM   = GetStringUnitType(UNIT_CGS_TONFM);
	m_szCM		= GetStringUnitType(UNIT_CGS_CM);
	m_szCM2		= GetStringUnitType(UNIT_CGS_CM2);
	m_szCM3		= GetStringUnitType(UNIT_CGS_CM3);
	m_szKgf_CM2 = GetStringUnitType(UNIT_CGS_KGF_CM2, bLsd? 1 : 0);
	m_szKgf_CM3 = GetStringUnitType(UNIT_CGS_KGF_CM3);
	m_szKgf_M2  = GetStringUnitType(UNIT_CGS_KGF_M2);
	m_szKgf_M3  = GetStringUnitType(UNIT_CGS_KGF_M3);
	m_szTonf_M  = GetStringUnitType(UNIT_CGS_TONF_M);
	m_szTonf_M2 = GetStringUnitType(UNIT_CGS_TONF_M2);
	m_szTonf_M3 = GetStringUnitType(UNIT_CGS_TONF_M3);
	m_szM = bLsd ? _T("mm") : _T("m");
	m_szM2 = bLsd ? _T("mm��") : _T("m��");
	m_szM3 = bLsd ? _T("mm��") : _T("m��");
	m_szM4 = bLsd ? _T("mm��") : _T("m��");
}

BOOL CARcBridgeDataStd::IsRailLoadLS()
{
	if(m_nTypeRailLoad==RAIL_LOAD_LS18) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_LS22) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_LS25) return TRUE;

	return FALSE;
}

BOOL CARcBridgeDataStd::IsRailLoadEL()
{
	if(m_nTypeRailLoad==RAIL_LOAD_EL18) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_EL22) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_EL25) return TRUE;

	return FALSE;
}

double CARcBridgeDataStd::GetValueBasic(double dDia, long nType)
{
	return m_pBasicConcInfo->GetRebarInfo(dDia, nType);
}

double CARcBridgeDataStd::GetValueBasic(long nDia, long nType)
{
	return m_pBasicConcInfo->GetRebarInfo(nDia, nType);
}

/*
#define BRIDGE_LOAD_DEAD		 0 // ��������, �������
#define BRIDGE_LOAD_LIVE_FULL	 1 // Ȱ���� (����)
#define BRIDGE_LOAD_LIVE_ECC	 2 // Ȱ���� (����)
#define BRIDGE_LOAD_TANK		 3 // ��ũ �� Ư�� ����������
#define BRIDGE_LOAD_TOAP_BOTH	 4 // �����
#define BRIDGE_LOAD_TOAP_ONE	 5 // �����
#define BRIDGE_LOAD_TEMP1		 6 // �µ�����(+)
#define BRIDGE_LOAD_TEMP2		 7 // �µ�����(-)
#define BRIDGE_LOAD_SHRINK		 8 // ��������
#define BRIDGE_LOAD_SUBSID		 9 // ����ħ��
#define BRIDGE_LOAD_SB			10 // �õ� �� ��������
#define BRIDGE_LOAD_LR			11 // ��뷹�� ������
#define BRIDGE_LOAD_EQ			12 // ��������
#define BRIDGE_LOAD_BASIC		13 // �⺻������ ����
*/
CString CARcBridgeDataStd::GetStringLoadCase(long nLoadCase, BOOL bEarthQuake)
{
	if(bEarthQuake)
	{
		if(nLoadCase==EARTHQUAKE_LOAD_DEAD)			return "D-EQ";	// ��������
		if(nLoadCase==EARTHQUAKE_LOAD_TRAP_X)		return "LOAD-X";	// ������ TRAP����
		if(nLoadCase==EARTHQUAKE_LOAD_TRAP_Y)		return "LOAD-Y";	// Ⱦ���� TRAP����
		if(nLoadCase==EARTHQUAKE_LOAD_TOAP)			return "H-EQ";	// �������,�������
		if(nLoadCase==EARTHQUAKE_LOAD_INERTIAL)		return "D-IN";	// ������
	}
	else
	{
		if(nLoadCase==BRIDGE_LOAD_DEAD)			return "D";		// ��������
		if(nLoadCase==BRIDGE_LOAD_SOIL)			return "VH";	// �������
		if(nLoadCase==BRIDGE_LOAD_LIVE_FULL)	return "LF";	// Ȱ����
		if(nLoadCase==BRIDGE_LOAD_LIVE_ECC)		return "LE";	// Ȱ����
		if(nLoadCase==BRIDGE_LOAD_TANK)			return "HEAVY";	// ������
		if(nLoadCase==BRIDGE_LOAD_TOAP_BOTH)	return "HB";	// �����
		if(nLoadCase==BRIDGE_LOAD_TOAP_ONE)		return "HO";	// �����
		if(nLoadCase==BRIDGE_LOAD_TEMP1)		return "G1";	// �µ�����(+)
		if(nLoadCase==BRIDGE_LOAD_TEMP2)		return "G2";	// �µ�����(-)
		if(nLoadCase==BRIDGE_LOAD_SHRINK)		return "GS";	// ��������
		if(nLoadCase==BRIDGE_LOAD_SUBSID)		return "SU";	// ����ħ��
		if(nLoadCase==BRIDGE_LOAD_SB)			return "SB";	// �õ� �� ��������
		if(nLoadCase==BRIDGE_LOAD_LR)			return "LR";	// ��뷹�� ������
		if(nLoadCase==BRIDGE_LOAD_EQ)			return "EQ";	// ��������
		if(nLoadCase==BRIDGE_LOAD_WATERHORI)		return "HW";	// �������
		if(nLoadCase==BRIDGE_LOAD_WATERVERT)		return "VW";	// ��������
	}

	return _T("");
}

BOOL CARcBridgeDataStd::IsSupportTypeStd(long nType)
{
	// ����		0x0001
	// ������		0x0002
	// ��������	0x0004
	// ���1��		0x0008
	// ���2��		0x0010
	// ��ġ��		0x0020
	// �ڽ���		0x0040
	DWORD dwType	= 0x0000;
	switch(nType)
	{
	case BRIDGE_TYPE_PORTAL:		dwType	= ARCBRIDGE_LOCK_CODE_PORTAL; break;
	case BRIDGE_TYPE_PI:			dwType = ARCBRIDGE_LOCK_CODE_PI; break;
	case BRIDGE_TYPE_SLAB:			dwType = ARCBRIDGE_LOCK_CODE_SLAB; break;
	case BRIDGE_TYPE_TRAP:			dwType = ARCBRIDGE_LOCK_CODE_TRAP; break;
	case BRIDGE_TYPE_TRAP2:			dwType = ARCBRIDGE_LOCK_CODE_TRAP2; break;
	case BRIDGE_TYPE_ARCH:			dwType = ARCBRIDGE_LOCK_CODE_ARCH; break;
	case BRIDGE_TYPE_BOX:			dwType = ARCBRIDGE_LOCK_CODE_BOX; break;
	case BRIDGE_TYPE_SLAB_STANDARD:	dwType = ARCBRIDGE_LOCK_CODE_SLAB_STD; break;
	default:
		ASSERT(FALSE);
		return FALSE;
		break;
	}

	if(m_dwTypeARcBridge & dwType) return TRUE;	

	return FALSE;
}

BOOL CARcBridgeDataStd::IsLockMaster()
{
	return m_dwTypeARcBridge == 0xffff;// aGetLockID() == SPLOCK_MASTER_LID;
}

// Plus : ����~��ġ,���
BOOL CARcBridgeDataStd::IsLockRcBridgePlus()
{
	return IsSupportTypeStd(BRIDGE_TYPE_ARCH);	
}

// Basic : ����,����,������
BOOL CARcBridgeDataStd::IsLockRcBridgeBasic()
{
	BOOL bSlab = IsSupportTypeStd(BRIDGE_TYPE_SLAB);
	BOOL bArch = IsSupportTypeStd(BRIDGE_TYPE_ARCH);

	return bSlab && !bArch;
}

// ���� : ������ ����
BOOL CARcBridgeDataStd::IsLockRcBridgePortal()
{
	return m_dwTypeARcBridge == ARCBRIDGE_LOCK_CODE_PORTAL;
}

// BOX�� : BOX�� ����
BOOL CARcBridgeDataStd::IsLockRcBridgeBox()
{
	return m_dwTypeARcBridge == ARCBRIDGE_LOCK_CODE_BOX;
}

// BOX�� ���
BOOL CARcBridgeDataStd::IsLockRcBridgeBoxModule()
{
	return IsSupportTypeStd(BRIDGE_TYPE_BOX);
}

BOOL CARcBridgeDataStd::IsLockRcBridgeStmModule()
{
	// LSD ���� ����� STM�� ����ȴ�.
	return (m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_STM || m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_LSD);
}

BOOL CARcBridgeDataStd::IsLockRcBridgeLsdModule()
{
	return (m_dwTypeARcBridge & ARCBRIDGE_LOCK_CODE_LSD);
}

// COutBridgeResister���� ���� �Լ� ����.
CString CARcBridgeDataStd::GetStringFootTypeAll(CRcBridgeRebar *pBri)
{
	CString szFootType = _T("");

	long nKind		= 0;
	BOOL bDirect	= FALSE;
	BOOL bMass		= FALSE;
	BOOL bPile		= FALSE;

	long nCnt = pBri->GetCountFooting();
	long i=0; for(i=0; i<nCnt; i++)
	{
		CFootingApp *pFoot = pBri->GetFooting(i);
		if(pFoot)
		{
			if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN) bDirect = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) bMass = TRUE;
			else if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) bPile = TRUE;
		}
	}

	if(bDirect)
	{
		if(nKind == 0) szFootType += "��������";
//		else if(nKind%2) szFootType += ", ��������";
		else szFootType += ", ��������";
		nKind++;
	}
	if(bMass)
	{
		if(nKind == 0) szFootType += "MASS����";
//		else if(nKind%2) szFootType += ", MASS����";
		else szFootType += ", MASS����";
		nKind++;
	}
	if(bPile)
	{
		if(nKind == 0) szFootType += "���ұ���";
//		else if(nKind%2) szFootType += ", ���ұ���";
		else szFootType += ", ���ұ���";
		nKind++;
	}
	
	return szFootType;
}

void CARcBridgeDataStd::SetVersionFlag(DWORD dwType)
{	
	m_dwTypeARcBridge	= dwType;
}

// ���� ��½� ���ϸ� �����
// m_nTypeFileNameForExcel;	
// ���� ��½� ���ϸ�(0 : ���ϸ�_������_����, 1 : ������_����, 2 : ���ϸ�_����, 3 : ���ϸ�_������, 4 : ������)
CString CARcBridgeDataStd::GetFileNameForExcel(long nBri)
{
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(!pBri) return _T("�������");

	CString sFileName	= _T("");

	CString sRunFileName	= m_pDoc->GetPathName();
	long nFind	= sRunFileName.Find(".arb");
	if(nFind > -1)
//  ReleaseBuffer�� �ϸ� CString�� �ּҸ� ������ ������ ������ ����� Path�� ��������. 
//		sRunFileName.ReleaseBuffer(nFind);
		sRunFileName  = sRunFileName.Left(sRunFileName.ReverseFind('.'));

	CString strBridgeDir(_T(""));
	strBridgeDir = GetStringBridgeDir(nBri);
		
	switch(m_nTypeFileNameForExcel)
	{
	case 0: sFileName.Format("%s_%s_%s", sRunFileName, pBri->m_strBridgeName, strBridgeDir); break;
	case 1: sFileName.Format("%s_%s", pBri->m_strBridgeName, strBridgeDir); break;
	case 2: sFileName.Format("%s_%s", sRunFileName, strBridgeDir); break;
	case 3: sFileName.Format("%s_%s", sRunFileName, pBri->m_strBridgeName); break;
	case 4: sFileName.Format("%s", pBri->m_strBridgeName); break;
	}

	return sFileName;
}

// ���������� ������ ����� ��� DataStd�� ������ ������ ������ ���� ��ġ ���� �ʾƼ� ������ �߻��� �� ����
// �׷��� ���÷� DataStd�� ������ ������ ������ ��ġ ���� ��� ��.
void CARcBridgeDataStd::SetLineAppToBridge()
{
	int nSizeBri = GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeRebar* pBri = GetBridgeRebar(nBr);
		if(!pBri->m_bUnderGroundRahmen)
		{
			*pBri->m_pLineApp = *m_pLineApp;
			CString strErrorMsg = _T("���� �����Ͱ� �ùٸ��� �ʽ��ϴ�.  ");
			pBri->m_pLineApp->SetDataIp(strErrorMsg);
		}
		pBri->GetLineBase()->ReSetSearchIp();
	}
}

// �����걳�� ������ ������ ǥ�ص� �۾��� ������ �� �ִ�.
BOOL CARcBridgeDataStd::IsEnableSlabStandardOutput()
{
	if(!(m_dwTypeARcBridge & BRIDGE_TYPE_SLAB_STANDARD)) return FALSE;
	long nCount	= GetBridgeSize();
	long i = 0; for(i = 0; i < nCount; i++)
	{
		CRcBridgeRebar *pBri	= GetBridgeRebar(i);
		if(!pBri) continue;

		if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
			return FALSE;
	}

	return TRUE;
}

// ���߶�౳�� ��쿡 ���������� �����ϰ�
// ���߶�౳�� �ƴ� ���� CARcBridgeDataStd�� ������ ������������ �����Ѵ�...
void CARcBridgeDataStd::MakeLinefoBridge(long nBridge)
{
	CRcBridgeApp	  *pBri = GetBridge(nBridge);

	*pBri->m_pLineApp = *m_pLineApp;
	CString strErrorMsg = _T("���� �����Ͱ� �ùٸ��� �ʽ��ϴ�.  ");
	pBri->m_pLineApp->SetDataIp(strErrorMsg);

	if(pBri->m_bUnderGroundRahmen)
	{
		// ���������� 100m�� �����Ͽ� ����
		CLineInfoApp *pLine = pBri->m_pLineApp;
		double vSlope = frM(pBri->m_dUnderGroundCenterELEnd-pBri->m_dUnderGroundCenterELStt)/pBri->m_dLengthBridge;
		double dAddLength = 100000;
		
		// 1) ���ܰ�ȹ�� ����
		pLine->m_VIPsu = 4;
		pLine->m_VIPData[0][0] = pBri->m_dStationBridgeStt - dAddLength;
		pLine->m_VIPData[0][1] = frM(pBri->m_dUnderGroundCenterELStt) - dAddLength*vSlope;
		pLine->m_VIPData[0][2] = 0;
		pLine->m_VIPData[1][0] = pBri->m_dStationBridgeStt;
		pLine->m_VIPData[1][1] = frM(pBri->m_dUnderGroundCenterELStt);
		pLine->m_VIPData[1][2] = 0;
		pLine->m_VIPData[2][0] = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
		pLine->m_VIPData[2][1] = frM(pBri->m_dUnderGroundCenterELEnd);
		pLine->m_VIPData[2][2] = 0;
		pLine->m_VIPData[3][0] = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
		pLine->m_VIPData[3][1] = frM(pBri->m_dUnderGroundCenterELEnd) + dAddLength*vSlope;
		pLine->m_VIPData[3][2] = 0;

		// 2) ���� ����
//		AhTPAMake(2, &pLine->m_pSlopLeft,  (CSlop*)0);
//		AhTPAMake(2, &pLine->m_pSlopRight, (CSlop*)0);
//		pLine->m_pSlopLeft[0]->m_dStationPyungubae = pBri->m_dStationBridgeStt - dAddLength;
//		pLine->m_pSlopLeft[1]->m_dStationPyungubae = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
//
//		pLine->m_pSlopRight[0]->m_dStationPyungubae = pBri->m_dStationBridgeStt - dAddLength;
//		pLine->m_pSlopRight[0]->m_dValuePyungubae   = pBri->m_dUnderGroundSlopeSlabRight/100;
//		pLine->m_pSlopRight[1]->m_dStationPyungubae = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dAddLength;
//		pLine->m_pSlopRight[1]->m_dValuePyungubae   = pBri->m_dUnderGroundSlopeSlabRight/100;

		// 3) ���ݰ�,��鼱���� �����ϰ� ���...
	}
}

// �����ڷ� ����/�б�
void CARcBridgeDataStd::SerializeBase(CArchive &ar)
{
	// 1 : m_bApplyRigidWallEQRotate;
	// 2 : m_Pi_F_Cobel;
	// 3 : m_nTypeRoadCode, m_nTypeRailCode (BOX_TYPE)
	// 4 : m_nLSDRoadType, m_dDesignRoadWidth m_Es_Steel, m_RatePoasong, m_plsdEnv �߰�
	// 5 : m_pUserBmData ����� ���� ����
	// 6 : m_bApplyBoredPileASD
	// 7 : m_nApplyPhcPileASD
	// 8 : m_nTypeCheckSteelPile
	// 9: m_nSelectPmin_Hyunta_Extreme
	// 10 : m_bPrintConditionApply
	// 11 : m_nTypeFootingSafetyApplyCoefficientImpact
	// 12 : m_nTypeEarthQuakeApply
	// 13 : m_dRateDAMP_EQ_CD

	long nFlagBase = 13;
	const CString sID	= "ARcBridge Base File";

	if(ar.IsStoring())
	{
		ar << nFlagBase;
		ar << sID;

		// SetGridDataFactor
		ar << m_nTypeConditionApply;
		ar << m_nTypeKciCode;
		ar << m_nTypeRoadCode;
		ar << m_nTypeRailCode;
		ar << m_nDegreeBridge;
		ar << m_nDegreeRail;
		ar << m_nTypeRailLoad;
		ar << m_nTypeRailQty;
		ar << m_nTypeUnit;
		ar << m_Fck;
		ar << m_Fy;
		ar << m_bIsDBar_ForDowelBar;
		ar << m_Ec;
		ar << m_Es;
		ar << m_NValue_Steel;
		ar << m_Pi_F; 
		ar << m_Pi_C; 
		ar << m_Pi_V; 
		ar << m_Pi_F_Cobel;
		ar << m_dRateDryAndShrinkage; 
		ar << m_dCoefficientExpansion; 
		ar << m_nDegreeEarthQuake;
		ar << m_nRegionEarthQuake;
		ar << m_dCoefficientGround;
		ar << m_nTypeCoefficientGround;
		ar << m_dRateDAMP_EQ_CD;
		ar << m_gValue;
		ar << m_nTypeRigidFootingEQ;
		ar << m_bApplyRigidWallEQRotate;
		ar << m_nTypeJointPosFootingEQ;
		ar << m_nTypePropertyPosFootingEQ;
		ar << m_nTypeSoilPressureEQ;
		ar << m_nTypeUpperWeightEQ;
		ar << m_dLengthRebarMax;
		ar << m_nTypeRebarConnect;
		ar << m_nTypeBMApply;
		ar << m_pBasicConcInfo->m_nRoundRebarConnect;
		ar << m_bApplyAllowInc;
		ar << m_nTypeFileNameForExcel;
		ar << m_nTypeCalcSlabEffWidth;
		ar << m_nTypeFootingSafetyApplyCoefficientImpact;
		ar << m_nApplyBoredPileASD;
		ar << m_nApplyPhcPileASD;
		ar << m_nTypeCheckSteelPile;
		ar << m_nSelectPmin_Hyunta_Extreme;
		ar << m_bPrintConditionApply;
		ar << m_nTypeEarthQuakeApply;
		ar << m_nTypeProcess;

		// SetGridDataJoint
		long i = 0; for(i = 0; i < REBAR_JOINT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_JOINT_COL; j++)
				ar << m_pBasicConcInfo->m_RebarJoint[i][j];
		}

		// SetGridDataRailLoad
		m_dArrTypeLoadTrainWheel.Serialize(ar);
		m_dArrLoadTrainWheel.Serialize(ar);
		m_dArrDistTrainWheel.Serialize(ar);

		// SetGridDataRebarFactor
		ar << m_pBasicConcInfo->m_dAlpha_UpRebar;
		ar << m_pBasicConcInfo->m_dAlpha_GenBar; 
		ar << m_pBasicConcInfo->m_dBeta_Tomak1; 
		ar << m_pBasicConcInfo->m_dBeta_Tomak2; 
		ar << m_pBasicConcInfo->m_dBeta_GenBar; 
		ar << m_pBasicConcInfo->m_dIpSmallConc; 
		ar << m_pBasicConcInfo->m_dIpGenConc; 

		// SetGridDataSettle
		for(i = 0; i < REBAR_SETTLE_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_SETTLE_COL; j++)
				 ar << m_pBasicConcInfo->m_RebarSettle[i][j];
		}

		// SetGridDataUWeight
		ar << m_pBasicConcInfo->m_UWeightSteel;
		ar << m_pBasicConcInfo->m_UWeightCastIron;
		ar << m_pBasicConcInfo->m_UWeightAluminum;
		ar << m_pBasicConcInfo->m_UWeightSteelConcrete;
		ar << m_pBasicConcInfo->m_UWeightGravel;
		ar << m_pBasicConcInfo->m_UWeightCement;
		ar << m_pBasicConcInfo->m_UWeightLumber;
		ar << m_pBasicConcInfo->m_UWeightInvChungje;
		ar << m_pBasicConcInfo->m_UWeightAsphalt;
		ar << m_pBasicConcInfo->m_UWeightConcrete;


		// SetGridDataWeight
		for(i = 0; i < REBAR_WEIGHT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_WEIGHT_COL; j++)
				ar << m_pBasicConcInfo->m_RebarWeight[i][j];
		}

		// Lsd�߰�
		ar << m_nLSDRoadType;
		ar << m_dDesignRoadWidth;
		ar << m_Es_Steel;
		ar << m_RatePoasong;
		m_plsdEnv->Serialize(ar);

		m_pUserBmData->Serialize(ar);
	}
	else
	{
		ar >> nFlagBase;
		CString sIDLoad	= _T("");
		ar >> sIDLoad;
		if(sIDLoad != sID)
		{
			AfxMessageBox("ARcBridge �����ڷ� ������ �ƴմϴ�.");
			return;
		}


				// SetGridDataFactor
		ar >> m_nTypeConditionApply;
		ar >> m_nTypeKciCode;
		if(nFlagBase > 2)
		{
			ar >> m_nTypeRoadCode;
			ar >> m_nTypeRailCode;
		}
		ar >> m_nDegreeBridge;
		ar >> m_nDegreeRail;
		ar >> m_nTypeRailLoad;
		ar >> m_nTypeRailQty;
		ar >> m_nTypeUnit;
		ar >> m_Fck;
		ar >> m_Fy;
		ar >> m_bIsDBar_ForDowelBar;
		ar >> m_Ec;
		ar >> m_Es;
		ar >> m_NValue_Steel;
		ar >> m_Pi_F; 
		ar >> m_Pi_C; 
		ar >> m_Pi_V; 
		if(nFlagBase > 1)
		{
			ar >> m_Pi_F_Cobel;
		}
		ar >> m_dRateDryAndShrinkage; 
		ar >> m_dCoefficientExpansion; 
		ar >> m_nDegreeEarthQuake;
		ar >> m_nRegionEarthQuake;
		ar >> m_dCoefficientGround;
		if(nFlagBase > 11)
			ar >> m_nTypeCoefficientGround;
		if(nFlagBase > 12)
			ar >> m_dRateDAMP_EQ_CD;
		ar >> m_gValue;
		ar >> m_nTypeRigidFootingEQ;
		if(nFlagBase > 0)
		{
			ar >> m_bApplyRigidWallEQRotate;
		}
		ar >> m_nTypeJointPosFootingEQ;
		ar >> m_nTypePropertyPosFootingEQ;
		ar >> m_nTypeSoilPressureEQ;
		ar >> m_nTypeUpperWeightEQ;
		ar >> m_dLengthRebarMax;
		ar >> m_nTypeRebarConnect;
		ar >> m_nTypeBMApply;
		ar >> m_pBasicConcInfo->m_nRoundRebarConnect;
		ar >> m_bApplyAllowInc;
		ar >> m_nTypeFileNameForExcel;
		ar >> m_nTypeCalcSlabEffWidth;
		if(nFlagBase > 10)
			ar >> m_nTypeFootingSafetyApplyCoefficientImpact;
		if(nFlagBase > 5)
			ar >> m_nApplyBoredPileASD;
		if(nFlagBase > 6)
			ar >> m_nApplyPhcPileASD;
		if(nFlagBase > 7)
			ar >> m_nTypeCheckSteelPile;
		if(nFlagBase > 8)
			ar >> m_nSelectPmin_Hyunta_Extreme;
		if(nFlagBase > 9)
			ar >> m_bPrintConditionApply;
		else
			m_bPrintConditionApply = FALSE;

		if(nFlagBase > 11)
			ar >> m_nTypeEarthQuakeApply;

		ar >> m_nTypeProcess;

		// SetGridDataJoint
		long i = 0; for(i = 0; i < REBAR_JOINT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_JOINT_COL; j++)
				ar >> m_pBasicConcInfo->m_RebarJoint[i][j];
		}

		// SetGridDataRailLoad
		m_dArrTypeLoadTrainWheel.Serialize(ar);
		m_dArrLoadTrainWheel.Serialize(ar);
		m_dArrDistTrainWheel.Serialize(ar);

		// SetGridDataRebarFactor
		ar >> m_pBasicConcInfo->m_dAlpha_UpRebar;
		ar >> m_pBasicConcInfo->m_dAlpha_GenBar; 
		ar >> m_pBasicConcInfo->m_dBeta_Tomak1; 
		ar >> m_pBasicConcInfo->m_dBeta_Tomak2; 
		ar >> m_pBasicConcInfo->m_dBeta_GenBar; 
		ar >> m_pBasicConcInfo->m_dIpSmallConc; 
		ar >> m_pBasicConcInfo->m_dIpGenConc; 

		// SetGridDataSettle
		for(i = 0; i < REBAR_SETTLE_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_SETTLE_COL; j++)
				 ar >> m_pBasicConcInfo->m_RebarSettle[i][j];
		}

		// SetGridDataUWeight
		ar >> m_pBasicConcInfo->m_UWeightSteel;
		ar >> m_pBasicConcInfo->m_UWeightCastIron;
		ar >> m_pBasicConcInfo->m_UWeightAluminum;
		ar >> m_pBasicConcInfo->m_UWeightSteelConcrete;
		ar >> m_pBasicConcInfo->m_UWeightGravel;
		ar >> m_pBasicConcInfo->m_UWeightCement;
		ar >> m_pBasicConcInfo->m_UWeightLumber;
		ar >> m_pBasicConcInfo->m_UWeightInvChungje;
		ar >> m_pBasicConcInfo->m_UWeightAsphalt;
		ar >> m_pBasicConcInfo->m_UWeightConcrete;


		// SetGridDataWeight
		for(i = 0; i < REBAR_WEIGHT_ROW; i++)
		{
			long j = 0; for(j = 0; j < REBAR_WEIGHT_COL; j++)
				ar >> m_pBasicConcInfo->m_RebarWeight[i][j];
		}

		if (nFlagBase > 3)
		{
			// Lsd�߰�
			ar >> m_nLSDRoadType;
			ar >> m_dDesignRoadWidth;
			ar >> m_Es_Steel;
			ar >> m_RatePoasong;
			m_plsdEnv->Serialize(ar);
		}
		if(nFlagBase > 4)
		{
			m_pUserBmData->Serialize(ar);
		}
	}
}

BOOL CARcBridgeDataStd::IsExistMainRebarStraightHorz(CColumnApp *pColumn, long nDan)
{
	BOOL bResult = FALSE;

	CTwinVector tv;
	CTwinVectorArray tvArrRebarPlan;
	pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, nDan);

	if(tvArrRebarPlan.GetSize() > 0)
	{
		switch(pColumn->m_nType)
		{
		case COL_TYPE_CIRCLE:
		case COL_TYPE_RECTANGLE:
		case COL_TYPE_TRACK:
			bResult = tvArrRebarPlan.GetTvByInfo(NOTRANS("��մܸ���"), tv);
			break;

		case COL_TYPE_POLY:
			tv = tvArrRebarPlan.GetAt(0);
			bResult = Compare(tv.GetLength(), 0.0, _T(">"));
			break;

		default:
			bResult = FALSE;
		}
	}

	return bResult;
}

BOOL CARcBridgeDataStd::IsExistMainRebarStraightVert(CColumnApp *pColumn, long nDan)
{
	BOOL bResult = FALSE;

	CTwinVector tv;
	CTwinVectorArray tvArrRebarPlan;
	pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, nDan);

	if(tvArrRebarPlan.GetSize() > 0)
	{
		switch(pColumn->m_nType)
		{
		case COL_TYPE_CIRCLE:
		case COL_TYPE_RECTANGLE:
		case COL_TYPE_TRACK:
			bResult = tvArrRebarPlan.GetTvByInfo(NOTRANS("��մܸ�������"), tv);
			break;

		case COL_TYPE_POLY:
			bResult = FALSE;
			break;

		default:
			bResult = FALSE;
		}
	}

	return bResult;
}

// ��� �ִ�ö�ٺ� ����
double CARcBridgeDataStd::GetPmaxColumn()
{
	if(IsDoro2010())
		return MaxRebar_Column_Doro2010;
	else
		return MaxRebar_Column;
}

void CARcBridgeDataStd::SyncBridgeFactor(CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM)
{
	if(pBriOrg == NULL || pBriForBM == NULL) return;
	//ARCBRIDGE-2492 ��ũ��Ʈ ���� ��ũ
	
	//// Fck ////
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fck[ix] = pBriOrg->m_Fck[ix];
	}

	//// Fy ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fy[ix] = pBriOrg->m_Fy[ix];
	}

	//// Fvy ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Fvy[ix] = pBriOrg->m_Fvy[ix];
	}

	//// Ec ///
	for (long ix =0; ix < ePartCount; ix++)
	{
		pBriForBM->m_Ec[ix] = pBriOrg->m_Ec[ix];
	}
}

BOOL CARcBridgeDataStd::MakeRcBridgeForBM( CRcBridgeRebar* pBriOrg, CRcBridgeRebar *pBriForBM, BOOL bForTogong )
{
	if(pBriOrg == NULL || pBriForBM == NULL) return FALSE;
	
	// ���� �̶�� ������ ������� �ڸ��� �ʰ� �ϳ��� ����.
	if(pBriOrg->m_bIsExp && pBriOrg->m_nHeightForSlabBM == 0)
		pBriOrg->m_nHeightForSlabBM	= 1;

	//��ġ�������� �̷� ���ϵ�
	if(pBriOrg->m_nDesPanel == -1 && pBriForBM->m_nDesPanel > -1)
		pBriOrg->m_nDesPanel = pBriForBM->m_nDesPanel;
	if(pBriOrg->m_nNamePanel == -1 && pBriForBM->m_nNamePanel > -1)
		pBriOrg->m_nNamePanel = pBriForBM->m_nNamePanel;

	*pBriForBM	= *pBriOrg;
	pBriForBM->m_pARcBridgeDataStd	= pBriOrg->m_pARcBridgeDataStd;
	pBriForBM->m_bBridgeForBM = TRUE;

	// �������� �׸�. //////
	// Box���� ���� ������ ��ü���̸� ������� ���� �������� ������ش�.
	pBriForBM->m_nTypePlacingMainRebar_UpperSlab	= 0;
	if(pBriForBM->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBriForBM->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ||
		pBriForBM->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		pBriForBM->m_nTypeJongdanForRebar	= 1;
	else
		pBriForBM->m_nTypeJongdanForRebar	= 0;

	// Box�����϶��� ��������� 
	if(bForTogong && pBriOrg->IsBoxType() == FALSE)
		pBriForBM->m_nTypeJongdanForRebar	= 1;	// ����� ��� ������ ����
	/////////////////////////



	// ��ü ���� ���� �ɼǿ� ���� �򱸹�� ��ȹ�� ���� /////////
	CLineInfo *pLine	= pBriForBM->GetLineBase();
	//*pLine	= *pBri->GetLineBase();
	if(pBriForBM->m_nTypeHeightWall == 0)	// �����߽� ���� ����
	{
		// �״�� ����ϸ� ��.
	}
	else if(pBriForBM->m_nTypeHeightWall == 1 || pBriForBM->m_nTypeHeightWall == 2 || pBriForBM->m_nTypeHeightWall == 3)	// 1 : ��ճ��� ����(�ִ�+�ּ�)/2, 2 : �ִ� ��ȹ��, 3 : ������ ��� ����
	{
		double dSta		= pBriOrg->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		CDPoint vAng	= pBriOrg->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		double dELMax	= pBriOrg->GetElMaxSlabUpper(dSta, vAng);
		double dELMin	= pBriOrg->GetElMinSlabUpper(dSta, vAng);
		double dEL		= pLine->GetElevationPlanEl(dSta) - pBriOrg->m_dTP;
		double dELUse	= pBriForBM->m_nTypeHeightWall == 1 || pBriForBM->m_nTypeHeightWall == 3 ? (dELMax+dELMin)/2 : dELMax;
		double dELDiff	= dEL - dELUse;

		long i = 0; for(i = 0; i < pLine->m_VIPsu; i++)
		{
			double dEL		= pLine->m_VIPData[i][1];
			pLine->m_VIPData[i][1]	= dEL - dELDiff;
		}

		// ��� ���踦 0���� ����
		// ��
		for(i = 0; i < pLine->m_pSlopLeft.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopLeft.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// ��
		for(i = 0; i < pLine->m_pSlopRight.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopRight.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// �� ������ ��� ������ ���
		// ������ ��� ���̿� �� �������� ��� ���̿��� ���̸� �����ϴ� EL�� �����Ѵ�.
		if(pBriForBM->m_nTypeHeightWall == 3)
		{
			long nCountJijum	= pBriOrg->GetCountJijum();
			for(i = 1; i < nCountJijum; i++)
			{
				CFootingApp *pFooting	= pBriOrg->GetFootingByJijumNumber(i);
				if(!pFooting) continue;
				if(pFooting->IsVirtualFooting()) continue;
				double dStaJijum	= i == nCountJijum-1 ? pBriOrg->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE) : pBriOrg->GetStationOnJijum(i, 0);
				CDPoint vAngJIjum	= i == nCountJijum-1 ? pBriOrg->GetAngleByTarget(BRIDGE_OFF_END_SLAB,0) : pBriOrg->GetAngleJijum(i);

				double dELMaxJijum	= pBriOrg->GetElMaxSlabUpper(dStaJijum, vAngJIjum);
				double dELMinJijum	= pBriOrg->GetElMinSlabUpper(dStaJijum, vAngJIjum);
				double dELUseJijum	= (dELMaxJijum+dELMinJijum)/2;

				double dELDiff	= dELUseJijum - dELUse;
				
				// ������ level�� �׸��� ��� ���ܰ�翡 ���� ���̵� �� ��� �Ѵ�.
				if(pBriForBM->m_nTypeJongdanForRebar == 0)
				{
					double dELDiffByJong	= pBriForBM->GetDiffELSttAndJijumForVertSection(i, FALSE);
					dELDiff -= dELDiffByJong;
				}
				////

				if(i == nCountJijum-1)
				{
					pBriForBM->m_dELEB -= dELDiff;
				}
				else
				{
					CWallApp *pWall	= pBriForBM->GetInWall(i-1);
					if(!pWall) continue;
					pWall->m_dELB -= dELDiff;
				}
				
			}
		}
	}

	if(pBriForBM->IsBoxType())
	{
		double dStaStt		= pBriOrg->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		double dStaEnd		= pBriOrg->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
		double dElStt = pLine->GetElevationPlanEl(dStaStt);
		double dElEnd = pLine->GetElevationPlanEl(dStaEnd);

		double dDiffH = dElEnd - dElStt;
		// �ϴܿ��� ���� ���� EL�� ������Ű�� ��ü ��� ���̸� ���������.
		if(pBriForBM->m_bSlopeLowerSlab)
		{
			// ������ ��ü ���̴� ����. ������ EL�� ���� ��� �����ϰ��̹Ƿ�
			// ���� �������� ���̸��� ��������.
			// ������ �����̶�� �׳� �ֵ� �ȴ�..
			if(dDiffH < 0)
			{
				long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
				{
					double dEL		= pLine->m_VIPData[ix][1];
					pLine->m_VIPData[ix][1]	= dEL + dDiffH;
				}
				pBriForBM->m_dELSB += dDiffH;
			}
		}
		else
		{
			// ���� �ϴ� EL�� �״�� �ΰ� ����� dDiffH/2 ��ŭ �ø��ų� ���� �ֱ� ���ؼ� ������ �̵����� ���ڴ�...
			long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
			{
				double dEL		= pLine->m_VIPData[ix][1];
				pLine->m_VIPData[ix][1]	= dEL + dDiffH/2.;
			}

			pBriForBM->m_bSlopeLowerSlab	= TRUE;
		}

		// ������ü ���� EL�� �������� ������ �������� ���ش�.
		double dStastt1		= pLine->GetStationByOffsetWidth(pBriForBM->m_dStationBridgeStt, -pBriForBM->m_dWS, pBriForBM->GetAngleJijum(0));
		double dEL		= pLine->GetElevationPlanEl(dStastt1);
 		long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
 		{
 			pLine->m_VIPData[ix][1]	= dEL;
 		}

		pBriForBM->SyncBridge();
	}

	pBriForBM->m_bSettingVertSection	= FALSE;
	pBriForBM->InitSaveCrossSection();
	pBriForBM->InitSaveDistRigidEndSlab();
	pBriForBM->InitSaveDistRigidEndWall();
	pBriForBM->InitSaveOrgInWall();
	pBriForBM->InitSaveXyMatchLineAndSlabUpper();
	pBriForBM->InitSaveXySlabUpperByLenFrSttStation();
	SyncBridgeFactor(pBriOrg, pBriForBM);
	pBriForBM->SyncBridge(0, TRUE);
	pBriForBM->SettingVertSection();
	
	
	return TRUE;
}

double CARcBridgeDataStd::GetEs_Steel()
{
	double dEs_Steel = 2100000;  
	if(IsDoro2010() || IsRailDesign2017() || (m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE && m_nTypeKciCode == 2) || IsLsdDesign())
	{
		dEs_Steel = 2050000;
	}

	return  dEs_Steel;
}

void CARcBridgeDataStd::SerializeLsd( CArchive &ar )
{
	// m_dDesignRoadWidth �߰�
	long nFlag(1);

	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_nTypeDesignMethod;
		ar << m_nLSDRoadType;
		ar << m_dDesignRoadWidth;

		m_plsdEnv->Serialize(ar);
		m_plsdRebar->Serialize(ar);
	}
	else
	{
		ar >> nFlag;

		ar >> m_nTypeDesignMethod;
		ar >> m_nLSDRoadType;
		if (nFlag > 0)
		{
			ar >> m_dDesignRoadWidth;
		}
		

		m_plsdEnv->Serialize(ar);
		m_plsdRebar->Serialize(ar);

		if(m_nTypeDesignMethod==DESIGN_METHOD_LSD)
		{
			m_nTypeKciCode = 2;
		}
	}
}

BOOL CARcBridgeDataStd::IsLsdDesign()
{
	if(IsLockRcBridgeLsdModule() && m_nTypeDesignMethod==DESIGN_METHOD_LSD)
		return TRUE;

	return FALSE;
}

void CARcBridgeDataStd::SetChangeDesignMethod()
{
	SetStringUnitType();
	BOOL bLSD = IsLsdDesign();

	m_pBasicConcInfo->SetDesignLsd(bLSD);

	if (bLSD)
	{
		m_nTypeConditionApply = DESIGN_CONDITION_ROAD;
		m_nTypeRoadCode = DESIGN_ROAD_2015;
		SetDataDefaultMatResistFactor();
		SetDataDefaultBaseResistFactor();
	}
	else
	{
		m_nTypeRoadCode = DESIGN_ROAD_2010;
		// �����з� - �Ϲ��� �Ѱ���¿����� ����
		long nSizeBorEx = m_pArrBoringEx.GetSize();
		for(long n=0; n<nSizeBorEx; n++)
		{
			CBoringDataEx *pBorEx = m_pArrBoringEx.GetAt(n);
			long nSizeBor = pBorEx->m_pArrBoring.GetSize();
			for(long nB=0; nB<nSizeBor; nB++)
			{
				CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(nB);
				if(pBor->m_nTypeSoil == 2)
				{
					pBor->m_nTypeSoil = 0; // ������
				}
			}
		}
	}

	long icBridge = GetBridgeSize();
	for(long ix=0; ix<icBridge; ++ix)
	{
		CRcBridgeRebar *pBri = GetBridgeRebar(ix);
		if(pBri == NULL) continue;

		// ����ȯ�� �������� �⺻��.
		if(bLSD)
		{
			long icJijum = pBri->GetCountJijum();
			for(long nJ=0; nJ<icJijum; ++nJ)
			{
				CFootingApp* pFoot = pBri->GetFooting(nJ);
				if(pFoot == NULL) continue;

				if(pBri->IsPileBase(nJ))
				{
					pFoot->m_nTypeEquation_Qa = 0;
				}
			}

			pBri->SetDataDefaultCondition();
			
			pBri->SetDataDefaultExposureRatingLsd();
			pBri->SetDataDefaultDesignRatingLsd();

			pBri->SetDataDefaultSelectLoadLsd();
			pBri->SetDataDefaultLoadFactor();
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE);
			pBri->SetDataDefaultModifiedFactor(-1);

			pBri->SetDataDefaultLoadFactor(FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT, FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME, FALSE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE, FALSE);

			if (pBri->m_nUnderGroundDesignSelect == 4) // �����������(��)�� �Ѱ���¼�������� ������� �ʱ�� ��
			{
				pBri->m_nUnderGroundDesignSelect = 3;
			}

			pBri->m_bPileNvalueAuto = FALSE;
		}
		else
		{
			pBri->SetDataDefaultSelectLoad();
			pBri->SetDataDefaultCombineUltLoad();
			pBri->SetDataDefaultCombineUseLoad();
			pBri->SetDataDefaultCombineEccLoad();
			pBri->SetDataDefaultCombineEQLoad();
		}

		// (ARCBRIDGE-2489) ���� ������� ���ݹݷ°�� �ε����� ���� �ٸ��� ������ ���� ���� ������.
		long nCntFoot = pBri->GetCountFooting();
		for(long ix=0; ix<nCntFoot; ++ix)
		{
			CFootingApp *pFoot = pBri->GetFooting(ix);
			if(pFoot == NULL) continue;

			for(long jx=0; jx<PILE_CASE_COUNT; ++jx)
				pFoot->m_exFooting.m_Pile.m_dKh_Input_Pile[jx] = 0.0;
		}
	}
}

void CARcBridgeDataStd::SetDataDefaultMatResistFactor()
{
	CLsdResistCoefMatCtl *pResistMat = &(m_plsdEnv->GetLsdResistCoefMatCtl());
	if(!pResistMat) return;

	m_plsdEnv->SetDataDefaultResistCoefMat();

	pResistMat->InitResistCoef();

	m_plsdEnv->GetLsdResistCoefMatCtl().SetUsed(eLSD_COMBINE_STRESS, FALSE);
}

void CARcBridgeDataStd::SetDataDefaultBaseResistFactor()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());
	if(!pResistCoefCtl) return;

	m_plsdEnv->SetDataDefaultResistCoefBase();

	pResistCoefCtl->InitResistCoefUsed_NotUsedAll();
	for(long ix = 0 ; ix <= eRESISTCOEF_AxialCompress_Composite ; ++ix)
	{
		pResistCoefCtl->SetUsed(ix, TRUE);
	}

	for(long ix = eRESISTCOEF_IntruStateBad ; ix <= eRESISTCOEF_Activity_Sandy ; ++ix)
	{
		pResistCoefCtl->SetUsed(ix, TRUE);
	}

	pResistCoefCtl->SetUsed(eRESISTCOEF_Activity_Rocky, TRUE);

	if(m_nTypeCheckSteelPile==1)
	{
		SetDataDefaultBaseResistFactor_SteelPile();
	}
}

void CARcBridgeDataStd::SetDataDefaultBaseResistFactor_SteelPile()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());
	if(!pResistCoefCtl) return;

	CLsdResistCoefBase *pStateBad = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_IntruStateBad);
	CLsdResistCoefBase *pStateGood = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_IntruStateGood);
	CLsdResistCoefBase *pStatePipe = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_NonDrivenPile_Pipe);
	CLsdResistCoefBase *pStateFlexure = pResistCoefCtl->GetResistCoefPAt(eRESISTCOEF_NonDrivenPile_Flexure);

	BOOL b2014 = m_nTypeCheckSteelPile==1 ? TRUE : FALSE;

	pStateBad->SetUltimatelimit(b2014 ? 0.9 : 0.6);
	pStateGood->SetUltimatelimit(b2014 ? 0.9 : 0.7);
	pStatePipe->SetUltimatelimit(b2014 ? 0.9 : 0.8);
	pStateFlexure->SetUltimatelimit(b2014 ? 0.9 : 1.0);

	pStateBad->SetRemark(b2014 ? _T("��.��(2014) 5.1") : _T("��.��(2015) 6.5.4.2"));
	pStateGood->SetRemark(b2014 ? _T("��.��(2014) 5.1") : _T("��.��(2015) 6.5.4.2"));
	pStatePipe->SetRemark(b2014 ? _T("��.��(2014) 7.1.1") : _T("��.��(2015) 6.5.4.2"));
	pStateFlexure->SetRemark(b2014 ? _T("��.��(2014) 7.1.1") : _T("��.��(2015) 6.5.4.2"));

	pStateFlexure->SetCategory2(b2014 ? _T("�ڿ� ���� (��b)") : _T("�ڿ� ���� (��f)"));
}

// BOOL blong : N/�� �� 4�ڸ� N/���� 6�ڸ��� ǥ���ϰ� ������ ���� ���� ��� ���� �� ǥ�� ���ַ��� 6�ڸ� 9�ڸ� ���� ǥ�� �ؾ� �Ѵ�. �Ϻ� ���� ������� �������.
CString CARcBridgeDataStd::GetStrDouble( double dValue, CString sUnitCGS, BOOL bLong, long nDec/*=-1*/, BOOL bDf, long nSpace, BOOL bExponent )
{
	BOOL bLsd = IsLsdDesign();

	if(bExponent == FALSE)
	{
		nDec = GetDecimal(sUnitCGS, nDec, bLong);
		if(bLsd && nDec == -1)
		{
			bExponent = TRUE;
		}
	}

	if(bExponent == TRUE)
	{
		nDec = 0;
		bDf = FALSE;
	}

	CString szRet = ::GetStrDouble(dValue, nDec, bLsd, bDf, bExponent);
	
	if(nSpace > 0)
	{
		if(nDec > 0)
			++nSpace;
		CString szCh(_T(""));
		long nLen = szRet.GetLength();
		for(long ix=0; ix< nSpace-nLen; ++ix)
			szCh += _T(" ");
		
		szRet = szCh + szRet;;
	}

	return szRet;
}

CString CARcBridgeDataStd::GetStrDoubleUnitChange( double dValue, CString csUnitOrg, CString csUnitChg, BOOL bLong, long nDec/*=-1*/, BOOL bDf, long nSpace, BOOL bExponent )
{
	double dValUnit = GetValueUnitChange(dValue, csUnitOrg, csUnitChg);

	return GetStrDouble(dValUnit, csUnitOrg, bLong, nDec, bDf, nSpace, bExponent);
}

long CARcBridgeDataStd::GetDecimal(CString sUnitCGS, long nDec, BOOL bLong)
{
	BOOL bLsd = IsLsdDesign();

	if(nDec < 0 || bLsd)
	{
		if(sUnitCGS == UNIT_CGS_TONF || sUnitCGS == UNIT_CGS_M || sUnitCGS == UNIT_CGS_TONFM || sUnitCGS == UNIT_CGS_M2)
			nDec = bLsd? 0 : 3;
		else if (sUnitCGS == UNIT_CGS_TONF_M)
			nDec = 3;//bLsd? 3 : 3;
		else if(sUnitCGS == UNIT_CGS_TONF_M2)
			nDec = bLsd? (bLong? 6 : 4) : 3;
		else if(sUnitCGS == UNIT_CGS_TONF_M3 || sUnitCGS == UNIT_CGS_KGF_CM3)
			nDec = bLsd? (bLong? 7 : 6) : 3;
		else if (sUnitCGS == UNIT_CGS_KGF_CM2 || sUnitCGS == UNIT_CGS_KGF_CM2)
			nDec = bLsd? (bLong? 6 : 4) : 3;
		else if (sUnitCGS == UNIT_CGS_KGF_M3)
			nDec = bLsd? (bLong? 7 : 6) : 3;
		else if (sUnitCGS == UNIT_CGS_M4)
			nDec = bLsd? -1 : 6;
		else
			nDec = 0;
	}	
	
	return nDec;
}

CString CARcBridgeDataStd::GetStringLoadDB( BOOL bDL/*=FALSE*/ )
{
	CString szDB(_T(""));
	BOOL bLsd = IsLsdDesign();
	if(bLsd)
	{
		szDB = _T("KL-510");
		if(m_nDegreeBridge==1)
		{
			szDB = _T("KL-382.5");
		}
		if(m_nDegreeBridge==2)
		{
			szDB = _T("KL-286.87");
		}
	}
	else
	{
		szDB = bDL? _T("DL-24") : _T("DB-24");
		if(m_nDegreeBridge==1)
		{
			szDB = bDL? _T("DL-18") : _T("DB-18");
		}
		if(m_nDegreeBridge==2)
		{
			szDB = bDL? _T("DL-13.5") : _T("DB-13.5");
		}
	}

	return szDB;
}

CString CARcBridgeDataStd::GetStringExcelFileForm()
{
	CXLControl excel;
	double dVersion = excel.GetVersion();

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel files (*.xls)|*.xls||") : _T("Microsoft Excel files (*.xlsx)|*.xlsx||");

	return sFileForm;
}

BOOL CARcBridgeDataStd::IsApplyBoredPileASD(CExPileApp *pPile)
{
	if(m_nApplyBoredPileASD == 0) return FALSE;

	if(pPile)
	{
		return pPile->IsBoredPile();
	}
	else
	{
		// pPile�� �������� �ʾ������� ���ú����� üũ �ϱ� ������ TRUE�ش�.
		return TRUE;
	}
}

BOOL CARcBridgeDataStd::IsApplyPhcPileASD( CExPileApp *pPile /*= NULL*/ )
{
	if(m_nApplyPhcPileASD == 0) return FALSE;

	if(pPile)
	{
		return pPile->IsPHC();
	}
	else
	{
		return TRUE;
	}
}

//nBri == -1(��� ����), nBri
void CARcBridgeDataStd::SyncApsDowelRebarSeperateRebarOption(BOOL bSet,  long nBri)
{
	if(nBri == -1)
	{
		long nCntBirSize = GetBridgeSize();
		for(long nBri =0; nBri < nCntBirSize; nBri++)
		{
			CRcBridgeApp *pBriApp = GetBridge(nBri);
			if(pBriApp == NULL) continue;

			pBriApp->m_bApsDowelSeparateRebar = bSet;
		}
	}
	else
	{
		CRcBridgeApp *pBriApp = GetBridge(nBri);
		if(pBriApp)
			pBriApp->m_bApsDowelSeparateRebar = bSet;
	}	
}

void CARcBridgeDataStd::SeparateRebarListByDescription(CTypedPtrArray <CObArray, CRebar*> *pArrRebar1, CTypedPtrArray <CObArray, CRebar*> *pArrRebar2, CString sDescription)
{
	if(!pArrRebar1 || !pArrRebar2)	return;

	if(pArrRebar1->GetSize() == 0)	return;
	if(sDescription == _T(""))	return;

	CRebar *pRB = NULL;
	for(long i = 0; i < pArrRebar1->GetSize(); i++)
	{
		pRB = pArrRebar1->GetAt(i);

		if(pRB->m_sDescription.Find(sDescription) == -1)
			continue;

		pArrRebar2->Add(new CRebar(pRB));

		delete pRB;
		pArrRebar1->RemoveAt(i);
		i--;
	}
}

void CARcBridgeDataStd::SetFooting_Qa_Pa__Calculate()
{
	CLsdResistCoefBaseCtl *pResistCoefCtl = &(m_plsdEnv->GetLsdResistCoefBaseCtl());

	ECoefficientBaseIndex eIndex[3] = {eResistCoefficient_Service, eResistCoefficient_Ultimate, eResistCoefficient_Extreme};

	for(long nBri=0; nBri < GetBridgeSize(); nBri++)
	{
		CRcBridgeApp *pBri = GetBridge(nBri);
		if(pBri == NULL) continue;

		for(long nJijum = 0; nJijum <= pBri->m_nQtyJigan; nJijum++)
		{
			CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
			if(pFooting == NULL) continue;

			for(long nCase=0; nCase<3; nCase++)
			{
				double piR = 1.0, piU = 1.0;
				if(pBri->IsPileBase(nJijum))
				{
					CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
					if(pPile->m_nType == PILE_TYPE_HYUNJANG)
					{
						CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
						if(pBoring)
						{
							piR = pResistCoefCtl->GetCoefficient((pBoring->m_nTypeSoil == 0) ? eRESISTCOEF_SinglePileCompress_Drill_Support_Sandy : eRESISTCOEF_SinglePileCompress_Drill_Support_Rock, eIndex[nCase]);
						}
						piU = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Drill, eIndex[nCase]);
					}
					else// Ÿ�Ը���
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePileCompress_Type, eIndex[nCase]);
						piU = pResistCoefCtl->GetCoefficient(eRESISTCOEF_SinglePilePullout_Type, eIndex[nCase]);
					}
				}
				else
				{
					if(pFooting->m_nTypeBaseGround != 2)
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Bearing_Sandy, eIndex[nCase]);	// (��b)	������, SPT ���
					}
					else
					{
						piR = pResistCoefCtl->GetCoefficient(eRESISTCOEF_Bearing_Rock, eIndex[nCase]);	// (��b)	�Ϲ����� ��ġ�� ����
					}
				}
				pFooting->m_Qa_Calculate[nCase] = pFooting->m_Qa_Calculate[0] * piR;
				pFooting->m_Pa_Calculate[nCase] = pFooting->m_Pa_Calculate[0] * piU;
			}
		}
	}
}

BOOL CARcBridgeDataStd::IsKRL_2012()
{
	if(IsRailDesign2017() == FALSE) return FALSE;

	if(m_nTypeRailLoad==RAIL_LOAD_EL18_2017) return FALSE;

	if(m_nTypeRailLoad==RAIL_LOAD_KRL2012) return TRUE;
	if(m_nTypeRailLoad==RAIL_LOAD_KRL2012_P) return TRUE;

	if((m_nTypeRailLoad==RAIL_LOAD_USER_2017 && m_DistributionLoadFront>0))
		return TRUE;

	return FALSE;
}

CString CARcBridgeDataStd::GetStringBridgeDir(long nBri)
{
	CRcBridgeApp *pBri	= GetBridge(nBri);
	if(!pBri) return _T("�������");

	CString strBridgeDir = _T("");
	if(pBri->m_nSeparBri==0)
		strBridgeDir = m_strLineDirName + m_strLineRevDirName;
	else if(pBri->m_nSeparBri==1)
		strBridgeDir = m_strLineDirName;
	else if(pBri->m_nSeparBri==2)
		strBridgeDir = m_strLineRevDirName;

	return strBridgeDir;
}

CString CARcBridgeDataStd::GetStringTypeCoefficientGround( long nType )
{
	CString szType = _T("");

	switch (nType)
	{
	case 0:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("��") : _T("S1");	break;
	case 1:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("��") : _T("S2");	break;
	case 2:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("��") : _T("S3");	break;
	case 3:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("��") : _T("S4");	break;
	case 4:
		szType = m_nTypeEarthQuakeApply == 0 ? _T("") : _T("S5");	break;
	default:
		szType = _T("");
	}

	return szType;
}

long CARcBridgeDataStd::GetTypeConcDesign()
{
	long nType = IsLsdDesign() ? DESIGN_TYPE_LSD : DESIGN_TYPE_LIMIT;
	if(IsDesignKDS2019())
	{
		nType = IsLsdDesign() ? DESIGN_TYPE_LSD_KDS_2019 : DESIGN_TYPE_LIMIT_KDS_2016;
	}

	return nType;
}
