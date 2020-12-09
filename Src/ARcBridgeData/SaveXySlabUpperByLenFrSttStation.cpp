// SaveXySlabUpperByLenFrSttStation.cpp: implementation of the CSaveXySlabUpperByLenFrSttStation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SaveXySlabUpperByLenFrSttStation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaveXySlabUpperByLenFrSttStation::CSaveXySlabUpperByLenFrSttStation()
{
	m_nLeft = -1;
	m_dDistFromStt = 0.;
	m_bVertDir = FALSE;
	m_nJongDan = -1;
}

CSaveXySlabUpperByLenFrSttStation::~CSaveXySlabUpperByLenFrSttStation()
{

}

void CSaveXySlabUpperByLenFrSttStation::SetValue(double dDistFromStt, long nLeft, BOOL bVertDir, long nJongDan, CDPoint xy)
{
	m_dDistFromStt	= dDistFromStt;
	m_nLeft	= nLeft;
	m_bVertDir	= bVertDir;
	m_nJongDan	= nJongDan;
	m_xy	= xy;
}
void CSaveXySlabUpperByLenFrSttStation::GetValue(CDPoint &xy)
{
	xy	= m_xy;
}

BOOL CSaveXySlabUpperByLenFrSttStation::IsMatch(double dDistFromStt, long nLeft, BOOL bVertDir, long nJongDan)
{
	if(m_dDistFromStt != dDistFromStt) return FALSE;
	if(m_nLeft != nLeft) return FALSE;
	if(m_bVertDir != bVertDir) return FALSE;
	if(m_nJongDan != nJongDan) return FALSE;

	return TRUE;
}
