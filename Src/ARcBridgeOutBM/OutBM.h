// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutBM.h: interface for the COutBM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_)
#define AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��з�
#define BM_BIG_BOHOBLOCK		0	// ���뺸ȣ��(��ũ��Ʈ���� ���� �ִ� ���� : ��ȣ�� ��ä�� ��ũ��Ʈ = ��ȣ�� * ��ũ��Ʈ�β�)
#define BM_BIG_CONCRETE			1	// ��ũ��Ʈ
#define BM_BIG_FORM					2	// ��Ǫ��
#define BM_BIG_POST					3	// ���ٸ�
#define BM_BIG_DECKFINISH			4	//��ũ�Ǵϼ� (���ٸ�-�����ٻ��̿� ������, ARCBRIDGE-3132)
#define BM_BIG_DOWELBAR			5	// ������
#define BM_BIG_SCAFFOLD			6	// ���
#define BM_BIG_SPACER				7	// �����̼�
#define BM_BIG_SLABCARE			8	// ��������
#define BM_BIG_CONCCARE			9	// ��ũ��Ʈ���
#define BM_BIG_ASPHALTPROOF		10	// �ƽ���Ʈ ���
#define BM_BIG_REBAR				11	// ö�ٰ�������
#define BM_BIG_SURFACE				12	// �� ����
#define BM_BIG_ASCON				13	// �ƽ��� ����
#define BM_BIG_SAND					14	// �� ����
#define BM_BIG_PERMEABLE			15	// ������ ����
#define BM_BIG_TACKCOTTING		16	// �� ����
#define BM_BIG_SURFACEPROOF		17	// ���� ���
#define BM_BIG_ANTIDROPPING		18	// ���Ϲ� ����
#define BM_BIG_HYUNTA				19	// ����Ÿ������
#define BM_BIG_NAMEPANEL		20	// ������
#define BM_BIG_DESPANEL			21	// ������
#define BM_BIG_TBM					22	// TBM��ġ
#define BM_BIG_STYROFOAM			23	// ��Ƽ����
#define BM_BIG_FLEXIBLEJOINT		24	// ��������
#define BM_BIG_FLEXIBLEJOINT_COVER	25	// �������� ����
#define BM_BIG_SHRINKAGE			26	// ������ ��ũ��Ʈ
#define BM_BIG_COVERPLATE		27	// cover plate
#define BM_BIG_MONUMENT		28	// ������
#define BM_BIG_NOTCH				29	// NOTCH
#define BM_BIG_PILE					30	// ����
#define BM_BIG_SCAFFOLD_LEG		31	// ���ٸ� 
#define BM_BIG_RUBBLEFILL			32	// ����(Box) �⼮ä��
//#define BM_BIG_DECKFINISH			32	//��ũ�Ǵϼ�
#define BM_BIG_COUNT				33

// �ߺз�
#define BM_MID_CONCRETE_MAIN	0	// ��ũ��Ʈ (��ü, ������, ���ӽ�����)
#define BM_MID_CONCRETE_LEAN	1	// ��ũ��Ʈ (������ũ��Ʈ)
#define BM_MID_CONCRETE_MASS	2	// ��ũ��Ʈ (mass)
#define BM_MID_CONCRETE_BOHO	3	// ��ũ��Ʈ (��ȣ��)
#define	BM_MID_FORM_3TIMES		4	// �Ų��� ����
#define BM_MID_FORM_PATTERN		5	// ���� ��Ǫ��
#define BM_MID_FORM_GANGJE		6	// ���� ��Ǫ��
#define	BM_MID_FORM_PATTERN_S	7	// ����� Ư¡�ִ� ���� (���̰�Ǫ��) 
#define BM_MID_FORM_GANGJE_S	8	// ����� Ư¡�ִ� ���� (�����Ǫ��)
#define BM_MID_FORM_COTTING_S	9	// ����� Ư¡�ִ� ���� (���ð�Ǫ��)
#define BM_MID_FORM_DECK_S		10	// ����� Ư¡�ִ� ���� (DECK PLATE)
#define BM_MID_FORM_ONE_S		11	// ����� Ư¡�ִ� ���� (1ȸ ���)
#define	BM_MID_FORM_CIRCLE		12	// ������Ǫ��
#define	BM_MID_FORM_4TIMES		13	// ���븶��
#define BM_MID_FORM_6TIMES		14	// ��ģ����
#define BM_MID_DOWELBAR			15	// ������
#define BM_MID_SCAFFOLD_MAIN	16	// ���(��ü)
#define BM_MID_SCAFFOLD_WING	17	// ���(������)
#define BM_MID_SLABCARE			18	// ������ ���
#define BM_MID_CONCCARE			19	// ��ũ��Ʈ���
#define BM_MID_REBAR			20	// ö�ٰ�������
#define BM_MID_ASCON			21	// �ƽ��� ����
#define BM_MID_TACK_RSC1		22	// �� ����(RSC-1)
#define BM_MID_SURFACEPROOF_PERCOLATE	23	// ������(ħ����)
#define BM_MID_ASPHALTPROOF		24	// �ƽ���Ʈ ���
#define BM_MID_STYROFOAM		25	// ��Ƽ����
#define BM_MID_FORM_EURO		26	// ������
#define BM_MID_FLEXIBLEJOINT	27	// ��������
#define BM_MID_SHRINKAGE		28	// ������ ��ũ��Ʈ
#define BM_MID_SCAFFOLD_WING_LEG	29	// ���ٸ�(������) 
#define BM_MID_SCAFFOLD_MAIN_LEG	30	// ���ٸ�(��ü) 
#define BM_MID_FLEXIBLEJOINT_COVER	31	// ������������
#define BM_MID_RUBBLEFILL	32	//����(Box) �⼮ä��
#define BM_MID_CONCRETE_GUIDEWALL	33	//��ȣ��
#define BM_MID_CONCRETE_GWHALF	34	//�ߺд�
#define BM_MID_CONCRETE_MONU	35	//��ũ��Ʈ ������

// ���⼭ ���� ��Ÿ ����
#define BM_MID_STEEL_INNER_DIGGING_BIG	32	// �������α���(�뱸��õ��)
#define BM_MID_STEEL_INNER_DIGGING_RCD	33	// �������α���(RCDõ��)
#define BM_MID_DIGGING_MACHINE_MOVE_DEPER	34	// ������� �̵��� ��ġ
#define BM_MID_PILE_BUILD	35				// ���� ������
#define BM_MID_SPACER_ALONE_HYUNTAPILE	36	// SPACER ���� �� ��ġ
#define BM_MID_REBAR_ANTI_RISE	37			// ö�� ���� ����
#define BM_MID_COPPING	38					// �κ� ����
#define BM_MID_REMAIN_SOIL	39				// ����ó��
#define BM_MID_REBAR_ASSM_AHP	40			// ö�ٰ�������
#define BM_MID_WASTE	41					// ��⹰ ó��
#define BM_MID_SLIME	42					// slime ����
#define BM_MID_CONCRETE_AHP	43				// concrete Ÿ��
#define BM_MID_SURFACE_AHP	44				// �� ����
#define BM_MID_REBAR_NET	45				// ö�ٸ� ����
// ������� ��Ÿ ����


// �Һз�
#define BM_SMALL_CONCRETE_FOOTING	0	// ��ũ��Ʈ (����)
#define BM_SMALL_CONCRETE_MAIN		1	// ��ũ��Ʈ (��ü)
#define BM_SMALL_CONCRETE_LEAN		2	// ��ũ��Ʈ (������ũ��Ʈ)
#define BM_SMALL_CONCRETE_MASS		3	// ��ũ��Ʈ (MASS ��ũ��Ʈ)
#define BM_SMALL_CONCRETE_WALL		4	// ��ũ��Ʈ (��ü)
#define BM_SMALL_CONCRETE_WING		5	// ��ũ��Ʈ (������)
#define BM_SMALL_CONCRETE_APS		6	// ��ũ��Ʈ (���ӽ�����)
#define BM_SMALL_CONCRETE_CNS		7	// ��ũ��Ʈ (���潽����)
#define BM_SMALL_CONCRETE_BOHO_FOOTING	8	// ��ũ��Ʈ (��ȣ�� ����)
#define BM_SMALL_CONCRETE_BOHO_BACKFILL	9	// ��ũ��Ʈ (��ȣ�� ��ä��)
#define BM_SMALL_FORM_VERTICAL		10	// ��Ǫ�� (������)
#define BM_SMALL_FORM_SLOPE			11	// ��Ǫ�� (����)
#define BM_SMALL_FORM_CURVE			12	// ��Ǫ�� (���)
#define BM_SMALL_FORM_HORI			13	// ��Ǫ�� (�����)
#define BM_SMALL_POST_STEELPIPE		14	// ���ٸ�(����)
#define BM_SMALL_POST_WOOD			15	// ���ٸ�(����)
#define BM_SMALL_POST_BRIDGE_STEEL	16	// ���ٸ�(������(����))
#define BM_SMALL_POST_HORI_CONNECT	17	// ���ٸ����򿬰���
#define BM_SMALL_DOWELBAR			18	// ������
#define BM_SMALL_SCAFFOLD_MAIN		19	// ���(��ü)
#define BM_SMALL_SCAFFOLD_WING		20	// ���(������)
#define BM_SMALL_SPACER_WALL		21	// �����̼�(��ü��)
#define BM_SMALL_SPACER_SLAB		22	// �����̼�(�������)
#define BM_SMALL_CONCCARE			23	// ��ũ��Ʈ���
#define BM_SMALL_ASPHALTPROOF		24	// �ƽ���Ʈ ���
#define BM_SMALL_REBAR_SIMPLE		25	// ö�ٰ�������(����)
#define BM_SMALL_REBAR_NORMAL		26	// ö�ٰ�������(����)
#define BM_SMALL_REBAR_COMPLEXITY1	27	// ö�ٰ�������(����)
#define BM_SMALL_REBAR_COMPLEXITY2	28	// ö�ٰ�������(�ſ캹��)
#define BM_SMALL_STYROFOAM			29	// ��Ƽ����
#define BM_SMALL_FLEXIBLEJOINT		30	// ��������
#define BM_SMALL_SHRINKAGE			31	// ������ ��ũ��Ʈ
#define BM_SMALL_SCAFFOLD_WING_LEG 32	// ���ٸ�(������)
#define BM_SMALL_SCAFFOLD_MAIN_LEG		33	// ���ٸ�(��ü)
#define BM_SMALL_FJ_NONE				34  // ����	
#define BM_SMALL_FJ_GUARDFENCE			35	// ��ȣ��
#define BM_SMALL_FJ_CENTERGUARD			36	// �ߺд�
#define BM_SMALL_FJ_CENTERGUARDHALF		37	// �ߺд�1/2
#define BM_SMALL_FJ_FOOTWAY				38	// ����
#define BM_SMALL_FJ_FOOTWAYBLOCK		39	// ������ ��輮
#define BM_SMALL_FJ_SOUNDPROOF			40	// ������ ����
#define BM_SMALL_FJ_RETAININGWALL		41	// �븷�̺�
#define BM_SMALL_FJ_HANDRAIL			42	// ��������
#define BM_SMALL_FJ_DIKE				43	// ����ũ
#define BM_SMALL_RUBBLEFILL				44	// ����(Box) �⼮ä��

#define BM_SMALL_CONCRETE_FLOOR			50	// �ٴ� �� ��

/*
#define BM_SMALL_REBAR1_10			21	// ö�ٰ�������(���� 10)
#define BM_SMALL_REBAR1_13			22	// ö�ٰ�������(���� 13)
#define BM_SMALL_REBAR1_1625		23	// ö�ٰ�������(���� 16~25)
#define BM_SMALL_REBAR1_2935		24	// ö�ٰ�������(���� 29~35)
#define BM_SMALL_REBAR2_10			25	// ö�ٰ�������(�� 10)
#define BM_SMALL_REBAR2_13			26	// ö�ٰ�������(�� 13)
#define BM_SMALL_REBAR2_1625		27	// ö�ٰ�������(�� 16~25)
#define BM_SMALL_REBAR2_2935		28	// ö�ٰ�������(�� 29~35)
*/

// ������ �з�
#define BM_ITEM_BOHOBLOCK_SF		0	// ��ȣ��(��������)
#define BM_ITEM_BOHOBLOCK_SS		1	// ��ȣ��(��������)
#define BM_ITEM_BOHOBLOCK_EF		2	// ��ȣ��(��������)
#define BM_ITEM_BOHOBLOCK_ES		3	// ��ȣ��(��������)
#define BM_ITEM_CONCRETE_FOOTING	4	// ��ũ��Ʈ (����)
#define BM_ITEM_CONCRETE_MAIN		5	// ��ũ��Ʈ (��ü)
#define BM_ITEM_CONCRETE_GUARDWALL	6	// ��ũ��Ʈ (��ȣ��)
#define BM_ITEM_CONCRETE_WING_SL	7	// ��ũ��Ʈ (��������������)
#define BM_ITEM_CONCRETE_WING_SR	8	// ��ũ��Ʈ (��������������)
#define BM_ITEM_CONCRETE_WING_EL	9	// ��ũ��Ʈ (��������������)
#define BM_ITEM_CONCRETE_WING_ER	10	// ��ũ��Ʈ (��������������)
#define BM_ITEM_CONCRETE_APS 		11	// ��ũ��Ʈ (���ӽ�����)
#define BM_ITEM_FORM_WING_SLF		12	// ��Ǫ�� (�������������� ����)
#define BM_ITEM_FORM_WING_SLB		13	// ��Ǫ�� (�������������� ���)
#define BM_ITEM_FORM_WING_SRF		14	// ��Ǫ�� (�������������� ����)
#define BM_ITEM_FORM_WING_SRB		15	// ��Ǫ�� (�������������� ���)
#define BM_ITEM_FORM_WING_ELF		16	// ��Ǫ�� (�������������� ����)
#define BM_ITEM_FORM_WING_ELB		17	// ��Ǫ�� (�������������� ���)
#define BM_ITEM_FORM_WING_ERF		18	// ��Ǫ�� (�������������� ����)
#define BM_ITEM_FORM_WING_ERB		19	// ��Ǫ�� (�������������� ���)

class CBMData;
class CBMString;
class CBMSumString;
class CRebarBMData;
class CRebarBMString;
class CRcBridgeRebar;
class CGuardWallRC;

#define MAX_JIJUM_COUNT_RC 21
#define CON_DECKFINISHER 0.585 

const long nMaxQtyHdan = 30;	// MAX_QTY_HDAN
const long nMaxQtyHyunta = 100;	// MAX_QTY_Hyunta

class AFX_EXT_CLASS COutBM : public COutBMBase  
{
public:
	void MakeBMFormGuardWall();
	void DimTextFormOfSlabELCalc(CDomyun *pDom);
	void MakeBMNotch();
	void MakeBMMonument();
	long DrawFirstPageMonument(CHgBaseBMStd *pBaseBMStd);
	void MakeBMFormMonument();
	void MakeBMConcreteMonument();
	void MakeBMCoverPlate();
	void MakeBMNonShrinkageConc();
	void MakeBMFlexibleJoint();
	void MakeBMFlexibleJointCover();	//�������� ����
	void DimTextLengthOfGuardWall(CDomyun *pDomP, CGuardWallRC *pGW, long nIdx, long nHDan);
	void DimTextArchRibConcAndForm(CDomyun *pDomP);
	void DimTextPostAreaOfArch(CDomyun *pDomP);
	double GetElBaseForForm(long nJ);
	void MakeBMAsphaltProof();
	void MakeBMRebar();
	void MakeBMRebarTable();
	long GetFormByPos(long nPos);
	// �Լ� ���� DataStd�� �̵�
	// void MakeRcBridgeForBM(BOOL bForTogong=FALSE);	// ������ ���ϱ� ���� ���ο� ���� ����
	void MakeBMPostWing();
	void MakeBMConcBohoBackFill();
	void MakeBMBohoBlock();
	void MakeBMConcBohoFooting();
	// ���� ����
	void MakeBMConcreteMain(BOOL bForDeductOfReFill=FALSE);		// ��ũ��Ʈ(��ü)
	void MakeBMConcreteGuardWall();								// ��ũ��Ʈ(��ȣ��)
		void MakeBMConcreteAps(BOOL bForDeductOfBackFill=FALSE, CTwinVector tvBackFillStt=CTwinVector(CVector(0, 0, 0), CVector(0, 0, 0)), CTwinVector tvBackFillEnd=CTwinVector(CVector(0, 0, 0), CVector(0, 0, 0)));	// ��ũ��Ʈ(���ӽ�����)
	void MakeBMFormMain();			// ��Ǫ��(��ü)
	void MakeBMPostMain();			// ���ٸ�(��ü)
	void MakeBMConcreteWing(long nForUse=0);		// ��ũ��Ʈ(������)
	void MakeBMHoriConnectMain();	// ���ٸ����򿬰���(��ü)
	void MakeBMHoriConnectWing();	// ���ٸ����򿬰���(������)
	void MakeBMDowelBar();			// ������(���ӽ�����)
	void MakeBMScaffoldMain();		// ���(��ü)
	void MakeBMScaffoldWing();		// ���(������)
	void MakeBMPile();
//	void MakeBMAntiDropping();
	void MakeBMSlabCare();
	void MakeBMSurfaceProof();
	void MakeBMTackCotting();
	void MakeBMAscon();
	void MakeBMSurface();
	void MakeBMRebar(long nType);
	void MakeBMConCareMain();
	void MakeBMConCareWing();
	void MakeBMSpacerMain();
	void MakeBMSpacerWing();
	void MakeBMFormWing();
	void MakeBMConCareAps();
	void MakeBMSpacerAps();
	void MakeBMStyrofoamAps();
	void MakeBMFormAps();
	void MakeBMTBM();
	void MakeBMDesPanel();
	void MakeBMNamePanel();
	void SetBMRebar(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nTypeComplex, long bIncludeFlexibleJointRebar=FALSE, CString sNote=_T(""));
	void MakeBMScaffoldLegWing(); //31724 //���ٸ�(������)
	void MakeBMScaffoldLegMain(); //���ٸ� (��ü)
	void SetInsertDomyunStd();
	void MakeRubbleBM();//�⼮���� (�Ϲ� ����): 
	void MakeBMDeckFinisher(); //��ũ�Ǵϼ�
	
	void CalculateBM(BOOL bNewCreateBM = TRUE);
	void ClearBM(BOOL bRemainStdBM=FALSE);
	void MakeBMStringStd();
	void MergeBMStd();
	COutBM(long nBri);
	virtual ~COutBM();

	// ����
	DWORD m_dwCalBM;	// ����Ǵ� ������ ����

	// ������(�����ȵ� ���� ����)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeStd;			// ���뺻ü(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallStd[nMaxQtyHdan];		// ��ȣ��(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonumentStd;			// ������(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingStd[2][2];		// ���볯����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsStd[2][2];		// ���ӽ�����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsStd[2][2][2];	// ���潽����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoStd[2];			// ��ȣ��(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMHyuntaStd[nMaxQtyHyunta];	 // ����Ÿ��(���� �ȵȰ�)

	// ��¿�(������ ����)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridge;					// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBridge;			// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWall[nMaxQtyHdan];				// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringGuardWall[nMaxQtyHdan];		// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonument;				// ������(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringMonument;		// ������(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWing[2][2];				// ���볯����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringWing[2][2];		// ���볯����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMAps[2][2];				// ���ӽ�����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringAps[2][2];		// ���ӽ�����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCns[2][2][2];			// ���潽����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringCns[2][2][2];	// ���潽����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBoho[2];				// ��ȣ��(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBoho[2];		// ��ȣ��(���� �Ȱ�) - ����ٰ� ���


	// ����������ǥ�� ��µ� ����
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeForPartSum;		// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallForPartSum[nMaxQtyHdan];	// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMMonumentPartSum;				// ������(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingForPartSum[2][2];	// ���볯����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsForPartSum[2][2];	// ���ӽ�����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsForPartSum[2][2][2];	// ���潽����(���� �Ȱ�) - ����ٰ� ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoForPartSum[2];		// ��ȣ��(���� �Ȱ�) - ����ٰ� ���
	
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBridgeSum;				// ���뺻ü - ������ ����ǥ ��� 
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBridgeSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMGuardWallSum;			// ��ȣ�� - ������ ����ǥ ��� - ������ ����
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringGuardWallSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMWingSum;				// ���볯���� - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringWingSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMApsSum;					// ���ӽ����� - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringApsSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMCnsSum;					// ���潽���� - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringCnsSum;

	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoSum;				// ��ȣ�� - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBohoSum;

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAllSum;		// ���뺻ü(��ü+������+���ӽ�����) - ������ ����ǥ ��� 
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSum;					// ��������(���� �ȵȰ�) - ������ ����ǥ ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSumForTitle;			// ��������(���� �ȵȰ�) - ������ ����ǥ ��� - Ÿ��Ʋ��

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringSepBridgeSum;	// ������ �и� ���� ����ǥ ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSepBridgeSum;


	// ö������ǥ
	CBMRebarTable m_rebarTableBridge;							// ��ü ö�� ����ǥ
	CBMRebarTable m_rebarTableGuardWall[nMaxQtyHdan];		// ��ȣ�� ö������ǥ
	CBMRebarTable m_rebarTableMoument;						// ������ ö������ǥ
	CBMRebarTable m_rebarTableWing[2][2];						// ������ ö������ǥ
	CBMRebarTable m_rebarTableAps[2][2];							// ���ӽ����� ö������ǥ
	CBMRebarTable m_rebarTableCns[2][2][2];						// ���潽���� ö������ǥ
	CBMRebarTable m_rebarTableBoho[2];							// ��ȣ�� ö������ǥ
	CBMRebarTable m_rebarTableHuynta[nMaxQtyHyunta];		// ����Ÿ������ ö������ǥ

	CBMRebarTable m_rebarTableBridgeSum;			// ��ü �Ѱ� ö�� ����ǥ
	CBMRebarTable m_rebarTableGuardWallSum;		// ��ȣ�� �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableMoumentSum;		// ������ �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableWingSum;			// ������ �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableApsSum;				// ���ӽ����� �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableCnsSum;				// ���潽���� �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableBohoSum;			// ��ȣ�� �Ѱ� ö������ǥ
	CBMRebarTable m_rebarTableHuyntaSum;			// ����Ÿ������ �Ѱ� ö������ǥ

	CBMRebarTable m_rebarTableAll;				// �Ѱ� ö������ǥ

	CBMRebarTable m_rebarSepBridgeSum;			// ������ �и� ���� ����ǥ

	// ö������
	// D�� H�� ���� ������.
	// �ɼǿ� ���� ���� �� ���� ����.
	// 0 : D��, 1 : H��
	void SetRebarBMData(CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataD, CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataH, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sNameBigClass);
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBridgeStd[2];			// ���뺻ü(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMGuardWallStd[nMaxQtyHdan][2];		// ��ȣ��(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMMonument[2];			// ������(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMWingStd[2][2][2];		// ���볯����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMApsStd[2][2][2];		// ���ӽ�����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMCnsStd[2][2][2][2];	// ���潽����(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBohoStd[2][2];		// ��ȣ��(���� �ȵȰ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMHyuntaStd[nMaxQtyHyunta][2];		// ����Ÿ��(���� �ȵȰ�)

	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBridge[2];			// ���뺻ü(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMGuardWall[nMaxQtyHdan][2];			// ��ȣ��(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMMonumentStd[2];			// ������(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMWing[2][2][2];		// ���볯����(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMAps[2][2][2];			// ���ӽ�����(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMCns[2][2][2][2];		// ���潽����(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMBoho[2][2];			// ��ȣ��(���� �Ȱ�)
	CTypedPtrArray <CObArray, CRebarBMData*> m_pArrRebarBMHyunta[nMaxQtyHyunta][2];			// ����Ÿ��(���� �Ȱ�)

	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringBridge[2];			// ���뺻ü
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringGuardWall[nMaxQtyHdan][2];			// ��ȣ��
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringMonument[2];			// ������
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringWing[2][2][2];		// ���볯����
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringAps[2][2][2];			// ���ӽ�����
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringCns[2][2][2][2];		// ���潽����
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringBoho[2][2];			// ��ȣ��
	CTypedPtrArray <CObArray, CRebarBMString*> m_pArrRebarBMStringHyunta[nMaxQtyHyunta][2];			// ����Ÿ��

	CARcBridgeOutBMStd* m_pStd;

	// �� ��¿� ���� �ɼǵ� /////////////////////////////////////////
	double m_dScaleDim_VertSection;		// ���ܸ鵵 ������
	double m_dHeightText_VertSection;	// ���ܸ鵵 �ؽ�Ʈ ����
	////////////////////////////////////////////////////////////////////
	void DrawFirstPageMain(CHgBaseBMStd *pBaseBMStd, long nKindBM);
	long DrawFirstPageGuardWall(CHgBaseBMStd *pBaseBMStd, long nHDan, long nIdx);
	void DrawFirstPageWing(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft);
	void DrawFirstPageAps(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft);

	void CalAreaConc(CRcBridgeRebar *pBri, BOOL bForDeductOfReFill);
	void CalAreaConcWing(CRcBridgeRebar *pBri, long nForUse=0);

	CString GetStringBigClass(long nIdxBigClass);

private:
	void MakeBMFormBoho();

	void SetInsertDomyunScaffoldMain();
	
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> m_pArrAreaConc;	// ���Ƿ� ��� �� tvAreaShape

	long GetIdxSameBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CBMData *pBM, DWORD exceptionFlag);
	void GetInfoPosAreaConc(CTwinVectorAreaShape *pArea, long &nTypeLeft, long &nTypeRight);
	void GetInfoMeter(CBMData *pBM, double &dFirstMeter, double &dSecondMeter);
	BOOL IsPrintMeter(CBMData *pBM);
	void RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dFirstMeter, double dSecondMeter, long nIdxStt=-1, long nIdxEnd=-1, long nOpt=0);
	CString GetStringMidClass(long nIdxMidClass, CApsApp *pAps=NULL, long nTypeOfConcrete=0, CGuardWallRC *pGW=NULL, double dUserFck = 0);
	CString GetStringSmallClass(long &nIdxSmallClass, BOOL bStt=TRUE);
	CString GetStringItemClass(long nIdxItemClass);

	long GetLevelTotal(long nIdxBigClass);

	void SetInsertDomyunPlaneFooting();
	void SetInsertDomyunPost();
	void SetInsertDomyunCross();
	
	long GetTvAreaShapePostWing(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, long nBaseNum);
	long GetTvAreaShapePostBracket(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, long nBaseNum);
	long GetTvAreaShapePostJigan(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nJigan, long nBaseNum);
	long GetTvAreaShapeApsPlane(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, BOOL bLean=FALSE);
	long GetTvAreaShapeApsSection(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft);
	
	CRcBridgeRebar* GetRcBridgeRebar(BOOL bOrg=FALSE);
	
	void SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, BOOL bNewCreateBM = TRUE);
	void ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM);

	CBMData *CreateBMStyrotormForSide(CBMData *pBMForm);
	void AddBMStyroformBySide(CBMData *pBMForm, DWORD nSide);
	
	long m_nBri;
};

#endif // !defined(AFX_OUTBM_H__DE5B9B42_F72B_4DFE_81DE_4EFB5D7956FB__INCLUDED_)
