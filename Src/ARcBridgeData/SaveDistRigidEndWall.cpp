// SaveDistRigidEndWall.cpp: implementation of the CSaveDistRigidEndWall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveDistRigidEndWall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveDistRigidEndWall::CSaveDistRigidEndWall()
{
	m_dRet = 0.;
	m_bUpper = FALSE;
	m_nJijum = -1;
}

CSaveDistRigidEndWall::~CSaveDistRigidEndWall()
{

}
BOOL CSaveDistRigidEndWall::IsMatch(long nJijum, BOOL bUpper)
{
	if(m_nJijum != nJijum) return FALSE;
	if(m_bUpper != bUpper) return FALSE;
	
	return TRUE;
}

void CSaveDistRigidEndWall::GetValue(double &dRet)
{
	dRet	= m_dRet;
}

void CSaveDistRigidEndWall::SetValue(long nJijum, double dRet, BOOL bUpper)
{
	m_nJijum	= nJijum;
	m_bUpper = bUpper;
	m_dRet		= dRet;
}
