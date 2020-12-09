// SaveCrossSection.cpp: implementation of the CSaveCrossSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveCrossSection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveCrossSection::CSaveCrossSection()
{
	m_bExp = FALSE;
	m_dSta = 0.;
	m_bRoadNotSlab = FALSE;
}

CSaveCrossSection::~CSaveCrossSection()
{

}

BOOL CSaveCrossSection::IsMatch(double dSta, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp)
{
	if(!Compare(m_dSta, dSta, "=")) return FALSE;
	if(!ComparePt(m_vAng, vAng, "=")) return FALSE;
	if(m_bRoadNotSlab != bRoadNotSlab) return FALSE;
	if(m_bExp != bExp) return FALSE;

	return TRUE;
}

void CSaveCrossSection::GetValue(CDPoint *A)
{
	long i = 0; for(i = 0; i < 30; i++)
		A[i]	= m_A[i];
}

void CSaveCrossSection::SetValue(double dSta, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp, CDPoint *A)
{
	m_dSta	= dSta;
	m_vAng	= vAng;
	m_bRoadNotSlab	= bRoadNotSlab;
	m_bExp	= bExp;

	long i = 0; for(i = 0; i < 30; i++)
		m_A[i]	= A[i];
}
