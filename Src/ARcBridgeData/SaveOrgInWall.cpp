// SaveOrgInWall.cpp: implementation of the CSaveOrgInWall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveOrgInWall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveOrgInWall::CSaveOrgInWall()
{
	m_bVertDir = FALSE;
	m_nIdxWall = -1;
	m_nLeft = -1;
}

CSaveOrgInWall::~CSaveOrgInWall()
{

}

BOOL CSaveOrgInWall::IsMatch(long nIdxWall, long nLeft, BOOL bVertDir)
{
	if(m_nIdxWall != nIdxWall) return FALSE;
	if(m_nLeft != nLeft) return FALSE;
	if(m_bVertDir != bVertDir) return FALSE;

	return TRUE;
}

void CSaveOrgInWall::SetValue(long nIdxWall, long nLeft, BOOL bVertDir, CDPoint xyOrg)
{
	m_nIdxWall	= nIdxWall;
	m_nLeft	= nLeft;
	m_bVertDir	= bVertDir;
	m_xyOrg	= xyOrg;
}

void CSaveOrgInWall::GetValue(CDPoint &xyOrg)
{
	xyOrg	= m_xyOrg;
}
