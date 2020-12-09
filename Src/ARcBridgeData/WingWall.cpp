// WingData.cpp: implementation of the CWingData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WingWall.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWingWall::CWingWall()
{
	m_bExist = FALSE;
	m_bRAngEnd = TRUE;
	m_nDirHunch = TRUE;
	m_dEL = 0.0;
	m_vAng = CDPoint(1, 0);
	m_nAttachPos	= 0;
	m_bSWHor	= FALSE;
	m_bAttachBraket	= TRUE;
	m_bMatchBottom	= FALSE;
	Clear();

	int i=0; for(i=0; i<2; i++)
	{
		for(long nDan = 0; nDan < 2; nDan++)
		{
			m_dDiaMainA[i][nDan] = 0;			// A���� ��ö�� ����[In or Out]
			m_dDiaMainB[i][nDan] = 0;			// B���� ��ö�� ����[In or Out]
			m_dDiaMainC[i][nDan] = 0;			// C���� ��ö�� ����[In or Out]
			m_dDiaMainD[i][nDan] = 0;			// D���� ��ö�� ����[In or Out]
			m_dDiaSupportA[i][nDan] = 0;		// A���� ���ö�� ����[In or Out]
			m_dDiaSupportB[i][nDan] = 0;		// B���� ���ö�� ����[In or Out]
			m_dDiaSupportC[i][nDan] = 0;		// C���� ���ö�� ����[In or Out]
			m_dDiaSupportD[i][nDan] = 0;		// D���� ���ö�� ����[In or Out]
		}
		m_nCountSlopeBlock[i] = 0;	// ������ ������κ����� ��� ö�� ���� ����
	}
	m_dDiaHuUpper = 0;			// ��ü ��� ��ġö�� ����
	m_dDiaHuLower = 0;			// ��ü �Ϻ� ��ġö�� ����
	m_dDiaHuFoot = 0;			// ���ʺ� ��ġö�� ����
	m_dDiaShearA = 0;			// A���� ����ö�� ����
	m_dDiaShearB = 0;			// B���� ����ö�� ����
	m_dDiaShearC = 0;			// C���� ����ö�� ����
	m_dDiaShearD = 0;			// D���� ����ö�� ����
	m_dSpaceShearA = 0;			// A���� ����ö�� ����
	m_dSpaceShearB = 0;			// B���� ����ö�� ����
	m_dSpaceShearC = 0;			// C���� ����ö�� ����
	m_dSpaceShearD = 0;			// D���� ����ö�� ����
	m_dStdVerCTC = 0;			// ǥ�� ���� ö�� ��� ����
	m_dMainCTCA = 0;			// A���� ��ö�� ��ġ ����
	m_dMainCTCB = 0;			// B���� ��ö�� ��ġ ����
	m_bSelMainCTCD = FALSE;		// D���� ��ö�� ������ �⺻ ������ 2���
	m_bUpperRebarSlope = FALSE;	// ������ ��ܺ� ��ö���� ���� ��ġ
	m_nCountShearBind = 0;		// ����ö�� ��ٽ� ��ö�� ���� ����
	m_nRebarCurve = 0;			// C���� ��ö�� ����� ó��(0:�ù漭, 1:����, 2:�)
	m_nHunchCTC = 0;			// ��ġ ö�� ����(0:����, 1:���)
	m_nTypeShearBC = 0;			// B, C ���� ����ö�ٹ�� ���� - 0: ����, 1: ����, 2: �����
	m_bAddRebarReinforce = FALSE;	// �亮����ö�� �߰�

	// �Ҵ� �Է�
	m_dDistNoriStart	= 0;
	m_dSlopeNoriFirst	= 0;
	m_dSlopeNoriNext	= 0;
	m_dHeightMaxSodan	= 0;
	m_dSlopeSodan		= 0;
	m_dWidthSodan		= 0;
	m_dWidthSodanWallFront	= 0;
	m_dElSodanBottom	= 0;
	m_bRetainWallFront	= 0;
	m_dHeightSoil		= 0;

	m_bUseUserOutput3D = FALSE;
	for(long ix = 0; ix < 5; ix++)
	{
		m_dMomentUltPlate[ix] = 0;	// A, B, C, D, A'
		m_dMomentUsePlate[ix] = 0;
		m_dMomentUseJudge_Plate[ix] = 0;
		m_dShearUltPlate[ix] = 0;	// A, B, C, D, A'
		m_dShearUsePlate[ix] = 0;	// A, B, C, D, A'
	}

	// ö������
	AhTPADelete(&m_pArrRebar, (CRebar*)0);
	AhTPADelete(&m_pArrRebar_User, (CRebar*)0);
	AhTPAMake(WW_REBAR_COUNT * 2, &m_pArrRebar, (CRebar*)0);
}

CWingWall::~CWingWall()
{
	AhTPADelete(&m_pArrRebar, (CRebar*)0);
	AhTPADelete(&m_pArrRebar_User, (CRebar*)0);
	AhTPADelete(&m_pArrAreaConc, (CRebar*)0);
}

CWingWall& CWingWall::operator=(const CWingWall& Obj)
{
	m_bExist = Obj.m_bExist;
	m_bRAngEnd = Obj.m_bRAngEnd;
	m_nDirHunch = Obj.m_nDirHunch;
	m_dEL = Obj.m_dEL;
	m_dL1 = Obj.m_dL1;
	m_dL2 = Obj.m_dL2;
	m_dHL = Obj.m_dHL;
	m_dH2 = Obj.m_dH2;
	m_dH3 = Obj.m_dH3;
	m_dT1 = Obj.m_dT1;
	m_dT2 = Obj.m_dT2;
	m_dLT = Obj.m_dLT;
	m_dHuW1 = Obj.m_dHuW1;
	m_dHuH1 = Obj.m_dHuH1;
	m_dHuW2 = Obj.m_dHuW2;
	m_dHuH2 = Obj.m_dHuH2;
	m_dSW = Obj.m_dSW;
	m_dSH = Obj.m_dSH;
	m_vAng = Obj.m_vAng;
	m_nAttachPos = Obj.m_nAttachPos;
	m_bSWHor = Obj.m_bSWHor;

	for(long ix = 0; ix < 2; ix++)
	{
		m_nCountLayerA[ix] = Obj.m_nCountLayerA[ix];
		m_nCountLayerB[ix] = Obj.m_nCountLayerB[ix];
		m_nCountLayerC[ix] = Obj.m_nCountLayerC[ix];
		m_nCountLayerD[ix] = Obj.m_nCountLayerD[ix];
	}
	
	int i=0; for(i=0; i<2; i++)
	{
		for(long nDan = 0; nDan < 2; nDan++)
		{
			m_xyArrUpper[i][nDan] = Obj.m_xyArrUpper[i][nDan];
			m_xyArrLower[i][nDan] = Obj.m_xyArrLower[i][nDan];
			m_xyArrSide[i][nDan] = Obj.m_xyArrSide[i][nDan];
		}
	}

	for(i=0; i<2; i++)
	{
		for(long nDan = 0; nDan < 2; nDan++)
		{
			m_dDiaMainA[i][nDan] = Obj.m_dDiaMainA[i][nDan];			// A���� ��ö�� ����[In or Out]
			m_dDiaMainB[i][nDan] = Obj.m_dDiaMainB[i][nDan];			// B���� ��ö�� ����[In or Out]
			m_dDiaMainC[i][nDan] = Obj.m_dDiaMainC[i][nDan];			// C���� ��ö�� ����[In or Out]
			m_dDiaMainD[i][nDan] = Obj.m_dDiaMainD[i][nDan];			// D���� ��ö�� ����[In or Out]
			m_dDiaSupportA[i][nDan] = Obj.m_dDiaSupportA[i][nDan];		// A���� ���ö�� ����[In or Out]
			m_dDiaSupportB[i][nDan] = Obj.m_dDiaSupportB[i][nDan];		// B���� ���ö�� ����[In or Out]
			m_dDiaSupportC[i][nDan] = Obj.m_dDiaSupportC[i][nDan];		// C���� ���ö�� ����[In or Out]
			m_dDiaSupportD[i][nDan] = Obj.m_dDiaSupportD[i][nDan];		// D���� ���ö�� ����[In or Out]
		}
		m_nCountSlopeBlock[i] = Obj.m_nCountSlopeBlock[i];	// ������ ��� ������κ����� ��� ö�� ���� ����
	}
	m_dDiaHuUpper = Obj.m_dDiaHuUpper;				// ��ü ��� ��ġö�� ����
	m_dDiaHuLower = Obj.m_dDiaHuLower;				// ��ü �Ϻ� ��ġö�� ����
	m_dDiaHuFoot = Obj.m_dDiaHuFoot;
	m_dDiaShearA = Obj.m_dDiaShearA;				// A���� ����ö�� ����
	m_dDiaShearB = Obj.m_dDiaShearB;				// B���� ����ö�� ����
	m_dDiaShearC = Obj.m_dDiaShearC;				// C���� ����ö�� ����
	m_dDiaShearD = Obj.m_dDiaShearD;				// D���� ����ö�� ����
	m_dSpaceShearA = Obj.m_dSpaceShearA;			// A���� ����ö�� ����
	m_dSpaceShearB = Obj.m_dSpaceShearB;			// B���� ����ö�� ����
	m_dSpaceShearC = Obj.m_dSpaceShearC;			// C���� ����ö�� ����
	m_dSpaceShearD = Obj.m_dSpaceShearD;			// D���� ����ö�� ����
	m_dStdVerCTC = Obj.m_dStdVerCTC;				// ǥ�� ���� ö�� ��� ����
	m_dMainCTCA = Obj.m_dMainCTCA;					// A���� ��ö�� ��ġ ����
	m_dMainCTCB = Obj.m_dMainCTCB;					// B���� ��ö�� ��ġ ����
	m_bSelMainCTCD = Obj.m_bSelMainCTCD;			// D���� ��ö�� ������ �⺻ ������ 2���
	m_bUpperRebarSlope = Obj.m_bUpperRebarSlope;	// ������ ��ܺ� ��ö���� ���� ��ġ
	m_nCountShearBind = Obj.m_nCountShearBind;		// ����ö�� ��ٽ� ��ö�� ���� ����
	m_nRebarCurve = Obj.m_nRebarCurve;				// C���� ��ö�� ����� ó��(0:�ù漭, 1:����, 2:�)
	m_nHunchCTC = Obj.m_nHunchCTC;					// ��ġ ö�� ����(0:����, 1:���)
	m_nTypeShearBC = Obj.m_nTypeShearBC;			// B, C ���� ����ö�ٹ�� ���� - 0: ����, 1: ����, 2: �����
	m_bAddRebarReinforce = Obj.m_bAddRebarReinforce;
	//�Ҵ� �Է�
	m_dDistNoriStart	= Obj.m_dDistNoriStart;
	m_dSlopeNoriFirst	= Obj.m_dSlopeNoriFirst;
	m_dSlopeNoriNext	= Obj.m_dSlopeNoriNext;
	m_dHeightMaxSodan	= Obj.m_dHeightMaxSodan;
	m_dSlopeSodan		= Obj.m_dSlopeSodan;
	m_dWidthSodan		= Obj.m_dWidthSodan;
	m_dWidthSodanWallFront	= Obj.m_dWidthSodanWallFront;
	m_dElSodanBottom	= Obj.m_dElSodanBottom;
	m_bRetainWallFront	= Obj.m_bRetainWallFront;
	m_dHeightSoil		= Obj.m_dHeightSoil;
	m_bUseUserOutput3D = Obj.m_bUseUserOutput3D;
	for(long ix = 0; ix < 5; ix++)
	{
		m_dMomentUltPlate[ix] = Obj.m_dMomentUltPlate[ix];	// A, B, C, D, A'
		m_dMomentUsePlate[ix] = Obj.m_dMomentUsePlate[ix];
		m_dMomentUseJudge_Plate[ix] = Obj.m_dMomentUseJudge_Plate[ix];
		m_dShearUltPlate[ix] = Obj.m_dShearUltPlate[ix];	// A, B, C, D, A'
		m_dShearUsePlate[ix] = Obj.m_dShearUsePlate[ix];	// A, B, C, D, A'
	}


	AhTPACopy(&m_pArrRebar, &Obj.m_pArrRebar, (CRebar*)0);	// ö������
	AhTPACopy(&m_pArrRebar_User, &Obj.m_pArrRebar_User, (CRebar*)0);	// ö������
	AhTPACopy(&m_pArrAreaConc, &Obj.m_pArrAreaConc, (CTwinVectorAreaShape*)0);	// ��ũ��Ʈ ����

	m_RebarShearDetailA	= Obj.m_RebarShearDetailA;
	m_RebarShearDetailB	= Obj.m_RebarShearDetailB;
	m_RebarShearDetailC	= Obj.m_RebarShearDetailC;
	m_RebarShearDetailD	= Obj.m_RebarShearDetailD;

	return *this;
}

void CWingWall::Serialize(CArchive &ar)
{
	//  9 : BoxŸ�� �۾�
	// 10 : BoxŸ�� �۾� , m_dMomentUltPlate,m_dMomentUsePlate,m_bUseUserOutput3D,m_dShearUltPlate
	// 11 : BoxŸ�� �۾� , ����� ö��
	// 12 : m_dHeightSoil ���� ���ǰ�
	// 13 : m_dH3
	// 14 : m_dMomentUseJudge_Plate
	long nFlag = 14;
	
	if (ar.IsStoring())
	{
		ar << nFlag;

		ar << m_bExist;
		ar << m_bRAngEnd;
		ar << m_nDirHunch;
		ar << m_dEL;
		ar << m_dL1;
		ar << m_dL2;
		ar << m_dHL;
		ar << m_dH2;
		ar << m_dH3;
		ar << m_dT1;
		ar << m_dT2;
		ar << m_dLT;
		ar << m_dHuW1;
		ar << m_dHuH1;
		ar << m_dHuW2;
		ar << m_dHuH2;
		ar << m_dSW;
		ar << m_dSH;
		ar << m_vAng;
		ar << m_nAttachPos;
		ar << m_bSWHor;

		// ��� ����
		for(long ix = 0; ix < 2; ix++)
		{
			ar << m_nCountLayerA[ix];
			ar << m_nCountLayerB[ix];
			ar << m_nCountLayerC[ix];
			ar << m_nCountLayerD[ix];
		}
		
		int i=0; for(i=0; i<2; i++)
		{
			for(long nDan = 0; nDan < 2; nDan++)
			{
				ar << m_xyArrUpper[i][nDan];
				ar << m_xyArrLower[i][nDan];
				ar << m_xyArrSide[i][nDan];
			}
		}

		ar << m_dStdVerCTC;			// ǥ�� ���� ö�� ��� ����
		ar << m_dMainCTCA;			// A���� ��ö�� ��ġ ����
		ar << m_dMainCTCB;			// B���� ��ö�� ��ġ ����
		ar << m_bSelMainCTCD;		// D���� ��ö�� ������ �⺻ ������ 2���
		ar << m_bUpperRebarSlope;	// ������ ��ܺ� ��ö���� ���� ��ġ
		ar << m_nCountShearBind;	// ����ö�� ��ٽ� ��ö�� ���� ����
		ar << m_nRebarCurve;		// C���� ��ö�� ����� ó��(0:�ù漭, 1:����, 2:�)
		ar << m_nHunchCTC;			// ��ġ ö�� ����(0:����, 1:���)
		ar << m_nTypeShearBC;		// B, C ���� ����ö�ٹ�� ���� - 0: ����, 1: ����, 2: �����
		ar << m_bAddRebarReinforce;
		for(i=0; i<2; i++)
		{
			ar << m_nCountSlopeBlock[i];	// ������ ������κ����� ��� ö�� ���� ����
			for(long nDan = 0; nDan < 2; nDan++)
			{
				ar << m_dDiaMainA[i][nDan];			// A���� ��ö�� ����[In or Out]
				ar << m_dDiaMainB[i][nDan];			// B���� ��ö�� ����[In or Out]
				ar << m_dDiaMainC[i][nDan];			// C���� ��ö�� ����[In or Out]
				ar << m_dDiaMainD[i][nDan];			// D���� ��ö�� ����[In or Out]
				ar << m_dDiaSupportA[i][nDan];		// A���� ���ö�� ����[In or Out]
				ar << m_dDiaSupportB[i][nDan];		// B���� ���ö�� ����[In or Out]
				ar << m_dDiaSupportC[i][nDan];		// C���� ���ö�� ����[In or Out]
				ar << m_dDiaSupportD[i][nDan];		// D���� ���ö�� ����[In or Out]
			}
		}
		ar << m_dDiaHuUpper;	// ��ü ��� ��ġö�� ����
		ar << m_dDiaHuLower;	// ��ü �Ϻ� ��ġö�� ����
		ar << m_dDiaHuFoot;
		ar << m_dDiaShearA;		// A���� ����ö�� ����
		ar << m_dDiaShearB;		// B���� ����ö�� ����
		ar << m_dDiaShearC;		// C���� ����ö�� ����
		ar << m_dDiaShearD;		// D���� ����ö�� ����
		ar << m_dSpaceShearA;	// A���� ����ö�� ����
		ar << m_dSpaceShearB;	// B���� ����ö�� ����
		ar << m_dSpaceShearC;	// C���� ����ö�� ����
		ar << m_dSpaceShearD;	// D���� ����ö�� ����
		AhTPASerial(ar, &m_pArrRebar, (CRebar*)0);	// ö������
		AhTPASerial(ar, &m_pArrRebar_User, (CRebar*)0);	// ö������
		// �Ҵ� �Է�
		ar << m_dDistNoriStart;
		ar << m_dSlopeNoriFirst;
		ar << m_dSlopeNoriNext;
		ar << m_dHeightMaxSodan;
		ar << m_dSlopeSodan;
		ar << m_dWidthSodan;
		ar << m_dWidthSodanWallFront;
		ar << m_dElSodanBottom;
		ar << m_bRetainWallFront;
		ar << m_dHeightSoil;

		m_RebarShearDetailA.Serialize(ar);
		m_RebarShearDetailB.Serialize(ar);
		m_RebarShearDetailC.Serialize(ar);
		m_RebarShearDetailD.Serialize(ar);

		ar << m_bUseUserOutput3D;
		for(long ix = 0; ix < 5; ix++)
		{
			ar << m_dMomentUltPlate[ix];	// A, B, C, D, A'
			ar << m_dMomentUsePlate[ix];
			ar << m_dShearUltPlate[ix];	// A, B, C, D, A'
			ar << m_dShearUsePlate[ix];	// A, B, C, D, A'
			ar << m_dMomentUseJudge_Plate[ix];
		}
	}
	else
	{
		ar >> nFlag;

		ar >> m_bExist;
		ar >> m_bRAngEnd;
		ar >> m_nDirHunch;
		ar >> m_dEL;
		ar >> m_dL1;
		ar >> m_dL2;
		ar >> m_dHL;
		if(nFlag > 8)
		{
			ar >> m_dH2;
		}
		if(nFlag > 12)
			ar >> m_dH3;
		ar >> m_dT1;
		ar >> m_dT2;
		ar >> m_dLT;
		ar >> m_dHuW1;
		ar >> m_dHuH1;
		ar >> m_dHuW2;
		ar >> m_dHuH2;
		ar >> m_dSW;
		ar >> m_dSH;
		ar >> m_vAng;
		if(nFlag > 2) ar >> m_nAttachPos;
		if(nFlag > 5) ar >> m_bSWHor;

		// ��� ����
		if(nFlag > 8)
		{
			for(long ix = 0; ix < 2; ix++)
			{
				ar >> m_nCountLayerA[ix];
				ar >> m_nCountLayerB[ix];
				ar >> m_nCountLayerC[ix];
				ar >> m_nCountLayerD[ix];
			}
		}
		int i=0; for(i=0; i<2; i++)
		{
			ar >> m_xyArrUpper[i][0];
			ar >> m_xyArrLower[i][0];
			ar >> m_xyArrSide[i][0];
			if(nFlag > 8)
			{
				ar >> m_xyArrUpper[i][1];
				ar >> m_xyArrLower[i][1];
				ar >> m_xyArrSide[i][1];
			}
		}

		ar >> m_dStdVerCTC;			// ǥ�� ���� ö�� ��� ����
		ar >> m_dMainCTCA;			// A���� ��ö�� ��ġ ����
		ar >> m_dMainCTCB;			// B���� ��ö�� ��ġ ����
		ar >> m_bSelMainCTCD;		// D���� ��ö�� ������ �⺻ ������ 2���
		ar >> m_bUpperRebarSlope;	// ������ ��ܺ� ��ö���� ���� ��ġ
		ar >> m_nCountShearBind;	// ����ö�� ��ٽ� ��ö�� ���� ����
		if(nFlag < 4) m_nCountShearBind--;

		ar >> m_nRebarCurve;		// C���� ��ö�� ����� ó��(0:�ù漭, 1:����, 2:�)	//2004. 10. 11 Goh
		ar >> m_nHunchCTC;			// ��ġ ö�� ����(0:����, 1:���)
		if(nFlag > 0)
		{
			ar >> m_nTypeShearBC;	// B, C ���� ����ö�ٹ�� ���� - 0: ����, 1: ����, 2: �����
		}
		if(nFlag > 4)
			ar >> m_bAddRebarReinforce;

		for(i=0; i<2; i++)
		{
			ar >> m_nCountSlopeBlock[i];	// ������ ������κ����� ��� ö�� ���� ����
			ar >> m_dDiaMainA[i][0];			// A���� ��ö�� ����[In or Out]
			ar >> m_dDiaMainB[i][0];			// B���� ��ö�� ����[In or Out]
			ar >> m_dDiaMainC[i][0];			// C���� ��ö�� ����[In or Out]
			ar >> m_dDiaMainD[i][0];			// D���� ��ö�� ����[In or Out]
			ar >> m_dDiaSupportA[i][0];		// A���� ���ö�� ����[In or Out]
			ar >> m_dDiaSupportB[i][0];		// B���� ���ö�� ����[In or Out]
			ar >> m_dDiaSupportC[i][0];		// C���� ���ö�� ����[In or Out]
			ar >> m_dDiaSupportD[i][0];		// D���� ���ö�� ����[In or Out]
			if(nFlag > 8)
			{
				ar >> m_dDiaMainA[i][1];
				ar >> m_dDiaMainB[i][1];
				ar >> m_dDiaMainC[i][1];
				ar >> m_dDiaMainD[i][1];
				ar >> m_dDiaSupportA[i][1];
				ar >> m_dDiaSupportB[i][1];
				ar >> m_dDiaSupportC[i][1];
				ar >> m_dDiaSupportD[i][1];
			}
		}

		ar >> m_dDiaHuUpper;	// ��ü ��� ��ġö�� ����
		ar >> m_dDiaHuLower;	// ��ü �Ϻ� ��ġö�� ����
		if(nFlag > 7) ar >> m_dDiaHuFoot;
		ar >> m_dDiaShearA;		// A���� ����ö�� ����
		ar >> m_dDiaShearB;		// B���� ����ö�� ����
		ar >> m_dDiaShearC;		// C���� ����ö�� ����
		ar >> m_dDiaShearD;		// D���� ����ö�� ����
		ar >> m_dSpaceShearA;	// A���� ����ö�� ����
		ar >> m_dSpaceShearB;	// B���� ����ö�� ����
		ar >> m_dSpaceShearC;	// C���� ����ö�� ����
		ar >> m_dSpaceShearD;	// D���� ����ö�� ����
		AhTPASerial(ar, &m_pArrRebar, (CRebar*)0);	// ö������
		long nSizeArr = m_pArrRebar.GetSize();
		for(i=0; i < WW_REBAR_COUNT*2 - nSizeArr; i++)
		{
			CRebar *pRB = new CRebar;
			m_pArrRebar.Add(pRB);
		}
		if(nFlag > 10)
		{
			AhTPASerial(ar, &m_pArrRebar_User, (CRebar*)0);	// ö������
		}

		// �Ҵ� �Է�
		ar >> m_dDistNoriStart;
		ar >> m_dSlopeNoriFirst;
		ar >> m_dSlopeNoriNext;
		ar >> m_dHeightMaxSodan;
		ar >> m_dSlopeSodan;
		ar >> m_dWidthSodan;
		ar >> m_dWidthSodanWallFront;
		if(nFlag > 1) ar >> m_dElSodanBottom;
		ar >> m_bRetainWallFront;
		if(nFlag > 11)
			ar >> m_dHeightSoil;

		if(nFlag > 6)
		{
			m_RebarShearDetailA.Serialize(ar);
			m_RebarShearDetailB.Serialize(ar);
			m_RebarShearDetailC.Serialize(ar);
			m_RebarShearDetailD.Serialize(ar);

			if(m_RebarShearDetailA.m_nType<0 || m_RebarShearDetailA.m_nType>=TYPE_SHEAR_SU) m_RebarShearDetailA.m_nType = TYPE_SHEAR_SU-1;
			if(m_RebarShearDetailB.m_nType<0 || m_RebarShearDetailB.m_nType>=TYPE_SHEAR_SU) m_RebarShearDetailB.m_nType = TYPE_SHEAR_SU-1;
			if(m_RebarShearDetailC.m_nType<0 || m_RebarShearDetailC.m_nType>=TYPE_SHEAR_SU) m_RebarShearDetailC.m_nType = TYPE_SHEAR_SU-1;
			if(m_RebarShearDetailD.m_nType<0 || m_RebarShearDetailD.m_nType>=TYPE_SHEAR_SU) m_RebarShearDetailD.m_nType = TYPE_SHEAR_SU-1;
		}
		else
		{
			m_RebarShearDetailA.m_dDia = m_dDiaShearA;
			m_RebarShearDetailB.m_dDia = m_dDiaShearB;
			m_RebarShearDetailC.m_dDia = m_dDiaShearC;
			m_RebarShearDetailD.m_dDia = m_dDiaShearD;

			m_RebarShearDetailA.m_nType = 3;
			m_RebarShearDetailB.m_nType = 3;
			m_RebarShearDetailC.m_nType = 3;
			m_RebarShearDetailD.m_nType = 3;
		}

		if(nFlag > 9)
		{
			ar >> m_bUseUserOutput3D;
			for(long ix = 0; ix < 5; ix++)
			{
				ar >> m_dMomentUltPlate[ix];	// A, B, C, D, A'
				ar >> m_dMomentUsePlate[ix];
				ar >> m_dShearUltPlate[ix];	// A, B, C, D, A'
				ar >> m_dShearUsePlate[ix];	// A, B, C, D, A'
				if(nFlag > 13)
				{
					ar >> m_dMomentUseJudge_Plate[ix];
				}
			}
		}
	}
}

void CWingWall::Clear()
{
	m_nDirHunch = 0;
	m_dL1 = 0.0;
	m_dL2 = 0.0;
	m_dHL = 0.0;
	m_dH2 = 0.0;
	m_dH3 = 0.0;
	m_dT1 = 0.0;
	m_dT2 = 0.0;
	m_dLT = 0.0;
	m_dHuW1 = 0.0;
	m_dHuH1 = 0.0;
	m_dHuW2 = 0.0;
	m_dHuH2 = 0.0;
	m_dSW = 0.0;
	m_dSH = 0.0;

	int i=0; for(i=0; i<2; i++)
	{
		for(long nDan = 0; nDan < 2; nDan++)
		{
			m_xyArrUpper[i][nDan].RemoveAll();
			m_xyArrLower[i][nDan].RemoveAll();
			m_xyArrSide[i][nDan].RemoveAll();
		}
	}
	for(long ix = 0; ix < 2; ix++)
	{
		m_nCountLayerA[ix] = 1;
		m_nCountLayerB[ix] = 1;
		m_nCountLayerC[ix] = 1;
		m_nCountLayerD[ix] = 1;
	}
	m_nAttachPos	= 0;
	m_bSWHor		= FALSE;
}

BOOL CWingWall::IsValid()
{
	if(m_dL1==0 && m_dL2==0) return FALSE;
	return TRUE;
}

// BOOL CWingWall::IsBlockA()
// {
// 	if(m_dL2==0) return FALSE;
// 	return TRUE;
// }

BOOL CWingWall::IsBlockB()
{
	if(m_dL2==0) return FALSE;
	return TRUE;
}

BOOL CWingWall::IsBlockC()
{
	if(m_dL2==0) return FALSE;
	return TRUE;
}

// BOOL CWingWall::IsBlockD()
// {
// 	if(m_dL1==0) return FALSE;
// 	return TRUE;
// }

void CWingWall::SetDataDefaultSodan(double dEl)
{
	// �Ҵ� �Է�
	m_dDistNoriStart	= 500;
	m_dSlopeNoriFirst	= 1.5;
	m_dSlopeNoriNext	= 1.8;
	m_dHeightMaxSodan	= 6000;
	m_dWidthSodan		= 1000;
	m_dSlopeSodan		= 4.0;
	m_dWidthSodanWallFront	= 0;
	m_dElSodanBottom	= toM(dEl);
	m_bRetainWallFront	= FALSE;
}

long CWingWall::GetCountRebarSlop(int nOut)
{
	long nRebarSlope = 0;
	if(!m_bUpperRebarSlope) 
	{
		nRebarSlope = 0;
	}
	else
	{
		long nBlockSlope = m_nCountSlopeBlock[nOut];
		long nSizeDim = m_xyArrSide[nOut][0].GetSize();

		if(nSizeDim > 0)
		{
			long n=0; for(n=0; n<nBlockSlope; n++)
			{
				CDPoint xyR = m_xyArrSide[nOut][0].GetAt(nSizeDim-1-n);
				nRebarSlope += (long)xyR.x;
			}
		}
	}

	return nRebarSlope;
}

double CWingWall::GetEndEl( BOOL bIncludeDancha/*=FALSE*/ )
{
	return bIncludeDancha? m_dEL : m_dEL - m_dH3;
}

