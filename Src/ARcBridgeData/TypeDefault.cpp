// TypeDefault.cpp: implementation of the CTypeDefault class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TypeDefault.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTypeDefault::CTypeDefault()
{
	m_nFlag			= 0;
	m_nIDDialog		= -1;				// 입력창 id
	m_dwDefaulted	= 0x00000000;		// 권고안이 되어 있는 상태
}

CTypeDefault::~CTypeDefault()
{

}

void CTypeDefault::Serialize(CArchive &ar)
{
	m_nFlag = 0;

	if(ar.IsStoring())
	{
		ar << m_nFlag;
		ar << m_nIDDialog;			// 입력창 id
		ar << m_dwDefaulted;		// 권고안이 되어 있는 상태
	}
	else
	{
		ar >> m_nFlag;
		ar >> m_nIDDialog;			// 입력창 id
		ar >> m_dwDefaulted;		// 권고안이 되어 있는 상태
	}
}

// dwtype의 권고안이 되어 있는지?
BOOL CTypeDefault::IsDefaulted(DWORD dwType)
{
	return m_dwDefaulted & dwType;
}


CTypeDefault& CTypeDefault::operator=(const CTypeDefault& Obj)
{
	m_nFlag			= Obj.m_nFlag;
	m_nIDDialog		= Obj.m_nIDDialog;		// 입력창 id
	m_dwDefaulted	= Obj.m_dwDefaulted;		// 권고안이 되어 있는 상태

	return *this;
}