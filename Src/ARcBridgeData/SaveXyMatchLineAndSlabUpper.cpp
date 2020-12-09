// SaveXyMatchLineAndSlabUpper.cpp: implementation of the CSaveXyMatchLineAndSlabUpper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveXyMatchLineAndSlabUpper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveXyMatchLineAndSlabUpper::CSaveXyMatchLineAndSlabUpper()
{
	m_dOffset = 0.;
	m_bVertDir = FALSE;
	m_nJongDan = -1;
	m_nLeft = -1;
}

CSaveXyMatchLineAndSlabUpper::~CSaveXyMatchLineAndSlabUpper()
{

}
void CSaveXyMatchLineAndSlabUpper::SetValue(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir, long nJongDan, CDPoint xyRet)
{
	m_xy	= xy;
	m_xyDir	= xyDir;
	m_nLeft	= nLeft;
	m_dOffset	= dOffset;
	m_bVertDir	= bVertDir;
	m_nJongDan	= nJongDan;
	m_xyRet	= xyRet;
}
void CSaveXyMatchLineAndSlabUpper::GetValue(CDPoint &xy)
{
	xy	= m_xyRet;
}

BOOL CSaveXyMatchLineAndSlabUpper::IsMatch(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir, long nJongDan)
{
	if(m_xy != xy) return FALSE;
	if(m_xyDir != xyDir) return FALSE;
	if(m_nLeft != nLeft) return FALSE;
	if(m_dOffset != dOffset) return FALSE;
	if(m_bVertDir != bVertDir) return FALSE;
	if(m_nJongDan != nJongDan) return FALSE;

	return TRUE;
}
