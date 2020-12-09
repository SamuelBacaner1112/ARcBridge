// ExpJointRef.cpp: implementation of the CExpJointRef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExpJointRef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExpJointRef::CExpJointRef()
{

}

CExpJointRef::~CExpJointRef()
{

}


CExpJointRef& CExpJointRef::operator=(const CExpJointRef& Obj)
{
	m_szName = Obj.m_szName;
	m_dMaxAllowExpL = Obj.m_dMaxAllowExpL;
	m_dBlockW  = Obj.m_dBlockW;
	m_dBlockH  = Obj.m_dBlockH;
	m_dUganMin = Obj.m_dUganMin;
	m_dUganMax = Obj.m_dUganMax;

	return *this;
}


void CExpJointRef::Serialize(CArchive& ar)
{
//	long nFlag = 0;
	long nFlag = 1;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_szName;
		ar << m_dMaxAllowExpL;
		ar << m_dBlockW;
		ar << m_dBlockH;
		ar << m_dUganMin;
		ar << m_dUganMax;
	}
	else
	{
		double dTemp;
		ar >> nFlag;
		ar >> m_szName;
		ar >> m_dMaxAllowExpL;
		ar >> m_dBlockW;
		ar >> m_dBlockH;
		ar >> m_dUganMin;
		if(nFlag<1)
			ar >> dTemp;
//		ar >> m_dUganAve;
		ar >> m_dUganMax;
		if(nFlag<1)
		{
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
		}
// 		ar >> m_dQuantityConcPerM2;
// 		ar >> m_dQuantityFormPerM2;
// 		ar >> m_dQuantityD13PerM2;
// 		ar >> m_dQuantityD16PerM2;
	}
}


//////////////////////////////////////////////////////////////////////////

CExpJointRefArr::CExpJointRefArr()
{
	SetRefDefault();
}

CExpJointRefArr::~CExpJointRefArr()
{
	AhTPADelete(&m_arrExpJointRef,(CExpJointRef*)0);
}


void CExpJointRefArr::Serialize(CArchive& ar)
{
// 	if(ar.IsStoring()) 
// 	{
		AhTPASerial(ar, &m_arrExpJointRef, (CExpJointRef*)0);
// 	}
// 	else 
// 	{
// 		AhTPASerial(ar, &m_arrExpJointRef, (CExpJointRef*)0);
// 	}
}

void CExpJointRefArr::SetRefDefault()
{
	AhTPADelete(&m_arrExpJointRef,(CExpJointRef*)0);
	
	for(long nRow=0; nRow<22*6; nRow+=6)
	{
		CExpJointRef	*pObj	= new CExpJointRef;

		pObj->m_szName			= ExpJointRef[nRow];
		pObj->m_dMaxAllowExpL	= atof(ExpJointRef[nRow+1]);
		pObj->m_dBlockW			= atof(ExpJointRef[nRow+2]);
		pObj->m_dBlockH			= atof(ExpJointRef[nRow+3]);
		pObj->m_dUganMin		= atof(ExpJointRef[nRow+4]);
		pObj->m_dUganMax		= atof(ExpJointRef[nRow+5]);

		m_arrExpJointRef.Add(pObj);
	}
}

CExpJointRef* CExpJointRefArr::GetExpJointRef(CString szName)
{
	for(long n=0; n < m_arrExpJointRef.GetSize(); n++)
	{
		CExpJointRef *pObj = m_arrExpJointRef.GetAt(n);
		
		if(pObj->m_szName == szName)	return pObj;
	}

	return NULL;
}

// long CExpJointRefArr::GetIndexByExpJointName(CString szName)
// {
// 	for(long nIndex=0; nIndex < m_arrExpJointRef.GetSize(); nIndex++)
// 	{
// 		CExpJointRef *pObj = m_arrExpJointRef.GetAt(nIndex);
// 		if(pObj->m_szName == szName) return nIndex;
// 	}
// 
// 	return 0;
// }

