// WallData.cpp: implementation of the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WallData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWallData::CWallData()
{
	m_nFlag = 0;

	InitWall();

	// �Ǻ� -------------------------------------------------------
	long i = 0; for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverLeft[i]		= 60;	// ��ü ���� �Ǻ�
		m_dCoverRight[i]	= 60;	// ��ü ������ �Ǻ�
		m_dCoverFrontBack[i]	= 60;	// ��ü �����ĸ�
	}
	//------------------------------------------------------- �Ǻ� 

	// ��� ----------------------------------------------------------
	m_bDetailInput23DanMainRebar	= FALSE;
	m_bNotRebar						= FALSE;
	m_nTypeRebarFront				= 1;					// ���鵵 �׸��� Ÿ��(0:�¿�������, 1:�߽ɱ���)
	m_nIdxCTC_MainRebar				= 0;
	m_dCTC_MainRebar				= 250;	// index���� double�� ����(2005.04.25)
	m_bSameOffsetPlaceTAndCBlock	= TRUE;	// ���� ���౸���� ���� �������� ��ġ
	m_bPlacing23DanByStandardOffset	= FALSE;
	m_bPlacingInWallRebarOfCycle24	= TRUE;
	m_nTypeCurveOfVertRebar			= 0;			// ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
	m_bApplyAngleToMainRebarCTC		= FALSE;		// ��ö�� CTC�� �簢�� ���� �Ǿ�� �ϴ���?(��ν����갡 �������� ��ġ��� ������ �Ǿ�� ��)
	m_dRangeSupportRebarForSameUpperSlab	= 0;	// ��ν�����ó�� ���� ��±� ����
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.RemoveAll();
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.Add(CDPoint(0, 0));
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_nCountBlock	= 1;
	m_dDistAddSupportRebarForSameUpperSlab[iLEFT]	= 0;	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	m_dDistAddSupportRebarForSameUpperSlab[iRIGHT]	= 0;	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	m_bRebarLowerSettle[0] = TRUE;
	m_bRebarLowerSettle[1] = TRUE;

	long left = 0; for(left = 0; left < 2; left++)
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_nCountJointMainRebar_Cycle1[left][dan]	= 2;
			m_nCountJointMainRebar_Cycle2[left][dan]	= 2;
		}
	}

	for(left = 0; left < 2; left++)
	{
		m_setRebarDan[left].Init();
	}
		
	m_nIdxCountCycleMainRebar	= 0;			// ��ö�� ����Ŭ ��.
	m_nCountWrapMainRebar		= 4;
	m_nTypeDefaultCycle3		= 0;				// 3cycle ����Ÿ��

	m_dCTC_SupportRebar			= 300;	// index���� double�� ����
	m_dCTC_ShearRebar			= 600;

	m_nTypeOutputMainRebar_Side	= 0;
	//---------------------------------------------------------- ��� 

	m_footing.m_exFooting.m_Pile.m_nTypeOrgDefault	= 0;
	m_footing.m_exFooting.m_Pile.m_bIndvPileInput	= TRUE;

	m_Fck	= 0;
	m_Fy	= 0;
	m_FyShear	= 0;
	m_dLengthRebarMax	= 8000;
	m_sNameRight		= "����";	// ��ü ������ �ش��ϴ� �̸�
	m_sNameLeft			= "����";	// ��ü ������ �ش��ϴ� �̸�
	m_pBasicConcInfo	= NULL;

	m_dCenterOfWall		= 0;				// �����߽� (�������� ����)
	m_tvArrLowerLimit_Front.RemoveAll();
	m_tvArrLowerLimit_FrontRebar.RemoveAll();
}

CWallData::~CWallData()
{
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(long left = 0; left < 2; left++)
		{
			for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				AhTPADelete(&m_pArrRCMain[cycle][left][dan], (CRebarInfoCycle*)0);
			}
		}
	}

	AhTPADelete(&m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	AhTPADelete(&m_pArrRebarMain,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarSupport,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarShear,	(CRebar*)0);
	AhTPADelete(&m_pArrRebarColumn,	(CRebar*)0);
}

void CWallData::Serialize(CArchive &ar)
{
	// 19 : m_tvLowerLimitOfMainRebar
	// 20 : BoxType �߰�.
	// 22 : m_bRebarLowerSettle
	// 23 : m_FyShear
	m_nFlag = 23;
	long i(0), left(0), dan(0);

	if(ar.IsStoring())
	{
		ar << m_nFlag;
		m_footing.Serialize(ar);
		// ��ü���� -------------------------------------------------------
		ar << m_bIs;
		ar << m_xyOrg;
		ar << m_nType;
		ar << m_bRoundRect;		// ����� ��ü����?(����� ������ ��쿡�� ����)
		ar << m_bSideIsArc;
			// �Ϲ�����
		ar << m_dELB;		// ����ϴ� EL
		ar << m_dW;			// ��ü �ʺ�
		ar << m_dH;			// ��ü ����(����)
		ar << m_dAngle;		// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
		ar << m_bApplyAngleEnd;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
			
			// V����
		ar << m_dELRT;		// V�� ������ Top
		ar << m_dELLT;		// V�� ������ Top

		ar << m_dWL;
		ar << m_dWR;
		ar << m_dLR;
		ar << m_dAngleV;		// V������ ���̰�
		ar << m_dR1;			// V�������� ���� �� ������(������� ���߿�)
		ar << m_dR2;			// V�������� ������ �� ������(������� ���߿�)
		m_Column.Serialize(ar);		// ��ü �ܸ��� ��ս��� ��� ��ü��� �� ����� �����.
		ar << m_nCountColumn;		// ��ս��� ��� ��� ����
		for(i = 0; i < m_nCountColumn; i++)
			ar << m_dDistColumn[i];	// �� ��հ��� �Ÿ�
		ar << m_bRevDirPlacingColumn;		// ��� ��ġ ������ �н��� �ݴ��������...
		ar << m_dAnglePath;

			// �������
		ar << m_bTopIsColumn;			// ����� ��ս�����...?
		ar << m_tvPathPlan;			// ���� ��ü�� ���� ��.
		ar << m_dMarginLeft;			// ��ü�� path���� ����ġ���� �̰ݵǴ� �Ÿ�
		ar << m_dMarginRight;			// ��ü�� path���� ������ġ���� �̰ݵǴ� �Ÿ�
		ar << m_bRAngRight;			// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
		ar << m_bRAngLeft;			// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
		ar << m_vAngLeft;				// ������ ����(�����Ⱦ��� 0��)
		ar << m_vAngRight;				// ������ ����(�����Ⱦ��� 0��)
		ar << m_dOffsetStt;				// Path���ۺ� ��������� �������� �� �Ÿ�
		ar << m_dOffsetEnd;				// Path���� ��������� �������� �� �Ÿ�

			// �����鵵 ��� ����
		m_tvArrLeftUpper.Serialize(ar);	// �����鵵 ��� ������ ����
		m_tvArrLeftUpperSlab.Serialize(ar);
		ar << m_dThickUppeSlab;			// ��ν����� �β�.
		ar << m_dThickHunch;			// ��ν����� ��ġ �β�.
		ar << m_dThickLowerSlab;		// �Ϻν����� �β�.
		ar << m_dThichLowerHunch;		// �Ϻν����� ��ġ �β�.

		// ------------------------------------------------------ ��ü����		

		// �Ǻ� -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar << m_dCoverLeft[i];	// ��ü ���� �Ǻ�
			ar << m_dCoverRight[i];	// ��ü ������ �Ǻ�
			ar << m_dCoverFrontBack[i];
		}
		//------------------------------------------------------- �Ǻ� 

		// ��� ----------------------------------------------------------
		ar << m_bDetailInput23DanMainRebar;
		ar << m_bNotRebar;
		ar << m_nIdxCTC_MainRebar;
		ar << m_dCTC_MainRebar;	// index���� double�� ����(2005.04.25)
		ar << m_bSameOffsetPlaceTAndCBlock;	// ���� ���౸���� ���� �������� ��ġ
		ar << m_bPlacing23DanByStandardOffset;
		ar << m_bPlacingInWallRebarOfCycle24;	// 2,4����Ŭ�� ���ؼ� ö���� ��������...??
		ar << m_nTypeCurveOfVertRebar;			// ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
		ar << m_bApplyAngleToMainRebarCTC;		// ��ö�� CTC�� �簢�� ���� �Ǿ�� �ϴ���?(��ν����갡 �������� ��ġ��� ������ �Ǿ�� ��)
		ar << m_dRangeSupportRebarForSameUpperSlab;	// ��ν�����ó�� ���� ��±� ����
		m_placeByBlock_SupportRebarAddSameUpperSlab.Serialize(ar);	// ��ν����� ó�� ���� ��±��� �߰��� ����
		for(left = 0; left < 2; left++)
		{
			ar << m_dDistAddSupportRebarForSameUpperSlab[left];	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
		}
		ar << m_bRebarLowerSettle[0];
		ar << m_bRebarLowerSettle[1];
		for(left = 0; left < 2; left++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				ar << m_nCountJointMainRebar_Cycle1[left][dan];
				ar << m_nCountJointMainRebar_Cycle2[left][dan];
			}
		}
		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					SerializeDoubleArray(ar, &m_dArrDistRC[cycle][left][dan]);
					SerializeDoubleArray(ar, &m_dArrDiaRC[cycle][left][dan]);
					SerializeDoubleArray(ar, &m_dArrExistRC[cycle][left][dan]);
				}
			}
		}

		for(left = 0; left < 2; left++)
		{
			m_setRebarDan[left].Serialize(ar);
		}
	
		ar << m_tvUpperLimitOfMainRebar ;	// ����� ��ö���� ��� �Ѱ�(RcBridge������ ��ν����� ��� ��ö�� 1���� ��ǥ�� �ǹ�)
		ar << m_nIdxCountCycleMainRebar;			// ��ö�� ����Ŭ ��.
		ar << m_nCountWrapMainRebar;
		ar << m_nTypeDefaultCycle3;				// 3cycle ����Ÿ��

		ar << m_dCoverLower_LowerSlab;
		ar << m_tvLowerLimitOfMainRebar;
		ar << m_tvLowerLimitOfSupportRebar;

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_placeByBlock_SupportRebar[left][dan].Serialize(ar);
			}
		}
		m_placeByBlock_ShearRebar.Serialize(ar);
		ar << m_nIdxCTC_SupportAndShearRebar;
		ar << m_dCTC_SupportRebar;	// index���� double�� ����
		ar << m_dCTC_ShearRebar;
		m_rebarShearDetail.Serialize(ar);
		ar << m_nTypeOutputMainRebar_Side;
		//---------------------------------------------------------- ��� 
		ar << m_Fck;
		ar << m_Fy;
		ar << m_FyShear;
		ar << m_dLengthRebarMax;

		AhTPASerial(ar, &m_pArrRebarMain, (CRebar*)0);	// ��ö��
		AhTPASerial(ar, &m_pArrRebarSupport, (CRebar*)0);	// ���ö��
		AhTPASerial(ar, &m_pArrRebarShear, (CRebar*)0);	// ����ö��
		AhTPASerial(ar, &m_pArrRebarColumn, (CRebar*)0);	// ��Ÿö��

		// �ӵ�������
		ar << m_xyOrgOld;
	}
	else
	{
		ar >> m_nFlag;
		m_footing.Serialize(ar);
		// ��ü���� -------------------------------------------------------
		ar >> m_bIs;
		ar >> m_xyOrg;
		ar >> m_nType;
		if(m_nFlag > 5)
			ar >> m_bRoundRect;		// ����� ��ü����?(����� ������ ��쿡�� ����)
		if(m_nFlag > 14)
			ar >> m_bSideIsArc;
			// �Ϲ�����
		ar >> m_dELB;		// ����ϴ� EL
		ar >> m_dW;			// ��ü �ʺ�
		ar >> m_dH;			// ��ü ����(����)
		ar >> m_dAngle;		// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
		ar >> m_bApplyAngleEnd;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
			
			// V����
		ar >> m_dELRT;		// V�� ������ Top
		ar >> m_dELLT;		// V�� ������ Top

		ar >> m_dWL;
		ar >> m_dWR;
		ar >> m_dLR;
		ar >> m_dAngleV;		// V������ ���̰�
		ar >> m_dR1;			// V�������� ���� �� ������(������� ���߿�)
		ar >> m_dR2;			// V�������� ������ �� ������(������� ���߿�)
		m_Column.Serialize(ar);		// ��ü �ܸ��� ��ս��� ��� ��ü��� �� ����� �����.
		ar >> m_nCountColumn;		// ��ս��� ��� ��� ����
		for(i = 0; i < m_nCountColumn; i++)
			ar >> m_dDistColumn[i];	// �� ��հ��� �Ÿ�
		ar >> m_bRevDirPlacingColumn;		// ��� ��ġ ������ �н��� �ݴ��������...
		ar >> m_dAnglePath;			// ������Ʈ�����߰�

			// �������
		ar >> m_bTopIsColumn;			// ����� ��ս�����...?
		ar >> m_tvPathPlan;		// ���� ��ü�� ���� ��.
		ar >> m_dMarginLeft;			// ��ü�� path���� ����ġ���� �̰ݵǴ� �Ÿ�
		ar >> m_dMarginRight;			// ��ü�� path���� ������ġ���� �̰ݵǴ� �Ÿ�
		ar >> m_bRAngRight;			// Path�� ���ۺο� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
		ar >> m_bRAngLeft;			// path�� ���ο� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
		ar >> m_vAngLeft;				// ������ ����(�����Ⱦ��� 0��)
		ar >> m_vAngRight;				// ������ ����(�����Ⱦ��� 0��)
		ar >> m_dOffsetStt;				// Path���ۺ� ��������� �������� �� �Ÿ�
		ar >> m_dOffsetEnd;				// Path���� ��������� �������� �� �Ÿ�

			// �����鵵 ��� ����
		m_tvArrLeftUpper.Serialize(ar);	// �����鵵 ��� ������ ����
		if(m_nFlag > 7)
			m_tvArrLeftUpperSlab.Serialize(ar);
		ar >> m_dThickUppeSlab;			// ��ν����� �β�.
		if(m_nFlag > 2)
			ar >> m_dThickHunch;			// ��ν����� ��ġ �β�.
		if(m_nFlag > 19)
		{
			ar >> m_dThickLowerSlab;		// �Ϻν����� �β�.
			ar >> m_dThichLowerHunch;		// �Ϻν����� ��ġ �β�.
		}

		// ------------------------------------------------------ ��ü����		

		// �Ǻ� -------------------------------------------------------
		for(i = 0; i < MAX_REBAR_DAN; i++)
		{
			ar >> m_dCoverLeft[i];	// ��ü ���� �Ǻ�
			ar >> m_dCoverRight[i];	// ��ü ������ �Ǻ�
			if(m_nFlag > 17)
				ar >> m_dCoverFrontBack[i];
			else
				m_dCoverFrontBack[i]	= m_dCoverLeft[i];
		}
		//------------------------------------------------------- �Ǻ� 

		// ��� ----------------------------------------------------------
		if(m_nFlag > 16)
			ar >> m_bDetailInput23DanMainRebar;
		if(m_nFlag > 13)
			ar >> m_bNotRebar;
		ar >> m_nIdxCTC_MainRebar;
		if(m_nFlag > 0)
			ar >> m_dCTC_MainRebar;	// index���� double�� ����(2005.04.25)
		ar >> m_bSameOffsetPlaceTAndCBlock;		// ���� ���౸���� ���� �������� ��ġ
		if(m_nFlag > 11)
			ar >> m_bPlacing23DanByStandardOffset;
		ar >> m_bPlacingInWallRebarOfCycle24;	// 2,4����Ŭ�� ���ؼ� ö���� ��������...?? �ø����߰�
		ar >> m_nTypeCurveOfVertRebar;			// ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
		if(m_nFlag > 3)
			ar >> m_bApplyAngleToMainRebarCTC;		// ��ö�� CTC�� �簢�� ���� �Ǿ�� �ϴ���?(��ν����갡 �������� ��ġ��� ������ �Ǿ�� ��)
		if(m_nFlag > 9)
			ar >> m_dRangeSupportRebarForSameUpperSlab;	// ��ν�����ó�� ���� ��±� ����
		if(m_nFlag > 15)
		{
			m_placeByBlock_SupportRebarAddSameUpperSlab.Serialize(ar);
			for(left = 0; left < 2; left++)
			{
				ar >> m_dDistAddSupportRebarForSameUpperSlab[left];	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
			}
		}
		if(m_nFlag > 20)
			ar >> m_bRebarLowerSettle[0];
		if(m_nFlag > 21)
			ar >> m_bRebarLowerSettle[1];

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				if(m_nFlag > 16 || dan == 0)
				{
					ar >> m_nCountJointMainRebar_Cycle1[left][dan];
					if(m_nFlag > 8)
						ar >> m_nCountJointMainRebar_Cycle2[left][dan];
				}
			}
		}

		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			for(left = 0; left < 2; left++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					if(m_nFlag > 16 || dan == 0)
					{
						SerializeDoubleArray(ar, &m_dArrDistRC[cycle][left][dan]);
						SerializeDoubleArray(ar, &m_dArrDiaRC[cycle][left][dan]);
						if(m_nFlag > 8)
							SerializeDoubleArray(ar, &m_dArrExistRC[cycle][left][dan]);
					}
				}
			}
		}

		for(left = 0; left < 2; left++)
		{
			m_setRebarDan[left].Serialize(ar);
		}

		ar >> m_tvUpperLimitOfMainRebar ;			// ����� ��ö���� ��� �Ѱ�(RcBridge������ ��ν����� ��� ��ö�� 1���� ��ǥ�� �ǹ�)
		ar >> m_nIdxCountCycleMainRebar;			// ��ö�� ����Ŭ ��.
		ar >> m_nCountWrapMainRebar;
		if(m_nFlag > 1)
			ar >> m_nTypeDefaultCycle3;				// 3cycle ����Ÿ��

		if(m_nFlag > 18)
		{
			ar >> m_dCoverLower_LowerSlab;
			ar >> m_tvLowerLimitOfMainRebar;
			ar >> m_tvLowerLimitOfSupportRebar;
		}

		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_placeByBlock_SupportRebar[left][dan].Serialize(ar);
			}
		}
		m_placeByBlock_ShearRebar.Serialize(ar);
		ar >> m_nIdxCTC_SupportAndShearRebar;
		if(m_nFlag > 0)
			ar >> m_dCTC_SupportRebar;	// index���� double�� ����
		if(m_nFlag > 12)
			ar >> m_dCTC_ShearRebar;
		else
			m_dCTC_ShearRebar	= m_dCTC_SupportRebar;
		if(m_nFlag > 6)
			m_rebarShearDetail.Serialize(ar);
		if(m_nFlag > 4)
			ar >> m_nTypeOutputMainRebar_Side;
		//---------------------------------------------------------- ��� 
		ar >> m_Fck;
		ar >> m_Fy;
		if(m_nFlag >22)
			ar >> m_FyShear;
		else
			m_FyShear = m_Fy;

		ar >> m_dLengthRebarMax;

		AhTPASerial(ar, &m_pArrRebarMain, (CRebar*)0);	// ��ö��
		AhTPASerial(ar, &m_pArrRebarSupport, (CRebar*)0);	// ���ö��
		AhTPASerial(ar, &m_pArrRebarShear, (CRebar*)0);	// ����ö��
		AhTPASerial(ar, &m_pArrRebarColumn, (CRebar*)0);	// ��Ÿö��

		if(m_nFlag > 10)
			ar >> m_xyOrgOld;
	}

	CheckSerialize();
}

CWallData& CWallData::operator=(const CWallData& Obj)
{
	long i(0), dan(0);
	m_tvArrLowerLimit_Front	= Obj.m_tvArrLowerLimit_Front;
	m_tvArrLowerLimit_FrontRebar	= Obj.m_tvArrLowerLimit_FrontRebar;
	// ��ü���� -------------------------------------------------------
	m_footing	= Obj.m_footing;
	m_bIs		= Obj.m_bIs;
	m_xyOrg		= Obj.m_xyOrg;
	m_nType		= Obj.m_nType;
	m_bRoundRect	= Obj.m_bRoundRect;		// ����� ��ü����?(����� ������ ��쿡�� ����)
	m_bSideIsArc	= Obj.m_bSideIsArc;
		// �Ϲ�����
	m_dDiffTop[0]	= Obj.m_dDiffTop[0];
	m_dDiffTop[1]	= Obj.m_dDiffTop[1];
	m_dELBDiff	= Obj.m_dELBDiff;
	m_dELB		= Obj.m_dELB;		// ����ϴ� EL
	m_dW		= Obj.m_dW;			// ��ü �ʺ�
	m_dH		= Obj.m_dH;			// ��ü ����(����)
	m_dAngle	= Obj.m_dAngle;		// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
	m_bApplyAngleEnd	= Obj.m_bApplyAngleEnd;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
		
		// V����
	m_dELRT		= Obj.m_dELRT;		// V�� ������ Top
	m_dELLT		= Obj.m_dELLT;		// V�� ������ Top

	m_dWL		= Obj.m_dWL;
	m_dWR		= Obj.m_dWR;
	m_dLR		= Obj.m_dLR;
	m_dAngleV	= Obj.m_dAngleV;		// V������ ���̰�
	m_dR1		= Obj.m_dR1;			// V�������� ���� �� ������(������� ���߿�)
	m_dR2		= Obj.m_dR2;			// V�������� ������ �� ������(������� ���߿�)

	// ��ս��� ������ġ�� ��ü�� ������ġ���� ������ �Ÿ��� ������ �Ǵ���.
	m_Column		= Obj.m_Column;		// ��ü �ܸ��� ��ս��� ��� ��ü��� �� ����� �����.
	m_nCountColumn	= Obj.m_nCountColumn;	// ��ս��� ��� ��� ����
	for(i = 0; i < MAX_COLUMN; i++)
		m_dDistColumn[i]	= Obj.m_dDistColumn[i];		// �� ��հ��� �Ÿ�
	m_bRevDirPlacingColumn	= Obj.m_bRevDirPlacingColumn;		// ��� ��ġ ������ �н��� �ݴ��������...
	m_dAnglePath			= Obj.m_dAnglePath;
		
	// �������
	m_bTopIsColumn	= Obj.m_bTopIsColumn;			// ����� ��ս�����...?
	m_tvPathPlan	= Obj.m_tvPathPlan;		// ���� ��ü�� ���� ��.
	m_dMarginLeft	= Obj.m_dMarginLeft;			// ��ü�� path���� ����ġ���� �̰ݵǴ� �Ÿ�
	m_dMarginRight	= Obj.m_dMarginRight;			// ��ü�� path���� ������ġ���� �̰ݵǴ� �Ÿ�
	m_bRAngLeft		= Obj.m_bRAngLeft;			// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	m_bRAngRight	= Obj.m_bRAngRight;			// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	m_vAngLeft		= Obj.m_vAngLeft;				// ������ ����(�����Ⱦ��� 0��)
	m_vAngRight		= Obj.m_vAngRight;				// ������ ����(�����Ⱦ��� 0��)
	m_dOffsetStt	= Obj.m_dOffsetStt;				// Path���ۺ� ��������� �������� �� �Ÿ�
	m_dOffsetEnd	= Obj.m_dOffsetEnd;				// Path���� ��������� �������� �� �Ÿ�

	// �����鵵 ��� ����
	m_tvArrLeftUpper	= Obj.m_tvArrLeftUpper;		// �����鵵 ��� ������ ����
	m_tvArrLeftUpperSlab	= Obj.m_tvArrLeftUpperSlab;
	m_dDiffHLeftByCenter	= Obj.m_dDiffHLeftByCenter;		// ��ü��� �߽ɰ� �������� ����
	m_dDiffHRightByCenter	= Obj.m_dDiffHRightByCenter;		// ��ü��� �߽ɰ� �������� ����

	m_dThickUppeSlab	= Obj.m_dThickUppeSlab;		// ��ν����� �β�.
	m_dThickHunch		= Obj.m_dThickHunch;		// ��ν����� ��ġ �β�.
	m_dThickHunchL		= Obj.m_dThickHunchL;				// ��ν����� ��ġ �β�.����
	m_dThickHunchR		= Obj.m_dThickHunchR;				// ��ν����� ��ġ �β�.����

	m_dThickLowerSlab	= Obj.m_dThickLowerSlab;	// �Ϻν����� �β�.
	m_dThichLowerHunch  = Obj.m_dThichLowerHunch;	// �Ϻν����� ��ġ �β�.
	// ------------------------------------------------------ ��ü����

	// �Ǻ� -------------------------------------------------------
	for(i = 0; i < MAX_REBAR_DAN; i++)
	{
		m_dCoverLeft[i]	= Obj.m_dCoverLeft[i];	// ��ü ���� �Ǻ�
		m_dCoverRight[i]	= Obj.m_dCoverRight[i];	// ��ü ������ �Ǻ�
		m_dCoverFrontBack[i]	= Obj.m_dCoverFrontBack[i];
	}
	//------------------------------------------------------- �Ǻ� 

	// ��� ----------------------------------------------------------
	m_bDetailInput23DanMainRebar	= Obj.m_bDetailInput23DanMainRebar;
	m_bNotRebar						= Obj.m_bNotRebar;
	m_nTypeRebarFront				= Obj.m_nTypeRebarFront;					// ���鵵 �׸��� Ÿ��(0:�¿�������, 1:�߽ɱ���)
	m_nIdxCTC_MainRebar				= Obj.m_nIdxCTC_MainRebar;
	m_dCTC_MainRebar				= Obj.m_dCTC_MainRebar;	// index���� double�� ����(2005.04.25)
	m_bSameOffsetPlaceTAndCBlock	= Obj.m_bSameOffsetPlaceTAndCBlock;	// ���� ���౸���� ���� �������� ��ġ
	m_bPlacing23DanByStandardOffset	= Obj.m_bPlacing23DanByStandardOffset;
	m_bPlacingInWallRebarOfCycle24	= Obj.m_bPlacingInWallRebarOfCycle24;	// 2,4����Ŭ�� ���ؼ� ö���� ��������...??
	m_nTypeCurveOfVertRebar			= Obj.m_nTypeCurveOfVertRebar;			// ����ö�� ����� Ÿ��(0 : �ڵ�, 1 : �, 2 : ����)
	m_bApplyAngleToMainRebarCTC		= Obj.m_bApplyAngleToMainRebarCTC;		// ��ö�� CTC�� �簢�� ���� �Ǿ�� �ϴ���?(��ν����갡 �������� ��ġ��� ������ �Ǿ�� ��)
	m_dRangeSupportRebarForSameUpperSlab	= Obj.m_dRangeSupportRebarForSameUpperSlab;	// ��ν�����ó�� ���� ��±� ����
	m_placeByBlock_SupportRebarAddSameUpperSlab	= Obj.m_placeByBlock_SupportRebarAddSameUpperSlab;
	long left = 0;
	for(left = 0; left < 2; left++)
	{
		m_dDistAddSupportRebarForSameUpperSlab[left]	= Obj.m_dDistAddSupportRebarForSameUpperSlab[left];	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	}

	for(left = 0; left < 2; left++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_nCountJointMainRebar_Cycle1[left][dan]	= Obj.m_nCountJointMainRebar_Cycle1[left][dan];
			m_nCountJointMainRebar_Cycle2[left][dan]	= Obj.m_nCountJointMainRebar_Cycle2[left][dan];
		}
		m_bRebarLowerSettle[left] = Obj.m_bRebarLowerSettle[left];
	}

	long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				AhTPACopy(&m_pArrRCMain[cycle][left][dan], &Obj.m_pArrRCMain[cycle][left][dan], (CRebarInfoCycle*)0);	// �¿����� rebar info cycle
		}
	}
	

	for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(left = 0; left < 2; left++)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_dArrDistRC[cycle][left][dan]	= Obj.m_dArrDistRC[cycle][left][dan];
				m_dArrDiaRC[cycle][left][dan]	= Obj.m_dArrDiaRC[cycle][left][dan];
				m_dArrExistRC[cycle][left][dan]	= Obj.m_dArrExistRC[cycle][left][dan];
			}
		}
	}
	for(left = 0; left < 2; left++)
	{
		m_setRebarDan[left]	= Obj.m_setRebarDan[left];
	}
	
	m_dCoverUpper_UpperSlab		= Obj.m_dCoverUpper_UpperSlab;
	m_tvUpperLimitOfMainRebar	= Obj.m_tvUpperLimitOfMainRebar ;	// ����� ��ö���� ��� �Ѱ�(RcBridge������ ��ν����� ��� ��ö�� 1���� ��ǥ�� �ǹ�)
	m_nIdxCountCycleMainRebar	= Obj.m_nIdxCountCycleMainRebar;	// ��ö�� ����Ŭ ��.
	m_nCountWrapMainRebar		= Obj.m_nCountWrapMainRebar;
	m_nTypeDefaultCycle3		= Obj.m_nTypeDefaultCycle3;				// 3cycle ����Ÿ��

	m_dCoverLower_LowerSlab		= Obj.m_dCoverLower_LowerSlab;
	m_tvLowerLimitOfMainRebar	= Obj.m_tvLowerLimitOfMainRebar;
	m_tvLowerLimitOfSupportRebar	= Obj.m_tvLowerLimitOfSupportRebar;

	for(left = 0; left < 2; left++)
	{
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_placeByBlock_SupportRebar[left][dan]	= Obj.m_placeByBlock_SupportRebar[left][dan];
		}
	}
	m_placeByBlock_ShearRebar		= Obj.m_placeByBlock_ShearRebar;
	m_nIdxCTC_SupportAndShearRebar	= Obj.m_nIdxCTC_SupportAndShearRebar;
	m_dCTC_SupportRebar				= Obj.m_dCTC_SupportRebar;	// index���� double�� ����
	m_dCTC_ShearRebar				= Obj.m_dCTC_ShearRebar;
	m_rebarShearDetail				= Obj.m_rebarShearDetail;
	m_nTypeOutputMainRebar_Side		= Obj.m_nTypeOutputMainRebar_Side;	

	// ö�� ����
	AhTPACopy(&m_pArrRebarMain, &Obj.m_pArrRebarMain, (CRebar*)0);			// ��ö��
	AhTPACopy(&m_pArrRebarSupport, &Obj.m_pArrRebarSupport, (CRebar*)0);	// ���ö��
	AhTPACopy(&m_pArrRebarShear, &Obj.m_pArrRebarShear, (CRebar*)0);		// ����ö��
	AhTPACopy(&m_pArrRebarColumn, &Obj.m_pArrRebarColumn, (CRebar*)0);		// ��պ� ö��	
	//---------------------------------------------------------- ��� 
	m_Fck					= Obj.m_Fck;
	m_Fy					= Obj.m_Fy;
	m_FyShear				= Obj.m_FyShear;
	m_dLengthRebarMax		= Obj.m_dLengthRebarMax;

	return *this;
}

// ��ü ���� �ʱ�ȭ
void CWallData::InitWall()
{
	long i = 0;

	m_dWidthExp[0]	= 0;
	m_dWidthExp[1]	= 0;
	m_dWidthOverRebarExp[0] = 0;
	m_dWidthOverRebarExp[1] = 0;
	m_dAngleUpperSlab_Front	= 0;
	m_dAngleUpperWall_Front	= 0;

	// ��ü���� -------------------------------------------------------
	m_bIs		= TRUE;
	m_xyOrg		= CDPoint(0, 0);
	m_nType		= WALL_TYPE_VER;
	m_bRoundRect	= FALSE;		// ����� ��ü����?(����� ������ ��쿡�� ����)
	m_bSideIsArc	= FALSE;

		// �Ϲ�����
	m_dDiffTop[0]	= 0;
	m_dDiffTop[1]	= 0;
	m_dELBDiff	= 0;
	m_dELB		= 0;			// ����ϴ� EL
	m_dW		= 0;			// ��ü �ʺ�
	m_dH		= 0;			// ��ü ����(����)
	m_dAngle	= 225;			// ��ü�� ������ ����(�Ϲ����� : 190 ~ 350���� ����)
	m_bApplyAngleEnd	= TRUE;	// ��ü�� ���� ������ ���κ� ���ʿ� ���缭 ����ó�� ����..
		
		// V����
	m_dELRT		= 0;			// V�� ������ Top
	m_dELLT		= 0;			// V�� ������ Top

	m_dWL		= 0;
	m_dWR		= 0;
	m_dLR		= 0;
	m_dAngleV	= 0;			// V������ ���̰�
	m_dR1		= 0;			// V�������� ���� �� ������(������� ���߿�)
	m_dR2		= 0;			// V�������� ������ �� ������(������� ���߿�)

		// �������
	// ��ս��� ������ġ�� ��ü�� ������ġ���� ������ �Ÿ��� ������ �Ǵ���.
	m_Column.m_bIs = TRUE;				// ��ü �ܸ��� ��ս��� ��� ��ü��� �� ����� �����.
	m_nCountColumn	= 3;				// ��ս��� ��� ��� ����
	for(i = 0; i < MAX_COLUMN; i++)
		m_dDistColumn[i] = 3000;		// �� ��հ��� �Ÿ�
	m_bRevDirPlacingColumn	= TRUE;		// ��� ��ġ ������ �н��� �ݴ��������...
	m_dAnglePath	= 90;

	m_bTopIsColumn	= FALSE;			// ����� ��ս�����...?
	m_dMarginLeft	= 0;				// ��ü�� path���� ����ġ���� �̰ݵǴ� �Ÿ�
	m_dMarginRight	= 0;				// ��ü�� path���� ������ġ���� �̰ݵǴ� �Ÿ�
	m_bRAngLeft = FALSE;				// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	m_bRAngRight = FALSE;				// �����ܿ� �������� ����?(���� �Ⱦ��� ������ Ȥ�ó� �ؼ� ����� ����)
	m_vAngLeft	= CDPoint(1, 0);		// ������ ����(�����Ⱦ��� 0��)
	m_vAngRight	= CDPoint(1, 0);		// ������ ����(�����Ⱦ��� 0��)
	m_dOffsetStt	= 0;				// Path���ۺ� ��������� �������� �� �Ÿ�
	m_dOffsetEnd	= 0;				// Path���� ��������� �������� �� �Ÿ�

		// �����鵵 ��� ����
	m_tvArrLeftUpper.RemoveAll();		// �����鵵 ��� ������ ����
	m_tvArrLeftUpperSlab.RemoveAll();
	m_dDiffHLeftByCenter	= 0;		// ��ü��� �߽ɰ� �������� ����
	m_dDiffHRightByCenter	= 0;		// ��ü��� �߽ɰ� �������� ����

	m_dThickUppeSlab	= 0;			// ��ν����� �β�.
	m_dThickHunch		= 0;			// ��ν����� ��ġ �β�.
	m_dThickHunchL		= 0;				// ��ν����� ��ġ �β�.����
	m_dThickHunchR		= 0;				// ��ν����� ��ġ �β�.����

	m_dThickLowerSlab	= 0;			//�Ϻν����� �β�.
	m_dThichLowerHunch	= 0;			//�Ϻν����� ��ġ �β�.
	// ------------------------------------------------------ ��ü����

	m_tvArrRebarMainSideForDim.RemoveAll();
}

// ���ö�� ��� �Է³��� ����
void CWallData::CopyPlaceRebar_SupportRebar(CWallData *pWall)
{
	if(!pWall) return;

	long left = 0; for(left = 0; left < 2; left++)
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			m_placeByBlock_SupportRebar[left][dan]	= pWall->m_placeByBlock_SupportRebar[left][dan];
	}
}

void CWallData::CopyPlaceRebar_ShearRebar(CWallData *pWall)
{
	if(!pWall) return;

	m_placeByBlock_ShearRebar	= pWall->m_placeByBlock_ShearRebar;
}

// ��ü �Ǻ�(�¿����� �Է� ������, �յڴ� �Է¹��� ����)
double CWallData::GetCoverFront(BOOL bFront, long nDan)
{
	if(nDan < 0 || nDan >= MAX_REBAR_DAN) nDan = 0;
	return m_dCoverFrontBack[nDan];
}

// ��ü �Ǻ�(�¿����� �Է� ������, ���ϴ� �Է¹��� ����)
// BOOL bApplyVertForSlope : ����� ��ü�� ���������� �Ǻ�
double CWallData::GetCoverUpper(BOOL bUpper, long nDan, BOOL bApplyVertForSlope)
{
	if(nDan < 0 || nDan >= MAX_REBAR_DAN) nDan = 0;
	double dCover	= bUpper ? m_dCoverUpper_UpperSlab : m_footing.m_dCoverLower[nDan];

	if(!bUpper && !m_footing.m_bIs && !m_footing.IsVirtualFooting())
	{
		dCover = m_dCoverLower_LowerSlab;
	}
	if(bApplyVertForSlope && m_nType == WALL_TYPE_SLOPE)
	{
		double dA	= m_dAngle < 90 ?  90 - m_dAngle : m_dAngle - 90;
		dA	= 90 - dA;
		dCover	= dCover / sin(ToRadian(dA));
	}

	return dCover;
}

// ��ö�ٿ� ���ؼ� ��ũ ������.
// bInit �̸� �ʱ�ȭ�� �Ѵ�.
// �׷��� ���� ��� ������ ���� ����
/*// ö�ٸ�ũ ������ ���� ���� ���� ///////////////////////////////
#define MARK_LEFT	0	// ���� ö�� ��ũ
#define MARK_RIGHT	1	// ���� ö�� ��ũ
/////////////////////////////////////////////////////////////////*/
CString CWallData::GetMarkRebarMain(long nType, BOOL bInput, long nCycle, BOOL bOnlyInit, BOOL bForBM, CString sDescription, long nDan)
{
	static long nNum	= 1;
	if(bOnlyInit) 
	{
		nNum		= 1;
		return _T("");
	}

	CString sMark		= _T("");
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;

		long i = 0; for(i = 0; i < m_pArrRebarMain.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarMain.GetAt(i);
			if(pRB->m_sDescription == sDescription)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}
	
	switch(nType)
	{
	case MARK_LEFT:		sMark	= bInput ? "WL" : "IW"; break;
	case MARK_RIGHT:	sMark	= bInput ? "WR" : "IW"; break;
	default: break;
	}

	if(!bInput || nDan==0)
	{
		sMark	= sMark + COMMA(nNum);
		nNum++;
	}

	return sMark;
}


long CWallData::GetCountCycleMainRebar()
{
	long nCountCycle	= 1;
	if(m_nIdxCountCycleMainRebar == 0) nCountCycle	= 2;
	else if(m_nIdxCountCycleMainRebar == 1) nCountCycle	= 4;
	
	return nCountCycle;
}

// �� �Լ��� CHgBaseBM���� ������ �Լ���
// CHgBaseBM�� �������� �°ݵǴ³� �� �Լ��� ���ŵǰ� CHgBaseBM���� ���� ������ ���� �� ���̴�.
CString CWallData::GetCircleText(long nNumber)
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

	if(nNumber <= 0 || nNumber >= nCount) return "";
	return sCircleText[nNumber-1];
}

CString CWallData::GetMarkRebarColumn(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;
		long i = 0; for(i = 0; i < m_pArrRebarColumn.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarColumn.GetAt(i);
			if(pRB->m_sDescription == sDescription)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("C");	
	
	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}
// serialize�� �ѵ� Ȥ�ó� flag�� ������ ���� ������ �ùٸ��� ���� ����Ÿ�� ������ 
// ���� �޽����� ������, �ʱ�ȭ�� �ؼ��� �ٷ� ������ �ִ°� �ٷ� ��� �ش�.
// Ư�� ö�� ���谰����..
void CWallData::CheckSerialize()
{

}

// long nBaseNum == -1 �̸� �������� �״�� ���
// -1�� �ƴϸ� nBaseNum������ ���ø� �ѵ� ����
CString CWallData::GetMarkRebarSupport(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") 
		{
			if(nType > -1 && nType < m_pArrRebarSupport.GetSize())
			{
				CRebar *pRB	= m_pArrRebarSupport.GetAt(nType);
				sMark	= pRB->m_sMark;
			}
			return sMark;
		}
		long i = 0; for(i = 0; i < m_pArrRebarSupport.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarSupport.GetAt(i);
			if(pRB->m_sDescription.Find(sDescription, 0) != -1)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("");	
	
	switch(nType)
	{
		case W_REBAR_S_L1 : sMark	= bInput ? "L1" : "D"; break;	// ���ö�� ���� 1��.
		case W_REBAR_S_L2 : sMark	= bInput ? "L2" : "D"; break;	// ���ö�� ���� 2��.
		case W_REBAR_S_L3 : sMark	= bInput ? "L3" : "D"; break;	// ���ö�� ���� 3��.
		case W_REBAR_S_R1 : sMark	= bInput ? "R1" : "D"; break;	// ���ö�� ���� 1��.
		case W_REBAR_S_R2 : sMark	= bInput ? "R2" : "D"; break;	// ���ö�� ���� 2��.
		case W_REBAR_S_R3 : sMark	= bInput ? "R3" : "D"; break;	// ���ö�� ���� 3��.
	}

	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}

CString CWallData::GetMarkRebarShear(long nType, BOOL bInput, BOOL bForBM, long nBaseNum, CString sDescription)
{
	static long nNum	= 1;
	if(nBaseNum != -1)
	{
		nNum	= nBaseNum;
		return "";
	}
	if(!bInput && !bForBM)
	{
		CString sMark	= _T("");

		if(sDescription == "") return sMark;
		long i = 0; for(i = 0; i < m_pArrRebarShear.GetSize(); i++)
		{
			CRebar *pRB	= m_pArrRebarShear.GetAt(i);
			if(pRB->m_sDescription.Find(sDescription, 0) != -1)
			{
				sMark	= pRB->m_sMark;
				break;
			}
		}
		return sMark;
	}

	CString sMark	= _T("");	
	
	switch(nType)
	{
		case W_REBAR_V : sMark	= bInput ? "V" : "S"; break;	// ����ö��
	}

	if(!bInput)
	{
		sMark	+= COMMA(nNum);
		nNum++;
	}

	return sMark;
}

void CWallData::SerializeDoubleArray(CArchive &ar, CDoubleArray *dArr)
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
		ar >> nSize;
		dArr->RemoveAll();

		long i = 0; for(i = 0; i < nSize; i++)
		{
			double d	= 0;

			ar >> d;
			dArr->Add(d);
		}
	}
}


// ���� �߽�(��ٽ� ������ �Ǵ� ��ġ)
double CWallData::GetCrossSectionCenterOfRoad(BOOL bLeft)
{
	double dMid	= 0;
	if(!bLeft)
	{
		dMid	= m_dCenterOfWall + m_dCenterOfWall;
	}

	return dMid;
}

BOOL CWallData::GetXyTanOfArc(CDPoint xyOrg, CDPoint vAng1, CDPoint vAng2, double dR, CDPoint &xyCen, CDPoint &xyArc1, CDPoint &xyArc2)
{
	CRebarPlacing rb;

	if(Compare(dR, 0.0, "="))
	{
		xyArc1	= xyOrg;
		xyArc2	= xyOrg;
		xyCen	= xyOrg;
		return FALSE;
	}

	double dAng1	= vAng1.GetAngleDegree();
	double dAng2	= vAng2.GetAngleDegree();
	double dAngMid	= dAng1>dAng2 ? ((dAng1+dAng2)/2) : ((dAng1+360+dAng2)/2);
	CDPoint vAngMid	= ToDPointFrDegree(dAngMid);

	BOOL bLeft		= TRUE;	// vAng1�� vAng2�� ���ʿ� �ִ��� �Ǵ�.
	if(rb.IsIncludeAngle(dAng1, dAng2, dAng2+180)) bLeft = TRUE;
	else bLeft	= FALSE;

	CTwinVector tvLineOrg;	// ��� �߽����� �� ���� �հ� �ȴ�.
	tvLineOrg.m_v1	= xyOrg;
	tvLineOrg.m_v2	= xyOrg + (vAng2 * 1000);
	Offset(tvLineOrg.m_v1, tvLineOrg.m_v2, bLeft ? -dR : dR);

	// �߽���ǥ ����.
	GetXyMatchLineAndLine(xyOrg, vAngMid, tvLineOrg.m_v1, tvLineOrg.GetXyDir(), xyCen);

	// ù��° ��(�߽ɿ��� ù��° ���� �������� ����)
	GetXyMatchLineAndLine(xyCen, vAng1.Rotate90(), xyOrg, vAng1, xyArc1);

	// �ι�° ��(�߽ɿ��� �ι�° ���� �������� ����)
	GetXyMatchLineAndLine(xyCen, vAng2.Rotate90(), xyOrg, vAng2, xyArc2);

	return TRUE;
}

// ������ �簢 �������� ����� ������ ������
double CWallData::GetAngleByAngle(double dAngle, double dAngleApply)
{
	if(dAngleApply == 90) return dAngle;

	double dLen		= 100;
	CDPoint xyTop	= CDPoint(0, 0);
	CDPoint xyAngle	= xyTop + ToDPointFrDegree(dAngle) * dLen;
	
	CDPoint vAng	= ToDPointFrDegree(dAngleApply);
	xyAngle.x		= xyAngle.x / vAng.y;

	dAngle			= (xyAngle - xyTop).Unit().GetAngleDegree();

	/*
	double dLen		= 100;
	CVector vTop	= CVector(0, 0, 0);
	CVector vLeft	= vTop + ToDPointFrDegree(180) * dLen;
	CVector vRight	= vTop + ToDPointFrDegree(180+dAngle) * dLen;

	// �簢 ����
	double rad	= ToRadian(dAngleApply);
	vLeft.z -= dLen * tan(rad);
	vRight.z += dLen * tan(rad); 

	double dA	= ~(vTop-vLeft);
	double dB	= ~(vTop-vRight);
	double dC	= ~(vLeft-vRight);


	dAngle	= ToDegree(acos(((dA*dA) + (dB*dB) - (dC*dC)) / (2 * dA * dB)));
	*/
	


	return dAngle;
}

// dAngleApply�� ������ ����� dAngle�� ������ �����ؼ� ����
double CWallData::GetAngleByVert(double dAngle, double dAngleApply)
{
	if(dAngleApply == 90) return dAngle;

	double dLen		= 100;
	CDPoint xyTop	= CDPoint(0, 0);
	CDPoint xyAngle	= xyTop + ToDPointFrDegree(dAngle) * dLen;
	
	CDPoint vAng	= ToDPointFrDegree(dAngleApply);
	xyAngle.x		= xyAngle.x * vAng.y;

	dAngle			= (xyAngle - xyTop).Unit().GetAngleDegree();

	return dAngle;
}

void CWallData::SaveCountJointRebar()
{
	if(m_nFlag <= 16)
	{
		// ���� ����
		long left = 0; for(left = 0; left < 2; left++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				m_nCountJointMainRebar_Cycle1[left][dan]	= m_nCountJointMainRebar_Cycle1_Old[left][dan];	// ������ü ������,�¿��� ���� ����
				m_nCountJointMainRebar_Cycle2[left][dan]	= m_nCountJointMainRebar_Cycle2_Old[left][dan];	// ������ü ������,�¿��� ���� ����
			}
		}
	}

}

CRebar* CWallData::GetRebarMainByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarMain.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarMain.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarSupportByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarSupport.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarSupport.GetAt(i);
		// ��ü�� ������ȣ�� ���� ������ find�� ����.
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarShearByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarShear.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarShear.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}

CRebar* CWallData::GetRebarColumnByDescription( CString sDescription )
{
	long i = 0; for(i = 0; i < m_pArrRebarColumn.GetSize(); i++)
	{
		CRebar *pRB	= m_pArrRebarColumn.GetAt(i);
		if(pRB->m_sDescription.Find(sDescription, 0) != -1)
		{
			return pRB;
		}
	}
	return NULL;
}
