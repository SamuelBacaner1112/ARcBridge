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
	m_nIDDialog		= -1;				// �Է�â id
	m_dwDefaulted	= 0x00000000;		// �ǰ���� �Ǿ� �ִ� ����
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
		ar << m_nIDDialog;			// �Է�â id
		ar << m_dwDefaulted;		// �ǰ���� �Ǿ� �ִ� ����
	}
	else
	{
		ar >> m_nFlag;
		ar >> m_nIDDialog;			// �Է�â id
		ar >> m_dwDefaulted;		// �ǰ���� �Ǿ� �ִ� ����
	}
}

// dwtype�� �ǰ���� �Ǿ� �ִ���?
BOOL CTypeDefault::IsDefaulted(DWORD dwType)
{
	return m_dwDefaulted & dwType;
}


CTypeDefault& CTypeDefault::operator=(const CTypeDefault& Obj)
{
	m_nFlag			= Obj.m_nFlag;
	m_nIDDialog		= Obj.m_nIDDialog;		// �Է�â id
	m_dwDefaulted	= Obj.m_dwDefaulted;		// �ǰ���� �Ǿ� �ִ� ����

	return *this;
}