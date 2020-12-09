// FlexibleJoint.cpp: implementation of the CFlexibleJoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlexibleJoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlexibleJoint::CFlexibleJoint()
{
	m_nFlag	= 0;

	// 단면 제원
	m_dWidth	= 0;
	m_dHeight	= 0;

	// 수량 산출
	m_sName		= _T("");
	m_dLenMargin	= 0;
	m_nCountMargin	= 0;
	m_rb[0].InitData();
	m_rb[1].InitData();

	// 철근
	long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_dLenHorRebar[cycle][dan]	= 0;
		}
	}
}

CFlexibleJoint::~CFlexibleJoint()
{

}

void CFlexibleJoint::Serialize(CArchive &ar)
{
	m_nFlag	= 0;

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		// 단면 제원
		ar << m_dWidth;
		ar << m_dHeight;

		// 수량 산출
		ar << m_sName;
		ar << m_dLenMargin;
		ar << m_nCountMargin;
		m_rb[0].Serialize(ar);
		m_rb[1].Serialize(ar);

		// 철근
		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				ar << m_dLenHorRebar[cycle][dan];
			}
		}
	}
	else
	{
		ar >> m_nFlag;

		// 단면 제원
		ar >> m_dWidth;
		ar >> m_dHeight;

		// 수량 산출
		ar >> m_sName;
		ar >> m_dLenMargin;
		ar >> m_nCountMargin;
		m_rb[0].Serialize(ar);
		m_rb[1].Serialize(ar);

		// 철근
		long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				ar >> m_dLenHorRebar[cycle][dan];
			}
		}
	}
}

CFlexibleJoint& CFlexibleJoint::operator=(const CFlexibleJoint& Obj)
{
	// 단면 제원
	m_dWidth		= Obj.m_dWidth;
	m_dHeight		= Obj.m_dHeight;

	// 수량 산출
	m_sName			= Obj.m_sName;
	m_dLenMargin	= Obj.m_dLenMargin;
	m_nCountMargin	= Obj.m_nCountMargin;
	m_rb[0]			= Obj.m_rb[0];
	m_rb[1]			= Obj.m_rb[1];

	return *this;
}