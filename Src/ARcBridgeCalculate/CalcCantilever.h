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

#define STR_LOAD_CONCRETE _T("구체자중")
#define STR_LOAD_SLAB _T("슬래브")
#define STR_LOAD_GUARDWALL _T("방호벽")
#define STR_LOAD_PAVEMENT _T("포장하중")
#define STR_LOAD_FOOTWAY _T("보도하중")
#define STR_LOAD_CROWD _T("군중하중")
#define STR_LOAD_BANGUM _T("방음벽")
#define STR_LOAD_NANGAN _T("난간")
#define STR_LOAD_VEHICLE _T("차량하중")
#define STR_LOAD_COLLISION_VEHICLE _T("차량충돌하중")
#define STR_LOAD_COLLISION_CROWD _T("군중에 의한 추력 또는 차량의 2차 충돌하중")
#define STR_LOAD_WIND _T("풍하중")
#define STR_LOAD_CENTRIFUGAL _T("원심하중")

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
	double m_dUnitWeightSteelConc;		// 철근 콘크리트 단위중량

	CCantileverCheckPosition *m_pCantileverCheck[2];		// [좌,우]
	vector<CTwinVectorAreaShape*> m_vArea[2][2]; //DC,DW  LEFT,RIGHT
	SVehicleCalc sVC[2];			// 차량윤하중
	SVehicleCalc sVC_Center;
	SCollisionCalc sCO[2];
	SCentrifugalCalc sCF[2];

	BOOL m_bCollisionVehicle[2];

private:
	stCantileverLoadUnit* GetUnitLoadConcreteDC(HGBOOL bLeft);			// 방호벽, 슬래브
	stCantileverLoadUnit* GetUnitLoadConcreteDW( HGBOOL bLeft );		 
	stCantileverLoadUnit* GetUnitLoadFootWay(HGBOOL bLeft);			// 보도하중
	stCantileverLoadUnit* GetUnitLoadCrowd(HGBOOL bLeft);				// 군중하중
	stCantileverLoadUnit* GetUnitLoadUpperStructure(HGBOOL bLeft, long nType);		// 난간, 방음벽
	stCantileverLoadUnit* GetUnitLoadPavement(HGBOOL bLeft);					// 포장하중
	stCantileverLoadUnit* GetUnitLoadVehicle(HGBOOL bLeft);					// 차량하중
	stCantileverLoadUnit* GetUnitLoadCollision(HGBOOL bLeft);					// 충돌하중
	stCantileverLoadUnit* GetUnitLoadWind(HGBOOL bLeft);						// 풍하중
	stCantileverLoadUnit* GetUnitLoadCentrifugal(HGBOOL bLeft);					// 원심하중

	void MakeListCombo(CCantileverCheckPosition *pCheckPosition);

	void MakeCantileverCheckPosition();
	void MakeListLoadCantilever(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft);
	double GetDistCantilever(HGBOOL bLeft);
	CDPoint GetXyHDanByType(HGBOOL bLeft, long nHDanType=4);		// 보도를 기본값으로 가짐.

	BOOL IsSelectedLoad(ELSDLoadSelectKind eKind, BOOL bDeckSlab);
	double GetLengthSupport(BOOL bLeft, BOOL bFootWay=FALSE);
	double CalcCentriodLoad_CF(double V, double R);

	void SetConcBasicCheckPosition(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft);
	CConcBasic *GetConcBasicData(HGBOOL bLeft, ELSDLoadGroupKind eGroupKind);

public:
	void WriteCantilever( CXLFormatText *pXLText, long nColPos, HGBOOL bLeft, long nIdxTitle, long nIdxSubTitle );			// 캔틸레버 출력
	void WriteLoadConcrete(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	// 방호벽(중분대),슬래브
	void WriteLoadPavement(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	// 포장
	void WriteLoadFootWay(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 보도하중
	void WriteLoadCrowd(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 군중하중
	void WriteLoadUpperStructure(CXLFormatText *pXLText, long &nIdxMiddle, long nType, HGBOOL bLeft); // 난간,방음벽
	void WriteLoadVehicle(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 활하중(차량하중)
	void WriteLoadWind(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 풍하중
	void WriteLoadCollision(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 충돌하중
	BOOL WriteLoadCentrifugal(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft); // 원심하중 : 횡분대에서도 같이 사용

	//집계표 출력
	void WriteCombineTotal(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//하중조합 
	void WriteTotalTable(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//하중집계
	void WriteComboTable(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);	//하중조합

	// 두께 적정성
	void WriteMinThickSlab(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	// 단면검토
	void WriteRebarCheck(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	void WriteRebarUsability(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	// 배력철근,단부,처짐
	void WriteSupportRebar(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft);
	void WriteDeflectionCheck(CXLFormatText *pXLText, HGBOOL bLeft);

	double DrawGuardWall(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC* pGW, HGBOOL bLeft );
};

