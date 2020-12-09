// SaveDistRigidEndSlab.cpp: implementation of the CSaveDistRigidEndSlab class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveDistRigidEndSlab.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveDistRigidEndSlab::CSaveDistRigidEndSlab()
{
	m_dRet = 0.;
	m_bSttPos = FALSE;
	m_nJijum = -1;
	m_bUpperSlab = TRUE;
}

CSaveDistRigidEndSlab::~CSaveDistRigidEndSlab()
{

}
BOOL CSaveDistRigidEndSlab::IsMatch(long nJijum, BOOL bSttPos, BOOL bUpperSlab)
{
	if(m_nJijum != nJijum) return FALSE;
	if(m_bSttPos != bSttPos) return FALSE;
	if(m_bUpperSlab != bUpperSlab) return FALSE;
	
	return TRUE;
}

void CSaveDistRigidEndSlab::GetValue(double &dRet)
{
	dRet	= m_dRet;
}

void CSaveDistRigidEndSlab::SetValue(long nJijum, BOOL bSttPos, BOOL bUpperSlab, double dRet)
{
	m_nJijum	= nJijum;
	m_bSttPos	= bSttPos;
	m_bUpperSlab = bUpperSlab;
	m_dRet		= dRet;
}
