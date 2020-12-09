#include "stdafx.h"
#include "ARcBridgeData.h"


CStmModelForArcbridge::CStmModelForArcbridge(void)
{
	InitData();
}


CStmModelForArcbridge::~CStmModelForArcbridge(void)
{
}

void CStmModelForArcbridge::InitData()
{
	m_nTypePosRebarTie	= 1;
	m_dUserPosRebarTie	= 0;
	m_nTypePosCompressStrut	= 4;
	m_dUserPosCompressStrut	= 0;

	m_nTypeLoadStressFlow = 0;
	m_nTypeStmModeling = 0;
	m_nTypeDistCornerRebar = 0;
	m_dUserDistCornerRebar = 0.4;
	m_nTypeDistHunchRebar = 0;
	m_dUserDistHunchRebar = 0.9;
	m_ixInnerAngle = 0;
	m_nTypeStirrupInnerTension = 1;
	m_dUserStirrupInnerTension = 0.1;
}

void CStmModelForArcbridge::Serialize( CArchive &ar )
{
	CStmModel::Serialize(ar);

	// 1 : m_nTypePosRebarTie 옵션 삭제
	long nFlag(1);
	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_nTypePosRebarTie;
		ar << m_dUserPosRebarTie;
		ar << m_nTypePosCompressStrut;
		ar << m_dUserPosCompressStrut;

		ar << m_nTypeLoadStressFlow;
		ar << m_nTypeStmModeling;
		ar << m_nTypeDistCornerRebar;
		ar << m_dUserDistCornerRebar;
		ar << m_nTypeDistHunchRebar;
		ar << m_dUserDistHunchRebar;
		ar << m_ixInnerAngle;
		ar << m_nTypeStirrupInnerTension;
		ar << m_dUserStirrupInnerTension;
	}
	else
	{
		ar >> nFlag;

		ar >> m_nTypePosRebarTie;
		ar >> m_dUserPosRebarTie;
		ar >> m_nTypePosCompressStrut;
		ar >> m_dUserPosCompressStrut;

		ar >> m_nTypeLoadStressFlow;
		ar >> m_nTypeStmModeling;
		ar >> m_nTypeDistCornerRebar;
		ar >> m_dUserDistCornerRebar;
		ar >> m_nTypeDistHunchRebar;
		ar >> m_dUserDistHunchRebar;
		ar >> m_ixInnerAngle;
		ar >> m_nTypeStirrupInnerTension;
		ar >> m_dUserStirrupInnerTension;
	}


	if(ar.IsLoading())
	{		
		if(nFlag < 1)
		{
			// AAB-6960
			if(m_nTypePosRebarTie > 1)
			{
				if(m_nTypePosRebarTie == 4) // Wt,max, 콘크리트타이 두께 옵션 삭제
					m_nTypePosRebarTie = 2; //사용자입력
				else 
					m_nTypePosRebarTie = 1; //권고안
			}
		}
	}
}
