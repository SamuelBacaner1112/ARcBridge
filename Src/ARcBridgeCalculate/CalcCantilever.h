#pragma once

struct SVehicleCalc
{
	double dMl;
	double dMtank;
	double dMTrail;
	double dPr;

	double DisFromWallTank;
	double dTankW;
	double dL2;
	double dP2;

	double DisFromWallTrailer;
	double dTrailerW;
	double dL3;
	double dE3;
	double dP3;

	SVehicleCalc()
	{
		dMl = 0;
		dMtank = 0;
		dMTrail = 0;
		dPr = 0;

		DisFromWallTank = 0;
		dTankW = 0;
		dL2 = 0;
		dP2 = 0;

		DisFromWallTrailer = 0;
		dTrailerW = 0;
		dL3 = 0;
		dE3 = 0;
		dP3 = 0;
	}
};

struct SCollisionCalc
{
	double dX;
	double dV;
	double dY;
	double dH;
	double dY2;
	double dH2;
	double dLoadPos;

	SCollisionCalc()
	{
		dX = 0;
		dV = 0;
		dY = 0;
		dH = 0;
		dY2 = 0;
		dH2 = 0;
		dLoadPos = 0;
	}
};

struct SCentrifugalCalc
{
	double dL1;
	double dL3;
	double dE1;
	double dE3;	
	double dHeight;
	double dPr1;
	double dPr2;
	double dPr3;
	double dCf1;
	double dCf2;
	double dCf3;
	double dPcf1;
	double dPcf2;
	double dPcf3;
	double dMcf1;
	double dMcf2;
	double dMcf3;

	SCentrifugalCalc()
	{
		dL1 = 0;
		dL3 = 0;
		dE1 = 0;
		dE3 = 0;	
		dHeight = 0;
		dPr1 = 0;
		dPr2 = 0;
		dPr3 = 0;
		dCf1 = 0;
		dCf2 = 0;
		dCf3 = 0;
		dPcf1 = 0;
		dPcf2 = 0;
		dPcf3 = 0;
		dMcf1 = 0;
		dMcf2 = 0;
		dMcf3 = 0;
	}
};

enum ECheckPosition {
	eCantilever_Left = 0,
	eCantilever_Right
};

enum eSlabLoadType{eSlabDC = 0, eSlabDW};

#define _1000MM	1000.0

#define VEHICLE_FACTOR_E1	0.8
#define VEHICLE_FACTOR_E2	1140.0

#define STR_LOAD_CONCRETE _T("��ü����")
#define STR_LOAD_SLAB _T("������")
#define STR_LOAD_GUARDWALL _T("��ȣ��")
#define STR_LOAD_PAVEMENT _T("��������")
#define STR_LOAD_FOOTWAY _T("��������")
#define STR_LOAD_CROWD _T("��������")
#define STR_LOAD_BANGUM _T("������")
#define STR_LOAD_NANGAN _T("����")
#define STR_LOAD_VEHICLE _T("��������")
#define STR_LOAD_COLLISION_VEHICLE _T("�����浹����")
#define STR_LOAD_COLLISION_CROWD _T("���߿� ���� �߷� �Ǵ� ������ 2�� �浹����")
#define STR_LOAD_WIND _T("ǳ����")
#define STR_LOAD_CENTRIFUGAL _T("��������")

#include "CantileverCheckPosition.h"

//class CSlabUnitLoad;
class AFX_EXT_CLASS CCalcCantilever
{
public:
	CCalcCantilever(void);
	~CCalcCantilever(void);

public:
	CCantileverCheckPosition *GetCheckPosition(HGBOOL bLeft);
	BOOL IsCalcExperienceDesign();

	// Calc
	void CalculateAll();

	void CalculateCheckPosition();
	HGBOOL GetValidCheckCantilever(HGBOOL bLeft);
	void MakeAreaConc(BOOL bLeft);
	void GetTvarrayGuardWall(CTwinVectorArray &tvArr, BOOL bLeft);
	void GetTvarrayCantilever(CTwinVectorArray &tvArr, BOOL bLeft);

	CARcBridgeCalcStd *m_pStd;
private:
	double m_dUnitWeightSteelConc;		// ö�� ��ũ��Ʈ �����߷�

	CCantileverCheckPosition *m_pCantileverCheck[2];		// [��,��]
	vector<CTwinVectorAreaShape*> m_vArea[2][2]; //DC,DW  LEFT,RIGHT
	SVehicleCalc sVC[2];			// ����������
	SVehicleCalc sVC_Center;
	SCollisionCalc sCO[2];
	SCentrifugalCalc sCF[2];

	BOOL m_bCollisionVehicle[2];

private:
	stCantileverLoadUnit* GetUnitLoadConcreteDC(HGBOOL bLeft);			// ��ȣ��, ������
	stCantileverLoadUnit* GetUnitLoadConcreteDW( HGBOOL bLeft );		 
	stCantileverLoadUnit* GetUnitLoadFootWay(HGBOOL bLeft);			// ��������
	stCantileverLoadUnit* GetUnitLoadCrowd(HGBOOL bLeft);				// ��������
	stCantileverLoadUnit* GetUnitLoadUpperStructure(HGBOOL bLeft, long nType);		// ����, ������
	stCantileverLoadUnit* GetUnitLoadPavement(HGBOOL bLeft);					// ��������
	stCantileverLoadUnit* GetUnitLoadVehicle(HGBOOL bLeft);					// ��������
	stCantileverLoadUnit* GetUnitLoadCollision(HGBOOL bLeft);					// �浹����
	stCantileverLoadUnit* GetUnitLoadWind(HGBOOL bLeft);						// ǳ����
	stCantileverLoadUnit* GetUnitLoadCentrifugal(HGBOOL bLeft);					// ��������

	void MakeListCombo(CCantileverCheckPosition *pCheckPosition);

	void MakeCantileverCheckPosition();
	void MakeListLoadCantilever(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft);
	double GetDistCantilever(HGBOOL bLeft);
	CDPoint GetXyHDanByType(HGBOOL bLeft, long nHDanType=4);		// ������ �⺻������ ����.

	BOOL IsSelectedLoad(ELSDLoadSelectKind eKind, BOOL bDeckSlab);
	double GetLengthSupport(BOOL bLeft, BOOL bFootWay=FALSE);
	double CalcCentriodLoad_CF(double V, double R);

	void SetConcBasicCheckPosition(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft);
	CConcBasic *GetConcBasicData(HGBOOL bLeft, ELSDLoadGroupKind eGroupKind);

public:
	void WriteCantilever( CXLFormatText *pXLText, long nColPos, HGBOOL bLeft, long nIdxTitle, long nIdxSubTitle );			// ĵƿ���� ���
	void WriteLoadConcrete(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	// ��ȣ��(�ߺд�),������
	void WriteLoadPavement(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	// ����
	void WriteLoadFootWay(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // ��������
	void WriteLoadCrowd(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // ��������
	void WriteLoadUpperStructure(CXLFormatText *pXLText, long &nIdxMiddle, long nType, HGBOOL bLeft); // ����,������
	void WriteLoadVehicle(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // Ȱ����(��������)
	void WriteLoadWind(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // ǳ����
	void WriteLoadCollision(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // �浹����
	BOOL WriteLoadCentrifugal(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // �������� : Ⱦ�д뿡���� ���� ���

	//����ǥ ���
	void WriteCombineTotal(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//�������� 
	void WriteTotalTable(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//��������
	void WriteComboTable(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//��������

	// �β� ������
	void WriteMinThickSlab(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	// �ܸ����
	void WriteRebarCheck(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	void WriteRebarUsability(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	// ���ö��,�ܺ�,ó��
	void WriteSupportRebar(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	void WriteDeflectionCheck(CXLFormatText *pXLText, HGBOOL bLeft);

	double DrawGuardWall(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC* pGW, HGBOOL bLeft );
};

