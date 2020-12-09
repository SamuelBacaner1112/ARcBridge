// Footway.cpp: implementation of the CFootway class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Footway.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFootway::CFootway()
{
}

CFootway::~CFootway()
{

}

void CFootway::Serialize(CArchive& ar)
{
	long nFlag = 2;
	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_nNumberHDan;
		ar << m_bRailRoad;
		ar << m_bLeftGuardFence;
		ar << m_nTypePave;
		ar << m_nTypeInnerFill;
		ar << m_dUnitWeightPave;
		ar << m_dUnitWeightInnerFill;
		ar << m_dH1;
		ar << m_dThickPaveFootway;
		ar << m_dSlopeUpper;
	}
	else
	{
		ar >> nFlag;

		if(nFlag >= 2)
		{
			ar >> m_nNumberHDan;
			ar >> m_bRailRoad;
			ar >> m_bLeftGuardFence;
		}
		ar >> m_nTypePave;
		ar >> m_nTypeInnerFill;
		ar >> m_dUnitWeightPave;
		ar >> m_dUnitWeightInnerFill;
		ar >> m_dH1;
		ar >> m_dThickPaveFootway;
		ar >> m_dSlopeUpper;
	}
}

CFootway& CFootway::operator=(const CFootway& Obj)
{
	return *this;
}


