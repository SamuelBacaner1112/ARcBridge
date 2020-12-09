// ARcBridgeDrawingOptionData.cpp: implementation of the CARcBridgeDrawingOptionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgedrawingoption.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeDrawingOptionData::CARcBridgeDrawingOptionData()
{
	DefaultDrawingOptionVerticalPlane();
	DefaultDrawingOptionGeneral();
	DefaultDrawingOptionRebar();
	DefaultDrawingOptionAps();
	DefaultDrawingOptionCns();
	DefaultDrawingOptionOthers();
	DefaultDrawingOptionShoeArrange();
	DefaultDrawingOptionBoring();
	DefaultDrawingOptionHyunTa();
	DefaultDrawingOptionSlabStandardOutput();
}

CARcBridgeDrawingOptionData::~CARcBridgeDrawingOptionData()
{

}

void CARcBridgeDrawingOptionData::Serialize(CArchive& ar)
{
	// 25 : m_bSepUpperAndLowerFootingRebar;	// ���� ������ ��� �ϸ��� �и��ؼ� ǥ��
	//         m_bSepUpperAndLowerSlabRebar;	// ��ν����� ������ ��� �ϸ��� �и��ؼ� ǥ��
	// 26 : m_bRebarTableSepByFy;			// ö�����ǥ fy���� �и��ؼ� ǥ��
	//			m_bRebarTableSepByFyAps;	// ö�����ǥ fy���� �и��ؼ� ǥ��(���ӽ�����)
	// 27 : DWORD  m_dwDrawRcStd;
	//		double m_dScaleRcStd1Base;	// ����1 �⺻ ������
	//		double m_dScaleRcStd2Base;	// ����2 �⺻ ������
	//		double m_dScaleRcStd3Base;	// ����3 �⺻ ������
	// 28 : m_dScaleRebarDetailPRF;		// �λ�������ǻ� 
	// 29 : m_bSepUpperSlab2DanAll;
	// 30 : m_nTypeHyunTaVol; 
	// 31 : m_bDrawHunchRebarOnly
	// 32 : m_nBasePosPlaneSupportRebarDim
	long nFlag = 32;
	if(ar.IsStoring())
	{
		ar << nFlag;

		// ����鵵
		ar << m_szDxfPlanName;
		ar << m_dwDrawVP;
		ar << m_dScaleVPBase;
		ar << m_dScaleVPCross;
		ar << m_dScaleVPLocation;
		ar << m_bDrawVPCoordinateXY;

		// �Ϲݵ�
		ar << m_dwDrawGen;
		long i=0; for(i=0; i<4; i++)
		{
			ar << m_bDrawGenWingWall[i][0];
			ar << m_bDrawGenWingWall[i][1];
			ar << m_bDrawGenWingWall[i][2];
		}
		ar << m_bDrawGenCoordinateXY;
		ar << m_bDimTableForPilePlane;	// ��鵵 ���� ���� ���̺� ó��
		ar << m_dScaleGenBase;
		ar << m_dScaleGenDetailA;
		ar << m_dScaleGenNotch;
		ar << m_dScaleGenOrder;
		ar << m_dScaleGenWingWall;
		ar << m_dScaleGenDetailB;
		ar << m_dScaleGenConcJoint;
		ar << m_dScaleFXJoint;			// �������� �� ������
		ar << m_dScaleGenCrack;		// �տ����߿� �� ������

		// ������
		ar << m_dwDrawRebar;
		for(i=0; i<4; i++)
		{
			ar << m_bDrawRebarWingWall[i][0];
			ar << m_bDrawRebarWingWall[i][1];
			ar << m_bDrawRebarWingWall[i][2];
			ar << m_bDrawRebarWingWall[i][3];
		}
		ar << m_nTypeMainRebarAssm;				// ��ö�� ������ ǥ�� ���
		ar << m_dScaleRebarBase;
		ar << m_dScaleRebarDetail;				// �� ������
		ar << m_dScaleRebarWingWall;			// ������ ������
		ar << m_dScaleBMD;					// BMD ������
		ar << m_dScaleSFD;					// SFD ������
		ar << m_dScaleAFD;
		ar << m_dScaleRebarDetailArchRibSpringging;	// ��ġ���� ��������� �� ������
		ar << m_dScaleRebarDetailColumnSection;	// ��� �ܸ� �� ������
		ar << m_dScaleRebarDetailGirderSection;	// �Ŵ� �ܸ� �� ������
		ar << m_dScaleRebarDetailSlabAcute;		// ������ �а��� �� ������
		ar << m_dScaleRebarDetailWallObtuse;		// ��ü ������ �� ������
		ar << m_dScaleRebarDetailFootingAcute;		// ���� �а��� �� ������
		ar << m_dScaleRebarDetailPRF;				// �λ�������� �� ������
		ar << m_bSeparateRebarBMWing;			// ������ ö�ټ��� �и� ���
		ar << m_bSeparateRebarBMColumn;			// ����� ö�ټ��� �и� ���
		ar << m_bSeparateRebarBMOther;			// ��Ÿö�� ö�ټ��� �и� ���(��ȣ��, �����ֹ�ħ)
		ar << m_bSeparateOutputRebarBMMain;		// ��ü ö�����ǥ �и� ���(������ ������� 2����� �þ)
		ar << m_nTypeOutRebarTable;				// ö�����ǥ ��¹��(0:��ü,1:��ö��,��Ÿö�ٱ���,2:������)
		ar << m_bRebarTableSepByFy;			// ö�����ǥ fy���� �и��ؼ� ǥ��
		ar << m_dSpaceBetweenRebarDetail;		// �󼼵��� ����
		ar << m_nCountRowEachTable;				// ö�����ǥ ���̺�� �ִ� row��
		ar << m_bMainRebarNumberingInStandardSection;	// ǥ�شܸ鵵�� ��ö�� �ѹ��� ǥ������?
		ar << m_bLengthInfoInMainRebarNumbering;			// ��ö�� ������ ö�� ��ȣ�� ���� ���� ǥ������?
		ar << m_bExcludeDoupleMainRebarCrossSection;	// Ⱦ�� ������ ��½� ��ġ�� ��ö�� ǥ�� ����(�� ġ���� �״�� ǥ��)
		ar << m_bSepFrontAndBackWallRebar;	// ��ü ������ ���� ����� �и� �ؼ� ǥ��
		ar << m_bSepUpperAndLowerFootingRebar;	// ���� ������ ��� �ϸ��� �и��ؼ� ǥ��
		ar << m_bSepUpperAndLowerSlabRebar;	// ��ν����� ������ ��� �ϸ��� �и��ؼ� ǥ��
		ar << m_nDirMarkUpperSlabMainRebar;	// ��ν����� ��ö�� ������ ö�� ���ü� ����(0 : �������� ����, 1 : �������� ����, 2 : �ڵ�)
		ar << m_bSepUpperSlab2DanAll;
		ar << m_bDrawHunchRebarOnly;
		ar << m_nBasePosPlaneSupportRebarDim;

		// ���ӽ����� ������
		ar << m_dwDrawRebarAps;
		ar << m_dScaleRebarAps;					// ���ӽ����� ������ ������
		ar << m_dScaleDetailAps;				// �󼼵� ������
		ar << m_dSpaceBetweenRebarDetailAps;	// �󼼵��� ����
		ar << m_bRebarTableSepByFyAps;	// ö�����ǥ fy���� �и��ؼ� ǥ��(���ӽ�����)

		// ���潽���� ������
		ar << m_dwDrawRebarCns;
		ar << m_nTypeCnsRebar;					// ���潽���� 1, 2 ǥ�����(0 : 1,2�� �Բ�ǥ��, 1 : �и��ؼ� ǥ��)
		ar << m_dScaleRebarCns;					// ���潽���� ������ ������
		ar << m_dSpaceBetweenRebarDetailCns;

		// ����Ÿ�� ����
		ar << m_dwDrawHyunTa;
		ar << m_dScaleHyunTaFront;
		ar << m_dScaleHyunTaSection;
		ar << m_dScaleHyunTaDetailHold;
		ar << m_dScaleHyunTaDetailSpac;
		ar << m_nTypeHyunTaCut;
		ar << m_nTypeHyunTaVol;

		// ���� �ֻ�
		ar << m_dwDrawBoring;
		ar << m_dScaleBoring;

		// ��Ÿ ����
		ar << m_dwDrawOthers;
		ar << m_dScaleRebarOthers;
		ar << m_dScaleDetailGuardWall;
		ar << m_dScaleDetailMonument;
		ar << m_dSpaceBetweenRebarDetailOthers;

		// RC �����걳 ǥ�ص�
		ar << m_dwDrawRcStd;
		ar << m_dScaleRcStd1Base;	// ����1 �⺻ ������
		ar << m_dScaleRcStd2Base;	// ����2 �⺻ ������
		ar << m_dScaleRcStd3Base;	// ����3 �⺻ ������

	}
	else
	{
		ar >> nFlag;

		// ����鵵
		if(nFlag>6)
			ar >> m_szDxfPlanName;
		ar >> m_dwDrawVP;
		ar >> m_dScaleVPBase;
		ar >> m_dScaleVPCross;
		ar >> m_dScaleVPLocation;
		if(nFlag>11)
			ar >> m_bDrawVPCoordinateXY;

		// �Ϲݵ�
		ar >> m_dwDrawGen;
		if(nFlag>9)
		{
			long i=0; for(i=0; i<4; i++)
			{
				ar >> m_bDrawGenWingWall[i][0];
				ar >> m_bDrawGenWingWall[i][1];
				ar >> m_bDrawGenWingWall[i][2];
			}
			ar >> m_bDrawGenCoordinateXY;
		}
		if(nFlag>10)
			ar >> m_bDimTableForPilePlane;	// ��鵵 ���� ���� ���̺� ó��

		ar >> m_dScaleGenBase;
		ar >> m_dScaleGenDetailA;
		ar >> m_dScaleGenNotch;
		ar >> m_dScaleGenOrder;
		ar >> m_dScaleGenWingWall;
		if(nFlag > 4)
			ar >> m_dScaleGenDetailB; 
		if(nFlag > 5)
			ar >> m_dScaleGenConcJoint;
		if(nFlag > 21)
		{
			ar >> m_dScaleFXJoint;			// �������� �� ������
			ar >> m_dScaleGenCrack;		// �տ����߿� �� ������
		}
		else
		{
			m_dScaleFXJoint	= m_dScaleGenDetailB;
			m_dScaleGenCrack	= m_dScaleGenConcJoint;
		}

		// ������
		ar >> m_dwDrawRebar;
		if(nFlag>9)
		{
			long i=0; for(i=0; i<4; i++)
			{
				ar >> m_bDrawRebarWingWall[i][0];
				ar >> m_bDrawRebarWingWall[i][1];
				ar >> m_bDrawRebarWingWall[i][2];
				if(nFlag > 16) ar >> m_bDrawRebarWingWall[i][3];
			}
		}
		ar >> m_nTypeMainRebarAssm;					// ��ö�� ������ ǥ�� ���
		ar >> m_dScaleRebarBase;
		ar >> m_dScaleRebarDetail;					// �� ������
		ar >> m_dScaleRebarWingWall;				// ������ ������
		if(nFlag > 2)
		{
			ar >> m_dScaleBMD;						// BMD ������
			ar >> m_dScaleSFD;						// SFD ������
		}
		if(nFlag > 18)
			ar >> m_dScaleAFD;

		if(nFlag > 19)
		{
			ar >> m_dScaleRebarDetailArchRibSpringging;	// ��ġ���� ��������� �� ������
			ar >> m_dScaleRebarDetailColumnSection;	// ��� �ܸ� �� ������
			ar >> m_dScaleRebarDetailGirderSection;	// �Ŵ� �ܸ� �� ������
			ar >> m_dScaleRebarDetailSlabAcute;		// ������ �а��� �� ������
			ar >> m_dScaleRebarDetailWallObtuse;		// ��ü ������ �� ������
			ar >> m_dScaleRebarDetailFootingAcute;		// ���� �а��� �� ������
		}
		if(nFlag > 27)
			ar >> m_dScaleRebarDetailPRF;			// �λ�������� �� ������
		if(nFlag > 0)
		{
			ar >> m_bSeparateRebarBMWing;			// ������ ö�ټ��� �и� ���
			ar >> m_bSeparateRebarBMColumn;			// ����� ö�ټ��� �и� ���
		}
		else
		{
			m_bSeparateRebarBMWing		= TRUE;		// ������ ö�ټ��� �и� ���
			m_bSeparateRebarBMColumn	= TRUE;		// ����� ö�ټ��� �и� ���
		}

		if(nFlag > 14)
			ar >> m_bSeparateRebarBMOther;			// ��Ÿö�� ö�ټ��� �и� ���(��ȣ��, �����ֹ�ħ)
		else
			m_bSeparateRebarBMOther	= TRUE;			// ��Ÿö�� ö�ټ��� �и� ���(��ȣ��, �����ֹ�ħ)

		if(nFlag > 1)
			ar >> m_bSeparateOutputRebarBMMain;		// ��ü ö�����ǥ �и� ���(������ ������� 2����� �þ)
		else
			m_bSeparateOutputRebarBMMain	= FALSE;// ��ü ö�����ǥ �и� ���(������ ������� 2����� �þ)

		ar >> m_nTypeOutRebarTable;					// ö�����ǥ ��¹��(0:��ü,1:��ö��,��Ÿö�ٱ���,2:������)
		if(nFlag > 25)
			ar >> m_bRebarTableSepByFy;			// ö�����ǥ fy���� �и��ؼ� ǥ��
		ar >> m_dSpaceBetweenRebarDetail;			// �󼼵��� ����
		ar >> m_nCountRowEachTable;					// ö�����ǥ ���̺�� �ִ� row��
		if(nFlag > 7) 
			ar >> m_bMainRebarNumberingInStandardSection;	// ǥ�شܸ鵵�� ��ö�� �ѹ��� ǥ������?
		if(nFlag > 8)
			ar >> m_bLengthInfoInMainRebarNumbering;			// ��ö�� ������ ö�� ��ȣ�� ���� ���� ǥ������?
		if(nFlag > 17)
			ar >> m_bExcludeDoupleMainRebarCrossSection;	// Ⱦ�� ������ ��½� ��ġ�� ��ö�� ǥ�� ����(�� ġ���� �״�� ǥ��)
		if(nFlag > 22)
			ar >> m_bSepFrontAndBackWallRebar;	// ��ü ������ ���� ����� �и� �ؼ� ǥ��
		if(nFlag > 24)
		{
			ar >> m_bSepUpperAndLowerFootingRebar;	// ���� ������ ��� �ϸ��� �и��ؼ� ǥ��
			ar >> m_bSepUpperAndLowerSlabRebar;	// ��ν����� ������ ��� �ϸ��� �и��ؼ� ǥ��
		}
		if(nFlag > 23)
			ar >> m_nDirMarkUpperSlabMainRebar;	// ��ν����� ��ö�� ������ ö�� ���ü� ����(0 : �������� ����, 1 : �������� ����, 2 : �ڵ�)
		if(nFlag > 28)
			ar >> m_bSepUpperSlab2DanAll;
		if(nFlag > 30)
			ar >> m_bDrawHunchRebarOnly;
		if(nFlag > 31)
			ar >> m_nBasePosPlaneSupportRebarDim;


		// ���ӽ����� ������
		ar >> m_dwDrawRebarAps;
		ar >> m_dScaleRebarAps;						// ���ӽ����� ������ ������
		ar >> m_dScaleDetailAps;					// �󼼵� ������
		ar >> m_dSpaceBetweenRebarDetailAps;		// �󼼵��� ����
		if(nFlag > 25)
			ar >> m_bRebarTableSepByFyAps;	// ö�����ǥ fy���� �и��ؼ� ǥ��(���ӽ�����)

		// ���潽���� ������
		ar >> m_dwDrawRebarCns;						// �ø����߰�
		ar >> m_nTypeCnsRebar;						// ���潽���� 1, 2 ǥ�����(0 : 1,2�� �Բ�ǥ��, 1 : �и��ؼ� ǥ��)	�ø����߰�
		ar >> m_dScaleRebarCns;						// ���潽���� ������ ������	�ø����߰�
		if(nFlag > 12)
			ar >> m_dSpaceBetweenRebarDetailCns;

		// ����Ÿ�� ����
		ar >> m_dwDrawHyunTa;
		ar >> m_dScaleHyunTaFront;
		ar >> m_dScaleHyunTaSection;
		if(nFlag > 15)
		{
			ar >> m_dScaleHyunTaDetailHold;
			ar >> m_dScaleHyunTaDetailSpac;
		}
		ar >> m_nTypeHyunTaCut;
		if(nFlag > 29)
		{
			ar >> m_nTypeHyunTaVol;
		}

		// ���� �ֻ�
		if(nFlag > 3)
		{
			ar >> m_dwDrawBoring;
			ar >> m_dScaleBoring;
		}

		// ��Ÿ ����
		if(nFlag > 13)
		{
			ar >> m_dwDrawOthers;
			ar >> m_dScaleRebarOthers;
			ar >> m_dScaleDetailGuardWall;
			if(nFlag > 20)
				ar >> m_dScaleDetailMonument;
			ar >> m_dSpaceBetweenRebarDetailOthers;
		}

		// RC �����걳 ǥ�ص�
		if(nFlag > 26)
		{
			ar >> m_dwDrawRcStd;
			ar >> m_dScaleRcStd1Base;	// ����1 �⺻ ������
			ar >> m_dScaleRcStd2Base;	// ����2 �⺻ ������
			ar >> m_dScaleRcStd3Base;	// ����3 �⺻ ������
		}
	}
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionVerticalPlane()
{
	m_szDxfPlanName		= _T("");
	m_dwDrawVP			= 0xFFFFFF;
	m_dScaleVPBase		=  300.0;
	m_dScaleVPCross		=  100.0;
	m_dScaleVPLocation	= 1000.0;
	m_bDrawVPCoordinateXY = TRUE;
}

// �����Ϲ�
void CARcBridgeDrawingOptionData::DefaultDrawingOptionGeneral()
{
	// �Ϲݵ�1
	m_dwDrawGen			= 0xFFFFFF;
	long i=0; for(i=0; i<4; i++)
	{
		m_bDrawGenWingWall[i][0] = TRUE;
		m_bDrawGenWingWall[i][1] = TRUE;
		m_bDrawGenWingWall[i][2] = FALSE;
	}
	m_bDrawGenCoordinateXY  = TRUE;
	m_bDimTableForPilePlane	= FALSE;	// ��鵵 ���� ���� ���̺� ó��
		
	m_dScaleGenBase		= 100.0;
	m_dScaleGenDetailA	= 20.0;
	m_dScaleGenNotch	= 2.0;
	m_dScaleGenOrder	= 200.0;
	m_dScaleGenWingWall = 100.0;
	m_dScaleGenDetailB	= 30.0;
	m_dScaleGenConcJoint = 5.0;
	m_dScaleGenCrack	= 5.0;		// �տ����߿� �� ������
	m_dScaleFXJoint	= 30.0;	// �������� �� ������
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionRebar()
{
	m_dwDrawRebar				= 0xFFFFFF;
	long i=0; for(i=0; i<4; i++)
	{
		m_bDrawRebarWingWall[i][0] = TRUE;
		m_bDrawRebarWingWall[i][1] = TRUE;
		m_bDrawRebarWingWall[i][2] = FALSE;
		m_bDrawRebarWingWall[i][3] = FALSE;
	}

	m_nTypeMainRebarAssm		= 0;			// ��ö�� ������ ǥ�� ���
	m_dScaleRebarBase			= 50.0;
	m_dScaleRebarDetail			= 20.0;			// �� ������
	m_dScaleRebarWingWall		= 50.0;			// ������ ������
	m_dScaleBMD					= 100.0;		// BMD ������
	m_dScaleSFD					= 100.0;		// SFD ������
	m_dScaleAFD					= 100.0;
	m_dScaleRebarDetailArchRibSpringging	= 30.0;		// ��ġ���� ��������� �� ������
	m_dScaleRebarDetailColumnSection		= 30.0;		// ��� �ܸ� �� ������
	m_dScaleRebarDetailGirderSection		= 30.0;		// �Ŵ� �ܸ� �� ������
	m_dScaleRebarDetailSlabAcute			= 30.0;		// ������ �а��� �� ������
	m_dScaleRebarDetailWallObtuse			= 30.0;		// ��ü ������ �� ������
	m_dScaleRebarDetailFootingAcute			= 30.0;		// ���� �а��� �� ������
	m_dScaleRebarDetailPRF					= 30.0;		// �λ�������� �� ������
	m_bSeparateRebarBMWing		= TRUE;			// ������ ö�ټ��� �и� ���
	m_bSeparateRebarBMColumn	= TRUE;			// ����� ö�ټ��� �и� ���
	m_bSeparateRebarBMOther		= TRUE;			// ��Ÿö�� ö�ټ��� �и� ���(��ȣ��, �����ֹ�ħ)
	m_bSeparateOutputRebarBMMain	= FALSE;	// ��ü ö�����ǥ �и� ���(������ ������� 2����� �þ)
	m_nTypeOutRebarTable		= 0;			// ö�����ǥ ��¹��(0:��ü,1:��ö��,��Ÿö�ٱ���,2:������)
	m_bRebarTableSepByFy	= TRUE;			// ö�����ǥ fy���� �и��ؼ� ǥ��
	m_nCountRowEachTable		= 70;			// ö�����ǥ ���̺�� �ִ� row��
	m_dSpaceBetweenRebarDetail	= 1000;			// �󼼵��� ����
	m_bMainRebarNumberingInStandardSection	= FALSE;	// ǥ�شܸ鵵�� ��ö�� �ѹ��� ǥ������?
	m_bLengthInfoInMainRebarNumbering		= TRUE;		// ��ö�� ������ ö�� ��ȣ�� ���� ���� ǥ������?
	m_bExcludeDoupleMainRebarCrossSection	= FALSE;	// Ⱦ�� ������ ��½� ��ġ�� ��ö�� ǥ�� ����(�� ġ���� �״�� ǥ��)
	m_bSepFrontAndBackWallRebar	= FALSE;	// ��ü ������ ���� ����� �и� �ؼ� ǥ��
	m_bSepUpperAndLowerFootingRebar	= FALSE;	// ���� ������ ��� �ϸ��� �и��ؼ� ǥ��
	m_bSepUpperAndLowerSlabRebar	= FALSE;	// ��ν����� ������ ��� �ϸ��� �и��ؼ� ǥ��
	m_bSepUpperSlab2DanAll			= FALSE;
	m_bDrawHunchRebarOnly			= FALSE;
	m_nBasePosPlaneSupportRebarDim	= 0;
	m_nDirMarkUpperSlabMainRebar	= 2;	// ��ν����� ��ö�� ������ ö�� ���ü� ����(0 : �������� ����, 1 : �������� ����, 2 : �ڵ�)
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionAps()
{
	m_dwDrawRebarAps				= 0xFFFFFF;
	m_dScaleRebarAps				= 50.0;		// ���ӽ����� ������ ������
	m_dScaleDetailAps				= 20.0;		// �󼼵� ������
	m_dSpaceBetweenRebarDetailAps	= 1000;		// �󼼵��� ����
	m_bRebarTableSepByFyAps	= TRUE;	// ö�����ǥ fy���� �и��ؼ� ǥ��(���ӽ�����)
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionOthers()
{
	m_dwDrawOthers				= 0xFFFFFF;
	m_dScaleRebarOthers			= 50.0;				// �⺻ ������
	m_dScaleDetailGuardWall		= 20.0;				// �� ������
	m_dScaleDetailMonument		= 10.0;				// ������ �� ������
	m_dSpaceBetweenRebarDetailOthers	= 1000;		// �󼼵��� ����
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionShoeArrange()
{
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionBoring()
{
	m_dwDrawBoring					= 0xFFFFFF;
	m_dScaleBoring					= 100.0;
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionCns()
{
	m_dwDrawRebarCns	= 0xFFFFFF;
	m_nTypeCnsRebar		= 0;
	m_dScaleRebarCns	= 50.0;	// ���潽���� ������ ������
	m_dSpaceBetweenRebarDetailCns	= 1000;

}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionHyunTa()
{
	m_dwDrawHyunTa			= 0xFFFFFF;
	m_dScaleHyunTaFront		= 50.0;
	m_dScaleHyunTaSection	= 30.0;
	m_dScaleHyunTaDetailHold	= 30.0;
	m_dScaleHyunTaDetailSpac	= 30.0;
	m_nTypeHyunTaCut		= 1;
	m_nTypeHyunTaVol = 0;
}

void CARcBridgeDrawingOptionData::DefaultDrawingOptionSlabStandardOutput()
{
	m_dwDrawRcStd	= 0xFFFFFF;
	m_dScaleRcStd1Base	= 100;
	m_dScaleRcStd2Base	= 50;
	m_dScaleRcStd3Base	= 30;
}